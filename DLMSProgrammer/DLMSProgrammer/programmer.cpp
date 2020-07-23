#include "programmer.h"
#include "ui_programmer.h"
#include "sha1.h"

using namespace std;


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

void Thread::init(struct parameter *para, std::vector<struct closure> *closure, bool set, bool get) {
    this->para = *para;
    this->closure = *closure;
    this->get = get;
    this->set = set;
}

void Thread::run() {
    for (uint16_t physical = this->para.physicalstart; physical <= this->para.physicalstop; physical++) {
        if(this->para.addressMode == 1) {
            if(this->para.level == DLMS_AUTHENTICATION_LOW) {
                this->cl = new CGXDLMSSecureClient(true,
                                             this->para.client,
                                             (1 << 30) | (this->para.logical << 16),
                                             this->para.level,
                                             this->para.password.ToString().data(),
                                             DLMS_INTERFACE_TYPE_HDLC);
            }
            else {
                this->cl = new CGXDLMSSecureClient(true,
                                             this->para.client,
                                             (1 << 30) | (this->para.logical << 16),
                                             this->para.level,
                                             nullptr,
                                             DLMS_INTERFACE_TYPE_HDLC);
            }
        }
        else if (this->para.addressMode == 2) {
            if(this->para.level == DLMS_AUTHENTICATION_LOW) {
                this->cl = new CGXDLMSSecureClient(true,
                                             this->para.client,
                                             (2 << 30) | (this->para.logical << 16) | (physical % 100),
                                             this->para.level,
                                             this->para.password.ToString().data(),
                                             DLMS_INTERFACE_TYPE_HDLC);
            }
            else {
                this->cl = new CGXDLMSSecureClient(true,
                                             this->para.client,
                                             (2 << 30) | (this->para.logical << 16) | (physical % 100),
                                             this->para.level,
                                             nullptr,
                                             DLMS_INTERFACE_TYPE_HDLC);
            }
        }
        else {
            if(this->para.level == DLMS_AUTHENTICATION_LOW) {
                this->cl = new CGXDLMSSecureClient(true,
                                             this->para.client,
                                             (3 << 30) | (this->para.logical << 16) | (physical % 10000),
                                             this->para.level,
                                             this->para.password.ToString().data(),
                                             DLMS_INTERFACE_TYPE_HDLC);
            }
            else {
                this->cl = new CGXDLMSSecureClient(true, this->para.client,
                                             (3 << 30) | (this->para.logical << 16) | (physical % 10000),
                                             this->para.level,
                                             nullptr,
                                             DLMS_INTERFACE_TYPE_HDLC);
            }
        }


        if(this->para.level == DLMS_AUTHENTICATION_HIGH_GMAC) {
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

        this->cl->SetAutoIncreaseInvokeID(true);

        CGXByteBuffer bb;

        bb.Clear();
        bb.SetHexString(QString::fromLocal8Bit("4446534552564552").toStdString());
        this->cl->GetCiphering()->SetSystemTitle(bb);

        bb.Clear();
        this->cl->GetCiphering()->SetDedicatedKey(bb);

        this->cl->GetCiphering()->SetAuthenticationKey(para.akey);
        this->cl->GetCiphering()->SetBlockCipherKey(para.ekey);

        this->comm = new class CGXCommunication(this->cl, 6000, GX_TRACE_LEVEL_OFF, nullptr);

        emit updateMessage("地址: " + QString::number(physical));

        int ret;
        if ((ret = this->comm->Open(para.comm.toStdString().data(), para.negotiate, 115200)) != 0) {
            delete this->cl;
            delete this->comm;
            this->cl = nullptr;
            this->comm = nullptr;
            emit updateMessage("打开串口出错");
            emit updateResult("地址: " + QString::number(physical) + " 操作失败");
            continue;
        }

        if ((ret = this->comm->InitializeConnection()) != 0) {
            this->comm->Close();

            delete this->cl;
            delete this->comm;
            this->cl = nullptr;
            this->comm = nullptr;
            emit updateMessage("初始化出错");
            emit updateResult("地址: " + QString::number(physical) + " 操作失败");
            continue;
        }

        bool phy = true;
        for(vector<struct closure>::iterator iter = this->closure.begin(); iter != this->closure.end(); iter++) {
            CGXDLMSCommon Object(iter->id, iter->obis.toStdString());
            QString result;
            CGXByteBuffer data;
            CGXReplyData reply;
            std::string value;

            result.clear();
            data.Clear();
            reply.Clear();
            value.clear();

            if(iter->data == "DATE") {
                QDateTime dt;
                QString val;
                QString date;

                date.append("0905");

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().date().year() / 256));
                date.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().date().year() % 256));
                date.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().date().month()));
                date.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().date().day()));
                date.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().date().dayOfWeek()));
                date.append(val);

                data.SetHexString(date.toStdString());
            }
            else if(iter->data == "TIME") {
                QDateTime dt;
                QString val;
                QString time;

                time.append("0904");

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().time().hour()));
                time.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().time().minute()));
                time.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().time().second()));
                time.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(0));
                time.append(val);

                data.SetHexString(time.toStdString());
            }
            else if(iter->data == "DATETIME") {
                QDateTime dt;
                QString val;
                QString datetime;

                datetime.append("090C");

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().date().year() / 256));
                datetime.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().date().year() % 256));
                datetime.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().date().month()));
                datetime.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().date().day()));
                datetime.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().date().dayOfWeek()));
                datetime.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().time().hour()));
                datetime.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().time().minute()));
                datetime.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(dt.currentDateTime().time().second()));
                datetime.append(val);

                val.clear();
                val.sprintf("%02X", static_cast<unsigned char>(0));
                datetime.append(val);

                datetime.append("800000");

                data.SetHexString(datetime.toStdString());
            }
            else {
                data.SetHexString(iter->data.toStdString());
            }

            result.append(iter->name + ": ");

            if(iter->method) {
                if(this->comm->Method(&Object, iter->index, data, reply) != DLMS_ERROR_CODE_OK) {
                    result.append("执行失败 ");
                    phy = false;
                }
                else {
                    result.append("执行成功 ");
                }
            }
            else {
                if(this->set == true) {
                    if(this->comm->Write(&Object, iter->index, data, reply) != DLMS_ERROR_CODE_OK) {
                        result.append("设置失败 ");
                        phy = false;
                    }
                    else {
                        result.append("设置成功 ");
                    }
                }
                if(this->get == true) {
                    if(this->comm->Read(&Object, iter->index, value) != DLMS_ERROR_CODE_OK) {
                        result.append("读取失败 ");
                        phy = false;
                    }
                    else {
                        QString str;
                        for (unsigned int n = 0; n < value.size(); n++) {
                            QString val;
                            val.sprintf("%02X", static_cast<unsigned char>(value.data()[n]));
                            str.append(val);
                        }

                        if(iter->data == "DATE") {
                            if(str.size() != 7*2) {
                                result.append("检查失败 ");
                                phy = false;
                            }
                            else {
                                result.append("检查成功 ");
                            }
                        }
                        else if(iter->data == "TIME") {
                            if(str.size() != 6*2) {
                                result.append("检查失败 ");
                                phy = false;
                            }
                            else {
                                result.append("检查成功 ");
                            }
                        }
                        else if(iter->data == "DATETIME") {
                            if(str.size() != 14*2) {
                                result.append("检查失败 ");
                                phy = false;
                            }
                            else {
                                result.append("检查成功 ");
                            }
                        }
                        else {
                            if(str != iter->data) {
                                result.append("检查失败 ");
                                phy = false;
                            }
                            else {
                                result.append("检查成功 ");
                            }
                        }
                    }
                }
            }

            emit updateMessage(result);
        }

        if(phy == false) {
            emit updateResult("地址: " + QString::number(physical) + " 操作失败");
        }
        else {
            emit updateResult("地址: " + QString::number(physical) + " 操作成功");
        }

        delete this->cl;
        this->comm->Close();
        delete this->comm;
        this->cl = nullptr;
        this->comm = nullptr;
    }

    emit finishWork();
}





