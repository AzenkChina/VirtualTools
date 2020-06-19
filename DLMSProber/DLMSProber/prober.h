#ifndef PROBER_H
#define PROBER_H

#include <QMainWindow>
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include "include/GXBytebuffer.h"
#include "include/GXDLMSCommon.h"
#include "communication.h"

struct parameter
{
    uint8_t motion;
    uint8_t client;
    uint16_t logical;
    uint16_t physical;
    uint8_t addressMode;
    CGXByteBuffer akey;
    CGXByteBuffer ekey;
    CGXByteBuffer password;
    DLMS_AUTHENTICATION level;
    QString comm;
    uint16_t classID ;
    QString obis;
    uint8_t index;
};

namespace Ui {
class Prober;
}

class Thread : public QThread
{
    Q_OBJECT

public:
    explicit Thread(QWidget *parent = nullptr);
    ~Thread();
    void init(struct parameter *para, QString data);

signals:
    void updateMessage(const QString);

    void finishWork();

private:
    CGXDLMSSecureClient *cl = nullptr;
    CGXCommunication *comm = nullptr;
    struct parameter para;
    QString data;
    void run();
};

class Prober : public QMainWindow
{
    Q_OBJECT

public:
    explicit Prober(QWidget *parent = nullptr);
    ~Prober();

private slots:
    void on_ButtonRead_pressed();

    void on_ButtonWrite_pressed();

    void on_ButtonExecute_pressed();

    void on_updateMessage(const QString);

    void on_finishWork();

    void on_Button2Text_pressed();

    void on_Button2Time_pressed();

    void on_ButtonHexDec_pressed();

    void on_SerialNo_highlighted(const QString &arg1);

    void on_ButtonGetAddress_pressed();

    void on_Serial_receive();

    void on_Timer_overflow();

    void on_Level_currentIndexChanged(int index);

private:
    Ui::Prober *ui;
    QSerialPort *Port = nullptr;
    QTimer *Timer = nullptr;
    class Thread *Thread = nullptr;
    QByteArray recv;

    void getAvaliableSerials();

    bool praseParameter(struct parameter &para);
};

#endif // PROBER_H
