#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFuture>
#include <QLineEdit>
#include <QComboBox>
#include <QLCDNumber>
#include <QCheckBox>
#include <QIntValidator>
#include <QChartView>
#include <QElapsedTimer>
#include <QMainWindow>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>
#include <QProgressDialog>
#include "modbus.h"
#include "ui_about.h"
#include "modbus-rtu.h"
#include "modbus.h"
#include "qcgaugewidget.h"

#define RELEASE_VERSION             "0.0.7"
#define MAX_LOOP                    6
#define MAX_PIPE                    18
#define RAZ                         0 
#define EEA                         1 

#define STABILITY_CHECK				true
#define NO_STABILITY_CHECK			false

/// control box
#define CONTROLBOX                  100 

/// master pipe
#define MASTER_WATERCUT             3
#define MASTER_WRITE                1
#define MASTER_READ                 0

/// progress bar type
#define F_BAR                       1
#define T_BAR                       0
   
/// calibration file names
#define HIGH                        "\\HIGHCUT\\HC"
#define FULL                        "\\FULLCUT\\FC" 
#define MID                         "\\MIDCUT\\MC"
#define LOW                         "\\LOWCUT\\LC"

/// header lines
#define HEADER3                     "Time From  Water  Osc  Tune Tuning            Incident Reflected                         Analog     User Input  Injection";
#define HEADER4                     "Run Start   Cut   Band Type Voltage Frequency  Power     Power   Temperature Pressure    Input        Value       Time     Comment";
#define HEADER5                     "========= ======= ==== ==== ======= ========= ======== ========= =========== ======== ============ ============ ========== ============";

/// loop
#define L1                          0
#define L2                          1 
#define L3                          2 
#define L4                          3 
#define L5                          4 
#define L6                          5 

/// pipe
#define P1                          0
#define P2                          1
#define P3                          2

/// loop + pipe
#define L1P1                         0
#define L1P2                         1
#define L1P3                         2
#define L2P1                         3 
#define L2P2                         4 
#define L2P3                         5
#define L3P1                         6 
#define L3P2                         7 
#define L3P3                         8 
#define L4P1                         9 
#define L4P2                         10 
#define L4P3                         11 
#define L5P1                         12 
#define L5P2                         13 
#define L5P3                         14 
#define L6P1                         15 
#define L6P2                         16 
#define L6P3                         17 
#define ALL                          18

#define EEA_INJECTION_FILE          "EEA INJECTION FILE"
#define RAZ_INJECTION_FILE          "RAZOR INJECTION FILE"

#define MAIN_SERVER                 "MainServer"
#define LOCAL_SERVER                "LocalServer"

//////////////////////////
/////// JSON KEYS ////////
//////////////////////////

#define L1_OIL_PUMP_RATE            "L1.OilPumpRate"
#define L2_OIL_PUMP_RATE            "L2.OilPumpRate"
#define L3_OIL_PUMP_RATE            "L3.OilPumpRate"
#define L4_OIL_PUMP_RATE            "L4.OilPumpRate"
#define L5_OIL_PUMP_RATE            "L5.OilPumpRate"
#define L6_OIL_PUMP_RATE            "L6.OilPumpRate"

#define L1_WATER_PUMP_RATE          "L1.WaterPumpRate"
#define L2_WATER_PUMP_RATE          "L2.WaterPumpRate"
#define L3_WATER_PUMP_RATE          "L3.WaterPumpRate"
#define L4_WATER_PUMP_RATE          "L4.WaterPumpRate"
#define L5_WATER_PUMP_RATE          "L5.WaterPumpRate"
#define L6_WATER_PUMP_RATE          "L6.WaterPumpRate"

#define L1_SMALL_WATER_PUMP_RATE    "L1.SmallWaterPumpRate"
#define L2_SMALL_WATER_PUMP_RATE    "L2.SmallWaterPumpRate"
#define L3_SMALL_WATER_PUMP_RATE    "L3.SmallWaterPumpRate"
#define L4_SMALL_WATER_PUMP_RATE    "L4.SmallWaterPumpRate"
#define L5_SMALL_WATER_PUMP_RATE    "L5.SmallWaterPumpRate"
#define L6_SMALL_WATER_PUMP_RATE    "L6.SmallWaterPumpRate"