Programmer::Programmer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Programmer) {
    ui->setupUi(this);

    this->getAvaliableSerials();

    this->on_Level_currentIndexChanged(ui->Level->currentIndex());
}

Programmer::~Programmer() {
    delete ui;
}

bool Programmer::praseParameter(struct parameter &para) {
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

    bool ret;

    para.client = static_cast<uint8_t>(ui->AddressClient->text().toUInt(&ret));
    if(ret != true){ui->LOG->appendPlainText("客户端地址不正确");return false;}
    para.logical = ui->AddressLogic->text().toUShort(&ret);
    if(ret != true){ui->LOG->appendPlainText("逻辑地址不正确");return false;}
    para.physicalstart = ui->AddressStart->text().toUShort(&ret);
    if(ret != true){ui->LOG->appendPlainText("物理地址不正确");return false;}
    para.physicalstop = ui->AddressEnd->text().toUShort(&ret);
    if(ret != true){ui->LOG->appendPlainText("物理地址不正确");return false;}
    if(para.physicalstart > para.physicalstop){ui->LOG->appendPlainText("物理地址不正确");return false;}
    switch (ui->AddressMode->currentIndex()) {
        case 0:para.addressMode = 1;break;
        case 1:para.addressMode = 2;break;
        case 2:para.addressMode = 4;break;
        default:para.addressMode = 4;break;
    }

    return true;
}

