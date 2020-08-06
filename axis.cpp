#include "axis.h"
#include "ui_axis.h"
#include <QtDebug>
#include <QMutex>
#include <QMessageBox>
#include "ACSC.h"

//class MainWindow;
axis::axis(QWidget *parent,QString name) :
    QWidget(parent),

    ui(new Ui::axis),
    count(256)
{
    ui->setupUi(this);
    size=new int;
    errorStr=new char[256];
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&axis::timerOut);
    this->setObjectName(name);

    if(this->objectName()=="Axis1")
    {
        ui->positiveLabel->setText(QString::number(250));
        ui->posSlider->setMaximum(250);

    }
    else if(this->objectName()=="Axis2")
    {
        ui->positiveLabel->setText(QString::number(100));
        ui->posSlider->setMaximum(100);
    }
    else if(this->objectName()=="Axis3")
    {
        ui->positiveLabel->setText(QString::number(100));
        ui->posSlider->setMaximum(100);
    }
    alarmBtn=new LightButton;
    positiveLimitBtn=new LightButton;
    negativeLimitBtn=new LightButton;
    enableBtn2=new LightButton;
    moveErrorBtn=new LightButton;
    moveStatusBtn=new LightButton;
    killStopBtn = new LightButton;
    alarmLabel=new QLabel;
    positiveLimitLabel=new QLabel;
    negativeLimitLabel=new QLabel;
    enableLabel=new QLabel;
    moveErrorLabel=new QLabel;
    moveStatusLabel=new QLabel;
    killStopLabel = new QLabel;

    posSlider= new QSlider;

    alarmLabel->setText("驱动器报警");
//    positiveLimitLabel->setText("正限位");
//    negativeLimitLabel->setText("负限位");
    enableLabel->setText("驱动器使能");
    moveErrorLabel->setText("运动错误");
    moveStatusLabel->setText("运动状态");
    killStopLabel->setText("急停输入");

    ui->gridLayout_6->addWidget(negativeLimitBtn,0,0);
    ui->gridLayout_6->addWidget(positiveLimitBtn,0,2);
    ui->gridLayout_5->addWidget(alarmLabel,0,0);
    ui->gridLayout_5->addWidget(enableLabel,0,2);
    ui->gridLayout_5->addWidget(moveErrorLabel,1,0);
    ui->gridLayout_5->addWidget(moveStatusLabel,1,2);
    ui->gridLayout_5->addWidget(killStopLabel,2,0);

    ui->gridLayout_5->addWidget(alarmBtn,0,1);
    ui->gridLayout_5->addWidget(enableBtn2,0,3);
    ui->gridLayout_5->addWidget(moveErrorBtn,1,1);
    ui->gridLayout_5->addWidget(moveStatusBtn,1,3);
    ui->gridLayout_5->addWidget(killStopBtn,2,1);

    ui->titleLabel->setText(name);
    QFont font;
    font.setPointSize(10);
    font.setBold(true);
    alarmLabel->setFont(font);
    enableLabel->setFont(font);
    moveErrorLabel->setFont(font);
    moveStatusLabel->setFont(font);
    killStopLabel->setFont(font);

  //  this->positiveLimitBtn->setBlue();
 //   this->negativeLimitBtn->setRed();
}

axis::~axis()
{
    delete ui;
    delete alarmBtn;
    delete positiveLimitBtn;
    delete negativeLimitBtn;
    delete enableBtn2;
    delete moveErrorBtn;
    delete moveStatusBtn;
    delete alarmLabel;
    delete positiveLimitLabel;
    delete negativeLimitLabel;
    delete enableLabel;
    delete moveErrorLabel;
    delete moveStatusLabel;
    delete killStopLabel;
    delete killStopBtn;
    delete timer;
}

