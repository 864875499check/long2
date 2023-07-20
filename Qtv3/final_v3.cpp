/******************************************************************
Copyright © Song zhihan Co., Ltd. 1990-2023. All rights reserved.
* @projectName   QtTerminal
* @brief         mainwindow.cpp
* @author        Song zhihan
* @email         1344358506@qq.com
* @date          2023-06-18
*******************************************************************/
#include "mainwindow.h"
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QRect>
#include <string.h>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 布局初始化 */
    layoutInit();

    /* 扫描系统的串口 */
    scanSerialPort();

    /* 波特率项初始化 */
    baudRateItemInit();

    /* 数据位项初始化 */
    dataBitsItemInit();

    /* 检验位项初始化 */
    parityItemInit();

    /* 停止位项初始化 */
    stopBitsItemInit();




    /***********************************test***********************************/

        /*
    timer[0]->start(200);
    timer[1]->start(1000);
    timer[2]->start(2000);


    connect(timer[0], SIGNAL(timeout()), this, SLOT(timerTimeOut()));
    connect(timer[1], SIGNAL(timeout()), this, SLOT(timerTimeOut1()));
    connect(timer[2], SIGNAL(timeout()), this, SLOT(timerTimeOut2()));



    qsrand(time(NULL));
*/
    /************************************************************************/

}

