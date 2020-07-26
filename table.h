#ifndef TABLE_H
#define TABLE_H

#include <QWidget>

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
private:
    Ui::table *ui;
};

#endif // TABLE_H
