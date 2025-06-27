#include "mainwindow.h"
#include "ui_mainwindow.h"

bool add=false;
bool sub=false;
bool divv=false;
bool mul=false;
float oldval=0.0f;
float newval=0.0f;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_b0_clicked()
{
    QString val=ui->displaylabel->text();
    if(val=="0"){
        ui->displaylabel->setText("0");
    }
    else{
        ui->displaylabel->setText(val+"0");
    }
}


void MainWindow::on_b1_clicked()
{
    QString val=ui->displaylabel->text();
    if(val=="0"){
        ui->displaylabel->setText("1");
    }
    else{
        ui->displaylabel->setText(val+"1");
    }
}


void MainWindow::on_b2_clicked()
{
    QString val=ui->displaylabel->text();
    if(val=="0"){
        ui->displaylabel->setText("2");
    }
    else{
        ui->displaylabel->setText(val+"2");
    }

}


void MainWindow::on_b3_clicked()
{
    QString val=ui->displaylabel->text();
    if(val=="0"){
        ui->displaylabel->setText("3");
    }
    else{
        ui->displaylabel->setText(val+"3");
    }
}




void MainWindow::on_b4_clicked()
{
    QString val=ui->displaylabel->text();
    if(val=="0"){
        ui->displaylabel->setText("4");
    }
    else{
        ui->displaylabel->setText(val+"4");
    }
}


void MainWindow::on_b5_clicked()
{
    QString val=ui->displaylabel->text();
    if(val=="0"){
        ui->displaylabel->setText("5");
    }
    else{
        ui->displaylabel->setText(val+"5");
    }
}


void MainWindow::on_b6_clicked()
{
    QString val=ui->displaylabel->text();
    if(val=="0"){
        ui->displaylabel->setText("6");
    }
    else{
        ui->displaylabel->setText(val+"6");
    }
}



void MainWindow::on_b7_clicked()
{
    QString val=ui->displaylabel->text();
    if(val=="0"){
        ui->displaylabel->setText("7");
    }
    else{
        ui->displaylabel->setText(val+"7");
    }
}


void MainWindow::on_b8_clicked()
{
    QString val=ui->displaylabel->text();
    if(val=="0"){
        ui->displaylabel->setText("8");
    }
    else{
        ui->displaylabel->setText(val+"8");
    }
}


void MainWindow::on_b9_clicked()
{
    QString val=ui->displaylabel->text();
    if(val=="0"){
        ui->displaylabel->setText("9");
    }
    else{
        ui->displaylabel->setText(val+"9");
    }
}
void MainWindow::on_bminus_clicked()
{
    oldval=ui->displaylabel->text().toFloat();
    add=false;
    sub=true;
    mul=false;
    divv=false;
    ui->displaylabel->setText("0");
}


void MainWindow::on_bdiv_clicked()
{
    oldval=ui->displaylabel->text().toFloat();
    add=false;
    sub=false;
    mul=false;
    divv=true;
    ui->displaylabel->setText("0");
}
void MainWindow::on_bmul_clicked()
{
    oldval=ui->displaylabel->text().toFloat();
    add=false;
    sub=false;
    mul=true;
    divv=false;
    ui->displaylabel->setText("0");
}
void MainWindow::on_bplus_clicked()
{
    oldval=ui->displaylabel->text().toFloat();
    add=true;
    sub=false;
    mul=false;
    divv=false;
    ui->displaylabel->setText("0");
}

void MainWindow::on_bpoint_clicked()
{
    QString val=ui->displaylabel->text();
    if(!val.contains('.')){
        ui->displaylabel->setText(val + ".");
    }
}

void MainWindow::on_bequal_clicked()
{
    newval=ui->displaylabel->text().toFloat();
    float result=0.0f;

    if(add==true){
        result=oldval+newval;
    }
    else if(sub==true){
        result=oldval-newval;
    }
    else if(mul==true){
        result=oldval*newval;
    }
    else if(divv==true){
        if(newval==0.0f){
            ui->displaylabel->setText("Error");
            return;
        }
        else{
            result=oldval/newval;
        }
    }
    else{
        ui->displaylabel->setText(ui->displaylabel->text());
    }
    ui->displaylabel->setText(QString::number((result)));
    add=false;
    sub=false;
    divv=false;
    mul=false;

}



void MainWindow::on_bdel_clicked()
{
    QString val=ui->displaylabel->text();
    if(val.length()>1){
        ui->displaylabel->setText(val.left(val.length()-1));
    }
    else{
        ui->displaylabel->setText("0");
    }

}


void MainWindow::on_bclear_clicked()
{
    ui->displaylabel->setText("0");
    add=false;
    sub=false;
    divv=false;
    mul=false;
    oldval=0.0f;
    newval=0.0f;

}
