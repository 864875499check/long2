/******************************************************************
Copyright © Song zhihan Co., Ltd. 1990-2023. All rights reserved.
* @projectName   QtTerminal
* @brief         mainwindow.cpp
* @author        Song zhihan
* @email         1344358506@qq.com
* @date          2023-06-18
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QChartView>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QValueAxis>

#include <QLCDNumber>
#include <QTime>


#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QProgressBar>

using namespace QtCharts;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /***********************图表************************/
    /* 接收数据接口 */
        //void receivedData(int);
        void receivedData(int note,int note_type,float value);

        /* 数据最大个数 */
        int maxSize;

        /* x轴上的最大值 */
        int maxX;

        /* y轴上的最大值 */
        int maxY;

        /* y轴 */
        QValueAxis *axisY[15];

        /* x轴 */
        QValueAxis *axisX[15];

        /* QList int类型容器 */
        QList<float> data_f[15];
        //QList<float> data_f[6];

        /* QSplineSeries对象（曲线）*/
        QSplineSeries *splineSeries[15];

        /* QChart图表 */
        QChart *chart[15];

        /* 图表视图 */
        QChartView *chartView[15];

        QScrollArea *ScrollArea_t;

        /* 定时器 */
        QTimer *timer[3];

        QByteArray *allData;

        QLabel *label_txt[2];

private slots:
    void timerTimeOut();
    void timerTimeOut1();
    void timerTimeOut2();

    /******************************************************/



    /**********************时间*****************************/

    private:
    /* 声明QLCDNumber对象 */
    QLCDNumber *lcdNumber;

    /* 声明QTimer对象 */
    QTimer *lcdTimer;

private slots:
    /* 槽函数 */
    void timerTimeOut_LcdTime();

    /******************************************************/
private:
    QLabel *labelImage[3];			//报警图表

    /**********************电量****************************/
private:
    /* 声明对象 */
    QProgressBar *progressBar[3];


    /* 用于设置当前QProgressBar的值 */
    int value;

    void receivedPowerData(int note,float value);
    /******************************************************/

private:
    /* 串口对象 */
    QSerialPort *serialPort;

    /* 用作接收数据 */
    QTextBrowser *textBrowser;

    /* 用作发送数据 */
    QTextEdit *textEdit;
    /*test*/
        QTextEdit *textEdit1;
        QTextEdit *textEdit2;
        QTextEdit *textEdit3;
        QTextEdit *textEdit4;
        QTextEdit *textEdit5;
        QTextEdit *textEdit6;
        QTextEdit *textEdit7;
        QTextEdit *textEdit8;
        QTextEdit *textEdit9;

    /* 按钮 */
    QPushButton *pushButton[2];

    QPushButton *pushButtonNote[6];

    /* 下拉选择盒子 */
    QComboBox *comboBox[5];

    /* 标签 */
    QLabel *label[5];

    /* 垂直布局 */
    QVBoxLayout *vboxLayout;

    /* 网络布局 */
    QGridLayout *gridLayout;
    QGridLayout *gridLayoutChart;
    QGridLayout *gridLayoutNote;
    QGridLayout *WRLayoutNote;

    /* 主布局 */
    QWidget *mainWidget;

    /* 设置功能区域 */
    QWidget *funcWidget;
    QWidget *chartWidget;
    QWidget *NoteWidget;
    QWidget *ReadWriteWidget;

    /* 布局初始化 */
    void layoutInit();

    /* 扫描系统可用串口 */
    void scanSerialPort();

    /* 波特率项初始化 */
    void baudRateItemInit();

    /* 数据位项初始化 */
    void dataBitsItemInit();

    /* 检验位项初始化 */
    void parityItemInit();

    /* 停止位项初始化 */
    void stopBitsItemInit();

private slots:
    void sendPushButtonClicked();
    void openSerialPortPushButtonClicked();
    void serialPortReadyRead();
    void NoteWake0();
    void ResWake0();
    void NoteWake1();
    void ResWake1();
    void NoteWake2();
    void ResWake2();
};
#endif // MAINWINDOW_H
