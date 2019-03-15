#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "QtCore"

#include "comm_socket.h"
#include <windows.h>
#include <string>
#include <math.h>

static uint8_t Running = 0;
static int32_t OutData[42] = {0};
static MMRESULT TimerID = 0;

enum __metering_mode
{
    SINGLE_PHASE = 0,
    THREE_PHASE_THREE,
    THREE_PHASE_FOUR,
};

struct
{
    double_t VoltagePhaseA;
    double_t VoltagePhaseB;
    double_t VoltagePhaseC;

    double_t CurrentPhaseA;
    double_t CurrentPhaseB;
    double_t CurrentPhaseC;
    double_t CurrentPhaseN;

    uint16_t AnglePhaseA;
    uint16_t AnglePhaseB;
    uint16_t AnglePhaseC;

    uint32_t HFPositive;
    uint32_t HFNegitive;

    enum __metering_mode mode;

}Configs;

struct
{
    double_t PositiveA;
    double_t PositiveB;
    double_t PositiveC;
    double_t PositiveT;

    double_t NegitiveA;
    double_t NegitiveB;
    double_t NegitiveC;
    double_t NegitiveT;

    double_t ApparentA;
    double_t ApparentB;
    double_t ApparentC;
    double_t ApparentT;

}Energy;

const static double PI = 3.14159265358;
static HANDLE hMutex;
static CommEmitter *Emitter;

static void WINAPI onTimeFunc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2)
{
    static uint32_t timing = 5;

    (void)wTimerID;
    (void)msg;
    (void)dwUser;
    (void)dwl;
    (void)dw2;

    timing += 1;

    if(Running)
    {
        double_t IncreasePositive = ((double_t)1000000 / (double_t)Configs.HFPositive);
        double_t IncreaseNegitive = ((double_t)1000000 / (double_t)Configs.HFNegitive);
        uint16_t numb;

        //mWh
        Energy.PositiveA += OutData[R_PA - 1] / 36000;
        Energy.PositiveB += OutData[R_PB - 1] / 36000;
        Energy.PositiveC += OutData[R_PC - 1] / 36000;
        Energy.PositiveT += OutData[R_PT - 1] / 36000;
        //mVarh
        Energy.NegitiveA += OutData[R_QA - 1] / 36000;
        Energy.NegitiveB += OutData[R_QB - 1] / 36000;
        Energy.NegitiveC += OutData[R_QC - 1] / 36000;
        Energy.NegitiveT += OutData[R_QT - 1] / 36000;
        //mVAh
        Energy.ApparentA += OutData[R_SA - 1] / 36000;
        Energy.ApparentB += OutData[R_SB - 1] / 36000;
        Energy.ApparentC += OutData[R_SC - 1] / 36000;
        Energy.ApparentT += OutData[R_ST - 1] / 36000;

        if(Energy.PositiveA > IncreasePositive)
        {
            numb = Energy.PositiveA / IncreasePositive;
            Energy.PositiveA -= (IncreasePositive * numb);
            OutData[R_EPA - 1] += numb;
        }

        if(Energy.PositiveB > IncreasePositive)
        {
            numb = Energy.PositiveB / IncreasePositive;
            Energy.PositiveB -= (IncreasePositive * numb);
            OutData[R_EPB - 1] += numb;
        }

        if(Energy.PositiveC > IncreasePositive)
        {
            numb = Energy.PositiveC / IncreasePositive;
            Energy.PositiveC -= (IncreasePositive * numb);
            OutData[R_EPC - 1] += numb;
        }

        if(Energy.PositiveT > IncreasePositive)
        {
            numb = Energy.PositiveT / IncreasePositive;
            Energy.PositiveT -= (IncreasePositive * numb);
            OutData[R_EPT - 1] += numb;
        }


        if(Energy.NegitiveA > IncreaseNegitive)
        {
            numb = Energy.NegitiveA / IncreaseNegitive;
            Energy.NegitiveA -= (IncreaseNegitive * numb);
            OutData[R_EQA - 1] += numb;
        }

        if(Energy.NegitiveB > IncreaseNegitive)
        {
            numb = Energy.NegitiveB / IncreaseNegitive;
            Energy.NegitiveB -= (IncreaseNegitive * numb);
            OutData[R_EQB - 1] += numb;
        }

        if(Energy.NegitiveC > IncreaseNegitive)
        {
            numb = Energy.NegitiveC / IncreaseNegitive;
            Energy.NegitiveC -= (IncreaseNegitive * numb);
            OutData[R_EQC - 1] += numb;
        }

        if(Energy.NegitiveT > IncreaseNegitive)
        {
            numb = Energy.NegitiveT / IncreaseNegitive;
            Energy.NegitiveT -= (IncreaseNegitive * numb);
            OutData[R_EQT - 1] += numb;
        }

        if(Energy.ApparentA > IncreasePositive)
        {
            numb = Energy.ApparentA / IncreasePositive;
            Energy.ApparentA -= (IncreasePositive * numb);
            OutData[R_ESA - 1] += numb;
        }

        if(Energy.ApparentB > IncreasePositive)
        {
            numb = Energy.ApparentB / IncreasePositive;
            Energy.ApparentB -= (IncreasePositive * numb);
            OutData[R_ESB - 1] += numb;
        }

        if(Energy.ApparentC > IncreasePositive)
        {
            numb = Energy.ApparentC / IncreasePositive;
            Energy.ApparentC -= (IncreasePositive * numb);
            OutData[R_ESC - 1] += numb;
        }

        if(Energy.ApparentT > IncreasePositive)
        {
            numb = Energy.ApparentT / IncreasePositive;
            Energy.ApparentT -= (IncreasePositive * numb);
            OutData[R_EST - 1] += numb;
        }
    }

    if(Emitter->Send((uint8_t *)OutData, sizeof(OutData)) == sizeof(OutData))
    {
        for(uint16_t count=0; count<(uint16_t)R_ESC; count++)
        {
            OutData[count] = 0;
        }
    }

    if(!Running)
    {
        Energy.PositiveA = 0;
        Energy.PositiveB = 0;
        Energy.PositiveC = 0;
        Energy.PositiveT = 0;
        Energy.NegitiveA = 0;
        Energy.NegitiveB = 0;
        Energy.NegitiveC = 0;
        Energy.NegitiveT = 0;
        Energy.ApparentA = 0;
        Energy.ApparentB = 0;
        Energy.ApparentC = 0;
        Energy.ApparentT = 0;
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    LPCWSTR mutexname = L"VirtualMeter::Power";
    Emitter = new CommEmitter(50002);
    memset(OutData, 0, sizeof(OutData));
    memset(&Energy, 0, sizeof(Energy));
    memset(&Configs, 0, sizeof(Configs));
    Configs.mode = THREE_PHASE_FOUR;
    Running = 0;

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

    ui->setupUi(this);

    TimerID = timeSetEvent(100, 1, (LPTIMECALLBACK)onTimeFunc, (DWORD)NULL, TIME_PERIODIC);
}

MainWindow::~MainWindow()
{
    if(hMutex != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hMutex);
    }

    if(TimerID != 0)
    {
        timeKillEvent(TimerID);
    }

    delete Emitter;
    delete ui;
}

