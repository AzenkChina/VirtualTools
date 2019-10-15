#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define LCD_MAX_WINDOWS             (2)//windows数量
#define LCD_WINDOW_MAIN             (0)//主windows
#define LCD_WINDOW_SUB              (1)//副windows

#define LCD_MAX_LABELS              (18)//label数量

#define LCD_LABEL_U1                (0)//符号 U1
#define LCD_LABEL_U2                (1)//符号 U2
#define LCD_LABEL_U3                (2)//符号 U3
#define LCD_LABEL_I1                (3)//符号 I1
#define LCD_LABEL_I2                (4)//符号 I2
#define LCD_LABEL_I3                (5)//符号 I3
#define LCD_LABEL_L1                (6)//符号 L1
#define LCD_LABEL_L2                (7)//符号 L2
#define LCD_LABEL_L3                (8)//符号 L3
#define LCD_LABEL_LN                (9)//符号 LN
#define LCD_LABEL_PF                (10)//符号 PF
#define LCD_LABEL_RELAY             (11)//继电器状态
#define LCD_LABEL_COMM              (12)//通讯符号
#define LCD_LABEL_WARN              (13)//报警符号
#define LCD_LABEL_QUAD              (14)//功率象限
#define LCD_LABEL_RATE              (15)//费率
#define LCD_LABEL_DATE              (16)//日期
#define LCD_LABEL_TIM               (17)//时间

/**
  * @brief  dot
  */
enum __lcd_dot
{
    LCD_DOT_NONE = 0,
    LCD_DOT_1,
    LCD_DOT_2,
    LCD_DOT_3,
    LCD_DOT_4,
    LCD_DOT_5,
    LCD_DOT_6,
    LCD_DOT_7,
    LCD_DOT_8,
    LCD_DOT_9,
    LCD_DOT_10,
    LCD_DOT_11,
    LCD_DOT_12,
};

/**
  * @brief  unit
  */
enum __lcd_unit
{
    LCD_UNIT_NONE = 0,

    LCD_UNIT_V,
    LCD_UNIT_KV,
    LCD_UNIT_MV,

    LCD_UNIT_A,
    LCD_UNIT_KA,
    LCD_UNIT_MA,

    LCD_UNIT_W,
    LCD_UNIT_KW,
    LCD_UNIT_MW,

    LCD_UNIT_VAR,
    LCD_UNIT_KVAR,
    LCD_UNIT_MVAR,

    LCD_UNIT_VA,
    LCD_UNIT_KVA,
    LCD_UNIT_MVA,

    LCD_UNIT_WH,
    LCD_UNIT_KWH,
    LCD_UNIT_MWH,

    LCD_UNIT_VARH,
    LCD_UNIT_KVARH,
    LCD_UNIT_MVARH,

    LCD_UNIT_VAH,
    LCD_UNIT_KVAH,
    LCD_UNIT_MVAH,

    LCD_UNIT_HZ,
    LCD_UNIT_KHZ,
    LCD_UNIT_MHZ,
};

/**
  * @brief  format of date
  */
enum __lcd_date_format
{
    //只有年月日
    LCD_DATE_YYMMDD = 0x01,
    LCD_DATE_DDMMYY = 0x02,
    LCD_DATE_MMDDYY = 0x04,

    //只有时分秒
    LCD_DATE_hhmmss = 0x08,
    LCD_DATE_ssmmhh = 0x10,
    LCD_DATE_mmsshh = 0x20,

    //只有月日时分
    LCD_DATE_MMDDhhmm = 0x41,
    LCD_DATE_DDMMhhmm = 0x42,
    LCD_DATE_MMDDmmhh = 0x43,
    LCD_DATE_DDMMmmhh = 0x44,
};


enum __win_show
{
    LCD_WIN_SHOW_BIN = 0,
    LCD_WIN_SHOW_DEC,
    LCD_WIN_SHOW_HEX,
    LCD_WIN_SHOW_DATE,
    LCD_WIN_SHOW_MSG,
    LCD_WIN_SHOW_NONE,
    LCD_WIN_SHOW_ALL,
};

enum __label_status
{
    LCD_LAB_SHOW_OFF = 0,
    LCD_LAB_SHOW_ON,
    LCD_LAB_SHOW_FLASH,
};

enum __glo_option
{
    LCD_GLO_NONE_OPTION = 0,
    LCD_GLO_SHOW_NONE,
    LCD_GLO_SHOW_ALL,

};

enum __bkl_option
{
    LCD_BKL_OFF = 0,
    LCD_BKL_ON,

};

struct __win_lcd_message
{
    struct
    {
        enum __lcd_dot          dot;
        enum __lcd_unit         unit;
        enum __lcd_date_format  format;
        enum __win_show         type;

        union
        {
            uint16_t            bin;
            int32_t             dec;
            uint32_t            hex;
            uint64_t            date;
            uint8_t             message[8];

        }                       value;

    }                           windows[LCD_MAX_WINDOWS];

    struct
    {
        enum __label_status     status;

        uint8_t                 value;

    }                           label[LCD_MAX_LABELS];

    enum __glo_option           global;

    enum __bkl_option           backlight;

};

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
