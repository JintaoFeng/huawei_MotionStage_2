#include "table.h"
#include "ui_table.h"
#include <QMenu>
#include <QDebug>
#include <QFileDialog>
#include <QTextEdit>
#include <QMessageBox>
#include <QInputDialog>
table::table(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::table)
{
    ui->setupUi(this);
    QList<QString> headers;
    QList<QString> Vheaders;
    headers<<"X"<<"Y"<<"Z";
    Vheaders<<"1";
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setColumnWidth(0,82);
    ui->tableWidget->setColumnWidth(1,82);
    ui->tableWidget->setColumnWidth(2,82);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
  //  ui->tableWidget->setRowCount(1);
  //  ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setVerticalHeaderLabels(Vheaders);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->verticalHeader()->setSectionsClickable(true);
    ui->tableWidget->horizontalHeader()->setSectionsClickable(true);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

 //   ui->tableWidget.set
    connect(ui->tableWidget->verticalHeader(),&QHeaderView::sectionDoubleClicked,this,&table::verRename);
    connect(ui->tableWidget->horizontalHeader(),&QHeaderView::sectionDoubleClicked,this,&table::horRename);
}

table::~table()
{
    delete ui;
}
void table::verRename(int logicIndex)
{
  //  qDebug()<<logicIndex<<endl;
    int a=ui->tableWidget->rowCount();
    QList<QString> Vheaders;
    bool ok=false;
    QString text = QInputDialog::getText(this,"INPUT","请输入点位名称",QLineEdit::Normal,"点位",&ok,Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    for(int i=0;i<a;i++)
    {
        Vheaders<<ui->tableWidget->verticalHeaderItem(i)->text();
    }
    Vheaders.replace(logicIndex,text);
    ui->tableWidget->setVerticalHeaderLabels(Vheaders);
}
void table::horRename(int logicIndex)
{
 //   qDebug()<<logicIndex<<endl;
    int a=ui->tableWidget->columnCount();
    QList<QString> Hheaders;
    bool ok=false;
    QString text = QInputDialog::getText(this,"INPUT","请输入轴名称",QLineEdit::Normal,"axis",&ok,Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    for(int i=0;i<a;i++)
    {
        Hheaders<<ui->tableWidget->horizontalHeaderItem(i)->text();
    }
    Hheaders.replace(logicIndex,text);
    ui->tableWidget->setHorizontalHeaderLabels(Hheaders);
}
void table::AddPoint()
{
    int a=ui->tableWidget->rowCount();
    QList<QString> Vheaders;
    bool ok=false;
    QString text = QInputDialog::getText(this,"INPUT","请输入点位名称",QLineEdit::Normal,"点位",&ok,Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    ui->tableWidget->insertRow(a);
    for(int i=0;i<a;i++)
    {
        Vheaders<<ui->tableWidget->verticalHeaderItem(i)->text();
    }
    Vheaders<<text;

    ui->tableWidget->setVerticalHeaderLabels(Vheaders);
    //   qDebug()<<ui->tableWidget->currentRow()<<endl;
  //  qDebug()<<this<<endl;
}
void table::DeletePoint()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}
void table::setPoint(QVector<double> &point)
{
    QTableWidgetItem item;
    int cur=ui->tableWidget->currentRow();
    if(cur>=0)
    {
        ui->tableWidget->setItem(cur,0,new QTableWidgetItem(QString::number(point.at(0)/1000.0)));
        ui->tableWidget->setItem(cur,1,new QTableWidgetItem(QString::number(point.at(1)/1000.0)));
        ui->tableWidget->setItem(cur,2,new QTableWidgetItem(QString::number(point.at(2)/1000.0)));
    }
    else
    {
        QMessageBox::warning(this,"Warning","请选择相应的位置！");
    }
}
QVector<double> table::getPoint()
{
    QVector<double> pos;
    int cur=ui->tableWidget->currentRow();
    if(cur>=0)
    {
        pos<<ui->tableWidget->item(cur,0)->text().toDouble()*1000;
        pos<<ui->tableWidget->item(cur,1)->text().toDouble()*1000;
        pos<<ui->tableWidget->item(cur,2)->text().toDouble()*1000;
    }
    else
    {
        QMessageBox::warning(this,"Warning","请选择相应的位置！");
      //  pos<<0<<0<<0;
    }
    return pos;
}
void table::save()
{
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save as..."),
            "//", tr("EXCEL files (*.xls);;HTML-Files (*.txt);;"));
    if (filepath != "")
    {
        int row = ui->tableWidget->rowCount();
        int col = 3;
        QList<QString> list;
        //添加列标题
        QString HeaderRow;
        for (int i = 0; i < col; i++)
        {
            HeaderRow.append(ui->tableWidget->horizontalHeaderItem(i)->text() + "\t");
        }
        list.push_back(HeaderRow);
        for (int i = 0; i < row; i++)
        {
            QString rowStr = "";
            rowStr += ui->tableWidget->verticalHeaderItem(i)->text() + "\t";
            for (int j = 0; j < col; j++){
                auto item=ui->tableWidget->item(i, j);
                if(item)
                    rowStr += item->text() + "\t";
                else
                    rowStr +="\t";
            }
            list.push_back(rowStr);
        }
        QTextEdit textEdit;
        for (int i = 0; i < list.size(); i++)
        {
            textEdit.append(list.at(i));
        }

        QFile file(filepath);
        if (file.open(QFile::WriteOnly | QIODevice::Text))
        {
            QTextStream ts(&file);
            ts.setCodec("utf-8");//这个地方大家自己判断是否用“utf-8”
            ts << textEdit.document()->toPlainText();
            file.close();
        }
    }

}
void table::load()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("Save as..."),
            "//", tr("EXCEL files (*.xls);;HTML-Files (*.txt);;"));
    if(filepath!="")
    {
        QFile file(filepath);
        if (file.open(QFile::ReadOnly | QIODevice::Text))
        {
            QTextStream ts(&file);
            ts.setCodec("utf-8");//这个地方大家自己判断是否用“utf-8”
            QString str=ts.readAll();
            QStringList strList=str.split("\n");
            int i=0;
            int j=0;
            QStringList Vheaders;
            if(!strList.isEmpty())
            {
                ui->tableWidget->clearContents();
                ui->tableWidget->setRowCount(1);
                ui->tableWidget->setRowCount(strList.count()-1);
                for(i=0;i<strList.count();i++)
                {
                    QStringList strList1=strList.at(i).split("\t");
                    if(i==0)
                    {
                        ui->tableWidget->setHorizontalHeaderLabels(strList1);
                        continue;
                    }
             //       qDebug()<<strList1<<endl;
                    for(j=0;j<strList1.count()-1;j++)
                    {
                        if(j==0)
                        {
                            Vheaders<<strList1.at(j);
                            continue;
                        }
                        ui->tableWidget->setItem(i-1,j-1,new QTableWidgetItem(strList1.at(j)));

                    }

                }
                ui->tableWidget->setVerticalHeaderLabels(Vheaders);
            }

            file.close();
        }
    }
}

