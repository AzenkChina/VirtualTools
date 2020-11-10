#ifndef PROBER_H
#define PROBER_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QFileDialog>
#include <QThread>
#include <QTime>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

#include "communication.h"
#include "include/GXDLMSCommon.h"
#include "include/GXBytebuffer.h"
#include "include/GXHelpers.h"

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
    QString ip;
    uint16_t port;
    bool negotiate = false;
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

class Thread : public QThread
{
    Q_OBJECT

public:
    explicit Thread(QWidget *parent = nullptr);
    ~Thread();
    void init(struct parameter *para, std::vector<struct closure> *closure, bool set, bool get);

signals:
    void updateMessage(const QString);

    void updateResult(const QString);

    void finishWork();

private:
    CGXDLMSSecureClient *cl = nullptr;
    CGXCommunication *comm = nullptr;
    struct parameter para;
    std::vector<struct closure> closure;
    bool set = true;
    bool get = false;
    void run();
};

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

    void on_updateMessage(const QString);

    void on_updateResult(const QString);

    void on_finishWork();

private:
    Ui::Programmer *ui;
    QByteArray recv;
    class Thread *Thread = nullptr;
    std::vector<struct closure> closure;

    void getAvaliableSerials();

    bool praseParameter(struct parameter &para);
};

#endif // PROBER_H
