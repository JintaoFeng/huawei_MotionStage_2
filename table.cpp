#include "table.h"
#include "ui_table.h"
#include <QMenu>
#include <QDebug>
#include <QFileDialog>
#include <QTextEdit>
#include <QMessageBox>
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
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setVerticalHeaderLabels(Vheaders);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
}

table::~table()
{
    delete ui;
}

void table::AddPoint()
{
    int a=ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(a+1);
 //   qDebug()<<ui->tableWidget->currentRow()<<endl;
  //  qDebug()<<this<<endl;
}
void table::DeletePoint()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
  //  qDebug()<<ui->tableWidget->currentRow()<<endl;
   // qDebug()<<this<<endl;
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
            if(!strList.isEmpty())
            {
                ui->tableWidget->clearContents();
                ui->tableWidget->setRowCount(1);
                ui->tableWidget->setRowCount(strList.count()-1);
       //        qDebug()<<strList.count()<<endl;
                for(i=1;i<strList.count();i++)
                {
                    QStringList strList1=strList.at(i).split("\t");
                    for(j=0;j<strList1.count()-1;j++)
                    {
                        ui->tableWidget->setItem(i-1,j,new QTableWidgetItem(strList1.at(j)));
             //           qDebug()<<strList1.at(j)<<endl;
                    }
                }
            }

            file.close();
        }
    }
}
