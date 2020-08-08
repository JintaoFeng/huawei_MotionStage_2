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
#include <QXmlStreamWriter>
#include <QDomDocument>
#include "ACSC.h"

HANDLE MainWindow::hComm=(HANDLE)-1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,errorCount(256)

{
    ui->setupUi(this);
    this->setWindowTitle("MotionStage");
    axi[0]=ACSC_AXIS_0;
    axi[1]=ACSC_AXIS_1;
    axi[2]=ACSC_AXIS_2;
    axi[3]=-1;
    errorStr=new char[256];
    errorSize=new int;

    QImage image("Resources/UPLogo.jpg");
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    ui->imageLabel->setGeometry(0,0,image.width(),image.height());
       QFont font;
       font.setPointSize(14);
    ui->textEdit->setFont(font);

    QTimer *timer=new QTimer;
    connect(timer,&QTimer::timeout,this,&MainWindow::ShowTime);
    ui->timeNumber->setDigitCount(8);
    ui->timeNumber->display(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->dataNumber->setDigitCount(10);
    ui->dataNumber->display(QDateTime::currentDateTime().toString("yyyy.MM.dd"));
    timer->start(1000);
    ui->tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);

//    table *tab=new table(this);

//    tab->setAttribute(Qt::WA_DeleteOnClose);
//    int cur=ui->tabWidget->addTab(tab,"产品");
//    tabWid.insert(cur,tab);
//    ui->tabWidget->setCurrentIndex(cur);

    axi1=new axis(this,"Axis1");
    axi2=new axis(this,"Axis2");
    axi3=new axis(this,"Axis3");

    ui->horizontalLayout_6->addWidget(axi1,0);
    ui->horizontalLayout_6->addWidget(axi2,0);
    ui->horizontalLayout_6->addWidget(axi3,0);

    setting=new QSettings("u-precision","huawei_MotionStage",this);
    ui->IPEdit->setText(setting->value("IP").toString());


    ReadXML();
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
    setting->setValue("IP",ui->IPEdit->text());
    WriteXML();
    delete ui;
}

