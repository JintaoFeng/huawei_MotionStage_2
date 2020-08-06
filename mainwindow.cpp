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

{
    ui->setupUi(this);
    this->setWindowTitle("MotionStage");

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

//    axi1->show();
//    axi2->show();
//    axi3->show();

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
    WriteXML();
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
//    retValue=GT_Open();
//    CommandHandler("open",retValue);
//    if(~retValue)
//    {
//        emit updateStart();
//    }
//    QString str= "GTS800.cfg";
//    QByteArray temp=str.toLatin1();
//    char* file=temp.data();
//    retValue=GT_LoadConfig(file);
//    CommandHandler("load config",retValue);
//    GT_ClrSts(1);
//    GT_ClrSts(2);
//    GT_ClrSts(3);
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
            char* error=new char[20];
            int count=20;
            int *size=new int;
            acsc_GetErrorString(hComm,acsc_GetLastError(),error,count,size);
            qDebug()<<"Error while opening communication: %d\n"<<
            acsc_GetLastError()<<*error<<endl;
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
            qDebug()<<"error opening communication: %d\n"<< acsc_GetLastError();
        }
        else
        {
            emit updateStart();
        }
  //      qDebug()<<hComm<<endl;
    }
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
    pos.clear();
    pos<<1200<<23000<<235;
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
