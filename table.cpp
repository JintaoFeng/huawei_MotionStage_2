#include "table.h"
#include "ui_table.h"
#include <QMenu>
#include <QDebug>
#include <QFileDialog>
#include <QTextEdit>
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
    ui->tableWidget->setItem(ui->tableWidget->currentRow(),0,new QTableWidgetItem(QString::number(point.at(0))));
    ui->tableWidget->setItem(ui->tableWidget->currentRow(),1,new QTableWidgetItem(QString::number(point.at(1))));
    ui->tableWidget->setItem(ui->tableWidget->currentRow(),2,new QTableWidgetItem(QString::number(point.at(2))));
}
QVector<double> table::getPoint()
{
    QVector<double> pos;
    int cur=ui->tableWidget->currentRow();
    pos<<ui->tableWidget->item(cur,0)->text().toDouble();
    pos<<ui->tableWidget->item(cur,1)->text().toDouble();
    pos<<ui->tableWidget->item(cur,2)->text().toDouble();
    return pos;
}
void table::save()
{
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save as..."),
            "..//hauwei_MotionStage", tr("EXCEL files (*.xls);;HTML-Files (*.txt);;"));
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
        //导出后将表格情况，这一步大家自己选择要不要
   //     ui->tableWidget->clearContents();
   //     ui->tableWidget->setRowCount(0);
    }

}
void table::load()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("Save as..."),
            "..//hauwei_MotionStage", tr("EXCEL files (*.xls);;HTML-Files (*.txt);;"));
    if(filepath!="")
    {
        QFile file(filepath);
        if (file.open(QFile::ReadOnly | QIODevice::Text))
        {
            QTextStream ts(&file);
            ts.setCodec("utf-8");//这个地方大家自己判断是否用“utf-8”
        //    ts << textEdit.document()->toPlainText();
            QString str=ts.readAll();
            QStringList strList=str.split("\n");
            QDataStream data;
            if(!strList.isEmpty())
            {
                QStringListIterator it(strList);
                while (it.hasNext()) {
              //      qDebug()<<it.next()<<endl;
                    it.next();
                    QString str1=it.next();
                    str1.split("\t");
                }

            }

     //       <<strList.<<endl;
            file.close();
        }
    }
}
