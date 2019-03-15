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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *StartButton;
    QLineEdit *VoltagePhaseA;
    QLineEdit *VoltagePhaseB;
    QLineEdit *VoltagePhaseC;
    QLineEdit *CurrentPhaseA;
    QLineEdit *CurrentPhaseB;
    QLineEdit *CurrentPhaseC;
    QLineEdit *CurrentPhaseN;
    QLineEdit *CurrenAngleA;
    QLineEdit *CurrenAngleB;
    QLineEdit *CurrenAngleC;
    QComboBox *MetringMode;
    QLineEdit *HFPostive;
    QLineEdit *HFNegitive;
    QLineEdit *PositivePowerPhaseA;
    QLineEdit *PositivePowerPhaseB;
    QLineEdit *PositivePowerPhaseC;
    QLineEdit *NegitivePowerPhaseA;
    QLineEdit *NegitivePowerPhaseB;
    QLineEdit *NegitivePowerPhaseC;
    QFrame *line;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *PositivePowerTotal;
    QLineEdit *NegitivePowerTotal;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QFrame *line_2;
    QFrame *line_3;
    QCheckBox *VoltageInverse;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(480, 400);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(480, 400));
        MainWindow->setMaximumSize(QSize(480, 400));
        QFont font;
        font.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font.setPointSize(12);
        MainWindow->setFont(font);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        StartButton = new QPushButton(centralWidget);
        StartButton->setObjectName(QStringLiteral("StartButton"));
        StartButton->setGeometry(QRect(320, 330, 140, 50));
        VoltagePhaseA = new QLineEdit(centralWidget);
        VoltagePhaseA->setObjectName(QStringLiteral("VoltagePhaseA"));
        VoltagePhaseA->setGeometry(QRect(50, 40, 80, 30));
        sizePolicy.setHeightForWidth(VoltagePhaseA->sizePolicy().hasHeightForWidth());
        VoltagePhaseA->setSizePolicy(sizePolicy);
        VoltagePhaseA->setMinimumSize(QSize(80, 30));
        VoltagePhaseA->setMaximumSize(QSize(80, 30));
        VoltagePhaseA->setFont(font);
        VoltagePhaseA->setContextMenuPolicy(Qt::NoContextMenu);
        VoltagePhaseA->setLayoutDirection(Qt::LeftToRight);
        VoltagePhaseA->setInputMethodHints(Qt::ImhNone);
        VoltagePhaseA->setMaxLength(8);
        VoltagePhaseA->setCursorPosition(1);
        VoltagePhaseA->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        VoltagePhaseB = new QLineEdit(centralWidget);
        VoltagePhaseB->setObjectName(QStringLiteral("VoltagePhaseB"));
        VoltagePhaseB->setGeometry(QRect(140, 40, 80, 30));
        sizePolicy.setHeightForWidth(VoltagePhaseB->sizePolicy().hasHeightForWidth());
        VoltagePhaseB->setSizePolicy(sizePolicy);
        VoltagePhaseB->setMinimumSize(QSize(80, 30));
        VoltagePhaseB->setMaximumSize(QSize(80, 30));
        VoltagePhaseB->setFont(font);
        VoltagePhaseB->setContextMenuPolicy(Qt::NoContextMenu);
        VoltagePhaseB->setInputMethodHints(Qt::ImhNone);
        VoltagePhaseB->setMaxLength(8);
        VoltagePhaseB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        VoltagePhaseC = new QLineEdit(centralWidget);
        VoltagePhaseC->setObjectName(QStringLiteral("VoltagePhaseC"));
        VoltagePhaseC->setGeometry(QRect(230, 40, 80, 30));
        sizePolicy.setHeightForWidth(VoltagePhaseC->sizePolicy().hasHeightForWidth());
        VoltagePhaseC->setSizePolicy(sizePolicy);
        VoltagePhaseC->setMinimumSize(QSize(80, 30));
        VoltagePhaseC->setMaximumSize(QSize(80, 30));
        VoltagePhaseC->setFont(font);
        VoltagePhaseC->setContextMenuPolicy(Qt::NoContextMenu);
        VoltagePhaseC->setInputMethodHints(Qt::ImhNone);
        VoltagePhaseC->setMaxLength(8);
        VoltagePhaseC->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        CurrentPhaseA = new QLineEdit(centralWidget);
        CurrentPhaseA->setObjectName(QStringLiteral("CurrentPhaseA"));
        CurrentPhaseA->setGeometry(QRect(50, 100, 80, 30));
        sizePolicy.setHeightForWidth(CurrentPhaseA->sizePolicy().hasHeightForWidth());
        CurrentPhaseA->setSizePolicy(sizePolicy);
        CurrentPhaseA->setMinimumSize(QSize(80, 30));
        CurrentPhaseA->setMaximumSize(QSize(80, 30));
        CurrentPhaseA->setFont(font);
        CurrentPhaseA->setContextMenuPolicy(Qt::NoContextMenu);
        CurrentPhaseA->setInputMethodHints(Qt::ImhNone);
        CurrentPhaseA->setMaxLength(8);
        CurrentPhaseA->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        CurrentPhaseB = new QLineEdit(centralWidget);
        CurrentPhaseB->setObjectName(QStringLiteral("CurrentPhaseB"));
        CurrentPhaseB->setGeometry(QRect(140, 100, 80, 30));
        sizePolicy.setHeightForWidth(CurrentPhaseB->sizePolicy().hasHeightForWidth());
        CurrentPhaseB->setSizePolicy(sizePolicy);
        CurrentPhaseB->setMinimumSize(QSize(80, 30));
        CurrentPhaseB->setMaximumSize(QSize(80, 30));
        CurrentPhaseB->setFont(font);
        CurrentPhaseB->setContextMenuPolicy(Qt::NoContextMenu);
        CurrentPhaseB->setInputMethodHints(Qt::ImhNone);
        CurrentPhaseB->setMaxLength(8);
        CurrentPhaseB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        CurrentPhaseC = new QLineEdit(centralWidget);
        CurrentPhaseC->setObjectName(QStringLiteral("CurrentPhaseC"));
        CurrentPhaseC->setGeometry(QRect(230, 100, 80, 30));
        sizePolicy.setHeightForWidth(CurrentPhaseC->sizePolicy().hasHeightForWidth());
        CurrentPhaseC->setSizePolicy(sizePolicy);
        CurrentPhaseC->setMinimumSize(QSize(80, 30));
        CurrentPhaseC->setMaximumSize(QSize(80, 30));
        CurrentPhaseC->setFont(font);
        CurrentPhaseC->setContextMenuPolicy(Qt::NoContextMenu);
        CurrentPhaseC->setInputMethodHints(Qt::ImhNone);
        CurrentPhaseC->setMaxLength(8);
        CurrentPhaseC->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        CurrentPhaseN = new QLineEdit(centralWidget);
        CurrentPhaseN->setObjectName(QStringLiteral("CurrentPhaseN"));
        CurrentPhaseN->setGeometry(QRect(320, 100, 80, 30));
        sizePolicy.setHeightForWidth(CurrentPhaseN->sizePolicy().hasHeightForWidth());
        CurrentPhaseN->setSizePolicy(sizePolicy);
        CurrentPhaseN->setMinimumSize(QSize(80, 30));
        CurrentPhaseN->setMaximumSize(QSize(80, 30));
        CurrentPhaseN->setFont(font);
        CurrentPhaseN->setContextMenuPolicy(Qt::NoContextMenu);
        CurrentPhaseN->setInputMethodHints(Qt::ImhNone);
        CurrentPhaseN->setMaxLength(8);
        CurrentPhaseN->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        CurrenAngleA = new QLineEdit(centralWidget);
        CurrenAngleA->setObjectName(QStringLiteral("CurrenAngleA"));
        CurrenAngleA->setGeometry(QRect(50, 160, 80, 30));
        sizePolicy.setHeightForWidth(CurrenAngleA->sizePolicy().hasHeightForWidth());
        CurrenAngleA->setSizePolicy(sizePolicy);
        CurrenAngleA->setMinimumSize(QSize(80, 30));
        CurrenAngleA->setMaximumSize(QSize(80, 30));
        CurrenAngleA->setFont(font);
        CurrenAngleA->setContextMenuPolicy(Qt::NoContextMenu);
        CurrenAngleA->setInputMethodHints(Qt::ImhNone);
        CurrenAngleA->setMaxLength(3);
        CurrenAngleA->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        CurrenAngleB = new QLineEdit(centralWidget);
        CurrenAngleB->setObjectName(QStringLiteral("CurrenAngleB"));
        CurrenAngleB->setGeometry(QRect(140, 160, 80, 30));
        sizePolicy.setHeightForWidth(CurrenAngleB->sizePolicy().hasHeightForWidth());
        CurrenAngleB->setSizePolicy(sizePolicy);
        CurrenAngleB->setMinimumSize(QSize(80, 30));
        CurrenAngleB->setMaximumSize(QSize(80, 30));
        CurrenAngleB->setFont(font);
        CurrenAngleB->setContextMenuPolicy(Qt::NoContextMenu);
        CurrenAngleB->setInputMethodHints(Qt::ImhNone);
        CurrenAngleB->setMaxLength(3);
        CurrenAngleB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        CurrenAngleC = new QLineEdit(centralWidget);
        CurrenAngleC->setObjectName(QStringLiteral("CurrenAngleC"));
        CurrenAngleC->setGeometry(QRect(230, 160, 80, 30));
        sizePolicy.setHeightForWidth(CurrenAngleC->sizePolicy().hasHeightForWidth());
        CurrenAngleC->setSizePolicy(sizePolicy);
        CurrenAngleC->setMinimumSize(QSize(80, 30));
        CurrenAngleC->setMaximumSize(QSize(80, 30));
        CurrenAngleC->setFont(font);
        CurrenAngleC->setContextMenuPolicy(Qt::NoContextMenu);
        CurrenAngleC->setInputMethodHints(Qt::ImhNone);
        CurrenAngleC->setMaxLength(3);
        CurrenAngleC->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        MetringMode = new QComboBox(centralWidget);
        MetringMode->addItem(QString());
        MetringMode->addItem(QString());
        MetringMode->addItem(QString());
        MetringMode->setObjectName(QStringLiteral("MetringMode"));
        MetringMode->setGeometry(QRect(230, 350, 80, 30));
        HFPostive = new QLineEdit(centralWidget);
        HFPostive->setObjectName(QStringLiteral("HFPostive"));
        HFPostive->setGeometry(QRect(50, 350, 80, 30));
        sizePolicy.setHeightForWidth(HFPostive->sizePolicy().hasHeightForWidth());
        HFPostive->setSizePolicy(sizePolicy);
        HFPostive->setMinimumSize(QSize(80, 30));
        HFPostive->setMaximumSize(QSize(80, 30));
        HFPostive->setFont(font);
        HFPostive->setContextMenuPolicy(Qt::NoContextMenu);
        HFPostive->setInputMethodHints(Qt::ImhNone);
        HFPostive->setMaxLength(6);
        HFPostive->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        HFNegitive = new QLineEdit(centralWidget);
        HFNegitive->setObjectName(QStringLiteral("HFNegitive"));
        HFNegitive->setGeometry(QRect(140, 350, 80, 30));
        sizePolicy.setHeightForWidth(HFNegitive->sizePolicy().hasHeightForWidth());
        HFNegitive->setSizePolicy(sizePolicy);
        HFNegitive->setMinimumSize(QSize(80, 30));
        HFNegitive->setMaximumSize(QSize(80, 30));
        HFNegitive->setFont(font);
        HFNegitive->setContextMenuPolicy(Qt::NoContextMenu);
        HFNegitive->setInputMethodHints(Qt::ImhNone);
        HFNegitive->setMaxLength(6);
        HFNegitive->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        PositivePowerPhaseA = new QLineEdit(centralWidget);
        PositivePowerPhaseA->setObjectName(QStringLiteral("PositivePowerPhaseA"));
        PositivePowerPhaseA->setGeometry(QRect(50, 220, 80, 30));
        sizePolicy.setHeightForWidth(PositivePowerPhaseA->sizePolicy().hasHeightForWidth());
        PositivePowerPhaseA->setSizePolicy(sizePolicy);
        PositivePowerPhaseA->setMinimumSize(QSize(80, 30));
        PositivePowerPhaseA->setMaximumSize(QSize(80, 30));
        PositivePowerPhaseA->setFont(font);
        PositivePowerPhaseA->setContextMenuPolicy(Qt::NoContextMenu);
        PositivePowerPhaseA->setInputMethodHints(Qt::ImhNone);
        PositivePowerPhaseA->setMaxLength(8);
        PositivePowerPhaseA->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        PositivePowerPhaseA->setReadOnly(true);
        PositivePowerPhaseB = new QLineEdit(centralWidget);
        PositivePowerPhaseB->setObjectName(QStringLiteral("PositivePowerPhaseB"));
        PositivePowerPhaseB->setGeometry(QRect(140, 220, 80, 30));
        sizePolicy.setHeightForWidth(PositivePowerPhaseB->sizePolicy().hasHeightForWidth());
        PositivePowerPhaseB->setSizePolicy(sizePolicy);
        PositivePowerPhaseB->setMinimumSize(QSize(80, 30));
        PositivePowerPhaseB->setMaximumSize(QSize(80, 30));
        PositivePowerPhaseB->setFont(font);
        PositivePowerPhaseB->setContextMenuPolicy(Qt::NoContextMenu);
        PositivePowerPhaseB->setInputMethodHints(Qt::ImhNone);
        PositivePowerPhaseB->setMaxLength(8);
        PositivePowerPhaseB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        PositivePowerPhaseB->setReadOnly(true);
        PositivePowerPhaseC = new QLineEdit(centralWidget);
        PositivePowerPhaseC->setObjectName(QStringLiteral("PositivePowerPhaseC"));
        PositivePowerPhaseC->setGeometry(QRect(230, 220, 80, 30));
        sizePolicy.setHeightForWidth(PositivePowerPhaseC->sizePolicy().hasHeightForWidth());
        PositivePowerPhaseC->setSizePolicy(sizePolicy);
        PositivePowerPhaseC->setMinimumSize(QSize(80, 30));
        PositivePowerPhaseC->setMaximumSize(QSize(80, 30));
        PositivePowerPhaseC->setFont(font);
        PositivePowerPhaseC->setContextMenuPolicy(Qt::NoContextMenu);
        PositivePowerPhaseC->setInputMethodHints(Qt::ImhNone);
        PositivePowerPhaseC->setMaxLength(8);
        PositivePowerPhaseC->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        PositivePowerPhaseC->setReadOnly(true);
        NegitivePowerPhaseA = new QLineEdit(centralWidget);
        NegitivePowerPhaseA->setObjectName(QStringLiteral("NegitivePowerPhaseA"));
        NegitivePowerPhaseA->setGeometry(QRect(50, 280, 80, 30));
        sizePolicy.setHeightForWidth(NegitivePowerPhaseA->sizePolicy().hasHeightForWidth());
        NegitivePowerPhaseA->setSizePolicy(sizePolicy);
        NegitivePowerPhaseA->setMinimumSize(QSize(80, 30));
        NegitivePowerPhaseA->setMaximumSize(QSize(80, 30));
        NegitivePowerPhaseA->setFont(font);
        NegitivePowerPhaseA->setContextMenuPolicy(Qt::NoContextMenu);
        NegitivePowerPhaseA->setInputMethodHints(Qt::ImhNone);
        NegitivePowerPhaseA->setMaxLength(8);
        NegitivePowerPhaseA->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        NegitivePowerPhaseA->setReadOnly(true);
        NegitivePowerPhaseB = new QLineEdit(centralWidget);
        NegitivePowerPhaseB->setObjectName(QStringLiteral("NegitivePowerPhaseB"));
        NegitivePowerPhaseB->setGeometry(QRect(140, 280, 80, 30));
        sizePolicy.setHeightForWidth(NegitivePowerPhaseB->sizePolicy().hasHeightForWidth());
        NegitivePowerPhaseB->setSizePolicy(sizePolicy);
        NegitivePowerPhaseB->setMinimumSize(QSize(80, 30));
        NegitivePowerPhaseB->setMaximumSize(QSize(80, 30));
        NegitivePowerPhaseB->setFont(font);
        NegitivePowerPhaseB->setContextMenuPolicy(Qt::NoContextMenu);
        NegitivePowerPhaseB->setInputMethodHints(Qt::ImhNone);
        NegitivePowerPhaseB->setMaxLength(8);
        NegitivePowerPhaseB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        NegitivePowerPhaseB->setReadOnly(true);
        NegitivePowerPhaseC = new QLineEdit(centralWidget);
        NegitivePowerPhaseC->setObjectName(QStringLiteral("NegitivePowerPhaseC"));
        NegitivePowerPhaseC->setGeometry(QRect(230, 280, 80, 30));
        sizePolicy.setHeightForWidth(NegitivePowerPhaseC->sizePolicy().hasHeightForWidth());
        NegitivePowerPhaseC->setSizePolicy(sizePolicy);
        NegitivePowerPhaseC->setMinimumSize(QSize(80, 30));
        NegitivePowerPhaseC->setMaximumSize(QSize(80, 30));
        NegitivePowerPhaseC->setFont(font);
        NegitivePowerPhaseC->setContextMenuPolicy(Qt::NoContextMenu);
        NegitivePowerPhaseC->setInputMethodHints(Qt::ImhNone);
        NegitivePowerPhaseC->setMaxLength(8);
        NegitivePowerPhaseC->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        NegitivePowerPhaseC->setReadOnly(true);
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, 190, 421, 21));
        line->setFrameShadow(QFrame::Sunken);
        line->setLineWidth(3);
        line->setFrameShape(QFrame::HLine);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(75, 15, 80, 20));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(165, 15, 80, 20));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(255, 15, 80, 20));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(330, 15, 80, 20));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(5, 45, 40, 20));
        label_5->setFrameShadow(QFrame::Plain);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(5, 105, 40, 20));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(5, 165, 40, 20));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(5, 225, 40, 20));
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(5, 285, 40, 20));
        PositivePowerTotal = new QLineEdit(centralWidget);
        PositivePowerTotal->setObjectName(QStringLiteral("PositivePowerTotal"));
        PositivePowerTotal->setGeometry(QRect(320, 220, 80, 30));
        sizePolicy.setHeightForWidth(PositivePowerTotal->sizePolicy().hasHeightForWidth());
        PositivePowerTotal->setSizePolicy(sizePolicy);
        PositivePowerTotal->setMinimumSize(QSize(80, 30));
        PositivePowerTotal->setMaximumSize(QSize(80, 30));
        PositivePowerTotal->setFont(font);
        PositivePowerTotal->setContextMenuPolicy(Qt::NoContextMenu);
        PositivePowerTotal->setInputMethodHints(Qt::ImhNone);
        PositivePowerTotal->setMaxLength(8);
        PositivePowerTotal->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        PositivePowerTotal->setReadOnly(true);
        NegitivePowerTotal = new QLineEdit(centralWidget);
        NegitivePowerTotal->setObjectName(QStringLiteral("NegitivePowerTotal"));
        NegitivePowerTotal->setGeometry(QRect(320, 280, 80, 30));
        sizePolicy.setHeightForWidth(NegitivePowerTotal->sizePolicy().hasHeightForWidth());
        NegitivePowerTotal->setSizePolicy(sizePolicy);
        NegitivePowerTotal->setMinimumSize(QSize(80, 30));
        NegitivePowerTotal->setMaximumSize(QSize(80, 30));
        NegitivePowerTotal->setFont(font);
        NegitivePowerTotal->setContextMenuPolicy(Qt::NoContextMenu);
        NegitivePowerTotal->setInputMethodHints(Qt::ImhNone);
        NegitivePowerTotal->setMaxLength(8);
        NegitivePowerTotal->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        NegitivePowerTotal->setReadOnly(true);
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(55, 325, 80, 20));
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(145, 325, 80, 20));
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(235, 325, 80, 20));
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(430, 45, 40, 20));
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(430, 105, 40, 20));
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(430, 165, 40, 20));
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(430, 225, 40, 20));
        label_17 = new QLabel(centralWidget);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(430, 285, 40, 20));
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(410, 0, 20, 321));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(0, 310, 421, 21));
        line_3->setFrameShadow(QFrame::Sunken);
        line_3->setLineWidth(3);
        line_3->setFrameShape(QFrame::HLine);
        VoltageInverse = new QCheckBox(centralWidget);
        VoltageInverse->setObjectName(QStringLiteral("VoltageInverse"));
        VoltageInverse->setGeometry(QRect(320, 40, 80, 30));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        MetringMode->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Power Emulator", nullptr));
        StartButton->setText(QApplication::translate("MainWindow", "\345\267\262\345\201\234\346\255\242", nullptr));
        VoltagePhaseA->setText(QApplication::translate("MainWindow", "0", nullptr));
        VoltagePhaseB->setText(QApplication::translate("MainWindow", "0", nullptr));
        VoltagePhaseC->setText(QApplication::translate("MainWindow", "0", nullptr));
        CurrentPhaseA->setText(QApplication::translate("MainWindow", "0", nullptr));
        CurrentPhaseB->setText(QApplication::translate("MainWindow", "0", nullptr));
        CurrentPhaseC->setText(QApplication::translate("MainWindow", "0", nullptr));
        CurrentPhaseN->setText(QApplication::translate("MainWindow", "0", nullptr));
        CurrenAngleA->setText(QApplication::translate("MainWindow", "0", nullptr));
        CurrenAngleB->setText(QApplication::translate("MainWindow", "0", nullptr));
        CurrenAngleC->setText(QApplication::translate("MainWindow", "0", nullptr));
        MetringMode->setItemText(0, QApplication::translate("MainWindow", "\345\215\225\347\233\270", nullptr));
        MetringMode->setItemText(1, QApplication::translate("MainWindow", "\344\270\211\347\233\270\344\270\211", nullptr));
        MetringMode->setItemText(2, QApplication::translate("MainWindow", "\344\270\211\347\233\270\345\233\233", nullptr));

        HFPostive->setText(QApplication::translate("MainWindow", "10", nullptr));
        HFNegitive->setText(QApplication::translate("MainWindow", "10", nullptr));
        PositivePowerPhaseA->setText(QApplication::translate("MainWindow", "0", nullptr));
        PositivePowerPhaseB->setText(QApplication::translate("MainWindow", "0", nullptr));
        PositivePowerPhaseC->setText(QApplication::translate("MainWindow", "0", nullptr));
        NegitivePowerPhaseA->setText(QApplication::translate("MainWindow", "0", nullptr));
        NegitivePowerPhaseB->setText(QApplication::translate("MainWindow", "0", nullptr));
        NegitivePowerPhaseC->setText(QApplication::translate("MainWindow", "0", nullptr));
        label->setText(QApplication::translate("MainWindow", "A\347\233\270", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "B\347\233\270", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "C\347\233\270", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "N\347\233\270/\346\200\273", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "\347\224\265\345\216\213", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "\347\224\265\346\265\201", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "\347\233\270\350\247\222", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "\346\234\211\345\212\237", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "\346\227\240\345\212\237", nullptr));
        PositivePowerTotal->setText(QApplication::translate("MainWindow", "0", nullptr));
        NegitivePowerTotal->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "\346\234\211\345\212\237\345\270\270\346\225\260", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "\346\227\240\345\212\237\345\270\270\346\225\260", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "\350\256\241\351\207\217\346\250\241\345\236\213", nullptr));
        label_13->setText(QApplication::translate("MainWindow", "\344\274\217\347\211\271", nullptr));
        label_14->setText(QApplication::translate("MainWindow", "\345\256\211\345\237\271", nullptr));
        label_15->setText(QApplication::translate("MainWindow", "\345\272\246", nullptr));
        label_16->setText(QApplication::translate("MainWindow", "\347\223\246", nullptr));
        label_17->setText(QApplication::translate("MainWindow", "\344\271\217", nullptr));
        VoltageInverse->setText(QApplication::translate("MainWindow", "\351\200\206\347\233\270\345\272\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
