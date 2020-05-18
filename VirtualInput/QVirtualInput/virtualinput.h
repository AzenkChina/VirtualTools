#ifndef VIRTUALINPUT_H
#define VIRTUALINPUT_H

#include <QObject>

class VirtualInput : public QObject
{
    Q_OBJECT

public:
    explicit VirtualInput(QObject *parent = nullptr);
    ~VirtualInput();
    QObject *ui;

    enum INDEX {ID01, ID02};
    Q_ENUM(INDEX)

signals:

public slots:
    void doSomething(enum INDEX ID);

    void TimerUpdate();

    void SendKeyboardData();

    void SendSensorsData();

    void SendBatteryData();

    void SendRelayData();

    void on_key_1_clicked();

    void on_key_2_clicked();

    void on_key_3_clicked();

    void on_key_4_clicked();

    void on_key_5_clicked();

    void on_key_6_clicked();

    void on_key_7_clicked();

    void on_key_8_clicked();

    void on_key_9_clicked();

    void on_key_0_clicked();

    void on_key_del_clicked();

    void on_key_enter_clicked();

    void on_key_up_pressed();

    void on_key_up_released();

    void on_key_down_pressed();

    void on_key_down_released();

    void on_key_prog_clicked();

    void on_check_main_cover_stateChanged(int arg1);

    void on_check_sub_cover_stateChanged(int arg1);

    void on_check_bat_rtc_stateChanged(int arg1);

    void on_check_bat_backup_stateChanged(int arg1);

    void on_check_magnetic_stateChanged(int arg1);

    void on_check_relay_stateChanged(int arg1);
};

#endif // VIRTUALINPUT_H
