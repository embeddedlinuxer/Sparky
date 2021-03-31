#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGroupBox>
#include <QProgressBar>
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

#define RELEASE_VERSION             "0.0.8"
#define RAZ                         0 
#define EEA                         1 

#define STABILITY_CHECK				true
#define NO_STABILITY_CHECK			false

/// injection material
#define WATER						0
#define OIL							1

/// sub system	
#define CONTROLBOX                  100 
#define MASTERPIPE					101
#define COIL_OIL_PUMP				60
#define COIL_WATER_PUMP				61

/// pipe cal status
#define DONE						0
#define ENABLED						1	
#define DISABLED					2		

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

/// pipe
#define P1                          0
#define P2                          1
#define P3                          2
#define ALL                         3

#define EEA_INJECTION_FILE          "EEA INJECTION FILE"
#define RAZ_INJECTION_FILE          "RAZOR INJECTION FILE"

#define MAIN_SERVER                 "MainServer"
#define LOCAL_SERVER                "LocalServer"

//////////////////////////
/////// JSON KEYS ////////
//////////////////////////

#define LOOP_OIL_PUMP_RATE            "LOOP.OilPumpRate"
#define LOOP_WATER_PUMP_RATE          "LOOP.WaterPumpRate"
#define LOOP_SMALL_WATER_PUMP_RATE    "LOOP.SmallWaterPumpRate"
#define LOOP_BUCKET                   "LOOP.Bucket"
#define LOOP_MARK                     "LOOP.Mark"
#define LOOP_METHOD                   "LOOP.Method"
#define LOOP_PRESSURE                 "LOOP.PresssureSensorSlope"
#define LOOP_MIN_TEMP                 "LOOP.MinRefTemp"
#define LOOP_MAX_TEMP                 "LOOP.MaxRefTemp"
#define LOOP_INJECTION_TEMP           "LOOP.InjectionTemp"
#define LOOP_X_DELAY                  "LOOP.XDelay"
#define LOOP_Y_FREQ                   "LOOP.YFreq"
#define LOOP_Z_TEMP                   "LOOP.ZTemp"
#define LOOP_INTERVAL_CALIBRATION     "LOOP.IntervalCalibration"
#define LOOP_INTERVAL_ROLLOVER  	  "LOOP.IntervalRollover"
#define LOOP_NUMBER  	  			  "LOOP.LoopNumber"
#define LOOP_MASTER_MIN  			  "LOOP.MasterMin"
#define LOOP_MASTER_MAX  			  "LOOP.MasterMax"
#define LOOP_MASTER_DELTA  			  "LOOP.MasterDelta"
#define LOOP_MASTER_DELTA_FINAL		  "LOOP.MasterDeltaFinal"
#define LOOP_MAX_INJECTION_WATER   	  "LOOP.MaxInjectionWater"
#define LOOP_MAX_INJECTION_OIL   	  "LOOP.MaxInjectionOil"

