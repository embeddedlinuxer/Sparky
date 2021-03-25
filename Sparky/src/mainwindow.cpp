#include <QtConcurrent>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QScrollBar>
#include <QTime>
#include <QFileDialog>
#include <QThread>
#include <errno.h>
#include <QSignalMapper>
#include <QListWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QInputDialog>
#include <QProgressDialog>
#include "mainwindow.h"
#include "BatchProcessor.h"
#include "modbus.h"
#include "modbus-private.h"
#include "modbus-rtu.h"
#include "ui_mainwindow.h"
#include "qextserialenumerator.h"

QT_CHARTS_USE_NAMESPACE

const int DataTypeColumn = 0;
const int AddrColumn = 1;
const int DataColumn = 2;

extern MainWindow * globalMainWin;

MainWindow::MainWindow( QWidget * _parent ) :
	QMainWindow( _parent ),
	ui( new Ui::MainWindowClass ),
    m_modbus_snipping( NULL ),
	m_poll(false),
	isModbusTransmissionFailed(false)
{
	ui->setupUi(this);

    /// versioning
    setWindowTitle(SPARKY);

    readJsonConfigFile();
    updateRegisters(EEA,0); // 0-EEA
    initializeToolbarIcons();
    initializeTabIcons();
    updateRegisterView();
    updateRequestPreview();
    enableHexView();
    setupModbusPorts();
    onLoopTabChanged(0);
    initializePipeObjects();
    initializeLoopObjects();
    initializeGraph();
    initializeModbusMonitor();
	setValidators();

    ui->regTable->setColumnWidth( 0, 150 );
    m_statusInd = new QWidget;
    m_statusInd->setFixedSize( 16, 16 );
    m_statusText = new QLabel;
    ui->statusBar->addWidget( m_statusInd );
    ui->statusBar->addWidget( m_statusText, 10 );
    resetStatus();

    serialNumberValidator = new QIntValidator(0, 999999, this);

    /// connections 
    connectModeChanged();
    connectProductBtnPressed();
    connectLoopDependentData();
    connectRadioButtons();
    connectSerialPort();
    connectActions();
    connectModbusMonitor();
    connectTimers();
    connectCalibrationControls();
    connectProfiler();
    connectToolbar();
    connectLineView();

    /// clear connection at start
    updateLoopTabIcon(ALL, false);

    /// reset stability progressbar
    updateStability(T_BAR,ALL,0);
}


MainWindow::~MainWindow()
{
	delete ui;
    delete m_statusInd;
    delete m_statusText;
    delete serialNumberValidator;
}


void
MainWindow::
setValidators()
{
	ui->lineEdit->setValidator( new QDoubleValidator(0, 1000000, 2, this) ); // loopVolume
	ui->lineEdit_37->setValidator( new QDoubleValidator(0, 1000000, 2, this) ); // waterRunStart
	ui->lineEdit_38->setValidator( new QDoubleValidator(0, 1000000, 2, this) ); // waterRunStop
	ui->lineEdit_39->setValidator( new QDoubleValidator(0, 1000000, 2, this) ); // oilRunStart
	ui->lineEdit_40->setValidator( new QDoubleValidator(0, 1000000, 2, this) ); // oilRunStop
}