void table::WriteXML(QDomElement &node)
{
 //   QStringList list=ui->tableWidget->Ver
    QDomDocument doc;
    for (int i=0;i<ui->tableWidget->rowCount();i++)
    {
        QDomElement *row=new QDomElement;
        *row=doc.createElement("ROW");
        row->setAttribute("rowName",ui->tableWidget->verticalHeaderItem(i)->text());

        for(int j=0;j<ui->tableWidget->columnCount();j++)
        {
            QDomElement *first=new QDomElement;
            *first=doc.createElement("COLUMN");
            first->setAttribute("columnName",ui->tableWidget->horizontalHeaderItem(j)->text());
            QDomText *text=new QDomText;
            QString rowStr;
            auto item=ui->tableWidget->item(i, j);
            if(item)
                rowStr += item->text();
            else
                rowStr +=" ";
            *text=doc.createTextNode(rowStr);
            first->appendChild(*text);
            row->appendChild(*first);
        }
        node.appendChild(*row);
    }

}
void table::ReadXML(QDomElement &node)
{
    if(!node.isNull())
    {
        QDomNodeList rowList=node.childNodes();
        QStringList verHeaderLabels;
        QStringList horHeaderLabels;
        for(int i=0;i<rowList.count();i++)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            verHeaderLabels<<rowList.at(i).toElement().attribute("rowName");
            QDomNodeList columnList=rowList.at(i).childNodes();
            for (int j=0;j<columnList.count();j++)
            {
                if(columnList.at(j).isElement())
                {
                    QDomElement e=columnList.at(j).toElement();
                    horHeaderLabels<<e.attribute("columnName");
                    ui->tableWidget->setItem(i,j,new QTableWidgetItem(e.text()));
                }
            }
            ui->tableWidget->setHorizontalHeaderLabels(horHeaderLabels);
            horHeaderLabels.clear();
        }
        ui->tableWidget->setVerticalHeaderLabels(verHeaderLabels);
        verHeaderLabels.clear();
    }
}