#define L1_BUCKET                   "L1.Bucket"
#define L2_BUCKET                   "L2.Bucket"
#define L3_BUCKET                   "L3.Bucket"
#define L4_BUCKET                   "L4.Bucket"
#define L5_BUCKET                   "L5.Bucket"
#define L6_BUCKET                   "L6.Bucket"

#define L1_MARK                     "L1.Mark"
#define L2_MARK                     "L2.Mark"
#define L3_MARK                     "L3.Mark"
#define L4_MARK                     "L4.Mark"
#define L5_MARK                     "L5.Mark"
#define L6_MARK                     "L6.Mark"

#define L1_METHOD                   "L1.Method"
#define L2_METHOD                   "L2.Method"
#define L3_METHOD                   "L3.Method"
#define L4_METHOD                   "L4.Method"
#define L5_METHOD                   "L5.Method"
#define L6_METHOD                   "L6.Method"

#define L1_PRESSURE                 "L1.PresssureSensorSlope"
#define L2_PRESSURE                 "L2.PresssureSensorSlope"
#define L3_PRESSURE                 "L3.PresssureSensorSlope"
#define L4_PRESSURE                 "L4.PresssureSensorSlope"
#define L5_PRESSURE                 "L5.PresssureSensorSlope"
#define L6_PRESSURE                 "L6.PresssureSensorSlope"

#define L1_MIN_TEMP                 "L1.MinRefTemp"
#define L2_MIN_TEMP                 "L2.MinRefTemp"
#define L3_MIN_TEMP                 "L3.MinRefTemp"
#define L4_MIN_TEMP                 "L4.MinRefTemp"
#define L5_MIN_TEMP                 "L5.MinRefTemp"
#define L6_MIN_TEMP                 "L6.MinRefTemp"

#define L1_MAX_TEMP                 "L1.MaxRefTemp"
#define L2_MAX_TEMP                 "L2.MaxRefTemp"
#define L3_MAX_TEMP                 "L3.MaxRefTemp"
#define L4_MAX_TEMP                 "L4.MaxRefTemp"
#define L5_MAX_TEMP                 "L5.MaxRefTemp"
#define L6_MAX_TEMP                 "L6.MaxRefTemp"

#define L1_INJECTION_TEMP           "L1.InjectionTemp"
#define L2_INJECTION_TEMP           "L2.InjectionTemp"
#define L3_INJECTION_TEMP           "L3.InjectionTemp"
#define L4_INJECTION_TEMP           "L4.InjectionTemp"
#define L5_INJECTION_TEMP           "L5.InjectionTemp"
#define L6_INJECTION_TEMP           "L6.InjectionTemp"

#define L1_X_DELAY                 	"L1.XDelay"
#define L2_X_DELAY                	"L2.XDelay"
#define L3_X_DELAY                 	"L3.XDelay"
#define L4_X_DELAY                 	"L4.XDelay"
#define L5_X_DELAY                 	"L5.XDelay"
#define L6_X_DELAY                 	"L6.XDelay"

#define L1_Y_FREQ                 	"L1.YFreq"
#define L2_Y_FREQ                 	"L2.YFreq"
#define L3_Y_FREQ                 	"L3.YFreq"
#define L4_Y_FREQ                	"L4.YFreq"
#define L5_Y_FREQ                 	"L5.YFreq"
#define L6_Y_FREQ                 	"L6.YFreq"

#define L1_Z_TEMP                 	"L1.ZTemp"
#define L2_Z_TEMP                 	"L2.ZTemp"
#define L3_Z_TEMP                 	"L3.ZTemp"
#define L4_Z_TEMP                 	"L4.ZTemp"
#define L5_Z_TEMP                 	"L5.ZTemp"
#define L6_Z_TEMP                 	"L6.ZTemp"