void axis::on_enableBtn_clicked()
{
    if(objectName()=="Axis1")
    {
//        GT_GetSts(1,&axisStatus);
//        if(axisStatus&0x200)
//        {
//            retValue=GT_AxisOff(1);
//            if(!retValue)
//            {
//                ui->enableBtn->setText("Enable");
//            }
//            commandHandle("Axis1 Enable off",retValue);
//        }
//        else
//        {
//            retValue=GT_AxisOn(1);
//            commandHandle("Axis1 Enable on",retValue);
//            if(!retValue)
//            {
//                ui->enableBtn->setText("Disable");
//            }
//        }
        retValue=acsc_Enable(mainWindow->GetHandle(),ACSC_AXIS_0,nullptr);
    }
    else if(objectName()=="Axis2")
    {
//        GT_GetSts(2,&axisStatus);
//        if(axisStatus&0x200)
//        {
//            retValue=GT_AxisOff(2);
//            if(!retValue)
//            {
//                ui->enableBtn->setText("Enable");
//            }
//            commandHandle("Axis2 Enable off",retValue);
//        }
//        else
//        {
//            retValue=GT_AxisOn(2);
//            commandHandle("Axis2 Enable on",retValue);
//            if(!retValue)
//            {
//                ui->enableBtn->setText("Disable");
//            }
//        }
        retValue=acsc_Enable(mainWindow->GetHandle(),ACSC_AXIS_1,nullptr);
    }
    else if(objectName()=="Axis3")
    {
//        GT_GetSts(3,&axisStatus);
//        if(axisStatus&0x200)
//        {
//            retValue=GT_AxisOff(3);
//            if(!retValue)
//            {
//                ui->enableBtn->setText("Enable");
//            }
//            commandHandle("Axis3 Enable off",retValue);
//        }
//        else
//        {
//            retValue=GT_AxisOn(3);
//            commandHandle("Axis3 Enable on",retValue);
//            if(!retValue)
//            {
//                ui->enableBtn->setText("Disable");
//            }
//        }
        retValue=acsc_Enable(mainWindow->GetHandle(),ACSC_AXIS_2,nullptr);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
    if(!retValue)
    {
        qDebug()<<"error"<<acsc_GetLastError()<<endl;
    }
}

void axis::timerOut()
{

    if(objectName()=="Axis1")
    {
//        GT_GetAxisEncPos(1,&pos,1);
//        GT_GetAxisEncVel(1,&vel,1);
//        GT_GetAxisEncAcc(1,&acc,1);
//        GT_GetAxisError(1,&error);
//        GT_GetAxisPrfAcc(1,&prfAcc);
//        GT_GetAxisPrfPos(1,&prfPos);
//        GT_GetAxisPrfVel(1,&prfVel);
  //      GT_GetSts(1,&axisStatus);
        acsc_GetRPosition(mainWindow->GetHandle(),ACSC_AXIS_0,&prfPos,nullptr);
    }
    else if(objectName()=="Axis2")
    {
 //       GT_GetAxisEncPos(2,&pos,1);
//        GT_GetAxisEncVel(2,&vel,1);
//        GT_GetAxisError(2,&error);
//        GT_GetAxisEncAcc(2,&acc,1);
//        GT_GetAxisPrfAcc(2,&prfAcc);
//        GT_GetAxisPrfPos(2,&prfPos);
//        GT_GetAxisPrfVel(2,&prfVel);
//        GT_GetSts(2,&axisStatus);
        acsc_GetRPosition(mainWindow->GetHandle(),ACSC_AXIS_1,&prfPos,nullptr);
    }
    else if(objectName()=="Axis3")
    {
 //       GT_GetAxisEncPos(3,&pos,1);
//        GT_GetAxisEncVel(3,&vel,1);
//        GT_GetAxisError(3,&error);
//        GT_GetAxisEncAcc(3,&acc,1);
//        GT_GetAxisPrfAcc(3,&prfAcc);
//        GT_GetAxisPrfPos(3,&prfPos);
//        GT_GetAxisPrfVel(3,&prfVel);
//        GT_GetSts(3,&axisStatus);
        acsc_GetRPosition(mainWindow->GetHandle(),ACSC_AXIS_2,&prfPos,nullptr);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
//    ui->fposText->setText(QString("%1").arg(pos,3,'f',1));
//    ui->fvelText->setText(QString("%1").arg(vel,3,'f',1));
//    ui->errorText->setText(QString("%1").arg(error,3,'f',1));
//    ui->faccText->setText(QString("%1").arg(acc,3,'f',1));
//    ui->prfPosText->setText(QString("%1").arg(prfPos,3,'f',1));
//    ui->prfVelText->setText(QString("%1").arg(prfVel,3,'f',1));
//    ui->prfAccText->setText(QString("%1").arg(prfAcc,3,'f',1));
    ui->posLabel->setText(QString("%1").arg(prfPos,3,'f',1));
    ui->posSlider->setSliderPosition((int)(prfPos));
//    if(axisStatus&0x02)
//    {
//        this->alarmBtn->setRed();
//    }
//    else
//    {
//        this->alarmBtn->setBlue();
//    }
//    if(axisStatus&0x20)
//    {
//        this->positiveLimitBtn->setRed();
//    }
//    else
//    {
//        this->positiveLimitBtn->setBlue();
//    }
//    if(axisStatus&0x10)
//    {
//        this->moveErrorBtn->setRed();
//    }
//    else
//    {
//        this->moveErrorBtn->setBlue();
//    }
//    if(axisStatus&0x40)
//    {
//        this->negativeLimitBtn->setRed();
//    }
//    else
//    {
//        this->negativeLimitBtn->setBlue();
//    }
//    if(axisStatus&0x200)
//    {
//        this->enableBtn2->setRed();
//        ui->enableBtn->setText("Disable");
//    }
//    else
//    {
//        this->enableBtn2->setBlue();
//        ui->enableBtn->setText("Enable");
//    }
//    if(axisStatus&0x400)
//    {
//        this->moveStatusBtn->setRed();
//    }
//    else
//    {
//        this->moveStatusBtn->setBlue();
//    }
//    if(axisStatus&0x100)
//    {
//        this->killStopBtn->setRed();
//    }
//    else
//    {
//        this->killStopBtn->setBlue();
//    }
}

void axis::on_zeroBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        retValue=GT_ZeroPos(1,1);
        commandHandle("Axis1 zero",retValue);
    }
    else if(objectName()=="Axis2")
    {
        retValue=GT_ZeroPos(2,1);
        commandHandle("Axis2 zero",retValue);
    }
    else if(objectName()=="Axis3")
    {
        retValue=GT_ZeroPos(3,1);
        commandHandle("Axis3 zero",retValue);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}

void axis::on_stopBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        retValue=GT_Stop(1,1);
        commandHandle("Axis1 stop",retValue);
        emit moveStop();
    }
    else if(objectName()=="Axis2")
    {
        retValue=GT_Stop(1<<1,1<<1);
        commandHandle("Axis2 stop",retValue);
        emit moveStop();
    }
    else if(objectName()=="Axis3")
    {
        retValue=GT_Stop(1<<2,1<<2);
        commandHandle("Axis3 stop",retValue);
        emit moveStop();
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}

