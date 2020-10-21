#include "prober.h"
#include "ui_prober.h"

Thread::Thread(QWidget *parent) {
    (void)parent;
}

Thread::~Thread() {
    if(this->cl != nullptr) {
        delete this->cl;
        this->cl = nullptr;
    }

    if(this->comm != nullptr) {
        this->comm->Close();
        delete this->comm;
        this->comm = nullptr;
    }
}

void Thread::init(struct parameter *para, QString data) {
    this->para = *para;
    this->data = data;
}

void Thread::run() {
    if(para.addressMode == 1) {
        if(para.level == DLMS_AUTHENTICATION_LOW) {
            this->cl = new CGXDLMSSecureClient(true,
                                         para.client,
                                         (1 << 30) | (para.logical << 16),
                                         para.level,
                                         para.password.ToString().data(),
                                         DLMS_INTERFACE_TYPE_HDLC);
        }
        else {
            this->cl = new CGXDLMSSecureClient(true,
                                         para.client,
                                         (1 << 30) | (para.logical << 16),
                                         para.level,
                                         nullptr,
                                         DLMS_INTERFACE_TYPE_HDLC);
        }
    }
    else if (para.addressMode == 2) {
        if(para.level == DLMS_AUTHENTICATION_LOW) {
            this->cl = new CGXDLMSSecureClient(true,
                                         para.client,
                                         (2 << 30) | (para.logical << 16) | (para.physical % 100),
                                         para.level,
                                         para.password.ToString().data(),
                                         DLMS_INTERFACE_TYPE_HDLC);
        }
        else {
            this->cl = new CGXDLMSSecureClient(true,
                                         para.client,
                                         (2 << 30) | (para.logical << 16) | (para.physical % 100),
                                         para.level,
                                         nullptr,
                                         DLMS_INTERFACE_TYPE_HDLC);
        }
    }
    else {
        if(para.level == DLMS_AUTHENTICATION_LOW) {
            this->cl = new CGXDLMSSecureClient(true,
                                         para.client,
                                         (3 << 30) | (para.logical << 16) | (para.physical % 10000),
                                         para.level,
                                         para.password.ToString().data(),
                                         DLMS_INTERFACE_TYPE_HDLC);
        }
        else {
            this->cl = new CGXDLMSSecureClient(true, para.client,
                                         (3 << 30) | (para.logical << 16) | (para.physical % 10000),
                                         para.level,
                                         nullptr,
                                         DLMS_INTERFACE_TYPE_HDLC);
        }
    }


    if(para.level == DLMS_AUTHENTICATION_HIGH_GMAC) {
        this->cl->GetCiphering()->SetSecurity(DLMS_SECURITY_AUTHENTICATION_ENCRYPTION);
    }
    else {
        this->cl->GetCiphering()->SetSecurity(DLMS_SECURITY_NONE);
    }

    this->cl->SetProposedConformance(static_cast<DLMS_CONFORMANCE>(\
                                   DLMS_CONFORMANCE_GENERAL_PROTECTION | \
                                   DLMS_CONFORMANCE_GENERAL_BLOCK_TRANSFER | \
                                   DLMS_CONFORMANCE_READ | \
                                   DLMS_CONFORMANCE_WRITE | \
                                   DLMS_CONFORMANCE_UN_CONFIRMED_WRITE | \
                                   DLMS_CONFORMANCE_ATTRIBUTE_0_SUPPORTED_WITH_SET | \
                                   DLMS_CONFORMANCE_PRIORITY_MGMT_SUPPORTED | \
                                   DLMS_CONFORMANCE_ATTRIBUTE_0_SUPPORTED_WITH_GET | \
                                   DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_GET_OR_READ | \
                                   DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_SET_OR_WRITE | \
                                   DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_ACTION | \
                                   DLMS_CONFORMANCE_MULTIPLE_REFERENCES | \
                                   DLMS_CONFORMANCE_INFORMATION_REPORT | \
                                   DLMS_CONFORMANCE_DATA_NOTIFICATION | \
                                   DLMS_CONFORMANCE_ACCESS | \
                                   DLMS_CONFORMANCE_PARAMETERIZED_ACCESS | \
                                   DLMS_CONFORMANCE_GET | \
                                   DLMS_CONFORMANCE_SET | \
                                   DLMS_CONFORMANCE_SELECTIVE_ACCESS | \
                                   DLMS_CONFORMANCE_EVENT_NOTIFICATION | \
                                   DLMS_CONFORMANCE_ACTION\
                                   ));

    this->cl->SetAutoIncreaseInvokeID(false);

    CGXByteBuffer bb;

    bb.Clear();
    bb.SetHexString(QString::fromLocal8Bit("4446534552564552").toStdString());
    this->cl->GetCiphering()->SetSystemTitle(bb);

    bb.Clear();
    if(this->para.ded) {
        bb.SetHexString(QString::fromLocal8Bit("31323334353637383132333435363738").toStdString());
    }
    this->cl->GetCiphering()->SetDedicatedKey(bb);

    this->cl->GetCiphering()->SetAuthenticationKey(para.akey);
    this->cl->GetCiphering()->SetBlockCipherKey(para.ekey);

    this->comm = new CGXCommunication(cl, 6000, GX_TRACE_LEVEL_OFF, nullptr);

    int ret;
    if(para.comm == "NET") {
        if ((ret = this->comm->Connect(para.ip.toStdString().data(), para.port)) != 0) {
            delete this->comm;
            this->comm = nullptr;
            delete this->cl;
            this->cl = nullptr;
            emit updateMessage("连接网络出错");
            emit finishWork();
            return;
        }
    }
    else {
        if ((ret = this->comm->Open(para.comm.toStdString().data(), para.negotiate, 115200)) != 0) {
            delete this->comm;
            this->comm = nullptr;
            delete this->cl;
            this->cl = nullptr;
            emit updateMessage("打开串口出错");
            emit finishWork();
            return;
        }
    }

    if ((ret = this->comm->InitializeConnection()) != 0) {
        this->comm->Close();
        delete this->comm;
        this->comm = nullptr;
        delete this->cl;
        this->cl = nullptr;
        emit updateMessage("链路初始化出错");
        emit finishWork();
        return;
    }

    CGXDLMSCommon Object(para.classID, para.obis.toStdString());

    if(this->para.motion == 1) {
        bb.Clear();
        bb.SetHexString(this->data.toStdString());
        std::string value;
        if(this->comm->Read(&Object, para.index, &bb, value) != DLMS_ERROR_CODE_OK) {
            emit updateMessage("读取失败");
        }
        else {
            QString str;
            for (unsigned int n = 0; n < value.size(); n++) {
                QString val;
                val.sprintf("%02X", static_cast<unsigned char>(value.data()[n]));
                str.append(val);
            }

            QString mark;
            mark.append("读取成功");
            mark.append("[");
            mark.append(QDateTime::currentDateTime().toString("hh:mm:ss"));
            mark.append("]");

            emit updateMessage(mark);
            emit updateMessage(str);
        }
    }
    else if(this->para.motion == 2) {
        bb.Clear();
        bb.SetHexString(this->data.toStdString());
        CGXReplyData reply;

        if(this->comm->Write(&Object, para.index, bb, reply) != DLMS_ERROR_CODE_OK) {
            emit updateMessage("设置失败");
        }
        else {
            CGXByteBuffer rep;
            rep = reply.GetData();
            QString str;
            for (unsigned int n = 0; n < rep.GetSize(); n++) {
                QString val;
                val.sprintf("%02X", static_cast<unsigned char>(rep.GetData()[n]));
                str.append(val);
            }

            QString mark;
            mark.append("设置成功");
            mark.append("[");
            mark.append(QDateTime::currentDateTime().toString("hh:mm:ss"));
            mark.append("]");

            emit updateMessage(mark);
            emit updateMessage(str);
        }
    }
    else {
        bb.Clear();
        bb.SetHexString(this->data.toStdString());
        CGXReplyData reply;

        if(this->comm->Method(&Object, para.index, bb, reply) != DLMS_ERROR_CODE_OK) {
            emit updateMessage("执行失败");
        }
        else {
            CGXByteBuffer rep;
            rep = reply.GetData();
            QString str;
            for (unsigned int n = 0; n < rep.GetSize(); n++) {
                QString val;
                val.sprintf("%02X", static_cast<unsigned char>(rep.GetData()[n]));
                str.append(val);
            }

            QString mark;
            mark.append("执行成功");
            mark.append("[");
            mark.append(QDateTime::currentDateTime().toString("hh:mm:ss"));
            mark.append("]");

            emit updateMessage(mark);
            emit updateMessage(str);
        }
    }

    this->comm->Close();
    delete this->comm;
    this->comm = nullptr;
    delete this->cl;
    this->cl = nullptr;

    emit finishWork();
}



