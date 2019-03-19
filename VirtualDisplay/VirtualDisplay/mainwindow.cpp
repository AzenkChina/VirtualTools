#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtCore"
#include "comm_socket.h"
#if defined ( __linux )
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#else
#include <windows.h>
#endif
#include <math.h>
#include <time.h>

static struct __win_lcd_message lcd_message;
static CommReceiver *Receiver = nullptr;
static uint8_t unit_changed = 0;
static uint8_t recv_updated = 0;
#if defined ( __linux )
static int fd;
#else
static HANDLE hMutex;
#endif

#if defined ( __linux )
static void *ThreadRecvMail(void *arg)
#else
static DWORD CALLBACK ThreadRecvMail(PVOID pvoid)
#endif
{
    uint32_t recv_record = 0;
    int32_t readcount = 0;
#if defined ( __linux )
    (void)arg;
#else
    (void)pvoid;
#endif

    Receiver = new CommReceiver(50001);

    while(1)
    {
#if defined ( __linux )
        usleep(2000);
#else
        Sleep(2);
#endif
        readcount = Receiver->Receive(reinterpret_cast<uint8_t *>(&lcd_message), sizeof(lcd_message));
        if(readcount != sizeof(lcd_message))
        {
            if(recv_record < (3*500))
            {
                recv_record += 1;
            }
            else if(recv_record == (3*500))
            {
                recv_record += 1;
                memset(reinterpret_cast<void *>(&lcd_message), 0, sizeof(lcd_message));
                lcd_message.global = LCD_GLO_SHOW_NONE;
                lcd_message.backlight = LCD_BKL_OFF;
                recv_updated = 0xff;
            }
        }
        else
        {
            recv_record = 0;
            unit_changed = 0;
            recv_updated = 0xff;
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
#if defined ( __linux )
    const char *lock = "/tmp/virtual_display.pid";
    struct flock fl;
    char mypid[16];
#else
    LPCWSTR MutexName = L"VirtualMeter::Display";
#endif

#if defined ( __linux )
    fd = open(lock, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if(fd < 0)
    {
        printf("can't create a lock.\n");
        exit(1);
    }

    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    if(fcntl(fd, F_SETLK, &fl) != 0)
    {
        printf("Multi-instance is not allowed!\n");
        exit(0);
    }

    ftruncate(fd, 0);
    sprintf(mypid, "%ld", static_cast<long>(getpid()));
    write(fd, mypid, strlen(mypid) + 1);
#else
    hMutex = CreateMutex(nullptr, TRUE, MutexName);
    DWORD Ret = GetLastError();

    if(hMutex)
    {
        if(Ret == ERROR_ALREADY_EXISTS)
        {
            CloseHandle(hMutex);
            return;
        }
    }
    else
    {
        CloseHandle(hMutex);
        return;
    }
#endif

    ui->setupUi(this);

    lcd_message.global = LCD_GLO_SHOW_NONE;
    lcd_message.backlight = LCD_BKL_OFF;

    setAutoFillBackground(true);
    setPalette(QPalette(QColor(160,160,160)));

    ui->WindowCentral->setVisible(false);
    ui->WindowCorner->setVisible(false);

    ui->LabelL1->setVisible(false);
    ui->LabelL2->setVisible(false);
    ui->LabelL3->setVisible(false);
    ui->LabelLN->setVisible(false);
    ui->LabelPF->setVisible(false);

    ui->LabelU1->setVisible(false);
    ui->LabelU2->setVisible(false);
    ui->LabelU3->setVisible(false);

    ui->LabelI1->setVisible(false);
    ui->LabelI2->setVisible(false);
    ui->LabelI3->setVisible(false);

    ui->LabelSignal->setVisible(false);
    ui->LabelComm->setVisible(false);
    ui->LabelWarnning->setVisible(false);

    ui->LabelQuad1->setVisible(false);
    ui->LabelQuad2->setVisible(false);
    ui->LabelQuad3->setVisible(false);
    ui->LabelQuad4->setVisible(false);
    ui->LabelQuadLine1->setVisible(false);
    ui->LabelQuadLine2->setVisible(false);

    ui->LabelRate1->setVisible(false);
    ui->LabelRate2->setVisible(false);
    ui->LabelRate3->setVisible(false);
    ui->LabelRate4->setVisible(false);

    ui->LabelBatteryBackup->setVisible(false);
    ui->LabelBatteryRtc->setVisible(false);
    ui->LabelRelay->setVisible(false);

    ui->LabelTime->setVisible(false);
    ui->LabelDate->setVisible(false);

    ui->LabelPrimary->setVisible(false);
    ui->LabelSecondary->setVisible(false);

    ui->LabelNetPower->setVisible(false);
    ui->LabelAuxPower->setVisible(false);

    ui->LabelMon->setVisible(false);

    ui->LabelNow->setVisible(false);
    ui->LabelLast->setVisible(false);

    ui->LabelNeg->setVisible(false);
    ui->LabelPos->setVisible(false);
    ui->LabelTotal->setVisible(false);
    ui->LabelEnergy->setVisible(false);

    ui->LabelTarif->setVisible(false);
    ui->LabelCredit->setVisible(false);
    ui->LabelNumber->setVisible(false);

    ui->LabelUnit->setVisible(false);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerUpdate()));

    timer->start(100);

#if defined ( __linux )
    pthread_t thread;
    pthread_attr_t thread_attr;

    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&thread, &thread_attr, ThreadRecvMail, nullptr);
    pthread_attr_destroy(&thread_attr);
#else
    HANDLE hThread;
    hThread = CreateThread(nullptr, 0, ThreadRecvMail, nullptr, 0, nullptr);
    CloseHandle(hThread);
#endif
}


