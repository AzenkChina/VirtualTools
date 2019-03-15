#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void TimerUpdate();

    void SendKeyboardData();

    void SendSensorsData();

    void SendBatteryData();

    void SendRelayData();

    void on_KEY_1_clicked();

    void on_KEY_2_clicked();

    void on_KEY_3_clicked();

    void on_KEY_4_clicked();

    void on_KEY_5_clicked();

    void on_KEY_6_clicked();

    void on_KEY_7_clicked();

    void on_KEY_8_clicked();

    void on_KEY_9_clicked();

    void on_KEY_0_clicked();

    void on_KEY_DEL_clicked();

    void on_KEY_ENTER_clicked();

    void on_KEY_UP_pressed();

    void on_KEY_UP_released();

    void on_KEY_DOWN_pressed();

    void on_KEY_DOWN_released();

    void on_KEY_PROG_clicked();

    void on_B_MAIN_COVER_stateChanged(int arg1);

    void on_B_SUB_COVER_stateChanged(int arg1);

    void on_B_BAT_RTC_stateChanged(int arg1);

    void on_B_BAT_BACKUP_stateChanged(int arg1);

    void on_B_MAGNETIC_stateChanged(int arg1);

    void on_B_RELAY_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
