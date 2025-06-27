import QtQuick            6.2
import QtQuick.Controls   6.2
import QtQuick.Layouts    1.3

ApplicationWindow {
    id: root
    width: 1000
    height: 700
    visible: true
    title: "Telback Monitor"
    color: "#000000"

    property int    selectedTab: 0
    property string liveLog: ""
    property bool   replayRunning: false

    /* ─────────── LIVE TAB ─────────── */
    Component {
        id: liveTab
        ColumnLayout {
            anchors.fill: parent
            spacing: 10
            anchors.margins: 10

            RowLayout {
                spacing: 10
                Button { text: "Start Receiver"; enabled: !receiver.isRunning; onClicked: receiver.startReceiving() }
                Button { text: "Stop Receiver";  enabled:  receiver.isRunning; onClicked: receiver.stopReceiving() }
                Button {
                    text: "Export CSV"
                    onClicked: {
                        if (controller.exportCsvAndLog())
                            liveStatus.text = "Exported to " + controller.defaultCsv()
                        else
                            liveStatus.text = "Export failed"
                    }
                }
                Button { text: "Display"; onClicked: liveOutput.text = liveLog }
                Label  { id: liveStatus; color: "lightgray" }
            }

            TextArea {
                id: liveOutput
                readOnly: true; wrapMode: Text.NoWrap
                font.family: "monospace"; color: "white"
                background: Rectangle { color: "#111" }
                Layout.fillWidth: true; Layout.fillHeight: true
            }
        }
    }

    /* ─────────── REPLAY TAB ─────────── */
    Component {
        id: replayTab
        ColumnLayout {
            anchors.fill: parent
            spacing: 10
            anchors.margins: 10

            property var replayLines: []
            property int replayIdx:   0

            Timer {
                id: replayTimer; interval: 200; repeat: true; running: false
                onTriggered: {
                    if (replayIdx < replayLines.length) {
                        replayOutput.text += replayLines[replayIdx] + "\n"
                        replayIdx++
                    } else {
                        running = false; replayRunning = false
                        replayStatus.text = "Replay finished."
                    }
                }
            }

            RowLayout {
                spacing: 10
                Button {
                    text: replayRunning ? "Replaying…" : "Start Replay"
                    enabled: !replayRunning
                    onClicked: {
                        const csv = controller.readCsv(runCsv)
                        if (csv.startsWith("[ERROR]")) { replayStatus.text = csv; return }
                        replayLines = csv.split("\n").filter(x=>x.trim()!== "").slice(1)
                        if (replayLines.length===0) { replayStatus.text="CSV empty – export first."; return }
                        replayIdx = 0; replayOutput.text=""; replayRunning=true; replayTimer.start()
                        replayStatus.text="Replaying "+replayLines.length+" packet(s)…"
                    }
                }
                Button {
                    text: "Stop Replay"
                    enabled: replayRunning
                    onClicked: { replayTimer.stop(); replayRunning=false; replayStatus.text="[REPLAY STOPPED]" }
                }
                Label { id: replayStatus; color: "lightgray" }
            }

            TextArea {
                id: replayOutput
                readOnly: true; wrapMode: Text.NoWrap
                font.family: "monospace"; color: "white"
                background: Rectangle { color: "#111" }
                Layout.fillWidth: true; Layout.fillHeight: true
                text: "Press “Start Replay” to stream packets."
            }
        }
    }



    /* ─────────── TOP BAR & TAB SWITCHER ─────────── */
    ColumnLayout {
        anchors.fill: parent; spacing: 0

        Rectangle {
            height: 50; width: parent.width
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#222" }
                GradientStop { position: 0.5; color: "#444" }
                GradientStop { position: 1.0; color: "#222" }
            }

            RowLayout {
                anchors.fill: parent; spacing: 10; anchors.margins: 10
                Button { text:"Live";      checkable:true; checked:selectedTab===0; onClicked:selectedTab=0 }
                Button { text:"Replay";    checkable:true; checked:selectedTab===1; onClicked:selectedTab=1 }
                Button { text:"Dashboard"; checkable:true; checked:selectedTab===2; onClicked:selectedTab=2 }
                Label  { text:"CSV: "+runCsv; color:"#aaa"; Layout.alignment: Qt.AlignVCenter }
            }
        }

        Loader {
            Layout.fillWidth: true; Layout.fillHeight: true
            sourceComponent: selectedTab===0 ? liveTab
                           : selectedTab===1 ? replayTab
                           : dashboardTab                   /* new */
        }
    }

    /* ───────── live packets into liveLog ───────── */
    Connections {
        target: receiver
        onPacketReceived: function(packet, ts) {
            const v = new Uint8Array(packet)
            const hex = Array.prototype.map.call(v,b=>('0'+b.toString(16)).slice(-2).toUpperCase()).join(" ")
            liveLog += `[RECEIVED] ${ts.toString()} | ${hex}\n`
        }
    }
}
