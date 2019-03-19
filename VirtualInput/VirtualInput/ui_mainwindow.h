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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *KEY_1;
    QPushButton *KEY_2;
    QPushButton *KEY_3;
    QPushButton *KEY_6;
    QPushButton *KEY_4;
    QPushButton *KEY_5;
    QPushButton *KEY_9;
    QPushButton *KEY_7;
    QPushButton *KEY_8;
    QPushButton *KEY_ENTER;
    QPushButton *KEY_0;
    QPushButton *KEY_DEL;
    QPushButton *KEY_PROG;
    QPushButton *KEY_UP;
    QPushButton *KEY_DOWN;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;
    QFrame *line_4;
    QFrame *line_5;
    QFrame *line_6;
    QCheckBox *B_MAIN_COVER;
    QCheckBox *B_SUB_COVER;
    QCheckBox *B_BAT_RTC;
    QFrame *line_7;
    QCheckBox *B_BAT_BACKUP;
    QCheckBox *B_MAGNETIC;
    QCheckBox *B_RELAY;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(480, 360);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(480, 360));
        MainWindow->setMaximumSize(QSize(480, 360));
        QFont font;
        font.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font.setPointSize(12);
        MainWindow->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8("Paint.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        KEY_1 = new QPushButton(centralWidget);
        KEY_1->setObjectName(QString::fromUtf8("KEY_1"));
        KEY_1->setGeometry(QRect(10, 15, 80, 40));
        KEY_2 = new QPushButton(centralWidget);
        KEY_2->setObjectName(QString::fromUtf8("KEY_2"));
        KEY_2->setGeometry(QRect(130, 15, 80, 40));
        KEY_3 = new QPushButton(centralWidget);
        KEY_3->setObjectName(QString::fromUtf8("KEY_3"));
        KEY_3->setGeometry(QRect(250, 15, 80, 40));
        KEY_6 = new QPushButton(centralWidget);
        KEY_6->setObjectName(QString::fromUtf8("KEY_6"));
        KEY_6->setGeometry(QRect(250, 95, 80, 40));
        KEY_4 = new QPushButton(centralWidget);
        KEY_4->setObjectName(QString::fromUtf8("KEY_4"));
        KEY_4->setGeometry(QRect(10, 95, 80, 40));
        KEY_5 = new QPushButton(centralWidget);
        KEY_5->setObjectName(QString::fromUtf8("KEY_5"));
        KEY_5->setGeometry(QRect(130, 95, 80, 40));
        KEY_9 = new QPushButton(centralWidget);
        KEY_9->setObjectName(QString::fromUtf8("KEY_9"));
        KEY_9->setGeometry(QRect(250, 175, 80, 40));
        KEY_7 = new QPushButton(centralWidget);
        KEY_7->setObjectName(QString::fromUtf8("KEY_7"));
        KEY_7->setGeometry(QRect(10, 175, 80, 40));
        KEY_8 = new QPushButton(centralWidget);
        KEY_8->setObjectName(QString::fromUtf8("KEY_8"));
        KEY_8->setGeometry(QRect(130, 175, 80, 40));
        KEY_ENTER = new QPushButton(centralWidget);
        KEY_ENTER->setObjectName(QString::fromUtf8("KEY_ENTER"));
        KEY_ENTER->setGeometry(QRect(250, 255, 80, 40));
        KEY_0 = new QPushButton(centralWidget);
        KEY_0->setObjectName(QString::fromUtf8("KEY_0"));
        KEY_0->setGeometry(QRect(10, 255, 80, 40));
        KEY_DEL = new QPushButton(centralWidget);
        KEY_DEL->setObjectName(QString::fromUtf8("KEY_DEL"));
        KEY_DEL->setGeometry(QRect(130, 255, 80, 40));
        KEY_PROG = new QPushButton(centralWidget);
        KEY_PROG->setObjectName(QString::fromUtf8("KEY_PROG"));
        KEY_PROG->setGeometry(QRect(370, 255, 90, 40));
        KEY_UP = new QPushButton(centralWidget);
        KEY_UP->setObjectName(QString::fromUtf8("KEY_UP"));
        KEY_UP->setGeometry(QRect(370, 15, 90, 80));
        KEY_DOWN = new QPushButton(centralWidget);
        KEY_DOWN->setObjectName(QString::fromUtf8("KEY_DOWN"));
        KEY_DOWN->setGeometry(QRect(370, 125, 90, 80));
        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(332, 0, 41, 311));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(0, 65, 351, 20));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(0, 145, 351, 20));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        line_4 = new QFrame(centralWidget);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setGeometry(QRect(0, 225, 351, 20));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);
        line_5 = new QFrame(centralWidget);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setGeometry(QRect(93, 0, 31, 311));
        line_5->setFrameShape(QFrame::VLine);
        line_5->setFrameShadow(QFrame::Sunken);
        line_6 = new QFrame(centralWidget);
        line_6->setObjectName(QString::fromUtf8("line_6"));
        line_6->setGeometry(QRect(220, 0, 31, 311));
        line_6->setFrameShape(QFrame::VLine);
        line_6->setFrameShadow(QFrame::Sunken);
        B_MAIN_COVER = new QCheckBox(centralWidget);
        B_MAIN_COVER->setObjectName(QString::fromUtf8("B_MAIN_COVER"));
        B_MAIN_COVER->setGeometry(QRect(130, 315, 80, 20));
        QFont font1;
        font1.setPointSize(10);
        B_MAIN_COVER->setFont(font1);
        B_SUB_COVER = new QCheckBox(centralWidget);
        B_SUB_COVER->setObjectName(QString::fromUtf8("B_SUB_COVER"));
        B_SUB_COVER->setGeometry(QRect(130, 335, 80, 20));
        B_SUB_COVER->setFont(font1);
        B_BAT_RTC = new QCheckBox(centralWidget);
        B_BAT_RTC->setObjectName(QString::fromUtf8("B_BAT_RTC"));
        B_BAT_RTC->setGeometry(QRect(10, 315, 101, 20));
        B_BAT_RTC->setFont(font1);
        line_7 = new QFrame(centralWidget);
        line_7->setObjectName(QString::fromUtf8("line_7"));
        line_7->setGeometry(QRect(0, 300, 481, 20));
        line_7->setFrameShadow(QFrame::Raised);
        line_7->setLineWidth(2);
        line_7->setFrameShape(QFrame::HLine);
        B_BAT_BACKUP = new QCheckBox(centralWidget);
        B_BAT_BACKUP->setObjectName(QString::fromUtf8("B_BAT_BACKUP"));
        B_BAT_BACKUP->setGeometry(QRect(10, 335, 101, 20));
        B_BAT_BACKUP->setFont(font1);
        B_MAGNETIC = new QCheckBox(centralWidget);
        B_MAGNETIC->setObjectName(QString::fromUtf8("B_MAGNETIC"));
        B_MAGNETIC->setGeometry(QRect(250, 315, 101, 20));
        B_MAGNETIC->setFont(font1);
        B_RELAY = new QCheckBox(centralWidget);
        B_RELAY->setObjectName(QString::fromUtf8("B_RELAY"));
        B_RELAY->setGeometry(QRect(250, 335, 101, 20));
        B_RELAY->setFont(font1);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Input Emulator", nullptr));
        KEY_1->setText(QApplication::translate("MainWindow", "1", nullptr));
        KEY_2->setText(QApplication::translate("MainWindow", "2", nullptr));
        KEY_3->setText(QApplication::translate("MainWindow", "3", nullptr));
        KEY_6->setText(QApplication::translate("MainWindow", "6", nullptr));
        KEY_4->setText(QApplication::translate("MainWindow", "4", nullptr));
        KEY_5->setText(QApplication::translate("MainWindow", "5", nullptr));
        KEY_9->setText(QApplication::translate("MainWindow", "9", nullptr));
        KEY_7->setText(QApplication::translate("MainWindow", "7", nullptr));
        KEY_8->setText(QApplication::translate("MainWindow", "8", nullptr));
        KEY_ENTER->setText(QApplication::translate("MainWindow", "\345\233\236\350\275\246", nullptr));
        KEY_0->setText(QApplication::translate("MainWindow", "0", nullptr));
        KEY_DEL->setText(QApplication::translate("MainWindow", "\345\210\240\351\231\244", nullptr));
        KEY_PROG->setText(QApplication::translate("MainWindow", "\347\274\226\347\250\213", nullptr));
        KEY_UP->setText(QApplication::translate("MainWindow", "\344\270\212\347\277\273", nullptr));
        KEY_DOWN->setText(QApplication::translate("MainWindow", "\344\270\213\347\277\273", nullptr));
        B_MAIN_COVER->setText(QApplication::translate("MainWindow", "\350\241\250\347\233\226\346\211\223\345\274\200", nullptr));
        B_SUB_COVER->setText(QApplication::translate("MainWindow", "\347\253\257\347\233\226\346\211\223\345\274\200", nullptr));
        B_BAT_RTC->setText(QApplication::translate("MainWindow", "\346\227\266\351\222\237\347\224\265\346\261\240\346\254\240\345\216\213", nullptr));
        B_BAT_BACKUP->setText(QApplication::translate("MainWindow", "\346\212\204\350\241\250\347\224\265\346\261\240\346\254\240\345\216\213", nullptr));
        B_MAGNETIC->setText(QApplication::translate("MainWindow", "\347\224\265\347\243\201\345\234\272\345\271\262\346\211\260", nullptr));
        B_RELAY->setText(QApplication::translate("MainWindow", "\347\273\247\347\224\265\345\231\250\346\215\237\345\235\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
