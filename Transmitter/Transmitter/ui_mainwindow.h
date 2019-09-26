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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *StartButton;
    QLineEdit *AddressStart;
    QComboBox *SerialNo;
    QLabel *label_1;
    QLabel *label_2;
    QComboBox *Baudrate;
    QComboBox *CheckBit;
    QComboBox *StopBit;
    QLineEdit *Password;
    QLabel *label_3;
    QLineEdit *AddressStop;
    QPushButton *FileButton;
    QTextBrowser *LogBrowser;
    QLineEdit *AddressCurrent;
    QLabel *label_4;
    QProgressBar *Process;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(480, 430);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(480, 430));
        MainWindow->setMaximumSize(QSize(480, 430));
        QFont font;
        font.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font.setPointSize(12);
        MainWindow->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8("Paint.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        StartButton = new QPushButton(centralWidget);
        StartButton->setObjectName(QString::fromUtf8("StartButton"));
        StartButton->setGeometry(QRect(380, 130, 90, 30));
        AddressStart = new QLineEdit(centralWidget);
        AddressStart->setObjectName(QString::fromUtf8("AddressStart"));
        AddressStart->setGeometry(QRect(190, 10, 150, 30));
        sizePolicy.setHeightForWidth(AddressStart->sizePolicy().hasHeightForWidth());
        AddressStart->setSizePolicy(sizePolicy);
        AddressStart->setMinimumSize(QSize(150, 30));
        AddressStart->setMaximumSize(QSize(150, 30));
        AddressStart->setFont(font);
        AddressStart->setContextMenuPolicy(Qt::NoContextMenu);
        AddressStart->setInputMethodHints(Qt::ImhNone);
        AddressStart->setMaxLength(12);
        AddressStart->setEchoMode(QLineEdit::Normal);
        AddressStart->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        SerialNo = new QComboBox(centralWidget);
        SerialNo->setObjectName(QString::fromUtf8("SerialNo"));
        SerialNo->setGeometry(QRect(10, 10, 80, 30));
        label_1 = new QLabel(centralWidget);
        label_1->setObjectName(QString::fromUtf8("label_1"));
        label_1->setGeometry(QRect(110, 15, 70, 20));
        label_1->setFrameShadow(QFrame::Plain);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(110, 55, 70, 20));
        Baudrate = new QComboBox(centralWidget);
        Baudrate->addItem(QString());
        Baudrate->addItem(QString());
        Baudrate->addItem(QString());
        Baudrate->addItem(QString());
        Baudrate->addItem(QString());
        Baudrate->addItem(QString());
        Baudrate->addItem(QString());
        Baudrate->addItem(QString());
        Baudrate->setObjectName(QString::fromUtf8("Baudrate"));
        Baudrate->setGeometry(QRect(10, 50, 80, 30));
        CheckBit = new QComboBox(centralWidget);
        CheckBit->addItem(QString());
        CheckBit->addItem(QString());
        CheckBit->addItem(QString());
        CheckBit->setObjectName(QString::fromUtf8("CheckBit"));
        CheckBit->setGeometry(QRect(10, 90, 80, 30));
        StopBit = new QComboBox(centralWidget);
        StopBit->addItem(QString());
        StopBit->addItem(QString());
        StopBit->addItem(QString());
        StopBit->setObjectName(QString::fromUtf8("StopBit"));
        StopBit->setGeometry(QRect(10, 130, 80, 30));
        Password = new QLineEdit(centralWidget);
        Password->setObjectName(QString::fromUtf8("Password"));
        Password->setGeometry(QRect(190, 90, 150, 30));
        sizePolicy.setHeightForWidth(Password->sizePolicy().hasHeightForWidth());
        Password->setSizePolicy(sizePolicy);
        Password->setMinimumSize(QSize(150, 30));
        Password->setMaximumSize(QSize(150, 30));
        Password->setFont(font);
        Password->setContextMenuPolicy(Qt::NoContextMenu);
        Password->setInputMethodHints(Qt::ImhNone);
        Password->setMaxLength(16);
        Password->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(110, 95, 70, 20));
        AddressStop = new QLineEdit(centralWidget);
        AddressStop->setObjectName(QString::fromUtf8("AddressStop"));
        AddressStop->setGeometry(QRect(190, 50, 150, 30));
        sizePolicy.setHeightForWidth(AddressStop->sizePolicy().hasHeightForWidth());
        AddressStop->setSizePolicy(sizePolicy);
        AddressStop->setMinimumSize(QSize(150, 30));
        AddressStop->setMaximumSize(QSize(150, 30));
        AddressStop->setFont(font);
        AddressStop->setContextMenuPolicy(Qt::NoContextMenu);
        AddressStop->setInputMethodHints(Qt::ImhNone);
        AddressStop->setMaxLength(12);
        AddressStop->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        FileButton = new QPushButton(centralWidget);
        FileButton->setObjectName(QString::fromUtf8("FileButton"));
        FileButton->setGeometry(QRect(380, 90, 90, 30));
        LogBrowser = new QTextBrowser(centralWidget);
        LogBrowser->setObjectName(QString::fromUtf8("LogBrowser"));
        LogBrowser->setGeometry(QRect(10, 170, 461, 221));
        LogBrowser->setContextMenuPolicy(Qt::DefaultContextMenu);