Prober::Prober(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Prober) {
    ui->setupUi(this);

    this->getAvaliableSerials();

    this->on_Level_currentIndexChanged(ui->Level->currentIndex());

    QTime Time(12,0,0,0);
    QDateTime DateTime(QDate::currentDate(), Time);
    ui->Time->setDateTime(DateTime);
}

Prober::~Prober() {
    delete ui;
}

bool Prober::praseParameter(struct parameter &para) {
    if(ui->AuthKey->text().size() != 32) {ui->LOG->appendPlainText("认证密钥不正确");return false;}
    if(ui->EncryptKey->text().size() != 32) {ui->LOG->appendPlainText("加密密钥不正确");return false;}
    if(ui->Password->text().size() < 16) {ui->LOG->appendPlainText("验证密码不正确");return false;}
    para.akey.SetHexString(ui->AuthKey->text().toStdString());
    para.ekey.SetHexString(ui->EncryptKey->text().toStdString());
    para.password.SetHexString(ui->Password->text().toStdString());

    if(ui->Level->currentIndex() == 1) {
        para.level = DLMS_AUTHENTICATION_LOW;
    }
    else if(ui->Level->currentIndex() == 2) {
        para.level = DLMS_AUTHENTICATION_HIGH_GMAC;
    }
    else {
        para.level = DLMS_AUTHENTICATION_NONE;
    }

    if(ui->DedKey->isChecked()) {
        para.ded = true;
    }
    else {
        para.ded = false;
    }

    if(ui->SerialNo->currentText() == "NET") {
        para.comm.append(ui->SerialNo->currentText());
        QRegExp reg("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[\\.]){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");
        if(!reg.exactMatch(ui->IPAddress->text())) {
            ui->LOG->appendPlainText("IP不正确");
            return false;
        }
        para.ip.append(ui->IPAddress->text());
        bool ret;
        para.port = ui->Port->text().toUShort(&ret);
        if(ret != true){
            ui->LOG->appendPlainText("端口不正确");
            return false;
        }
    }
    else {
#if defined(Q_OS_WIN32)
        para.comm.append(ui->SerialNo->currentText());
#else
        para.comm.append(QString::fromStdString("/dev/") + ui->SerialNo->currentText());
#endif
        para.comm.append(":");
        switch(ui->Baudrate->currentIndex()) {
            case 0:para.comm.append("1200");para.negotiate = false;break;
            case 1:para.comm.append("2400");para.negotiate = false;break;
            case 2:para.comm.append("4800");para.negotiate = false;break;
            case 3:para.comm.append("9600");para.negotiate = false;break;
            case 4:para.comm.append("19200");para.negotiate = false;break;
            case 5:para.comm.append("38400");para.negotiate = false;break;
            case 6:para.comm.append("57600");para.negotiate = false;break;
            case 7:para.comm.append("115200");para.negotiate = false;break;
            case 8:para.comm.append("300");para.negotiate = true;break;
            default:para.comm.append("9600");para.negotiate = false;break;
        }
        para.comm.append(":8");
        switch (ui->CheckBit->currentIndex()) {
            case 0:para.comm.append("None");break;
            case 1:para.comm.append("Odd");break;
            case 2:para.comm.append("Even");break;
            default:para.comm.append("None");break;
        }
        switch (ui->StopBit->currentIndex()) {
            case 0:para.comm.append("0");break;
            case 1:para.comm.append("1");break;
            case 2:para.comm.append("2");break;
            default:para.comm.append("0");break;
        }
    }

    bool ret;
    QString val;

    para.classID = ui->ClassID->text().toUShort(&ret);if(ret != true){ui->LOG->appendPlainText("类标识不正确");return false;}

    ui->OBIS1->text().toUShort(&ret);
    if(ret != true){ui->LOG->appendPlainText("OBIS不正确");return false;}
    val.clear();
    val.sprintf("%d", static_cast<unsigned char>(ui->OBIS1->text().toUShort(&ret)));
    para.obis.append(val);
    para.obis.append(".");

    ui->OBIS2->text().toUShort(&ret);
    if(ret != true){ui->LOG->appendPlainText("OBIS不正确");return false;}
    val.clear();
    val.sprintf("%d", static_cast<unsigned char>(ui->OBIS2->text().toUShort(&ret)));
    para.obis.append(val);
    para.obis.append(".");

    ui->OBIS3->text().toUShort(&ret);
    if(ret != true){ui->LOG->appendPlainText("OBIS不正确");return false;}
    val.clear();
    val.sprintf("%d", static_cast<unsigned char>(ui->OBIS3->text().toUShort(&ret)));
    para.obis.append(val);
    para.obis.append(".");

    ui->OBIS4->text().toUShort(&ret);
    if(ret != true){ui->LOG->appendPlainText("OBIS不正确");return false;}
    val.clear();
    val.sprintf("%d", static_cast<unsigned char>(ui->OBIS4->text().toUShort(&ret)));
    para.obis.append(val);
    para.obis.append(".");

    ui->OBIS5->text().toUShort(&ret);
    if(ret != true){ui->LOG->appendPlainText("OBIS不正确");return false;}
    val.clear();
    val.sprintf("%d", static_cast<unsigned char>(ui->OBIS5->text().toUShort(&ret)));
    para.obis.append(val);
    para.obis.append(".");

    ui->OBIS6->text().toUShort(&ret);
    if(ret != true){ui->LOG->appendPlainText("OBIS不正确");return false;}
    val.clear();
    val.sprintf("%d", static_cast<unsigned char>(ui->OBIS6->text().toUShort(&ret)));
    para.obis.append(val);

    para.index = static_cast<uint8_t>(ui->Attribute->text().toUShort(&ret));
    if(ret != true){ui->LOG->appendPlainText("属性/方法不正确");return false;}

    para.client = static_cast<uint8_t>(ui->AddressClient->text().toUInt(&ret));
    if(ret != true){ui->LOG->appendPlainText("客户端地址不正确");return false;}
    para.logical = ui->AddressLogic->text().toUShort(&ret);
    if(ret != true){ui->LOG->appendPlainText("逻辑地址不正确");return false;}
    para.physical = ui->AddressReal->text().toUShort(&ret);
    if(ret != true){ui->LOG->appendPlainText("物理地址不正确");return false;}

    switch (ui->AddressMode->currentIndex()) {
        case 0:para.addressMode = 1;break;
        case 1:para.addressMode = 2;break;
        case 2:para.addressMode = 4;break;
        default:para.addressMode = 4;break;
    }

    return true;
}