void MainWindow::layoutInit()
{
    /* 获取屏幕的分辨率，Qt官方建议使用这
     * 种方法获取屏幕分辨率，防上多屏设备导致对应不上
     * 注意，这是获取整个桌面系统的分辨率
     */
    QList <QScreen *> list_screen =  QGuiApplication::screens();

    /* 如果是ARM平台，直接设置大小为屏幕的大小 */
#if __arm__
    /* 重设大小 */
    this->resize(list_screen.at(0)->geometry().width(),
                 list_screen.at(0)->geometry().height());
#else
    /* 否则则设置主窗体大小为800x480 */
    this->resize(1800, 1000);
#endif
    /* 初始化 */
    serialPort = new QSerialPort(this);
    textBrowser = new QTextBrowser();
    textEdit = new QTextEdit();
    vboxLayout = new QVBoxLayout();
    funcWidget = new QWidget(this);

    NoteWidget = new QWidget(this);
    mainWidget = new QWidget(this);
    ReadWriteWidget = new QWidget(this);
    gridLayout = new QGridLayout();
    gridLayoutChart = new QGridLayout();
    gridLayoutNote = new QGridLayout();
    WRLayoutNote = new QGridLayout();
    ScrollArea_t = new QScrollArea(this);




    chartWidget = new QWidget();
    ScrollArea_t->setWidget(chartWidget);


    ReadWriteWidget->setGeometry(0, 0, 1800, 100);

    chartWidget->setGeometry(0, 0, 2000, 1500);
    ScrollArea_t->setGeometry(0, 200, 1800, 600);
    NoteWidget->setGeometry(0, 800, 1800, 80);
    funcWidget->setGeometry(0, 900, 1800, 100);

    //mainWidget->setGeometry(0, 600, 1800, 300);
    //NoteWidget->setGeometry(0, 800, 1800, 900);

    //funcWidget->setGeometry(0, 900, 1800, 1080);

    //NoteWidget->setGeometry(0, 700, 1600, 800);

    allData = new QByteArray();

    /***************************标志******************************************/
    label_txt[0] = new QLabel(this);
    label_txt[0]->setGeometry(200, 100, 200, 50);
    label_txt[0]->setText("LoongOS");
    label_txt[0]->setStyleSheet(
                    "QLabel{border:0px solid rgb(200, 200, 200);"
                    "font-size:45px;}"
                );



    label_txt[1] = new QLabel(this);
    label_txt[1]->setGeometry(1200, 100, 500, 50);

    label_txt[1]->setText("工业无线物联网传感系统");
    label_txt[1]->setStyleSheet(
                    "QLabel{border:0px solid rgb(200, 200, 200);"
                    "font-size:45px;}"
                );
    //lcdNumber->setGeometry(620, 100, 500, 50);
    /*********************************************************************/




    /**********************************************打印测试**********************************************/
    /*
    QFile file("/home/songzhihan/Qt/data.txt");
        if(file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append)){
        QDateTime dateTimeOut= QDateTime::currentDateTime();
        // 将内容写入文件
        QTextStream out(&file);

        out << dateTimeOut.toString("yyyy-MM-dd hh:mm:ss") << "\r\n";
        out << dateTimeOut.toString("yyyy-MM-dd hh:mm:ss") << "\r\n";
        out << dateTimeOut.toString("yyyy-MM-dd hh:mm:ss") << "\r\n";
        out << dateTimeOut.toString("yyyy-MM-dd hh:mm:ss") << "\r\n";

        file.close();
        qDebug()<<"completed.";
        }else{
        qDebug()<<"failed to create a new file!";
        }

        */
    /**********************************************************************************************/

    /*****************************时间********************************/



    /* 实例化与设置显示的位置大小*/
    lcdNumber = new QLCDNumber(this);
    lcdNumber->setGeometry(620, 100, 500, 50);

    /* 设置显示的位数8位 */
    lcdNumber->setDigitCount(19);
    /* 设置样式 */
    lcdNumber->setSegmentStyle(QLCDNumber::Flat);

    /* 设置lcd显示为当前系统时间 */
    //QTime time_lcd_t = QTime::currentTime();
    //QData data_lcd_t = QDate::currentDate();
    QDateTime dateTime= QDateTime::currentDateTime();

    /* 设置显示的样式 */
    //lcdNumber->display(data_lcd_t.toString() + time_lcd_t.toString("hh:mm:ss"));
    lcdNumber->setStyleSheet(
                    "QLCDNumber{border:2px solid rgb(0, 0, 0);"
                );

    lcdNumber->display(dateTime.toString("yyyy-MM-dd hh:mm:ss"));//格式化时间
    lcdTimer = new QTimer(this);
    /* 设置定时器1000毫秒发送一个timeout()信号 */
    lcdTimer->start(1000);

    /* 信号槽连接 */
    connect(lcdTimer, SIGNAL(timeout()), this,
            SLOT(timerTimeOut_LcdTime()));



    /*****************************************************************/


    /***************************报警**************************************/

    QPixmap pixmap(":/images/green.png");
    for(int m=0;m<3;m++)
    {
    labelImage[m] = new QLabel(this);
    labelImage[m]->setPixmap(pixmap);
    labelImage[m]->setScaledContents(true);
    }
    labelImage[0]->setGeometry(265, 150, 50, 50);
    labelImage[1]->setGeometry(795, 150, 50, 50);
    labelImage[2]->setGeometry(1325, 150, 50, 50);


    /*test*/
    /*
    if(1){
    //if(((float)((pdata[13]<<8)+pdata[14])) >= 2000.0 ){
        QPixmap pixmap_red(":/images/red.png");
        labelImage[0]->setPixmap(pixmap_red);

    }
    */
    /*****************************************************************/



    /*****************************图表********************************/
        /* 最大储存maxSize - 1个数据 */
        maxSize = 51;
        /* x轴上的最大值 */
        maxX = 5000;
        /* y轴最大值 */
        maxY = 40;
        QList <QString> listChart;
        listChart<<"温度:"<<"湿度:"<<"CO2:"<<"光强:"<<"烟雾浓度:";

        QList <QString> UnitChart;
        UnitChart<<"温度/℃:"<<"湿度/%:"<<"CO2/ppm:"<<"光强:"<<"烟雾浓度:";

        int span[5][2]={{0,40},{0,100},{0,3000},{0,5000},{0,3000}};

        for(int i=0;i<=4;i++)
        {
        /* splineSeries曲线实例化（折线用QLineSeries） */
        splineSeries[i] = new QSplineSeries();
        /* 图表实例化 */
        chart[i] = new QChart();
        /* 图表视图实例化 */
        chartView[i] = new QChartView();

        /* 坐标轴 */
        axisY[i] = new QValueAxis();
        axisX[i] = new QValueAxis();
        /* 定时器 */
        timer[i/5] = new QTimer(this);

        /* legend译图例类型，以绘图的颜色区分，本例设置为隐藏 */
        chart[i]->legend()->hide();
        /* chart设置标题 */
        chart[i]->setTitle("节点" + QString::number(i/5+1) + listChart[i]);
        /* 添加一条曲线splineSeries */
        chart[i]->addSeries(splineSeries[i]);

        /* 设置显示格式 */
        axisY[i]->setLabelFormat("%i");
        /* y轴标题 */
        axisY[i]->setTitleText(UnitChart[i]);
        /* y轴标题位置（设置坐标轴的方向） */
        chart[i]->addAxis(axisY[i], Qt::AlignLeft);
        /* 设置y轴范围 */
        axisY[i]->setRange(span[i][0], span[i][1]);
        /* 将splineSeries附加于y轴上 */
        splineSeries[i]->attachAxis(axisY[i]);

        /* 设置显示格式 */
        axisX[i]->setLabelFormat("%i");
        /* x轴标题 */
        axisX[i]->setTitleText("时间/ms");
        /* x轴标题位置（设置坐标轴的方向） */
        chart[i]->addAxis(axisX[i], Qt::AlignBottom);
        /* 设置x轴范围 */
        axisX[i]->setRange(0, maxX);
        /* 将splineSeries附加于x轴上 */
        splineSeries[i]->attachAxis(axisX[i]);

        /* 将图表的内容设置在图表视图上 */
        chartView[i]->setChart(chart[i]);
        /* 设置抗锯齿 */
        chartView[i]->setRenderHint(QPainter::Antialiasing);
        }

        for(int i=5;i<=9;i++)
        {
        /* splineSeries曲线实例化（折线用QLineSeries） */
        splineSeries[i] = new QSplineSeries();
        /* 图表实例化 */
        chart[i] = new QChart();
        /* 图表视图实例化 */
        chartView[i] = new QChartView();

        /* 坐标轴 */
        axisY[i] = new QValueAxis();
        axisX[i] = new QValueAxis();
        /* 定时器 */
        timer[i/5] = new QTimer(this);

        /* legend译图例类型，以绘图的颜色区分，本例设置为隐藏 */
        chart[i]->legend()->hide();
        /* chart设置标题 */
        chart[i]->setTitle("节点" + QString::number(i/5 + 1) + listChart[i-5]);
        /* 添加一条曲线splineSeries */
        chart[i]->addSeries(splineSeries[i]);

        /* 设置显示格式 */
        axisY[i]->setLabelFormat("%i");
        /* y轴标题 */
        axisY[i]->setTitleText(UnitChart[i-5]);
        /* y轴标题位置（设置坐标轴的方向） */
        chart[i]->addAxis(axisY[i], Qt::AlignLeft);
        /* 设置y轴范围 */
        axisY[i]->setRange(span[i-5][0], span[i-5][1]);
        /* 将splineSeries附加于y轴上 */
        splineSeries[i]->attachAxis(axisY[i]);

        /* 设置显示格式 */
        axisX[i]->setLabelFormat("%i");
        /* x轴标题 */
        axisX[i]->setTitleText("时间/ms");
        /* x轴标题位置（设置坐标轴的方向） */
        chart[i]->addAxis(axisX[i], Qt::AlignBottom);
        /* 设置x轴范围 */
        axisX[i]->setRange(0, maxX);
        /* 将splineSeries附加于x轴上 */
        splineSeries[i]->attachAxis(axisX[i]);

        /* 将图表的内容设置在图表视图上 */
        chartView[i]->setChart(chart[i]);
        /* 设置抗锯齿 */
        chartView[i]->setRenderHint(QPainter::Antialiasing);
        }

        for(int i=10;i<=14;i++)
        {
        /* splineSeries曲线实例化（折线用QLineSeries） */
        splineSeries[i] = new QSplineSeries();
        /* 图表实例化 */
        chart[i] = new QChart();
        /* 图表视图实例化 */
        chartView[i] = new QChartView();

        /* 坐标轴 */
        axisY[i] = new QValueAxis();
        axisX[i] = new QValueAxis();
        /* 定时器 */
        timer[i/5] = new QTimer(this);

        /* legend译图例类型，以绘图的颜色区分，本例设置为隐藏 */
        chart[i]->legend()->hide();
        /* chart设置标题 */
        chart[i]->setTitle("节点" + QString::number(i/5 + 1 ) + listChart[i-10]);
        /* 添加一条曲线splineSeries */
        chart[i]->addSeries(splineSeries[i]);

        /* 设置显示格式 */
        axisY[i]->setLabelFormat("%i");
        /* y轴标题 */
        axisY[i]->setTitleText(UnitChart[i-10]);
        /* y轴标题位置（设置坐标轴的方向） */
        chart[i]->addAxis(axisY[i], Qt::AlignLeft);
        /* 设置y轴范围 */
        axisY[i]->setRange(span[i-10][0], span[i-10][1]);
        /* 将splineSeries附加于y轴上 */
        splineSeries[i]->attachAxis(axisY[i]);

        /* 设置显示格式 */
        axisX[i]->setLabelFormat("%i");
        /* x轴标题 */
        axisX[i]->setTitleText("时间/ms");
        /* x轴标题位置（设置坐标轴的方向） */
        chart[i]->addAxis(axisX[i], Qt::AlignBottom);
        /* 设置x轴范围 */
        axisX[i]->setRange(0, maxX);
        /* 将splineSeries附加于x轴上 */
        splineSeries[i]->attachAxis(axisX[i]);

        /* 将图表的内容设置在图表视图上 */
        chartView[i]->setChart(chart[i]);
        /* 设置抗锯齿 */
        chartView[i]->setRenderHint(QPainter::Antialiasing);
        }

        /******************************************************************/




        /*******************************电量*******************************/
    for(int i=0;i<3;i++){
    progressBar[i] = new QProgressBar(this);
    //progressBar[i]->setGeometry(300, 200, 200, 60);

    /*样式表设置，常用使用setStyleSheet来设置样式（实现界面美化的功能），
     * 具体可参考seTyleSheet */
    progressBar[i]->setStyleSheet (
                "QProgressBar{border:8px solid #FFFFFF;"
                "height:30;"
                "border-image:url(:/images/battery.png);" //背景图片
                "text-align:center;"    // 文字居中
                "color:rgb(255,0,0);"
                "font:20px;"      // 字体大小为20px
                "border-radius:10px;}"
                "QProgressBar::chunk{"
                "border-radius:5px;" // 斑马线圆角
                "border:1px solid black;" // 黑边，默认无边
                "background-color:green;"
                "width:10px;margin:1px;}" // 宽度和间距
                );

    /* 设置progressBar的范围值 */
    progressBar[i]->setRange(0, 100);
    /* 初始化value为0 */
    value = 0;
    /* 给progressBar设置当前值 */
    progressBar[i]->setValue(value);

    /* 设置当前文本字符串的显示格式 */
    //progressBar[i]->setFormat("充电中%p%");


    progressBar[i]->setValue(value);
    /* 若value值大于100，令value再回到0 */
    if(value>100)
        value = 0;
}
        /******************************************************************/



    /* QList链表，字符串类型 */
    QList <QString> list1;
    list1<<"串口号:"<<"波特率:"<<"数据位:"<<"检验位:"<<"停止位:";

    for (int i = 0; i < 5; i++) {
        label[i] = new QLabel(list1[i]);
        /* 设置最小宽度与高度 */
        label[i]->setMinimumSize(80, 30);
        label[i]->setMaximumSize(400, 40);
        /* 自动调整label的大小 */
        label[i]->setSizePolicy(
                    QSizePolicy::Expanding,
                    QSizePolicy::Expanding
                    );
        /* 将label[i]添加至网格的坐标(0, i) */
        gridLayout->addWidget(label[i], 0, i);
    }

    for (int i = 0; i < 5; i++) {
        comboBox[i] = new QComboBox();
        comboBox[i]->setMinimumSize(80, 30);
        comboBox[i]->setMaximumSize(400, 40);
        /* 自动调整label的大小 */
        comboBox[i]->setSizePolicy(
                    QSizePolicy::Expanding,
                    QSizePolicy::Expanding
                    );
        /* 将comboBox[i]添加至网格的坐标(1, i) */
        gridLayout->addWidget(comboBox[i], 1, i);
    }

    /* QList链表，字符串类型 */
    QList <QString> list2;
    list2<<"发送"<<"打开串口";

    for (int i = 0; i < 2; i++) {
        pushButton[i] = new QPushButton(list2[i]);
        pushButton[i]->setMinimumSize(80, 30);
        pushButton[i]->setMaximumSize(400, 40);
        /* 自动调整label的大小 */
        pushButton[i]->setSizePolicy(
                    QSizePolicy::Expanding,
                    QSizePolicy::Expanding
                    );
        /* 将pushButton[0]添加至网格的坐标(i, 5) */
        gridLayout->addWidget(pushButton[i], i, 5);
    }
    pushButton[0]->setEnabled(false);

    QList <QString> list3;
    list3<<"唤醒"<<"休眠"<<"唤醒"<<"休眠"<<"唤醒"<<"休眠";
    for (int i = 0; i < 6; i++) {
        pushButtonNote[i] = new QPushButton(list3[i]);
        pushButtonNote[i]->setMinimumSize(80, 30);
        pushButtonNote[i]->setMaximumSize(400, 40);
        /* 自动调整label的大小 */
        pushButtonNote[i]->setSizePolicy(
                    QSizePolicy::Expanding,
                    QSizePolicy::Expanding
                    );
    }

    gridLayoutNote->addWidget(progressBar[0], 0, 0);
    gridLayoutNote->addWidget(pushButtonNote[0], 0, 1);
    gridLayoutNote->addWidget(pushButtonNote[1], 0, 2);

    gridLayoutNote->addWidget(progressBar[1], 0, 3);
    gridLayoutNote->addWidget(pushButtonNote[2], 0, 4);
    gridLayoutNote->addWidget(pushButtonNote[3], 0, 5);

    gridLayoutNote->addWidget(progressBar[2], 0, 6);
    gridLayoutNote->addWidget(pushButtonNote[4], 0, 7);
    gridLayoutNote->addWidget(pushButtonNote[5], 0, 8);
    pushButtonNote[1]->setEnabled(false);
    pushButtonNote[3]->setEnabled(false);
    pushButtonNote[5]->setEnabled(false);

        /* 布局 */
        WRLayoutNote->addWidget(textBrowser, 0, 0);
        WRLayoutNote->addWidget(textEdit, 0, 1);
        ReadWriteWidget->setLayout(WRLayoutNote);
        //vboxLayout->addWidget(ReadWriteWidget);

        gridLayoutChart->addWidget(chartView[0], 0, 0);
        gridLayoutChart->addWidget(chartView[1], 1, 0);
        gridLayoutChart->addWidget(chartView[2], 2, 0);
        gridLayoutChart->addWidget(chartView[3], 3, 0);
        gridLayoutChart->addWidget(chartView[4], 4, 0);
        gridLayoutChart->addWidget(chartView[5], 0, 1);
        gridLayoutChart->addWidget(chartView[6], 1, 1);
        gridLayoutChart->addWidget(chartView[7], 2, 1);
        gridLayoutChart->addWidget(chartView[8], 3, 1);
        gridLayoutChart->addWidget(chartView[9], 4, 1);
        gridLayoutChart->addWidget(chartView[10], 0, 2);
        gridLayoutChart->addWidget(chartView[11], 1, 2);
        gridLayoutChart->addWidget(chartView[12], 2, 2);
        gridLayoutChart->addWidget(chartView[13], 3, 2);
        gridLayoutChart->addWidget(chartView[14], 4, 2);


        chartWidget->setLayout(gridLayoutChart);
        //vboxLayout->addWidget(chartWidget);
        //vboxLayout->addWidget(ScrollArea_t);

        NoteWidget->setLayout(gridLayoutNote);
        //vboxLayout->addWidget(NoteWidget);

        funcWidget->setLayout(gridLayout);
        //vboxLayout->addWidget(funcWidget);

        //mainWidget->setLayout(vboxLayout);
        //this->setCentralWidget(mainWidget);

    /* 占位文本 */
    textBrowser->setPlaceholderText("接收到的消息");
    textEdit->setText("LoongOS");

    /* 信号槽连接 */
    connect(pushButton[0], SIGNAL(clicked()),
            this, SLOT(sendPushButtonClicked()));
    connect(pushButton[1], SIGNAL(clicked()),
            this, SLOT(openSerialPortPushButtonClicked()));

    connect(pushButtonNote[0], SIGNAL(clicked()),
            this, SLOT(NoteWake0()));
    connect(pushButtonNote[1], SIGNAL(clicked()),
            this, SLOT(ResWake0()));
    connect(pushButtonNote[2], SIGNAL(clicked()),
            this, SLOT(NoteWake1()));
    connect(pushButtonNote[3], SIGNAL(clicked()),
            this, SLOT(ResWake1()));
    connect(pushButtonNote[4], SIGNAL(clicked()),
            this, SLOT(NoteWake2()));
    connect(pushButtonNote[5], SIGNAL(clicked()),
            this, SLOT(ResWake2()));

    connect(serialPort, SIGNAL(readyRead()),
            this, SLOT(serialPortReadyRead()));
}