void Programmer::on_Level_currentIndexChanged(int index) {
    if(index == 0) {
        ui->label_7->setVisible(false);
        ui->label_8->setVisible(false);
        ui->label_9->setVisible(false);

        ui->EncryptKey->setVisible(false);
        ui->AuthKey->setVisible(false);
        ui->Password->setVisible(false);
    }
    else if (index == 1) {
        ui->label_7->setVisible(false);
        ui->label_8->setVisible(false);
        ui->label_9->setVisible(true);

        ui->EncryptKey->setVisible(false);
        ui->AuthKey->setVisible(false);
        ui->Password->setVisible(true);
    }
    else if (index == 2) {
        ui->label_7->setVisible(true);
        ui->label_8->setVisible(true);
        ui->label_9->setVisible(false);

        ui->EncryptKey->setVisible(true);
        ui->AuthKey->setVisible(true);
        ui->Password->setVisible(false);
    }
}

void Programmer::on_ButtonStart_pressed() {
    if(this->closure.size() <= 0) {
        return;
    }

    if(this->Thread) {
        if(this->Thread->isRunning()) {
            disconnect(this->Thread, SIGNAL(updateMessage(const QString)), this, SLOT(on_updateMessage(const QString)));
            disconnect(this->Thread, SIGNAL(updateResult(const QString)), this, SLOT(on_updateResult(const QString)));
            disconnect(this->Thread, SIGNAL(finishWork()), this, SLOT(on_finishWork()));
            this->Thread->terminate();
            this->Thread->wait();
            delete this->Thread;
            this->Thread = nullptr;

            ui->ButtonStart->setText(QString::fromStdString("开始运行"));
            return;
        }

        delete this->Thread;
        this->Thread = nullptr;
    }

    struct parameter para;
    if(this->praseParameter(para) != true) {
        return;
    }

    this->Thread = new class Thread();
    this->Thread->init(&para, &this->closure, ui->BoxSet->isChecked(), ui->BoxGet->isChecked());

    connect(this->Thread, SIGNAL(updateMessage(const QString)), this, SLOT(on_updateMessage(const QString)));
    connect(this->Thread, SIGNAL(updateResult(const QString)), this, SLOT(on_updateResult(const QString)));
    connect(this->Thread, SIGNAL(finishWork()), this, SLOT(on_finishWork()));

    ui->ButtonStart->setText(QString::fromStdString("正在运行"));

    ui->LOG->clear();
    this->Thread->start();
}

void Programmer::on_updateMessage(const QString arg) {
    ui->LOG->appendPlainText(arg);
}

void Programmer::on_updateResult(const QString arg) {
    ui->Result->appendPlainText(arg);
}

void Programmer::on_finishWork() {
    disconnect(this->Thread, SIGNAL(updateMessage(const QString)), this, SLOT(on_updateMessage(const QString)));
    disconnect(this->Thread, SIGNAL(updateResult(const QString)), this, SLOT(on_updateResult(const QString)));
    disconnect(this->Thread, SIGNAL(finishWork()), this, SLOT(on_finishWork()));

    ui->ButtonStart->setText(QString::fromStdString("开始运行"));
}

