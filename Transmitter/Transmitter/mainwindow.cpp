#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <string.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->Baudrate->setCurrentIndex(3);
    ui->CheckBit->setCurrentIndex(2);

    this->flush_serial();
    this->Timer = new QTimer(this);
    connect(this->Timer, SIGNAL(timeout()), this, SLOT(on_Timer_overflow()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_Serial_receive() {
    QByteArray info = this->Serial->readAll();
    if(this->buff.size() < 1024) {
        this->buff.append(info);
    }
}

uint16_t MainWindow::getframe(uint16_t size, uint8_t *buff) {
    if(this->buff_size != this->buff.size()) {
        this->buff_size = this->buff.size();
        this->recv_frame_timeout = 0;
    }
    else {
        if(this->recv_frame_timeout < 30) {
            this->recv_frame_timeout += 10;
            if(this->recv_frame_timeout >= 30) {
                if(this->buff_size) {
                    if(this->buff_size > size) {
                        memcpy(buff, this->buff.data(), size);
                        this->buff.clear();
                        return(size);
                    }
                    else {
                        memcpy(buff, this->buff.data(), static_cast<size_t>(this->buff_size));
                        this->buff.clear();
                        return(static_cast<uint16_t>(this->buff_size));
                    }
                }
            }
        }
    }

    return(0);
}

void MainWindow::on_Timer_overflow() {
    uint8_t frame[256];
    memset(frame, 0, sizeof(frame));

    if(this->sequence == SEND_REQUEST) {
        goto send;
    }
    else if(this->sequence == WAIT_RESPONSE) {
        goto recv;
    }

send:
    if(this->step == SEND_HEADER) {
        //启动一次p2p文件传输
        frame[0] = SOH;
        frame[1] = 0;
        frame[2] = 0xff;

        memcpy(&frame[3], ui->Password->text().toStdString().data(), static_cast<size_t>(ui->Password->text().toStdString().size()));
        memcpy(&frame[19], this->FileName.toStdString().data(), static_cast<size_t>(this->FileName.size()));
        frame[131] = this->sum(128, &frame[3]);

        frame[132] = (this->current >> 56) & 0xff;
        frame[133] = (this->current >> 48) & 0xff;
        frame[134] = (this->current >> 40) & 0xff;
        frame[135] = (this->current >> 32) & 0xff;
        frame[136] = (this->current >> 24) & 0xff;
        frame[137] = (this->current >> 16) & 0xff;
        frame[138] = (this->current >> 8) & 0xff;
        frame[139] = (this->current >> 0) & 0xff;

        frame[140] = this->sum(8, &frame[132]);

        this->Serial->write((const char *)frame, 141);
        this->sequence = WAIT_RESPONSE;
        this->wait_frame_timeout = 0;
        this->packet_in_file = 0;

        ui->LogBrowser->append(QString::fromStdString("设备 ") + QString::number(this->current) + QString::fromStdString(" 启动传输"));
    }
    else if(this->step == SEND_FILE) {
        //传输一帧数据
        frame[0] = SOH;
        frame[1] = ((this->packet_in_file + 1) & 0xff);
        frame[2] = ~((this->packet_in_file + 1) & 0xff);
        memset(&frame[3], CTRLZ, 128);
        //还有数据待传输
        if((this->packet_in_file * 128) < this->file.size()) {
            //剩余数据不小于128字节
            if(((this->packet_in_file * 128) + 128) < this->file.size()) {
                memcpy(&frame[3], &this->file.data()[this->packet_in_file * 128], 128);
            }
            //剩余数据小于128字节
            else {
                memcpy(&frame[3], &this->file.data()[this->packet_in_file * 128], static_cast<size_t>((this->file.size() - this->packet_in_file * 128)));
            }

            frame[131] = this->sum(128, &frame[3]);

            frame[132] = (this->current >> 56) & 0xff;
            frame[133] = (this->current >> 48) & 0xff;
            frame[134] = (this->current >> 40) & 0xff;
            frame[135] = (this->current >> 32) & 0xff;
            frame[136] = (this->current >> 24) & 0xff;
            frame[137] = (this->current >> 16) & 0xff;
            frame[138] = (this->current >> 8) & 0xff;
            frame[139] = (this->current >> 0) & 0xff;

            frame[140] = this->sum(8, &frame[132]);

            this->Serial->write((const char *)frame, 141);

            this->sequence = WAIT_RESPONSE;
            this->wait_frame_timeout = 0;

            ui->LogBrowser->append(QString::fromStdString("包 ") + QString::number(this->packet_in_file) + QString::fromStdString(" 正在传输"));
        }
        //已经没有数据待传输
        else {
            //已经没有数据了
            frame[0] = EOT;
            frame[1] = (this->current >> 56) & 0xff;
            frame[2] = (this->current >> 48) & 0xff;
            frame[3] = (this->current >> 40) & 0xff;
            frame[4] = (this->current >> 32) & 0xff;
            frame[5] = (this->current >> 24) & 0xff;
            frame[6] = (this->current >> 16) & 0xff;
            frame[7] = (this->current >> 8) & 0xff;
            frame[8] = (this->current >> 0) & 0xff;

            frame[9] = this->sum(8, &frame[1]);

            this->Serial->write((const char *)frame, 10);
            this->sequence = WAIT_RESPONSE;
            this->wait_frame_timeout = 0;
            this->step = SEND_END;

            ui->LogBrowser->append(QString::fromStdString("设备 ") + QString::number(this->current) + QString::fromStdString(" 正在结束"));
        }
    }
    else if(this->step == SEND_END) {
        //已经没有数据了
        frame[0] = EOT;
        frame[1] = (this->current >> 56) & 0xff;
        frame[2] = (this->current >> 48) & 0xff;
        frame[3] = (this->current >> 40) & 0xff;
        frame[4] = (this->current >> 32) & 0xff;
        frame[5] = (this->current >> 24) & 0xff;
        frame[6] = (this->current >> 16) & 0xff;
        frame[7] = (this->current >> 8) & 0xff;
        frame[8] = (this->current >> 0) & 0xff;

        frame[9] = this->sum(8, &frame[1]);

        this->Serial->write((const char *)frame, 10);
        this->sequence = WAIT_RESPONSE;
        this->wait_frame_timeout = 0;

        ui->LogBrowser->append(QString::fromStdString("设备 ") + QString::number(this->current) + QString::fromStdString(" 正在结束"));
    }
    else {
        //结束传输
        ui->LogBrowser->append(QString::fromStdString("传输出错，停止"));
        goto end;
    }

    return;

recv:
    if(step == SEND_HEADER) {
        if(this->wait_frame_timeout < TIM_OVERFLOW) {
            this->wait_frame_timeout += TIM_PERIOD;
            if(this->wait_frame_timeout >= TIM_OVERFLOW) {
                if(this->retry < RETRY_TIME) {
                    this->retry += 1;
                    if(this->retry >= RETRY_TIME) {
                        ui->LogBrowser->append(QString::fromStdString("设备 ") + QString::number(this->current) + QString::fromStdString(" 传输失败"));
                        goto next;
                    }
                    else {
                        ui->LogBrowser->append(QString::fromStdString("第 ") + QString::number(this->retry) + QString::fromStdString(" 次重试"));
                        this->sequence = SEND_REQUEST;
                    }
                }
                else {
                    ui->LogBrowser->append(QString::fromStdString("设备 ") + QString::number(this->current) + QString::fromStdString(" 传输失败"));
                    goto next;
                }
            }
            else {
                uint16_t frame_length = this->getframe(sizeof(frame), frame);
                if(!frame_length) {
                    return;
                }
                else {
                    if(frame_length < 10) {
                        return;
                    }

                    if(frame[0] != NAK) {
                        return;
                    }

                    uint64_t address = 0;

                    address += frame[frame_length - 9];
                    address <<= 8;
                    address += frame[frame_length - 8];
                    address <<= 8;
                    address += frame[frame_length - 7];
                    address <<= 8;
                    address += frame[frame_length - 6];
                    address <<= 8;
                    address += frame[frame_length - 5];
                    address <<= 8;
                    address += frame[frame_length - 4];
                    address <<= 8;
                    address += frame[frame_length - 3];
                    address <<= 8;
                    address += frame[frame_length - 2];

                    if(frame[frame_length - 1] != this->sum(8, &frame[frame_length - 9])) {
                        return;
                    }
                    else if (address != this->current) {
                        return;
                    }
                    else {
                        //初始化成功，开始传输文件
                        this->sequence = SEND_REQUEST;
                        this->step = SEND_FILE;
                        this->retry = 0;
                    }
                }
            }
        }
        else {
            ui->LogBrowser->append(QString::fromStdString("设备 ") + QString::number(this->current) + QString::fromStdString(" 传输失败"));
            goto next;
        }
    }
    else if(step == SEND_FILE) {
        if(this->wait_frame_timeout < TIM_OVERFLOW) {
            this->wait_frame_timeout += TIM_PERIOD;
            if(this->wait_frame_timeout >= TIM_OVERFLOW) {
                if(this->retry < RETRY_TIME) {
                    this->retry += 1;
                    if(this->retry >= RETRY_TIME) {
                        ui->LogBrowser->append(QString::fromStdString("设备 ") + QString::number(this->current) + QString::fromStdString(" 传输失败"));
                        goto next;
                    }
                    else {
						ui->LogBrowser->append(QString::fromStdString("第 ") + QString::number(this->retry) + QString::fromStdString(" 次重试"));
                        this->sequence = SEND_REQUEST;
                    }
                }
                else {
                    ui->LogBrowser->append(QString::fromStdString("设备 ") + QString::number(this->current) + QString::fromStdString(" 传输失败"));
                    goto next;
                }
            }
            else {
                uint16_t frame_length = this->getframe(sizeof(frame), frame);
                if(!frame_length) {
                    return;
                }
                else {
                    if(frame_length < 10) {
                        return;
                    }

                    uint64_t address = 0;

                    address += frame[frame_length - 9];
                    address <<= 8;
                    address += frame[frame_length - 8];
                    address <<= 8;
                    address += frame[frame_length - 7];
                    address <<= 8;
                    address += frame[frame_length - 6];
                    address <<= 8;
                    address += frame[frame_length - 5];
                    address <<= 8;
                    address += frame[frame_length - 4];
                    address <<= 8;
                    address += frame[frame_length - 3];
                    address <<= 8;
                    address += frame[frame_length - 2];

                    if(frame[frame_length - 1] != this->sum(8, &frame[frame_length - 9])) {
                        return;
                    }
                    else if(address != this->current) {
                        return;
                    }
                    else if(frame[0] != ACK) {
                        return;
                    }
                    else {
                        //完成本包传输，开始下一步
                        ui->LogBrowser->append(QString::fromStdString("包 ") + QString::number(this->packet_in_file) + QString::fromStdString(" 传输成功"));
                        this->packet_in_file += 1;
                        this->sequence = SEND_REQUEST;
                        this->retry = 0;
                    }
                }
            }
        }
        else {
            ui->LogBrowser->append(QString::fromStdString("设备 ") + QString::number(this->current) + QString::fromStdString(" 传输失败"));
            goto next;
        }
    }
    else if(step == SEND_END) {
        if(this->wait_frame_timeout < TIM_OVERFLOW) {
            wait_frame_timeout += TIM_PERIOD;
            if(wait_frame_timeout >= TIM_OVERFLOW) {
                if(retry < RETRY_TIME) {
                    retry += 1;
                    if(retry >= RETRY_TIME) {
                        ui->LogBrowser->append(QString::fromStdString("设备 ") + QString::number(this->current) + QString::fromStdString(" 传输失败"));
                        goto next;
                    }
                    else {
						ui->LogBrowser->append(QString::fromStdString("第 ") + QString::number(this->retry) + QString::fromStdString(" 次重试"));
                        sequence = SEND_REQUEST;
                    }
                }
                else {
                    ui->LogBrowser->append(QString::fromStdString("设备 ") + QString::number(this->current) + QString::fromStdString(" 传输失败"));
                    goto next;
                }
            }
            else {
                uint16_t frame_length = this->getframe(sizeof(frame), frame);
                if(!frame_length) {
                    return;
                }
                else {
                    if(frame_length < 10) {
                        return;
                    }

                    uint64_t address = 0;

                    address += frame[frame_length - 9];
                    address <<= 8;
                    address += frame[frame_length - 8];
                    address <<= 8;
                    address += frame[frame_length - 7];
                    address <<= 8;
                    address += frame[frame_length - 6];
                    address <<= 8;
                    address += frame[frame_length - 5];
                    address <<= 8;
                    address += frame[frame_length - 4];
                    address <<= 8;
                    address += frame[frame_length - 3];
                    address <<= 8;
                    address += frame[frame_length - 2];

                    if(frame[frame_length - 1] != this->sum(8, &frame[frame_length - 9])) {
                        return;
                    }
                    else if (address != this->current) {
                        return;
                    }
                    else if(frame[0] != ACK) {
                        return;
                    }
                    else {
                        //完成此次传输，开始下一个地址
                        ui->LogBrowser->append(QString::fromStdString("设备 ") + QString::number(this->current) + QString::fromStdString(" 传输成功"));
                        sequence = SEND_REQUEST;
                        retry = 0;
                        goto next;
                    }
                }
            }
        }
        else {
            ui->LogBrowser->append(QString::fromStdString("设备 ") + QString::number(this->current) + QString::fromStdString(" 传输失败"));
            goto next;
        }
    }
    else {
        //结束传输
        ui->LogBrowser->append(QString::fromStdString("传输出错，停止"));
        goto end;
    }
    return;

next:
    this->sequence = SEND_REQUEST;
    this->step = SEND_HEADER;
    this->retry = 0;
    this->wait_frame_timeout = 0;
    this->packet_in_file = 0;
    this->recv_frame_timeout = 0;
    this->buff_size = 0;
    this->buff.clear();

    this->Serial->readAll();

    if(this->current < this->stop) {
        this->current += 1;
        ui->AddressCurrent->setText(QString::number(this->current));
        ui->Process->setValue(static_cast<int>(((this->current - this->start) * 100) / (this->stop - this->start)));
    }
    else {
        goto end;
    }

    return;

end:
    this->sequence = SEND_REQUEST;
    this->step = SEND_HEADER;
    this->retry = 0;
    this->wait_frame_timeout = 0;
    this->packet_in_file = 0;
    this->recv_frame_timeout = 0;
    this->buff_size = 0;
    this->buff.clear();

    this->Timer->stop();
    ui->StartButton->setText(QString::fromStdString("已停止"));

    ui->SerialNo->setEnabled(true);
    ui->Baudrate->setEnabled(true);
    ui->CheckBit->setEnabled(true);
    ui->StopBit->setEnabled(true);
    ui->AddressStart->setEnabled(true);
    ui->AddressStop->setEnabled(true);
    ui->Password->setEnabled(true);
    ui->FileButton->setEnabled(true);
    this->Serial->close();
    delete this->Serial;
    this->Serial = nullptr;
    this->flush_serial();

    ui->Process->setValue(100);
    ui->LogBrowser->append(QString::fromStdString("全部设备传输结束"));

    QDateTime date_time = QDateTime::currentDateTime();
    QString fileName = date_time.toString("yyMMddhhmmss");
    fileName += date_time.toString(".log");

    QFile *File = new QFile(fileName);
    if (!File) {
        return;
    }
    if (!File->open(QIODevice::ReadWrite)) {
        return;
    }
    File->write(ui->LogBrowser->document()->toPlainText().toStdString().data(), ui->LogBrowser->document()->toPlainText().toStdString().size());
    File->close();
    delete File;

    return;
}

void MainWindow::on_StartButton_released() {
    if(this->Timer->isActive()) {
        //正在运行
        this->Timer->stop();
        ui->StartButton->setText(QString::fromStdString("已停止"));

        ui->SerialNo->setEnabled(true);
        ui->Baudrate->setEnabled(true);
        ui->CheckBit->setEnabled(true);
        ui->StopBit->setEnabled(true);
        ui->AddressStart->setEnabled(true);
        ui->AddressStop->setEnabled(true);
        ui->Password->setEnabled(true);
        ui->FileButton->setEnabled(true);
        ui->Process->setValue(0);
        this->Serial->close();
        delete this->Serial;
        this->Serial = nullptr;
        this->flush_serial();

        this->sequence = SEND_REQUEST;
        this->step = SEND_HEADER;
        this->retry = 0;
        this->wait_frame_timeout = 0;
        this->packet_in_file = 0;
        this->recv_frame_timeout = 0;
        this->buff_size = 0;
        this->buff.clear();
    }
    else {
        //已经停止
        ui->LogBrowser->clear();
        ui->AddressCurrent->setText("0");

        this->start = static_cast<uint64_t>(ui->AddressStart->text().toULongLong());
        this->stop = static_cast<uint64_t>(ui->AddressStop->text().toULongLong());

        if((this->start == 0) || (this->stop == 0) || (this->start > this->stop)) {
            ui->LogBrowser->append(QString::fromStdString("地址范围不正确"));
            return;
        }

        if(this->file.size() <= 0) {
            ui->LogBrowser->append(QString::fromStdString("文件不可用"));
            return;
        }

        if(ui->SerialNo->count() <= 0) {
            ui->LogBrowser->append(QString::fromStdString("没有串口可用"));
            return;
        }

        this->Serial = new QSerialPort();
#if defined(Q_OS_WIN32)
        this->Serial->setPortName(ui->SerialNo->currentText());
#elif defined(Q_OS_LINUX)
        this->Serial->setPortName(QString::fromStdString("/dev/") + ui->SerialNo->currentText());
#endif
        if(!this->Serial->open(QIODevice::ReadWrite)) {
            ui->LogBrowser->append(QString::fromStdString("串口无法打开"));
            delete this->Serial;
            this->Serial = nullptr;
            return;
        }

        this->Serial->setDataBits(QSerialPort::Data8);
        this->Serial->setFlowControl(QSerialPort::NoFlowControl);
        switch(ui->Baudrate->currentIndex()) {
            case 0:this->Serial->setBaudRate(QSerialPort::Baud1200,QSerialPort::AllDirections);break;
            case 1:this->Serial->setBaudRate(QSerialPort::Baud2400,QSerialPort::AllDirections);break;
            case 2:this->Serial->setBaudRate(QSerialPort::Baud4800,QSerialPort::AllDirections);break;
            case 3:this->Serial->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);break;
            case 4:this->Serial->setBaudRate(QSerialPort::Baud19200,QSerialPort::AllDirections);break;
            case 5:this->Serial->setBaudRate(QSerialPort::Baud38400,QSerialPort::AllDirections);break;
            case 6:this->Serial->setBaudRate(QSerialPort::Baud57600,QSerialPort::AllDirections);break;
            case 7:this->Serial->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);break;
            default:this->Serial->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);break;
        }
        switch (ui->CheckBit->currentIndex()) {
            case 0:this->Serial->setParity(QSerialPort::NoParity);break;
            case 1:this->Serial->setParity(QSerialPort::OddParity);break;
            case 2:this->Serial->setParity(QSerialPort::EvenParity);break;
            default:this->Serial->setParity(QSerialPort::NoParity);break;
        }
        switch (ui->StopBit->currentIndex()) {
            case 0:this->Serial->setStopBits(QSerialPort::OneStop);break;
            case 1:this->Serial->setStopBits(QSerialPort::OneAndHalfStop);break;
            case 2:this->Serial->setStopBits(QSerialPort::TwoStop);break;
            default:this->Serial->setStopBits(QSerialPort::OneStop);break;
        }

        connect(this->Serial,SIGNAL(readyRead()),this,SLOT(on_Serial_receive()));

        this->current = this->start;
        ui->AddressCurrent->setText(QString::number(this->current));

        ui->StartButton->setText(QString::fromStdString("运行中"));
        this->Timer->start(TIM_PERIOD);

        ui->SerialNo->setEnabled(false);
        ui->Baudrate->setEnabled(false);
        ui->CheckBit->setEnabled(false);
        ui->StopBit->setEnabled(false);
        ui->AddressStart->setEnabled(false);
        ui->AddressStop->setEnabled(false);
        ui->Password->setEnabled(false);
        ui->FileButton->setEnabled(false);
        ui->Process->setValue(0);
        ui->LogBrowser->append(QString::fromStdString("文件名称 ") + this->FileName);
        ui->LogBrowser->append(QString::fromStdString("文件大小 ") + QString::number(this->file.size()) + QString::fromStdString(" 字节"));
    }
}