/*发送数据*/
void MainWindow::NoteWake0()
{
    textEdit->setText("Dev11eD");	// 设备0|1-唤醒|eD-end
    QByteArray data = textEdit->toPlainText().toUtf8();
    pushButtonNote[0]->setEnabled(false);
    pushButtonNote[1]->setEnabled(true);
    serialPort->write(data);
}

void MainWindow::ResWake0()
{
    textEdit->setText("Dev10eD");	// 设备0|0-休眠|eD-end
    QByteArray data = textEdit->toPlainText().toUtf8();
    serialPort->write(data);
    pushButtonNote[0]->setEnabled(true);
    pushButtonNote[1]->setEnabled(false);
}

void MainWindow::NoteWake1()
{
    textEdit->setText("Dev21eD");	// 设备1|1-唤醒|eD-end
    QByteArray data = textEdit->toPlainText().toUtf8();
    serialPort->write(data);
    pushButtonNote[2]->setEnabled(false);
    pushButtonNote[3]->setEnabled(true);
}

void MainWindow::ResWake1()
{
    textEdit->setText("Dev20eD");	// 设备1|0-休眠|eD-end
    QByteArray data = textEdit->toPlainText().toUtf8();
    serialPort->write(data);
    pushButtonNote[2]->setEnabled(true);
    pushButtonNote[3]->setEnabled(false);
}