void MainWindow::CalcutePower()
{
    QString str = "0";
    double_t val[6] = {0};

    if(Running)
    {
        val[0] = Configs.VoltagePhaseA * Configs.CurrentPhaseA;
        val[1] = Configs.VoltagePhaseB * Configs.CurrentPhaseB;
        val[2] = Configs.VoltagePhaseC * Configs.CurrentPhaseC;

        OutData[R_SA - 1] = val[0] * 1000;
        OutData[R_SB - 1] = val[1] * 1000;
        OutData[R_SC - 1] = val[2] * 1000;
        OutData[R_ST - 1] = (val[0] + val[1] + val[2]) * 1000;


        val[3] = val[0] * cos(2 * PI * Configs.AnglePhaseA / 360);
        val[4] = val[3];
        str = QString::number(val[3], 'f', 2);
        ui->PositivePowerPhaseA->setText(str);
        OutData[R_PA - 1] = val[3] * 1000;

        val[3] = val[1] * cos(2 * PI * Configs.AnglePhaseB / 360);
        val[4] += val[3];
        str = QString::number(val[3], 'f', 2);
        ui->PositivePowerPhaseB->setText(str);
        OutData[R_PB - 1] = val[3] * 1000;

        val[3] = val[2] * cos(2 * PI * Configs.AnglePhaseC / 360);
        val[4] += val[3];
        str = QString::number(val[3], 'f', 2);
        ui->PositivePowerPhaseC->setText(str);
        OutData[R_PC - 1] = val[3] * 1000;

        str = QString::number(val[4], 'f', 2);
        ui->PositivePowerTotal->setText(str);
        OutData[R_PT - 1] = val[4] * 1000;



        val[3] = val[0] * sin(2 * PI * Configs.AnglePhaseA / 360);
        val[5] = val[3];
        str = QString::number(val[3], 'f', 2);
        ui->NegitivePowerPhaseA->setText(str);
        OutData[R_QA - 1] = val[3] * 1000;

        val[3] = val[1] * sin(2 * PI * Configs.AnglePhaseB / 360);
        val[5] += val[3];
        str = QString::number(val[3], 'f', 2);
        ui->NegitivePowerPhaseB->setText(str);
        OutData[R_QB - 1] = val[3] * 1000;

        val[3] = val[2] * sin(2 * PI * Configs.AnglePhaseC / 360);
        val[5] += val[3];
        str = QString::number(val[3], 'f', 2);
        ui->NegitivePowerPhaseC->setText(str);
        OutData[R_QC - 1] = val[3] * 1000;

        str = QString::number(val[5], 'f', 2);
        ui->NegitivePowerTotal->setText(str);
        OutData[R_QT - 1] = val[5] * 1000;


        OutData[R_PFA - 1] = fabs(cos(2 * PI * Configs.AnglePhaseA / 360))  * 1000;
        OutData[R_PFB - 1] = fabs(cos(2 * PI * Configs.AnglePhaseB / 360)) * 1000;
        OutData[R_PFC - 1] = fabs(cos(2 * PI * Configs.AnglePhaseC / 360)) * 1000;
        OutData[R_PFT - 1] = fabs(cos(atan(val[5]/val[4]))) * 1000;

        OutData[R_UARMS - 1] = Configs.VoltagePhaseA * 1000;
        OutData[R_UBRMS - 1] = Configs.VoltagePhaseB * 1000;
        OutData[R_UCRMS - 1] = Configs.VoltagePhaseC * 1000;

        OutData[R_IARMS - 1] = Configs.CurrentPhaseA * 1000;
        OutData[R_IBRMS - 1] = Configs.CurrentPhaseB * 1000;
        OutData[R_ICRMS - 1] = Configs.CurrentPhaseC * 1000;
        OutData[R_ITRMS - 1] = Configs.CurrentPhaseN * 1000;

        OutData[R_PGA - 1] = Configs.AnglePhaseA * 1000;
        OutData[R_PGB - 1] = Configs.AnglePhaseB * 1000;
        OutData[R_PGC - 1] = Configs.AnglePhaseC * 1000;

        if(Configs.mode == THREE_PHASE_FOUR)
        {
            if(ui->VoltageInverse->isChecked())
            {
                OutData[R_YUAUB - 1] = 240 * 1000;
                OutData[R_YUAUC - 1] = 120 * 1000;
                OutData[R_YUBUC - 1] = 240 * 1000;
            }
            else
            {
                OutData[R_YUAUB - 1] = 120 * 1000;
                OutData[R_YUAUC - 1] = 240 * 1000;
                OutData[R_YUBUC - 1] = 120 * 1000;
            }
        }
        else if(Configs.mode == THREE_PHASE_THREE)
        {
			OutData[R_YUAUB - 1] = 120 * 1000;
			OutData[R_YUAUC - 1] = 240 * 1000;
			OutData[R_YUBUC - 1] = 120 * 1000;
        }
        else
        {
            OutData[R_YUAUB - 1] = 0;
            OutData[R_YUAUC - 1] = 0;
            OutData[R_YUBUC - 1] = 0;
        }


        OutData[R_FREQ - 1] = 50 * 1000;
    }
    else
    {
        OutData[R_EPA - 1] = 0;
        OutData[R_EPB - 1] = 0;
        OutData[R_EPC - 1] = 0;
        OutData[R_EPT - 1] = 0;
        OutData[R_EQA - 1] = 0;
        OutData[R_EQB - 1] = 0;
        OutData[R_EQC - 1] = 0;
        OutData[R_EQT - 1] = 0;
        OutData[R_ESA - 1] = 0;
        OutData[R_ESB - 1] = 0;
        OutData[R_ESC - 1] = 0;
        OutData[R_EST - 1] = 0;

        OutData[R_SA - 1] = 0;
        OutData[R_SB - 1] = 0;
        OutData[R_SC - 1] = 0;
        OutData[R_ST - 1] = 0;

        ui->PositivePowerPhaseA->setText(str);
        OutData[R_PA - 1] = 0;
        ui->PositivePowerPhaseB->setText(str);
        OutData[R_PB - 1] = 0;
        ui->PositivePowerPhaseC->setText(str);
        OutData[R_PC - 1] = 0;
        ui->PositivePowerTotal->setText(str);
        OutData[R_PT - 1] = 0;

        ui->NegitivePowerPhaseA->setText(str);
        OutData[R_QA - 1] = 0;
        ui->NegitivePowerPhaseB->setText(str);
        OutData[R_QB - 1] = 0;
        ui->NegitivePowerPhaseC->setText(str);
        OutData[R_QC - 1] = 0;
        ui->NegitivePowerTotal->setText(str);
        OutData[R_QT - 1] = 0;

        OutData[R_PFA - 1] = 0;
        OutData[R_PFB - 1] = 0;
        OutData[R_PFC - 1] = 0;
        OutData[R_PFT - 1] = 0;

        OutData[R_UARMS - 1] = 0;
        OutData[R_UBRMS - 1] = 0;
        OutData[R_UCRMS - 1] = 0;

        OutData[R_IARMS - 1] = 0;
        OutData[R_IBRMS - 1] = 0;
        OutData[R_ICRMS - 1] = 0;
        OutData[R_ITRMS - 1] = 0;

        OutData[R_PGA - 1] = 0;
        OutData[R_PGB - 1] = 0;
        OutData[R_PGC - 1] = 0;

        OutData[R_YUAUB - 1] = 0;
        OutData[R_YUAUC - 1] = 0;
        OutData[R_YUBUC - 1] = 0;

        OutData[R_FREQ - 1] = 0;
    }
}