/// calib file names
#define FILE_LIST                   "Filelist.LST"
#define CALIBRAT_LCI                "CALIBRAT.LCI"
#define ADJUSTED_LCI                "ADJUSTED.LCI"
#define ROLLOVER_LCR                "ROLLOVER.LCR"

#define TIMER_DELAY         6000
#define NO_FILE             0
#define S_CALIBRAT         	1 
#define S_ADJUSTED         	2 
#define S_ROLLOVER          3 
#define S_FILELIST          4 

#define SLEEP_TIME          10
#define SLAVE_CALIBRATION   0xFA
#define FUNC_READ_FLOAT     0x04
#define FUNC_READ_INT       0x04 
#define FUNC_READ_COIL      0x01 
#define FUNC_WRITE_FLOAT    0x10
#define FUNC_WRITE_INT      0x06
#define FUNC_WRITE_COIL     0x05
#define BYTE_READ_FLOAT     2
#define BYTE_READ_INT       1
#define BYTE_READ_COIL      1
#define FLOAT_R             0
#define FLOAT_W             1
#define INT_R               2
#define INT_W               3
#define COIL_R              4
#define COIL_W              5
#define ADDR_OFFSET         1

#define RAZ_SN              201 
#define RAZ_WATERCUT        3
#define RAZ_TEMPERATURE     5
#define RAZ_SALINITY        9
#define RAZ_FREQUENCY       19
#define RAZ_OIL_INDEX       37
#define RAZ_OIL_RP          61
#define RAZ_OIL_DENSITY     155
#define RAZ_MEAS_AI         173
#define RAZ_TRIM_AI         175

#define EEA_WATERCUT        40006
#define EEA_TEMPERATURE     40051 //REG_TEMPERATURE_EXTERNAL
#define EEA_SALINITY        40011
#define EEA_FREQUENCY       400
#define EEA_OIL_INDEX       37
#define EEA_OIL_RP          61
#define EEA_OIL_DENSITY     155


QT_CHARTS_USE_NAMESPACE

class AboutDialog : public QDialog, public Ui::AboutDialog
{
public:
    AboutDialog( QWidget * _parent ) :
        QDialog( _parent )
    {
        setupUi( this );
        aboutTextLabel->setText(aboutTextLabel->text().arg( "0.3.0" ) );
    }
};

typedef struct PIPE_OBJECT 
{
    int osc;
    int tempStability;
    int freqStability;
    QString calFile;
	bool isCal;
    QString mainDirPath;
    QString localDirPath;
    QString pipeId;
    QFile file;
    QFile fileCalibrate;
    QFile fileAdjusted;
    QFile fileRollover;
    QLineEdit * slave; 
    QSplineSeries * series;
    QElapsedTimer * etimer;
    QCheckBox * checkBox;
    QCheckBox * lineView; 
    QLCDNumber * lcdWatercut;
    QLCDNumber * lcdStartFreq;
    QLCDNumber * lcdFreq;
    QLCDNumber * lcdTemp;
    QLCDNumber * lcdReflectedPower;

	double temperature;
    double temperature_prev;
    double frequency;
    double frequency_prev;
    double watercut;
    double oilrp;
    double measai;
    double trimai;

	PIPE_OBJECT() : osc(0), tempStability(0), freqStability(0), calFile(""), isCal(false), mainDirPath(""), localDirPath(""), pipeId(""), file(""), fileCalibrate("CALIBRATE"), fileAdjusted("ADJUSTED"), fileRollover("ROLLOVER"), slave(new QLineEdit), series(new QSplineSeries), etimer(new QElapsedTimer), lineView(new QCheckBox), checkBox(new QCheckBox), lcdWatercut(new QLCDNumber), lcdStartFreq(new QLCDNumber), lcdFreq(new QLCDNumber), lcdTemp(new QLCDNumber), lcdReflectedPower(new QLCDNumber), temperature(0), frequency(0), temperature_prev(0), frequency_prev(0), watercut(0), oilrp(0), measai(0), trimai(0) {}

    //This is the destructor.  Will delete the array of vertices, if present.
    ~PIPE_OBJECT()
    {
        if (slave) delete slave;
        if (series) delete series;
        if (etimer) delete etimer;
        if (checkBox) delete checkBox;
        if (lineView) delete lineView; 
        if (lcdWatercut) delete lcdWatercut;
        if (lcdStartFreq) delete lcdStartFreq;
        if (lcdFreq) delete lcdFreq;
        if (lcdTemp) delete lcdTemp;
        if (lcdReflectedPower) delete lcdReflectedPower;
    }

} PIPES;


