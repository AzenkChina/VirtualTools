#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "comm_socket.h"
#if defined ( __linux )
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#else
#include <windows.h>
#endif
#include "QtCore"


#define KEY_ID_00               ((uint16_t)0x00)
#define KEY_ID_01               ((uint16_t)0x01)
#define KEY_ID_02               ((uint16_t)0x02)
#define KEY_ID_03               ((uint16_t)0x03)
#define KEY_ID_04               ((uint16_t)0x04)
#define KEY_ID_05               ((uint16_t)0x05)
#define KEY_ID_06               ((uint16_t)0x06)
#define KEY_ID_07               ((uint16_t)0x07)
#define KEY_ID_08               ((uint16_t)0x08)
#define KEY_ID_09               ((uint16_t)0x09)

#define KEY_ID_BACK             ((uint16_t)0x0A)
#define KEY_ID_ENTER            ((uint16_t)0x0B)
#define KEY_ID_PROGRAM          ((uint16_t)0x0C)

#define KEY_ID_UP               ((uint16_t)0x10)
#define KEY_ID_DOWN             ((uint16_t)0x20)


enum __key_status
{
    KEY_NONE = 0,
    KEY_PRESS,
    KEY_RELEASE,
    KEY_LONG_PRESS,
    KEY_LONG_RELEASE,
};

enum __switch_status
{
    SWITCH_OPEN = 0,
    SWITCH_CLOSE,
    SWITCH_DONTCARE,
    SWITCH_UNKNOWN,
};

enum __battery_status
{
    BAT_FULL = 0,
    BAT_LOW,
    BAT_EMPTY,
};

enum __sensors_type
{
    SENSORS_TYPE_MAIN_COVER = 0,
    SENSORS_TYPE_SUB_COVER,
    SENSORS_TYPE_MAGNETIC,
};

enum __battery_type
{
    BATTERY_TYPE_RTC = 0,
    BATTERY_TYPE_BACKUP,
};

struct __key_data
{
    uint16_t id;
    enum __key_status status;
};

struct __battery_data
{
    enum __battery_type type;
    enum __battery_status status;
};

struct __sensors_data
{
    enum __sensors_type type;
    enum __switch_status status;
};

struct __relay_data
{
    enum __switch_status status;
};


static CommEmitter *EmitterKeyboard;
static CommEmitter *EmitterSesors;
static CommEmitter *EmitterBattery;
static CommEmitter *EmitterRelay;

#if defined ( __linux )
static int fd;
#else
static HANDLE hMutex;
#endif

static uint8_t StartPress = 0;
static uint32_t PressTime = 0;
static struct __key_data KeyData;
static struct __battery_data BatteryData;
static struct __sensors_data SensorsData;
static struct __relay_data RelayData;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
#if defined ( __linux )
    const char *lock = "/tmp/virtual_input.pid";
    struct flock fl;
    char mypid[16];
#else
    LPCWSTR mutexname = L"VirtualMeter::Input";
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
    sprintf(mypid, "%ld", (long)getpid());
    write(fd, mypid, strlen(mypid) + 1);
#else
    hMutex = CreateMutex(NULL, TRUE, mutexname);
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

    EmitterKeyboard = new CommEmitter(50006);
    EmitterSesors = new CommEmitter(50003);
    EmitterBattery = new CommEmitter(50005);
    EmitterRelay = new CommEmitter(50004);

    KeyData.status = KEY_NONE;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerUpdate()));

    timer->start(100);
}

MainWindow::~MainWindow()
{
    delete EmitterKeyboard;
    delete EmitterSesors;
    delete EmitterBattery;
    delete EmitterRelay;
    delete ui;
}

void MainWindow::TimerUpdate()
{
    if(StartPress)
    {
        if(PressTime < 10000)
        {
            PressTime += 100;
        }
    }
    else
    {
        PressTime = 0;
    }
}

void MainWindow::SendKeyboardData()
{
    if(KeyData.status != KEY_NONE)
    {
        EmitterKeyboard->Send((uint8_t *)&KeyData, sizeof(KeyData));
        KeyData.status = KEY_NONE;
    }
}

void MainWindow::SendSensorsData()
{
    EmitterSesors->Send((uint8_t *)&SensorsData, sizeof(SensorsData));
}

void MainWindow::SendBatteryData()
{
    EmitterBattery->Send((uint8_t *)&BatteryData, sizeof(BatteryData));
}