void
MainWindow::
delay(int sec = 2)
{
    QTime dieTime= QTime::currentTime().addSecs(sec);
    while (QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void
MainWindow::
initializeModbusMonitor()
{
    ui->groupBox_103->setEnabled(TRUE);
    ui->groupBox_106->setEnabled(FALSE);
    ui->groupBox_107->setEnabled(FALSE);
    ui->functionCode->setCurrentIndex(3);
}


void
MainWindow::
initializeToolbarIcons() {

    ui->toolBar->addAction(ui->actionOpen);
    ui->toolBar->addAction(ui->actionSave);
    ui->toolBar->addSeparator();
    ui->actionDisconnect->setDisabled(TRUE);
    ui->actionConnect->setEnabled(TRUE);
}

void
MainWindow::
keyPressEvent(QKeyEvent* event)
{
	if( event->key() == Qt::Key_Control )
	{
		//set flag to request polling
        if( LOOP[0].modbus != NULL )	m_poll = true;
        if( ! m_pollTimer->isActive() )	ui->sendBtn->setText( tr("Poll") );
	}
}

void
MainWindow::
keyReleaseEvent(QKeyEvent* event)
{
	if( event->key() == Qt::Key_Control )
	{
		m_poll = false;
        if( ! m_pollTimer->isActive() )	ui->sendBtn->setText( tr("Send") );
	}
}

void MainWindow::onSendButtonPress( void )
{
	// if already polling then stop
	if( m_pollTimer->isActive() )
	{
		m_pollTimer->stop();
		ui->sendBtn->setText( tr("Send") );
	}
	else
	{
		// if polling requested then enable timer
		if( m_poll )
		{
			m_pollTimer->start( 1000 );
			ui->sendBtn->setText( tr("Stop") );
		}

		sendModbusRequest();
	}
}

void MainWindow::busMonitorAddItem( bool isRequest,
                    uint16_t slave,
					uint8_t func,
					uint16_t addr,
					uint16_t nb,
					uint16_t expectedCRC,
					uint16_t actualCRC )
{
	QTableWidget * bm = ui->busMonTable;
	const int rowCount = bm->rowCount();
	bm->setRowCount( rowCount+1 );

    QTableWidgetItem * numItem;
	QTableWidgetItem * ioItem = new QTableWidgetItem( isRequest ? tr( "Req >>" ) : tr( "<< Resp" ) );
	QTableWidgetItem * slaveItem = new QTableWidgetItem( QString::number( slave ) );
	QTableWidgetItem * funcItem = new QTableWidgetItem( QString::number( func ) );
	QTableWidgetItem * addrItem = new QTableWidgetItem( QString::number( addr ) );
    (ui->radioButton_181->isChecked()) ?numItem  = new QTableWidgetItem( QString::number( 2 ) ) : numItem = new QTableWidgetItem( QString::number( 1 ) );
	QTableWidgetItem * crcItem = new QTableWidgetItem;

	if( func > 127 )
	{
		addrItem->setText( QString() );
		numItem->setText( QString() );
		funcItem->setText( tr( "Exception (%1)" ).arg( func-128 ) );
		funcItem->setForeground( Qt::red );
	}
	else
	{
		if( expectedCRC == actualCRC )
		{
			crcItem->setText( QString().sprintf( "%.4x", actualCRC ) );
		}
		else
		{
			crcItem->setText( QString().sprintf( "%.4x (%.4x)", actualCRC, expectedCRC ) );
			crcItem->setForeground( Qt::red );
		}
	}
	ioItem->setFlags( ioItem->flags() & ~Qt::ItemIsEditable );
	slaveItem->setFlags( slaveItem->flags() & ~Qt::ItemIsEditable );
	funcItem->setFlags( funcItem->flags() & ~Qt::ItemIsEditable );
	addrItem->setFlags( addrItem->flags() & ~Qt::ItemIsEditable );
	numItem->setFlags( numItem->flags() & ~Qt::ItemIsEditable );
	crcItem->setFlags( crcItem->flags() & ~Qt::ItemIsEditable );
	bm->setItem( rowCount, 0, ioItem );
	bm->setItem( rowCount, 1, slaveItem );
    bm->setItem( rowCount, 2, funcItem );
	bm->setItem( rowCount, 3, addrItem );
	bm->setItem( rowCount, 4, numItem );
	bm->setItem( rowCount, 5, crcItem );
	bm->verticalScrollBar()->setValue( bm->verticalScrollBar()->maximum() );
}


void MainWindow::busMonitorRawData( uint8_t * data, uint8_t dataLen, bool addNewline )
{
	if( dataLen > 0 )
	{
		QString dump = ui->rawData->toPlainText();
		for( int i = 0; i < dataLen; ++i )
		{
			dump += QString().sprintf( "%.2x ", data[i] );
		}
		if( addNewline )
		{
			dump += "\n";
		}
		ui->rawData->setPlainText( dump );
		ui->rawData->verticalScrollBar()->setValue( 100000 );
		ui->rawData->setLineWrapMode( QPlainTextEdit::NoWrap );
	}
}

// static
void MainWindow::stBusMonitorAddItem( modbus_t * modbus, uint8_t isRequest, uint16_t slave, uint8_t func, uint16_t addr, uint16_t nb, uint16_t expectedCRC, uint16_t actualCRC )
{
    Q_UNUSED(modbus);
    globalMainWin->busMonitorAddItem( isRequest, slave, func, addr+1, nb, expectedCRC, actualCRC );
}

// static
void MainWindow::stBusMonitorRawData( modbus_t * modbus, uint8_t * data, uint8_t dataLen, uint8_t addNewline )
{
    Q_UNUSED(modbus);
    globalMainWin->busMonitorRawData( data, dataLen, addNewline != 0 );
}

static QString descriptiveDataTypeName( int funcCode )
{
	switch( funcCode )
	{
		case MODBUS_FC_READ_COILS:
		case MODBUS_FC_WRITE_SINGLE_COIL:
		case MODBUS_FC_WRITE_MULTIPLE_COILS:
			return "Coil (binary)";
		case MODBUS_FC_READ_DISCRETE_INPUTS:
			return "Discrete Input (binary)";
		case MODBUS_FC_READ_HOLDING_REGISTERS:
		case MODBUS_FC_WRITE_SINGLE_REGISTER:
		case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
			return "Holding Register (16 bit)";
		case MODBUS_FC_READ_INPUT_REGISTERS:
			return "Input Register (16 bit)";
		default:
			break;
	}
	return "Unknown";
}


static inline QString embracedString( const QString & s )
{
	return s.section( '(', 1 ).section( ')', 0, 0 );
}


static inline int stringToHex( QString s )
{
	return s.replace( "0x", "" ).toInt( NULL, 16 );
}


void MainWindow::updateRequestPreview( void )
{
	const int slave = ui->slaveID->value();
    const int func = stringToHex( embracedString(ui->functionCode->currentText() ) );
	const int addr = ui->startAddr->value()-1;
	const int num = ui->numCoils->value();
	if( func == MODBUS_FC_WRITE_SINGLE_COIL || func == MODBUS_FC_WRITE_SINGLE_REGISTER )
	{
		ui->requestPreview->setText(
			QString().sprintf( "%.2x  %.2x  %.2x %.2x ",
					slave,
					func,
					addr >> 8,
					addr & 0xff ) );
	}
	else
	{
		ui->requestPreview->setText(
			QString().sprintf( "%.2x  %.2x  %.2x %.2x  %.2x %.2x",
					slave,
					func,
					addr >> 8,
					addr & 0xff,
					num >> 8,
					num & 0xff ) );
	}
}




void MainWindow::updateRegisterView( void )
{
	const int func = stringToHex( embracedString(ui->functionCode->currentText() ) );
	const QString funcType = descriptiveDataTypeName( func );
	const int addr = ui->startAddr->value();

	int rowCount = 0;
	switch( func )
	{
		case MODBUS_FC_WRITE_SINGLE_REGISTER:
		case MODBUS_FC_WRITE_SINGLE_COIL:
			ui->numCoils->setEnabled( false );
			rowCount = 1;
			break;
		case MODBUS_FC_WRITE_MULTIPLE_COILS:
		case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
			rowCount = ui->numCoils->value();
		default:
			ui->numCoils->setEnabled( true );
			break;
	}

	ui->regTable->setRowCount( rowCount );
	for( int i = 0; i < rowCount; ++i )
	{
		QTableWidgetItem * dtItem = new QTableWidgetItem( funcType );
		QTableWidgetItem * addrItem = new QTableWidgetItem( QString::number( addr+i ) );
		QTableWidgetItem * dataItem = new QTableWidgetItem( QString::number( 0 ) );

		dtItem->setFlags( dtItem->flags() & ~Qt::ItemIsEditable	);
		addrItem->setFlags( addrItem->flags() & ~Qt::ItemIsEditable );
		ui->regTable->setItem( i, DataTypeColumn, dtItem );
		ui->regTable->setItem( i, AddrColumn, addrItem );
		ui->regTable->setItem( i, DataColumn, dataItem );
	}

	ui->regTable->setColumnWidth( 0, 150 );
}


void MainWindow::enableHexView( void )
{
	const int func = stringToHex( embracedString(
					ui->functionCode->currentText() ) );

	bool b_enabled =
		func == MODBUS_FC_READ_HOLDING_REGISTERS ||
		func == MODBUS_FC_READ_INPUT_REGISTERS;

	ui->checkBoxHexData->setEnabled( b_enabled );
}


void MainWindow::sendModbusRequest( void )
{
    // UPDATE m_modbus_snipping WITH THE CURRENT
    if (ui->tabWidget_2->currentIndex() == 0)      m_modbus_snipping = LOOP[0].modbus;

	if( m_modbus_snipping == NULL )
	{
		setStatusError( tr("Not configured!") );
		return;
	}

	const int slave = ui->slaveID->value();
	const int func = stringToHex(embracedString(ui->functionCode->currentText()));
	const int addr = ui->startAddr->value()-1;
	int num = ui->numCoils->value();
	uint8_t dest[1024];
	uint16_t * dest16 = (uint16_t *) dest;

	memset( dest, 0, 1024 );

	int ret = -1;
	bool is16Bit = false;
	bool writeAccess = false;
	const QString funcType = descriptiveDataTypeName( func );

	modbus_set_slave( m_modbus_snipping, slave );

	switch( func )
	{
		case MODBUS_FC_READ_COILS:
			ret = modbus_read_bits( m_modbus_snipping, addr, num, dest );
			break;
		case MODBUS_FC_READ_DISCRETE_INPUTS:
			ret = modbus_read_input_bits( m_modbus_snipping, addr, num, dest );
			break;
		case MODBUS_FC_READ_HOLDING_REGISTERS:
			ret = modbus_read_registers( m_modbus_snipping, addr, num, dest16 );
			is16Bit = true;
			break;
		case MODBUS_FC_READ_INPUT_REGISTERS:
			ret = modbus_read_input_registers(m_modbus_snipping, addr, num, dest16 );
			is16Bit = true;
			break;
		case MODBUS_FC_WRITE_SINGLE_COIL:
            //ret = modbus_write_bit( m_modbus_snipping, addr,ui->regTable->item( 0, DataColumn )->text().toInt(0, 0) ? 1 : 0 );
            ret = modbus_write_bit( m_modbus_snipping, addr,ui->radioButton_184->isChecked() ? 1 : 0 );
			writeAccess = true;
			num = 1;
			break;
		case MODBUS_FC_WRITE_SINGLE_REGISTER:
            //ret = modbus_write_register( m_modbus_snipping, addr,ui->regTable->item( 0, DataColumn )->text().toInt(0, 0) );
            ret = modbus_write_register( m_modbus_snipping, addr,ui->lineEdit_111->text().toInt(0, 0) );
			writeAccess = true;
			num = 1;
			break;
		case MODBUS_FC_WRITE_MULTIPLE_COILS:
		{
			uint8_t * data = new uint8_t[num];
			for( int i = 0; i < num; ++i ) data[i] = ui->regTable->item( i, DataColumn )->text().toInt(0, 0);
			ret = modbus_write_bits( m_modbus_snipping, addr, num, data );
			delete[] data;
			writeAccess = true;
			break;
		}
		case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
		{
            float value;
            QString qvalue = ui->lineEdit_109->text();
            QTextStream floatTextStream(&qvalue);
            floatTextStream >> value;
            quint16 (*reg)[2] = reinterpret_cast<quint16(*)[2]>(&value);
            uint16_t * data = new uint16_t[2];            
            data[0] = (*reg)[1];
            data[1] = (*reg)[0];
            ret = modbus_write_registers( m_modbus_snipping, addr, 2, data );
			delete[] data;
			writeAccess = true;
			break;
		}
		default:
			break;
	}

	if( ret == num  )
	{
		isModbusTransmissionFailed = false;

		if( writeAccess )
		{
			m_statusText->setText(tr( "Values successfully sent" ) );
			m_statusInd->setStyleSheet( "background: #0b0;" );
			m_statusTimer->start( 2000 );
		}
        else
		{
			bool b_hex = is16Bit && ui->checkBoxHexData->checkState() == Qt::Checked;
			QString qs_num;
            QString qs_output = "0x";
            bool ok = false;

			ui->regTable->setRowCount( num );
			for( int i = 0; i < num; ++i )
			{
				int data = is16Bit ? dest16[i] : dest[i];
                QString qs_tmp;

				QTableWidgetItem * dtItem = new QTableWidgetItem( funcType );
				QTableWidgetItem * addrItem = new QTableWidgetItem(QString::number( ui->startAddr->value()+i ) );
				qs_num.sprintf( b_hex ? "0x%04x" : "%d", data);
				qs_tmp.sprintf("%04x", data);
                qs_output.append(qs_tmp);
				QTableWidgetItem * dataItem = new QTableWidgetItem( qs_num );
				dtItem->setFlags( dtItem->flags() & ~Qt::ItemIsEditable );
				addrItem->setFlags( addrItem->flags() & ~Qt::ItemIsEditable );
				dataItem->setFlags( dataItem->flags() & ~Qt::ItemIsEditable );
				ui->regTable->setItem( i, DataTypeColumn, dtItem );
				ui->regTable->setItem( i, AddrColumn, addrItem );
				ui->regTable->setItem( i, DataColumn, dataItem );
                if (ui->radioButton_182->isChecked()) ui->lineEdit_111->setText(QString::number(data));
                else if (ui->radioButton_183->isChecked())
                {
                    (data) ? ui->radioButton_184->setChecked(true) : ui->radioButton_185->setChecked(true);
                }
            }

            QByteArray array = QByteArray::fromHex(qs_output.toLatin1());
            const float d = toFloat(array);

            if (ui->radioButton_181->isChecked())
            {
                (b_hex) ? ui->lineEdit_109->setText(qs_output) : ui->lineEdit_109->setText(QString::number(d,'f',10)) ;
            }
		}
	}
	else
	{
		QString err;

		if( ret < 0 )
		{
			if(
#ifdef WIN32
					errno == WSAETIMEDOUT ||
#endif
					errno == EIO
																	)
			{
				err += tr( "I/O error" );
				err += ": ";
				err += tr( "did not receive any data from slave." );
			}
			else
			{
				err += tr( "Protocol error" );
				err += ": ";
				err += tr( "Slave threw exception '" );
				err += modbus_strerror( errno );
				err += tr( "' or function not implemented." );
			}
		}
		else
		{
			err += tr( "Protocol error" );
			err += ": ";
			err += tr( "Number of registers returned does not "
					"match number of registers requested!" );
		}

		if( err.size() > 0 )
			setStatusError( err );

		isModbusTransmissionFailed = true;
	}
}


void MainWindow::resetStatus( void )
{
	m_statusText->setText( tr( "Ready" ) );
	m_statusInd->setStyleSheet( "background: #aaa;" );
}

void MainWindow::pollForDataOnBus( void )
{
	if( LOOP[0].modbus )
	{
		modbus_poll( LOOP[0].modbus );
	}
}


void MainWindow::openBatchProcessor()
{
	BatchProcessor( this, LOOP[0].modbus ).exec();
}


void MainWindow::aboutQModBus( void )
{
	AboutDialog( this ).exec();
}

void MainWindow::onRtuPortActive(bool active, int loop)
{
	if (active) {
        LOOP[0].modbus = this->modbus();

        //LOOP[loop].modbus = this->modbus();
		if (LOOP[0].modbus) {
			modbus_register_monitor_add_item_fnc(LOOP[0].modbus, MainWindow::stBusMonitorAddItem);
			modbus_register_monitor_raw_data_fnc(LOOP[0].modbus, MainWindow::stBusMonitorRawData);
		}
	}
	else LOOP[0].modbus = NULL;
}


void
MainWindow::
setStatusError(const QString &msg)
{
    m_statusText->setText( msg );
    m_statusInd->setStyleSheet( "background: red;" );
    m_statusTimer->start( 2000 );
}


void
MainWindow::
initializePipeObjects()
{
	/// label
    PIPE[0].pipeId = "P1";
    PIPE[1].pipeId = "P2";
    PIPE[2].pipeId = "P3";

	/// slave
	PIPE[0].slave = ui->lineEdit_2;
	PIPE[1].slave = ui->lineEdit_7;
	PIPE[2].slave = ui->lineEdit_13;

    /// on/off graph line view
    PIPE[0].lineView = ui->checkBox_19;
    PIPE[1].lineView = ui->checkBox_20;
    PIPE[2].lineView = ui->checkBox_21;

    /// on/off pipe switch
    PIPE[0].checkBox = ui->checkBox;
    PIPE[1].checkBox = ui->checkBox_2;
    PIPE[2].checkBox = ui->checkBox_3;

    /// lcdWatercut
    PIPE[0].lcdWatercut = ui->lcdNumber_4;
    PIPE[1].lcdWatercut = ui->lcdNumber_53;
    PIPE[2].lcdWatercut = ui->lcdNumber_58;

    /// lcdStartFreq
    PIPE[0].lcdStartFreq = ui->lcdNumber_47;
    PIPE[1].lcdStartFreq = ui->lcdNumber_51;
    PIPE[2].lcdStartFreq = ui->lcdNumber_56;

    /// lcdFreq 
    PIPE[0].lcdFreq = ui->lcdNumber_48;
    PIPE[1].lcdFreq = ui->lcdNumber_55;
    PIPE[2].lcdFreq = ui->lcdNumber_60;

    /// lcdTemp 
    PIPE[0].lcdTemp = ui->lcdNumber_49;
    PIPE[1].lcdTemp = ui->lcdNumber_54;
    PIPE[2].lcdTemp = ui->lcdNumber_59;

    /// lcd 
    PIPE[0].lcdReflectedPower = ui->lcdNumber_50;
    PIPE[1].lcdReflectedPower = ui->lcdNumber_52;
    PIPE[2].lcdReflectedPower = ui->lcdNumber_57;
}


void
MainWindow::
initializeLoopObjects()
{
	int j = 0;
    {
        /// setup chart
        LOOP[j].chart->setTitle("LOOP "+QString::number(j+1));
        LOOP[j].chart->legend()->hide();
        LOOP[j].chartView->setChart(LOOP[j].chart);

        /// axisX
        LOOP[j].axisX->setRange(0,1000);
        LOOP[j].axisX->setTickCount(11);
        LOOP[j].axisX->setTickInterval(100);
        LOOP[j].axisX->setLabelFormat("%i");
        LOOP[j].axisX->setTitleText("Frequency (Mhz)");

        /// axisY
        LOOP[j].axisY->setRange(0,100);
        LOOP[j].axisY->setTickCount(11);
        LOOP[j].axisY->setLabelFormat("%i");
        LOOP[j].axisY->setTitleText("Watercut (%)");

        /// axisY3
        LOOP[j].axisY3->setRange(0,2.5);
        LOOP[j].axisY3->setTickCount(11);
        LOOP[j].axisY3->setLabelFormat("%.1f");
        LOOP[j].axisY3->setTitleText("Reflected Power (V)");

        /// add axisX
        LOOP[j].chart->addAxis(LOOP[j].axisX, Qt::AlignBottom);

        /// add axisY
        LOOP[j].chart->addAxis(LOOP[j].axisY, Qt::AlignLeft);
    
        /// add axisY3
        LOOP[j].chart->addAxis(LOOP[j].axisY3, Qt::AlignRight);

        /// linePenColor
        LOOP[j].axisY->setLinePenColor(PIPE[3*j + 0].series->pen().color());
        LOOP[j].axisY->setLinePenColor(PIPE[3*j + 1].series->pen().color());
        LOOP[j].axisY->setLinePenColor(PIPE[3*j + 2].series->pen().color());

        /// setLabelColor
        LOOP[j].axisY->setLabelsColor(PIPE[3*j + 0].series->pen().color());
        LOOP[j].axisY->setLabelsColor(PIPE[3*j + 1].series->pen().color());
        LOOP[j].axisY->setLabelsColor(PIPE[3*j + 2].series->pen().color());

        /// render hint 
        LOOP[j].chartView->setRenderHint(QPainter::Antialiasing);
    }

    /// loop volume
    LOOP[0].loopVolume = ui->lineEdit;

    /// saltStart 
    LOOP[0].saltStart = ui->comboBox_31;

    /// saltStop
    LOOP[0].saltStop = ui->comboBox_33;

    /// oilTemp
    LOOP[0].oilTemp = ui->comboBox_32;

    /// waterRunStart 
    LOOP[0].waterRunStart = ui->lineEdit_37;

    /// waterRunStop 
    LOOP[0].waterRunStop = ui->lineEdit_38;

    /// oilRunStart 
    LOOP[0].oilRunStart = ui->lineEdit_39;

    /// oilRunStop 
    LOOP[0].oilRunStop = ui->lineEdit_40;

    /// masterWatercut
    LOOP[0].masterWatercut = ui->lcdNumber_8;

    /// masterSalinity
    LOOP[0].masterSalinity = ui->lcdNumber_9;

    /// masterOilAdj
    LOOP[0].masterOilAdj = ui->lcdNumber_10;
}


void
MainWindow::
initializeGraph()
{
    /// draw chart lines
    updateChart(ui->gridLayout_5,LOOP[0].chartView,LOOP[0].chart,PIPE[0].series,90,5,280,48,300,68,400,89);
    updateChart(ui->gridLayout_5,LOOP[0].chartView,LOOP[0].chart,PIPE[1].series,100,5,300,48,400,68,600,89);
    updateChart(ui->gridLayout_5,LOOP[0].chartView,LOOP[0].chart,PIPE[2].series,110,5,400,48,600,68,800,89);
}


void
MainWindow::
updateLineView()
{
    for (int pipe = 0; pipe < 3; pipe++) (PIPE[pipe].lineView) ? PIPE[pipe].series->show() : PIPE[pipe].series->hide();
}

void
MainWindow::
updateChart(QGridLayout * layout, QChartView * chartView, QChart * chart, QSplineSeries * series, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    *series << QPointF(x1, y1) << QPointF(x2, y2) << QPointF(x3, y3) << QPointF(x4, y4);
    chart->addSeries(series);
    layout->addWidget(chartView,0,0);
    updateLineView();
}

void
MainWindow::
updateLoopStatus(const int loop, const double a, const double b, const double c)
{
}


void
MainWindow::
connectTimers()
{
    QTimer * t = new QTimer( this );
    connect( t, SIGNAL(timeout()), this, SLOT(pollForDataOnBus()));
    t->start( 5 );

    m_pollTimer = new QTimer( this );
    connect( m_pollTimer, SIGNAL(timeout()), this, SLOT(sendModbusRequest()));

    m_statusTimer = new QTimer( this );
    connect( m_statusTimer, SIGNAL(timeout()), this, SLOT(resetStatus()));
    m_statusTimer->setSingleShot(true);
}


void
MainWindow::
connectLoopDependentData()
{
    connect(ui->tabWidget_2, SIGNAL(currentChanged(int)), this, SLOT(onLoopTabChanged(int)));
}

void
MainWindow::
connectRadioButtons()
{
    connect(ui->radioButton_187, SIGNAL(toggled(bool)), this, SLOT(onReadButtonPressed(bool)));
    connect(ui->radioButton_186, SIGNAL(toggled(bool)), this, SLOT(onWriteButtonPressed(bool)));
}


void
MainWindow::
connectSerialPort()
{
    connect( ui->groupBox_18, SIGNAL(toggled(bool)), this, SLOT(onCheckBoxChecked(bool)));
    connect( this, SIGNAL(connectionError(const QString&)), this, SLOT(setStatusError(const QString&)));
}


void
MainWindow::
connectActions()
{
    connect( ui->actionAbout_QModBus, SIGNAL( triggered() ),this, SLOT( aboutQModBus() ) );
    connect( ui->functionCode, SIGNAL( currentIndexChanged( int ) ),this, SLOT( enableHexView() ) );
}


void
MainWindow::
connectModbusMonitor()
{
    connect( ui->slaveID, SIGNAL( valueChanged( int ) ),this, SLOT( updateRequestPreview() ) );
    connect( ui->functionCode, SIGNAL( currentIndexChanged( int ) ),this, SLOT( updateRequestPreview() ) );
    connect( ui->startAddr, SIGNAL( valueChanged( int ) ),this, SLOT( updateRequestPreview() ) );
    connect( ui->numCoils, SIGNAL( valueChanged( int ) ),this, SLOT( updateRequestPreview() ) );
    connect( ui->functionCode, SIGNAL( currentIndexChanged( int ) ),this, SLOT( updateRegisterView() ) );
    connect( ui->numCoils, SIGNAL( valueChanged( int ) ),this, SLOT( updateRegisterView() ) );
    connect( ui->startAddr, SIGNAL( valueChanged( int ) ),this, SLOT( updateRegisterView() ) );
    connect( ui->sendBtn, SIGNAL(pressed()),this, SLOT( onSendButtonPress() ) );
    connect( ui->groupBox_105, SIGNAL( toggled(bool)), this, SLOT( onEquationTableChecked(bool)));
}


void
MainWindow::
connectToolbar()
{
    connect(ui->actionSave, SIGNAL(triggered()),this,SLOT(saveCsvFile()));
    connect(ui->actionOpen, SIGNAL(triggered()),this,SLOT(loadCsvFile()));

    /// injection pump rates
    QSignalMapper* signalPumpRatesMapper = new QSignalMapper (this) ; 
    connect(ui->actionInjection_Pump_Rates, SIGNAL(triggered()),signalPumpRatesMapper,SLOT(map()));
    signalPumpRatesMapper -> setMapping(ui->actionInjection_Pump_Rates, 0);
    connect (signalPumpRatesMapper, SIGNAL(mapped(int)), this, SLOT(injectionPumpRates(int)));

    /// injection bucket
    QSignalMapper* signalBucketMapper = new QSignalMapper (this) ; 
    connect(ui->actionInjection_Bucket, SIGNAL(triggered()),signalBucketMapper,SLOT(map()));
    signalBucketMapper -> setMapping(ui->actionInjection_Bucket, 0);
    connect (signalBucketMapper, SIGNAL(mapped(int)), this, SLOT(injectionBucket(int)));

    /// injection mark
    QSignalMapper* signalMarkMapper = new QSignalMapper (this) ; 
    connect(ui->actionInjection_Mark, SIGNAL(triggered()),signalMarkMapper,SLOT(map()));
    signalMarkMapper -> setMapping(ui->actionInjection_Mark, 0);
    connect (signalMarkMapper, SIGNAL(mapped(int)), this, SLOT(injectionMark(int)));

    /// injection method
    QSignalMapper* signalMethodMapper = new QSignalMapper (this) ; 
    connect(ui->actionInjection_Method, SIGNAL(triggered()),signalMethodMapper,SLOT(map()));
    signalMethodMapper -> setMapping(ui->actionInjection_Method, 0);
    connect (signalMethodMapper, SIGNAL(mapped(int)), this, SLOT(injectionMethod(int)));

    /// pressure sensor slope
    QSignalMapper* signalPressure = new QSignalMapper (this) ; 
    connect(ui->actionPressure_Sensor_Slope, SIGNAL(triggered()),signalPressure,SLOT(map()));
    signalPressure -> setMapping(ui->actionPressure_Sensor_Slope, 0);
    connect (signalPressure, SIGNAL(mapped(int)), this, SLOT(onActionPressureSensorSlope(int)));

    /// Minimum reference temperature 
    QSignalMapper* signalMinRefTemp = new QSignalMapper (this) ; 
    connect(ui->actionMin_Ref_Temp, SIGNAL(triggered()),signalMinRefTemp,SLOT(map()));
    signalMinRefTemp -> setMapping(ui->actionMin_Ref_Temp, 0);
    connect (signalMinRefTemp, SIGNAL(mapped(int)), this, SLOT(onMinRefTemp(int)));

	/// Maximum reference temperature 
    QSignalMapper* signalMaxRefTemp = new QSignalMapper (this) ; 
    connect(ui->actionMax_Ref_Temp, SIGNAL(triggered()),signalMaxRefTemp,SLOT(map()));
    signalMaxRefTemp -> setMapping(ui->actionMax_Ref_Temp, 0);
    connect (signalMaxRefTemp, SIGNAL(mapped(int)), this, SLOT(onMaxRefTemp(int)));

	/// injection temperature 
    QSignalMapper* signalInjectionTemp = new QSignalMapper (this) ; 
    connect(ui->actionInjection_Temp, SIGNAL(triggered()),signalInjectionTemp,SLOT(map()));
    signalInjectionTemp -> setMapping(ui->actionInjection_Temp, 0);
    connect (signalInjectionTemp, SIGNAL(mapped(int)), this, SLOT(onInjectionTemp(int)));

    /// Delta Stability X seconds 
    QSignalMapper* signalxDelay = new QSignalMapper (this) ; 
    connect(ui->actionX_seconds, SIGNAL(triggered()),signalxDelay,SLOT(map()));
    signalxDelay -> setMapping(ui->actionX_seconds, 0);
    connect (signalxDelay, SIGNAL(mapped(int)), this, SLOT(onXDelay(int)));

    /// Delta Stability Frequency 
    QSignalMapper* signalY_KHz = new QSignalMapper (this) ; 
    connect(ui->actionY_KHz, SIGNAL(triggered()),signalY_KHz,SLOT(map()));
    signalY_KHz -> setMapping(ui->actionY_KHz, 0);
    connect (signalY_KHz, SIGNAL(mapped(int)), this, SLOT(onYFreq(int)));

    /// Delta Stability Temperature
    QSignalMapper* signalZ_Temp = new QSignalMapper (this) ; 
    connect(ui->actionZ_C, SIGNAL(triggered()),signalZ_Temp,SLOT(map()));
    signalZ_Temp -> setMapping(ui->actionZ_C, 0);
    connect (signalZ_Temp, SIGNAL(mapped(int)), this, SLOT(onZTemp(int)));

	/// Interval - calibration 
    QSignalMapper* signalIntervalCalibration = new QSignalMapper (this) ; 
    connect(ui->actionCalibration, SIGNAL(triggered()),signalIntervalCalibration,SLOT(map()));
    signalIntervalCalibration -> setMapping(ui->actionCalibration, 0);
    connect (signalIntervalCalibration, SIGNAL(mapped(int)), this, SLOT(onIntervalCalibration(int)));

	/// Interval - calibration 
    QSignalMapper* signalIntervalRollover = new QSignalMapper (this) ; 
    connect(ui->actionRollover, SIGNAL(triggered()),signalIntervalRollover,SLOT(map()));
    signalIntervalRollover -> setMapping(ui->actionRollover, 0);
    connect (signalIntervalRollover, SIGNAL(mapped(int)), this, SLOT(onIntervalRollover(int)));


    /// file folders
    connect(ui->actionMain_Server, SIGNAL(triggered()),this,SLOT(onActionMainServer()));
    connect(ui->actionLocal_Server, SIGNAL(triggered()),this,SLOT(onActionLocalServer()));
}


void
MainWindow::
connectCalibrationControls()
{
    QSignalMapper* signalPressed_0 = new QSignalMapper (this) ; 
    connect(ui->pushButton_4, SIGNAL(pressed()),signalPressed_0, SLOT(map()));
    signalPressed_0->setMapping(ui->pushButton_4, 0);
    connect (signalPressed_0, SIGNAL(mapped(int)), this, SLOT(onCalibrationButtonPressed(int)));
}


/// hide/show graph line
void
MainWindow::
toggleLineView_P1(bool b)
{
   (b) ? PIPE[0].series->show() : PIPE[0].series->hide();
}

void
MainWindow::
toggleLineView_P2(bool b)
{
   (b) ? PIPE[1].series->show() : PIPE[1].series->hide();
}

void
MainWindow::
toggleLineView_P3(bool b)
{
   (b) ? PIPE[2].series->show() : PIPE[2].series->hide();
}

void
MainWindow::
connectLineView()
{
    connect(ui->checkBox_19, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_P1(bool)));
    connect(ui->checkBox_20, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_P2(bool)));
    connect(ui->checkBox_21, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_P3(bool)));
}