typedef struct LOOP_OBJECT 
{
    bool isCal;
	bool isEEA;
	QString mode;
	double injectionOilPumpRate;
    double injectionWaterPumpRate;
    double injectionSmallWaterPumpRate;
    double injectionBucket;
    double injectionMark;
    double injectionMethod;
    double pressureSensorSlope;
    int minRefTemp;
    int maxRefTemp;
    int injectionTemp;
    int xDelay;
    double yFreq;
    double zTemp;
	QString filExt;
	QString calExt;
	QString adjExt;
	QString rolExt;
    
	/// register address for calibration
    int ID_SN_PIPE;
    int ID_WATERCUT;
    int ID_TEMPERATURE;
    int ID_SALINITY;
    int ID_OIL_ADJUST;
    int ID_WATER_ADJUST;
    int ID_FREQ;
    int ID_OIL_RP;

    QLineEdit * loopVolume;
	QComboBox * saltStart;
	QComboBox * saltStop;
    QComboBox * oilTemp;
	QLineEdit * waterRunStart;
	QLineEdit * waterRunStop;
	QLineEdit * oilRunStart;
	QLineEdit * oilRunStop;
	QLCDNumber * masterWatercut;
	QLCDNumber * masterSalinity;
	QLCDNumber * masterOilAdj;

	modbus_t * modbus;
    modbus_t * serialModbus;
    QChart * chart;
    QChartView * chartView;
    QValueAxis * axisX;
    QValueAxis * axisY;
    QValueAxis * axisY3;

	LOOP_OBJECT() : isCal(false), isEEA(0), mode(""), injectionOilPumpRate(0), injectionWaterPumpRate(0), injectionSmallWaterPumpRate(0), injectionBucket(0), injectionMark(0), injectionMethod(0), pressureSensorSlope(0), minRefTemp(0), maxRefTemp(0), injectionTemp(0), xDelay(0), yFreq(0), zTemp(0), filExt(""), calExt(""), adjExt(""), rolExt(""), ID_SN_PIPE(0), ID_WATERCUT(0), ID_TEMPERATURE(0), ID_SALINITY(0), ID_OIL_ADJUST(0), ID_WATER_ADJUST(0), ID_FREQ(0), ID_OIL_RP(0), loopVolume(new QLineEdit), saltStart(new QComboBox), saltStop(new QComboBox), oilTemp(new QComboBox), waterRunStart(new QLineEdit), waterRunStop(new QLineEdit), oilRunStart(new QLineEdit), oilRunStop(new QLineEdit), masterWatercut(new QLCDNumber), masterSalinity(new QLCDNumber), masterOilAdj(new QLCDNumber), modbus(NULL), serialModbus(NULL), chart(new QChart), chartView(new QChartView), axisX(new QValueAxis), axisY(new QValueAxis), axisY3(new QValueAxis) {};

	~LOOP_OBJECT()
	{
		if (chart) delete chart;
		if (chartView) delete chartView;
		if (axisX) delete axisX;
		if (axisY) delete axisY;
		if (axisY3) delete axisY3;
        if (loopVolume) delete loopVolume;
        if (saltStart) delete saltStart;
        if (saltStop) delete saltStop;
        if (oilTemp) delete oilTemp;
        if (waterRunStart) delete waterRunStart;
        if (waterRunStop) delete waterRunStop;
        if (oilRunStart) delete oilRunStart;
        if (oilRunStop) delete oilRunStop;
        if (masterWatercut) delete masterWatercut;
        if (masterSalinity) delete masterSalinity;
        if (masterOilAdj) delete masterOilAdj;
	}
    
} LOOPS;

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget * parent = 0 );
    ~MainWindow();

    void delay(int);
    modbus_t*  modbus() { return LOOP[0].serialModbus; }
    modbus_t*  modbus_2() { return LOOP[1].serialModbus; }
    modbus_t*  modbus_3() { return LOOP[2].serialModbus; }
    modbus_t*  modbus_4() { return LOOP[3].serialModbus; }
    modbus_t*  modbus_5() { return LOOP[4].serialModbus; }
    modbus_t*  modbus_6() { return LOOP[5].serialModbus; }

    int setupModbusPort();
    int setupModbusPort_2();
    int setupModbusPort_3();
    int setupModbusPort_4();
    int setupModbusPort_5();
    int setupModbusPort_6();

	void readData(const int, const bool);
    void updateLoopStatus(const int, const double, const double, const double);
	void injectWater(const bool);
	void setProductAndCalibrationMode(const int);
    void masterPipe(int, QString, bool);
    void prepareForNextFile(const int, const QString);
    void writeToCalFile(int, QString);
    void closeCalibrationFile(int, int, double);
    void changeModbusInterface(const QString &port, char parity, const int);
    void releaseSerialModbus(const int);
	void setValidators();
    void initializeGraph();
    void initializePipeObjects();
    void initializeLoopObjects();
    void setInputValidator(void);
    bool validateSerialNumber(const int, modbus_t *);   
    void updatePipeReading(const int, const double, const double, const double, const double, const double); 
    bool getUserInputMessage(const QString, const QString, const QString);
    void displayMessage(const QString, const QString, const QString);
    void busMonitorAddItem( bool isRequest,uint16_t slave,uint8_t func,uint16_t addr,uint16_t nb,uint16_t expectedCRC,uint16_t actualCRC );
    static void stBusMonitorAddItem( modbus_t * modbus,uint8_t isOut, uint16_t slave, uint8_t func, uint16_t addr,uint16_t nb, uint16_t expectedCRC, uint16_t actualCRC );
    static void stBusMonitorRawData( modbus_t * modbus, uint8_t * data,uint8_t dataLen, uint8_t addNewline );
    void busMonitorRawData( uint8_t * data, uint8_t dataLen, bool addNewline );
    void connectSerialPort();
    void connectModeChanged();
    void connectActions();
    void connectModbusMonitor();
    void connectTimers();
    void connectRegisters();
    void connectLoopDependentData();
    void connectCalibrationControls();
    void connectProfiler();
    void connectToolbar();
    void connectRadioButtons();
    void connectLineView();
    void connectReturnPressed();
    void connectProductBtnPressed();
    void setupModbusPorts();
    void updateLoopTabIcon(const int, const bool);
    void updateStartButtonLabel(const int);
    bool prepareCalibration(const int);
    void initializeTabIcons();
    float toFloat(QByteArray arr);
    void initializeModbusMonitor();
    void onFunctionCodeChanges();
    double sendCalibrationRequest(int, modbus_t *, int, int, int, int, uint8_t *, uint16_t *, bool, bool, QString);
    void updateChart(QGridLayout *, QChartView *, QChart *, QSplineSeries *, double, double, double, double, double, double, double, double);
    void updateLineView();

