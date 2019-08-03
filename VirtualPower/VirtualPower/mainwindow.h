#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/**
  * @brief  计量数据元
  */
enum __metering_meta
{
    R_EPT	 = 0x01, //合相有功电能
    R_EPA	 = 0x02, //A相有功电能
    R_EPB	 = 0x03, //B相有功电能
    R_EPC	 = 0x04, //C相有功电能

    R_EQT	 = 0x05, //合相无功电能
    R_EQA	 = 0x06, //A相无功电能
    R_EQB	 = 0x07, //B相无功电能
    R_EQC	 = 0x08, //C相无功电能

    R_EST	 = 0x09, //合相视在电能
    R_ESA	 = 0x0A, //A相视在电能
    R_ESB	 = 0x0B, //B相视在电能
    R_ESC	 = 0x0C, //C相视在电能

    R_PT	 = 0x0D, //合相有功功率
    R_PA	 = 0x0E, //A相有功功率
    R_PB	 = 0x0F, //B相有功功率
    R_PC	 = 0x10, //C相有功功率

    R_QT	 = 0x11, //合相无功功率
    R_QA	 = 0x12, //A相无功功率
    R_QB	 = 0x13, //B相无功功率
    R_QC	 = 0x14, //C相无功功率

    R_ST	 = 0x15, //合相视在功率
    R_SA	 = 0x16, //A相视在功率
    R_SB	 = 0x17, //B相视在功率
    R_SC	 = 0x18, //C相视在功率

    R_PFT	 = 0x19, //合相功率因数
    R_PFA	 = 0x1A, //A相功率因数
    R_PFB	 = 0x1B, //B相功率因数
    R_PFC	 = 0x1C, //C相功率因数


    R_UARMS	 = 0x1D, //A相电压有效值
    R_UBRMS	 = 0x1E, //B相电压有效值
    R_UCRMS	 = 0x1F, //C相电压有效值

    R_ITRMS	 = 0x20, //三相电流矢量和的有效值
    R_IARMS	 = 0x21, //A相电流有效值
    R_IBRMS	 = 0x22, //B相电流有效值
    R_ICRMS	 = 0x23, //C相电流有效值


    R_PGA	 = 0x24, //A相电流与电压相角
    R_PGB	 = 0x25, //B相电流与电压相角
    R_PGC	 = 0x26, //C相电流与电压相角

    R_YUAUB	 = 0x27, //Ua与Ub的电压夹角
    R_YUAUC	 = 0x28, //Ua与Uc的电压夹角
    R_YUBUC	 = 0x29, //Ub与Uc的电压夹角

    R_FREQ	 = 0x2A, //频率
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void CalcutePower();

    void on_VoltagePhaseA_editingFinished();

    void on_VoltagePhaseB_editingFinished();

    void on_VoltagePhaseC_editingFinished();

    void on_CurrentPhaseA_editingFinished();

    void on_CurrentPhaseB_editingFinished();

    void on_CurrentPhaseC_editingFinished();

    void on_CurrentPhaseN_editingFinished();

    void on_CurrenAngleA_textChanged(const QString &arg1);

    void on_CurrenAngleB_textChanged(const QString &arg1);

    void on_CurrenAngleC_textChanged(const QString &arg1);

    void on_HFPostive_textChanged(const QString &arg1);

    void on_HFNegitive_textChanged(const QString &arg1);

    void on_VoltageInverse_released();

    void on_MetringMode_currentIndexChanged(int index);

    void on_StartButton_released();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