void
MainWindow::
readJsonConfigFile()
{
    bool ok = false;
	QFile file("./sparky.json");
   	file.open(QIODevice::ReadOnly);
	QString jsonString = file.readAll();
	QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonString.toUtf8()));
    QJsonObject jsonObj = jsonDoc.object(); 
    QVariantMap json = jsonObj.toVariantMap();

    /// file server 
	m_mainServer = json[MAIN_SERVER].toString();
	m_localServer = json[LOCAL_SERVER].toString();

    /// calibration control variables
	LOOP[0].injectionOilPumpRate = json[LOOP_OIL_PUMP_RATE].toDouble();
	LOOP[0].injectionWaterPumpRate = json[LOOP_WATER_PUMP_RATE].toDouble();
	LOOP[0].injectionSmallWaterPumpRate = json[LOOP_SMALL_WATER_PUMP_RATE].toDouble();
	LOOP[0].injectionBucket = json[LOOP_BUCKET].toDouble();
	LOOP[0].injectionMark = json[LOOP_MARK].toDouble();
	LOOP[0].injectionMethod = json[LOOP_METHOD].toDouble();
	LOOP[0].pressureSensorSlope = json[LOOP_PRESSURE].toDouble();
	LOOP[0].minRefTemp = json[LOOP_MIN_TEMP].toInt();
	LOOP[0].maxRefTemp = json[LOOP_MAX_TEMP].toInt();
	LOOP[0].injectionTemp = json[LOOP_INJECTION_TEMP].toInt();
	LOOP[0].xDelay = json[LOOP_X_DELAY].toInt();
	LOOP[0].yFreq = json[LOOP_Y_FREQ].toDouble();
	LOOP[0].zTemp = json[LOOP_Z_TEMP].toDouble();
	LOOP[0].intervalCalibration = json[LOOP_INTERVAL_CALIBRATION].toDouble();
	LOOP[0].intervalRollover = json[LOOP_INTERVAL_ROLLOVER].toDouble();

    /// done. close file.
	file.close();
}


void
MainWindow::
writeJsonConfigFile(void)
{
	QFile file(QStringLiteral("sparky.json"));
    file.open(QIODevice::WriteOnly);

	QJsonObject json;

    /// file server
    json[MAIN_SERVER] = m_mainServer;
	json[LOCAL_SERVER] = m_localServer;

    /// calibration control variables
	json[LOOP_OIL_PUMP_RATE] = QString::number(LOOP[0].injectionOilPumpRate);
	json[LOOP_WATER_PUMP_RATE] = QString::number(LOOP[0].injectionWaterPumpRate);
	json[LOOP_SMALL_WATER_PUMP_RATE] = QString::number(LOOP[0].injectionSmallWaterPumpRate);
	json[LOOP_BUCKET] = QString::number(LOOP[0].injectionBucket);
	json[LOOP_MARK] = QString::number(LOOP[0].injectionMark);
	json[LOOP_METHOD] = QString::number(LOOP[0].injectionMethod);
	json[LOOP_PRESSURE] = QString::number(LOOP[0].pressureSensorSlope);
	json[LOOP_MIN_TEMP] = QString::number(LOOP[0].minRefTemp);
	json[LOOP_MAX_TEMP] = QString::number(LOOP[0].maxRefTemp);
	json[LOOP_INJECTION_TEMP] = QString::number(LOOP[0].injectionTemp);
	json[LOOP_X_DELAY] = QString::number(LOOP[0].xDelay);
	json[LOOP_Y_FREQ] = QString::number(LOOP[0].yFreq);
	json[LOOP_Z_TEMP] = QString::number(LOOP[0].zTemp);
	json[LOOP_INTERVAL_CALIBRATION] = QString::number(LOOP[0].intervalCalibration);
	json[LOOP_INTERVAL_ROLLOVER] = QString::number(LOOP[0].intervalRollover);

    /// file server 
	json[MAIN_SERVER] = m_mainServer;
	json[LOCAL_SERVER] = m_localServer;

	file.write(QJsonDocument(json).toJson());
	file.close();
}

void
MainWindow::
injectionPumpRates(const int index)
{
    bool ok;
    LOOP[0].injectionOilPumpRate = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+ tr("Injection Oil Pump Rate"),tr("Enter Injection Oil Pump Rate [mL/min]"),LOOP[0].injectionOilPumpRate , -10000, 10000, 2, &ok,Qt::WindowFlags(), 1);
    LOOP[0].injectionWaterPumpRate = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+ tr("Injection Water Pump Rate"),tr("Enter Injection Water Pump Rate [mL/min]"), LOOP[0].injectionWaterPumpRate, -10000, 10000, 2, &ok,Qt::WindowFlags(), 1);
    if (ui->radioButton_6->isChecked()) LOOP[0].injectionSmallWaterPumpRate = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+tr("Injection Small Water Pump Rate"),tr("Enter Injection Small Water Pump Rate [mL/min]"),LOOP[0].injectionSmallWaterPumpRate , -10000, 10000, 2, &ok,Qt::WindowFlags(), 1);
    
    /// update json config file
    writeJsonConfigFile();
}

void
MainWindow::
injectionBucket(int index)
{
    bool ok;
    LOOP[0].injectionBucket = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+ tr("Injection Bucket"),tr("Enter Injection Bucket [L]"),LOOP[0].injectionBucket , -10000, 10000, 2, &ok,Qt::WindowFlags(), 1);
     
    /// update json config file
    writeJsonConfigFile();
}

void
MainWindow::
injectionMark(int index)
{
    bool ok;
    LOOP[0].injectionMark = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+tr("Injection Mark"),tr("Enter Injection Mark [L]"), LOOP[0].injectionMark, -10000, 10000, 2, &ok,Qt::WindowFlags(), 1);
     
    /// update json config file
    writeJsonConfigFile();
}


void
MainWindow::
injectionMethod(int index)
{
    bool ok;
    LOOP[0].injectionMethod = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+tr("Injection Method"),tr("Enter Injection Method: 201"), LOOP[0].injectionMethod, -10000, 10000, 2, &ok,Qt::WindowFlags(), 1);

    /// update json config file
    writeJsonConfigFile();
}


void
MainWindow::
onActionPressureSensorSlope(int index)
{
    bool ok;
    LOOP[0].pressureSensorSlope = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+tr("Pressure Sensor Slope"),tr("Enter Pressure Sensor Slope: 1.0"), LOOP[0].pressureSensorSlope, -10000, 10000, 2, &ok,Qt::WindowFlags(), 1);

    /// update json config file
    writeJsonConfigFile();
}

void
MainWindow::
onMinRefTemp(int index)
{
    bool ok;
    LOOP[0].minRefTemp = QInputDialog::getInt(this,"[LOOP "+QString::number(index+1)+"] "+tr("Minimum Reference Temperature"),tr("Enter Minimum Reference Temperature: 20"), LOOP[0].minRefTemp, 0, 1000, 2, &ok);

    /// update json config file
    writeJsonConfigFile();
}

void
MainWindow::
onMaxRefTemp(int index)
{
    bool ok;
    LOOP[0].maxRefTemp = QInputDialog::getInt(this,"[LOOP "+QString::number(index+1)+"] "+tr("Maximum Reference Temperature"),tr("Enter Maximum Reference Temperature: 20"), LOOP[0].maxRefTemp, 0, 1000, 2, &ok);

    /// update json config file
    writeJsonConfigFile();
}

void
MainWindow::
onInjectionTemp(int index)
{
    bool ok;
    LOOP[0].injectionTemp = QInputDialog::getInt(this,"[LOOP "+QString::number(index+1)+"] "+tr("Injection Temperature"),tr("Enter Injection Temperature: 20"), LOOP[0].injectionTemp, 0, 1000, 2, &ok);

    /// update json config file
    writeJsonConfigFile();
}


void
MainWindow::
onXDelay(int index)
{
    bool ok;
    LOOP[0].xDelay = QInputDialog::getInt(this,"[LOOP "+QString::number(index+1)+"] "+tr("X Delay"),tr("Enter Delay Peroid (seconds): 10"), LOOP[0].xDelay, 0, 3600, 2, &ok);

    /// update json config file
    writeJsonConfigFile();
}

void
MainWindow::
onYFreq(int index)
{
    bool ok;
    LOOP[0].yFreq = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+tr("Y Delta Frequency"),tr("Enter Y Delta Fequency (KHz): 1000000"), LOOP[0].yFreq, 0, 1000000, 2, &ok,Qt::WindowFlags(), 1);

    /// update json config file
    writeJsonConfigFile();
}


void
MainWindow::
onZTemp(int index)
{
    bool ok;
    LOOP[0].zTemp = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+tr("Z Delta Temperature"),tr("Enter Z Delta Temperature (°C): 0.1"), LOOP[0].zTemp, 0, 1000, 2, &ok,Qt::WindowFlags(), 1);

    /// update json config file
    writeJsonConfigFile();
}


void
MainWindow::
onIntervalCalibration(const int index)
{
    bool ok;
    LOOP[0].intervalCalibration = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+tr("Calibration Watercut Interval [ % ]"),tr("Enter Calibration Watercut Interval (%): 0.25"), LOOP[0].intervalCalibration, 0, 1000, 2, &ok,Qt::WindowFlags(), 1);

    /// update json config file
    writeJsonConfigFile();
}


void
MainWindow::
onIntervalRollover(const int index)
{
    bool ok;
    LOOP[0].intervalRollover = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+tr("Rollover Watercut Interval [ % ]"),tr("Enter Rollover Watercut Interval (%): 1.0"), LOOP[0].intervalRollover, 0, 1000, 2, &ok,Qt::WindowFlags(), 1);

    /// update json config file
    writeJsonConfigFile();
}


void
MainWindow::
onActionMainServer()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),m_mainServer,QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dirName.isEmpty() && !dirName.isNull()) m_mainServer = dirName;
    
    /// update json config file
    writeJsonConfigFile();
}

