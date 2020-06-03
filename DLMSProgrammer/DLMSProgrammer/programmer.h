#ifndef PROBER_H
#define PROBER_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QFileDialog>
#include <QTimer>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

#include "include/GXBytebuffer.h"

struct parameter
{
    uint8_t client;
    uint16_t logical;
    uint16_t physicalstart;
    uint16_t physicalstop;
    uint8_t addressMode;
    CGXByteBuffer akey;
    CGXByteBuffer ekey;
    CGXByteBuffer password;
    DLMS_AUTHENTICATION level;
    QString comm;
};

struct closure
{
    QString name;
    uint16_t id;
    QString obis;
    uint8_t index;
    QString data;
    bool method;
};

namespace Ui {
class Programmer;
}

class Programmer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Programmer(QWidget *parent = nullptr);
    ~Programmer();

private slots:
    void on_SerialNo_activated(const QString &arg1);

    void on_Level_currentIndexChanged(int index);

    void on_ButtonLoad_pressed();

    void on_ButtonStart_pressed();

    void on_Timer_overflow();

private:
    Ui::Programmer *ui;
    QByteArray recv;
    QTimer *Timer = nullptr;
    std::vector<struct closure> closure;

    void getAvaliableSerials();

    bool praseParameter(struct parameter &para);
};

#endif // PROBER_H
