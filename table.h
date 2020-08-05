#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QtXml>

namespace Ui {
class table;
}

class table : public QWidget
{
    Q_OBJECT

public:
    explicit table(QWidget *parent = nullptr);
    ~table();

    void AddPoint();
    void DeletePoint();
    void setPoint(QVector<double> &point);
    QVector<double> getPoint();
    void save();
    void load();
    void WriteXML(QDomElement &node);
    void ReadXML(QDomElement &node);
private slots:
    void verRename(int logicalIndex);
    void horRename(int logicalIndex);
private:
    Ui::table *ui;
};

#endif // TABLE_H