void
MainWindow::
onActionLocalServer()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),m_localServer,QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    
    if (!dirName.isEmpty() && !dirName.isNull()) m_localServer = dirName;

    /// update json config file
    writeJsonConfigFile();
}


void
MainWindow::
onEquationTableChecked(bool isTable)
{
    if (!isTable) ui->tableWidget->setRowCount(0);
}


double
MainWindow::
sendCalibrationRequest(int dataType, modbus_t * serialModbus, int func, int address, int num, int ret, uint8_t * dest, uint16_t * dest16, bool is16Bit, bool writeAccess, QString funcType)
{
    //////// address offset /////////
    int addr = address - ADDR_OFFSET;
    /////////////////////////////////

    switch( func )
    {
		case MODBUS_FC_READ_COILS:
            break;
        case MODBUS_FC_READ_DISCRETE_INPUTS:
            ret = modbus_read_input_bits( serialModbus, addr, num, dest );
            break;
        case MODBUS_FC_READ_HOLDING_REGISTERS:
            ret = modbus_read_registers( serialModbus, addr, num, dest16 );
            is16Bit = true;
            break;
        case MODBUS_FC_READ_INPUT_REGISTERS:
            ret = modbus_read_input_registers(serialModbus, addr, num, dest16 );
            is16Bit = true;
            break;
        case MODBUS_FC_WRITE_SINGLE_COIL:
            ret = modbus_write_bit( serialModbus, addr,ui->radioButton_184->isChecked() ? 1 : 0 );
            writeAccess = true;
            num = 1;
            break;
        case MODBUS_FC_WRITE_SINGLE_REGISTER:
            ret = modbus_write_register( serialModbus, addr,ui->lineEdit_111->text().toInt(0, 0) );
            writeAccess = true;
            num = 1;
            break;
        case MODBUS_FC_WRITE_MULTIPLE_COILS:
        {
            uint8_t * data = new uint8_t[num];
            for( int i = 0; i < num; ++i ) data[i] = ui->regTable->item( i, DataColumn )->text().toInt(0, 0);
            ret = modbus_write_bits( serialModbus, addr, num, data );
            delete[] data;
            writeAccess = true;
            break;
        }
        case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
        {
            float value;
            QString qvalue = ui->lineEdit_109->text();
            QTextStream floatTextStream(&qvalue);
            floatTextStream >> value;
            quint16 (*reg)[2] = reinterpret_cast<quint16(*)[2]>(&value);
            uint16_t * data = new uint16_t[2];
            data[0] = (*reg)[1];
            data[1] = (*reg)[0];
            ret = modbus_write_registers( serialModbus, addr, 2, data );
            delete[] data;
            writeAccess = true;
            break;
        }
        default:
            break;
    }

	if( ret == num  )
    {
		isModbusTransmissionFailed = false;

        if( writeAccess )
        {
            m_statusInd->setStyleSheet( "background: #0b0;" );
            m_statusTimer->start( 2000 );
        }
        else
        {
            QString qs_num;
            QString qs_output = "0x";
            bool ok = false;

            ui->regTable->setRowCount( num );
            for( int i = 0; i < num; ++i )
            {
                int data = is16Bit ? dest16[i] : dest[i];
                QString qs_tmp;

                qs_num.sprintf("%d", data);
                qs_tmp.sprintf("%04x", data);
                qs_output.append(qs_tmp);

				if (dataType == INT_R)  // INT_READ
            	{
					ui->lineEdit_111->setText(QString::number(data));
                	return data;
            	}
            	else if (dataType == COIL_R)  // COIL_READ
            	{
					(data) ? ui->radioButton_184->setChecked(true) : ui->radioButton_185->setChecked(true);
                	return (data) ? 1 : 0;
            	}
            }
            
            double d;
			if (dataType == FLOAT_R) // FLOAT_READ
            {
                QByteArray array = QByteArray::fromHex(qs_output.toLatin1());
                d = toFloat(array);
				ui->lineEdit_109->setText(QString::number(d,'f',3)) ;
                return d;
            }
        }
    }
}


void
MainWindow::
saveCsvFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save Equation"), "",tr("CSV file (*.csv);;All Files (*)"));

    if (fileName.isEmpty()) return;
    QFile file(fileName);
    QTextStream out(&file);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("Unable to open file"),file.errorString());
        return;
    }

    for ( int i = 0; i < ui->tableWidget->rowCount(); i++ )
    {
        QString dataStream;

        for (int j=0; j < ui->tableWidget->item(i,6)->text().toInt()+7; j++)
        {
             dataStream.append(ui->tableWidget->item(i,j)->text()+",");
        }

        out << dataStream << endl;
    }

    file.close();
}


void
MainWindow::
loadCsvTemplate()
{
    int line = 0;
    QFile file;
    QString razorTemplatePath = QCoreApplication::applicationDirPath()+"/razor.csv";
    QString eeaTemplatePath = QCoreApplication::applicationDirPath()+"/eea.csv";

    if (ui->radioButton_190->isChecked()) // eea
        file.setFileName(eeaTemplatePath);
    else
        file.setFileName(razorTemplatePath);

    if (!file.open(QIODevice::ReadOnly)) return;

    QTextStream str(&file);

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    while (!str.atEnd()) {

        QString s = str.readLine();
        if (s.size() == 0)
        {
            file.close();
            break;
        }
        else {
            line++;
        }

        // split data
        QStringList valueList = s.split(',');

        if (!valueList[0].contains("*"))
        {
            // insert a new row
            ui->tableWidget->insertRow( ui->tableWidget->rowCount() );

            // insert columns
            while (ui->tableWidget->columnCount() < valueList[6].toInt()+7)
            {
                ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
            }

            // fill the data in the talbe cell
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(valueList[0])); // Name
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(valueList[1])); // Slave
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem(valueList[2])); // Address
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 3, new QTableWidgetItem(valueList[3])); // Type
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 4, new QTableWidgetItem(valueList[4])); // Scale
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 5, new QTableWidgetItem(valueList[5])); // RW
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 6, new QTableWidgetItem(valueList[6])); // Qty
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 7, new QTableWidgetItem(valueList[7])); // Value

            // enable uploadEquationButton
            ui->startEquationBtn->setEnabled(1);
        }
    }

    // set column width
    ui->tableWidget->setColumnWidth(0,120); // Name
    ui->tableWidget->setColumnWidth(1,30);  // Slave
    ui->tableWidget->setColumnWidth(2,50);  // Address
    ui->tableWidget->setColumnWidth(3,40);  // Type
    ui->tableWidget->setColumnWidth(4,30);  // Scale
    ui->tableWidget->setColumnWidth(5,30);  // RW
    ui->tableWidget->setColumnWidth(6,30);  // Qty

    // close file
    file.close();
}

void
MainWindow::
loadCsvFile()
{
    int line = 0;

    QString fileName = QFileDialog::getOpenFileName( this, tr("Open CSV file"), QDir::currentPath(), tr("CSV files (*.csv)") );
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) return;

    QTextStream str(&file);

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    while (!str.atEnd()) {

        QString s = str.readLine();
        if (s.size() == 0)
        {
            file.close();
            break;
        }
        else {
            line++;
        }

        QStringList valueList = s.split(',');
        if (!valueList[0].contains("*"))
        {
            // insert a new row
            ui->tableWidget->insertRow( ui->tableWidget->rowCount() );

            // insert columns
            while (ui->tableWidget->columnCount() < valueList[6].toInt()+7)
            {
                ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
            }

            // fill the data in the talbe cell
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(valueList[0])); // Name
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(valueList[1])); // Slave
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem(valueList[2])); // Address
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 3, new QTableWidgetItem(valueList[3])); // Type
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 4, new QTableWidgetItem(valueList[4])); // Scale
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 5, new QTableWidgetItem(valueList[5])); // RW
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 6, new QTableWidgetItem(valueList[6])); // Qty
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 7, new QTableWidgetItem(valueList[7])); // Value

            // fill the value list
            for (int j = 0; j < valueList[6].toInt(); j++)
            {
                QString cellData = valueList[7+j];
                if (valueList[3].contains("int")) cellData = cellData.mid(0, cellData.indexOf("."));

                ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, j+7, new QTableWidgetItem(cellData));
            }

            // enable uploadEquationButton
            ui->startEquationBtn->setEnabled(1);
        }
    }

    // set column width
    ui->tableWidget->setColumnWidth(0,120); // Name
    ui->tableWidget->setColumnWidth(1,30);  // Slave
    ui->tableWidget->setColumnWidth(2,50);  // Address
    ui->tableWidget->setColumnWidth(3,40);  // Type
    ui->tableWidget->setColumnWidth(4,30);  // Scale
    ui->tableWidget->setColumnWidth(5,30);  // RW
    ui->tableWidget->setColumnWidth(6,30);  // Qty

    // close file
    file.close();
}

void
MainWindow::
onEquationButtonPressed()
{
    ui->startEquationBtn->setEnabled(false);
    ui->startEquationBtn->setText( tr("Loading") );

    if (ui->radioButton_188->isChecked())
    {
        if( m_pollTimer->isActive() )
        {
            m_pollTimer->stop();
            ui->startEquationBtn->setText( tr("Loading") );
        }
        else
        {
            // if polling requested then enable timer
            if( m_poll )
            {
                m_pollTimer->start( 1000 );
                ui->sendBtn->setText( tr("Loading") );
            }

            onUploadEquation();
        }
    }
    else
    {
        if( m_pollTimer->isActive() )
        {
            m_pollTimer->stop();
            ui->startEquationBtn->setText( tr("Loading") );
        }
        else
        {
            // if polling requested then enable timer
            if( m_poll )
            {
                m_pollTimer->start( 1000 );
                ui->sendBtn->setText( tr("Loading") );
            }
        
            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(0);

            onDownloadEquation();
        }
    }

    ui->startEquationBtn->setText(tr("Start"));
    ui->startEquationBtn->setEnabled(true);
}


void
MainWindow::
onDownloadButtonChecked(bool isChecked)
{
    if (isChecked)
    {
        ui->startEquationBtn->setEnabled(true);
    }
    else {
        ui->startEquationBtn->setEnabled(true);
    }
}


void
MainWindow::
onDownloadEquation()
{
    int value = 0;
    int rangeMax = 0;

    ui->slaveID->setValue(1);                           // set slave ID
    ui->radioButton_187->setChecked(true);              // read mode
    ui->startEquationBtn->setEnabled(false);

    // load empty equation file
    loadCsvTemplate();

    /// get rangeMax of progressDialog
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) rangeMax+=ui->tableWidget->item(i,6)->text().toInt();

    QProgressDialog progress("Downloading...", "Abort", 0, rangeMax, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setAutoClose(true);
    progress.setAutoReset(true);

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
         int regAddr = ui->tableWidget->item(i,2)->text().toInt();

         if (ui->tableWidget->item(i,3)->text().contains("float"))
         {
             ui->numCoils->setValue(2);                  // 2 bytes
             ui->radioButton_181->setChecked(TRUE);      // float type
             ui->functionCode->setCurrentIndex(3);       // function code
             for (int x=0; x < ui->tableWidget->item(i,6)->text().toInt(); x++)
             {
                if (progress.wasCanceled()) return;
                if (ui->tableWidget->item(i,6)->text().toInt() > 1) progress.setLabelText("Downloading \""+ui->tableWidget->item(i,0)->text()+"\" "+QString::number(x+1));
                else progress.setLabelText("Downloading \""+ui->tableWidget->item(i,0)->text()+"\"");
                progress.setValue(value++);

                 ui->startAddr->setValue(regAddr);       // set address
                 onSendButtonPress();                    // send
                 delay();
                 regAddr = regAddr+2;                    // update reg address
                 ui->tableWidget->setItem( i, x+7, new QTableWidgetItem(ui->lineEdit_109->text()));
             }
         }
         else if (ui->tableWidget->item(i,3)->text().contains("int"))
         {
            if (progress.wasCanceled()) return;
            progress.setLabelText("Downloading \""+ui->tableWidget->item(i,0)->text()+"\"");
            progress.setValue(value++);

             ui->numCoils->setValue(1);                  // 1 byte
             ui->radioButton_182->setChecked(TRUE);      // int type
             ui->functionCode->setCurrentIndex(3);       // function code
             ui->startAddr->setValue(regAddr);           // address
             onSendButtonPress();                        // send
             delay();
             ui->tableWidget->setItem( i, 7, new QTableWidgetItem(ui->lineEdit_111->text()));
         }
         else
         {
            if (progress.wasCanceled()) return;
            progress.setLabelText("Downloading \""+ui->tableWidget->item(i,0)->text()+"\"");
            progress.setValue(value++);

             ui->numCoils->setValue(1);                  // 1 byte
             ui->radioButton_183->setChecked(TRUE);      // coil type
             ui->functionCode->setCurrentIndex(0);       // function code
             ui->startAddr->setValue(regAddr);           // address
             onSendButtonPress();                        // send
             delay();
         }
     }
}