void MainWindow::NoteWake2()
{
    textEdit->setText("Dev31eD");	// 设备2|1-唤醒|eD-end
    QByteArray data = textEdit->toPlainText().toUtf8();
    serialPort->write(data);
    pushButtonNote[4]->setEnabled(false);
    pushButtonNote[5]->setEnabled(true);
}

void MainWindow::ResWake2()
{
    textEdit->setText("Dev30eD");	// 设备2|0-休眠|eD-end
    QByteArray data = textEdit->toPlainText().toUtf8();
    serialPort->write(data);
    pushButtonNote[4]->setEnabled(true);
    pushButtonNote[5]->setEnabled(false);
}

void MainWindow::scanSerialPort()
{
    /* 查找可用串口 */
    foreach (const QSerialPortInfo &info,
            QSerialPortInfo::availablePorts()) {
        comboBox[0]->addItem(info.portName());
    }
}

void MainWindow::baudRateItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"1200"<<"2400"<<"4800"<<"9600"
       <<"19200"<<"38400"<<"57600"
      <<"115200"<<"230400"<<"460800"
     <<"921600";
    for (int i = 0; i < 11; i++) {
        comboBox[1]->addItem(list[i]);
    }
    comboBox[1]->setCurrentIndex(7);
}

void MainWindow::dataBitsItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"5"<<"6"<<"7"<<"8";
    for (int i = 0; i < 4; i++) {
        comboBox[2]->addItem(list[i]);
    }
    comboBox[2]->setCurrentIndex(3);
}