void MainWindow::SendRelayData()
{
    EmitterRelay->Send((uint8_t *)&RelayData, sizeof(RelayData));
}


void MainWindow::on_KEY_1_clicked()
{
    KeyData.id = KEY_ID_01;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void MainWindow::on_KEY_2_clicked()
{
    KeyData.id = KEY_ID_02;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void MainWindow::on_KEY_3_clicked()
{
    KeyData.id = KEY_ID_03;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void MainWindow::on_KEY_4_clicked()
{
    KeyData.id = KEY_ID_04;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void MainWindow::on_KEY_5_clicked()
{
    KeyData.id = KEY_ID_05;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void MainWindow::on_KEY_6_clicked()
{
    KeyData.id = KEY_ID_06;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void MainWindow::on_KEY_7_clicked()
{
    KeyData.id = KEY_ID_07;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void MainWindow::on_KEY_8_clicked()
{
    KeyData.id = KEY_ID_08;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void MainWindow::on_KEY_9_clicked()
{
    KeyData.id = KEY_ID_09;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void MainWindow::on_KEY_0_clicked()
{
    KeyData.id = KEY_ID_00;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void MainWindow::on_KEY_DEL_clicked()
{
    KeyData.id = KEY_ID_BACK;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void MainWindow::on_KEY_ENTER_clicked()
{
    KeyData.id = KEY_ID_ENTER;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void MainWindow::on_KEY_UP_pressed()
{
    KeyData.id = KEY_ID_UP;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
    StartPress = 0xff;
}

void MainWindow::on_KEY_UP_released()
{
    StartPress = 0;
    KeyData.id = KEY_ID_UP;
    if(PressTime >= 3000)
    {
        KeyData.status = KEY_LONG_RELEASE;
    }
    else
    {
        KeyData.status = KEY_RELEASE;
    }

    SendKeyboardData();
}

void MainWindow::on_KEY_DOWN_pressed()
{
    KeyData.id = KEY_ID_DOWN;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
    StartPress = 0xff;
}

void MainWindow::on_KEY_DOWN_released()
{
    StartPress = 0;
    KeyData.id = KEY_ID_DOWN;
    if(PressTime >= 3000)
    {
        KeyData.status = KEY_LONG_RELEASE;
    }
    else
    {
        KeyData.status = KEY_RELEASE;
    }
    SendKeyboardData();
}

void MainWindow::on_KEY_PROG_clicked()
{
    KeyData.id = KEY_ID_PROGRAM;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void MainWindow::on_B_MAIN_COVER_stateChanged(int arg1)
{
    SensorsData.type = SENSORS_TYPE_MAIN_COVER;
    if(arg1)
    {
        SensorsData.status = SWITCH_OPEN;
    }
    else
    {
        SensorsData.status = SWITCH_CLOSE;
    }
    SendSensorsData();
}

void MainWindow::on_B_SUB_COVER_stateChanged(int arg1)
{
    SensorsData.type = SENSORS_TYPE_SUB_COVER;
    if(arg1)
    {
        SensorsData.status = SWITCH_OPEN;
    }
    else
    {
        SensorsData.status = SWITCH_CLOSE;
    }
    SendSensorsData();
}

void MainWindow::on_B_BAT_RTC_stateChanged(int arg1)
{
    BatteryData.type = BATTERY_TYPE_RTC;
    if(arg1)
    {
        BatteryData.status = BAT_EMPTY;
    }
    else
    {
        BatteryData.status = BAT_FULL;
    }
    SendBatteryData();
}

void MainWindow::on_B_BAT_BACKUP_stateChanged(int arg1)
{
    BatteryData.type = BATTERY_TYPE_BACKUP;
    if(arg1)
    {
        BatteryData.status = BAT_EMPTY;
    }
    else
    {
        BatteryData.status = BAT_FULL;
    }
    SendBatteryData();
}

void MainWindow::on_B_MAGNETIC_stateChanged(int arg1)
{
    SensorsData.type = SENSORS_TYPE_MAGNETIC;
    if(arg1)
    {
        SensorsData.status = SWITCH_OPEN;
    }
    else
    {
        SensorsData.status = SWITCH_CLOSE;
    }
    SendSensorsData();
}

void MainWindow::on_B_RELAY_stateChanged(int arg1)
{
    if(arg1)
    {
        RelayData.status = SWITCH_UNKNOWN;
    }
    else
    {
        RelayData.status = SWITCH_DONTCARE;
    }
    SendRelayData();
}
