/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLCDNumber *WindowCorner;
    QLCDNumber *WindowCentral;
    QLabel *LabelRate1;
    QLabel *LabelRate2;
    QLabel *LabelRate3;
    QLabel *LabelRate4;
    QLabel *LabelL1;
    QLabel *LabelL2;
    QLabel *LabelL3;
    QLabel *LabelLN;
    QLabel *LabelU3;
    QLabel *LabelU2;
    QLabel *LabelU1;
    QLabel *LabelI3;
    QLabel *LabelI2;
    QLabel *LabelI1;
    QLCDNumber *LabelNumber;
    QProgressBar *LabelSignal;
    QProgressBar *LabelBatteryBackup;
    QProgressBar *LabelBatteryRtc;
    QLabel *LabelTime;
    QLabel *LabelDate;
    QLabel *LabelNow;
    QLabel *LabelLast;
    QLabel *LabelMon;
    QLabel *LabelNeg;
    QLabel *LabelPos;
    QLabel *LabelTotal;
    QLabel *LabelEnergy;
    QLabel *LabelPF;
    QLabel *LabelPrimary;
    QLabel *LabelSecondary;
    QLabel *LabelNetPower;
    QLabel *LabelAuxPower;
    QLabel *LabelCredit;
    QLabel *LabelUnit;
    QLabel *LabelTarif;
    QLabel *LabelQuad1;
    QLabel *LabelQuad4;
    QLabel *LabelQuad2;
    QLabel *LabelQuad3;
    QLabel *LabelRelay;
    QLabel *LabelWarnning;
    QLabel *LabelComm;
    QFrame *LabelQuadLine1;
    QFrame *LabelQuadLine2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(540, 200);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(540, 200));
        MainWindow->setMaximumSize(QSize(540, 200));
        QFont font;
        font.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        MainWindow->setFont(font);
        MainWindow->setCursor(QCursor(Qt::ArrowCursor));
        MainWindow->setContextMenuPolicy(Qt::DefaultContextMenu);
        MainWindow->setAutoFillBackground(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setMinimumSize(QSize(540, 200));
        centralWidget->setMaximumSize(QSize(540, 200));
        WindowCorner = new QLCDNumber(centralWidget);
        WindowCorner->setObjectName(QStringLiteral("WindowCorner"));
        WindowCorner->setGeometry(QRect(10, 10, 150, 30));
        WindowCorner->setMinimumSize(QSize(150, 30));
        WindowCorner->setMaximumSize(QSize(150, 30));
        WindowCorner->setLineWidth(1);
        WindowCorner->setSmallDecimalPoint(true);
        WindowCorner->setDigitCount(8);
        WindowCorner->setMode(QLCDNumber::Dec);
        WindowCorner->setSegmentStyle(QLCDNumber::Filled);
        WindowCorner->setProperty("intValue", QVariant(88888888));
        WindowCentral = new QLCDNumber(centralWidget);
        WindowCentral->setObjectName(QStringLiteral("WindowCentral"));
        WindowCentral->setGeometry(QRect(50, 100, 420, 80));
        WindowCentral->setMinimumSize(QSize(420, 80));
        WindowCentral->setMaximumSize(QSize(420, 80));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        WindowCentral->setFont(font1);
        WindowCentral->setFrameShape(QFrame::Box);
        WindowCentral->setFrameShadow(QFrame::Raised);
        WindowCentral->setSmallDecimalPoint(true);
        WindowCentral->setDigitCount(8);
        WindowCentral->setMode(QLCDNumber::Dec);
        WindowCentral->setSegmentStyle(QLCDNumber::Filled);
        WindowCentral->setProperty("value", QVariant(8.88889e+7));
        WindowCentral->setProperty("intValue", QVariant(88888888));
        LabelRate1 = new QLabel(centralWidget);
        LabelRate1->setObjectName(QStringLiteral("LabelRate1"));
        LabelRate1->setGeometry(QRect(10, 50, 20, 20));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font2.setPointSize(12);
        font2.setBold(false);
        font2.setWeight(50);
        LabelRate1->setFont(font2);
        LabelRate2 = new QLabel(centralWidget);
        LabelRate2->setObjectName(QStringLiteral("LabelRate2"));
        LabelRate2->setGeometry(QRect(30, 50, 20, 20));
        LabelRate2->setFont(font2);
        LabelRate3 = new QLabel(centralWidget);
        LabelRate3->setObjectName(QStringLiteral("LabelRate3"));
        LabelRate3->setGeometry(QRect(10, 70, 20, 20));
        LabelRate3->setFont(font2);
        LabelRate4 = new QLabel(centralWidget);
        LabelRate4->setObjectName(QStringLiteral("LabelRate4"));
        LabelRate4->setGeometry(QRect(30, 70, 20, 20));
        LabelRate4->setFont(font2);
        LabelL1 = new QLabel(centralWidget);
        LabelL1->setObjectName(QStringLiteral("LabelL1"));
        LabelL1->setGeometry(QRect(60, 80, 20, 20));
        LabelL1->setFont(font2);
        LabelL2 = new QLabel(centralWidget);
        LabelL2->setObjectName(QStringLiteral("LabelL2"));
        LabelL2->setGeometry(QRect(90, 80, 20, 20));
        LabelL2->setFont(font2);
        LabelL3 = new QLabel(centralWidget);
        LabelL3->setObjectName(QStringLiteral("LabelL3"));
        LabelL3->setGeometry(QRect(120, 80, 20, 20));
        LabelL3->setFont(font2);
        LabelLN = new QLabel(centralWidget);
        LabelLN->setObjectName(QStringLiteral("LabelLN"));
        LabelLN->setGeometry(QRect(150, 80, 20, 20));
        LabelLN->setFont(font2);
        LabelU3 = new QLabel(centralWidget);
        LabelU3->setObjectName(QStringLiteral("LabelU3"));
        LabelU3->setGeometry(QRect(330, 25, 21, 21));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font3.setPointSize(10);
        font3.setBold(false);
        font3.setWeight(50);
        font3.setKerning(true);
        LabelU3->setFont(font3);
        LabelU2 = new QLabel(centralWidget);
        LabelU2->setObjectName(QStringLiteral("LabelU2"));
        LabelU2->setGeometry(QRect(310, 25, 20, 20));
        LabelU2->setFont(font3);
        LabelU1 = new QLabel(centralWidget);
        LabelU1->setObjectName(QStringLiteral("LabelU1"));
        LabelU1->setGeometry(QRect(290, 25, 20, 20));
        LabelU1->setFont(font3);
        LabelI3 = new QLabel(centralWidget);
        LabelI3->setObjectName(QStringLiteral("LabelI3"));
        LabelI3->setGeometry(QRect(400, 25, 20, 20));
        LabelI3->setFont(font3);
        LabelI3->setLayoutDirection(Qt::RightToLeft);
        LabelI2 = new QLabel(centralWidget);
        LabelI2->setObjectName(QStringLiteral("LabelI2"));
        LabelI2->setGeometry(QRect(380, 25, 20, 20));
        LabelI2->setFont(font3);
        LabelI2->setLayoutDirection(Qt::RightToLeft);
        LabelI1 = new QLabel(centralWidget);
        LabelI1->setObjectName(QStringLiteral("LabelI1"));
        LabelI1->setGeometry(QRect(360, 25, 20, 20));
        LabelI1->setFont(font3);
        LabelI1->setLayoutDirection(Qt::RightToLeft);
        LabelNumber = new QLCDNumber(centralWidget);
        LabelNumber->setObjectName(QStringLiteral("LabelNumber"));
        LabelNumber->setGeometry(QRect(210, 10, 41, 30));
        LabelNumber->setSmallDecimalPoint(false);
        LabelNumber->setDigitCount(2);
        LabelNumber->setProperty("intValue", QVariant(88));
        LabelSignal = new QProgressBar(centralWidget);
        LabelSignal->setObjectName(QStringLiteral("LabelSignal"));
        LabelSignal->setGeometry(QRect(430, 80, 41, 20));
        LabelSignal->setValue(100);
        LabelSignal->setTextVisible(false);
        LabelBatteryBackup = new QProgressBar(centralWidget);
        LabelBatteryBackup->setObjectName(QStringLiteral("LabelBatteryBackup"));
        LabelBatteryBackup->setGeometry(QRect(370, 80, 15, 20));
        LabelBatteryBackup->setValue(100);
        LabelBatteryBackup->setTextVisible(false);
        LabelBatteryBackup->setOrientation(Qt::Vertical);
        LabelBatteryRtc = new QProgressBar(centralWidget);
        LabelBatteryRtc->setObjectName(QStringLiteral("LabelBatteryRtc"));
        LabelBatteryRtc->setGeometry(QRect(400, 80, 15, 20));
        LabelBatteryRtc->setValue(100);
        LabelBatteryRtc->setTextVisible(false);
        LabelBatteryRtc->setOrientation(Qt::Vertical);
        LabelTime = new QLabel(centralWidget);
        LabelTime->setObjectName(QStringLiteral("LabelTime"));
        LabelTime->setGeometry(QRect(230, 80, 40, 20));
        LabelTime->setFont(font2);
        LabelDate = new QLabel(centralWidget);
        LabelDate->setObjectName(QStringLiteral("LabelDate"));
        LabelDate->setGeometry(QRect(270, 80, 40, 20));
        LabelDate->setFont(font2);
        LabelNow = new QLabel(centralWidget);
        LabelNow->setObjectName(QStringLiteral("LabelNow"));
        LabelNow->setGeometry(QRect(180, 5, 31, 20));
        QFont font4;
        font4.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font4.setPointSize(10);
        font4.setBold(false);
        font4.setWeight(50);
        LabelNow->setFont(font4);
        LabelLast = new QLabel(centralWidget);
        LabelLast->setObjectName(QStringLiteral("LabelLast"));
        LabelLast->setGeometry(QRect(180, 20, 31, 20));
        LabelLast->setFont(font4);
        LabelMon = new QLabel(centralWidget);
        LabelMon->setObjectName(QStringLiteral("LabelMon"));
        LabelMon->setGeometry(QRect(260, 5, 31, 20));
        LabelMon->setFont(font4);
        LabelNeg = new QLabel(centralWidget);
        LabelNeg->setObjectName(QStringLiteral("LabelNeg"));
        LabelNeg->setGeometry(QRect(300, 5, 31, 20));
        LabelNeg->setFont(font4);
        LabelPos = new QLabel(centralWidget);
        LabelPos->setObjectName(QStringLiteral("LabelPos"));
        LabelPos->setGeometry(QRect(330, 5, 31, 20));
        LabelPos->setFont(font4);
        LabelTotal = new QLabel(centralWidget);
        LabelTotal->setObjectName(QStringLiteral("LabelTotal"));
        LabelTotal->setGeometry(QRect(355, 5, 31, 20));
        LabelTotal->setFont(font4);
        LabelEnergy = new QLabel(centralWidget);
        LabelEnergy->setObjectName(QStringLiteral("LabelEnergy"));
        LabelEnergy->setGeometry(QRect(385, 5, 41, 20));
        LabelEnergy->setFont(font4);
        LabelPF = new QLabel(centralWidget);
        LabelPF->setObjectName(QStringLiteral("LabelPF"));
        LabelPF->setGeometry(QRect(180, 80, 20, 20));
        LabelPF->setFont(font2);
        LabelPrimary = new QLabel(centralWidget);
        LabelPrimary->setObjectName(QStringLiteral("LabelPrimary"));
        LabelPrimary->setGeometry(QRect(60, 50, 30, 20));
        LabelPrimary->setFont(font4);
        LabelSecondary = new QLabel(centralWidget);
        LabelSecondary->setObjectName(QStringLiteral("LabelSecondary"));
        LabelSecondary->setGeometry(QRect(60, 60, 30, 20));
        LabelSecondary->setFont(font4);
        LabelNetPower = new QLabel(centralWidget);
        LabelNetPower->setObjectName(QStringLiteral("LabelNetPower"));
        LabelNetPower->setGeometry(QRect(75, 50, 15, 20));
        LabelNetPower->setFont(font4);
        LabelAuxPower = new QLabel(centralWidget);
        LabelAuxPower->setObjectName(QStringLiteral("LabelAuxPower"));
        LabelAuxPower->setGeometry(QRect(75, 60, 15, 20));
        LabelAuxPower->setFont(font4);
        LabelCredit = new QLabel(centralWidget);
        LabelCredit->setObjectName(QStringLiteral("LabelCredit"));
        LabelCredit->setGeometry(QRect(480, 100, 50, 20));
        LabelCredit->setFont(font2);
        LabelCredit->setLayoutDirection(Qt::RightToLeft);
        LabelUnit = new QLabel(centralWidget);
        LabelUnit->setObjectName(QStringLiteral("LabelUnit"));
        LabelUnit->setGeometry(QRect(480, 130, 50, 20));
        QFont font5;
        font5.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font5.setPointSize(12);
        font5.setBold(true);
        font5.setWeight(75);
        LabelUnit->setFont(font5);
        LabelUnit->setLayoutDirection(Qt::RightToLeft);
        LabelTarif = new QLabel(centralWidget);
        LabelTarif->setObjectName(QStringLiteral("LabelTarif"));
        LabelTarif->setGeometry(QRect(310, 80, 40, 20));
        LabelTarif->setFont(font2);
        LabelQuad1 = new QLabel(centralWidget);
        LabelQuad1->setObjectName(QStringLiteral("LabelQuad1"));
        LabelQuad1->setGeometry(QRect(510, 0, 20, 20));
        LabelQuad1->setFont(font5);
        LabelQuad4 = new QLabel(centralWidget);
        LabelQuad4->setObjectName(QStringLiteral("LabelQuad4"));
        LabelQuad4->setGeometry(QRect(510, 30, 20, 20));
        LabelQuad4->setFont(font5);
        LabelQuad2 = new QLabel(centralWidget);
        LabelQuad2->setObjectName(QStringLiteral("LabelQuad2"));
        LabelQuad2->setGeometry(QRect(480, 0, 20, 20));
        LabelQuad2->setFont(font5);
        LabelQuad3 = new QLabel(centralWidget);
        LabelQuad3->setObjectName(QStringLiteral("LabelQuad3"));
        LabelQuad3->setGeometry(QRect(480, 30, 20, 20));
        LabelQuad3->setFont(font5);
        LabelRelay = new QLabel(centralWidget);
        LabelRelay->setObjectName(QStringLiteral("LabelRelay"));
        LabelRelay->setGeometry(QRect(200, 60, 40, 15));
        QFont font6;
        font6.setBold(false);
        font6.setWeight(50);
        LabelRelay->setFont(font6);
        LabelWarnning = new QLabel(centralWidget);
        LabelWarnning->setObjectName(QStringLiteral("LabelWarnning"));
        LabelWarnning->setGeometry(QRect(430, 60, 20, 15));
        QFont font7;
        font7.setItalic(true);
        LabelWarnning->setFont(font7);
        LabelComm = new QLabel(centralWidget);
        LabelComm->setObjectName(QStringLiteral("LabelComm"));
        LabelComm->setGeometry(QRect(460, 60, 40, 15));
        LabelComm->setFont(font7);
        LabelQuadLine1 = new QFrame(centralWidget);
        LabelQuadLine1->setObjectName(QStringLiteral("LabelQuadLine1"));
        LabelQuadLine1->setGeometry(QRect(480, 15, 50, 20));
        LabelQuadLine1->setFrameShadow(QFrame::Plain);
        LabelQuadLine1->setFrameShape(QFrame::HLine);
        LabelQuadLine2 = new QFrame(centralWidget);
        LabelQuadLine2->setObjectName(QStringLiteral("LabelQuadLine2"));
        LabelQuadLine2->setGeometry(QRect(502, 0, 3, 50));
        LabelQuadLine2->setFrameShadow(QFrame::Plain);
        LabelQuadLine2->setFrameShape(QFrame::VLine);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Display Emulater", nullptr));
        LabelRate1->setText(QApplication::translate("MainWindow", "T1", nullptr));
        LabelRate2->setText(QApplication::translate("MainWindow", "T2", nullptr));
        LabelRate3->setText(QApplication::translate("MainWindow", "T3", nullptr));
        LabelRate4->setText(QApplication::translate("MainWindow", "T4", nullptr));
        LabelL1->setText(QApplication::translate("MainWindow", "L1", nullptr));
        LabelL2->setText(QApplication::translate("MainWindow", "L2", nullptr));
        LabelL3->setText(QApplication::translate("MainWindow", "L3", nullptr));
        LabelLN->setText(QApplication::translate("MainWindow", "LN", nullptr));
        LabelU3->setText(QApplication::translate("MainWindow", "U3", nullptr));
        LabelU2->setText(QApplication::translate("MainWindow", "U2", nullptr));
        LabelU1->setText(QApplication::translate("MainWindow", "U1", nullptr));
        LabelI3->setText(QApplication::translate("MainWindow", "-I3", nullptr));
        LabelI2->setText(QApplication::translate("MainWindow", "-I2", nullptr));
        LabelI1->setText(QApplication::translate("MainWindow", "-I1", nullptr));
        LabelTime->setText(QApplication::translate("MainWindow", "Time", nullptr));
        LabelDate->setText(QApplication::translate("MainWindow", "Date", nullptr));
        LabelNow->setText(QApplication::translate("MainWindow", "Now", nullptr));
        LabelLast->setText(QApplication::translate("MainWindow", "Last", nullptr));
        LabelMon->setText(QApplication::translate("MainWindow", "Mon", nullptr));
        LabelNeg->setText(QApplication::translate("MainWindow", "Neg", nullptr));
        LabelPos->setText(QApplication::translate("MainWindow", "Pos", nullptr));
        LabelTotal->setText(QApplication::translate("MainWindow", "Tot", nullptr));
        LabelEnergy->setText(QApplication::translate("MainWindow", "Energy", nullptr));
        LabelPF->setText(QApplication::translate("MainWindow", "PF", nullptr));
        LabelPrimary->setText(QApplication::translate("MainWindow", "P", nullptr));
        LabelSecondary->setText(QApplication::translate("MainWindow", "S", nullptr));
        LabelNetPower->setText(QApplication::translate("MainWindow", "1", nullptr));
        LabelAuxPower->setText(QApplication::translate("MainWindow", "2", nullptr));
        LabelCredit->setText(QApplication::translate("MainWindow", "Credit", nullptr));
        LabelUnit->setText(QString());
        LabelTarif->setText(QApplication::translate("MainWindow", "Tarif", nullptr));
        LabelQuad1->setText(QApplication::translate("MainWindow", "I", nullptr));
        LabelQuad4->setText(QApplication::translate("MainWindow", "IV", nullptr));
        LabelQuad2->setText(QApplication::translate("MainWindow", "II", nullptr));
        LabelQuad3->setText(QApplication::translate("MainWindow", "III", nullptr));
        LabelRelay->setText(QApplication::translate("MainWindow", "OPEN", nullptr));
        LabelWarnning->setText(QApplication::translate("MainWindow", "W", nullptr));
        LabelComm->setText(QApplication::translate("MainWindow", "COM", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