void Prober::on_Level_currentIndexChanged(int index) {
    if(index == 0) {
        ui->label_7->setVisible(false);
        ui->label_8->setVisible(false);
        ui->label_9->setVisible(false);

        ui->EncryptKey->setVisible(false);
        ui->AuthKey->setVisible(false);
        ui->Password->setVisible(false);
        ui->DedKey->setVisible(false);
    }
    else if (index == 1) {
        ui->label_7->setVisible(false);
        ui->label_8->setVisible(false);
        ui->label_9->setVisible(true);

        ui->EncryptKey->setVisible(false);
        ui->AuthKey->setVisible(false);
        ui->Password->setVisible(true);
        ui->DedKey->setVisible(false);
    }
    else if (index == 2) {
        ui->label_7->setVisible(true);
        ui->label_8->setVisible(true);
        ui->label_9->setVisible(false);

        ui->EncryptKey->setVisible(true);
        ui->AuthKey->setVisible(true);
        ui->Password->setVisible(false);
        ui->DedKey->setVisible(true);
    }
}

void Prober::on_ButtonRead_pressed() {
    ui->ButtonRead->setEnabled(false);
    ui->ButtonWrite->setEnabled(false);
    ui->ButtonExecute->setEnabled(false);
    ui->LOG->clear();

    if((ui->Plain->toPlainText().remove(' ').remove('\n').remove('\r').size() % 2) != 0) {
        ui->ButtonRead->setEnabled(true);
        ui->ButtonWrite->setEnabled(true);
        ui->ButtonExecute->setEnabled(true);
        ui->LOG->appendPlainText("输入框内数据不正确");
        return;
    }

    if(this->Thread) {
        if(this->Thread->isRunning()) {
            disconnect(this->Thread, SIGNAL(updateMessage(const QString)), this, SLOT(on_updateMessage(const QString)));
            disconnect(this->Thread, SIGNAL(finishWork()), this, SLOT(on_finishWork()));
            this->Thread->terminate();
            this->Thread->wait();
        }

        delete this->Thread;
        this->Thread = nullptr;
    }

    struct parameter para;

    if(this->praseParameter(para) != true) {
        ui->ButtonRead->setEnabled(true);
        ui->ButtonWrite->setEnabled(true);
        ui->ButtonExecute->setEnabled(true);
        return;
    }

    para.motion = 1;

    this->Thread = new class Thread();
    this->Thread->init(&para, ui->Plain->toPlainText().remove(' ').remove('\n').remove('\r'));

    connect(this->Thread, SIGNAL(updateMessage(const QString)), this, SLOT(on_updateMessage(const QString)));
    connect(this->Thread, SIGNAL(finishWork()), this, SLOT(on_finishWork()));

    this->Thread->start();
}