void MainWindow::on_VoltagePhaseA_textChanged(const QString &arg1)
{
    double_t val = arg1.toDouble();
    if(val < 0)
    {
        val = -val;
    }
    Configs.VoltagePhaseA = val;

    if(arg1.indexOf('.') != -1)
    {
        QString str = QString::number(Configs.VoltagePhaseA, 'f', 3);
        ui->VoltagePhaseA->setText(str);
    }
    else
    {
        QString str = QString::number(Configs.VoltagePhaseA, 'f', 0);
        ui->VoltagePhaseA->setText(str);
    }

    CalcutePower();
}

void MainWindow::on_VoltagePhaseB_textChanged(const QString &arg1)
{
    double_t val = arg1.toDouble();
    if(val < 0)
    {
        val = -val;
    }
    Configs.VoltagePhaseB = val;

    if(arg1.indexOf('.') != -1)
    {
        QString str = QString::number(Configs.VoltagePhaseB, 'f', 3);
        ui->VoltagePhaseB->setText(str);
    }
    else
    {
        QString str = QString::number(Configs.VoltagePhaseB, 'f', 0);
        ui->VoltagePhaseB->setText(str);
    }

    CalcutePower();
}

void MainWindow::on_VoltagePhaseC_textChanged(const QString &arg1)
{
    double_t val = arg1.toDouble();
    if(val < 0)
    {
        val = -val;
    }
    Configs.VoltagePhaseC = val;

    if(arg1.indexOf('.') != -1)
    {
        QString str = QString::number(Configs.VoltagePhaseC, 'f', 3);
        ui->VoltagePhaseC->setText(str);
    }
    else
    {
        QString str = QString::number(Configs.VoltagePhaseC, 'f', 0);
        ui->VoltagePhaseC->setText(str);
    }

    CalcutePower();
}

