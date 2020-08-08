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
    programErrorBtn = new LightButton;
    alarmLabel=new QLabel;
    positiveLimitLabel=new QLabel;
    negativeLimitLabel=new QLabel;
    enableLabel=new QLabel;
    moveErrorLabel=new QLabel;
    moveStatusLabel=new QLabel;
    killStopLabel = new QLabel;
    programErrorLabel = new QLabel;

    posSlider= new QSlider;

    alarmLabel->setText("驱动器报警");
    enableLabel->setText("驱动器使能");
    moveErrorLabel->setText("运动错误");
    moveStatusLabel->setText("运动状态");
    killStopLabel->setText("速度误差");
    programErrorLabel->setText("程序错误");

    ui->gridLayout_6->addWidget(negativeLimitBtn,0,0);
    ui->gridLayout_6->addWidget(positiveLimitBtn,0,2);
    ui->gridLayout_5->addWidget(alarmLabel,0,0);
    ui->gridLayout_5->addWidget(enableLabel,0,2);
    ui->gridLayout_5->addWidget(moveErrorLabel,1,0);
    ui->gridLayout_5->addWidget(moveStatusLabel,1,2);
    ui->gridLayout_5->addWidget(killStopLabel,2,0);
    ui->gridLayout_5->addWidget(programErrorLabel,2,2);

    ui->gridLayout_5->addWidget(alarmBtn,0,1);
    ui->gridLayout_5->addWidget(enableBtn2,0,3);
    ui->gridLayout_5->addWidget(moveErrorBtn,1,1);
    ui->gridLayout_5->addWidget(moveStatusBtn,1,3);
    ui->gridLayout_5->addWidget(killStopBtn,2,1);
    ui->gridLayout_5->addWidget(programErrorBtn,2,3);

    ui->titleLabel->setText(name);
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    alarmLabel->setFont(font);
    enableLabel->setFont(font);
    moveErrorLabel->setFont(font);
    moveStatusLabel->setFont(font);
    killStopLabel->setFont(font);
    programErrorLabel->setFont(font);
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
        if(motorStatus&0x01)
        {
            retValue=acsc_Disable(MainWindow::hComm,ACSC_AXIS_0,nullptr);
            ui->enableBtn->setText("ENABLE");
        }
        else
        {
            retValue=acsc_Enable(MainWindow::hComm,ACSC_AXIS_0,nullptr);
            ui->enableBtn->setText("DISABLE");
        }

    }
    else if(objectName()=="Axis2")
    {
        if(motorStatus&0x01)
        {
            retValue=acsc_Disable(MainWindow::hComm,ACSC_AXIS_1,nullptr);
            ui->enableBtn->setText("ENABLE");
        }
        else
        {
            retValue=acsc_Enable(MainWindow::hComm,ACSC_AXIS_1,nullptr);
            ui->enableBtn->setText("DISABLE");
        }
    }
    else if(objectName()=="Axis3")
    {
        if(motorStatus&0x01)
        {
            retValue=acsc_Disable(MainWindow::hComm,ACSC_AXIS_2,nullptr);
            ui->enableBtn->setText("ENABLE");
        }
        else
        {
            retValue=acsc_Enable(MainWindow::hComm,ACSC_AXIS_2,nullptr);
            ui->enableBtn->setText("DISABLE");
        }
    }
    else if(objectName()==nullptr)
    {
        return;
    }
    commandHandle(retValue);
}