void MainWindow::ShowTime()
{
    ui->timeNumber->display(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->dataNumber->display(QDateTime::currentDateTime().toString("yyyy.MM.dd"));

    acsc_GetMotorState(hComm,ACSC_AXIS_0,&motorStatus1,nullptr);
    acsc_GetMotorState(hComm,ACSC_AXIS_1,&motorStatus2,nullptr);
    acsc_GetMotorState(hComm,ACSC_AXIS_2,&motorStatus3,nullptr);
    acsc_GetConnectionInfo(MainWindow::hComm,&connectInfo);
    if(connectInfo.Type==0)
    {
        ui->connectBtn->setText("Connect");
    }
    else
    {
        ui->connectBtn->setText("DisCon");
    }
    if(motorStatus1&0x1 && motorStatus2&0x1 && motorStatus3&0x1)
    {
        ui->enableAllBtn->setText("DISABLE");
    }
    else
    {
        ui->enableAllBtn->setText("ENABLE");
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
    acsc_GetConnectionInfo(MainWindow::hComm,&connectInfo);
    if(connectInfo.Type==0)
    {
        if(ui->connectBox->currentText()=="TCP")
        {
            HANDLE hComm=(HANDLE)-1;
            int Port = 703;
            QString ip=ui->IPEdit->text();
            QByteArray data=ip.toLatin1();
            char *IP=data.data();
            hComm = acsc_OpenCommEthernetTCP(IP,Port);
            if (hComm == ACSC_INVALID)
            {
                CommandHandler(0);
            }
            else
            {
                emit updateStart();
            }
        }
        else if(ui->connectBox->currentText()=="Simulation")
        {
            hComm = acsc_OpenCommSimulator();
            if (hComm == ACSC_INVALID)
            {
                CommandHandler(0);
            }
            else
            {
                emit updateStart();
            }
        }
        ui->connectBtn->setText("DisCon");
    }
    else
    {
        acsc_CloseComm(hComm);
        ui->connectBtn->setText("Connect");
    }

}

void MainWindow::CommandHandler(int value)
{
    if(!value)
    {
        retValue=acsc_GetLastError();
        if(retValue)
        {
            memset(errorStr,0,256);
            if(acsc_GetErrorString(hComm,retValue,errorStr,errorCount,errorSize))
            {
                errorStr[*errorSize]='\0';
                QMessageBox::warning(this,"ERROR",errorStr);
            }
        }

    }
}

void MainWindow::on_enableAllBtn_clicked()
{
    if(ui->enableAllBtn->text()=="ENABLE")
    {
        acsc_EnableM(hComm,axi,nullptr);
        ui->enableAllBtn->setText("DISABLE");
    }
    else
    {
        acsc_DisableM(hComm,axi,nullptr);
        ui->enableAllBtn->setText("ENABLE");
    }
}

void MainWindow::on_killStopBtn_clicked()
{
    acsc_KillM(MainWindow::hComm,axi,nullptr);
}


void MainWindow::on_setBtn_clicked()
{
    QVector<double> pos;
    double fPos;
    acsc_GetFPosition(hComm,ACSC_AXIS_0,&fPos,nullptr);
    pos<<fPos;
    acsc_GetFPosition(hComm,ACSC_AXIS_1,&fPos,nullptr);
    pos<<fPos;
    acsc_GetFPosition(hComm,ACSC_AXIS_2,&fPos,nullptr);
    pos<<fPos;
//    pos.clear();
//    pos<<1200<<23000<<235;
    int cur=ui->tabWidget->currentIndex();
    tabWid.at(cur)->setPoint(pos);
}

void MainWindow::on_moveBtn_clicked()
{
    QVector<double> pos;
    int cur=ui->tabWidget->currentIndex();
    pos=tabWid.at(cur)->getPoint();
    double fpos[4]={pos.at(0),pos.at(1),pos.at(2),-1};
    if(!pos.isEmpty())
    {
        qDebug()<<pos<<endl;
        acsc_ToPointM(hComm,0,axi,fpos,nullptr);
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

//void MainWindow::ReadXML()
//{
//    QStringList m_data;
//    QFile file("my.xml");
//    if (!file.open(QFile::ReadOnly | QFile::Text))
//    {
//   //     qDebug()<<"Error: cannot open file";
//        return;
//    }
//    QXmlStreamReader reader;
//    reader.setDevice(&file);
//    while (!reader.atEnd())
//    {
//        QXmlStreamReader::TokenType type = reader.readNext();
//        if (type == QXmlStreamReader::Characters && !reader.isWhitespace())
//        {
//            QString temp(reader.text().toUtf8());
//            m_data.append(temp);
//        }
//   //     qDebug()<<m_data<<endl;
//    }
//    int k=0;
//    if(m_data.size()/3==0)
//        return;
//    for(int i=0;i<m_data.size();i++)
//    {
//        table *tab=new table;
//        k=ui->tabWidget->addTab(tab,m_data.at(i));
//        tab->setAttribute(Qt::WA_DeleteOnClose);
//        tabWid.insert(k,tab);
//        ui->tabWidget->setCurrentIndex(k);
//        for(int j=0;j<3;j++)
//        {
//            QTableWidgetItem *newItem=new QTableWidgetItem();
//            newItem->setText(m_data.at(k++));
//            //qDebug()<<newItem->text();
//            ui->strategyTableWidget->setItem(i,j,newItem);
//        }
//    }
//}

void MainWindow::ReadXML()
{
    QStringList m_data;
    QFile file("data.xml");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<<"Error: cannot open file";
        return;
    }
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();
    QDomElement root=doc.documentElement();
    QDomNode node=root.firstChild();    //返回第一个子节点
   // qDebug()<<node.nodeName()<<endl;
    while(!node.isNull())
    {
        if(node.isElement())
        {
            QDomElement e= node.toElement();
        //    qDebug()<<e.tagName()<<e.attribute("tabText");
            if(e.tagName()=="table")
            {
                table *tab= new table;
                int k=ui->tabWidget->addTab(tab,e.attribute("tabText"));
                tabWid.insert(k,tab);
                tab->setAttribute(Qt::WA_DeleteOnClose);
                ui->tabWidget->setCurrentIndex(k);
                tabWid.at(k)->ReadXML(e);
            }
        }
        node=node.nextSibling();
    }
}

//void MainWindow::WriteXML()
//{
//    QFile file("my.xml");
//    if (!file.open(QFile::WriteOnly | QFile::Text))
//    {
//        qDebug() << "Error: cannot open file";
//        return;
//    }
//    QXmlStreamWriter writer(&file);
//    writer.setAutoFormatting(true);
//    writer.writeStartDocument();
//    writer.writeStartElement("tab");
//    for(int i=0;i <ui->tabWidget->count();i++)
//    {
//        writer.writeStartElement(QString("tab_%1").arg(i));
//        writer.writeTextElement("tabText",ui->tabWidget->tabText(i));
//        tabWid.at(i)->
//        writer.writeEndElement();
//    }
//    writer.writeEndElement();
//    writer.writeEndDocument();
//    file.close();
//}
void MainWindow::WriteXML()
{
    QFile file("data.xml");
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Error: cannot open file";
        return;
    }
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root=doc.createElement("tab");
    doc.appendChild(root);              //添加根节点tab
    for(int i=0;i<ui->tabWidget->count();i++)
    {
        QDomElement *table=new QDomElement;
        *table=doc.createElement("table");    //每个tab创建一个节点
        table->setAttribute("tabText",ui->tabWidget->tabText(i));
        tabWid.at(i)->WriteXML(*table);
        root.appendChild(*table);
   //     qDebug()<<i<<ui->tabWidget->count()<<tabWid.size()<<endl;
    }
    QTextStream outStream(&file);
    doc.save(outStream,4);
    file.close();
}

void MainWindow::on_connectBox_currentIndexChanged(const QString &arg1)
{
    if(arg1=="TCP")
    {
        ui->IPEdit->setEnabled(true);
    }
    else if(arg1=="Simulation")
    {
        ui->IPEdit->setDisabled(true);
    }
}
