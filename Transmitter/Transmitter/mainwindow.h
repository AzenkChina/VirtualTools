#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QTime>

#define SOH             0x01 //Xmodem数据头
#define EOT             0x04 //发送结束
#define ACK             0x06 //认可响应
#define NAK             0x15 //不认可响应
#define CAN             0x18 //撤销传送
#define CTRLZ           0x1A //填充字符

#define SEND_REQUEST    0x00 //发送请求
#define WAIT_RESPONSE   0xFF //等待回复

#define SEND_HEADER     0x00 //发送文件头
#define SEND_FILE       0x03 //发送文件体
#define SEND_END        0x0C //发送结束

#define TIM_PERIOD      10 //定时器周期(毫秒)
#define TIM_OVERFLOW    (5*1000) //接收超时时间(毫秒)
#define RETRY_TIME      4 //重试次数

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Serial_receive();

    void on_Timer_overflow();

    void on_StartButton_released();

    void on_FileButton_pressed();

private:
    Ui::MainWindow *ui;
    QTimer *Timer = nullptr;
    QString FileName;
    QSerialPort *Serial = nullptr;

    QByteArray file;
    QByteArray buff;
    uint64_t start;
    uint64_t stop;
    uint64_t current;

    int recv_frame_timeout = 0;
    int buff_size = 0;

    uint8_t sequence = SEND_REQUEST;
    uint8_t step = SEND_HEADER;
    uint8_t retry = 0;
    uint32_t wait_frame_timeout = 0;
    int packet_in_file = 0;

    void flush_serial();
    uint8_t sum(uint8_t size, uint8_t *buff);
    uint16_t getframe(uint16_t size, uint8_t *buff);
};

#endif // MAINWINDOW_H