#define FILE_LIST                   "Filelist.LST"

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
	int status;
	int rolloverTracker;
	QString calFile;
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
	QProgressBar * freqProgress;
	QProgressBar * tempProgress;

	double temperature;
    double temperature_prev;
    double frequency;
    double frequency_prev;
    double frequency_start;
    double oilrp;
    double measai;
    double trimai;

	PIPE_OBJECT() : osc(0), tempStability(0), freqStability(0), status(ENABLED), rolloverTracker(0), calFile(""),  mainDirPath(""), localDirPath(""), pipeId(""), file(""), fileCalibrate("CALIBRATE"), fileAdjusted("ADJUSTED"), fileRollover("ROLLOVER"), slave(new QLineEdit), series(new QSplineSeries), etimer(new QElapsedTimer), lineView(new QCheckBox), checkBox(new QCheckBox), lcdWatercut(new QLCDNumber), lcdStartFreq(new QLCDNumber), lcdFreq(new QLCDNumber), lcdTemp(new QLCDNumber), lcdReflectedPower(new QLCDNumber), freqProgress(new QProgressBar), tempProgress(new QProgressBar),temperature(0), frequency(0), temperature_prev(0), frequency_prev(0), frequency_start(0), oilrp(0), measai(0), trimai(0) {}

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
	bool isMaster;
    bool isCal;
	bool isEEA;
	bool isInitialAMB;
	bool isInitialMinRef;
	bool isInitialMaxRef;
	bool isInitialCal;
	QString mode;
	double masterMin;
	double masterMax;
	double masterDelta;
	double masterDeltaFinal;
    double watercut;
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
	int loopNumber;
	int maxInjectionWater;
	int maxInjectionOil;
    double yFreq;
    double zTemp;
	double intervalCalibration;
	double intervalRollover;
	QString filExt;
	QString calExt;
	QString adjExt;
	QString rolExt;
	QString operatorName;
    
	/// register address for calibration
    int ID_SN_PIPE;
    int ID_WATERCUT;
    int ID_TEMPERATURE;
    int ID_SALINITY;
    int ID_OIL_ADJUST;
    int ID_WATER_ADJUST;
    int ID_FREQ;
    int ID_OIL_RP;
    int ID_MASTER_WATERCUT;
    int ID_MASTER_SALINITY;
    int ID_MASTER_OIL_ADJUST;
	
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

	LOOP_OBJECT() : isMaster(false), isCal(false), isEEA(0), isInitialAMB(1), isInitialMinRef(1), isInitialMaxRef(1), isInitialCal(1), mode(""), masterMin(0), masterMax(0),masterDelta(0), masterDeltaFinal(0), watercut(0), injectionOilPumpRate(0), injectionWaterPumpRate(0), injectionSmallWaterPumpRate(0), injectionBucket(0), injectionMark(0), injectionMethod(0), pressureSensorSlope(0), minRefTemp(0), maxRefTemp(0), injectionTemp(0), xDelay(0), loopNumber(0), maxInjectionWater(80), maxInjectionOil(200), yFreq(0), zTemp(0), intervalCalibration(0), intervalRollover(0), filExt(""), calExt(""), adjExt(""), rolExt(""), operatorName(""), ID_SN_PIPE(0), ID_WATERCUT(0), ID_TEMPERATURE(0), ID_SALINITY(0), ID_OIL_ADJUST(0), ID_WATER_ADJUST(0), ID_FREQ(0), ID_OIL_RP(0), ID_MASTER_WATERCUT(0), ID_MASTER_SALINITY(0), ID_MASTER_OIL_ADJUST(0), loopVolume(new QLineEdit), saltStart(new QComboBox), saltStop(new QComboBox), oilTemp(new QComboBox), waterRunStart(new QLineEdit), waterRunStop(new QLineEdit), oilRunStart(new QLineEdit), oilRunStop(new QLineEdit), masterWatercut(new QLCDNumber), masterSalinity(new QLCDNumber), masterOilAdj(new QLCDNumber), modbus(NULL), serialModbus(NULL), chart(new QChart), chartView(new QChartView), axisX(new QValueAxis), axisY(new QValueAxis), axisY3(new QValueAxis) {};

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
    modbus_t*  modbus() { return LOOP.serialModbus; }
    modbus_t*  modbus_2() { return LOOP.serialModbus; }
    modbus_t*  modbus_3() { return LOOP.serialModbus; }
    modbus_t*  modbus_4() { return LOOP.serialModbus; }
    modbus_t*  modbus_5() { return LOOP.serialModbus; }
    modbus_t*  modbus_6() { return LOOP.serialModbus; }

    int setupModbusPort();
    int setupModbusPort_2();
    int setupModbusPort_3();
    int setupModbusPort_4();
    int setupModbusPort_5();
    int setupModbusPort_6();

	void readMasterPipe();
	void updateCalibrationLcd(const double, const double, const double, const double);
	void readData(const int, const bool);
    void updateLoopStatus(const int, const double, const double, const double);
	void inject(const int, const bool);
	void setProductAndCalibrationMode();
    void masterPipe(int, QString, bool);
    void prepareForNextFile(const int, const QString);
    void writeToCalFile(int, QString);
    void closeCalibrationFile(int, int, double);
    void changeModbusInterface(const QString &port, char parity);
    void releaseSerialModbus();
	void setValidators();
    void initializeGraph();
    void initializePipeObjects();
    void initializeLoopObjects();
    void setInputValidator(void);
    bool validateSerialNumber(modbus_t *);   
    void updatePipeStatus(const int, const double, const double, const double, const double, const double); 
    bool getUserInputMessage(const QString, const QString, const QString);
    bool displayMessage(const QString, const QString, const QString);
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
    void connectCalibrationControls();
    void connectProfiler();
    void connectToolbar();
    void connectRadioButtons();
    void connectLineView();
    void connectReturnPressed();
    void connectProductBtnPressed();
	void connectMasterPipe();
    void setupModbusPorts();
    void updateLoopTabIcon(const bool);
    void updateStartButtonLabel();
    bool prepareCalibration();
    void initializeTabIcons();
    float toFloat(QByteArray arr);
    void initializeModbusMonitor();
    void onFunctionCodeChanges();
    double sendCalibrationRequest(int, modbus_t *, int, int, int, int, uint8_t *, uint16_t *, bool, bool, QString);
    void updateChart(QGridLayout *, QChartView *, QChart *, QSplineSeries *, double, double, double, double, double, double, double, double);
    void updateLineView();

private slots:

	void onMasterPipeToggled(const bool);
    void calibrate();
    void stopCalibration();
	void updateFileList(const QString, const int, const int);
	void createCalibrateFile(const int, const int, const QString, const QString, const QString, const QString);
    void onCalibrationButtonPressed();
    void onRtuPortActive(bool);
    void changeSerialPort(int);

    void toggleLineView_P1(bool); 
    void toggleLineView_P2(bool); 
    void toggleLineView_P3(bool); 

    void onModeChanged(bool);

    /// config menu
    void injectionPumpRates();
    void injectionBucket();
    void injectionMark();
    void injectionMethod();
    void onActionPressureSensorSlope();
	void onMinRefTemp();
	void onMaxRefTemp();
	void onInjectionTemp();
    void onXDelay();
    void onYFreq();
    void onZTemp();
    void onActionMainServer();
    void onActionLocalServer();
    void onIntervalCalibration();
    void onIntervalRollover();
    void onLoopNumber();
	void onActionMinMaster();
	void onActionMaxMaster();
	void onActionDeltaMaster();
	void onActionDeltaMasterFinal();
	void onActionWater();
	void onActionOil();

    void createLoopFile(const int, const QString, const QString, const QString, const int);
    void initializeToolbarIcons(void);
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
    void onUpdateRegisters(const bool);
    void onDownloadButtonChecked(bool);
    void saveCsvFile();
    void onEquationTableChecked(bool);
    void onUnlockFactoryDefaultBtnPressed();
    void onLockFactoryDefaultBtnPressed();
    void onUpdateFactoryDefaultPressed();
    void updatePipeStability(const bool, const int, const int);
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
	QFuture<void> future_P1;
	QFuture<void> future_P2;
	QFuture<void> future_P3;

	/// loop objects
	LOOPS LOOP;

	/// pipe objects
	PIPES PIPE [3];
};

#endif // MAINWINDOW_H