void axis::on_clearBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        retValue=GT_ClrSts(1,1);
    //    GT_SetDoBitReverse(MC_CLEAR,1,0,2);
        commandHandle("Axis1 Clear warning",retValue);
    }
    else if(objectName()=="Axis2")
    {
        retValue=GT_ClrSts(2,1);
   //     GT_SetDoBitReverse(MC_CLEAR,2,1,1);
        commandHandle("Axis2 Clear warning",retValue);
    }
    else if(objectName()=="Axis3")
    {
        retValue=GT_ClrSts(3,1);
   //     GT_SetDoBitReverse(MC_CLEAR,3,1,1);
        commandHandle("Axis3 Clear warning",retValue);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}


void axis::updateStart()
{
    this->timer->start(200);
}

void axis::updateTerm()
{
    timer->stop();
}

Home::Home(QObject* parent):QObject(parent)
{

}
void Home::doWorks(short axis)
{   int retValue;
    static QMutex mutex;
    retValue=1;
//    QMutexLocker locker(&mutex);

    retValue = GT_GetHomePrm(axis,&tHomePrm);
    tHomePrm.mode=11;
    tHomePrm.moveDir=-1;
    tHomePrm.indexDir=1;
    tHomePrm.edge=0;
    tHomePrm.triggerIndex=axis;
    tHomePrm.velHigh=5;
    tHomePrm.velLow=4;
    tHomePrm.acc=0.1;
    tHomePrm.dec=0.1;
    tHomePrm.searchHomeDistance=200000;
    tHomePrm.searchIndexDistance=30000;
    tHomePrm.escapeStep=1000;
    tHomePrm.pad2[1]=1;
    if(axis==1)
    {
        tHomePrm.velHigh=10;
        tHomePrm.velLow=8;
        tHomePrm.homeOffset=-124000;
    }
    else if(axis==2)
    {
        tHomePrm.homeOffset=-30000;
    }
    else if(axis==3)
    {
        tHomePrm.homeOffset=-31000;
    }

    retValue = GT_GoHome(axis,&tHomePrm);
    do
    {
        GT_GetHomeStatus(axis,&tHomeSta);
    }while(tHomeSta.run);
    GT_ClrSts(axis);
    GT_ZeroPos(axis);

    emit workFinished();
}

