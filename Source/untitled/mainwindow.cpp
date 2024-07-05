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

QString code_table;
QString item_name;
int addressQty;

int address,length,RL;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionOpen, &QAction::triggered,this,&MainWindow::open );
    connect(ui->actionExit, &QAction::triggered,this,&MainWindow::exit );
    connect(ui->actionAbout,&QAction::triggered,this,&MainWindow::about);

    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("E:\\da\\SQLITE EXPERT v1.1.2\\MAIN.db3");
    db.open();

}

MainWindow::~MainWindow()
{
    F_main.close();
    delete ui;
}

/*
 * This function opens a window to select the file
 * in order to extract the information and codes.
*/
void MainWindow::open()
{
    fileName=QFileDialog::getOpenFileName(this,"Select BIN or HEX file",
    "C:/Users/Win-10/Documents/QT","Binary(*.bin);;All(*);;Hex(*.hex)",NULL);
    ts.setDevice(&F_main);
    F_main.setFileName(fileName);
    ui->pl_Address->setText(fileName);
    F_main.open(QFile::ReadWrite );
}

/*
 * This function is triggerd by "File => Exit"
 * It closes the software
*/
void MainWindow::exit()
{
    if(F_main.isOpen())
    F_main.close();
    close();
}

/*s
 * This function is triggerd by "Help => About"
 * It gives some information about the software
*/
void MainWindow::about()
{

}

/*
 * This function reads the combobox's item and
 * store it to select the appropriate record
 * from the main table in order to extract the
 * data from the item's table.
*/
void MainWindow::fileFeature()
{
    QSqlQuery ff;
    item_name= ui->comSel->currentText();
    ff.exec("SELECT \"codeTable\", \"addressQty\" FROM Main WHERE Name = '"+item_name+"'");
    ff.next();
    code_table=ff.value(0).toString();
    addressQty =ff.value(1).toInt();
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

    QString itos;
    for(uint8_t j=0; j<addressQty; j++){

        itos.setNum(j);
        q.exec("SELECT length, address, RL FROM \""+item_name+"\" WHERE \"index\"="+itos+" ");
        q.next();
        length= q.value(0).toInt();
        address= q.value(1).toInt();
        RL= q.value(2).toInt();


        for (int i=0; i<length;i++){
            /*
             * This piece of code defines the direction of
             * every address' movement.
             *
             * Movement to Right is 1
             * Movement to Left is 0
            */
            if(RL == 1){
                F_main.seek(address+i);
            }else if(RL == 0){
                F_main.seek(address-i);
            }

            s=F_main.read(1).toHex();
            q.exec("SELECT eqCode FROM "+code_table+" WHERE Hex=x'"+s+"'");
            q.next();
            s_res+= q.value(0).toString();
            ui->pl_res->setText(s_res);
        }
    }

}