#ifndef QT_NO_TOOLTIP
        LogBrowser->setToolTip(QString::fromUtf8(""));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        LogBrowser->setStatusTip(QString::fromUtf8(""));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        LogBrowser->setWhatsThis(QString::fromUtf8(""));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_ACCESSIBILITY
        LogBrowser->setAccessibleName(QString::fromUtf8(""));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        LogBrowser->setAccessibleDescription(QString::fromUtf8(""));
#endif // QT_NO_ACCESSIBILITY
        AddressCurrent = new QLineEdit(centralWidget);
        AddressCurrent->setObjectName(QString::fromUtf8("AddressCurrent"));
        AddressCurrent->setGeometry(QRect(190, 130, 150, 30));
        sizePolicy.setHeightForWidth(AddressCurrent->sizePolicy().hasHeightForWidth());
        AddressCurrent->setSizePolicy(sizePolicy);
        AddressCurrent->setMinimumSize(QSize(150, 30));
        AddressCurrent->setMaximumSize(QSize(150, 30));
        AddressCurrent->setFont(font);
        AddressCurrent->setContextMenuPolicy(Qt::NoContextMenu);
        AddressCurrent->setInputMethodHints(Qt::ImhNone);
        AddressCurrent->setMaxLength(12);
        AddressCurrent->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        AddressCurrent->setReadOnly(true);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(110, 135, 70, 20));
        Process = new QProgressBar(centralWidget);
        Process->setObjectName(QString::fromUtf8("Process"));
        Process->setGeometry(QRect(10, 400, 460, 22));
        sizePolicy.setHeightForWidth(Process->sizePolicy().hasHeightForWidth());
        Process->setSizePolicy(sizePolicy);
        Process->setMinimumSize(QSize(460, 22));
        Process->setMaximumSize(QSize(460, 22));
        Process->setValue(0);
        Process->setTextVisible(false);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        SerialNo->setCurrentIndex(-1);
        Baudrate->setCurrentIndex(0);
        CheckBit->setCurrentIndex(0);
        StopBit->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Transmitter", nullptr));
        StartButton->setText(QApplication::translate("MainWindow", "\345\267\262\345\201\234\346\255\242", nullptr));
        AddressStart->setText(QApplication::translate("MainWindow", "1", nullptr));
        label_1->setText(QApplication::translate("MainWindow", "\350\265\267\345\247\213\345\234\260\345\235\200", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "\347\273\223\346\235\237\345\234\260\345\235\200", nullptr));
        Baudrate->setItemText(0, QApplication::translate("MainWindow", "1200", nullptr));
        Baudrate->setItemText(1, QApplication::translate("MainWindow", "2400", nullptr));
        Baudrate->setItemText(2, QApplication::translate("MainWindow", "4800", nullptr));
        Baudrate->setItemText(3, QApplication::translate("MainWindow", "9600", nullptr));
        Baudrate->setItemText(4, QApplication::translate("MainWindow", "19200", nullptr));
        Baudrate->setItemText(5, QApplication::translate("MainWindow", "38400", nullptr));
        Baudrate->setItemText(6, QApplication::translate("MainWindow", "57600", nullptr));
        Baudrate->setItemText(7, QApplication::translate("MainWindow", "115200", nullptr));

        CheckBit->setItemText(0, QApplication::translate("MainWindow", "\346\227\240\346\240\241\351\252\214", nullptr));
        CheckBit->setItemText(1, QApplication::translate("MainWindow", "\345\245\207\346\240\241\351\252\214", nullptr));
        CheckBit->setItemText(2, QApplication::translate("MainWindow", "\345\201\266\346\240\241\351\252\214", nullptr));

        StopBit->setItemText(0, QApplication::translate("MainWindow", "1\344\275\215", nullptr));
        StopBit->setItemText(1, QApplication::translate("MainWindow", "1\344\275\215\345\215\212", nullptr));
        StopBit->setItemText(2, QApplication::translate("MainWindow", "2\344\275\215", nullptr));

        Password->setText(QApplication::translate("MainWindow", "meter>management", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "\351\200\232\344\277\241\345\257\206\347\240\201", nullptr));
        AddressStop->setText(QApplication::translate("MainWindow", "1", nullptr));
        FileButton->setText(QApplication::translate("MainWindow", "\346\234\252\345\212\240\350\275\275", nullptr));
        LogBrowser->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'\347\255\211\347\272\277'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        AddressCurrent->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "\345\275\223\345\211\215\345\234\260\345\235\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
