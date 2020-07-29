#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QMenu>
#include <QInputDialog>
#include <QString>
#include <QVBoxLayout>
#include "table.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MotionStage");

    QImage image("Resources/UPLogo.jpg");
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    ui->imageLabel->setGeometry(0,0,image.width(),image.height());
       QFont font;
       font.setPointSize(14);
    ui->textEdit->setFont(font);
 //   this->s

    QTimer *timer=new QTimer;
    connect(timer,&QTimer::timeout,this,&MainWindow::ShowTime);
    ui->timeNumber->setDigitCount(8);
    ui->timeNumber->display(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->dataNumber->setDigitCount(10);
    ui->dataNumber->display(QDateTime::currentDateTime().toString("yyyy.MM.dd"));
    timer->start(1000);
    ui->tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    table *tab=new table(this);

    tab->setAttribute(Qt::WA_DeleteOnClose);
    int cur=ui->tabWidget->addTab(tab,"产品");
    tabWid.insert(cur,tab);
    ui->tabWidget->setCurrentIndex(cur);

    axi1=new axis(this,"Axis1");
    axi2=new axis(this,"Axis2");
    axi3=new axis(this,"Axis3");

    ui->horizontalLayout_6->addWidget(axi1,0);
    ui->horizontalLayout_6->addWidget(axi2,0);
    ui->horizontalLayout_6->addWidget(axi3,0);

//    axi1->show();
//    axi2->show();
//    axi3->show();


    //axis中的返回值处理链接到mainwindow上
    connect(axi1,&axis::commandHandle,this,&MainWindow::CommandHandler,Qt::DirectConnection);
    connect(axi2,&axis::commandHandle,this,&MainWindow::CommandHandler,Qt::DirectConnection);
    connect(axi3,&axis::commandHandle,this,&MainWindow::CommandHandler,Qt::DirectConnection);

    connect(this,&MainWindow::updateStart,axi1,&axis::updateStart);
    connect(this,&MainWindow::updateStart,axi2,&axis::updateStart);
    connect(this,&MainWindow::updateStart,axi3,&axis::updateStart);

    connect(this,&MainWindow::updateTerm,axi1,&axis::updateTerm);
    connect(this,&MainWindow::updateTerm,axi2,&axis::updateTerm);
    connect(this,&MainWindow::updateTerm,axi3,&axis::updateTerm);

    connect(this,&MainWindow::move,axi1,&axis::Move);
    connect(this,&MainWindow::move,axi2,&axis::Move);
    connect(this,&MainWindow::move,axi3,&axis::Move);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowTime()
{
    ui->timeNumber->display(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->dataNumber->display(QDateTime::currentDateTime().toString("yyyy.MM.dd"));

    GT_GetSts(1,&axisStatus1);
    GT_GetSts(2,&axisStatus2);
    GT_GetSts(3,&axisStatus3);
    if(axisStatus1&0x200 && axisStatus2&0x200 && axisStatus3&0x200)
    {
        ui->enableAllBtn->setText("Disable");
    }
    else
    {
        ui->enableAllBtn->setText("Enable");
    }
}

void MainWindow::on_tabWidget_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    QMenu* menu=new QMenu;
    menu->addAction("添加产品型号",this,&MainWindow::AddTab);
    menu->addAction("删除产品型号",this,&MainWindow::DeleteTab);
    menu->addSeparator();
    menu->addAction("添加点位",this,&MainWindow::AddPoint);
    menu->addAction("删除点位",this,&MainWindow::DeletePoint);
    menu->exec(QCursor::pos());
    delete  menu;
}
void MainWindow::AddTab()
{
    QString txtTitle="请输入产品名称:";
    QString title="INPUT";
    bool ok=false;
    QString text = QInputDialog::getText(this,title,txtTitle,QLineEdit::Normal,"产品",&ok,Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    table *tab=new table(this);
    tab->setAttribute(Qt::WA_DeleteOnClose);
    int cur=ui->tabWidget->addTab(tab,text);
    tabWid.insert(cur,tab);
 //   qDebug()<<cur<<endl;
    ui->tabWidget->setCurrentIndex(cur);

}
void MainWindow::DeleteTab()
{
    int cur=ui->tabWidget->currentIndex();
    tabWid.removeAt(cur);
    ui->tabWidget->removeTab(cur) ;
}

void MainWindow::AddPoint()
{
    int a=ui->tabWidget->currentIndex();
    tabWid.at(a)->AddPoint();
}
void MainWindow::DeletePoint()
{
    int a=ui->tabWidget->currentIndex();
    tabWid.at(a)->DeletePoint();
}
void MainWindow::on_connectBtn_clicked()
{
    int retValue;
    retValue=GT_Open();
    CommandHandler("open",retValue);
    if(~retValue)
    {
        emit updateStart();
    }
    QString str= "GTS800.cfg";
    QByteArray temp=str.toLatin1();
    char* file=temp.data();
    retValue=GT_LoadConfig(file);
    CommandHandler("load config",retValue);
    GT_ClrSts(1);
    GT_ClrSts(2);
    GT_ClrSts(3);
}

void MainWindow::CommandHandler(QString command, int value)
{
    QString str;
    if(value)
    {
        str=QString("%1  failed!!  %2").arg(command).arg(value);

   //     qDebug()<<str<<endl;
    }
    else
    {
        str=QString("%1  successful!!  %2").arg(command).arg(value);
    //    qDebug()<<str<<endl;
    }
    ui->textEdit->append(str);
}

void MainWindow::on_enableAllBtn_clicked()
{
    GT_GetSts(1,&axisStatus1);
    GT_GetSts(2,&axisStatus2);
    GT_GetSts(3,&axisStatus3);
    if(axisStatus1&0x200 && axisStatus2&0x200 && axisStatus3&0x200)
    {
        retValue=GT_AxisOff(1);
        CommandHandler("Axis1 Enable off",retValue);
        retValue=GT_AxisOff(2);
        CommandHandler("Axis2 Enable off",retValue);
        retValue=GT_AxisOff(3);
        if(!retValue)
        {
            ui->enableAllBtn ->setText("Enable");
        }
        CommandHandler("Axis3 Enable off",retValue);
    }
    else
    {
        retValue=GT_AxisOn(1);
        CommandHandler("Axis1 Enable on",retValue);
        retValue=GT_AxisOn(2);
        CommandHandler("Axis2 Enable on",retValue);
        retValue=GT_AxisOn(3);
        CommandHandler("Axis3 Enable on",retValue);
        if(!retValue)
        {
            ui->enableAllBtn->setText("Disable");
        }
    }
}

void MainWindow::on_killStopBtn_clicked()
{
    emit axis1->moveStop();
    emit axis2->moveStop();
    emit axis3->moveStop();
    GT_Stop(0Xf,0XF);
}

void MainWindow::on_closeBtn_clicked()
{
    retValue=GT_Close();
    emit updateTerm();
    CommandHandler("close",retValue);
}

void MainWindow::on_setBtn_clicked()
{
    QVector<double> pos;
    double prfPos;
    GT_GetPrfPos(1,&prfPos);
    pos<<prfPos;
    GT_GetPrfPos(2,&prfPos);
    pos<<prfPos;
    GT_GetPrfPos(3,&prfPos);
    pos<<prfPos;
  //  pos.clear();
  //  pos<<1200<<23000<<235;
    int cur=ui->tabWidget->currentIndex();
    tabWid.at(cur)->setPoint(pos);
}

void MainWindow::on_moveBtn_clicked()
{
    QVector<double> pos;
    int cur=ui->tabWidget->currentIndex();
    pos=tabWid.at(cur)->getPoint();
 //   qDebug()<<pos<<endl;
    if(!pos.isEmpty())
    {
   //     pos1<<(int)(pos.at(0))<<(int)(pos.at(1))<<(int)(pos.at(2));
        emit move(pos);
    }
    else
    {
        QMessageBox::warning(this,"Warning","不能使用空位移！！");
    }
}

void MainWindow::on_saveBtn_clicked()
{
    int cur=ui->tabWidget->currentIndex();
    tabWid.at(cur)->save();

}

void MainWindow::on_loadBtn_clicked()
{
    int cur=ui->tabWidget->currentIndex();
    tabWid.at(cur)->load();
}

void MainWindow::on_tabWidget_tabBarDoubleClicked(int index)
{
    bool ok=false;
    QString text = QInputDialog::getText(this,"INPUT","请输入产品名称",QLineEdit::Normal,"产品",&ok,Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    ui->tabWidget->tabBar()->setTabText(index,text);
}
