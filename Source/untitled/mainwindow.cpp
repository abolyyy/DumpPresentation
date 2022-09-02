#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFile"
#include "QDir"
#include "QFileDialog"
#include "QIODevice"
#include "QTextStream"
#include "QDataStream"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QMessageBox>

QFile F_main;
QDataStream ts;
QSqlDatabase db;
int Address;
int Length;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionOpen, &QAction::triggered,this,&MainWindow::open );
    connect(ui->actionExit, &QAction::triggered,this,&MainWindow::exit );
    connect(ui->actionAbout,&QAction::triggered,this,&MainWindow::about);

    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Program Files\\DumpPresentation\\DBFile\\file.db3");
    db.open();

}

MainWindow::~MainWindow()
{
    F_main.close();
    delete ui;
}

void MainWindow::open()
{
    fileName=QFileDialog::getOpenFileName(this,"Select BIN or HEX file",
    "C:/Users/Win-10/Documents/QT","Binary(*.bin);;All(*);;Hex(*.hex)",NULL);
    ts.setDevice(&F_main);
    F_main.setFileName(fileName);
    ui->pl_Address->setText(fileName);
    F_main.open(QFile::ReadWrite );

}

void MainWindow::exit()
{
    if(F_main.isOpen())
    F_main.close();
    close();
}
void MainWindow::about()
{

}

void MainWindow::fileFeature()
{
    QSqlQuery ff;
    QString comName= ui->comSel->currentText();
    ff.exec("SELECT Address,Length FROM Main WHERE Name = '"+comName+"'");
    ff.next();
    Address=ff.value(0).toInt();
    Length =ff.value(1).toInt();
}

void MainWindow::on_pushButton_clicked()
{
    if(!F_main.isOpen()){
        QMessageBox::warning(ui->label,"Faild","Please Open a file first. \n GUIDE: File -> Open ->select your .bin file " );
        return;
    }

    QString s="1b"; //Select and Add Variable 
    QString s_res;  //final result (string)
    QSqlQuery q;    //SQL Query
    fileFeature();

    for (int i=0; i<Length;i++){
        F_main.seek(Address+i);
        s=F_main.read(1).toHex();
        q.exec("SELECT eqCode FROM Codec WHERE Hex=x'"+s+"'");
        q.next();
        s_res+= q.value(0).toString();
        ui->pl_res->setText(s_res);

    }

}

