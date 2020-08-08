#ifndef AXIS_H
#define AXIS_H

#include <QWidget>
#include "lightbutton.h"
#include <QLabel>
#include <QSlider>
#include "gts.h"
#include <QTimer>
#include <QThread>
#include "mainwindow.h"

namespace Ui {
class axis;
}
class Home;
class RealtiveMove;
class AbsoluteMove;
class MainWindow;
class axis : public QWidget
{
    Q_OBJECT

public:
    explicit axis(QWidget *parent = nullptr,QString name="axis");
    ~axis();
    MainWindow *mainWindow;

public slots:
    void updateStart();
    void updateTerm();
    void Move(QVector<double> pos);

private slots:
    void on_enableBtn_clicked();
    void timerOut();

    void on_zeroBtn_clicked();

    void on_stopBtn_clicked();

    void on_clearBtn_clicked();

    void on_homeBtn_clicked();

    void on_positiveMoveBtn_clicked();

    void on_negitiveMoveBtn_clicked();

    void on_velEdit_editingFinished();

    void on_accEdit_editingFinished();

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
    LightButton *programErrorBtn;
    QLabel *alarmLabel;
    QLabel *positiveLimitLabel;
    QLabel *negativeLimitLabel;
    QLabel *enableLabel;
    QLabel *moveErrorLabel;
    QLabel *moveStatusLabel;
    QLabel *killStopLabel;
    QLabel *programErrorLabel;

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
    int axisStatus;
    int motorStatus;
    int safetyStatus;

    char* errorStr;
    int count;
    int* size;

signals:
    void commandHandle(int value);

};


#endif // AXIS_H