void
MainWindow::
onUploadEquation()
{
    int value = 0;
    int rangeMax = 0;
    bool isReinit = false;
    QMessageBox msgBox;
	isModbusTransmissionFailed = false;

    /// get rangeMax of progressDialog
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) rangeMax+=ui->tableWidget->item(i,6)->text().toInt();
    
    msgBox.setText("You can reinitialize existing registers and coils.");
    msgBox.setInformativeText("Do you want to reinitialize registers and coils?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    switch (ret) {
        case QMessageBox::Yes:
            isReinit = true;
            break;
        case QMessageBox::No:
            isReinit = false;
            break;
        case QMessageBox::Cancel:
        default: return;
    }
        
    ui->slaveID->setValue(1);                           // set slave ID
    ui->radioButton_186->setChecked(true);              // write mode    

    QProgressDialog progress("Uploading...", "Abort", 0, rangeMax, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setAutoClose(true);
    progress.setAutoReset(true);

    /// unlock fct default regs & coils (999)
    ui->numCoils->setValue(1);                      // 1 byte
    ui->radioButton_183->setChecked(TRUE);          // coil type
    ui->functionCode->setCurrentIndex(4);           // function type
    ui->startAddr->setValue(999);                   // address
    ui->radioButton_184->setChecked(true);          // set value
    if (progress.wasCanceled()) return;
    progress.setValue(0);
    progress.setLabelText("Unlocking factory registers....");
    onSendButtonPress();
    delay();
	if (isModbusTransmissionFailed) 
	{
		isModbusTransmissionFailed = false;
		msgBox.setText("Modbus Transmission Failed.");
    	msgBox.setInformativeText("Do you want to continue with next item?");
    	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    	msgBox.setDefaultButton(QMessageBox::No);
    	int ret = msgBox.exec();
    	switch (ret) {
        	case QMessageBox::Yes:
            	break;
        	case QMessageBox::No:
        	default: return;
    	}

	}

    if (isReinit)
    {
        ui->numCoils->setValue(1);                      // set byte count 1
        ui->radioButton_183->setChecked(TRUE);          // set type coil 
        ui->functionCode->setCurrentIndex(4);           // set function type
        ui->radioButton_185->setChecked(true);          // set coils unlocked 

        ui->startAddr->setValue(25);                    // set address 25
        if (progress.wasCanceled()) return;
        progress.setLabelText("Reinitializing registers....");
        progress.setValue(0);
        onSendButtonPress();
        delay();
		if (isModbusTransmissionFailed) 
		{
			isModbusTransmissionFailed = false;
			msgBox.setText("Modbus Transmission Failed.");
    		msgBox.setInformativeText("Do you want to continue with next item?");
    		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    		msgBox.setDefaultButton(QMessageBox::No);
    		int ret = msgBox.exec();
    		switch (ret) {
        		case QMessageBox::Yes: break;
        		case QMessageBox::No:
        		default: return;
    		}

		}
        ui->startAddr->setValue(26);                    // set address 26
        if (progress.wasCanceled()) return;
        progress.setValue(0);
        onSendButtonPress();
        delay(8);                                       // need extra time to restart
		if (isModbusTransmissionFailed) 
		{
			isModbusTransmissionFailed = false;
			msgBox.setText("Modbus Transmission Failed.");
    		msgBox.setInformativeText("Do you want to continue with next item?");
    		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    		msgBox.setDefaultButton(QMessageBox::No);
    		int ret = msgBox.exec();
    		switch (ret) {
        		case QMessageBox::Yes: break;
        		case QMessageBox::No:
        		default: return;
    		}

		}	
        /// unlock fct default regs & coils (999)
        ui->numCoils->setValue(1);                      // 1 byte
        ui->radioButton_183->setChecked(TRUE);          // coil type
        ui->functionCode->setCurrentIndex(4);           // function type
        ui->startAddr->setValue(999);                   // address
        ui->radioButton_184->setChecked(true);          // set value
        if (progress.wasCanceled()) return;
        progress.setValue(0);
        progress.setLabelText("Unlocking factory registers....");
        onSendButtonPress();
        delay();
		if (isModbusTransmissionFailed) 
		{
			isModbusTransmissionFailed = false;
			msgBox.setText("Modbus Transmission Failed.");
    		msgBox.setInformativeText("Do you want to continue with next item?");
    		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    		msgBox.setDefaultButton(QMessageBox::No);
    		int ret = msgBox.exec();
    		switch (ret) {
        		case QMessageBox::Yes: break;
        		case QMessageBox::No:
        		default: return;
    		}
		}
   }

   for (int i = 0; i < ui->tableWidget->rowCount(); i++)
   {
        int regAddr = ui->tableWidget->item(i,2)->text().toInt();
        if (ui->tableWidget->item(i,3)->text().contains("float"))
        {
            ui->numCoils->setValue(2);                  // 2 bytes
            ui->radioButton_181->setChecked(TRUE);      // float type
            ui->functionCode->setCurrentIndex(7);       // function code
            for (int x=0; x < ui->tableWidget->item(i,6)->text().toInt(); x++)
            {
                QString val = ui->tableWidget->item(i,7+x)->text();
                ui->startAddr->setValue(regAddr);       // set address
                ui->lineEdit_109->setText(val);         // set value
                if (progress.wasCanceled()) return;
                if (ui->tableWidget->item(i,6)->text().toInt() > 1) progress.setLabelText("Uploading \""+ui->tableWidget->item(i,0)->text()+"["+QString::number(x+1)+"]"+"\""+","+" \""+val+"\"");
                else progress.setLabelText("Uploading \""+ui->tableWidget->item(i,0)->text()+"\""+","+" \""+val+"\"");
                progress.setValue(value++);
                onSendButtonPress();                    // send
                regAddr += 2;                           // update reg address
                delay();
				if (isModbusTransmissionFailed) 
				{
					isModbusTransmissionFailed = false;
					if (ui->tableWidget->item(i,6)->text().toInt() > 1) msgBox.setText("Modbus Transmission Failed: "+ui->tableWidget->item(i,0)->text()+"["+QString::number(x+1)+"]");
					else msgBox.setText("Modbus Transmission Failed: "+ui->tableWidget->item(i,0)->text());
    				msgBox.setInformativeText("Do you want to continue with next item?");
    				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    				msgBox.setDefaultButton(QMessageBox::No);
    				int ret = msgBox.exec();
    				switch (ret) {
        				case QMessageBox::Yes: break;
        				case QMessageBox::No:
        				default: return;
    				}
				}
            }
        }
        else if (ui->tableWidget->item(i,3)->text().contains("int"))
        {
            QString val = ui->tableWidget->item(i,7)->text();
            ui->numCoils->setValue(1);                  // 1 byte
            ui->radioButton_182->setChecked(TRUE);      // int type
            ui->functionCode->setCurrentIndex(5);       // function code
            ui->lineEdit_111->setText(val);             // set value
            ui->startAddr->setValue(regAddr);           // address
            if (progress.wasCanceled()) return;
            progress.setLabelText("Uploading \""+ui->tableWidget->item(i,0)->text()+"\""+","+" \""+val+"\"");
            progress.setValue(value++);
            onSendButtonPress();                        // send
            delay();
			if (isModbusTransmissionFailed) 
			{
				isModbusTransmissionFailed = false;
				msgBox.setText("Modbus Transmission Failed: "+ui->tableWidget->item(i,0)->text());
    			msgBox.setInformativeText("Do you want to continue with next item?");
    			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    			msgBox.setDefaultButton(QMessageBox::No);
    			int ret = msgBox.exec();
    			switch (ret) {
        			case QMessageBox::Yes: break;
        			case QMessageBox::No:
        			default: return;
    			}
			}
        }
        else
        {
            ui->numCoils->setValue(1);                  // 1 byte
            ui->radioButton_183->setChecked(TRUE);      // coil type
            ui->functionCode->setCurrentIndex(4);       // function code
            ui->startAddr->setValue(regAddr);           // address
            if (ui->tableWidget->item(i,7)->text().toInt() == 1)
            {
                ui->radioButton_184->setChecked(true);  // TRUE
                progress.setLabelText("Uploading \""+ui->tableWidget->item(i,0)->text()+"\""+","+" \"1\"");
            }
            else 
            {
                ui->radioButton_185->setChecked(true);  // FALSE
                progress.setLabelText("Uploading \""+ui->tableWidget->item(i,0)->text()+"\""+","+" \"0\"");
            }
            if (progress.wasCanceled()) return;
            progress.setValue(value++);
            onSendButtonPress();                        // send
            delay();
			if (isModbusTransmissionFailed) 
			{
				isModbusTransmissionFailed = false;
				msgBox.setText("Modbus Transmission Failed: "+ui->tableWidget->item(i,0)->text());
    			msgBox.setInformativeText("Do you want to continue with next item?");
    			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    			msgBox.setDefaultButton(QMessageBox::No);
    			int ret = msgBox.exec();
    			switch (ret) {
        			case QMessageBox::Yes: break;
        			case QMessageBox::No:
        			default: return;
    			}
			}
        }
    }

    /// update factory default values
    ui->numCoils->setValue(1);                      // 1 byte
    ui->radioButton_183->setChecked(TRUE);          // coil type
    ui->functionCode->setCurrentIndex(4);           // function code
    ui->radioButton_184->setChecked(true);          // set value

    /// unlock factory default registers
    ui->startAddr->setValue(999);                   // address 999
    onSendButtonPress();
    delay();

    /// update factory default registers
    ui->startAddr->setValue(9999);                  // address 99999
    onSendButtonPress();
    delay();
}

void
MainWindow::
onUnlockFactoryDefaultBtnPressed()
{
    ui->numCoils->setValue(1);                      // 1 byte
    ui->radioButton_183->setChecked(TRUE);          // coil type
    ui->functionCode->setCurrentIndex(4);           // function code
    ui->radioButton_184->setChecked(true);          // set value

    /// unlock factory default registers
    ui->startAddr->setValue(999);                   // address 999
    onSendButtonPress();
    delay();
}


void
MainWindow::
onLockFactoryDefaultBtnPressed()
{
    ui->numCoils->setValue(1);                      // 1 byte
    ui->radioButton_183->setChecked(TRUE);          // coil type
    ui->functionCode->setCurrentIndex(4);           // function code
    ui->radioButton_185->setChecked(true);          // set value

    /// unlock factory default registers
    ui->startAddr->setValue(999);                   // address 999
    onSendButtonPress();
    delay();
}


void
MainWindow::
onUpdateFactoryDefaultPressed()
{
    if ( ui->radioButton_192->isChecked()) return;

    QMessageBox msgBox;

    msgBox.setText("Factory default values will be permanently changed.");
    msgBox.setInformativeText("Are you sure you want to do this?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret) {
        case QMessageBox::Yes: break;
        case QMessageBox::No:
        default: return;
    }
 
    ui->numCoils->setValue(1);                      // 1 byte
    ui->radioButton_183->setChecked(TRUE);          // coil type
    ui->functionCode->setCurrentIndex(4);           // function code
    ui->radioButton_184->setChecked(true);          // set value

    /// update factory default registers
    ui->startAddr->setValue(9999);                  // address 99999
    onSendButtonPress();
    delay();
}


void
MainWindow::
connectProfiler()
{
    connect(ui->radioButton_193, SIGNAL(pressed()), this, SLOT(onUnlockFactoryDefaultBtnPressed()));
    connect(ui->radioButton_192, SIGNAL(pressed()), this, SLOT(onLockFactoryDefaultBtnPressed()));
    connect(ui->pushButton_2, SIGNAL(pressed()), this, SLOT(onUpdateFactoryDefaultPressed()));
    connect(ui->startEquationBtn, SIGNAL(pressed()), this, SLOT(onEquationButtonPressed()));
    connect(ui->radioButton_189, SIGNAL(toggled(bool)), this, SLOT(onDownloadButtonChecked(bool)));
}


void
MainWindow::
setupModbusPorts()
{
    setupModbusPort();
}


int
MainWindow::
setupModbusPort()
{
    QSettings s;

    int portIndex = 0;
    int i = 0;
    ui->comboBox->disconnect();
    ui->comboBox->clear();
    foreach( QextPortInfo port, QextSerialEnumerator::getPorts() )
    {
        ui->comboBox->addItem( port.friendName );

        if( port.friendName == s.value( "serialinterface" ) )
        {
            portIndex = i;
        }
        ++i;
    }
    ui->comboBox->setCurrentIndex( portIndex );
    ui->comboBox_2->setCurrentIndex(0);
    ui->comboBox_3->setCurrentIndex(0);
    ui->comboBox_4->setCurrentIndex(0);
    ui->comboBox_5->setCurrentIndex(0);

    connect( ui->comboBox, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort( int ) ) );
    connect( ui->comboBox_2, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort( int ) ) );
    connect( ui->comboBox_3, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort( int ) ) );
    connect( ui->comboBox_4, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort( int ) ) );
    connect( ui->comboBox_5, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort( int ) ) );

    changeSerialPort( portIndex );
    return portIndex;
}


//static inline QString embracedString( const QString & s )
//{
    //return s.section( '(', 1 ).section( ')', 0, 0 );
//}


void
MainWindow::
releaseSerialModbus(const int loop)
{
    modbus_close( LOOP[0].serialModbus );
    modbus_free( LOOP[0].serialModbus );
    LOOP[0].serialModbus = NULL;
    updateLoopTabIcon(0, false);
}


void
MainWindow::
changeSerialPort( int )
{
    const int iface = ui->comboBox->currentIndex();

    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    if( !ports.isEmpty() )
    {
        QSettings settings;
        settings.setValue( "serialinterface", ports[iface].friendName );
        settings.setValue( "serialbaudrate", ui->comboBox_2->currentText() );
        settings.setValue( "serialparity", ui->comboBox_3->currentText() );
        settings.setValue( "serialdatabits", ui->comboBox_4->currentText() );
        settings.setValue( "serialstopbits", ui->comboBox_5->currentText() );
        QString port = ports[iface].portName;

        // is it a serial port in the range COM1 .. COM9?
        if ( port.startsWith( "COM" ) )
        {
            // use windows communication device name "\\.\COMn"
            port = "\\\\.\\" + port;
        }

        char parity;
        switch( ui->comboBox_3->currentIndex() )
        {
            case 1: parity = 'O'; break;
            case 2: parity = 'E'; break;
            default:
            case 0: parity = 'N'; break;
        }

        changeModbusInterface(port, parity, 0);
        onRtuPortActive(true,0);
    }
    else emit connectionError( tr( "No serial port found at Loop_1" ) );
}


void
MainWindow::
changeModbusInterface(const QString& port, char parity, const int loop)
{
    releaseSerialModbus(0);
    LOOP[0].serialModbus = modbus_new_rtu( port.toLatin1().constData(),ui->comboBox_2->currentText().toInt(),parity,ui->comboBox_3->currentText().toInt(),ui->comboBox_4->currentText().toInt() );
            
    if( modbus_connect( LOOP[0].serialModbus ) == -1 )
    {
        emit connectionError( tr( "Could not connect serial port at LOOP " )+QString::number(0) );
        releaseSerialModbus(0);
    }
    else
        updateLoopTabIcon(0, true);
}


void
MainWindow::
onCheckBoxChecked(bool checked)
{
    clearMonitors();

    if (checked) setupModbusPort();
    else releaseSerialModbus(0);

    onRtuPortActive(checked,0);
}


void
MainWindow::
clearMonitors()
{
    ui->rawData->clear();
    ui->regTable->setRowCount(0);
    ui->busMonTable->setRowCount(0);
}


void
MainWindow::
updateStability(const bool isF, const int pipe, const int value)
{
    if (pipe == 0) (isF) ? ui->progressBar->setValue(value) : ui->progressBar_2->setValue(value);
    else if (pipe == 1) (isF) ? ui->progressBar_4->setValue(value) : ui->progressBar_3->setValue(value);
    else if (pipe == 2) (isF) ? ui->progressBar_6->setValue(value) : ui->progressBar_5->setValue(value);
    else if (pipe == ALL) 
    {
        ui->progressBar->setValue(value);
        ui->progressBar_2->setValue(value);
        ui->progressBar_4->setValue(value);
        ui->progressBar_3->setValue(value);
        ui->progressBar_6->setValue(value);
        ui->progressBar_5->setValue(value);
    }
}


///////////////////////////////////////////
/// depending on the mode,
/// we use some fields differently.
///////////////////////////////////////////

void
MainWindow::
onModeChanged(bool isLow)
{
    ui->groupBox_65->setEnabled(!isLow); // salt
    ui->groupBox_113->setEnabled(!isLow); // oil temp
	ui->groupBox_10->setEnabled(!isLow); // oil run

    if (isLow)
    {
        ui->groupBox_9->setTitle("WATERCUT [%]");
        ui->label_4->setText("HIGH");
        ui->label_5->setText("LOW");
        ui->lineEdit_37->setText("4");
        ui->lineEdit_38->setText("0");
    }
    else
    {
        ui->groupBox_9->setTitle("WATER RUN [%]");
        ui->label_4->setText("START");
        ui->label_5->setText("STOP");
        ui->lineEdit_37->setText("99");
        ui->lineEdit_38->setText("60");
    }
}

void
MainWindow::
connectModeChanged()
{
    connect(ui->radioButton_6, SIGNAL(toggled(bool)), this, SLOT(onModeChanged(bool)));
}


void
MainWindow::
onProductSelected(const bool prd)
{
    if (ui->tabWidget_2->currentIndex() == 0) updateRegisters(prd,0);
}


void
MainWindow::
connectProductBtnPressed()
{
    connect(ui->radioButton, SIGNAL(toggled(bool)), this, SLOT(onProductSelected(bool)));
}


void
MainWindow::
onLoopTabChanged(int index)
{
}