MainWindow::~MainWindow()
{
#if defined ( __linux )

#else
    CloseHandle(hMutex);
#endif
    delete ui;
}


void MainWindow::TimerUpdate()
{
    double Val;
    uint8_t Mul = 0;

    static uint8_t flash_record = 0;
    static uint8_t flash_on = 0;

    flash_record += 1;
    if(flash_record == 2)
    {
        flash_record = 0;
        if(flash_on)
        {
            flash_on = 0;
        }
        else
        {
            flash_on = 0xff;
        }
    }

    if(!recv_updated)
    {
        return;
    }

    recv_updated = 0;

    if(lcd_message.backlight == LCD_BKL_ON)
    {
        setPalette(QPalette(QColor(240,240,255)));
    }
    else
    {
        setPalette(QPalette(QColor(160,160,160)));
    }

    if(lcd_message.global == LCD_GLO_SHOW_NONE)
    {
        ui->WindowCentral->setVisible(false);
        ui->WindowCorner->setVisible(false);

        ui->LabelL1->setVisible(false);
        ui->LabelL2->setVisible(false);
        ui->LabelL3->setVisible(false);
        ui->LabelLN->setVisible(false);
        ui->LabelPF->setVisible(false);

        ui->LabelU1->setVisible(false);
        ui->LabelU2->setVisible(false);
        ui->LabelU3->setVisible(false);

        ui->LabelI1->setVisible(false);
        ui->LabelI2->setVisible(false);
        ui->LabelI3->setVisible(false);

        ui->LabelSignal->setVisible(false);
        ui->LabelComm->setVisible(false);
        ui->LabelWarnning->setVisible(false);

        ui->LabelQuad1->setVisible(false);
        ui->LabelQuad2->setVisible(false);
        ui->LabelQuad3->setVisible(false);
        ui->LabelQuad4->setVisible(false);
        ui->LabelQuadLine1->setVisible(false);
        ui->LabelQuadLine2->setVisible(false);

        ui->LabelRate1->setVisible(false);
        ui->LabelRate2->setVisible(false);
        ui->LabelRate3->setVisible(false);
        ui->LabelRate4->setVisible(false);

        ui->LabelBatteryBackup->setVisible(false);
        ui->LabelBatteryRtc->setVisible(false);
        ui->LabelRelay->setVisible(false);

        ui->LabelTime->setVisible(false);
        ui->LabelDate->setVisible(false);

        ui->LabelPrimary->setVisible(false);
        ui->LabelSecondary->setVisible(false);

        ui->LabelNetPower->setVisible(false);
        ui->LabelAuxPower->setVisible(false);

        ui->LabelMon->setVisible(false);

        ui->LabelNow->setVisible(false);
        ui->LabelLast->setVisible(false);

        ui->LabelNeg->setVisible(false);
        ui->LabelPos->setVisible(false);
        ui->LabelTotal->setVisible(false);
        ui->LabelEnergy->setVisible(false);

        ui->LabelTarif->setVisible(false);
        ui->LabelCredit->setVisible(false);
        ui->LabelNumber->setVisible(false);

        ui->LabelUnit->setVisible(false);

        return;
    }
    else if(lcd_message.global == LCD_GLO_SHOW_ALL)
    {
        ui->WindowCentral->setVisible(true);
        ui->WindowCorner->setVisible(true);

        ui->LabelL1->setVisible(true);
        ui->LabelL2->setVisible(true);
        ui->LabelL3->setVisible(true);
        ui->LabelLN->setVisible(true);
        ui->LabelPF->setVisible(true);

        ui->LabelU1->setVisible(true);
        ui->LabelU2->setVisible(true);
        ui->LabelU3->setVisible(true);

        ui->LabelI1->setVisible(true);
        ui->LabelI2->setVisible(true);
        ui->LabelI3->setVisible(true);
        ui->LabelI1->setText("-I1");
        ui->LabelI2->setText("-I2");
        ui->LabelI3->setText("-I3");

        ui->LabelSignal->setVisible(true);
        ui->LabelComm->setVisible(true);
        ui->LabelWarnning->setVisible(true);

        ui->LabelQuad1->setVisible(true);
        ui->LabelQuad2->setVisible(true);
        ui->LabelQuad3->setVisible(true);
        ui->LabelQuad4->setVisible(true);
        ui->LabelQuadLine1->setVisible(true);
        ui->LabelQuadLine2->setVisible(true);

        ui->LabelRate1->setVisible(true);
        ui->LabelRate2->setVisible(true);
        ui->LabelRate3->setVisible(true);
        ui->LabelRate4->setVisible(true);

        ui->LabelBatteryBackup->setVisible(true);
        ui->LabelBatteryRtc->setVisible(true);
        ui->LabelRelay->setVisible(true);

        ui->LabelTime->setVisible(true);
        ui->LabelDate->setVisible(true);

        ui->LabelPrimary->setVisible(true);
        ui->LabelSecondary->setVisible(true);

        ui->LabelNetPower->setVisible(true);
        ui->LabelAuxPower->setVisible(true);

        ui->LabelMon->setVisible(true);

        ui->LabelNow->setVisible(true);
        ui->LabelLast->setVisible(true);

        ui->LabelNeg->setVisible(true);
        ui->LabelPos->setVisible(true);
        ui->LabelTotal->setVisible(true);
        ui->LabelEnergy->setVisible(true);

        ui->LabelTarif->setVisible(true);
        ui->LabelCredit->setVisible(true);
        ui->LabelNumber->setVisible(true);

        ui->LabelUnit->setVisible(true);

        ui->WindowCentral->display(88888888);
        ui->WindowCorner->display(88888888);
        ui->LabelNumber->display(88);

        return;
    }

    if(lcd_message.label[LCD_LABEL_U1].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelU1->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_U1].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelU1->setVisible(true);
        }
        else
        {
            ui->LabelU1->setVisible(false);
        }

    }
    else
    {
        ui->LabelU1->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_U2].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelU2->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_U2].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelU2->setVisible(true);
        }
        else
        {
            ui->LabelU2->setVisible(false);
        }

    }
    else
    {
        ui->LabelU2->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_U3].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelU3->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_U3].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelU3->setVisible(true);
        }
        else
        {
            ui->LabelU3->setVisible(false);
        }

    }
    else
    {
        ui->LabelU3->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_I1].status == LCD_LAB_SHOW_ON)
    {
        if(lcd_message.label[LCD_LABEL_I1].value)
        {
            ui->LabelI1->setText("-I1");
        }
        else
        {
            ui->LabelI1->setText("I1");
        }
        ui->LabelI1->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_I1].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            if(lcd_message.label[LCD_LABEL_I1].value)
            {
                ui->LabelI1->setText("-I1");
            }
            else
            {
                ui->LabelI1->setText("I1");
            }
            ui->LabelI1->setVisible(true);
        }
        else
        {
            ui->LabelI1->setVisible(false);
        }

    }
    else
    {
        ui->LabelI1->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_I2].status == LCD_LAB_SHOW_ON)
    {
        if(lcd_message.label[LCD_LABEL_I2].value)
        {
            ui->LabelI2->setText("-I2");
        }
        else
        {
            ui->LabelI2->setText("I2");
        }
        ui->LabelI2->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_I2].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            if(lcd_message.label[LCD_LABEL_I2].value)
            {
                ui->LabelI2->setText("-I2");
            }
            else
            {
                ui->LabelI2->setText("I2");
            }
            ui->LabelI2->setVisible(true);
        }
        else
        {
            ui->LabelI2->setVisible(false);
        }

    }
    else
    {
        ui->LabelI2->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_I3].status == LCD_LAB_SHOW_ON)
    {
        if(lcd_message.label[LCD_LABEL_I3].value)
        {
            ui->LabelI3->setText("-I3");
        }
        else
        {
            ui->LabelI3->setText("I3");
        }
        ui->LabelI3->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_I3].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            if(lcd_message.label[LCD_LABEL_I3].value)
            {
                ui->LabelI3->setText("-I3");
            }
            else
            {
                ui->LabelI3->setText("I3");
            }
            ui->LabelI3->setVisible(true);
        }
        else
        {
            ui->LabelI3->setVisible(false);
        }

    }
    else
    {
        ui->LabelI3->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_L1].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelL1->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_L1].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelL1->setVisible(true);
        }
        else
        {
            ui->LabelL1->setVisible(false);
        }

    }
    else
    {
        ui->LabelL1->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_L2].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelL2->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_L2].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelL2->setVisible(true);
        }
        else
        {
            ui->LabelL2->setVisible(false);
        }

    }
    else
    {
        ui->LabelL2->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_L3].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelL3->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_L3].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelL3->setVisible(true);
        }
        else
        {
            ui->LabelL3->setVisible(false);
        }

    }
    else
    {
        ui->LabelL3->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_LN].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelLN->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_LN].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelLN->setVisible(true);
        }
        else
        {
            ui->LabelLN->setVisible(false);
        }

    }
    else
    {
        ui->LabelLN->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_PF].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelPF->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_PF].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelPF->setVisible(true);
        }
        else
        {
            ui->LabelPF->setVisible(false);
        }

    }
    else
    {
        ui->LabelPF->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_WARN].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelWarnning->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_WARN].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelWarnning->setVisible(true);
        }
        else
        {
            ui->LabelWarnning->setVisible(false);
        }

    }
    else
    {
        ui->LabelWarnning->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_COMM].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelComm->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_COMM].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelComm->setVisible(true);
        }
        else
        {
            ui->LabelComm->setVisible(false);
        }

    }
    else
    {
        ui->LabelComm->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_SIGNAL].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelSignal->setValue(lcd_message.label[LCD_LABEL_SIGNAL].value);
        ui->LabelSignal->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_SIGNAL].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelSignal->setValue(lcd_message.label[LCD_LABEL_SIGNAL].value);
            ui->LabelSignal->setVisible(true);
        }
        else
        {
            ui->LabelSignal->setVisible(false);
        }

    }
    else
    {
        ui->LabelSignal->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_PHASE].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelQuadLine1->setVisible(true);
        ui->LabelQuadLine2->setVisible(true);

        switch(lcd_message.label[LCD_LABEL_PHASE].value)
        {
            case 1:
            {
                ui->LabelQuad1->setVisible(true);
                ui->LabelQuad2->setVisible(false);
                ui->LabelQuad3->setVisible(false);
                ui->LabelQuad4->setVisible(false);
                break;
            }
            case 2:
            {
                ui->LabelQuad1->setVisible(false);
                ui->LabelQuad2->setVisible(true);
                ui->LabelQuad3->setVisible(false);
                ui->LabelQuad4->setVisible(false);
                break;
            }
            case 3:
            {
                ui->LabelQuad1->setVisible(false);
                ui->LabelQuad2->setVisible(false);
                ui->LabelQuad3->setVisible(true);
                ui->LabelQuad4->setVisible(false);
                break;
            }
            case 4:
            {
                ui->LabelQuad1->setVisible(false);
                ui->LabelQuad2->setVisible(false);
                ui->LabelQuad3->setVisible(false);
                ui->LabelQuad4->setVisible(true);
                break;
            }
        }
    }
    else if(lcd_message.label[LCD_LABEL_PHASE].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelQuadLine1->setVisible(true);
            ui->LabelQuadLine2->setVisible(true);

            switch(lcd_message.label[LCD_LABEL_PHASE].value)
            {
                case 1:
                {
                    ui->LabelQuad1->setVisible(true);
                    ui->LabelQuad2->setVisible(false);
                    ui->LabelQuad3->setVisible(false);
                    ui->LabelQuad4->setVisible(false);
                    break;
                }
                case 2:
                {
                    ui->LabelQuad1->setVisible(false);
                    ui->LabelQuad2->setVisible(true);
                    ui->LabelQuad3->setVisible(false);
                    ui->LabelQuad4->setVisible(false);
                    break;
                }
                case 3:
                {
                    ui->LabelQuad1->setVisible(false);
                    ui->LabelQuad2->setVisible(false);
                    ui->LabelQuad3->setVisible(true);
                    ui->LabelQuad4->setVisible(false);
                    break;
                }
                case 4:
                {
                    ui->LabelQuad1->setVisible(false);
                    ui->LabelQuad2->setVisible(false);
                    ui->LabelQuad3->setVisible(false);
                    ui->LabelQuad4->setVisible(true);
                    break;
                }
            }
        }
        else
        {
            ui->LabelQuadLine1->setVisible(false);
            ui->LabelQuadLine2->setVisible(false);
            ui->LabelQuad1->setVisible(false);
            ui->LabelQuad2->setVisible(false);
            ui->LabelQuad3->setVisible(false);
            ui->LabelQuad4->setVisible(false);
        }

    }
    else
    {
        ui->LabelQuadLine1->setVisible(false);
        ui->LabelQuadLine2->setVisible(false);
        ui->LabelQuad1->setVisible(false);
        ui->LabelQuad2->setVisible(false);
        ui->LabelQuad3->setVisible(false);
        ui->LabelQuad4->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_RATE].status == LCD_LAB_SHOW_ON)
    {
        switch(lcd_message.label[LCD_LABEL_RATE].value)
        {
            case 1:
            {
                ui->LabelRate1->setVisible(true);
                ui->LabelRate2->setVisible(false);
                ui->LabelRate3->setVisible(false);
                ui->LabelRate4->setVisible(false);
                break;
            }
            case 2:
            {
                ui->LabelRate1->setVisible(false);
                ui->LabelRate2->setVisible(true);
                ui->LabelRate3->setVisible(false);
                ui->LabelRate4->setVisible(false);
                break;
            }
            case 3:
            {
                ui->LabelRate1->setVisible(false);
                ui->LabelRate2->setVisible(false);
                ui->LabelRate3->setVisible(true);
                ui->LabelRate4->setVisible(false);
                break;
            }
            case 4:
            {
                ui->LabelRate1->setVisible(false);
                ui->LabelRate2->setVisible(false);
                ui->LabelRate3->setVisible(false);
                ui->LabelRate4->setVisible(true);
                break;
            }
        }
    }
    else if(lcd_message.label[LCD_LABEL_RATE].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            switch(lcd_message.label[LCD_LABEL_RATE].value)
            {
                case 1:
                {
                    ui->LabelRate1->setVisible(true);
                    ui->LabelRate2->setVisible(false);
                    ui->LabelRate3->setVisible(false);
                    ui->LabelRate4->setVisible(false);
                    break;
                }
                case 2:
                {
                    ui->LabelRate1->setVisible(false);
                    ui->LabelRate2->setVisible(true);
                    ui->LabelRate3->setVisible(false);
                    ui->LabelRate4->setVisible(false);
                    break;
                }
                case 3:
                {
                    ui->LabelRate1->setVisible(false);
                    ui->LabelRate2->setVisible(false);
                    ui->LabelRate3->setVisible(true);
                    ui->LabelRate4->setVisible(false);
                    break;
                }
                case 4:
                {
                    ui->LabelRate1->setVisible(false);
                    ui->LabelRate2->setVisible(false);
                    ui->LabelRate3->setVisible(false);
                    ui->LabelRate4->setVisible(true);
                    break;
                }
            }
        }
        else
        {
            ui->LabelRate1->setVisible(false);
            ui->LabelRate2->setVisible(false);
            ui->LabelRate3->setVisible(false);
            ui->LabelRate4->setVisible(false);
        }

    }
    else
    {
        ui->LabelRate1->setVisible(false);
        ui->LabelRate2->setVisible(false);
        ui->LabelRate3->setVisible(false);
        ui->LabelRate4->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_BATBAK].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelBatteryBackup->setValue(lcd_message.label[LCD_LABEL_BATBAK].value);
        ui->LabelBatteryBackup->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_BATBAK].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelBatteryBackup->setValue(lcd_message.label[LCD_LABEL_BATBAK].value);
            ui->LabelBatteryBackup->setVisible(true);
        }
        else
        {
            ui->LabelBatteryBackup->setVisible(false);
        }

    }
    else
    {
        ui->LabelBatteryBackup->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_BATRTC].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelBatteryRtc->setValue(lcd_message.label[LCD_LABEL_BATRTC].value);
        ui->LabelBatteryRtc->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_BATRTC].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelBatteryRtc->setValue(lcd_message.label[LCD_LABEL_BATRTC].value);
            ui->LabelBatteryRtc->setVisible(true);
        }
        else
        {
            ui->LabelBatteryRtc->setVisible(false);
        }

    }
    else
    {
        ui->LabelBatteryRtc->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_RELAY].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelRelay->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_RELAY].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelRelay->setVisible(true);
        }
        else
        {
            ui->LabelRelay->setVisible(false);
        }
    }
    else
    {
        ui->LabelRelay->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_DATE].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelDate->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_DATE].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelDate->setVisible(true);
        }
        else
        {
            ui->LabelDate->setVisible(false);
        }

    }
    else
    {
        ui->LabelDate->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_TIM].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelTime->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_TIM].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelTime->setVisible(true);
        }
        else
        {
            ui->LabelTime->setVisible(false);
        }

    }
    else
    {
        ui->LabelTime->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_SECOND].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelSecondary->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_SECOND].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelSecondary->setVisible(true);
        }
        else
        {
            ui->LabelSecondary->setVisible(false);
        }

    }
    else
    {
        ui->LabelSecondary->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_PRIM].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelPrimary->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_PRIM].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelPrimary->setVisible(true);
        }
        else
        {
            ui->LabelPrimary->setVisible(false);
        }

    }
    else
    {
        ui->LabelPrimary->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_NET].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelNetPower->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_NET].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelNetPower->setVisible(true);
        }
        else
        {
            ui->LabelNetPower->setVisible(false);
        }

    }
    else
    {
        ui->LabelNetPower->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_AUX].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelAuxPower->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_AUX].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelAuxPower->setVisible(true);
        }
        else
        {
            ui->LabelAuxPower->setVisible(false);
        }

    }
    else
    {
        ui->LabelAuxPower->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_MON].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelMon->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_MON].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelMon->setVisible(true);
        }
        else
        {
            ui->LabelMon->setVisible(false);
        }

    }
    else
    {
        ui->LabelMon->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_NOW].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelU1->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_NOW].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelNow->setVisible(true);
        }
        else
        {
            ui->LabelNow->setVisible(false);
        }

    }
    else
    {
        ui->LabelNow->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_LAST].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelLast->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_LAST].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelLast->setVisible(true);
        }
        else
        {
            ui->LabelLast->setVisible(false);
        }

    }
    else
    {
        ui->LabelLast->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_TOTAL].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelTotal->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_TOTAL].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelTotal->setVisible(true);
        }
        else
        {
            ui->LabelTotal->setVisible(false);
        }

    }
    else
    {
        ui->LabelTotal->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_NEG].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelNeg->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_NEG].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelNeg->setVisible(true);
        }
        else
        {
            ui->LabelNeg->setVisible(false);
        }

    }
    else
    {
        ui->LabelNeg->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_POS].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelPos->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_POS].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelPos->setVisible(true);
        }
        else
        {
            ui->LabelPos->setVisible(false);
        }

    }
    else
    {
        ui->LabelPos->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_TARIF].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelTarif->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_TARIF].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelTarif->setVisible(true);
        }
        else
        {
            ui->LabelTarif->setVisible(false);
        }

    }
    else
    {
        ui->LabelTarif->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_CREDIT].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelCredit->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_CREDIT].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelCredit->setVisible(true);
        }
        else
        {
            ui->LabelCredit->setVisible(false);
        }

    }
    else
    {
        ui->LabelCredit->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_ENERGY].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelEnergy->setVisible(true);
    }
    else if(lcd_message.label[LCD_LABEL_ENERGY].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelEnergy->setVisible(true);
        }
        else
        {
            ui->LabelEnergy->setVisible(false);
        }

    }
    else
    {
        ui->LabelEnergy->setVisible(false);
    }

    if(lcd_message.label[LCD_LABEL_NUMBER].status == LCD_LAB_SHOW_ON)
    {
        ui->LabelNumber->setVisible(true);
        ui->LabelNumber->display(lcd_message.label[LCD_LABEL_NUMBER].value);
    }
    else if(lcd_message.label[LCD_LABEL_NUMBER].status == LCD_LAB_SHOW_FLASH)
    {
        if(flash_on)
        {
            ui->LabelNumber->setVisible(true);
            ui->LabelNumber->display(lcd_message.label[LCD_LABEL_NUMBER].value);
        }
        else
        {
            ui->LabelNumber->setVisible(false);
        }

    }
    else
    {
        ui->LabelNumber->setVisible(false);
    }


    Mul = static_cast<uint8_t>(lcd_message.windows[LCD_WINDOW_MAIN].dot);
    ui->WindowCentral->setVisible(true);
    switch(lcd_message.windows[LCD_WINDOW_MAIN].type)
    {
        case LCD_WIN_SHOW_BIN:
        {
            Val = lcd_message.windows[LCD_WINDOW_MAIN].value.bin;
            Val /= pow(2, Mul);
            ui->WindowCentral->setMode(QLCDNumber::Bin);
            ui->WindowCentral->display(Val);
            break;
        }
        case LCD_WIN_SHOW_DEC:
        {
            Val = lcd_message.windows[LCD_WINDOW_MAIN].value.dec;
            Val /= pow(10, Mul);
            ui->WindowCentral->setMode(QLCDNumber::Dec);

            if((Val >= 1000) && (Val < 1000000))
            {
                Val /= 1000;

                if(!unit_changed)
                {
                    lcd_message.windows[LCD_WINDOW_MAIN].unit = static_cast<enum __lcd_unit>(static_cast<uint8_t>(lcd_message.windows[LCD_WINDOW_MAIN].unit) + 1);
                    unit_changed = 0xff;
                }

                ui->WindowCentral->display(Val);
            }
            else if(Val >= 1000000)
            {
                Val /= 1000000;

                if(!unit_changed)
                {
                    lcd_message.windows[LCD_WINDOW_MAIN].unit = static_cast<enum __lcd_unit>(static_cast<uint8_t>(lcd_message.windows[LCD_WINDOW_MAIN].unit) + 2);
                    unit_changed = 0xff;
                }

                ui->WindowCentral->display(Val);
            }
            else
            {
                ui->WindowCentral->display(Val);
            }

            break;
        }
        case LCD_WIN_SHOW_HEX:
        {
            Val = lcd_message.windows[LCD_WINDOW_MAIN].value.hex;
            Val /= pow(16, Mul);
            ui->WindowCentral->setMode(QLCDNumber::Hex);
            ui->WindowCentral->display(Val);
            break;
        }
        case LCD_WIN_SHOW_DATE:
        {
            struct tm *ptm;
            const time_t date = static_cast<const time_t>(lcd_message.windows[LCD_WINDOW_MAIN].value.date);

            ptm = localtime(&date);
            Val = 0;

            switch(lcd_message.windows[LCD_WINDOW_MAIN].format)
            {
                case LCD_DATE_YYMMDD:
                {
                    Val = (ptm->tm_year % 100) * 1000000;
                    Val += (ptm->tm_mon+1 % 100) * 1000;
                    Val += (ptm->tm_mday % 100) * 1;
                    break;
                }
                case LCD_DATE_DDMMYY:
                {
                    Val = (ptm->tm_year % 100) * 1;
                    Val += (ptm->tm_mon+1 % 100) * 1000;
                    Val += (ptm->tm_mday % 100) * 100000;
                    break;
                }
                case LCD_DATE_MMDDYY:
                {
                    Val = (ptm->tm_year % 100) * 1;
                    Val += (ptm->tm_mon+1 % 100) * 1000000;
                    Val += (ptm->tm_mday % 100) * 1000;
                    break;
                }
                case LCD_DATE_hhmmss:
                {
                    Val = (ptm->tm_hour % 100) * 1000000;
                    Val += (ptm->tm_min % 100) * 1000;
                    Val += (ptm->tm_sec % 100) * 1;
                    break;
                }
                case LCD_DATE_ssmmhh:
                {
                    Val = (ptm->tm_hour % 100) * 1;
                    Val += (ptm->tm_min % 100) * 1000;
                    Val += (ptm->tm_sec % 100) * 1000000;
                    break;
                }
                case LCD_DATE_mmsshh:
                {
                    Val = (ptm->tm_hour % 100) * 1;
                    Val += (ptm->tm_min % 100) * 1000000;
                    Val += (ptm->tm_sec % 100) * 1000;
                    break;
                }
                case LCD_DATE_MMDDhhmm:
                {
                    Val += (ptm->tm_mon+1 % 100) * 1000000;
                    Val += (ptm->tm_mday % 100) * 10000;
                    Val = (ptm->tm_hour % 100) * 100;
                    Val += (ptm->tm_min % 100) * 1;
                    break;
                }
                case LCD_DATE_DDMMhhmm:
                {
                    Val += (ptm->tm_mon+1 % 100) * 10000;
                    Val += (ptm->tm_mday % 100) * 1000000;
                    Val = (ptm->tm_hour % 100) * 100;
                    Val += (ptm->tm_min % 100) * 1;
                    break;
                }
                case LCD_DATE_MMDDmmhh:
                {
                    Val += (ptm->tm_mon+1 % 100) * 1000000;
                    Val += (ptm->tm_mday % 100) * 10000;
                    Val = (ptm->tm_hour % 100) * 1;
                    Val += (ptm->tm_min % 100) * 100;
                    break;
                }
                case LCD_DATE_DDMMmmhh:
                {
                    Val += (ptm->tm_mon+1 % 100) * 10000;
                    Val += (ptm->tm_mday % 100) * 1000000;
                    Val = (ptm->tm_hour % 100) * 1;
                    Val += (ptm->tm_min % 100) * 100;
                    break;
                }
            }
            ui->WindowCentral->setMode(QLCDNumber::Dec);
            ui->WindowCentral->display(Val);
            break;
        }
        case LCD_WIN_SHOW_MSG:
        {
            Val = 0xFFFFFFFF;
            ui->WindowCentral->setMode(QLCDNumber::Hex);
            ui->WindowCentral->display(Val);
            break;
        }
        case LCD_WIN_SHOW_NONE:
        {
            ui->WindowCentral->setMode(QLCDNumber::Dec);
            ui->WindowCentral->setVisible(false);
            break;
        }
        case LCD_WIN_SHOW_ALL:
        {
            ui->WindowCentral->setMode(QLCDNumber::Dec);
            ui->WindowCentral->display(88888888);
            break;
        }
    }

    ui->LabelUnit->setVisible(true);
    switch(lcd_message.windows[LCD_WINDOW_MAIN].unit)
    {
        case LCD_UNIT_NONE:
            ui->LabelUnit->setText("");
            break;
        case LCD_UNIT_V:
            ui->LabelUnit->setText("V");
            break;
        case LCD_UNIT_KV:
            ui->LabelUnit->setText("KV");
            break;
        case LCD_UNIT_MV:
            ui->LabelUnit->setText("MV");
            break;
        case LCD_UNIT_A:
            ui->LabelUnit->setText("A");
            break;
        case LCD_UNIT_KA:
            ui->LabelUnit->setText("KA");
            break;
        case LCD_UNIT_MA:
            ui->LabelUnit->setText("MA");
            break;
        case LCD_UNIT_W:
            ui->LabelUnit->setText("W");
            break;
        case LCD_UNIT_KW:
            ui->LabelUnit->setText("KW");
            break;
        case LCD_UNIT_MW:
            ui->LabelUnit->setText("MW");
            break;
        case LCD_UNIT_VAR:
            ui->LabelUnit->setText("Var");
            break;
        case LCD_UNIT_KVAR:
            ui->LabelUnit->setText("KVar");
            break;
        case LCD_UNIT_MVAR:
            ui->LabelUnit->setText("MVar");
            break;
        case LCD_UNIT_VA:
            ui->LabelUnit->setText("VA");
            break;
        case LCD_UNIT_KVA:
            ui->LabelUnit->setText("KVA");
            break;
        case LCD_UNIT_MVA:
            ui->LabelUnit->setText("MVA");
            break;
        case LCD_UNIT_WH:
            ui->LabelUnit->setText("Wh");
            break;
        case LCD_UNIT_KWH:
            ui->LabelUnit->setText("KWh");
            break;
        case LCD_UNIT_MWH:
            ui->LabelUnit->setText("MWh");
            break;
        case LCD_UNIT_VARH:
            ui->LabelUnit->setText("Varh");
            break;
        case LCD_UNIT_KVARH:
            ui->LabelUnit->setText("KVarh");
            break;
        case LCD_UNIT_MVARH:
            ui->LabelUnit->setText("MVarh");
            break;
        case LCD_UNIT_VAH:
            ui->LabelUnit->setText("VAh");
            break;
        case LCD_UNIT_KVAH:
            ui->LabelUnit->setText("KVAh");
            break;
        case LCD_UNIT_MVAH:
            ui->LabelUnit->setText("MVAh");
            break;
        case LCD_UNIT_HZ:
            ui->LabelUnit->setText("Hz");
            break;
        case LCD_UNIT_KHZ:
            ui->LabelUnit->setText("KHz");
            break;
        case LCD_UNIT_MHZ:
            ui->LabelUnit->setText("MHz");
            break;
    }

    Mul = static_cast<uint8_t>(lcd_message.windows[LCD_WINDOW_SUB].dot);
    ui->WindowCorner->setVisible(true);
    switch(lcd_message.windows[LCD_WINDOW_SUB].type)
    {
        case LCD_WIN_SHOW_BIN:
        {
            Val = lcd_message.windows[LCD_WINDOW_SUB].value.bin;
            Val /= pow(2, Mul);
            ui->WindowCorner->setMode(QLCDNumber::Bin);
            ui->WindowCorner->display(Val);
            break;
        }
        case LCD_WIN_SHOW_DEC:
        {
            Val = lcd_message.windows[LCD_WINDOW_SUB].value.dec;
            Val /= pow(10, Mul);
            ui->WindowCorner->setMode(QLCDNumber::Dec);
            ui->WindowCorner->display(Val);
            break;
        }
        case LCD_WIN_SHOW_HEX:
        {
            Val = lcd_message.windows[LCD_WINDOW_SUB].value.hex;
            Val /= pow(16, Mul);
            ui->WindowCorner->setMode(QLCDNumber::Hex);
            ui->WindowCorner->display(Val);
            break;
        }
        case LCD_WIN_SHOW_DATE:
        {
            struct tm *ptm;
            const time_t date = static_cast<const time_t>(lcd_message.windows[LCD_WINDOW_MAIN].value.date);

            ptm = localtime(&date);
            Val = 0;

            switch(lcd_message.windows[LCD_WINDOW_MAIN].format)
            {
                case LCD_DATE_YYMMDD:
                {
                    Val = (ptm->tm_year % 100) * 1000000;
                    Val += (ptm->tm_mon+1 % 100) * 1000;
                    Val += (ptm->tm_mday % 100) * 1;
                    break;
                }
                case LCD_DATE_DDMMYY:
                {
                    Val = (ptm->tm_year % 100) * 1;
                    Val += (ptm->tm_mon+1 % 100) * 1000;
                    Val += (ptm->tm_mday % 100) * 100000;
                    break;
                }
                case LCD_DATE_MMDDYY:
                {
                    Val = (ptm->tm_year % 100) * 1;
                    Val += (ptm->tm_mon+1 % 100) * 1000000;
                    Val += (ptm->tm_mday % 100) * 1000;
                    break;
                }
                case LCD_DATE_hhmmss:
                {
                    Val = (ptm->tm_hour % 100) * 1000000;
                    Val += (ptm->tm_min % 100) * 1000;
                    Val += (ptm->tm_sec % 100) * 1;
                    break;
                }
                case LCD_DATE_ssmmhh:
                {
                    Val = (ptm->tm_hour % 100) * 1;
                    Val += (ptm->tm_min % 100) * 1000;
                    Val += (ptm->tm_sec % 100) * 1000000;
                    break;
                }
                case LCD_DATE_mmsshh:
                {
                    Val = (ptm->tm_hour % 100) * 1;
                    Val += (ptm->tm_min % 100) * 1000000;
                    Val += (ptm->tm_sec % 100) * 1000;
                    break;
                }
                case LCD_DATE_MMDDhhmm:
                {
                    Val += (ptm->tm_mon+1 % 100) * 1000000;
                    Val += (ptm->tm_mday % 100) * 10000;
                    Val = (ptm->tm_hour % 100) * 100;
                    Val += (ptm->tm_min % 100) * 1;
                    break;
                }
                case LCD_DATE_DDMMhhmm:
                {
                    Val += (ptm->tm_mon+1 % 100) * 10000;
                    Val += (ptm->tm_mday % 100) * 1000000;
                    Val = (ptm->tm_hour % 100) * 100;
                    Val += (ptm->tm_min % 100) * 1;
                    break;
                }
                case LCD_DATE_MMDDmmhh:
                {
                    Val += (ptm->tm_mon+1 % 100) * 1000000;
                    Val += (ptm->tm_mday % 100) * 10000;
                    Val = (ptm->tm_hour % 100) * 1;
                    Val += (ptm->tm_min % 100) * 100;
                    break;
                }
                case LCD_DATE_DDMMmmhh:
                {
                    Val += (ptm->tm_mon+1 % 100) * 10000;
                    Val += (ptm->tm_mday % 100) * 1000000;
                    Val = (ptm->tm_hour % 100) * 1;
                    Val += (ptm->tm_min % 100) * 100;
                    break;
                }
            }
            ui->WindowCorner->setMode(QLCDNumber::Dec);
            ui->WindowCorner->display(Val);
            break;
        }
        case LCD_WIN_SHOW_MSG:
        {
            Val = 0xFFFFFFFF;
            ui->WindowCorner->setMode(QLCDNumber::Hex);
            ui->WindowCorner->display(Val);
            break;
        }
        case LCD_WIN_SHOW_NONE:
        {
            ui->WindowCorner->setMode(QLCDNumber::Dec);
            ui->WindowCorner->setVisible(false);
            break;
        }
        case LCD_WIN_SHOW_ALL:
        {
            ui->WindowCorner->setMode(QLCDNumber::Dec);
            ui->WindowCorner->display(88888888);
            break;
        }
    }
}