void Prober::on_ButtonWrite_pressed() {
    ui->ButtonRead->setEnabled(false);
    ui->ButtonWrite->setEnabled(false);
    ui->ButtonExecute->setEnabled(false);
    ui->LOG->clear();

    if((ui->Plain->toPlainText().remove(' ').remove('\n').remove('\r').size() % 2) != 0) {
        ui->ButtonRead->setEnabled(true);
        ui->ButtonWrite->setEnabled(true);
        ui->ButtonExecute->setEnabled(true);
        ui->LOG->appendPlainText("输入框内数据不正确");
        return;
    }

    if(this->Thread) {
        if(this->Thread->isRunning()) {
            disconnect(this->Thread, SIGNAL(updateMessage(const QString)), this, SLOT(on_updateMessage(const QString)));
            disconnect(this->Thread, SIGNAL(finishWork()), this, SLOT(on_finishWork()));
            this->Thread->terminate();
            this->Thread->wait();
        }

        delete this->Thread;
        this->Thread = nullptr;
    }

    struct parameter para;

    if(this->praseParameter(para) != true) {
        ui->ButtonRead->setEnabled(true);
        ui->ButtonWrite->setEnabled(true);
        ui->ButtonExecute->setEnabled(true);
        return;
    }

    para.motion = 2;

    this->Thread = new class Thread();
    this->Thread->init(&para, ui->Plain->toPlainText().remove(' ').remove('\n').remove('\r'));

    connect(this->Thread, SIGNAL(updateMessage(const QString)), this, SLOT(on_updateMessage(const QString)));
    connect(this->Thread, SIGNAL(finishWork()), this, SLOT(on_finishWork()));

    this->Thread->start();
}