void
MainWindow::
updateLoopTabIcon(const int loop, const bool connected)
{
    QIcon icon(QLatin1String(":/green.ico"));
    QIcon icoff(QLatin1String(":/red.ico"));
    if (loop == ALL)
    {
        (connected) ? ui->tabWidget_2->setTabIcon(0,icon) : ui->tabWidget_2->setTabIcon(0,icoff);
    }
    else
    {
        (connected) ? ui->tabWidget_2->setTabIcon(loop,icon) : ui->tabWidget_2->setTabIcon(loop,icoff);
    }
}


void
MainWindow::
updateStartButtonLabel(const int loop)
{
    LOOP[0].isCal ? ui->pushButton_4->setText("S T O P") : ui->pushButton_4->setText("S T A R T");
}

void
MainWindow::
initializeTabIcons()
{
    QIcon icon(QLatin1String(":/red.ico"));
    ui->tabWidget_2->setTabIcon(0,icon);
}


void
MainWindow::
createCalibrateFile(const int sn, const int pipe, const QString startValue, const QString stopValue, const QString saltValue, const QString filename)
{
    QDateTime currentDataTime = QDateTime::currentDateTime();

    /// headers 
    QString header0;
    LOOP[0].isEEA ? header0 = EEA_INJECTION_FILE : header0 = RAZ_INJECTION_FILE;
    QString header1("SN"+QString::number(sn)+" | "+LOOP[0].mode.split("\\").at(1) +" | "+currentDataTime.toString()+" | "+PIPE[pipe].pipeId+" | "+PROJECT+RELEASE_VERSION); 
    QString header2("INJECTION:  "+startValue+" % "+"to "+stopValue+" % "+"Watercut at "+saltValue+" % "+"Salinity\n");
    QString header3 = HEADER3;
    QString header4 = HEADER4;
    QString header5 = HEADER5;

	/// CALIBRAT, ADJUSTED, ROLLOVER (LOWCUT ONLY)
    if (LOOP[0].mode == LOW)
    {
        QString rolloverValue;
        QString header21;
        QString header22;

        /// create headers
        header2 = "TEMPERATURE:  "+startValue+" °C "+"to "+stopValue+" °C\n";
        header21 = "INJECTION:  "+LOOP[0].waterRunStop->text()+" % "+"to "+LOOP[0].waterRunStart->text()+" % Watercut\n";
        header22 = "ROLLOVER:  "+rolloverValue+" % "+"to "+"rollover\n";

		/// set filenames
		PIPE[pipe].fileCalibrate.setFileName(PIPE[pipe].mainDirPath+"\\"+QString("CALIBRAT").append(LOOP[0].calExt));
		PIPE[pipe].fileAdjusted.setFileName(PIPE[pipe].mainDirPath+"\\"+QString("ADJUSTED").append(LOOP[0].adjExt));
		PIPE[pipe].fileRollover.setFileName(PIPE[pipe].mainDirPath+"\\"+QString("ROLLOVER").append(LOOP[0].rolExt));

        /// update PIPE object 
        if (filename == "CALIBRAT") 
		{
			/// CALIBRAT
        	if (!QFileInfo(PIPE[pipe].fileCalibrate).exists()) 
        	{
            	QTextStream streamCalibrate(&PIPE[pipe].fileCalibrate);
            	PIPE[pipe].fileCalibrate.open(QIODevice::WriteOnly | QIODevice::Text);
            	streamCalibrate << header0 << '\n' << header1 << '\n' << header21 << '\n' << header3 << '\n' << header4 << '\n' << header5 << '\n';
            	PIPE[pipe].fileCalibrate.close();
		
				/// update file list	
				updateFileList(QFileInfo(PIPE[pipe].fileCalibrate).fileName(), sn, LOOP[0].mode, pipe);
        	}
		}
        else if (filename == "ADJUSTED") 
		{
        	if (!QFileInfo(PIPE[pipe].fileAdjusted).exists())
        	{ 
            	QTextStream streamAdjusted(&PIPE[pipe].fileAdjusted);
            	PIPE[pipe].fileAdjusted.open(QIODevice::WriteOnly | QIODevice::Text);
            	streamAdjusted << header0 << '\n' << header1 << '\n' << header21 << '\n' << header3 << '\n' << header4 << '\n' << header5 << '\n';
            	PIPE[pipe].fileAdjusted.close();

				/// update file list	
				updateFileList(QFileInfo(PIPE[pipe].fileAdjusted).fileName(), sn, LOOP[0].mode, pipe);
        	}	
		}
        else if (filename == "ROLLOVER") 
		{
	        /// ROLLOVER 
   		    if (!QFileInfo(PIPE[pipe].fileRollover).exists()) 
        	{
            	QTextStream streamRollover(&PIPE[pipe].fileRollover);
            	PIPE[pipe].fileRollover.open(QIODevice::WriteOnly | QIODevice::Text);
            	streamRollover << header0 << '\n' << header1 << '\n' << header22 << '\n' << header3 << '\n' << header4 << '\n' << header5 << '\n';
            	PIPE[pipe].fileRollover.close();
			
				/// update file list	
				updateFileList(QFileInfo(PIPE[pipe].fileRollover).fileName(), sn, LOOP[0].mode, pipe);
        	}
		}
    }
}


void
MainWindow::
updateFileList(const QString fileName, const int sn, const QString mode, const int pipe)
{
    QString header0;
    QFile file;
    QTextStream streamList(&file);
    QDateTime currentDataTime = QDateTime::currentDateTime();

    LOOP[0].isEEA ? header0 = EEA_INJECTION_FILE : header0 = RAZ_INJECTION_FILE;
    QString header1("SN"+QString::number(sn)+" | "+LOOP[0].mode.split("\\").at(1) +" | "+currentDataTime.toString()+" | "+PIPE[pipe].pipeId+" | "+PROJECT+RELEASE_VERSION); 
 
    file.setFileName(PIPE[pipe].mainDirPath+"\\"+FILE_LIST);
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

	/// write header to streamList
    if (QFileInfo(file).exists())
	{
		if (QFileInfo(file).size() == 0) streamList << header0 << '\n' << header1 << '\n' << '\n' << fileName << '\n';
    	else streamList << fileName << '\n';
	}

    file.close();
}


void
MainWindow::
createLoopFile(const int sn, const QString mode, const QString startValue, const QString stopValue, const QString saltValue, const int pipe)
{
	/// set file names
    QDateTime currentDataTime = QDateTime::currentDateTime();
    QString fileName = QString(startValue).append("_").append(stopValue).append(LOOP[0].filExt);

    /// headers 
    QString header0;
    LOOP[0].isEEA ? header0 = EEA_INJECTION_FILE : header0 = RAZ_INJECTION_FILE;
    QString header1("SN"+QString::number(sn)+" | "+LOOP[0].mode.split("\\").at(1) +" | "+currentDataTime.toString()+" | "+PIPE[pipe].pipeId+" | "+PROJECT+RELEASE_VERSION); 
    QString header2("INJECTION:  "+startValue+" % "+"to "+stopValue+" % "+"Watercut at "+saltValue+" % "+"Salinity\n");
    if (LOOP[0].mode == LOW) header2 = "TEMPERATURE:  "+startValue+" °C "+"to "+stopValue+" °C\n";
    QString header3 = HEADER3;
    QString header4 = HEADER4;
    QString header5 = HEADER5;

    /// update PIPE object for later use
    PIPE[pipe].file.setFileName(PIPE[pipe].mainDirPath+"\\"+fileName);

    /// stream
    QTextStream stream(&PIPE[pipe].file);

    /// open file
    PIPE[pipe].file.open(QIODevice::WriteOnly | QIODevice::Text);

    /// write headers to stream
    stream << header0 << '\n' << header1 << '\n' << header2 << '\n' << header3 << '\n' << header4 << '\n' << header5 << '\n';

    /// close file
    PIPE[pipe].file.close();

	/// update file list
	updateFileList(fileName, sn, mode, pipe);
}


void
MainWindow::
displayMessage(const QString windowTitle, const QString line1, const QString line2 = "")
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(windowTitle);
    msgBox.setText(line1);
    msgBox.setInformativeText(line2);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int msgAnswer = msgBox.exec();
    switch (msgAnswer) {
        case QMessageBox::Yes: return;
        default: return;
    }
}


bool
MainWindow::
getUserInputMessage(const QString msg_0, const QString msg_1, const QString msg_2)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(msg_0);
    msgBox.setText(msg_1);
    msgBox.setInformativeText(msg_2);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    int msgAnswer = msgBox.exec();
    switch (msgAnswer) {
        case QMessageBox::Ok: return true;
        case QMessageBox::Cancel: return false; 
        default: return true;
    }
}


void
MainWindow::
setProductAndCalibrationMode(const int loop)
{
   	/// product
   	LOOP[0].isEEA = ui->radioButton->isChecked();  
    
	/// mode
   	if (ui->radioButton_3->isChecked()) LOOP[0].mode = HIGH;
   	else if (ui->radioButton_4->isChecked()) LOOP[0].mode = FULL;
   	else if (ui->radioButton_5->isChecked()) LOOP[0].mode = MID;
   	else if (ui->radioButton_6->isChecked()) LOOP[0].mode = LOW;

	/// set file extensions
	if (LOOP[0].mode == HIGH) 
	{
		LOOP[0].filExt = ".HCI";
		LOOP[0].calExt = ".HCI";
		LOOP[0].adjExt = ".HCI";
		LOOP[0].rolExt = ".HCR";
	}
	else if (LOOP[0].mode == FULL) 
	{
		LOOP[0].filExt = ".FCI";
		LOOP[0].calExt = ".FCI";
		LOOP[0].adjExt = ".FCI";
		LOOP[0].rolExt = ".FCR";
	}
	else if (LOOP[0].mode == MID) 
	{
		LOOP[0].filExt = ".MCI";
		LOOP[0].calExt = ".MCI";
		LOOP[0].adjExt = ".MCI";
		LOOP[0].rolExt = ".MCR";
	}
	else if (LOOP[0].mode == LOW) 
	{
		LOOP[0].filExt = ".LCT";
		LOOP[0].calExt = ".LCI";
		LOOP[0].adjExt = ".LCI";
		LOOP[0].rolExt = ".LCR";
	}
}


bool
MainWindow::
validateSerialNumber(const int loop, modbus_t * serialModbus)
{
	uint8_t dest[1024];
    uint16_t * dest16 = (uint16_t *) dest;
    int ret = -1;
    bool is16Bit = false;
    bool writeAccess = false;
    const QString funcType = descriptiveDataTypeName( FUNC_READ_INT );

	/// unlock FCT registers
   	modbus_write_register(serialModbus,999,1);

	for (int pipe = 0; pipe < 3; pipe++)
	{
		/// slave id exists
		if (PIPE[pipe].slave->text().isEmpty()) PIPE[pipe].isCal = false;
		else
		{
			/// set slave
   			memset( dest, 0, 1024 );
   			modbus_set_slave( serialModbus, PIPE[pipe].slave->text().toInt());

       		/// read pipe serial number
   			sendCalibrationRequest(FLOAT_R, serialModbus, FUNC_READ_INT, LOOP[0].ID_SN_PIPE, BYTE_READ_INT, ret, dest, dest16, is16Bit, writeAccess, funcType);

			/// verify if serial number matches with pipe
   			if (*dest16 != PIPE[pipe].slave->text().toInt()) 
			{
       			displayMessage(QString("LOOP ")+QString::number(0 + 1),QString("LOOP ")+QString::number(0 + 1)+QString(" PIPE ")+QString::number(pipe + 1),"Invalid Serial Number!");

				/// lock FCT registers
   				modbus_write_register(serialModbus,999,0);

				/// cancel calibration
				LOOP[0].isCal = false;

				/// if any sn exists but invalid, then we exits.
				return false;
			}
			else PIPE[pipe].isCal = true;
		}
	}

	/// lock FCT registers
   	modbus_write_register(serialModbus,999,0);

    return true;
}


bool
MainWindow::
prepareCalibration(const int loop)
{
	/// validate 0 volume
	if (LOOP[0].loopVolume->text().toDouble() < 1)
	{
       	displayMessage(QString("LOOP ")+QString::number(0 + 1),QString("LOOP ")+QString::number(0 + 1),"No valid 0 volume");
		return false;
	}

	/// pipe sn exists?
	if (PIPE[0*3].slave->text().isEmpty() && PIPE[0*3+1].slave->text().isEmpty() && PIPE[0*3+2].slave->text().isEmpty()) 
	{
       	displayMessage(QString("LOOP ")+QString::number(0 + 1),QString("LOOP ")+QString::number(0 + 1),"No valid serial number found");
		return false;
	}

	/// toggle start button
    LOOP[0].isCal = !LOOP[0].isCal;

	/// calibration on
	if (LOOP[0].isCal)
	{
		/// set product & calibration mode
		setProductAndCalibrationMode(0);

    	/// update product specific register IDs
    	updateRegisters(LOOP[0].isEEA,0);

		/// pipe specific vars
		for (int pipe = 0; pipe < 3; pipe++)
		{
			PIPE[pipe].tempStability = 0;
   			PIPE[pipe].freqStability = 0;
   			PIPE[pipe].etimer->restart();
   			PIPE[pipe].mainDirPath = m_mainServer+LOOP[0].mode+PIPE[pipe].slave->text(); 

			if (LOOP[0].mode == LOW) prepareForNextFile(pipe, QString("AMB").append("_").append(QString::number(LOOP[0].minRefTemp)).append(LOOP[0].filExt));

			if (ui->radioButton_7->isChecked()) PIPE[pipe].osc = 1;
   			else if (ui->radioButton_8->isChecked()) PIPE[pipe].osc = 2;
   			else if (ui->radioButton_9->isChecked()) PIPE[pipe].osc = 3;
   			else PIPE[pipe].osc = 4;
		}

    	/// validate 0 connect
    	if (LOOP[0].modbus == NULL)
    	{
        	/// calibration fails
        	LOOP[0].isCal = false;

        	/// update tab icon
        	updateStartButtonLabel(0);

        	displayMessage(QString("LOOP "),QString("LOOP "),"Bad Serial Connection");

        	/// exit
        	return LOOP[0].isCal;
    	}

		/// validate slave ids
    	if (!validateSerialNumber(0, LOOP[0].serialModbus))
		{
			/// reset LOOP[0].isCal
			LOOP[0].isCal = false;

        	/// update tab icon
        	updateStartButtonLabel(0);

        	/// exit
        	return LOOP[0].isCal;
		}
	}

	return true;
}


void
MainWindow::
onCalibrationButtonPressed(int loop)
{
    /// scan calibration variables
    if (!prepareCalibration(0)) return;
	
    /// LOOP calibration on
    if (LOOP[0].isCal)
    {
		for (int pipe = 0; pipe < 3; pipe++)
		{
			if (PIPE[pipe].isCal)
			{
      			QDir dir;
       			int fileCounter = 2;
  
				/// update run indicator
   				PIPE[pipe].checkBox->setChecked(PIPE[pipe].isCal);
	
    			/// start calibration
    			updateStability(F_BAR, pipe, 0);
    			updateStability(T_BAR, pipe, 0);

       			/// create file directory "g:/FULLCUT/FC" + "8756"
       			if (!dir.exists(PIPE[pipe].mainDirPath)) dir.mkpath(PIPE[pipe].mainDirPath);
       			else
       			{
           			while (1)
           			{
               			if (!dir.exists(PIPE[pipe].mainDirPath+"_"+QString::number(fileCounter))) 
               			{
                   			PIPE[pipe].mainDirPath += "_"+QString::number(fileCounter);
                   			dir.mkpath(PIPE[pipe].mainDirPath);

							/// update first file path
							if (LOOP[0].mode == LOW) prepareForNextFile(pipe, QString("AMB").append("_").append(QString::number(LOOP[0].minRefTemp)).append(LOOP[0].filExt));

                   			break;
               			}
               			else fileCounter++;
           			}
       			}

				if (pipe == 0) 
				{
					future_P1 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_P1.waitForFinished();
				}
				else if (pipe == 1) 
				{
					future_P2 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_P2.waitForFinished();
				}
				else if (pipe == 2) 
				{
					future_P3 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_P3.waitForFinished();
				}
			}
    	}
	}
	else
    {
		/// LOOP[0].isCal == FALSE
		for (int i=0; i < 3; i++) stopCalibration(i);
	}

	/// update pipe indicator 
    updateStartButtonLabel(0);
}		


