#ifndef AXIS_H
#define AXIS_H

#include <QWidget>
#include "lightbutton.h"
#include <QLabel>
#include <QSlider>
#include "gts.h"
#include <QTimer>
#include <QThread>
namespace Ui {
class axis;
}
class Home;
class RealtiveMove;
class AbsoluteMove;
class axis : public QWidget
{
    Q_OBJECT

public:
    explicit axis(QWidget *parent = nullptr,QString name="axis");
    ~axis();

public slots:
    void updateStart();
    void updateTerm();
    void Move(QVector<double> pos);
    void absoluteThreadFinished();

private slots:
    void on_enableBtn_clicked();
    void timerOut();

    void on_zeroBtn_clicked();

    void on_stopBtn_clicked();

    void on_clearBtn_clicked();

    void on_homeBtn_clicked();

    void on_positiveMoveBtn_clicked();

    void on_negitiveMoveBtn_clicked();

private:
    Ui::axis *ui;

    QSlider* posSlider;
    LightButton *alarmBtn;
    LightButton *positiveLimitBtn;
    LightButton *negativeLimitBtn;
    LightButton *enableBtn2;
    LightButton *moveErrorBtn;
    LightButton *moveStatusBtn;
    LightButton *killStopBtn;
    QLabel *alarmLabel;
    QLabel *positiveLimitLabel;
    QLabel *negativeLimitLabel;
    QLabel *enableLabel;
    QLabel *moveErrorLabel;
    QLabel *moveStatusLabel;
    QLabel *killStopLabel;

    int retValue;
    QTimer* timer;
    double pos;
    double vel;
    double error;
    double acc;
    double prfPos;
    double prfVel;
    double prfAcc;
    double pPos;
    TJogPrm jogPrm;
    TTrapPrm trapPrm;
    long axisStatus;

    QThread* homeThread;
    Home *home;

    RealtiveMove *realtiveMove;
    QThread *realtiveThread;

    QThread* absoluteThread;
    AbsoluteMove* absoluteMove;

signals:
    void commandHandle(QString command,int value);
    void absolute(short profile,double start,double end,int rep);
    void realtive(short profile,double start,double end,int rep);
    void moveStop();
    void sHome(int axis);

};


class Home:public QObject
{
    Q_OBJECT
public:
    Home(QObject *parent=nullptr);
    ~Home(){}
public slots:
    void doWorks(short axis);
    void moveStop();
    void start();
private:
    THomeStatus tHomeSta;
    THomeStatus tHomeSta1;
    THomeStatus tHomeSta2;
    THomeStatus tHomeSta3;
    THomePrm tHomePrm;
signals:
    void workFinished();

};

class RealtiveMove:public QObject
{
    Q_OBJECT
public:
    RealtiveMove(QObject *parent=nullptr);
    ~RealtiveMove(){

    }
public slots:
    void doWorks(short profile,double start,double end,int rep);
    void moveStop();

signals:
    void workFinshed();


private:
    long axisState;
    int m_stop;
};

class AbsoluteMove:public QObject
{
    Q_OBJECT
public:
    AbsoluteMove(QObject *parent=nullptr);
    ~AbsoluteMove(){

    }
public slots:
    void doWorks(short profile,double start,double end,int rep);
    void moveStop();

signals:
    void workFinshed();


private:
    long axisState;
    int m_stop;
};

#endif // AXIS_H