void Prober::on_ButtonExecute_pressed() {
    ui->ButtonRead->setEnabled(false);
    ui->ButtonWrite->setEnabled(false);
    ui->ButtonExecute->setEnabled(false);
    ui->LOG->clear();

    if((ui->Plain->toPlainText().remove(' ').remove('\n').remove('\r').size() % 2) != 0) {
        ui->ButtonRead->setEnabled(true);
        ui->ButtonWrite->setEnabled(true);
        ui->ButtonExecute->setEnabled(true);
        ui->LOG->appendPlainText("输入框内数据不正确");
        return;
    }

    if(this->Thread) {
        if(this->Thread->isRunning()) {
            disconnect(this->Thread, SIGNAL(updateMessage(const QString)), this, SLOT(on_updateMessage(const QString)));
            disconnect(this->Thread, SIGNAL(finishWork()), this, SLOT(on_finishWork()));
            this->Thread->terminate();
            this->Thread->wait();
        }

        delete this->Thread;
        this->Thread = nullptr;
    }

    struct parameter para;

    if(this->praseParameter(para) != true) {
        ui->ButtonRead->setEnabled(true);
        ui->ButtonWrite->setEnabled(true);
        ui->ButtonExecute->setEnabled(true);
        return;
    }

    para.motion = 3;

    this->Thread = new class Thread();
    this->Thread->init(&para, ui->Plain->toPlainText().remove(' ').remove('\n').remove('\r'));

    connect(this->Thread, SIGNAL(updateMessage(const QString)), this, SLOT(on_updateMessage(const QString)));
    connect(this->Thread, SIGNAL(finishWork()), this, SLOT(on_finishWork()));

    this->Thread->start();
}

void Prober::on_updateMessage(const QString arg) {
    ui->LOG->appendPlainText(arg);
}

void Prober::on_finishWork() {
    disconnect(this->Thread, SIGNAL(updateMessage(const QString)), this, SLOT(on_updateMessage(const QString)));
    disconnect(this->Thread, SIGNAL(finishWork()), this, SLOT(on_finishWork()));

    ui->ButtonRead->setEnabled(true);
    ui->ButtonWrite->setEnabled(true);
    ui->ButtonExecute->setEnabled(true);
}


void Prober::getAvaliableSerials() {
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
    ui->SerialNo->addItem(QString::fromStdString("NET"));
    for(int cnt=0; cnt<ui->SerialNo->count(); cnt++) {
        if(current == ui->SerialNo->itemText(cnt)) {
            ui->SerialNo->setCurrentIndex(cnt);
        }
    }
    if(ui->SerialNo->currentText() == "NET") {
        ui->IPAddress->setVisible(true);
        ui->Port->setVisible(true);
        ui->label_17->setVisible(true);
        ui->Baudrate->setVisible(false);
        ui->StopBit->setVisible(false);
        ui->CheckBit->setVisible(false);
        ui->label_4->setVisible(false);
        ui->label_5->setVisible(false);
        ui->label_10->setText(QString::fromStdString("   端口"));
    }
    else {
        ui->IPAddress->setVisible(false);
        ui->Port->setVisible(false);
        ui->label_17->setVisible(false);
        ui->Baudrate->setVisible(true);
        ui->StopBit->setVisible(true);
        ui->CheckBit->setVisible(true);
        ui->label_4->setVisible(true);
        ui->label_5->setVisible(true);
        ui->label_10->setText(QString::fromStdString("停止位"));
    }
}