void
MainWindow::
stopCalibration(int pipe)
{
	if (pipe == P1) 
	{
		if (future_P1.isStarted()) future_P1.cancel();
	}
	else if (pipe == P2) 
	{
		if (future_P2.isStarted()) future_P2.cancel();
	}
	else if (pipe == P3) 
	{
		if (future_P3.isStarted()) future_P3.cancel();
	}
}

void
MainWindow::
readData(const int pipe, const bool isStability)
{
    uint8_t dest[1024];
    uint16_t * dest16 = (uint16_t *) dest;
    int ret = -1;
    bool is16Bit = false;
    bool writeAccess = false;
    const QString funcType = descriptiveDataTypeName( FUNC_READ_FLOAT );

    /// reset connection
    memset( dest, 0, 1024 );
    modbus_set_slave( LOOP[0].serialModbus, PIPE[pipe].slave->text().toInt() );
 
    /// get temperature
    PIPE[pipe].temperature = sendCalibrationRequest(FLOAT_R, LOOP[0].serialModbus, FUNC_READ_FLOAT, LOOP[0].ID_TEMPERATURE, BYTE_READ_FLOAT, ret, dest, dest16, is16Bit, writeAccess, funcType);

	if (isStability)
	{
		/// check temp stability
    	if (PIPE[pipe].tempStability < 5) 
		{
			if (abs(PIPE[pipe].temperature - PIPE[pipe].temperature_prev) <= LOOP[0].zTemp) PIPE[pipe].tempStability++;
			else PIPE[pipe].tempStability = 0;

			PIPE[pipe].temperature_prev = PIPE[pipe].temperature;
		}
		else PIPE[pipe].tempStability = 5;

    	updateStability(T_BAR, pipe, PIPE[pipe].tempStability*20);
	}
	else
	{
	    PIPE[pipe].freqStability = 0;
   		PIPE[pipe].tempStability = 0;

		updateStability(F_BAR, pipe, 0);
		updateStability(T_BAR, pipe, 0);
	}

    QThread::msleep(SLEEP_TIME);

    /// get frequency
    PIPE[pipe].frequency = sendCalibrationRequest(FLOAT_R, LOOP[0].serialModbus, FUNC_READ_FLOAT, LOOP[0].ID_FREQ, BYTE_READ_FLOAT, ret, dest, dest16, is16Bit, writeAccess, funcType);

	if (isStability)
	{
		/// check freq stability
		if (PIPE[pipe].freqStability < 5) 
		{
       		if (abs(PIPE[pipe].frequency - PIPE[pipe].frequency_prev) <= LOOP[0].yFreq) PIPE[pipe].freqStability++;
			else PIPE[pipe].freqStability = 0;

       		PIPE[pipe].frequency_prev = PIPE[pipe].frequency;
		}
		else PIPE[pipe].freqStability = 5;

    	updateStability(F_BAR, pipe, PIPE[pipe].freqStability*20);
	}
	else
	{
	    PIPE[pipe].freqStability = 0;
   		PIPE[pipe].tempStability = 0;

		updateStability(F_BAR, pipe, 0);
		updateStability(T_BAR, pipe, 0);
	}

    QThread::msleep(SLEEP_TIME);

    /// get oil_rp 
    PIPE[pipe].oilrp = sendCalibrationRequest(FLOAT_R, LOOP[0].serialModbus, FUNC_READ_FLOAT, LOOP[0].ID_OIL_RP, BYTE_READ_FLOAT, ret, dest, dest16, is16Bit, writeAccess, funcType);
    QThread::msleep(SLEEP_TIME);

    /// get measured ai
    PIPE[pipe].measai = sendCalibrationRequest(FLOAT_R, LOOP[0].serialModbus, FUNC_READ_FLOAT, RAZ_MEAS_AI, BYTE_READ_FLOAT, ret, dest, dest16, is16Bit, writeAccess, funcType);
    QThread::msleep(SLEEP_TIME);

    /// get trimmed ai
    PIPE[pipe].trimai = sendCalibrationRequest(FLOAT_R, LOOP[0].serialModbus, FUNC_READ_FLOAT, RAZ_TRIM_AI, BYTE_READ_FLOAT, ret, dest, dest16, is16Bit, writeAccess, funcType);
    QThread::msleep(SLEEP_TIME);

    /// update pipe reading
	updatePipeReading(pipe, PIPE[pipe].watercut, PIPE[pipe].frequency_start, PIPE[pipe].frequency, PIPE[pipe].temperature, PIPE[pipe].oilrp);
}


void
MainWindow::
calibrate(const int pipe)
{
	static double injectionTime = 0;
	static double totalInjectionTime = 0;
	static double accumulatedInjectionTime_prev = 0;
    QString data_stream;
 
    /// get elapsed time
    int elapsedTime = PIPE[pipe].etimer->elapsed()/1000;

    /// update pipe tab icon and control variables
    updateStartButtonLabel(0); 

    /// calibration enabled? 
    if (LOOP[0].isCal)
    {
		///////////////////////////
		///////////////////////////
        /// LOWCUT
		///////////////////////////
		///////////////////////////
		
        if (LOOP[0].mode == LOW)
        {
			/// AMB_"LOOP[0].minRefTemp".LCT
            if (QFileInfo(PIPE[pipe].file).fileName() == QString("AMB").append("_").append(QString::number(LOOP[0].minRefTemp)).append(LOOP[0].filExt))
            {
			    /// validate run condition 
                if (PIPE[pipe].checkBox->isChecked() && ((PIPE[pipe].tempStability != 5) || (PIPE[pipe].freqStability != 5)))
                {
					/// read data
					readData(pipe, STABILITY_CHECK);
					PIPE[pipe].watercut = LOOP[0].waterRunStop->text().toDouble();
					data_stream = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14").arg(elapsedTime, 9, 'g', -1, ' ').arg(PIPE[pipe].watercut,7,'f',2,' ').arg(PIPE[pipe].osc, 4, 'g', -1, ' ').arg(" INT").arg(1, 7, 'g', -1, ' ').arg(PIPE[pipe].frequency,9,'f',3,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].oilrp,9,'f',2,' ').arg(PIPE[pipe].temperature,11,'f',2,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].measai,12,'f',2,' ').arg(PIPE[pipe].trimai,12,'f',2,' ').arg(injectionTime,10,'f',2,' ').arg(0,12,'f',2,' ');

                    /// create a new file if needed
                    if (!QFileInfo(PIPE[pipe].file).exists()) 
                    {
						/// reset injectionTime
						injectionTime = 0;
						totalInjectionTime = 0;
						accumulatedInjectionTime_prev = 0;

                        /// indicate the operator to enter measured initial watercut 
                        bool ok;
                        QString msg = PIPE[pipe].slave->text().append(": Enter Measured Initial Watercut");
                        LOOP[0].waterRunStop->setText(QInputDialog::getText(this, QString("LOOP ")+QString::number(0 + 1)+QString(", PIPE ")+QString::number(pipe%3+1),tr(qPrintable(msg)), QLineEdit::Normal,"0.0", &ok));

						/// update data_stream with the user entered initial watercut
						data_stream = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14").arg(elapsedTime, 9, 'g', -1, ' ').arg(LOOP[0].waterRunStop->text().toDouble(),7,'f',2,' ').arg(PIPE[pipe].osc, 4, 'g', -1, ' ').arg(" INT").arg(1, 7, 'g', -1, ' ').arg(PIPE[pipe].frequency,9,'f',3,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].oilrp,9,'f',2,' ').arg(PIPE[pipe].temperature,11,'f',2,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].measai,12,'f',2,' ').arg(PIPE[pipe].trimai,12,'f',2,' ').arg(injectionTime,10,'f',2,' ').arg(0,12,'f',2,' ');

                        /// indicate the operator to set temp to LOOP[0].minRefTemp C degrees
                        if (getUserInputMessage(QString("LOOP ")+QString::number(0 + 1)+QString(", PIPE ")+QString::number(pipe%3+1), PIPE[pipe].slave->text().append(": Set The Heat Exchanger Temperature"), QString::number(LOOP[0].minRefTemp).append("°C"))) createLoopFile(PIPE[pipe].slave->text().toInt(), LOOP[0].mode, "AMB", QString::number(LOOP[0].minRefTemp), LOOP[0].saltStop->currentText(), pipe);
                        else 
						{
							stopCalibration(pipe);
							return;
						}

                        /// indicate the operator to fill the water container to the mark 
                		displayMessage(QString("LOOP ")+QString::number(0 + 1)+QString(", PIPE ")+QString::number(pipe%3+1),QString("Serial Number: ")+PIPE[pipe].slave->text()+QString("                              "),"Fill the water container to the mark");
                    }

                    writeToCalFile(pipe, data_stream);
                }
                else 
                {
                    prepareForNextFile(pipe, QString::number(LOOP[0].minRefTemp).append("_").append(QString::number(LOOP[0].maxRefTemp)).append(LOOP[0].filExt));
                }
            }
            else if (QFileInfo(PIPE[pipe].file).fileName() == QString::number(LOOP[0].minRefTemp).append("_").append(QString::number(LOOP[0].maxRefTemp)).append(LOOP[0].filExt))
            {
                if (PIPE[pipe].checkBox->isChecked() && ((PIPE[pipe].tempStability != 5) || (PIPE[pipe].freqStability != 5)))
                {
					/// read data
					readData(pipe, STABILITY_CHECK);
					PIPE[pipe].watercut = LOOP[0].waterRunStop->text().toDouble();
					data_stream = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14").arg(elapsedTime, 9, 'g', -1, ' ').arg(PIPE[pipe].watercut,7,'f',2,' ').arg(PIPE[pipe].osc, 4, 'g', -1, ' ').arg(" INT").arg(1, 7, 'g', -1, ' ').arg(PIPE[pipe].frequency,9,'f',3,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].oilrp,9,'f',2,' ').arg(PIPE[pipe].temperature,11,'f',2,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].measai,12,'f',2,' ').arg(PIPE[pipe].trimai,12,'f',2,' ').arg(injectionTime,10,'f',2,' ').arg(0,12,'f',2,' ');

                    if (!QFileInfo(PIPE[pipe].file).exists()) 
                    {
                        if (getUserInputMessage(QString("LOOP ")+QString::number(0 + 1)+QString(", PIPE ")+QString::number(pipe%3+1), PIPE[pipe].slave->text().append(": Set Heat Exchanger Temperature"),QString::number(LOOP[0].maxRefTemp).append("°C"))) createLoopFile(PIPE[pipe].slave->text().toInt(), LOOP[0].mode, QString::number(LOOP[0].minRefTemp), QString::number(LOOP[0].maxRefTemp), LOOP[0].saltStop->currentText(), pipe);
                        else 
						{
							stopCalibration(pipe);
							return;
						}
                    }

                    writeToCalFile(pipe, data_stream);
                }
                else 
                {
                    prepareForNextFile(pipe, QString::number(LOOP[0].maxRefTemp).append("_").append(QString::number(LOOP[0].injectionTemp)).append(LOOP[0].filExt));
					readData(pipe, STABILITY_CHECK);
					PIPE[pipe].frequency_start = PIPE[pipe].frequency;
                }
            }
            else if (QFileInfo(PIPE[pipe].file).fileName() == QString::number(LOOP[0].maxRefTemp).append("_").append(QString::number(LOOP[0].injectionTemp)).append(LOOP[0].filExt))
            {
				/// read data
				readData(pipe, STABILITY_CHECK);
				PIPE[pipe].watercut = LOOP[0].waterRunStop->text().toDouble();
				data_stream = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14").arg(elapsedTime, 9, 'g', -1, ' ').arg(PIPE[pipe].watercut,7,'f',2,' ').arg(PIPE[pipe].osc, 4, 'g', -1, ' ').arg(" INT").arg(1, 7, 'g', -1, ' ').arg(PIPE[pipe].frequency,9,'f',3,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].oilrp,9,'f',2,' ').arg(PIPE[pipe].temperature,11,'f',2,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].measai,12,'f',2,' ').arg(PIPE[pipe].trimai,12,'f',2,' ').arg(injectionTime,10,'f',2,' ').arg(0,12,'f',2,' ');

                if (PIPE[pipe].checkBox->isChecked() && ((PIPE[pipe].tempStability != 5) || (PIPE[pipe].freqStability != 5)))
                {
                    if (!QFileInfo(PIPE[pipe].file).exists()) 
                    {
                        if (getUserInputMessage(QString("LOOP ")+QString::number(0 + 1)+QString(", PIPE ")+QString::number(pipe%3+1), PIPE[pipe].slave->text().append(": Set Heat Exchanger Temperature"),QString::number(LOOP[0].injectionTemp).append("°C"))) createLoopFile(PIPE[pipe].slave->text().toInt(), LOOP[0].mode, QString::number(LOOP[0].maxRefTemp), QString::number(LOOP[0].injectionTemp), LOOP[0].saltStop->currentText(), pipe);
                        else 
						{
							stopCalibration(pipe);
							return;
						}
                    }
            
                    writeToCalFile(pipe, data_stream);
                }
                else 
                {
                    prepareForNextFile(pipe,"CALIBRAT.LCI");
                }
            }

			///////////////////////////////////
			///////////////////////////////////
			/// start real calibration here
			///////////////////////////////////
			///////////////////////////////////
			
            else if (QFileInfo(PIPE[pipe].file).fileName() == "CALIBRAT.LCI") // CALIBRAT.LCI 
            {   
				/// read data
				readData(pipe, NO_STABILITY_CHECK);
				data_stream = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14").arg(elapsedTime, 9, 'g', -1, ' ').arg(PIPE[pipe].watercut,7,'f',2,' ').arg(PIPE[pipe].osc, 4, 'g', -1, ' ').arg(" INT").arg(1, 7, 'g', -1, ' ').arg(PIPE[pipe].frequency,9,'f',3,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].oilrp,9,'f',2,' ').arg(PIPE[pipe].temperature,11,'f',2,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].measai,12,'f',2,' ').arg(PIPE[pipe].trimai,12,'f',2,' ').arg(injectionTime,10,'f',4,' ').arg(0,12,'f',2,' ');

				/// waterRunStart is actually the MAX watercut value in LOWCUT mode 
                if (PIPE[pipe].checkBox->isChecked() && (LOOP[0].waterRunStart->text().toDouble() >= PIPE[pipe].watercut))
                {
                    /// create a new file if needed
        			if (!QFileInfo(PIPE[pipe].file).exists()) 
                    {
                        bool ok;
                        QString msg = PIPE[pipe].slave->text().append(": Enter Measured Initial Watercut");
                        LOOP[0].waterRunStop->setText(QInputDialog::getText(this, QString("LOOP ")+QString::number(0 + 1)+QString(", PIPE ")+QString::number(pipe%3+1),tr(qPrintable(msg)), QLineEdit::Normal,"0.0", &ok));

						/// re-read data
						data_stream = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14").arg(elapsedTime, 9, 'g', -1, ' ').arg(LOOP[0].waterRunStop->text().toDouble(),7,'f',2,' ').arg(PIPE[pipe].osc, 4, 'g', -1, ' ').arg(" INT").arg(1, 7, 'g', -1, ' ').arg(PIPE[pipe].frequency,9,'f',3,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].oilrp,9,'f',2,' ').arg(PIPE[pipe].temperature,11,'f',2,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].measai,12,'f',2,' ').arg(PIPE[pipe].trimai,12,'f',2,' ').arg(0,10,'f',2,' ').arg(0,12,'f',2,' ');

						createCalibrateFile(PIPE[pipe].slave->text().toInt(), pipe, LOOP[0].waterRunStop->text(), LOOP[0].waterRunStart->text(), 0, "CALIBRAT");
				
						/// set next watercut increased by 0.25	
						PIPE[pipe].watercut = LOOP[0].intervalCalibration; // set initial watercut
	
						updatePipeReading(pipe, PIPE[pipe].watercut, PIPE[pipe].frequency, PIPE[pipe].frequency, PIPE[pipe].temperature, PIPE[pipe].oilrp);
                    }

                    writeToCalFile(pipe, data_stream);

					/// next injection time and update totalInjectionTime
					double accumulatedInjectionTime = -(LOOP[0].loopVolume->text().toDouble()/(LOOP[0].injectionWaterPumpRate/60))*log((1-(PIPE[pipe].watercut - LOOP[0].waterRunStop->text().toDouble())/100));
					injectionTime = accumulatedInjectionTime - accumulatedInjectionTime_prev;
					totalInjectionTime += injectionTime;
					accumulatedInjectionTime_prev = accumulatedInjectionTime;
					
					/// inject water to the pipe for "injectionTime" seconds
					// TODO - IMPLEMENT injectWater()
				 	injectWater(true);					 // start injecting water
        			QThread::msleep(injectionTime*1000); // wait for "injectionTime" secs
					injectWater(false);					 // stop injecting water

					/// update calibration status
					updateCalibrationLcd(PIPE[pipe].watercut, 0, injectionTime, injectionTime*LOOP[0].injectionWaterPumpRate/60);

					/// set next watercut
					PIPE[pipe].watercut += LOOP[0].intervalCalibration;
                }
                else 
                {
					bool ok;
					QString msg;

                    /// enter measured watercut
                    msg = PIPE[pipe].slave->text().append(": Enter Measured Watercut [%]");
                    double measuredWatercut = QInputDialog::getDouble(this, QString("LOOP ")+QString::number(0 + 1)+QString(", PIPE ")+QString::number(pipe%3+1),tr(qPrintable(msg)), 0.0, 0, 100, 2, &ok,Qt::WindowFlags(), 1);

                    /// ask user total injected volume 
					msg = PIPE[pipe].slave->text().append(": Enter Injected Volume [ML]");
	                double injectedVolume = QInputDialog::getDouble(this, QString("LOOP ")+QString::number(0 + 1)+QString(", PIPE ")+QString::number(pipe%3+1),tr(qPrintable(msg)), 0.0, 0, 100000, 3, &ok,Qt::WindowFlags(), 1);

					/// ADJUSTED.LCI
				 	if (abs(injectedVolume - (LOOP[0].injectionWaterPumpRate/60)*totalInjectionTime) > 0) 
					{
						//QFile::copy(PIPE[pipe].fileCalibrate.fileName(),PIPE[pipe].fileAdjusted.fileName());
						if (PIPE[pipe].fileCalibrate.open(QIODevice::ReadOnly))
						{
							int i = 0;
							PIPE[pipe].fileAdjusted.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
   							QTextStream in(&PIPE[pipe].fileCalibrate);
   							QTextStream out(&PIPE[pipe].fileAdjusted);
   							while (!in.atEnd())
   							{
      							QString line = in.readLine();
								if (i > 6) 
								{
									/// process string and correct watercut
									QStringList data = line.split(" ");
									QString newData[data.size()];
									int k = 0;
									int x = 0;
									double correctedWatercut;

									for (int j=0; j<data.size(); j++)
									{
										if (data[j] != "") 
										{	
											newData[k] = data[j];
											if (k==1) x = j; /// saving index for watercut
											if (k==12) 
											{
												correctedWatercut = (LOOP[0].waterRunStop->text().toDouble() + 100) - (100*exp(-(LOOP[0].injectionWaterPumpRate/60)*QString(newData[k]).toDouble()/LOOP[0].loopVolume->text().toDouble()));
												data[x] = QString("%1").arg(correctedWatercut,7,'f',2,' ');
											}
											k++;
										}
									}

									/// re-create a corrected data_stream
									line = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14").arg(newData[0].toDouble(), 9, 'g', -1, ' ').arg(correctedWatercut,7,'f',2,' ').arg(newData[2].toDouble(), 4, 'g', -1, ' ').arg(" INT").arg(1, 7, 'g', -1, ' ').arg(newData[5].toDouble(),9,'f',3,' ').arg(0,8,'f',2,' ').arg(newData[7].toDouble(),9,'f',2,' ').arg(newData[8].toDouble(),11,'f',2,' ').arg(0,8,'f',2,' ').arg(newData[10].toDouble(),12,'f',2,' ').arg(newData[11].toDouble(),12,'f',2,' ').arg(0,10,'f',2,' ').arg(0,12,'f',2,' ');
								}

								out << line << '\n';
								i++;
   							}

   							PIPE[pipe].fileCalibrate.close();
   							PIPE[pipe].fileAdjusted.close();
						}
					}

                    /// finalize and close
    				QDateTime currentDataTime = QDateTime::currentDateTime();
    				QString data_stream   = QString("Total injection time   = %1 s").arg(totalInjectionTime, 10, 'g', -1, ' ');
    				QString data_stream_2 = QString("Total injection volume = %1 mL").arg(injectedVolume, 10, 'g', -1, ' ');
    				QString data_stream_3 = QString("Loop volume            = %1 mL").arg(LOOP[0].loopVolume->text().toDouble(), 10, 'g', -1, ' ');
    				QString data_stream_4 = QString("Measured watercut      = %1 %").arg(measuredWatercut, 10, 'f', 2, ' ');
    				QString data_stream_5 = QString("[%1]").arg(currentDataTime.toString());

					if (QFileInfo(PIPE[pipe].fileCalibrate).exists())
					{
						QTextStream stream(&PIPE[pipe].fileCalibrate);
    					PIPE[pipe].fileCalibrate.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    					stream << '\n' << '\n' << data_stream << '\n' << data_stream_2 << '\n' << data_stream_3 << '\n' << data_stream_4 << '\n' << data_stream_5 << '\n';
    					PIPE[pipe].fileCalibrate.close();
					}

					if (QFileInfo(PIPE[pipe].fileAdjusted).exists())
					{
						QTextStream stream(&PIPE[pipe].fileAdjusted);
    					PIPE[pipe].fileAdjusted.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    					stream << '\n' << '\n' << data_stream << '\n' << data_stream_2 << '\n' << data_stream_3 << '\n' << data_stream_4 << '\n' << data_stream_5 << '\n';
    					PIPE[pipe].fileAdjusted.close();
					}

                    /// stop calibration
                    LOOP[0].isCal = false;
                    prepareForNextFile(pipe, NO_FILE);
                    updateStartButtonLabel(0);
					stopCalibration(pipe);
                	PIPE[pipe].checkBox->setChecked(false);
                	displayMessage(QString("LOOP ")+QString::number(0 + 1)+QString(", PIPE ")+QString::number(pipe%3+1),QString("Serial Number: ")+PIPE[pipe].slave->text()+QString("                                    "),"Calibration Finished Successfully!");
					return;
                }
            }
			
            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////
            /// restart singleshot timer. otherwise, calibration will be ended
            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////
            
            if (LOOP[0].isCal && PIPE[pipe].checkBox->isChecked()) QTimer::singleShot(LOOP[0].xDelay*1000, [this, pipe] () {calibrate(pipe);}); /// xDelay * 1000 = xDelay seconds
        }
    }
} 