void MainWindow::parityItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"None"<<"Even"<<"Odd"<<"Space"<<"Mark";
    for (int i = 0; i < 5; i++) {
        comboBox[3]->addItem(list[i]);
    }
    comboBox[3]->setCurrentIndex(0);
}

void MainWindow::stopBitsItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"1"<<"2";
    for (int i = 0; i < 2; i++) {
        comboBox[4]->addItem(list[i]);
    }
    comboBox[4]->setCurrentIndex(0);
}

void MainWindow::sendPushButtonClicked()
{
    /* 获取textEdit数据,转换成utf8格式的字节流 */
    QByteArray data = textEdit->toPlainText().toUtf8();
    serialPort->write(data);
}

void MainWindow::openSerialPortPushButtonClicked()
{
    if (pushButton[1]->text() == "打开串口") {
        /* 设置串口名 */
        serialPort->setPortName(comboBox[0]->currentText());
        /* 设置波特率 */
        serialPort->setBaudRate(comboBox[1]->currentText().toInt());
        /* 设置数据位数 */
        switch (comboBox[2]->currentText().toInt()) {
        case 5:
            serialPort->setDataBits(QSerialPort::Data5);
            break;
        case 6:
            serialPort->setDataBits(QSerialPort::Data6);
            break;
        case 7:
            serialPort->setDataBits(QSerialPort::Data7);
            break;
        case 8:
            serialPort->setDataBits(QSerialPort::Data8);
            break;
        default: break;
        }
        /* 设置奇偶校验 */
        switch (comboBox[3]->currentIndex()) {
        case 0:
            serialPort->setParity(QSerialPort::NoParity);
            break;
        case 1:
            serialPort->setParity(QSerialPort::EvenParity);
            break;
        case 2:
            serialPort->setParity(QSerialPort::OddParity);
            break;
        case 3:
            serialPort->setParity(QSerialPort::SpaceParity);
            break;
        case 4:
            serialPort->setParity(QSerialPort::MarkParity);
            break;
        default: break;
        }
        /* 设置停止位 */
        switch (comboBox[4]->currentText().toInt()) {
        case 1:
            serialPort->setStopBits(QSerialPort::OneStop);
            break;
        case 2:
            serialPort->setStopBits(QSerialPort::TwoStop);
            break;
        default: break;
        }
        /* 设置流控制 */
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        if (!serialPort->open(QIODevice::ReadWrite))
            QMessageBox::about(NULL, "错误",
                               "串口无法打开！可能串口已经被占用！");
        else {
            for (int i = 0; i < 5; i++)
                comboBox[i]->setEnabled(false);
            pushButton[1]->setText("关闭串口");
            pushButton[0]->setEnabled(true);
        }
    } else {
        serialPort->close();
        for (int i = 0; i < 5; i++)
            comboBox[i]->setEnabled(true);
        pushButton[1]->setText("打开串口");
        pushButton[0]->setEnabled(false);
    }
}