void Prober::on_SerialNo_activated(const QString &arg1) {
#if defined(Q_OS_WIN32)
    ui->SerialNo->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->SerialNo->addItem(info.portName());
    }
    if(ui->SerialNo->count() == 0) {
        ui->SerialNo->addItem(QString::fromStdString("COM1"));
    }
    ui->SerialNo->addItem(QString::fromStdString("NET"));
    for(int cnt=0; cnt<ui->SerialNo->count(); cnt++) {
        if(arg1 == ui->SerialNo->itemText(cnt)) {
            ui->SerialNo->setCurrentIndex(cnt);
        }
    }
#else
    (void)arg1;
#endif
    if(ui->SerialNo->currentText() == "NET") {
        ui->IPAddress->setVisible(true);
        ui->Port->setVisible(true);
        ui->label_17->setVisible(true);
        ui->Baudrate->setVisible(false);
        ui->StopBit->setVisible(false);
        ui->CheckBit->setVisible(false);
        ui->label_4->setVisible(false);
        ui->label_5->setVisible(false);
        ui->label_10->setText(QString::fromStdString("   端口"));
    }
    else {
        ui->IPAddress->setVisible(false);
        ui->Port->setVisible(false);
        ui->label_17->setVisible(false);
        ui->Baudrate->setVisible(true);
        ui->StopBit->setVisible(true);
        ui->CheckBit->setVisible(true);
        ui->label_4->setVisible(true);
        ui->label_5->setVisible(true);
        ui->label_10->setText(QString::fromStdString("停止位"));
    }
}