void MainWindow::on_FileButton_pressed() {
    //加载文件
    this->file.clear();

    QFile *File;
    QString fileName = QFileDialog::getOpenFileName(this);
    File = new QFile(fileName);
    if (!File->open(QIODevice::ReadOnly)) {
        ui->FileButton->setText(QString::fromStdString("未加载"));
        delete File;
        return;
    }

    QFileInfo info(fileName);
    this->FileName = info.fileName();
    if (this->FileName.size() > 32) {
        ui->FileButton->setText(QString::fromStdString("未加载"));
        File->close();
        delete File;
        return;
    }

    this->file.append(File->readAll());
    File->close();
    delete File;

    ui->FileButton->setText(QString::fromStdString("已加载"));
}

void MainWindow::on_SerialNo_activated(const QString &arg1) {
#if defined(Q_OS_WIN32)
    ui->SerialNo->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->SerialNo->addItem(info.portName());
    }
    if(ui->SerialNo->count() == 0) {
        ui->SerialNo->addItem(QString::fromStdString("COM1"));
    }
    for(int cnt=0; cnt<ui->SerialNo->count(); cnt++) {
        if(arg1 == ui->SerialNo->itemText(cnt)) {
            ui->SerialNo->setCurrentIndex(cnt);
        }
    }
#else
    (void)arg1;