#define DATA_TYPE_TEMP	1
#define DATA_TYPE_HUMI	2
#define DATA_TYPE_CO2	3
#define DATA_TYPE_LIGHT	4
#define DATA_TYPE_GAS	5
/*接收数据处理*/
void MainWindow::serialPortReadyRead()
{
    /* 接收缓冲区中读取数据 */

    using namespace std;

    QByteArray dataDuf;
    dataDuf	= serialPort->readAll();

    (*allData).append(dataDuf);
    //textBrowser->insertPlainText((*allData));
    //if((*allData).contains('6') || (*allData).contains('n') || (*allData).size()>=14){
    if( (*allData).contains('\n') ){


    //QByteArray allDataChar=allData->toLatin1();
    unsigned char *pdata = (unsigned char *)((*allData).data());
    //dataDuf	= serialPort->readAll();
    //unsigned char *pdata2 = (unsigned char *)(dataDuf.data());



     if(pdata[0]==0x21&&pdata[1]==0x16&&pdata[2]==0x01)		// 节点1
    {
        if(pdata[3]==0xDA)	//	节点1数据
        {
            //QByteArray dev0("dev1 data\r\n");
            //textBrowser->insertPlainText(dev0);	// 提示接收到设备1数据

            receivedData(0, DATA_TYPE_TEMP, 	(175.0*(float)((pdata[15]<<8)+pdata[16])/65535.0-45.0));
            receivedData(0, DATA_TYPE_HUMI, 	(100.0*(float)((pdata[17]<<8)+pdata[18])/65535.0));
            receivedData(0, DATA_TYPE_CO2, 		(float)((pdata[19]<<8)+pdata[20]));
            receivedData(0, DATA_TYPE_LIGHT, 	(float)((pdata[11]<<8)+pdata[12]));
            receivedData(0, DATA_TYPE_GAS, 		(float)((pdata[13]<<8)+pdata[14]));

            receivedPowerData(0, (float)pdata[10]);
        }
        else if(pdata[3]==0x01)	// 节点1上线
        {
            QByteArray dev1("End_device<1> ONline!\r\n");
            textBrowser->insertPlainText(dev1);
            pushButtonNote[0]->setEnabled(false);
            pushButtonNote[1]->setEnabled(true);
        }
        else if(pdata[3]==0x00)	// 节点1下线
        {
            QByteArray dev1("End_device<1> OFFline!\r\n");
            textBrowser->insertPlainText(dev1);
            pushButtonNote[0]->setEnabled(true);
            pushButtonNote[1]->setEnabled(false);
        }
    }
    else if(pdata[0]==0x21&&pdata[1]==0x16&&pdata[2]==0x02)	// 节点2
    {
        if(pdata[3]==0xDA)	//	节点2数据
        {
            //QByteArray dev1("dev2 data\r\n");
            //textBrowser->insertPlainText(dev1);	// 提示接收到设备2数据

            receivedData(1, DATA_TYPE_TEMP, 	(175.0*(float)((pdata[15]<<8)+pdata[16])/65535.0-45.0));
            receivedData(1, DATA_TYPE_HUMI, 	(100.0*(float)((pdata[17]<<8)+pdata[18])/65535.0));
            receivedData(1, DATA_TYPE_CO2, 		(float)((pdata[19]<<8)+pdata[20]));
            receivedData(1, DATA_TYPE_LIGHT, 	(float)((pdata[11]<<8)+pdata[12]));
            receivedData(1, DATA_TYPE_GAS, 		(float)((pdata[13]<<8)+pdata[14]));

            receivedPowerData(1, (float)pdata[10]);
        }
        else if(pdata[3]==0x01)	// 节点2上线
        {
            QByteArray dev2("End_device<2> ONline!\r\n");
            textBrowser->insertPlainText(dev2);
            pushButtonNote[2]->setEnabled(false);
            pushButtonNote[3]->setEnabled(true);
        }
        else if(pdata[3]==0x00)	// 节点2下线
        {
            QByteArray dev2("End_device<2> OFFline!\r\n");
            textBrowser->insertPlainText(dev2);
            pushButtonNote[2]->setEnabled(true);
            pushButtonNote[3]->setEnabled(false);
        }
    }
    else if(pdata[0]==0x21&&pdata[1]==0x16&&pdata[2]==0x03)	// 节点3
    {
        if(pdata[3]==0xDA)	//	节点3数据
        {
            //QByteArray dev2("dev3 data\r\n");
            //textBrowser->insertPlainText(dev2);	// 提示接收到设备3数据

            receivedData(2, DATA_TYPE_TEMP, 	(175.0*(float)((pdata[15]<<8)+pdata[16])/65535.0-45.0));
            receivedData(2, DATA_TYPE_HUMI, 	(100.0*(float)((pdata[17]<<8)+pdata[18])/65535.0));
            receivedData(2, DATA_TYPE_CO2, 		(float)((pdata[19]<<8)+pdata[20]));
            receivedData(2, DATA_TYPE_LIGHT, 	(float)((pdata[11]<<8)+pdata[12]));
            receivedData(2, DATA_TYPE_GAS, 		(float)((pdata[13]<<8)+pdata[14]));

            receivedPowerData(2, (float)pdata[10]);
        }
        else if(pdata[3]==0x01)	// 节点3上线
        {
            QByteArray dev3("End_device<3> ONline!\r\n");
            textBrowser->insertPlainText(dev3);
            pushButtonNote[4]->setEnabled(false);
            pushButtonNote[5]->setEnabled(true);
        }
        else if(pdata[3]==0x00)	// 节点3下线
        {
            QByteArray dev3("End_device<3> OFFline!\r\n");
            textBrowser->insertPlainText(dev3);
            pushButtonNote[4]->setEnabled(true);
            pushButtonNote[5]->setEnabled(false);
        }
    }

    /***************************************保存数据*********************************************/
    if(pdata[0]==0x21&&pdata[1]==0x16&&pdata[3]==0xDA)
    {
        QFile file("/home/loongson/project/data.txt");
        if(file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append)){
        QDateTime dateTimeOut= QDateTime::currentDateTime();
        // 将内容写入文件
        QTextStream out(&file);
        out << " ________节点 "<< pdata[2] << "_________"<< "\r\n";
        out << dateTimeOut.toString("yyyy-MM-dd hh:mm:ss") << "\r\n";
        out << "【__温度__】\t"<< (175.0*(float)((pdata[15]<<8)+pdata[16])/65535.0-45.0) << "℃" <<"\r\n";
        out << "【__湿度__】\t"<< (100.0*(float)((pdata[17]<<8)+pdata[18])/65535.0) << "%" <<"\r\n";
        out << "【二氧化碳】\t"<< (float)((pdata[19]<<8)+pdata[20]) << "ppm" <<"\r\n";
        out << "【光照强度】\t"<< (float)((pdata[11]<<8)+pdata[12]) <<"\r\n";
        out << "【烟雾浓度】\t"<< (float)((pdata[13]<<8)+pdata[14]) <<"\r\n";
        out << "\r\n";

        file.close();
        qDebug()<<"completed.";
        }else{
        qDebug()<<"failed to create a new file!";
        }
    /******************************************************************************************/



    /***************************************报警检测*********************************************/

    if(((float)((pdata[13]<<8)+pdata[14])) >= 2000.0 ){
        QPixmap pixmap_red(":/images/red.png");
        labelImage[(int)pdata[2]-1]->setPixmap(pixmap_red);

    }
    else{
        QPixmap pixmap_green(":/images/green.png");
        labelImage[(int)pdata[2]-1]->setPixmap(pixmap_green);
    }

    if((float)pdata[10] <= 30 )
    {
    /* 设置当前文本字符串的显示格式 */
    progressBar[(int)pdata[2]-1]->setFormat("电量低%p%");

    /*样式表设置，常用使用setStyleSheet来设置样式（实现界面美化的功能），
     * 具体可参考seTyleSheet */
    progressBar[(int)pdata[2]-1]->setStyleSheet (
                "QProgressBar{border:8px solid #FFFFFF;"
                "height:30;"
                "border-image:url(:/images/battery.png);" //背景图片
                "text-align:center;"    // 文字居中
                "color:rgb(0,0,0);"
                "font:20px;"      // 字体大小为20px
                "border-radius:10px;}"
                "QProgressBar::chunk{"
                "border-radius:5px;" // 斑马线圆角
                "border:1px solid black;" // 黑边，默认无边
                "background-color:red;"
                "width:10px;margin:1px;}" // 宽度和间距
                );

    }
    else if((float)pdata[10] > 30){
    /* 设置当前文本字符串的显示格式 */
    progressBar[(int)pdata[2]-1]->setFormat("%p%");

    /*样式表设置，常用使用setStyleSheet来设置样式（实现界面美化的功能），
     * 具体可参考seTyleSheet */
    progressBar[(int)pdata[2]-1]->setStyleSheet (
                "QProgressBar{border:8px solid #FFFFFF;"
                "height:30;"
                "border-image:url(:/images/battery.png);" //背景图片
                "text-align:center;"    // 文字居中
                "color:rgb(0,0,0);"
                "font:20px;"      // 字体大小为20px
                "border-radius:10px;}"
                "QProgressBar::chunk{"
                "border-radius:5px;" // 斑马线圆角
                "border:1px solid black;" // 黑边，默认无边
                "background-color:green;"
                "width:10px;margin:1px;}" // 宽度和间距
                );
    }

    }


    else if(pdata[0]==0x21&&pdata[1]==0x16){
        QFile file("/home/loongson/project/data.txt");
        if(file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append)){
        QDateTime dateTimeOut= QDateTime::currentDateTime();
        // 将内容写入文件
        QTextStream out(&file);


        if(pdata[3]==0x01)
        {
            out << " ****************************************************************************************** "<< "\r\n";
            out << dateTimeOut.toString("yyyy-MM-dd hh:mm:ss") << "  ";
            out << " ________节点 "<< pdata[2]<< " ONLine " << "_________"<< "\r\n";
            out << "\r\n";
        }
        else if(pdata[3]==0x00)
        {
            out << dateTimeOut.toString("yyyy-MM-dd hh:mm:ss") << "  ";
            out << " ________节点 "<< pdata[2]<< " OFFLine " << "_________"<< "\r\n";
            out << " ****************************************************************************************** "<< "\r\n";
            out << "\r\n";
        }



        file.close();
        qDebug()<<"completed.";
        }else{
        qDebug()<<"failed to create a new file!";
        }

    }
    /*
    (int)pdata[2]


    labelImage[m] = new QLabel(this);

    labelImage[m]->setScaledContents(true);

    labelImage[0]->setGeometry(265, 150, 50, 50);
    labelImage[1]->setGeometry(795, 150, 50, 50);
    labelImage[2]->setGeometry(1325, 150, 50, 50);
    */

    /******************************************************************************************/


    allData->clear();
    }

    //QByteArray winBuf = serialPort->readAll();
    //textBrowser->insertPlainText(winBuf);
}