void
MainWindow::
injectWater(const bool isInjectWater)
{
	if (isInjectWater) return;
	else return;

	/// slave id: 100
	/// oil pump coil : 60
	/// water pump coil : 61
}


void
MainWindow::
prepareForNextFile(const int pipe, const QString nextFileId)
{
    PIPE[pipe].file.setFileName(PIPE[pipe].mainDirPath+"\\"+nextFileId);
    PIPE[pipe].freqStability = 0;
    PIPE[pipe].tempStability = 0;

    updateStability(F_BAR, pipe, 0);
    updateStability(T_BAR, pipe, 0);
}


void
MainWindow::
writeToCalFile(int pipe, QString data_stream)
{
    /// write to file
    QTextStream stream(&PIPE[pipe].file);
    PIPE[pipe].file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    stream << data_stream << '\n' ;
    PIPE[pipe].file.close();
}


void
MainWindow::
onFunctionCodeChanges()
{
    if (ui->radioButton_181->isChecked()) // float
    {
        if (ui->radioButton_187->isChecked()) // read
        {
            ui->functionCode->setCurrentIndex(3);
        }
        else
        {
            ui->functionCode->setCurrentIndex(7);
        }
    }
    else if (ui->radioButton_182->isChecked()) // integer
    {
        if (ui->radioButton_187->isChecked()) // read
        {
            ui->functionCode->setCurrentIndex(3);
        }
        else
        {
            ui->functionCode->setCurrentIndex(5);
        }
    }
    else // coil
    {
        if (ui->radioButton_187->isChecked()) // read
        {
            ui->functionCode->setCurrentIndex(0);
        }
        else
        {
            ui->functionCode->setCurrentIndex(4);
        }
    }
}


void
MainWindow::
onFloatButtonPressed(bool enabled)
{
    if (enabled) ui->numCoils->setValue(2); // quantity
    if (ui->radioButton_187->isChecked())
    {
        onReadButtonPressed(true);
    }
    else
    {
        onWriteButtonPressed(true);
    }

    ui->groupBox_103->setEnabled(TRUE);
    ui->groupBox_106->setEnabled(FALSE);
    ui->groupBox_107->setEnabled(FALSE);
    ui->lineEdit_111->clear();
}

void
MainWindow::
onIntegerButtonPressed(bool enabled)
{
    if (enabled) ui->numCoils->setValue(1);
    if (ui->radioButton_187->isChecked())
    {
        onReadButtonPressed(true);
    }
    else
    {
        onWriteButtonPressed(true);
    }
    ui->groupBox_103->setEnabled(FALSE);
    ui->groupBox_106->setEnabled(TRUE);
    ui->groupBox_107->setEnabled(FALSE);
    ui->lineEdit_109->clear();
 }

void
MainWindow::
onCoilButtonPressed(bool enabled)
{
    if (enabled) ui->numCoils->setValue(1);
    if (ui->radioButton_187->isChecked())
    {
        onReadButtonPressed(true);
    }
    else
    {
        onWriteButtonPressed(true);
    }

    ui->groupBox_103->setEnabled(FALSE);
    ui->groupBox_106->setEnabled(FALSE);
    ui->groupBox_107->setEnabled(TRUE);
    ui->lineEdit_109->clear();
    ui->lineEdit_111->clear();
}

void
MainWindow::
onReadButtonPressed(bool enabled)
{
    if (enabled)
    {
        ui->lineEdit_109->setReadOnly(true);
        ui->lineEdit_111->setReadOnly(true);
        ui->groupBox_107->setEnabled(true);
    }

    onFunctionCodeChanges();
}

void
MainWindow::
onWriteButtonPressed(bool enabled)
{
    if (enabled)
    {
        if (ui->radioButton_181->isChecked())
            ui->lineEdit_109->setReadOnly(false);
        else if (ui->radioButton_182->isChecked())
            ui->lineEdit_111->setReadOnly(false);
        else
            ui->groupBox_107->setEnabled(true);
    }

    onFunctionCodeChanges();
}

float 
MainWindow::
toFloat(QByteArray f)
{
    bool ok;
    int sign = 1;

    f = f.toHex(); // Convert to Hex

    f = QByteArray::number(f.toLongLong(&ok, 16), 2);    // Convert hex to binary

    if(f.length() == 32) {
        if(f.at(0) == '1') sign =-1;     // If bit 0 is 1 number is negative
        f.remove(0,1);                   // Remove sign bit
    }

    QByteArray fraction = f.right(23);  // Get the fractional part
    double mantissa = 0;
    for(int i = 0; i < fraction.length(); i++){  // Iterate through the array to claculate the fraction as a decimal.
        if(fraction.at(i) == '1')
            mantissa += 1.0 / (pow(2, i+1));
    }

    int exponent = f.left(f.length() - 23).toLongLong(&ok, 2) - 127;     // Calculate the exponent

    return (sign * pow(2, exponent) * (mantissa + 1.0));
}


void
MainWindow::
updateRegisters(const bool isEEA, const int loop)
{
    if (isEEA)
    {
        LOOP[0].ID_SN_PIPE = 201;
        LOOP[0].ID_WATERCUT = 3; // register id
        LOOP[0].ID_TEMPERATURE = 5;
        LOOP[0].ID_SALINITY = 9;
        LOOP[0].ID_OIL_ADJUST = 15;
        LOOP[0].ID_WATER_ADJUST = 17;
        LOOP[0].ID_FREQ = 19;
        LOOP[0].ID_OIL_RP = 61;

        ui->groupBox_6->setEnabled(true);
    }
    else
    {
        LOOP[0].ID_SN_PIPE = 201;
        LOOP[0].ID_WATERCUT = 3; // register id
        LOOP[0].ID_TEMPERATURE = 5;
        LOOP[0].ID_SALINITY = 9;
        LOOP[0].ID_OIL_ADJUST = 15;
        LOOP[0].ID_WATER_ADJUST = 17;
        LOOP[0].ID_FREQ = 19;
        LOOP[0].ID_OIL_RP = 61;

        ui->radioButton_5->setChecked(true);
        ui->groupBox_6->setEnabled(false);
    }
}


void
MainWindow::
setInputValidator(void)
{
    /// serial number 
    ui->lineEdit_2->setValidator(serialNumberValidator);
    ui->lineEdit_7->setValidator(serialNumberValidator);
    ui->lineEdit_13->setValidator(serialNumberValidator);
}


void
MainWindow::
updatePipeReading(const int pipe, const double watercut, const double startfreq, const double freq, const double temp, const double rp)
{
    PIPE[pipe].lcdWatercut->display(watercut);
    PIPE[pipe].lcdStartFreq->display(startfreq);
    PIPE[pipe].lcdFreq->display(freq);
    PIPE[pipe].lcdTemp->display(temp);
    PIPE[pipe].lcdReflectedPower->display(rp);
} 


void
MainWindow::
updateCalibrationLcd(const double watercut, const double salinity, const double injectionTime, const double injectionVol)
{
	ui->lcdNumber_26->display(watercut);
	ui->lcdNumber_7->display(salinity);
	ui->lcdNumber_6->display(injectionTime);
	ui->lcdNumber_5->display(injectionVol);
}