#endif
}

void MainWindow::flush_serial() {
    QString current = ui->SerialNo->currentText();
    ui->SerialNo->clear();
#if defined(Q_OS_WIN32)
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->SerialNo->addItem(info.portName());
    }
    if(ui->SerialNo->count() == 0) {
        ui->SerialNo->addItem(QString::fromStdString("COM1"));
    }
#elif defined(Q_OS_LINUX)
    ui->SerialNo->addItem(QString::fromStdString("ttyS0"));
    ui->SerialNo->addItem(QString::fromStdString("ttyS1"));
    ui->SerialNo->addItem(QString::fromStdString("ttyS2"));
    ui->SerialNo->addItem(QString::fromStdString("ttyUSB0"));
    ui->SerialNo->addItem(QString::fromStdString("ttyUSB1"));
    ui->SerialNo->addItem(QString::fromStdString("ttyUSB2"));
#endif
    for(int cnt=0; cnt<ui->SerialNo->count(); cnt++) {
        if(current == ui->SerialNo->itemText(cnt)) {
            ui->SerialNo->setCurrentIndex(cnt);
        }
    }
}

uint8_t MainWindow::sum(uint8_t size, uint8_t *buff) {
    uint8_t result = 0;
    for (uint8_t cnt=0; cnt < size; cnt++) {
        result += buff[cnt];
    }
    return(result);
}
