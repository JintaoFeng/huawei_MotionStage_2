#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QMenu>
#include <QInputDialog>
#include <QString>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MotionStage");

    QImage image("../googolMotion/Resources/UPLogo.jpg");
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    ui->imageLabel->setGeometry(0,0,image.width(),image.height());

    QTimer *timer=new QTimer;
    connect(timer,&QTimer::timeout,this,&MainWindow::ShowTime);
    ui->timeNumber->setDigitCount(8);
    ui->timeNumber->display(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->dataNumber->setDigitCount(10);
    ui->dataNumber->display(QDateTime::currentDateTime().toString("yyyy.MM.dd"));
    timer->start(1000);
    ui->tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowTime()
{
    ui->timeNumber->display(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->dataNumber->display(QDateTime::currentDateTime().toString("yyyy.MM.dd"));
}



void MainWindow::on_tabWidget_tabBarClicked(int index)
{
//    if(ui->tabWi
//    ui->tabWidget->addTab(new QWidget,"st");
//    ui->tabWidget->currentIndex();
//    ui->tabWidget->widget
//    qDebug()<<index<<endl;
}

void MainWindow::on_tabWidget_customContextMenuRequested(const QPoint &pos)
{
    qDebug()<<"123"<<endl;
    Q_UNUSED(pos)
    QMenu* menu=new QMenu;
    menu->addAction("添加产品信号",this,&MainWindow::AddTab);
    menu->addAction("删除产品型号",this,&MainWindow::DeleteTab);
    menu->addSeparator();
    menu->addAction("添加点位");
    menu->addAction("删除点位");
    menu->exec(QCursor::pos());
    delete  menu;
}
void MainWindow::AddTab()
{
    QString txtTitle="请输入产品名称:";
    QString title="INPUT";
    QString text = QInputDialog::getText(this,title,txtTitle,QLineEdit::Normal,"产品");
    ui->tabWidget->addTab(new QWidget(this),text);
}
void MainWindow::DeleteTab()
{
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex()) ;
}