private slots:

    void calibrate(const int);
	void updateFileList(const QString, const int, const QString, const int);
	void createCalibrateFile(const int, const int, const QString, const QString, const QString, const QString);
    void stopCalibration(int);
    void onCalibrationButtonPressed(int);
    void onRtuPortActive(bool,int);
    void changeSerialPort(int);
    void changeSerialPort_2(int);
    void changeSerialPort_3(int);
    void changeSerialPort_4(int);
    void changeSerialPort_5(int);
    void changeSerialPort_6(int);

    void toggleLineView_L1P1(bool); 
    void toggleLineView_L1P2(bool); 
    void toggleLineView_L1P3(bool); 
    void toggleLineView_L2P1(bool); 
    void toggleLineView_L2P2(bool); 
    void toggleLineView_L2P3(bool); 
    void toggleLineView_L3P1(bool); 
    void toggleLineView_L3P2(bool); 
    void toggleLineView_L3P3(bool); 
    void toggleLineView_L4P1(bool); 
    void toggleLineView_L4P2(bool); 
    void toggleLineView_L4P3(bool); 
    void toggleLineView_L5P1(bool); 
    void toggleLineView_L5P2(bool); 
    void toggleLineView_L5P3(bool); 
    void toggleLineView_L6P1(bool); 
    void toggleLineView_L6P2(bool); 
    void toggleLineView_L6P3(bool); 

    void onProductSelected(bool);
    void onModeChanged(bool);

    /// config menu
    void injectionPumpRates(int);
    void injectionBucket(int);
    void injectionMark(int);
    void injectionMethod(int);
    void onActionPressureSensorSlope(int);
	void onMinRefTemp(int);
	void onMaxRefTemp(int);
	void onInjectionTemp(int);
    void onXDelay(int);
    void onYFreq(int);
    void onZTemp(int);
    void onActionMainServer();
    void onActionLocalServer();

    void createLoopFile(const int, const QString, const QString, const QString, const QString, const int);
    void initializeToolbarIcons(void);
    void onLoopTabChanged(int);
    void clearMonitors( void );
    void updateRequestPreview( void );
    void updateRegisterView( void );
    void enableHexView( void );
    void sendModbusRequest( void );
    void onSendButtonPress( void );
    void pollForDataOnBus( void );
    void openBatchProcessor();
    void aboutQModBus( void );
    void onCheckBoxChecked(bool);
    void resetStatus( void );
    void setStatusError(const QString &msg);    
    void onFloatButtonPressed(bool);
    void onIntegerButtonPressed(bool);
    void onCoilButtonPressed(bool);
    void onReadButtonPressed(bool);
    void onWriteButtonPressed(bool);
    void onEquationButtonPressed();
    void loadCsvFile();
    void loadCsvTemplate();
    void onUploadEquation();
    void onDownloadEquation();
    void updateRegisters(const bool, const int);
    void onDownloadButtonChecked(bool);
    void saveCsvFile();
    void onEquationTableChecked(bool);
    void onUnlockFactoryDefaultBtnPressed();
    void onLockFactoryDefaultBtnPressed();
    void onUpdateFactoryDefaultPressed();
    void updateStability(const bool, const int, const int);
    void readJsonConfigFile();
    void writeJsonConfigFile();