void Programmer::getAvaliableSerials() {
    QString current = ui->SerialNo->currentText();
    ui->SerialNo->clear();
#if defined(Q_OS_WIN32)
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->SerialNo->addItem(info.portName());
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

void Programmer::on_SerialNo_activated(const QString &arg1) {
#if defined(Q_OS_WIN32)
    ui->SerialNo->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->SerialNo->addItem(info.portName());
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

void Programmer::on_ButtonLoad_pressed() {
    //加载文件
    QString fileName = QFileDialog::getOpenFileName(this);

    if(fileName == nullptr) {
        ui->ButtonLoad->setText(QString::fromStdString("未加载"));
        return;
    }

    //打开文本文件
    ifstream in(fileName.toStdString());
    if(!in.is_open()) {
        ui->ButtonLoad->setText(QString::fromStdString("未加载"));
        return;
    }
    //按行读取文件
    string s = "";
    vector<string> file_line;
    while(getline(in,s)) {
        file_line.push_back(s);
    }
    in.close();

    //去除每行中的杂乱信息
    for(vector<string>::iterator iter = file_line.begin(); iter != file_line.end(); iter++) {
        //去除注释
        if(!iter->empty()) {
            if(iter->find("//") != iter->npos) {
                iter->erase(iter->find("//"));
            }
        }
        if(!iter->empty()) {
            if(iter->find("#") != iter->npos) {
                iter->erase(iter->find("#"));
            }
        }
        //替换tab为空格
        if(!iter->empty()) {
            replace(iter->begin(), iter->end(), '\t', ' ');
        }
        //去除行首空格
        if(!iter->empty()) {
            iter->erase(0,iter->find_first_not_of(" "));
        }
        //去除行尾空格
        if(!iter->empty()) {
            iter->erase(iter->find_last_not_of(" ") + 1);
        }
    }

    //去除空行
    vector<string> file_trim;
    for(vector<string>::iterator iter = file_line.begin(); iter != file_line.end(); iter++) {
        if(!iter->empty()) {
            file_trim.push_back(*iter);
        }
    }

    this->closure.clear();
    struct closure element;
    uint16_t line = 0;
    for(vector<string>::iterator iter = file_trim.begin(); iter != file_trim.end(); iter++) {
        if(iter->empty()) {
            ui->ButtonLoad->setText(QString::fromStdString("未加载"));
            ui->LOG->appendPlainText("处理空行失败");
            return;
        }
        else if((iter->find("[") == 0) && (iter->find("]") != iter->npos)) {
            //数据项名称
            if(line >= 5) {
                this->closure.push_back(element);
            }
            iter->erase(0,iter->find_first_not_of("["));
            iter->erase(iter->find_last_not_of("]") + 1);
            if(iter->empty()) {
                ui->ButtonLoad->setText(QString::fromStdString("未加载"));
                ui->LOG->appendPlainText("空的项目名称");
                return;
            }

            element.name.clear();
            element.id = 0;
            element.obis.clear();
            element.index = 0;
            element.data.clear();
            element.method = false;
            line = 0;

            element.name = QString(iter->data());
        }
        else if((iter->find("[") == 0) || (iter->find("]") != iter->npos)) {
            //任何一行都不允许出现单“[”或者单“]”
            ui->ButtonLoad->setText(QString::fromStdString("未加载"));
            ui->LOG->appendPlainText("错误的项目名称标识符“[”或者“]”");
            return;
        }
        else if(line == 1) {
            //属性或者方法
            if(iter->find("A") == 0) {
                element.method = false;
            }
            else if(iter->find("M") == 0) {
                element.method = true;
            }
            else {
                ui->ButtonLoad->setText(QString::fromStdString("未加载"));
                if(element.name.size()) {
                    ui->LOG->appendPlainText("错误的属性/方法，在：" + element.name);
                }
                else {
                    ui->LOG->appendPlainText("错误的属性/方法");
                }
                return;
            }
        }
        else if(line == 2) {
            //CLASS
            bool ret;
            element.id = QString::fromLocal8Bit(iter->data()).toUShort(&ret, 16);
            if(ret != true) {
                ui->ButtonLoad->setText(QString::fromStdString("未加载"));
                if(element.name.size()) {
                    ui->LOG->appendPlainText("错误的类标识，在：" + element.name);
                }
                else {
                    ui->LOG->appendPlainText("错误的类标识");
                }
                return;
            }
        }
        else if(line == 3) {
            //OBIS
            bool ret;
            QStringList lst = QString::fromLocal8Bit(iter->data()).split('.');
            if(lst.size() != 6) {
                ui->ButtonLoad->setText(QString::fromStdString("未加载"));
                if(element.name.size()) {
                    ui->LOG->appendPlainText("错误的OBIS，在：" + element.name);
                }
                else {
                    ui->LOG->appendPlainText("错误的OBIS");
                }
                return;
            }
            for (int i = 0; i < lst.size(); ++i) {
                uint16_t val = lst.at(i).toUShort(&ret, 16);
                if((ret != true) || (val > 255)) {
                    ui->ButtonLoad->setText(QString::fromStdString("未加载"));
                    if(element.name.size()) {
                        ui->LOG->appendPlainText("错误的OBIS，在：" + element.name);
                    }
                    else {
                        ui->LOG->appendPlainText("错误的OBIS");
                    }
                    return;
                }
                element.obis.append(QString::number(val));
                if(i != (lst.size() - 1)) {
                    element.obis.append('.');
                }
            }
        }
        else if(line == 4) {
            //INDEX
            bool ret;
            element.index = static_cast<uint8_t>(QString::fromLocal8Bit(iter->data()).toUShort(&ret, 16));
            if(ret != true) {
                ui->ButtonLoad->setText(QString::fromStdString("未加载"));
                if(element.name.size()) {
                    ui->LOG->appendPlainText("错误的索引，在：" + element.name);
                }
                else {
                    ui->LOG->appendPlainText("错误的索引");
                }
                return;
            }
        }
        else {
            //DATA
            if(*iter == "DATE") {
                element.data.clear();
                element.data.append("DATE");
            }
            else if (*iter == "TIME") {
                element.data.clear();
                element.data.append("TIME");
            }
            else if (*iter == "DATETIME") {
                element.data.clear();
                element.data.append("DATETIME");
            }
            else {
                if((element.data == "DATE") || (element.data == "TIME") || (element.data == "DATETIME")) {
                    continue;
                }

                QStringList lst = QString::fromLocal8Bit(iter->data()).split("");
                for (int i = 1; i < (lst.size() - 2); ++i) {
                    if( ((lst.at(i) < '0') || (lst.at(i) > '9')) &&
                        ((lst.at(i) < 'a') || (lst.at(i) > 'f')) &&
                        ((lst.at(i) < 'A') || (lst.at(i) > 'F'))) {
                        ui->ButtonLoad->setText(QString::fromStdString("未加载"));
                        if(element.name.size()) {
                            ui->LOG->appendPlainText("错误的数据，在：" + element.name);
                        }
                        else {
                            ui->LOG->appendPlainText("错误的数据");
                        }
                        return;
                    }
                }
                element.data.append(QString::fromLocal8Bit(iter->data()));
            }
        }

        line += 1;
    }

    if(line >= 5) {
        this->closure.push_back(element);
    }

    ui->ButtonLoad->setText(QString::fromStdString("已加载"));
    ui->LOG->clear();
    ui->Result->clear();

    int index = 0;
    SHA1_CTX ctx;
    uint8_t result[SHA1_BLOCK_SIZE];
    sha1_init(&ctx);
    for(vector<struct closure>::iterator iter = this->closure.begin(); iter != this->closure.end(); iter++) {
        index += 1;
        ui->LOG->appendPlainText(QString::number(index) + " " + iter->name);
        sha1_update(&ctx, (const uint8_t *)(iter->name.toStdString().c_str()), iter->name.toStdString().size());
        sha1_update(&ctx, (const uint8_t *)(iter->obis.toStdString().c_str()), iter->obis.toStdString().size());
        sha1_update(&ctx, (const uint8_t *)(iter->data.toStdString().c_str()), iter->data.toStdString().size());
        sha1_update(&ctx, (const uint8_t *)(QString::number(iter->id).toStdString().c_str()), QString::number(iter->id).toStdString().size());
        sha1_update(&ctx, (const uint8_t *)(QString::number(iter->index).toStdString().c_str()), QString::number(iter->index).toStdString().size());
        if(iter->method) {
            sha1_update(&ctx, (const uint8_t *)("METHOD"), strlen("METHOD"));
        }
    }
    sha1_final(&ctx, result);

    QString str;
    for (unsigned int n = 0; n < sizeof(result); n++) {
        QString val;
        val.sprintf("%02X", static_cast<unsigned char>(result[n]));
        str.append(val);
    }
    ui->LOG->appendPlainText(QString::fromStdString(""));
    ui->LOG->appendPlainText(QString::fromStdString("数据项校验：") + str);
}
