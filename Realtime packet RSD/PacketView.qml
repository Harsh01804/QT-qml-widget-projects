import QtQuick 6.2
import QtQuick.Layouts 1.3

Rectangle {
    width: parent?.width ?? 800
    height: implicitHeight
    color: ListView.isCurrentItem ? "#e0f7fa" : "#ffffff"
    border.color: "#888"
    border.width: 1
    radius: 4

    property string timestamp
    property string hex
    property string checksum

    ColumnLayout {
        anchors.fill: parent
        spacing: 4

        RowLayout {
            spacing: 10
            Text {
                text: timestamp
                font.bold: true
                font.pixelSize: 12
                Layout.alignment: Qt.AlignVCenter
            }
            Text {
                text: "Chk:" + checksum
                font.pixelSize: 12
                Layout.alignment: Qt.AlignVCenter
            }
        }

        Text {
            text: hex
            font.family: "monospace"
            font.pixelSize: 10
            elide: Text.ElideRight
            wrapMode: Text.NoWrap
            Layout.fillWidth: true
        }
    }
}