signals:
    void connectionError(const QString &msg);

private:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    Ui::MainWindowClass * ui;

    /// versioning
    QString PROJECT_VERSION = RELEASE_VERSION;
    QString PROJECT         = "Sparky ";
    QString PHASEDYNAMICS   = " - Phase Dynamics";
    QString SPARKY          = PROJECT + PROJECT_VERSION + PHASEDYNAMICS;

    /// calibration file location
    QString m_localServer;
	QString m_mainServer;

	/// connection
    modbus_t * m_modbus_snipping;
    QIntValidator *serialNumberValidator;
    QWidget * m_statusInd;
    QLabel * m_statusText;
    QTimer * m_pollTimer;
    QTimer * m_statusTimer;
    bool m_tcpActive;
    bool m_poll;
	bool isModbusTransmissionFailed;

	/// QFuture Calibration Thread
	QFuture<void> future_L1P1;
	QFuture<void> future_L1P2;
	QFuture<void> future_L1P3;

	QFuture<void> future_L2P1;
	QFuture<void> future_L2P2;
	QFuture<void> future_L2P3;

	QFuture<void> future_L3P1;
	QFuture<void> future_L3P2;
	QFuture<void> future_L3P3;

	QFuture<void> future_L4P1;
	QFuture<void> future_L4P2;
	QFuture<void> future_L4P3;

	QFuture<void> future_L5P1;
	QFuture<void> future_L5P2;
	QFuture<void> future_L5P3;

	QFuture<void> future_L6P1;
	QFuture<void> future_L6P2;
	QFuture<void> future_L6P3;

	/// loop objects
	LOOPS LOOP[MAX_LOOP];

	/// pipe objects
	PIPES PIPE [MAX_PIPE];
};

#endif // MAINWINDOW_H