void Home::moveStop()
{

}
void Home::start()
{
    qDebug()<<"start!!"<<endl;
}
void axis::on_homeBtn_clicked()
{
    homeThread=new QThread;
    home=new Home;
    home->moveToThread(homeThread);

    connect(this,&axis::sHome,home,&Home::doWorks);

    connect(homeThread,&QThread::finished,home,&Home::deleteLater);
    connect(home,&Home::destroyed,homeThread,&QThread::deleteLater);
    connect(home,&Home::workFinished,[this](){
        homeThread->quit();
    });
    homeThread->start();
    if(objectName()=="Axis1")
    {
        emit sHome(1);
    }
    else if(objectName()=="Axis2")
    {
        emit sHome(2);
    }
    else if(objectName()=="Axis3")
    {
        emit sHome(3);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}

void axis::on_positiveMoveBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        retValue=acsc_ToPoint(mainWindow->GetHandle(),ACSC_AMF_RELATIVE,ACSC_AXIS_0,ui->posEdit->text().toDouble(),nullptr);
        qDebug()<<retValue<<endl;
    }
    else if(objectName()=="Axis2")
    {
//        GT_ClrSts(2,1);
//        GT_GetSts(2,&axisStatus);
//        if(axisStatus&0x20)
//        {
//            QMessageBox::warning(this,"Warning","该自由度处于正限位状态，请先往负方向运动");
//            return;
//        }
//        GT_GetTrapPrm(2,&trapPrm);
//        trapPrm.acc=ui->accEdit->text().toDouble();
//        trapPrm.dec=ui->accEdit->text().toDouble();
//        trapPrm.smoothTime=49;
//        GT_PrfTrap(2);
//        GT_SetTrapPrm(2,&trapPrm);
//        retValue=GT_SetVel(2,ui->velEdit->text().toDouble());
//        commandHandle("axis2 setVel",retValue);
//        GT_GetPrfPos(2,&pPos);
//        pPos+=ui->posEdit->text().toDouble()*1000;
//        GT_SetPos(2,(long)pPos);
//        GT_Update(1<<1);
        retValue=acsc_ToPoint(mainWindow->GetHandle(),ACSC_AMF_RELATIVE,ACSC_AXIS_0,ui->posEdit->text().toDouble(),nullptr);
        qDebug()<<retValue<<endl;
    }
    else if(objectName()=="Axis3")
    {
//        GT_ClrSts(3,1);
//        GT_GetSts(3,&axisStatus);
//        if(axisStatus&0x20)
//        {
//            QMessageBox::warning(this,"Warning","该自由度处于正限位状态，请先往负方向运动");
//            return;
//        }
//        GT_GetTrapPrm(3,&trapPrm);
//        trapPrm.acc=ui->accEdit->text().toDouble();
//        trapPrm.dec=ui->accEdit->text().toDouble();
//        trapPrm.smoothTime=49;
//        GT_PrfTrap(3);
//        GT_SetTrapPrm(3,&trapPrm);
//        retValue=GT_SetVel(3,ui->velEdit->text().toDouble());
//        commandHandle("axis3 setVel",retValue);
//        GT_GetPrfPos(3,&pPos);
//        pPos+=ui->posEdit->text().toDouble()*1000;
//        GT_SetPos(3,(long)pPos);
//        GT_Update(1<<2);
        retValue=acsc_ToPoint(mainWindow->GetHandle(),ACSC_AMF_RELATIVE,ACSC_AXIS_0,ui->posEdit->text().toDouble(),nullptr);
        qDebug()<<retValue<<endl;
    }


    {
        return;
    }
    if(!retValue)
    {
        retValue=acsc_GetLastError();
        acsc_GetErrorString(mainWindow->GetHandle(),retValue,errorStr,count,size);
        qDebug()<<errorStr<<endl;
    }
}