void axis::timerOut()
{
    if(objectName()=="Axis1")
    {
        retValue=acsc_GetAxisState(MainWindow::hComm,ACSC_AXIS_0,&axisStatus,nullptr);
        retValue=acsc_GetMotorState(MainWindow::hComm,ACSC_AXIS_0,&motorStatus,nullptr);
        retValue=acsc_GetSafetyInputPort(MainWindow::hComm,ACSC_AXIS_0,&safetyStatus,nullptr);
        retValue=acsc_GetRPosition(MainWindow::hComm,ACSC_AXIS_0,&prfPos,nullptr);
    }
    else if(objectName()=="Axis2")
    {
        retValue=acsc_GetAxisState(MainWindow::hComm,ACSC_AXIS_1,&axisStatus,nullptr);
        retValue=acsc_GetMotorState(MainWindow::hComm,ACSC_AXIS_1,&motorStatus,nullptr);
        retValue=acsc_GetSafetyInputPort(MainWindow::hComm,ACSC_AXIS_1,&safetyStatus,nullptr);
        retValue=acsc_GetRPosition(MainWindow::hComm,ACSC_AXIS_1,&prfPos,nullptr);
    }
    else if(objectName()=="Axis3")
    {
        retValue=acsc_GetAxisState(MainWindow::hComm,ACSC_AXIS_2,&axisStatus,nullptr);
        retValue=acsc_GetMotorState(MainWindow::hComm,ACSC_AXIS_2,&motorStatus,nullptr);
        retValue=acsc_GetSafetyInputPort(MainWindow::hComm,ACSC_AXIS_2,&safetyStatus,nullptr);
        retValue=acsc_GetRPosition(MainWindow::hComm,ACSC_AXIS_2,&prfPos,nullptr);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
    ui->posLabel->setText(QString("%1").arg(prfPos,3,'f',1));
    ui->posSlider->setSliderPosition((int)(prfPos));
    if(safetyStatus&0x0200)
    {
        this->alarmBtn->setRed();
    }
    else
    {
        this->alarmBtn->setBlue();
    }
    if(safetyStatus&0x01)
    {
        this->positiveLimitBtn->setRed();
    }
    else
    {
        this->positiveLimitBtn->setBlue();
    }
    if(safetyStatus&0x1000)
    {
        this->moveErrorBtn->setRed();
    }
    else
    {
        this->moveErrorBtn->setBlue();
    }
    if(safetyStatus&0x02)
    {
        this->negativeLimitBtn->setRed();
    }
    else
    {
        this->negativeLimitBtn->setBlue();
    }
    if(motorStatus&0x1)
    {
        this->enableBtn2->setRed();
        ui->enableBtn->setText("DISABLE");
    }
    else
    {
        this->enableBtn2->setBlue();
        ui->enableBtn->setText("ENABLE");
    }
    if(motorStatus&0x20)
    {
        this->moveStatusBtn->setRed();
    }
    else
    {
        this->moveStatusBtn->setBlue();
    }
    if(safetyStatus&0x4000)
    {
        this->killStopBtn->setRed();
    }
    else
    {
        this->killStopBtn->setBlue();
    }
    if(safetyStatus&0x2000000)
    {
        this->programErrorBtn->setRed();
    }
    else
    {
        this->programErrorBtn->setBlue();
    }
}

void axis::on_zeroBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        retValue=acsc_SetRPosition(MainWindow::hComm,ACSC_AXIS_0,0,nullptr);
        retValue=acsc_SetFPosition(MainWindow::hComm,ACSC_AXIS_0,0,nullptr);
    }
    else if(objectName()=="Axis2")
    {
        retValue=acsc_SetRPosition(MainWindow::hComm,ACSC_AXIS_1,0,nullptr);
        retValue=acsc_SetFPosition(MainWindow::hComm,ACSC_AXIS_1,0,nullptr);
    }
    else if(objectName()=="Axis3")
    {
        retValue=acsc_SetRPosition(MainWindow::hComm,ACSC_AXIS_2,0,nullptr);
        retValue=acsc_SetFPosition(MainWindow::hComm,ACSC_AXIS_2,0,nullptr);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
    commandHandle(retValue);
}

void axis::on_stopBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        retValue=acsc_Kill(MainWindow::hComm,ACSC_AXIS_0,nullptr);

    }
    else if(objectName()=="Axis2")
    {
        retValue=acsc_Kill(MainWindow::hComm,ACSC_AXIS_1,nullptr);
    }
    else if(objectName()=="Axis3")
    {
        retValue=acsc_Kill(MainWindow::hComm,ACSC_AXIS_2,nullptr);

    }
    else if(objectName()==nullptr)
    {
        return;
    }
    commandHandle(retValue);
}

void axis::on_clearBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        retValue=acsc_FaultClear(MainWindow::hComm,ACSC_AXIS_0,nullptr);
    }
    else if(objectName()=="Axis2")
    {
        retValue=acsc_FaultClear(MainWindow::hComm,ACSC_AXIS_1,nullptr);

    }
    else if(objectName()=="Axis3")
    {
        retValue=acsc_FaultClear(MainWindow::hComm,ACSC_AXIS_2,nullptr);
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

void axis::on_homeBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        retValue=acsc_RunBuffer(MainWindow::hComm,1,nullptr,nullptr);
    }
    else if(objectName()=="Axis2")
    {
        retValue=acsc_RunBuffer(MainWindow::hComm,2,nullptr,nullptr);
    }
    else if(objectName()=="Axis3")
    {
        retValue=acsc_RunBuffer(MainWindow::hComm,3,nullptr,nullptr);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
    commandHandle(retValue);
}

void axis::on_positiveMoveBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        retValue=acsc_ToPoint(MainWindow::hComm,ACSC_AMF_RELATIVE,ACSC_AXIS_0,ui->posEdit->text().toDouble(),nullptr);
    }
    else if(objectName()=="Axis2")
    {
        retValue=acsc_ToPoint(MainWindow::hComm,ACSC_AMF_RELATIVE,ACSC_AXIS_1,ui->posEdit->text().toDouble(),nullptr);
    }
    else if(objectName()=="Axis3")
    {
        retValue=acsc_ToPoint(MainWindow::hComm,ACSC_AMF_RELATIVE,ACSC_AXIS_2,ui->posEdit->text().toDouble(),nullptr);
    }
    else
    {
        return;
    }

    commandHandle(retValue);
}

void axis::on_negitiveMoveBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        retValue=acsc_ToPoint(mainWindow->GetHandle(),ACSC_AMF_RELATIVE,ACSC_AXIS_0,-ui->posEdit->text().toDouble(),nullptr);
    }
    else if(objectName()=="Axis2")
    {
        retValue=acsc_ToPoint(mainWindow->GetHandle(),ACSC_AMF_RELATIVE,ACSC_AXIS_1,-ui->posEdit->text().toDouble(),nullptr);
    }
    else if(objectName()=="Axis3")
    {
        retValue=acsc_ToPoint(mainWindow->GetHandle(),ACSC_AMF_RELATIVE,ACSC_AXIS_2,-ui->posEdit->text().toDouble(),nullptr);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
    commandHandle(retValue);
}

void axis::Move(QVector<double> pos)
{
    if(objectName()=="Axis1")
    {
    }
    else if(objectName()=="Axis2")
    {

    }
    else if(objectName()=="Axis3")
    {

    }
    else if(objectName()==nullptr)
    {
        return;
    }

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