void MainWindow::receivedData(int note,int note_type,float value)
{
    int num=note*5+note_type-1;

    /* 将数据添加到data中 */
    data_f[num].append(value);

    /* 当储存数据的个数大于最大值时，把第一个数据删除 */
    while (data_f[num].size() > maxSize) {
        /* 移除data中第一个数据 */
        data_f[num].removeFirst();
    }

    /* 先清空 */
    splineSeries[num]->clear();

    /* 计算x轴上的点与点之间显示的间距 */
    int xSpace = maxX / (maxSize - 1);

    /* 添加点，xSpace * i 表示第i个点的x轴的位置 */
    for (int i = 0; i < data_f[num].size(); ++i) {
        splineSeries[num]->append(xSpace * i, data_f[num].at(i));
    }

}

void MainWindow::receivedPowerData(int note,float value){
    progressBar[note]->setValue(value);
    /* 若value值大于100，令value再回到100 */
    if(value>100)
        value = 100;
}

void MainWindow::timerTimeOut()
{
    /* 产生随机0~maxY之间的数据 */
    //receivedData(0,1,qrand() % maxY);
    //receivedData(0,2,qrand() % maxY);
    //receivedData(0,3,qrand() % maxY);
    //receivedData(0,4,qrand() % maxY);
    //receivedData(0,5,qrand() % maxY);
}

void MainWindow::timerTimeOut1()
{
    /* 产生随机0~maxY之间的数据 */
    //receivedData(1,1,qrand() % maxY);
    //receivedData(1,2,qrand() % maxY);
    //receivedData(1,3,qrand() % maxY);
    //receivedData(1,4,qrand() % maxY);
    //receivedData(1,5,qrand() % maxY);
}

void MainWindow::timerTimeOut2()
{
    /* 产生随机0~maxY之间的数据 */
    //receivedData(2,1,qrand() % maxY);
    //receivedData(2,2,qrand() % maxY);
    //receivedData(2,3,qrand() % maxY);
    //receivedData(2,4,qrand() % maxY);
    //receivedData(2,5,qrand() % maxY);
}

void MainWindow::timerTimeOut_LcdTime()
{
    /* 当定时器计时1000毫秒后，刷新lcd显示当前系统时间 */

    //QTime time_lcd_t = QTime::currentTime();
    QDateTime dateTime= QDateTime::currentDateTime();
    /* 设置显示的样式 */
    //lcdNumber->display(time_lcd_t.toString("hh:mm:ss"));
    lcdNumber->display(dateTime.toString("yyyy-MM-dd hh:mm:ss"));
}


MainWindow::~MainWindow()
{
}