void MainWindow::on_CurrentPhaseA_textChanged(const QString &arg1)
{
    double_t val = arg1.toDouble();
    if(val < 0)
    {
        val = -val;
    }
    Configs.CurrentPhaseA = val;

    if(arg1.indexOf('.') != -1)
    {
        QString str = QString::number(Configs.CurrentPhaseA, 'f', 3);
        ui->CurrentPhaseA->setText(str);
    }
    else
    {
        QString str = QString::number(Configs.CurrentPhaseA, 'f', 0);
        ui->CurrentPhaseA->setText(str);
    }

    CalcutePower();
}

void MainWindow::on_CurrentPhaseB_textChanged(const QString &arg1)
{
    double_t val = arg1.toDouble();
    if(val < 0)
    {
        val = -val;
    }
    Configs.CurrentPhaseB = val;

    if(arg1.indexOf('.') != -1)
    {
        QString str = QString::number(Configs.CurrentPhaseB, 'f', 3);
        ui->CurrentPhaseB->setText(str);
    }
    else
    {
        QString str = QString::number(Configs.CurrentPhaseB, 'f', 0);
        ui->CurrentPhaseB->setText(str);
    }

    CalcutePower();
}

void MainWindow::on_CurrentPhaseC_textChanged(const QString &arg1)
{
    double_t val = arg1.toDouble();
    if(val < 0)
    {
        val = -val;
    }
    Configs.CurrentPhaseC = val;

    if(arg1.indexOf('.') != -1)
    {
        QString str = QString::number(Configs.CurrentPhaseC, 'f', 3);
        ui->CurrentPhaseC->setText(str);
    }
    else
    {
        QString str = QString::number(Configs.CurrentPhaseC, 'f', 0);
        ui->CurrentPhaseC->setText(str);
    }

    CalcutePower();
}