void Prober::on_ButtonGetAddress_pressed() {
    if(this->Thread) {
        if(this->Thread->isRunning()) {
            disconnect(this->Thread, SIGNAL(updateMessage(const QString)), this, SLOT(on_updateMessage(const QString)));
            disconnect(this->Thread, SIGNAL(finishWork()), this, SLOT(on_finishWork()));
            this->Thread->terminate();
            this->Thread->wait();
        }
		delete this->Thread;
		this->Thread = nullptr;
    }

    if(Port != nullptr) {
        disconnect(Port, SIGNAL(readyRead()), this, SLOT(on_Serial_receive()));
        Port->close();
        delete Port;
        Port = nullptr;
    }

    if(Sock != nullptr) {
        disconnect(Sock, SIGNAL(readyRead()), this, SLOT(on_Sock_receive()));
        Sock->close();
        delete Sock;
        Sock = nullptr;
    }

    if(Timer != nullptr) {
        disconnect(Timer, SIGNAL(timeout()), this, SLOT(on_Timer_overflow()));
        Timer->stop();
        Timer = nullptr;
    }

    ui->ButtonRead->setEnabled(false);
    ui->ButtonWrite->setEnabled(false);
    ui->ButtonExecute->setEnabled(false);
    ui->ButtonGetAddress->setEnabled(false);

    const unsigned char packet[] = {
        0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x68, 0xaa,
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x68, 0x13, 0x00,
        0xdf, 0x16
    };

    if(ui->SerialNo->currentText() == "NET") {
        QRegExp reg("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[\\.]){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");
        if(!reg.exactMatch(ui->IPAddress->text())) {
            ui->ButtonRead->setEnabled(true);
            ui->ButtonWrite->setEnabled(true);
            ui->ButtonExecute->setEnabled(true);
            ui->ButtonGetAddress->setEnabled(true);
            ui->LOG->appendPlainText("IP不正确");
            return;
        }
        bool ret;
        ui->Port->text().toUShort(&ret);
        if(ret != true){
            ui->ButtonRead->setEnabled(true);
            ui->ButtonWrite->setEnabled(true);
            ui->ButtonExecute->setEnabled(true);
            ui->ButtonGetAddress->setEnabled(true);
            ui->LOG->appendPlainText("端口不正确");
            return;
        }
        Sock = new QTcpSocket();
        Sock->abort();
        Sock->connectToHost(ui->IPAddress->text(), ui->Port->text().toUShort());
        if(!Sock->waitForConnected(3000)) {
            ui->ButtonRead->setEnabled(true);
            ui->ButtonWrite->setEnabled(true);
            ui->ButtonExecute->setEnabled(true);
            ui->ButtonGetAddress->setEnabled(true);
            ui->LOG->appendPlainText("建立连接失败");
            return;
        }

        connect(Sock, SIGNAL(readyRead()), this, SLOT(on_Sock_receive()));

        recv.clear();
        Sock->write(reinterpret_cast<const char *>(packet), sizeof(packet));

        Timer = new QTimer(this);
        connect(Timer, SIGNAL(timeout()), this, SLOT(on_Timer_overflow()));
        Timer->start(1500);

        Sock->flush();
        ui->AddressReal->setText("");
    }
    else {
        Port = new QSerialPort();
    #if defined(Q_OS_WIN32)
        Port->setPortName(ui->SerialNo->currentText());
    #elif defined(Q_OS_LINUX)
        Port->setPortName(QString::fromStdString("/dev/") + ui->SerialNo->currentText());
    #endif
        if(!Port->open(QIODevice::ReadWrite)) {
            delete Port;
            Port = nullptr;
            ui->ButtonRead->setEnabled(true);
            ui->ButtonWrite->setEnabled(true);
            ui->ButtonExecute->setEnabled(true);
            ui->ButtonGetAddress->setEnabled(true);
            return;
        }

        Port->setDataBits(QSerialPort::Data8);
        Port->setFlowControl(QSerialPort::NoFlowControl);
        switch(ui->Baudrate->currentIndex()) {
            case 0:Port->setBaudRate(QSerialPort::Baud1200,QSerialPort::AllDirections);break;
            case 1:Port->setBaudRate(QSerialPort::Baud2400,QSerialPort::AllDirections);break;
            case 2:Port->setBaudRate(QSerialPort::Baud4800,QSerialPort::AllDirections);break;
            case 3:Port->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);break;
            case 4:Port->setBaudRate(QSerialPort::Baud19200,QSerialPort::AllDirections);break;
            case 5:Port->setBaudRate(QSerialPort::Baud38400,QSerialPort::AllDirections);break;
            case 6:Port->setBaudRate(QSerialPort::Baud57600,QSerialPort::AllDirections);break;
            case 7:Port->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);break;
            default:Port->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);break;
        }
        switch (ui->CheckBit->currentIndex()) {
            case 0:Port->setParity(QSerialPort::NoParity);break;
            case 1:Port->setParity(QSerialPort::OddParity);break;
            case 2:Port->setParity(QSerialPort::EvenParity);break;
            default:Port->setParity(QSerialPort::NoParity);break;
        }
        switch (ui->StopBit->currentIndex()) {
            case 0:Port->setStopBits(QSerialPort::OneStop);break;
            case 1:Port->setStopBits(QSerialPort::OneAndHalfStop);break;
            case 2:Port->setStopBits(QSerialPort::TwoStop);break;
            default:Port->setStopBits(QSerialPort::OneStop);break;
        }

        connect(Port, SIGNAL(readyRead()), this, SLOT(on_Serial_receive()));

        recv.clear();
        Port->write(reinterpret_cast<const char *>(packet), sizeof(packet));

        Timer = new QTimer(this);
        connect(Timer, SIGNAL(timeout()), this, SLOT(on_Timer_overflow()));
        Timer->start(1500);

        Port->flush();
        ui->AddressReal->setText("");
    }
}

void Prober::on_Serial_receive() {
    QByteArray info = Port->readAll();
    recv.append(info);
}

void Prober::on_Sock_receive() {
    QByteArray info = Sock->readAll();
    recv.append(info);
}

void Prober::on_Timer_overflow() {
    if(Port != nullptr) {
        disconnect(Port, SIGNAL(readyRead()), this, SLOT(on_Serial_receive()));
        Port->close();
        delete Port;
        Port = nullptr;
    }

    if(Sock != nullptr) {
        disconnect(Sock, SIGNAL(readyRead()), this, SLOT(on_Sock_receive()));
        Sock->close();
        delete Sock;
        Sock = nullptr;
    }

    if(Timer != nullptr) {
        disconnect(Timer, SIGNAL(timeout()), this, SLOT(on_Timer_overflow()));
        Timer->stop();
        Timer = nullptr;
    }

    if((recv.size() >= 18) && (recv.data()[recv.size() - 1] == 0x16)) {
        unsigned short add;
        add = (((recv.data()[recv.size() - 7] - 0x33) >> 4) & 0x0f) * 1000;
        add += (((recv.data()[recv.size() - 7] - 0x33) >> 0) & 0x0f) * 100;
        add += (((recv.data()[recv.size() - 8] - 0x33) >> 4) & 0x0f) * 10;
        add += (((recv.data()[recv.size() - 8] - 0x33) >> 0) & 0x0f) * 1;

        ui->AddressReal->setText(QString::number(add));
    }

    ui->ButtonRead->setEnabled(true);
    ui->ButtonWrite->setEnabled(true);
    ui->ButtonExecute->setEnabled(true);
    ui->ButtonGetAddress->setEnabled(true);
}