void RealtiveMove::doWorks(short profile,double start,double end,int rep)
{
    long mask=long(1<<(profile-1));
    if(!rep)
    {
        start+=end;
    }
    else
    {
        start-=end;
    }
 //   qDebug()<<start<<endl;
    GT_SetPos(profile,(long)start);
    GT_Update(mask);
    do
    {
        if(m_stop)
        {
            m_stop=0;
            break;
        }
        GT_GetSts(profile,&axisState);
    }while(axisState&0x400);

    emit workFinshed();
}

void RealtiveMove::moveStop()
{
    this->m_stop=1;
}

RealtiveMove::RealtiveMove(QObject *parent):QObject(parent)
{

}

void axis::on_negitiveMoveBtn_clicked()
{
    if(objectName()=="Axis1")
    {
//        GT_ClrSts(1,1);
//        GT_GetSts(1,&axisStatus);
//        if(axisStatus&0x40)
//        {
//            QMessageBox::warning(this,"Warning","该自由度处于负限位状态，请先往正方向运动");
//            return;
//        }
//        GT_GetTrapPrm(1,&trapPrm);
//        trapPrm.acc=ui->accEdit->text().toDouble();
//        trapPrm.dec=ui->accEdit->text().toDouble();
//        trapPrm.smoothTime=49;
//        GT_PrfTrap(1);
//        GT_SetTrapPrm(1,&trapPrm);
//        retValue=GT_SetVel(1,ui->velEdit->text().toDouble());
//        commandHandle("axis1 setVel",retValue);
//        GT_GetPrfPos(1,&pPos);
//        pPos-=ui->posEdit->text().toDouble()*1000;
//        GT_SetPos(1,(long)pPos);
//        GT_Update(1);
        retValue=acsc_ToPoint(mainWindow->GetHandle(),ACSC_AMF_RELATIVE,ACSC_AXIS_0,-ui->posEdit->text().toDouble(),nullptr);
        qDebug()<<retValue<<endl;
    }
    else if(objectName()=="Axis2")
    {
//        GT_ClrSts(2,1);
//        GT_GetSts(2,&axisStatus);
//        if(axisStatus&0x40)
//        {
//            QMessageBox::warning(this,"Warning","该自由度处于负限位状态，请先往正方向运动");
//            return;
//        }
//        GT_GetTrapPrm(2,&trapPrm);
//        trapPrm.acc=ui->accEdit->text().toDouble();
//        trapPrm.dec=ui->accEdit->text().toDouble();
//        trapPrm.smoothTime=49;
//        GT_PrfTrap(2);
//        GT_SetTrapPrm(2,&trapPrm);
//        retValue=GT_SetVel(2,ui->velEdit->text().toDouble());
//        commandHandle("axis2 setVel",retValue);
//        GT_GetPrfPos(2,&pPos);
//        pPos-=ui->posEdit->text().toDouble()*1000;
//        GT_SetPos(2,(long)pPos);
//        GT_Update(1<<1);
        retValue=acsc_ToPoint(mainWindow->GetHandle(),ACSC_AMF_RELATIVE,ACSC_AXIS_1,-ui->posEdit->text().toDouble(),nullptr);
        qDebug()<<retValue<<endl;
    }
    else if(objectName()=="Axis3")
    {
//        GT_ClrSts(3,1);
//        GT_GetSts(3,&axisStatus);
//        if(axisStatus&0x40)
//        {
//            QMessageBox::warning(this,"Warning","该自由度处于负限位状态，请先往正方向运动");
//            return;
//        }
//        GT_GetTrapPrm(3,&trapPrm);
//        trapPrm.acc=ui->accEdit->text().toDouble();
//        trapPrm.dec=ui->accEdit->text().toDouble();
//        trapPrm.smoothTime=49;
//        GT_PrfTrap(3);
//        GT_SetTrapPrm(3,&trapPrm);
//        retValue=GT_SetVel(3,ui->velEdit->text().toDouble());
//        commandHandle("axis3 setVel",retValue);
//        GT_GetPrfPos(3,&pPos);
//        pPos-= ui->posEdit->text().toDouble()*1000;
//        GT_SetPos(3,(long)pPos);
//        GT_Update(1<<2);
        retValue=acsc_ToPoint(mainWindow->GetHandle(),ACSC_AMF_RELATIVE,ACSC_AXIS_2,-ui->posEdit->text().toDouble(),nullptr);
        qDebug()<<retValue<<endl;
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}

void axis::absoluteThreadFinished()
{
    absoluteMove=nullptr;
    absoluteThread=nullptr;
//    qDebug()<<absoluteMove<<absoluteThread<<endl;
}

void axis::Move(QVector<double> pos)
{
//    absoluteThread=new QThread(nullptr);
//    absoluteMove=new AbsoluteMove;
//    absoluteMove->moveToThread(absoluteThread);

//    connect(this,&axis::absolute,absoluteMove,&AbsoluteMove::doWorks);

//    connect(absoluteThread,&QThread::finished,absoluteMove,&QObject::deleteLater);
//    connect(absoluteMove,&AbsoluteMove::destroyed,absoluteThread,&QThread::deleteLater);

//    connect(this,&axis::moveStop,absoluteMove,&AbsoluteMove::moveStop,Qt::DirectConnection);
//    connect(absoluteThread,&QThread::destroyed,this,&axis::absoluteThreadFinished);

//    connect(absoluteMove,&AbsoluteMove::workFinshed,[this](){
//        absoluteThread->quit();
//    });
//    absoluteThread->start();

    if(objectName()=="Axis1")
    {
        GT_GetTrapPrm(1,&trapPrm);
        trapPrm.acc=ui->accEdit->text().toDouble();
        trapPrm.dec=ui->accEdit->text().toDouble();
        trapPrm.smoothTime=49;
        GT_PrfTrap(1);
        GT_SetTrapPrm(1,&trapPrm);
        retValue=GT_SetVel(1,ui->velEdit->text().toDouble());
        commandHandle("axis1 setVel",retValue);
        GT_SetPos(1,(long)pos.at(0));
        GT_Update(1);
     //   emit absolute(1,0,pos.at(0),0);
    }
    else if(objectName()=="Axis2")
    {
        GT_GetTrapPrm(2,&trapPrm);
        trapPrm.acc=ui->accEdit->text().toDouble();
        trapPrm.dec=ui->accEdit->text().toDouble();
        trapPrm.smoothTime=49;
        GT_PrfTrap(2);
        GT_SetTrapPrm(2,&trapPrm);
        retValue=GT_SetVel(2,ui->velEdit->text().toDouble());
        commandHandle("axis2 setVel",retValue);
        GT_SetPos(2,(long)pos.at(1));
        GT_Update(1<<1);

   //     emit absolute(2,0,pos.at(1),0);
    }
    else if(objectName()=="Axis3")
    {
        GT_GetTrapPrm(3,&trapPrm);
        trapPrm.acc=ui->accEdit->text().toDouble();
        trapPrm.dec=ui->accEdit->text().toDouble();
        trapPrm.smoothTime=49;
        GT_PrfTrap(3);
        GT_SetTrapPrm(3,&trapPrm);
        retValue=GT_SetVel(3,ui->velEdit->text().toDouble());
        commandHandle("axis3 setVel",retValue);
        GT_SetPos(3,(long)pos.at(2));
        GT_Update(1<<2);
     //   emit absolute(3,0,pos.at(2),0);
    }
    else if(objectName()==nullptr)
    {
        return;
    }

}



AbsoluteMove::AbsoluteMove(QObject* parent):QObject(parent),m_stop(0)
{

}

void AbsoluteMove::doWorks(short profile,double start,double end,int rep)
{
    long mask=long(1<<(profile-1));

    GT_SetPos(profile,(long)end);
    GT_Update(mask);
    do
    {
        if(m_stop)
        {
            m_stop=0;
            GT_Stop(profile,1);
            break;
        }
        GT_GetSts(profile,&axisState);
    }while(axisState&0x400);

    emit workFinshed();
}


void AbsoluteMove::moveStop()
{
    m_stop=1;
}



void axis::on_posEdit_editingFinished()
{

}

void axis::on_velEdit_editingFinished()
{
//    qDebug()<<mainWindow->GetHandle()<<endl;
    int retValue=0;
    if(this->objectName()=="Axis1")
    {
        retValue=acsc_SetVelocity(mainWindow->GetHandle(),ACSC_AXIS_0,ui->velEdit->text().toDouble(),nullptr);

    }
    else if(this->objectName()=="Axis2")
    {
        retValue=acsc_SetVelocity(mainWindow->GetHandle(),ACSC_AXIS_1,ui->velEdit->text().toDouble(),nullptr);

    }
    else if(this->objectName()=="Axis3")
    {
        retValue=acsc_SetVelocity(mainWindow->GetHandle(),ACSC_AXIS_2,ui->velEdit->text().toDouble(),nullptr);

    }
    if(!retValue)
    {
        qDebug()<<"error"<<acsc_GetLastError()<<endl;
    }
 //   qDebug()<<retValue<<endl;
}

void axis::on_accEdit_editingFinished()
{
    int retValue=0;
    if(this->objectName()=="Axis1")
    {
        retValue= acsc_SetAcceleration(mainWindow->GetHandle(),ACSC_AXIS_0,ui->accEdit->text().toDouble(),nullptr);
        retValue= acsc_SetDeceleration(mainWindow->GetHandle(),ACSC_AXIS_0,ui->accEdit->text().toDouble(),nullptr);
    }
    else if(this->objectName()=="Axis2")
    {
        retValue= acsc_SetAcceleration(mainWindow->GetHandle(),ACSC_AXIS_1,ui->accEdit->text().toDouble(),nullptr);
        retValue= acsc_SetDeceleration(mainWindow->GetHandle(),ACSC_AXIS_1,ui->accEdit->text().toDouble(),nullptr);
    }
    else if(this->objectName()=="Axis3")
    {
        retValue= acsc_SetAcceleration(mainWindow->GetHandle(),ACSC_AXIS_2,ui->accEdit->text().toDouble(),nullptr);
        retValue= acsc_SetDeceleration(mainWindow->GetHandle(),ACSC_AXIS_2,ui->accEdit->text().toDouble(),nullptr);
    }
    if(!retValue)
    {
        qDebug()<<"error"<<acsc_GetLastError()<<endl;
    }
}