void MainWindow::on_CurrentPhaseN_textChanged(const QString &arg1)
{
    double_t val = arg1.toDouble();
    if(val < 0)
    {
        val = -val;
    }
    Configs.CurrentPhaseN = val;

    if(arg1.indexOf('.') != -1)
    {
        QString str = QString::number(Configs.CurrentPhaseN, 'f', 3);
        ui->CurrentPhaseN->setText(str);
    }
    else
    {
        QString str = QString::number(Configs.CurrentPhaseN, 'f', 0);
        ui->CurrentPhaseN->setText(str);
    }

    CalcutePower();
}

void MainWindow::on_CurrenAngleA_textChanged(const QString &arg1)
{
    double_t val = arg1.toDouble();
    if(val < 0)
    {
        val = -val;
    }

    Configs.AnglePhaseA = val;
    Configs.AnglePhaseA %= 360;

    QString str = QString::number(Configs.AnglePhaseA, 'f', 0);
    ui->CurrenAngleA->setText(str);

    CalcutePower();
}

void MainWindow::on_CurrenAngleB_textChanged(const QString &arg1)
{
    double_t val = arg1.toDouble();
    if(val < 0)
    {
        val = -val;
    }

    Configs.AnglePhaseB = val;
    Configs.AnglePhaseB %= 360;

    QString str = QString::number(Configs.AnglePhaseB, 'f', 0);
    ui->CurrenAngleB->setText(str);

    CalcutePower();
}

void MainWindow::on_CurrenAngleC_textChanged(const QString &arg1)
{
    double_t val = arg1.toDouble();
    if(val < 0)
    {
        val = -val;
    }

    Configs.AnglePhaseC = val;
    Configs.AnglePhaseC %= 360;

    QString str = QString::number(Configs.AnglePhaseC, 'f', 0);
    ui->CurrenAngleC->setText(str);

    CalcutePower();
}

void MainWindow::on_HFPostive_textChanged(const QString &arg1)
{
    double_t val = arg1.toDouble();
    if(val < 0)
    {
        val = -val;
    }

    Configs.HFPositive = val;

    QString str = QString::number(Configs.HFPositive);
    ui->HFPostive->setText(str);
}

void MainWindow::on_HFNegitive_textChanged(const QString &arg1)
{
    double_t val = arg1.toDouble();
    if(val < 0)
    {
        val = -val;
    }

    Configs.HFNegitive = val;

    QString str = QString::number(Configs.HFNegitive);
    ui->HFNegitive->setText(str);
}

void MainWindow::on_VoltageInverse_released()
{
    CalcutePower();
}