void Prober::on_Button2Text_pressed()
{
    QString str;

    QDateTime DateTime = ui->Time->dateTime();

    str.sprintf("%s%02X", str.toLocal8Bit().data(), 0x09);
    str.sprintf("%s%02X", str.toLocal8Bit().data(), 0x0c);

    str.sprintf("%s%c", str.toLocal8Bit().data(), ' ');

    str.sprintf("%s%02X", str.toLocal8Bit().data(), static_cast<uchar>(DateTime.date().year() / 256));
    str.sprintf("%s%02X", str.toLocal8Bit().data(), static_cast<uchar>(DateTime.date().year() % 256));
    str.sprintf("%s%02X", str.toLocal8Bit().data(), static_cast<uchar>(DateTime.date().month()));
    str.sprintf("%s%02X", str.toLocal8Bit().data(), static_cast<uchar>(DateTime.date().day()));
    str.sprintf("%s%02X", str.toLocal8Bit().data(), static_cast<uchar>(DateTime.date().dayOfWeek()));

    str.sprintf("%s%c", str.toLocal8Bit().data(), ' ');

    str.sprintf("%s%02X", str.toLocal8Bit().data(), static_cast<uchar>(DateTime.time().hour()));
    str.sprintf("%s%02X", str.toLocal8Bit().data(), static_cast<uchar>(DateTime.time().minute()));
    str.sprintf("%s%02X", str.toLocal8Bit().data(), 0x00);
    str.sprintf("%s%02X", str.toLocal8Bit().data(), 0xff);

    str.sprintf("%s%c", str.toLocal8Bit().data(), ' ');

    str.sprintf("%s%02X", str.toLocal8Bit().data(), 0x80);
    str.sprintf("%s%02X", str.toLocal8Bit().data(), 0x00);
    str.sprintf("%s%02X", str.toLocal8Bit().data(), 0xff);

    ui->Text->setText(str);
}

void Prober::on_Button2Time_pressed() {
    if(ui->Text->text() == QString::fromStdString("")) {
        return;
    }

    QString str = ui->Text->text();
    str.remove(' ');

    QStringList list = str.split("");
    if(list.size() != (28 + 2)) {
        return;
    }

    bool ret;
    uchar val[14];
    memset(val, 0, sizeof(val));
    for(int i = 0; i < (list.size() - 2); ++i) {
        if(i%2) {
            val[i/2] += list.at(i + 1).toUShort(&ret, 16);
        }
        else {
            val[i/2] += list.at(i + 1).toUShort(&ret, 16) * 16;
        }

        if(!ret) {
            return;
        }
    }

    if((val[0] != 0x09) || (val[1] != 0x0c)) {
        return;
    }

    QDate Date(((val[2] *256) + val[3]), val[4], val[5]);
    QTime Time(val[7], val[8], val[9], 0);

    QDateTime DateTime(Date, Time);
    ui->Time->setDateTime(DateTime);
    ui->Text->setText(str);
}

void Prober::on_ButtonHexDec_pressed() {
    if(ui->ButtonHexDec->text() == QString::fromStdString("十进制")) {
        if(ui->HexDec->text() != QString::fromStdString("")) {
            bool ret;
            uint val = ui->HexDec->text().toUInt(&ret, 10);
            ui->HexDec->setMaxLength(4);
            if(ret) {
                ui->HexDec->setText(QString::number(val, 16).toUpper());
            }
            else {
                ui->HexDec->setText(QString::fromStdString(""));
            }
        }
        ui->HexDec->setMaxLength(4);
        ui->ButtonHexDec->setText(QString::fromStdString("十六进制"));
    }
    else {
        if(ui->HexDec->text() != QString::fromStdString("")) {
            bool ret;
            uint val = ui->HexDec->text().toUInt(&ret, 16);
            ui->HexDec->setMaxLength(5);
            if(ret) {
                ui->HexDec->setText(QString::number(val, 10));
            }
            else {
                ui->HexDec->setText(QString::fromStdString(""));
            }
        }
        ui->HexDec->setMaxLength(5);
        ui->ButtonHexDec->setText(QString::fromStdString("十进制"));
    }
}
