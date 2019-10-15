/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
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
    QLabel *LabelTime;
    QLabel *LabelDate;
    QLabel *LabelPF;
    QLabel *LabelUnit;
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
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
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
        QIcon icon;
        icon.addFile(QString::fromUtf8("Paint.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setAutoFillBackground(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setMinimumSize(QSize(540, 200));
        centralWidget->setMaximumSize(QSize(540, 200));
        WindowCorner = new QLCDNumber(centralWidget);
        WindowCorner->setObjectName(QString::fromUtf8("WindowCorner"));
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
        WindowCentral->setObjectName(QString::fromUtf8("WindowCentral"));
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
        WindowCentral->setProperty("value", QVariant(88888888.000000000000000));
        WindowCentral->setProperty("intValue", QVariant(88888888));
        LabelRate1 = new QLabel(centralWidget);
        LabelRate1->setObjectName(QString::fromUtf8("LabelRate1"));
        LabelRate1->setGeometry(QRect(10, 50, 20, 20));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font2.setPointSize(12);
        font2.setBold(false);
        font2.setWeight(50);
        LabelRate1->setFont(font2);
        LabelRate2 = new QLabel(centralWidget);
        LabelRate2->setObjectName(QString::fromUtf8("LabelRate2"));
        LabelRate2->setGeometry(QRect(30, 50, 20, 20));
        LabelRate2->setFont(font2);
        LabelRate3 = new QLabel(centralWidget);
        LabelRate3->setObjectName(QString::fromUtf8("LabelRate3"));
        LabelRate3->setGeometry(QRect(10, 70, 20, 20));
        LabelRate3->setFont(font2);
        LabelRate4 = new QLabel(centralWidget);
        LabelRate4->setObjectName(QString::fromUtf8("LabelRate4"));
        LabelRate4->setGeometry(QRect(30, 70, 20, 20));
        LabelRate4->setFont(font2);
        LabelL1 = new QLabel(centralWidget);
        LabelL1->setObjectName(QString::fromUtf8("LabelL1"));
        LabelL1->setGeometry(QRect(60, 80, 20, 20));
        LabelL1->setFont(font2);
        LabelL2 = new QLabel(centralWidget);
        LabelL2->setObjectName(QString::fromUtf8("LabelL2"));
        LabelL2->setGeometry(QRect(90, 80, 20, 20));
        LabelL2->setFont(font2);
        LabelL3 = new QLabel(centralWidget);
        LabelL3->setObjectName(QString::fromUtf8("LabelL3"));
        LabelL3->setGeometry(QRect(120, 80, 20, 20));
        LabelL3->setFont(font2);
        LabelLN = new QLabel(centralWidget);
        LabelLN->setObjectName(QString::fromUtf8("LabelLN"));
        LabelLN->setGeometry(QRect(150, 80, 20, 20));
        LabelLN->setFont(font2);
        LabelU3 = new QLabel(centralWidget);
        LabelU3->setObjectName(QString::fromUtf8("LabelU3"));
        LabelU3->setGeometry(QRect(330, 25, 21, 21));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font3.setPointSize(10);
        font3.setBold(false);
        font3.setWeight(50);
        font3.setKerning(true);
        LabelU3->setFont(font3);
        LabelU2 = new QLabel(centralWidget);
        LabelU2->setObjectName(QString::fromUtf8("LabelU2"));
        LabelU2->setGeometry(QRect(310, 25, 20, 20));
        LabelU2->setFont(font3);
        LabelU1 = new QLabel(centralWidget);
        LabelU1->setObjectName(QString::fromUtf8("LabelU1"));
        LabelU1->setGeometry(QRect(290, 25, 20, 20));
        LabelU1->setFont(font3);
        LabelI3 = new QLabel(centralWidget);
        LabelI3->setObjectName(QString::fromUtf8("LabelI3"));
        LabelI3->setGeometry(QRect(400, 25, 20, 20));
        LabelI3->setFont(font3);
        LabelI3->setLayoutDirection(Qt::RightToLeft);
        LabelI2 = new QLabel(centralWidget);
        LabelI2->setObjectName(QString::fromUtf8("LabelI2"));
        LabelI2->setGeometry(QRect(380, 25, 20, 20));
        LabelI2->setFont(font3);
        LabelI2->setLayoutDirection(Qt::RightToLeft);
        LabelI1 = new QLabel(centralWidget);
        LabelI1->setObjectName(QString::fromUtf8("LabelI1"));
        LabelI1->setGeometry(QRect(360, 25, 20, 20));
        LabelI1->setFont(font3);
        LabelI1->setLayoutDirection(Qt::RightToLeft);
        LabelTime = new QLabel(centralWidget);
        LabelTime->setObjectName(QString::fromUtf8("LabelTime"));
        LabelTime->setGeometry(QRect(230, 80, 40, 20));
        LabelTime->setFont(font2);
        LabelDate = new QLabel(centralWidget);
        LabelDate->setObjectName(QString::fromUtf8("LabelDate"));
        LabelDate->setGeometry(QRect(270, 80, 40, 20));
        LabelDate->setFont(font2);
        LabelPF = new QLabel(centralWidget);
        LabelPF->setObjectName(QString::fromUtf8("LabelPF"));
        LabelPF->setGeometry(QRect(180, 80, 20, 20));
        LabelPF->setFont(font2);
        LabelUnit = new QLabel(centralWidget);
        LabelUnit->setObjectName(QString::fromUtf8("LabelUnit"));
        LabelUnit->setGeometry(QRect(480, 130, 50, 20));
        QFont font4;
        font4.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font4.setPointSize(12);
        font4.setBold(true);
        font4.setWeight(75);
        LabelUnit->setFont(font4);
        LabelUnit->setLayoutDirection(Qt::RightToLeft);
        LabelQuad1 = new QLabel(centralWidget);
        LabelQuad1->setObjectName(QString::fromUtf8("LabelQuad1"));
        LabelQuad1->setGeometry(QRect(510, 0, 20, 20));
        LabelQuad1->setFont(font4);
        LabelQuad4 = new QLabel(centralWidget);
        LabelQuad4->setObjectName(QString::fromUtf8("LabelQuad4"));
        LabelQuad4->setGeometry(QRect(510, 30, 20, 20));
        LabelQuad4->setFont(font4);
        LabelQuad2 = new QLabel(centralWidget);
        LabelQuad2->setObjectName(QString::fromUtf8("LabelQuad2"));
        LabelQuad2->setGeometry(QRect(480, 0, 20, 20));
        LabelQuad2->setFont(font4);
        LabelQuad3 = new QLabel(centralWidget);
        LabelQuad3->setObjectName(QString::fromUtf8("LabelQuad3"));
        LabelQuad3->setGeometry(QRect(480, 30, 20, 20));
        LabelQuad3->setFont(font4);
        LabelRelay = new QLabel(centralWidget);
        LabelRelay->setObjectName(QString::fromUtf8("LabelRelay"));
        LabelRelay->setGeometry(QRect(200, 60, 40, 15));
        QFont font5;
        font5.setBold(false);
        font5.setWeight(50);
        LabelRelay->setFont(font5);
        LabelWarnning = new QLabel(centralWidget);
        LabelWarnning->setObjectName(QString::fromUtf8("LabelWarnning"));
        LabelWarnning->setGeometry(QRect(430, 60, 20, 15));
        QFont font6;
        font6.setItalic(true);
        LabelWarnning->setFont(font6);
        LabelComm = new QLabel(centralWidget);
        LabelComm->setObjectName(QString::fromUtf8("LabelComm"));
        LabelComm->setGeometry(QRect(460, 60, 40, 15));
        LabelComm->setFont(font6);
        LabelQuadLine1 = new QFrame(centralWidget);
        LabelQuadLine1->setObjectName(QString::fromUtf8("LabelQuadLine1"));
        LabelQuadLine1->setGeometry(QRect(480, 15, 50, 20));
        LabelQuadLine1->setFrameShadow(QFrame::Plain);
        LabelQuadLine1->setFrameShape(QFrame::HLine);
        LabelQuadLine2 = new QFrame(centralWidget);
        LabelQuadLine2->setObjectName(QString::fromUtf8("LabelQuadLine2"));
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
        LabelPF->setText(QApplication::translate("MainWindow", "PF", nullptr));
        LabelUnit->setText(QString());
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