void MainWindow::on_MetringMode_currentIndexChanged(int index)
{
    QString str = "0";
    Configs.mode = (enum __metering_mode)index;

    ui->VoltagePhaseA->setVisible(true);
    ui->VoltagePhaseB->setVisible(true);
    ui->VoltagePhaseC->setVisible(true);
    ui->CurrentPhaseA->setVisible(true);
    ui->CurrentPhaseB->setVisible(true);
    ui->CurrentPhaseC->setVisible(true);
    ui->CurrentPhaseN->setVisible(true);
    ui->CurrenAngleA->setVisible(true);
    ui->CurrenAngleB->setVisible(true);
    ui->CurrenAngleC->setVisible(true);
    ui->PositivePowerPhaseA->setVisible(true);
    ui->PositivePowerPhaseB->setVisible(true);
    ui->PositivePowerPhaseC->setVisible(true);
    ui->PositivePowerTotal->setVisible(true);
    ui->NegitivePowerPhaseA->setVisible(true);
    ui->NegitivePowerPhaseB->setVisible(true);
    ui->NegitivePowerPhaseC->setVisible(true);
    ui->NegitivePowerTotal->setVisible(true);
    ui->VoltageInverse->setVisible(true);

    switch(Configs.mode)
    {
        case SINGLE_PHASE:
        {
            ui->VoltagePhaseA->setVisible(false);
            ui->VoltagePhaseC->setVisible(false);
            ui->CurrentPhaseA->setVisible(false);
            ui->CurrentPhaseC->setVisible(false);
            ui->CurrenAngleA->setVisible(false);
            ui->CurrenAngleC->setVisible(false);
            ui->PositivePowerPhaseA->setVisible(false);
            ui->PositivePowerPhaseB->setVisible(false);
            ui->PositivePowerPhaseC->setVisible(false);
            ui->NegitivePowerPhaseA->setVisible(false);
            ui->NegitivePowerPhaseB->setVisible(false);
            ui->NegitivePowerPhaseC->setVisible(false);
            ui->VoltageInverse->setVisible(false);

            ui->VoltagePhaseA->setText(str);
            ui->VoltagePhaseC->setText(str);
            ui->CurrentPhaseA->setText(str);
            ui->CurrentPhaseC->setText(str);
            ui->CurrenAngleA->setText(str);
            ui->CurrenAngleC->setText(str);

            Configs.VoltagePhaseA = 0;
            Configs.VoltagePhaseC = 0;
            Configs.CurrentPhaseA = 0;
            Configs.CurrentPhaseC = 0;
            Configs.AnglePhaseA = 0;
            Configs.AnglePhaseC = 0;
            break;
        }
        case THREE_PHASE_THREE:
        {
            ui->VoltagePhaseB->setVisible(false);
            ui->CurrentPhaseB->setVisible(false);
            ui->CurrentPhaseN->setVisible(false);
            ui->CurrenAngleB->setVisible(false);
            ui->PositivePowerPhaseB->setVisible(false);
            ui->NegitivePowerPhaseB->setVisible(false);
            ui->VoltageInverse->setVisible(false);

            ui->VoltagePhaseB->setText(str);
            ui->CurrentPhaseB->setText(str);
            ui->CurrentPhaseN->setText(str);
            ui->CurrenAngleB->setText(str);

            Configs.VoltagePhaseB = 0;
            Configs.CurrentPhaseB = 0;
            Configs.CurrentPhaseN = 0;
            Configs.AnglePhaseB = 0;
            break;
        }
        case THREE_PHASE_FOUR:
        {
            break;
        }
    }
}

void MainWindow::on_StartButton_released()
{
    //开始按钮按下
    if(Running == 0)
    {
        Running = 0xff;
        ui->StartButton->setText(QString::fromStdString("运行中"));

        if(Configs.HFPositive < 10)
        {
            Configs.HFPositive = 10;
            QString str = QString::number(Configs.HFPositive);
            ui->HFPostive->setText(str);
        }
        if(Configs.HFNegitive < 10)
        {
            Configs.HFNegitive = 10;
            QString str = QString::number(Configs.HFNegitive);
            ui->HFNegitive->setText(str);
        }

        ui->HFPostive->setDisabled(true);
        ui->HFNegitive->setDisabled(true);
        ui->MetringMode->setDisabled(true);

        CalcutePower();
    }
    else
    {
        QString str = "0";

        Running = 0;
        ui->StartButton->setText(QString::fromStdString("已停止"));
        ui->HFPostive->setDisabled(false);
        ui->HFNegitive->setDisabled(false);
        ui->MetringMode->setDisabled(false);

        ui->PositivePowerPhaseA->setText(str);
        ui->PositivePowerPhaseB->setText(str);
        ui->PositivePowerPhaseC->setText(str);
        ui->PositivePowerTotal->setText(str);
        ui->NegitivePowerPhaseA->setText(str);
        ui->NegitivePowerPhaseB->setText(str);
        ui->NegitivePowerPhaseC->setText(str);
        ui->NegitivePowerTotal->setText(str);

        memset(OutData, 0, sizeof(OutData));
    }
}
