#include "virtualinput.h"
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

#define KEY_ID_00               (static_cast<uint16_t>(0x0001))
#define KEY_ID_01               (static_cast<uint16_t>(0x0002))
#define KEY_ID_02               (static_cast<uint16_t>(0x0004))
#define KEY_ID_03               (static_cast<uint16_t>(0x0008))
#define KEY_ID_04               (static_cast<uint16_t>(0x0010))
#define KEY_ID_05               (static_cast<uint16_t>(0x0020))
#define KEY_ID_06               (static_cast<uint16_t>(0x0040))
#define KEY_ID_07               (static_cast<uint16_t>(0x0080))
#define KEY_ID_08               (static_cast<uint16_t>(0x0100))
#define KEY_ID_09               (static_cast<uint16_t>(0x0200))

#define KEY_ID_BACK             (static_cast<uint16_t>(0x0400))
#define KEY_ID_ENTER            (static_cast<uint16_t>(0x0800))
#define KEY_ID_PROGRAM          (static_cast<uint16_t>(0x1000))

#define KEY_ID_UP               (static_cast<uint16_t>(0x2000))
#define KEY_ID_DOWN             (static_cast<uint16_t>(0x4000))


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


VirtualInput::VirtualInput(QObject *parent) : QObject(parent)
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
    sprintf(mypid, "%ld", static_cast<long>(getpid()));
    write(fd, mypid, strlen(mypid) + 1);
#else
    hMutex = CreateMutex(nullptr, TRUE, mutexname);
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

    EmitterKeyboard = new CommEmitter(50006);
    EmitterSesors = new CommEmitter(50003);
    EmitterBattery = new CommEmitter(50005);
    EmitterRelay = new CommEmitter(50004);

    KeyData.status = KEY_NONE;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerUpdate()));

    timer->start(100);
}

VirtualInput::~VirtualInput()
{
    delete EmitterKeyboard;
    delete EmitterSesors;
    delete EmitterBattery;
    delete EmitterRelay;
}


void VirtualInput::doSomething(enum INDEX ID)
{
    if(ID != ID01)
    {
        qDebug() << "c++: doSomething() ID error";
    }

    qDebug() << "c++: doSomething() called";
    this->ui->setProperty("width", 640);
    this->ui->setProperty("height", 480);
    this->ui->setProperty("title", "VirtualInput");
}


void VirtualInput::TimerUpdate()
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

void VirtualInput::SendKeyboardData()
{
    if(KeyData.status != KEY_NONE)
    {
        EmitterKeyboard->Send(reinterpret_cast<uint8_t *>(&KeyData), sizeof(KeyData));
        KeyData.status = KEY_NONE;
    }
}

void VirtualInput::SendSensorsData()
{
    EmitterSesors->Send(reinterpret_cast<uint8_t *>(&SensorsData), sizeof(SensorsData));
}

void VirtualInput::SendBatteryData()
{
    EmitterBattery->Send(reinterpret_cast<uint8_t *>(&BatteryData), sizeof(BatteryData));
}

void VirtualInput::SendRelayData()
{
    EmitterRelay->Send(reinterpret_cast<uint8_t *>(&RelayData), sizeof(RelayData));
}


void VirtualInput::on_key_1_clicked()
{
    KeyData.id = KEY_ID_01;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void VirtualInput::on_key_2_clicked()
{
    KeyData.id = KEY_ID_02;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void VirtualInput::on_key_3_clicked()
{
    KeyData.id = KEY_ID_03;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void VirtualInput::on_key_4_clicked()
{
    KeyData.id = KEY_ID_04;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void VirtualInput::on_key_5_clicked()
{
    KeyData.id = KEY_ID_05;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void VirtualInput::on_key_6_clicked()
{
    KeyData.id = KEY_ID_06;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void VirtualInput::on_key_7_clicked()
{
    KeyData.id = KEY_ID_07;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void VirtualInput::on_key_8_clicked()
{
    KeyData.id = KEY_ID_08;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void VirtualInput::on_key_9_clicked()
{
    KeyData.id = KEY_ID_09;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void VirtualInput::on_key_0_clicked()
{
    KeyData.id = KEY_ID_00;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void VirtualInput::on_key_del_clicked()
{
    KeyData.id = KEY_ID_BACK;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void VirtualInput::on_key_enter_clicked()
{
    KeyData.id = KEY_ID_ENTER;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void VirtualInput::on_key_up_pressed()
{
    KeyData.id = KEY_ID_UP;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
    PressTime = 0;
    StartPress = 0xff;
}

void VirtualInput::on_key_up_released()
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

void VirtualInput::on_key_down_pressed()
{
    KeyData.id = KEY_ID_DOWN;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
    PressTime = 0;
    StartPress = 0xff;
}

void VirtualInput::on_key_down_released()
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

void VirtualInput::on_key_prog_clicked()
{
    KeyData.id = KEY_ID_PROGRAM;
    KeyData.status = KEY_PRESS;
    SendKeyboardData();
}

void VirtualInput::on_check_main_cover_stateChanged(int arg1)
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

void VirtualInput::on_check_sub_cover_stateChanged(int arg1)
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

void VirtualInput::on_check_bat_rtc_stateChanged(int arg1)
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

void VirtualInput::on_check_bat_backup_stateChanged(int arg1)
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

void VirtualInput::on_check_magnetic_stateChanged(int arg1)
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

void VirtualInput::on_check_relay_stateChanged(int arg1)
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
