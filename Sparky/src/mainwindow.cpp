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

    /// initialize LOOPS and PIPES
	//LOOP = new LOOPS[MAX_LOOP];
	//PIPE = new PIPES[MAX_PIPE];

    readJsonConfigFile();
    updateRegisters(EEA,L1); // L1-EEA
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
    else if (ui->tabWidget_2->currentIndex() == 1) m_modbus_snipping = LOOP[1].modbus;
    else if (ui->tabWidget_2->currentIndex() == 2) m_modbus_snipping = LOOP[2].modbus;
    else if (ui->tabWidget_2->currentIndex() == 3) m_modbus_snipping = LOOP[3].modbus; 
    else if (ui->tabWidget_2->currentIndex() == 4) m_modbus_snipping = LOOP[4].modbus;
    else                                           m_modbus_snipping = LOOP[5].modbus; 

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
        if (loop == L1) LOOP[L1].modbus = this->modbus();
        else if (loop == L2) LOOP[L2].modbus = this->modbus_2();
        else if (loop == L3) LOOP[L3].modbus = this->modbus_3();
        else if (loop == L4) LOOP[L4].modbus = this->modbus_4();
        else if (loop == L5) LOOP[L5].modbus = this->modbus_5();
        else if (loop == L6) LOOP[L6].modbus = this->modbus_6();

        //LOOP[loop].modbus = this->modbus();
		if (LOOP[loop].modbus) {
			modbus_register_monitor_add_item_fnc(LOOP[loop].modbus, MainWindow::stBusMonitorAddItem);
			modbus_register_monitor_raw_data_fnc(LOOP[loop].modbus, MainWindow::stBusMonitorRawData);
		}
	}
	else LOOP[loop].modbus = NULL;
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
		/*
    /// dynamic memory allocation 
    for (int j = 0; j < MAX_PIPE; j++)
    {
        PIPE[j].series = new QSplineSeries;
        PIPE[j].etimer = new QElapsedTimer;
    }
*/
	/// label
    PIPE[0].pipeId = "L1P1";
    PIPE[1].pipeId = "L1P2";
    PIPE[2].pipeId = "L1P3";
    PIPE[3].pipeId = "L2P1";
    PIPE[4].pipeId = "L2P2";
    PIPE[5].pipeId = "L2P3";
    PIPE[6].pipeId = "L3P1";
    PIPE[7].pipeId = "L3P2";
    PIPE[8].pipeId = "L3P3";
    PIPE[9].pipeId = "L4P1";
    PIPE[10].pipeId = "L4P2";
    PIPE[11].pipeId = "L4P3";
    PIPE[12].pipeId = "L5P1";
    PIPE[13].pipeId = "L5P2";
    PIPE[14].pipeId = "L5P3";
    PIPE[15].pipeId = "L6P1";
    PIPE[16].pipeId = "L6P2";
    PIPE[17].pipeId = "L6P3";

	/// slave
	PIPE[0].slave = ui->lineEdit_2;
	PIPE[1].slave = ui->lineEdit_7;
	PIPE[2].slave = ui->lineEdit_13;
	PIPE[3].slave = ui->lineEdit_3;
    PIPE[4].slave = ui->lineEdit_8;
    PIPE[5].slave = ui->lineEdit_14;
    PIPE[6].slave = ui->lineEdit_5;
    PIPE[7].slave = ui->lineEdit_9;
    PIPE[8].slave = ui->lineEdit_15;
    PIPE[9].slave = ui->lineEdit_10;
    PIPE[10].slave = ui->lineEdit_11;
    PIPE[11].slave = ui->lineEdit_16;
    PIPE[12].slave = ui->lineEdit_17;
    PIPE[13].slave = ui->lineEdit_18;
    PIPE[14].slave = ui->lineEdit_19;
    PIPE[15].slave = ui->lineEdit_21;
    PIPE[16].slave = ui->lineEdit_22;
    PIPE[17].slave = ui->lineEdit_23;

    /// on/off graph line view
    PIPE[0].lineView = ui->checkBox_19;
    PIPE[1].lineView = ui->checkBox_20;
    PIPE[2].lineView = ui->checkBox_21;
    PIPE[3].lineView = ui->checkBox_22;
    PIPE[4].lineView = ui->checkBox_23;
    PIPE[5].lineView = ui->checkBox_24;
    PIPE[6].lineView = ui->checkBox_25;
    PIPE[7].lineView = ui->checkBox_26;
    PIPE[8].lineView = ui->checkBox_27;
    PIPE[9].lineView = ui->checkBox_28;
    PIPE[10].lineView = ui->checkBox_29;
    PIPE[11].lineView = ui->checkBox_30;
    PIPE[12].lineView = ui->checkBox_31;
    PIPE[13].lineView = ui->checkBox_32;
    PIPE[14].lineView = ui->checkBox_33;
    PIPE[15].lineView = ui->checkBox_34;
    PIPE[16].lineView = ui->checkBox_35;
    PIPE[17].lineView = ui->checkBox_36;

    /// on/off pipe switch
    PIPE[0].checkBox = ui->checkBox;
    PIPE[1].checkBox = ui->checkBox_2;
    PIPE[2].checkBox = ui->checkBox_3;
    PIPE[3].checkBox = ui->checkBox_4;
    PIPE[4].checkBox = ui->checkBox_5;
    PIPE[5].checkBox = ui->checkBox_6;
    PIPE[6].checkBox = ui->checkBox_7;
    PIPE[7].checkBox = ui->checkBox_8;
    PIPE[8].checkBox = ui->checkBox_9;
    PIPE[9].checkBox = ui->checkBox_10;
    PIPE[10].checkBox = ui->checkBox_11;
    PIPE[11].checkBox = ui->checkBox_12;
    PIPE[12].checkBox = ui->checkBox_13;
    PIPE[13].checkBox = ui->checkBox_14;
    PIPE[14].checkBox = ui->checkBox_15;
    PIPE[15].checkBox = ui->checkBox_16;
    PIPE[16].checkBox = ui->checkBox_17;
    PIPE[17].checkBox = ui->checkBox_18;

    /// lcdWatercut
    PIPE[0].lcdWatercut = ui->lcdNumber_4;
    PIPE[1].lcdWatercut = ui->lcdNumber_53;
    PIPE[2].lcdWatercut = ui->lcdNumber_58;
    PIPE[3].lcdWatercut = ui->lcdNumber_34;
    PIPE[4].lcdWatercut = ui->lcdNumber_67;
    PIPE[5].lcdWatercut = ui->lcdNumber_72;
    PIPE[6].lcdWatercut = ui->lcdNumber_42;
    PIPE[7].lcdWatercut = ui->lcdNumber_81;
    PIPE[8].lcdWatercut = ui->lcdNumber_86;
    PIPE[9].lcdWatercut = ui->lcdNumber_92;
    PIPE[10].lcdWatercut = ui->lcdNumber_99;
    PIPE[11].lcdWatercut = ui->lcdNumber_104;
    PIPE[12].lcdWatercut = ui->lcdNumber_114;
    PIPE[13].lcdWatercut = ui->lcdNumber_121;
    PIPE[14].lcdWatercut = ui->lcdNumber_126;
    PIPE[15].lcdWatercut = ui->lcdNumber_136;
    PIPE[16].lcdWatercut = ui->lcdNumber_143;
    PIPE[17].lcdWatercut = ui->lcdNumber_148;

    /// lcdStartFreq
    PIPE[0].lcdStartFreq = ui->lcdNumber_47;
    PIPE[1].lcdStartFreq = ui->lcdNumber_51;
    PIPE[2].lcdStartFreq = ui->lcdNumber_56;
    PIPE[3].lcdStartFreq = ui->lcdNumber_61;
    PIPE[4].lcdStartFreq = ui->lcdNumber_65;
    PIPE[5].lcdStartFreq = ui->lcdNumber_70;
    PIPE[6].lcdStartFreq = ui->lcdNumber_75;
    PIPE[7].lcdStartFreq = ui->lcdNumber_79;
    PIPE[8].lcdStartFreq = ui->lcdNumber_84;
    PIPE[9].lcdStartFreq = ui->lcdNumber_93;
    PIPE[10].lcdStartFreq = ui->lcdNumber_97;
    PIPE[11].lcdStartFreq = ui->lcdNumber_102;
    PIPE[12].lcdStartFreq = ui->lcdNumber_115;
    PIPE[13].lcdStartFreq = ui->lcdNumber_119;
    PIPE[14].lcdStartFreq = ui->lcdNumber_124;
    PIPE[15].lcdStartFreq = ui->lcdNumber_137;
    PIPE[16].lcdStartFreq = ui->lcdNumber_141;
    PIPE[17].lcdStartFreq = ui->lcdNumber_146;

    /// lcdFreq 
    PIPE[0].lcdFreq = ui->lcdNumber_48;
    PIPE[1].lcdFreq = ui->lcdNumber_55;
    PIPE[2].lcdFreq = ui->lcdNumber_60;
    PIPE[3].lcdFreq = ui->lcdNumber_62;
    PIPE[4].lcdFreq = ui->lcdNumber_69;
    PIPE[5].lcdFreq = ui->lcdNumber_74;
    PIPE[6].lcdFreq = ui->lcdNumber_76;
    PIPE[7].lcdFreq = ui->lcdNumber_83;
    PIPE[8].lcdFreq = ui->lcdNumber_88;
    PIPE[9].lcdFreq = ui->lcdNumber_94;
    PIPE[10].lcdFreq = ui->lcdNumber_101;
    PIPE[11].lcdFreq = ui->lcdNumber_106;
    PIPE[12].lcdFreq = ui->lcdNumber_116;
    PIPE[13].lcdFreq = ui->lcdNumber_123;
    PIPE[14].lcdFreq = ui->lcdNumber_128;
    PIPE[15].lcdFreq = ui->lcdNumber_138;
    PIPE[16].lcdFreq = ui->lcdNumber_145;
    PIPE[17].lcdFreq = ui->lcdNumber_150;

    /// lcdTemp 
    PIPE[0].lcdTemp = ui->lcdNumber_49;
    PIPE[1].lcdTemp = ui->lcdNumber_54;
    PIPE[2].lcdTemp = ui->lcdNumber_59;
    PIPE[3].lcdTemp = ui->lcdNumber_63;
    PIPE[4].lcdTemp = ui->lcdNumber_68;
    PIPE[5].lcdTemp = ui->lcdNumber_73;
    PIPE[6].lcdTemp = ui->lcdNumber_77;
    PIPE[7].lcdTemp = ui->lcdNumber_82;
    PIPE[8].lcdTemp = ui->lcdNumber_87;
    PIPE[9].lcdTemp = ui->lcdNumber_95;
    PIPE[10].lcdTemp = ui->lcdNumber_100;
    PIPE[11].lcdTemp = ui->lcdNumber_105;
    PIPE[12].lcdTemp = ui->lcdNumber_117;
    PIPE[13].lcdTemp = ui->lcdNumber_122;
    PIPE[14].lcdTemp = ui->lcdNumber_127;
    PIPE[15].lcdTemp = ui->lcdNumber_139;
    PIPE[16].lcdTemp = ui->lcdNumber_144;
    PIPE[17].lcdTemp = ui->lcdNumber_149;

    /// lcd 
    PIPE[0].lcdReflectedPower = ui->lcdNumber_50;
    PIPE[1].lcdReflectedPower = ui->lcdNumber_52;
    PIPE[2].lcdReflectedPower = ui->lcdNumber_57;
    PIPE[3].lcdReflectedPower = ui->lcdNumber_64;
    PIPE[4].lcdReflectedPower = ui->lcdNumber_66;
    PIPE[5].lcdReflectedPower = ui->lcdNumber_71;
    PIPE[6].lcdReflectedPower = ui->lcdNumber_78;
    PIPE[7].lcdReflectedPower = ui->lcdNumber_80;
    PIPE[8].lcdReflectedPower = ui->lcdNumber_85;
    PIPE[9].lcdReflectedPower = ui->lcdNumber_96;
    PIPE[10].lcdReflectedPower = ui->lcdNumber_98;
    PIPE[11].lcdReflectedPower = ui->lcdNumber_103;
    PIPE[12].lcdReflectedPower = ui->lcdNumber_118;
    PIPE[13].lcdReflectedPower = ui->lcdNumber_120;
    PIPE[14].lcdReflectedPower = ui->lcdNumber_125;
    PIPE[15].lcdReflectedPower = ui->lcdNumber_140;
    PIPE[16].lcdReflectedPower = ui->lcdNumber_142;
    PIPE[17].lcdReflectedPower = ui->lcdNumber_147;
}


void
MainWindow::
initializeLoopObjects()
{
    for (int j = 0; j < MAX_LOOP; j++)
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
    LOOP[1].loopVolume = ui->lineEdit_4;
    LOOP[2].loopVolume = ui->lineEdit_6;
    LOOP[3].loopVolume = ui->lineEdit_12;
    LOOP[4].loopVolume = ui->lineEdit_20;
    LOOP[5].loopVolume = ui->lineEdit_24;

    /// saltStart 
    LOOP[0].saltStart = ui->comboBox_31;
    LOOP[1].saltStart = ui->comboBox_35;
    LOOP[2].saltStart = ui->comboBox_38;
    LOOP[3].saltStart = ui->comboBox_41;
    LOOP[4].saltStart = ui->comboBox_44;
    LOOP[5].saltStart = ui->comboBox_47;

    /// saltStop
    LOOP[0].saltStop = ui->comboBox_33;
    LOOP[1].saltStop = ui->comboBox_34;
    LOOP[2].saltStop = ui->comboBox_37;
    LOOP[3].saltStop = ui->comboBox_40;
    LOOP[4].saltStop = ui->comboBox_43;
    LOOP[5].saltStop = ui->comboBox_46;

    /// oilTemp
    LOOP[0].oilTemp = ui->comboBox_32;
    LOOP[1].oilTemp = ui->comboBox_36;
    LOOP[2].oilTemp = ui->comboBox_39;
    LOOP[3].oilTemp = ui->comboBox_42;
    LOOP[4].oilTemp = ui->comboBox_45;
    LOOP[5].oilTemp = ui->comboBox_48;

    /// waterRunStart 
    LOOP[0].waterRunStart = ui->lineEdit_37;
    LOOP[1].waterRunStart = ui->lineEdit_41;
    LOOP[2].waterRunStart = ui->lineEdit_45;
    LOOP[3].waterRunStart = ui->lineEdit_49;
    LOOP[4].waterRunStart = ui->lineEdit_53;
    LOOP[5].waterRunStart = ui->lineEdit_57;

    /// waterRunStop 
    LOOP[0].waterRunStop = ui->lineEdit_38;
    LOOP[1].waterRunStop = ui->lineEdit_42;
    LOOP[2].waterRunStop = ui->lineEdit_46;
    LOOP[3].waterRunStop = ui->lineEdit_50;
    LOOP[4].waterRunStop = ui->lineEdit_54;
    LOOP[5].waterRunStop = ui->lineEdit_58;

    /// oilRunStart 
    LOOP[0].oilRunStart = ui->lineEdit_39;
    LOOP[1].oilRunStart = ui->lineEdit_43;
    LOOP[2].oilRunStart = ui->lineEdit_47;
    LOOP[3].oilRunStart = ui->lineEdit_51;
    LOOP[4].oilRunStart = ui->lineEdit_55;
    LOOP[5].oilRunStart = ui->lineEdit_59;

    /// oilRunStop 
    LOOP[0].oilRunStop = ui->lineEdit_40;
    LOOP[1].oilRunStop = ui->lineEdit_44;
    LOOP[2].oilRunStop = ui->lineEdit_48;
    LOOP[3].oilRunStop = ui->lineEdit_52;
    LOOP[4].oilRunStop = ui->lineEdit_56;
    LOOP[5].oilRunStop = ui->lineEdit_60;

    /// masterWatercut
    LOOP[0].masterWatercut = ui->lcdNumber_8;
    LOOP[1].masterWatercut = ui->lcdNumber_11;
    LOOP[2].masterWatercut = ui->lcdNumber_14;
    LOOP[3].masterWatercut = ui->lcdNumber_17;
    LOOP[4].masterWatercut = ui->lcdNumber_20;
    LOOP[5].masterWatercut = ui->lcdNumber_23;

    /// masterSalinity
    LOOP[0].masterSalinity = ui->lcdNumber_9;
    LOOP[1].masterSalinity = ui->lcdNumber_12;
    LOOP[2].masterSalinity = ui->lcdNumber_15;
    LOOP[3].masterSalinity = ui->lcdNumber_18;
    LOOP[4].masterSalinity = ui->lcdNumber_21;
    LOOP[5].masterSalinity = ui->lcdNumber_24;

    /// masterOilAdj
    LOOP[0].masterOilAdj = ui->lcdNumber_10;
    LOOP[1].masterOilAdj = ui->lcdNumber_13;
    LOOP[2].masterOilAdj = ui->lcdNumber_16;
    LOOP[3].masterOilAdj = ui->lcdNumber_19;
    LOOP[4].masterOilAdj = ui->lcdNumber_22;
    LOOP[5].masterOilAdj = ui->lcdNumber_25;
}


void
MainWindow::
initializeGraph()
{
    /// draw chart lines
    updateChart(ui->gridLayout_5,LOOP[0].chartView,LOOP[0].chart,PIPE[0].series,90,5,280,48,300,68,400,89);
    updateChart(ui->gridLayout_5,LOOP[0].chartView,LOOP[0].chart,PIPE[1].series,100,5,300,48,400,68,600,89);
    updateChart(ui->gridLayout_5,LOOP[0].chartView,LOOP[0].chart,PIPE[2].series,110,5,400,48,600,68,800,89);

    updateChart(ui->gridLayout_6,LOOP[1].chartView,LOOP[1].chart,PIPE[3].series,90,5,280,48,300,68,400,89);
    updateChart(ui->gridLayout_6,LOOP[1].chartView,LOOP[1].chart,PIPE[4].series,100,5,300,48,400,68,600,89);
    updateChart(ui->gridLayout_6,LOOP[1].chartView,LOOP[1].chart,PIPE[5].series,110,5,400,48,600,68,800,89);

    updateChart(ui->gridLayout_9,LOOP[2].chartView,LOOP[2].chart,PIPE[6].series,90,5,280,48,300,68,400,89);
    updateChart(ui->gridLayout_9,LOOP[2].chartView,LOOP[2].chart,PIPE[7].series,100,5,300,48,400,68,600,89);
    updateChart(ui->gridLayout_9,LOOP[2].chartView,LOOP[2].chart,PIPE[8].series,110,5,400,48,600,68,800,89);

    updateChart(ui->gridLayout_13,LOOP[3].chartView,LOOP[3].chart,PIPE[9].series,90,5,280,48,300,68,400,89);
    updateChart(ui->gridLayout_13,LOOP[3].chartView,LOOP[3].chart,PIPE[10].series,100,5,300,48,400,68,600,89);
    updateChart(ui->gridLayout_13,LOOP[3].chartView,LOOP[3].chart,PIPE[11].series,110,5,400,48,600,68,800,89);

    updateChart(ui->gridLayout_17,LOOP[4].chartView,LOOP[4].chart,PIPE[12].series,90,5,280,48,300,68,400,89);
    updateChart(ui->gridLayout_17,LOOP[4].chartView,LOOP[4].chart,PIPE[13].series,100,5,300,48,400,68,600,89);
    updateChart(ui->gridLayout_17,LOOP[4].chartView,LOOP[4].chart,PIPE[14].series,110,5,400,48,600,68,800,89);

    updateChart(ui->gridLayout_21,LOOP[5].chartView,LOOP[5].chart,PIPE[15].series,90,5,280,48,300,68,400,89);
    updateChart(ui->gridLayout_21,LOOP[5].chartView,LOOP[5].chart,PIPE[16].series,100,5,300,48,400,68,600,89);
    updateChart(ui->gridLayout_21,LOOP[5].chartView,LOOP[5].chart,PIPE[17].series,110,5,400,48,600,68,800,89);
}


void
MainWindow::
updateLineView()
{
    for (int pipe = 0; pipe < MAX_PIPE; pipe++) (PIPE[pipe].lineView) ? PIPE[pipe].series->show() : PIPE[pipe].series->hide();
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
    connect( ui->groupBox_28, SIGNAL(toggled(bool)), this, SLOT(onCheckBoxChecked(bool)));
    connect( ui->groupBox_38, SIGNAL(toggled(bool)), this, SLOT(onCheckBoxChecked(bool)));
    connect( ui->groupBox_48, SIGNAL(toggled(bool)), this, SLOT(onCheckBoxChecked(bool)));
    connect( ui->groupBox_58, SIGNAL(toggled(bool)), this, SLOT(onCheckBoxChecked(bool)));
    connect( ui->groupBox_68, SIGNAL(toggled(bool)), this, SLOT(onCheckBoxChecked(bool)));
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
    connect(ui->actionInjection_Pump_Rates_2, SIGNAL(triggered()),signalPumpRatesMapper,SLOT(map()));
    connect(ui->actionInjection_Pump_Rates_3, SIGNAL(triggered()),signalPumpRatesMapper,SLOT(map()));
    connect(ui->actionInjection_Pump_Rates_4, SIGNAL(triggered()),signalPumpRatesMapper,SLOT(map()));
    connect(ui->actionInjection_Pump_Rates_5, SIGNAL(triggered()),signalPumpRatesMapper,SLOT(map()));
    connect(ui->actionInjection_Pump_Rates_6, SIGNAL(triggered()),signalPumpRatesMapper,SLOT(map()));
    signalPumpRatesMapper -> setMapping(ui->actionInjection_Pump_Rates, L1);
    signalPumpRatesMapper -> setMapping(ui->actionInjection_Pump_Rates_2, L2);
    signalPumpRatesMapper -> setMapping(ui->actionInjection_Pump_Rates_3, L3);
    signalPumpRatesMapper -> setMapping(ui->actionInjection_Pump_Rates_4, L4);
    signalPumpRatesMapper -> setMapping(ui->actionInjection_Pump_Rates_5, L5);
    signalPumpRatesMapper -> setMapping(ui->actionInjection_Pump_Rates_6, L6);
    connect (signalPumpRatesMapper, SIGNAL(mapped(int)), this, SLOT(injectionPumpRates(int)));

    /// injection bucket
    QSignalMapper* signalBucketMapper = new QSignalMapper (this) ; 
    connect(ui->actionInjection_Bucket, SIGNAL(triggered()),signalBucketMapper,SLOT(map()));
    connect(ui->actionInjection_Bucket_2, SIGNAL(triggered()),signalBucketMapper,SLOT(map()));
    connect(ui->actionInjection_Bucket_3, SIGNAL(triggered()),signalBucketMapper,SLOT(map()));
    connect(ui->actionInjection_Bucket_4, SIGNAL(triggered()),signalBucketMapper,SLOT(map()));
    connect(ui->actionInjection_Bucket_5, SIGNAL(triggered()),signalBucketMapper,SLOT(map()));
    connect(ui->actionInjection_Bucket_6, SIGNAL(triggered()),signalBucketMapper,SLOT(map()));
    signalBucketMapper -> setMapping(ui->actionInjection_Bucket, L1);
    signalBucketMapper -> setMapping(ui->actionInjection_Bucket_2, L2);
    signalBucketMapper -> setMapping(ui->actionInjection_Bucket_3, L3);
    signalBucketMapper -> setMapping(ui->actionInjection_Bucket_4, L4);
    signalBucketMapper -> setMapping(ui->actionInjection_Bucket_5, L5);
    signalBucketMapper -> setMapping(ui->actionInjection_Bucket_6, L6);
    connect (signalBucketMapper, SIGNAL(mapped(int)), this, SLOT(injectionBucket(int)));

    /// injection mark
    QSignalMapper* signalMarkMapper = new QSignalMapper (this) ; 
    connect(ui->actionInjection_Mark, SIGNAL(triggered()),signalMarkMapper,SLOT(map()));
    connect(ui->actionInjection_Mark_2, SIGNAL(triggered()),signalMarkMapper,SLOT(map()));
    connect(ui->actionInjection_Mark_3, SIGNAL(triggered()),signalMarkMapper,SLOT(map()));
    connect(ui->actionInjection_Mark_4, SIGNAL(triggered()),signalMarkMapper,SLOT(map()));
    connect(ui->actionInjection_Mark_5, SIGNAL(triggered()),signalMarkMapper,SLOT(map()));
    connect(ui->actionInjection_Mark_6, SIGNAL(triggered()),signalMarkMapper,SLOT(map()));
    signalMarkMapper -> setMapping(ui->actionInjection_Mark, L1);
    signalMarkMapper -> setMapping(ui->actionInjection_Mark_2, L2);
    signalMarkMapper -> setMapping(ui->actionInjection_Mark_3, L3);
    signalMarkMapper -> setMapping(ui->actionInjection_Mark_4, L4);
    signalMarkMapper -> setMapping(ui->actionInjection_Mark_5, L5);
    signalMarkMapper -> setMapping(ui->actionInjection_Mark_6, L6);
    connect (signalMarkMapper, SIGNAL(mapped(int)), this, SLOT(injectionMark(int)));

    /// injection method
    QSignalMapper* signalMethodMapper = new QSignalMapper (this) ; 
    connect(ui->actionInjection_Method, SIGNAL(triggered()),signalMethodMapper,SLOT(map()));
    connect(ui->actionInjection_Method_2, SIGNAL(triggered()),signalMethodMapper,SLOT(map()));
    connect(ui->actionInjection_Method_3, SIGNAL(triggered()),signalMethodMapper,SLOT(map()));
    connect(ui->actionInjection_Method_4, SIGNAL(triggered()),signalMethodMapper,SLOT(map()));
    connect(ui->actionInjection_Method_5, SIGNAL(triggered()),signalMethodMapper,SLOT(map()));
    connect(ui->actionInjection_Method_6, SIGNAL(triggered()),signalMethodMapper,SLOT(map()));
    signalMethodMapper -> setMapping(ui->actionInjection_Method, L1);
    signalMethodMapper -> setMapping(ui->actionInjection_Method_2, L2);
    signalMethodMapper -> setMapping(ui->actionInjection_Method_3, L3);
    signalMethodMapper -> setMapping(ui->actionInjection_Method_4, L4);
    signalMethodMapper -> setMapping(ui->actionInjection_Method_5, L5);
    signalMethodMapper -> setMapping(ui->actionInjection_Method_6, L6);
    connect (signalMethodMapper, SIGNAL(mapped(int)), this, SLOT(injectionMethod(int)));

    /// pressure sensor slope
    QSignalMapper* signalPressure = new QSignalMapper (this) ; 
    connect(ui->actionPressure_Sensor_Slope, SIGNAL(triggered()),signalPressure,SLOT(map()));
    connect(ui->actionPressure_Sensor_Slope_2, SIGNAL(triggered()),signalPressure,SLOT(map()));
    connect(ui->actionPressure_Sensor_Slope_3, SIGNAL(triggered()),signalPressure,SLOT(map()));
    connect(ui->actionPressure_Sensor_Slope_4, SIGNAL(triggered()),signalPressure,SLOT(map()));
    connect(ui->actionPressure_Sensor_Slope_5, SIGNAL(triggered()),signalPressure,SLOT(map()));
    connect(ui->actionPressure_Sensor_Slope_6, SIGNAL(triggered()),signalPressure,SLOT(map()));
    signalPressure -> setMapping(ui->actionPressure_Sensor_Slope, L1);
    signalPressure -> setMapping(ui->actionPressure_Sensor_Slope_2, L2);
    signalPressure -> setMapping(ui->actionPressure_Sensor_Slope_3, L3);
    signalPressure -> setMapping(ui->actionPressure_Sensor_Slope_4, L4);
    signalPressure -> setMapping(ui->actionPressure_Sensor_Slope_5, L5);
    signalPressure -> setMapping(ui->actionPressure_Sensor_Slope_6, L6);
    connect (signalPressure, SIGNAL(mapped(int)), this, SLOT(onActionPressureSensorSlope(int)));

    /// Minimum reference temperature 
    QSignalMapper* signalMinRefTemp = new QSignalMapper (this) ; 
    connect(ui->actionMin_Ref_Temp, SIGNAL(triggered()),signalMinRefTemp,SLOT(map()));
    connect(ui->actionMin_Ref_Temp_2, SIGNAL(triggered()),signalMinRefTemp,SLOT(map()));
    connect(ui->actionMin_Ref_Temp_3, SIGNAL(triggered()),signalMinRefTemp,SLOT(map()));
    connect(ui->actionMin_Ref_Temp_4, SIGNAL(triggered()),signalMinRefTemp,SLOT(map()));
    connect(ui->actionMin_Ref_Temp_5, SIGNAL(triggered()),signalMinRefTemp,SLOT(map()));
    connect(ui->actionMin_Ref_Temp_6, SIGNAL(triggered()),signalMinRefTemp,SLOT(map()));
    signalMinRefTemp -> setMapping(ui->actionMin_Ref_Temp, L1);
    signalMinRefTemp -> setMapping(ui->actionMin_Ref_Temp_2, L2);
    signalMinRefTemp -> setMapping(ui->actionMin_Ref_Temp_3, L3);
    signalMinRefTemp -> setMapping(ui->actionMin_Ref_Temp_4, L4);
    signalMinRefTemp -> setMapping(ui->actionMin_Ref_Temp_5, L5);
    signalMinRefTemp -> setMapping(ui->actionMin_Ref_Temp_6, L6);
    connect (signalMinRefTemp, SIGNAL(mapped(int)), this, SLOT(onMinRefTemp(int)));

	/// Maximum reference temperature 
    QSignalMapper* signalMaxRefTemp = new QSignalMapper (this) ; 
    connect(ui->actionMax_Ref_Temp, SIGNAL(triggered()),signalMaxRefTemp,SLOT(map()));
    connect(ui->actionMax_Ref_Temp_2, SIGNAL(triggered()),signalMaxRefTemp,SLOT(map()));
    connect(ui->actionMax_Ref_Temp_3, SIGNAL(triggered()),signalMaxRefTemp,SLOT(map()));
    connect(ui->actionMax_Ref_Temp_4, SIGNAL(triggered()),signalMaxRefTemp,SLOT(map()));
    connect(ui->actionMax_Ref_Temp_5, SIGNAL(triggered()),signalMaxRefTemp,SLOT(map()));
    connect(ui->actionMax_Ref_Temp_6, SIGNAL(triggered()),signalMaxRefTemp,SLOT(map()));
    signalMaxRefTemp -> setMapping(ui->actionMax_Ref_Temp, L1);
    signalMaxRefTemp -> setMapping(ui->actionMax_Ref_Temp_2, L2);
    signalMaxRefTemp -> setMapping(ui->actionMax_Ref_Temp_3, L3);
    signalMaxRefTemp -> setMapping(ui->actionMax_Ref_Temp_4, L4);
    signalMaxRefTemp -> setMapping(ui->actionMax_Ref_Temp_5, L5);
    signalMaxRefTemp -> setMapping(ui->actionMax_Ref_Temp_6, L6);
    connect (signalMaxRefTemp, SIGNAL(mapped(int)), this, SLOT(onMaxRefTemp(int)));

	/// injection temperature 
    QSignalMapper* signalInjectionTemp = new QSignalMapper (this) ; 
    connect(ui->actionInjection_Temp, SIGNAL(triggered()),signalInjectionTemp,SLOT(map()));
    connect(ui->actionInjection_Temp_2, SIGNAL(triggered()),signalInjectionTemp,SLOT(map()));
    connect(ui->actionInjection_Temp_3, SIGNAL(triggered()),signalInjectionTemp,SLOT(map()));
    connect(ui->actionInjection_Temp_4, SIGNAL(triggered()),signalInjectionTemp,SLOT(map()));
    connect(ui->actionInjection_Temp_5, SIGNAL(triggered()),signalInjectionTemp,SLOT(map()));
    connect(ui->actionInjection_Temp_6, SIGNAL(triggered()),signalInjectionTemp,SLOT(map()));
    signalInjectionTemp -> setMapping(ui->actionInjection_Temp, L1);
    signalInjectionTemp -> setMapping(ui->actionInjection_Temp_2, L2);
    signalInjectionTemp -> setMapping(ui->actionInjection_Temp_3, L3);
    signalInjectionTemp -> setMapping(ui->actionInjection_Temp_4, L4);
    signalInjectionTemp -> setMapping(ui->actionInjection_Temp_5, L5);
    signalInjectionTemp -> setMapping(ui->actionInjection_Temp_6, L6);
    connect (signalInjectionTemp, SIGNAL(mapped(int)), this, SLOT(onInjectionTemp(int)));

    /// Delta Stability X seconds 
    QSignalMapper* signalxDelay = new QSignalMapper (this) ; 
    connect(ui->actionX_seconds, SIGNAL(triggered()),signalxDelay,SLOT(map()));
    connect(ui->actionX_seconds_2, SIGNAL(triggered()),signalxDelay,SLOT(map()));
    connect(ui->actionX_seconds_3, SIGNAL(triggered()),signalxDelay,SLOT(map()));
    connect(ui->actionX_seconds_4, SIGNAL(triggered()),signalxDelay,SLOT(map()));
    connect(ui->actionX_seconds_5, SIGNAL(triggered()),signalxDelay,SLOT(map()));
    connect(ui->actionX_seconds_6, SIGNAL(triggered()),signalxDelay,SLOT(map()));
    signalxDelay -> setMapping(ui->actionX_seconds, L1);
    signalxDelay -> setMapping(ui->actionX_seconds_2, L2);
    signalxDelay -> setMapping(ui->actionX_seconds_3, L3);
    signalxDelay -> setMapping(ui->actionX_seconds_4, L4);
    signalxDelay -> setMapping(ui->actionX_seconds_5, L5);
    signalxDelay -> setMapping(ui->actionX_seconds_6, L6);
    connect (signalxDelay, SIGNAL(mapped(int)), this, SLOT(onXDelay(int)));

    /// Delta Stability Frequency 
    QSignalMapper* signalY_KHz = new QSignalMapper (this) ; 
    connect(ui->actionY_KHz, SIGNAL(triggered()),signalY_KHz,SLOT(map()));
    connect(ui->actionY_KHz_2, SIGNAL(triggered()),signalY_KHz,SLOT(map()));
    connect(ui->actionY_KHz_3, SIGNAL(triggered()),signalY_KHz,SLOT(map()));
    connect(ui->actionY_KHz_4, SIGNAL(triggered()),signalY_KHz,SLOT(map()));
    connect(ui->actionY_KHz_5, SIGNAL(triggered()),signalY_KHz,SLOT(map()));
    connect(ui->actionY_KHz_6, SIGNAL(triggered()),signalY_KHz,SLOT(map()));
    signalY_KHz -> setMapping(ui->actionY_KHz, L1);
    signalY_KHz -> setMapping(ui->actionY_KHz_2, L2);
    signalY_KHz -> setMapping(ui->actionY_KHz_3, L3);
    signalY_KHz -> setMapping(ui->actionY_KHz_4, L4);
    signalY_KHz -> setMapping(ui->actionY_KHz_5, L5);
    signalY_KHz -> setMapping(ui->actionY_KHz_6, L6);
    connect (signalY_KHz, SIGNAL(mapped(int)), this, SLOT(onYFreq(int)));

    /// Delta Stability Temperature
    QSignalMapper* signalZ_Temp = new QSignalMapper (this) ; 
    connect(ui->actionZ_C, SIGNAL(triggered()),signalZ_Temp,SLOT(map()));
    connect(ui->actionZ_C_2, SIGNAL(triggered()),signalZ_Temp,SLOT(map()));
    connect(ui->actionZ_C_3, SIGNAL(triggered()),signalZ_Temp,SLOT(map()));
    connect(ui->actionZ_C_4, SIGNAL(triggered()),signalZ_Temp,SLOT(map()));
    connect(ui->actionZ_C_5, SIGNAL(triggered()),signalZ_Temp,SLOT(map()));
    connect(ui->actionZ_C_6, SIGNAL(triggered()),signalZ_Temp,SLOT(map()));
    signalZ_Temp -> setMapping(ui->actionZ_C, L1);
    signalZ_Temp -> setMapping(ui->actionZ_C_2, L2);
    signalZ_Temp -> setMapping(ui->actionZ_C_3, L3);
    signalZ_Temp -> setMapping(ui->actionZ_C_4, L4);
    signalZ_Temp -> setMapping(ui->actionZ_C_5, L5);
    signalZ_Temp -> setMapping(ui->actionZ_C_6, L6);
    connect (signalZ_Temp, SIGNAL(mapped(int)), this, SLOT(onZTemp(int)));

    /// file folders
    connect(ui->actionMain_Server, SIGNAL(triggered()),this,SLOT(onActionMainServer()));
    connect(ui->actionLocal_Server, SIGNAL(triggered()),this,SLOT(onActionLocalServer()));
}


void
MainWindow::
connectCalibrationControls()
{
    /// start L1
    QSignalMapper* signalPressed_L1 = new QSignalMapper (this) ; 
    connect(ui->pushButton_4, SIGNAL(pressed()),signalPressed_L1, SLOT(map()));
    signalPressed_L1->setMapping(ui->pushButton_4, L1);
    connect (signalPressed_L1, SIGNAL(mapped(int)), this, SLOT(onCalibrationButtonPressed(int)));

    /// start L2
    QSignalMapper* signalPressed_L2 = new QSignalMapper (this) ; 
    connect(ui->pushButton_9, SIGNAL(pressed()),signalPressed_L2, SLOT(map()));
    signalPressed_L2->setMapping(ui->pushButton_9, L2);
    connect (signalPressed_L2, SIGNAL(mapped(int)), this, SLOT(onCalibrationButtonPressed(int)));
 
    /// start L3
    QSignalMapper* signalPressed_L3 = new QSignalMapper (this) ; 
    connect(ui->pushButton_12, SIGNAL(pressed()),signalPressed_L3, SLOT(map()));
    signalPressed_L3->setMapping(ui->pushButton_12, L3);
    connect (signalPressed_L3, SIGNAL(mapped(int)), this, SLOT(onCalibrationButtonPressed(int)));
 
    /// start L4
    QSignalMapper* signalPressed_L4 = new QSignalMapper (this) ; 
    connect(ui->pushButton_15, SIGNAL(pressed()),signalPressed_L4, SLOT(map()));
    signalPressed_L4->setMapping(ui->pushButton_15, L4);
    connect (signalPressed_L4, SIGNAL(mapped(int)), this, SLOT(onCalibrationButtonPressed(int)));
 
    /// start L5
    QSignalMapper* signalPressed_L5 = new QSignalMapper (this) ; 
    connect(ui->pushButton_18, SIGNAL(pressed()),signalPressed_L5, SLOT(map()));
    signalPressed_L5->setMapping(ui->pushButton_18, L5);
    connect (signalPressed_L5, SIGNAL(mapped(int)), this, SLOT(onCalibrationButtonPressed(int)));
 
    /// start L6
    QSignalMapper* signalPressed_L6 = new QSignalMapper (this) ; 
    connect(ui->pushButton_21, SIGNAL(pressed()),signalPressed_L6, SLOT(map()));
    signalPressed_L6->setMapping(ui->pushButton_21, L6);
    connect (signalPressed_L6, SIGNAL(mapped(int)), this, SLOT(onCalibrationButtonPressed(int)));
}


/// hide/show graph line
void
MainWindow::
toggleLineView_L1P1(bool b)
{
   (b) ? PIPE[0].series->show() : PIPE[0].series->hide();
}

void
MainWindow::
toggleLineView_L1P2(bool b)
{
   (b) ? PIPE[1].series->show() : PIPE[1].series->hide();
}

void
MainWindow::
toggleLineView_L1P3(bool b)
{
   (b) ? PIPE[2].series->show() : PIPE[2].series->hide();
}

void
MainWindow::
toggleLineView_L2P1(bool b)
{
   (b) ? PIPE[3].series->show() : PIPE[3].series->hide();
}

void
MainWindow::
toggleLineView_L2P2(bool b)
{
   (b) ? PIPE[4].series->show() : PIPE[4].series->hide();
}

void
MainWindow::
toggleLineView_L2P3(bool b)
{
   (b) ? PIPE[5].series->show() : PIPE[5].series->hide();
}

void
MainWindow::
toggleLineView_L3P1(bool b)
{
   (b) ? PIPE[6].series->show() : PIPE[6].series->hide();
}

void
MainWindow::
toggleLineView_L3P2(bool b)
{
   (b) ? PIPE[7].series->show() : PIPE[7].series->hide();
}

void
MainWindow::
toggleLineView_L3P3(bool b)
{
   (b) ? PIPE[8].series->show() : PIPE[8].series->hide();
}

void
MainWindow::
toggleLineView_L4P1(bool b)
{
   (b) ? PIPE[9].series->show() : PIPE[9].series->hide();
}

void
MainWindow::
toggleLineView_L4P2(bool b)
{
   (b) ? PIPE[10].series->show() : PIPE[10].series->hide();
}

void
MainWindow::
toggleLineView_L4P3(bool b)
{
   (b) ? PIPE[11].series->show() : PIPE[11].series->hide();
}

void
MainWindow::
toggleLineView_L5P1(bool b)
{
   (b) ? PIPE[12].series->show() : PIPE[12].series->hide();
}

void
MainWindow::
toggleLineView_L5P2(bool b)
{
   (b) ? PIPE[13].series->show() : PIPE[13].series->hide();
}

void
MainWindow::
toggleLineView_L5P3(bool b)
{
   (b) ? PIPE[14].series->show() : PIPE[14].series->hide();
}

void
MainWindow::
toggleLineView_L6P1(bool b)
{
   (b) ? PIPE[15].series->show() : PIPE[15].series->hide();
}

void
MainWindow::
toggleLineView_L6P2(bool b)
{
   (b) ? PIPE[16].series->show() : PIPE[16].series->hide();
}

void
MainWindow::
toggleLineView_L6P3(bool b)
{
   (b) ? PIPE[17].series->show() : PIPE[17].series->hide();
}

void
MainWindow::
connectLineView()
{
    connect(ui->checkBox_19, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L1P1(bool)));
    connect(ui->checkBox_20, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L1P2(bool)));
    connect(ui->checkBox_21, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L1P3(bool)));

    connect(ui->checkBox_22, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L2P1(bool)));
    connect(ui->checkBox_23, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L2P2(bool)));
    connect(ui->checkBox_24, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L2P3(bool)));

    connect(ui->checkBox_25, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L3P1(bool)));
    connect(ui->checkBox_26, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L3P2(bool)));
    connect(ui->checkBox_27, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L3P3(bool)));

    connect(ui->checkBox_28, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L4P1(bool)));
    connect(ui->checkBox_29, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L4P2(bool)));
    connect(ui->checkBox_30, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L4P3(bool)));

    connect(ui->checkBox_31, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L5P1(bool)));
    connect(ui->checkBox_32, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L5P2(bool)));
    connect(ui->checkBox_33, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L5P3(bool)));

    connect(ui->checkBox_34, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L6P1(bool)));
    connect(ui->checkBox_35, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L6P2(bool)));
    connect(ui->checkBox_36, SIGNAL(clicked(bool)), this, SLOT(toggleLineView_L6P3(bool)));
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
	LOOP[L1].injectionOilPumpRate = json[L1_OIL_PUMP_RATE].toDouble();
	LOOP[L1].injectionWaterPumpRate = json[L1_WATER_PUMP_RATE].toDouble();
	LOOP[L1].injectionSmallWaterPumpRate = json[L1_SMALL_WATER_PUMP_RATE].toDouble();
	LOOP[L1].injectionBucket = json[L1_BUCKET].toDouble();
	LOOP[L1].injectionMark = json[L1_MARK].toDouble();
	LOOP[L1].injectionMethod = json[L1_METHOD].toDouble();
	LOOP[L1].pressureSensorSlope = json[L1_PRESSURE].toDouble();
	LOOP[L1].minRefTemp = json[L1_MIN_TEMP].toInt();
	LOOP[L1].maxRefTemp = json[L1_MAX_TEMP].toInt();
	LOOP[L1].injectionTemp = json[L1_INJECTION_TEMP].toInt();
	LOOP[L1].xDelay = json[L1_X_DELAY].toInt();
	LOOP[L1].yFreq = json[L1_Y_FREQ].toDouble();
	LOOP[L1].zTemp = json[L1_Z_TEMP].toDouble();

	LOOP[L2].injectionOilPumpRate = json[L2_OIL_PUMP_RATE].toDouble();
	LOOP[L2].injectionWaterPumpRate = json[L2_WATER_PUMP_RATE].toDouble();
	LOOP[L2].injectionSmallWaterPumpRate = json[L2_SMALL_WATER_PUMP_RATE].toDouble();
	LOOP[L2].injectionBucket = json[L2_BUCKET].toDouble();
	LOOP[L2].injectionMark = json[L2_MARK].toDouble();
	LOOP[L2].injectionMethod = json[L2_METHOD].toDouble();
	LOOP[L2].pressureSensorSlope = json[L2_PRESSURE].toDouble();
	LOOP[L2].minRefTemp = json[L2_MIN_TEMP].toInt();
	LOOP[L2].maxRefTemp = json[L2_MAX_TEMP].toInt();
	LOOP[L2].injectionTemp = json[L2_INJECTION_TEMP].toInt();
	LOOP[L2].xDelay = json[L2_X_DELAY].toInt();
	LOOP[L2].yFreq = json[L2_Y_FREQ].toDouble();
	LOOP[L2].zTemp = json[L2_Z_TEMP].toDouble();

	LOOP[L3].injectionOilPumpRate = json[L3_OIL_PUMP_RATE].toDouble();
	LOOP[L3].injectionWaterPumpRate = json[L3_WATER_PUMP_RATE].toDouble();
	LOOP[L3].injectionSmallWaterPumpRate = json[L3_SMALL_WATER_PUMP_RATE].toDouble();
	LOOP[L3].injectionBucket = json[L3_BUCKET].toDouble();
	LOOP[L3].injectionMark = json[L3_MARK].toDouble();
	LOOP[L3].injectionMethod = json[L3_METHOD].toDouble();
	LOOP[L3].pressureSensorSlope = json[L3_PRESSURE].toDouble();
	LOOP[L3].minRefTemp = json[L3_MIN_TEMP].toInt();
	LOOP[L3].maxRefTemp = json[L3_MAX_TEMP].toInt();
	LOOP[L3].injectionTemp = json[L3_INJECTION_TEMP].toInt();
	LOOP[L3].xDelay = json[L3_X_DELAY].toInt();
	LOOP[L3].yFreq = json[L3_Y_FREQ].toDouble();
	LOOP[L3].zTemp = json[L3_Z_TEMP].toDouble();

	LOOP[L4].injectionOilPumpRate = json[L4_OIL_PUMP_RATE].toDouble();
	LOOP[L4].injectionWaterPumpRate = json[L4_WATER_PUMP_RATE].toDouble();
	LOOP[L4].injectionSmallWaterPumpRate = json[L4_SMALL_WATER_PUMP_RATE].toDouble();
	LOOP[L4].injectionBucket = json[L4_BUCKET].toDouble();
	LOOP[L4].injectionMark = json[L4_MARK].toDouble();
	LOOP[L4].injectionMethod = json[L4_METHOD].toDouble();
	LOOP[L4].pressureSensorSlope = json[L4_PRESSURE].toDouble();
	LOOP[L4].minRefTemp = json[L4_MIN_TEMP].toInt();
	LOOP[L4].maxRefTemp = json[L4_MAX_TEMP].toInt();
	LOOP[L4].injectionTemp = json[L4_INJECTION_TEMP].toInt();
	LOOP[L4].xDelay = json[L4_X_DELAY].toInt();
	LOOP[L4].yFreq = json[L4_Y_FREQ].toDouble();
	LOOP[L4].zTemp = json[L4_Z_TEMP].toDouble();

	LOOP[L5].injectionOilPumpRate = json[L5_OIL_PUMP_RATE].toDouble();
	LOOP[L5].injectionWaterPumpRate = json[L5_WATER_PUMP_RATE].toDouble();
	LOOP[L5].injectionSmallWaterPumpRate = json[L5_SMALL_WATER_PUMP_RATE].toDouble();
	LOOP[L5].injectionBucket = json[L5_BUCKET].toDouble();
	LOOP[L5].injectionMark = json[L5_MARK].toDouble();
	LOOP[L5].injectionMethod = json[L5_METHOD].toDouble();
	LOOP[L5].pressureSensorSlope = json[L5_PRESSURE].toDouble();
	LOOP[L5].minRefTemp = json[L5_MIN_TEMP].toInt();
	LOOP[L5].maxRefTemp = json[L5_MAX_TEMP].toInt();
	LOOP[L5].injectionTemp = json[L5_INJECTION_TEMP].toInt();
	LOOP[L5].xDelay = json[L5_X_DELAY].toInt();
	LOOP[L5].yFreq = json[L5_Y_FREQ].toDouble();
	LOOP[L5].zTemp = json[L5_Z_TEMP].toDouble();

	LOOP[L6].injectionOilPumpRate = json[L6_OIL_PUMP_RATE].toDouble();
	LOOP[L6].injectionWaterPumpRate = json[L6_WATER_PUMP_RATE].toDouble();
	LOOP[L6].injectionSmallWaterPumpRate = json[L6_SMALL_WATER_PUMP_RATE].toDouble();
	LOOP[L6].injectionBucket = json[L6_BUCKET].toDouble();
	LOOP[L6].injectionMark = json[L6_MARK].toDouble();
	LOOP[L6].injectionMethod = json[L6_METHOD].toDouble();
	LOOP[L6].pressureSensorSlope = json[L6_PRESSURE].toDouble();
	LOOP[L6].minRefTemp = json[L6_MIN_TEMP].toInt();
	LOOP[L6].maxRefTemp = json[L6_MAX_TEMP].toInt();
	LOOP[L6].injectionTemp = json[L6_INJECTION_TEMP].toInt();
	LOOP[L6].xDelay = json[L6_X_DELAY].toInt();
	LOOP[L6].yFreq = json[L6_Y_FREQ].toDouble();
	LOOP[L6].zTemp = json[L6_Z_TEMP].toDouble();

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
	json[L1_OIL_PUMP_RATE] = QString::number(LOOP[L1].injectionOilPumpRate);
	json[L1_WATER_PUMP_RATE] = QString::number(LOOP[L1].injectionWaterPumpRate);
	json[L1_SMALL_WATER_PUMP_RATE] = QString::number(LOOP[L1].injectionSmallWaterPumpRate);
	json[L1_BUCKET] = QString::number(LOOP[L1].injectionBucket);
	json[L1_MARK] = QString::number(LOOP[L1].injectionMark);
	json[L1_METHOD] = QString::number(LOOP[L1].injectionMethod);
	json[L1_PRESSURE] = QString::number(LOOP[L1].pressureSensorSlope);
	json[L1_MIN_TEMP] = QString::number(LOOP[L1].minRefTemp);
	json[L1_MAX_TEMP] = QString::number(LOOP[L1].maxRefTemp);
	json[L1_INJECTION_TEMP] = QString::number(LOOP[L1].injectionTemp);
	json[L1_X_DELAY] = QString::number(LOOP[L1].xDelay);
	json[L1_Y_FREQ] = QString::number(LOOP[L1].yFreq);
	json[L1_Z_TEMP] = QString::number(LOOP[L1].zTemp);

	json[L2_OIL_PUMP_RATE] = QString::number(LOOP[L2].injectionOilPumpRate);
	json[L2_WATER_PUMP_RATE] = QString::number(LOOP[L2].injectionWaterPumpRate);
	json[L2_SMALL_WATER_PUMP_RATE] = QString::number(LOOP[L2].injectionSmallWaterPumpRate);
	json[L2_BUCKET] = QString::number(LOOP[L2].injectionBucket);
	json[L2_MARK] = QString::number(LOOP[L2].injectionMark);
	json[L2_METHOD] = QString::number(LOOP[L2].injectionMethod);
	json[L2_PRESSURE] = QString::number(LOOP[L2].pressureSensorSlope);
	json[L2_MIN_TEMP] = QString::number(LOOP[L2].minRefTemp);
	json[L2_MAX_TEMP] = QString::number(LOOP[L2].maxRefTemp);
	json[L2_INJECTION_TEMP] = QString::number(LOOP[L2].injectionTemp);
	json[L2_X_DELAY] = QString::number(LOOP[L2].xDelay);
	json[L2_Y_FREQ] = QString::number(LOOP[L2].yFreq);
	json[L2_Z_TEMP] = QString::number(LOOP[L2].zTemp);

	json[L3_OIL_PUMP_RATE] = QString::number(LOOP[L3].injectionOilPumpRate);
	json[L3_WATER_PUMP_RATE] = QString::number(LOOP[L3].injectionWaterPumpRate);
	json[L3_SMALL_WATER_PUMP_RATE] = QString::number(LOOP[L3].injectionSmallWaterPumpRate);
	json[L3_BUCKET] = QString::number(LOOP[L3].injectionBucket);
	json[L3_MARK] = QString::number(LOOP[L3].injectionMark);
	json[L3_METHOD] = QString::number(LOOP[L3].injectionMethod);
	json[L3_PRESSURE] = QString::number(LOOP[L3].pressureSensorSlope);
	json[L3_MIN_TEMP] = QString::number(LOOP[L3].minRefTemp);
	json[L3_MAX_TEMP] = QString::number(LOOP[L3].maxRefTemp);
	json[L3_INJECTION_TEMP] = QString::number(LOOP[L3].injectionTemp);
	json[L3_X_DELAY] = QString::number(LOOP[L3].xDelay);
	json[L3_Y_FREQ] = QString::number(LOOP[L3].yFreq);
	json[L3_Z_TEMP] = QString::number(LOOP[L3].zTemp);

	json[L4_OIL_PUMP_RATE] = QString::number(LOOP[L4].injectionOilPumpRate);
	json[L4_WATER_PUMP_RATE] = QString::number(LOOP[L4].injectionWaterPumpRate);
	json[L4_SMALL_WATER_PUMP_RATE] = QString::number(LOOP[L4].injectionSmallWaterPumpRate);
	json[L4_BUCKET] = QString::number(LOOP[L4].injectionBucket);
	json[L4_MARK] = QString::number(LOOP[L4].injectionMark);
	json[L4_METHOD] = QString::number(LOOP[L4].injectionMethod);
	json[L4_PRESSURE] = QString::number(LOOP[L4].pressureSensorSlope);
	json[L4_MIN_TEMP] = QString::number(LOOP[L4].minRefTemp);
	json[L4_MAX_TEMP] = QString::number(LOOP[L4].maxRefTemp);
	json[L4_INJECTION_TEMP] = QString::number(LOOP[L4].injectionTemp);
	json[L4_X_DELAY] = QString::number(LOOP[L4].xDelay);
	json[L4_Y_FREQ] = QString::number(LOOP[L4].yFreq);
	json[L4_Z_TEMP] = QString::number(LOOP[L4].zTemp);

	json[L5_OIL_PUMP_RATE] = QString::number(LOOP[L5].injectionOilPumpRate);
	json[L5_WATER_PUMP_RATE] = QString::number(LOOP[L5].injectionWaterPumpRate);
	json[L5_SMALL_WATER_PUMP_RATE] = QString::number(LOOP[L5].injectionSmallWaterPumpRate);
	json[L5_BUCKET] = QString::number(LOOP[L5].injectionBucket);
	json[L5_MARK] = QString::number(LOOP[L5].injectionMark);
	json[L5_METHOD] = QString::number(LOOP[L5].injectionMethod);
	json[L5_PRESSURE] = QString::number(LOOP[L5].pressureSensorSlope);
	json[L5_MIN_TEMP] = QString::number(LOOP[L5].minRefTemp);
	json[L5_MAX_TEMP] = QString::number(LOOP[L5].maxRefTemp);
	json[L5_INJECTION_TEMP] = QString::number(LOOP[L5].injectionTemp);
	json[L5_X_DELAY] = QString::number(LOOP[L5].xDelay);
	json[L5_Y_FREQ] = QString::number(LOOP[L5].yFreq);
	json[L5_Z_TEMP] = QString::number(LOOP[L5].zTemp);

	json[L6_OIL_PUMP_RATE] = QString::number(LOOP[L6].injectionOilPumpRate);
	json[L6_WATER_PUMP_RATE] = QString::number(LOOP[L6].injectionWaterPumpRate);
	json[L6_SMALL_WATER_PUMP_RATE] = QString::number(LOOP[L6].injectionSmallWaterPumpRate);
	json[L6_BUCKET] = QString::number(LOOP[L6].injectionBucket);
	json[L6_MARK] = QString::number(LOOP[L6].injectionMark);
	json[L6_METHOD] = QString::number(LOOP[L6].injectionMethod);
	json[L6_PRESSURE] = QString::number(LOOP[L6].pressureSensorSlope);
	json[L6_MIN_TEMP] = QString::number(LOOP[L6].minRefTemp);
	json[L6_MAX_TEMP] = QString::number(LOOP[L6].maxRefTemp);
	json[L6_INJECTION_TEMP] = QString::number(LOOP[L6].injectionTemp);
	json[L6_X_DELAY] = QString::number(LOOP[L6].xDelay);
	json[L6_Y_FREQ] = QString::number(LOOP[L6].yFreq);
	json[L6_Z_TEMP] = QString::number(LOOP[L6].zTemp);

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
    LOOP[index].injectionOilPumpRate = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+ tr("Injection Oil Pump Rate"),tr("Enter Injection Oil Pump Rate [mL/min]"),LOOP[index].injectionOilPumpRate , -10000, 10000, 2, &ok,Qt::WindowFlags(), 1);
    LOOP[index].injectionWaterPumpRate = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+ tr("Injection Water Pump Rate"),tr("Enter Injection Water Pump Rate [mL/min]"), LOOP[index].injectionWaterPumpRate, -10000, 10000, 2, &ok,Qt::WindowFlags(), 1);
    if (ui->radioButton_6->isChecked()) LOOP[index].injectionSmallWaterPumpRate = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+tr("Injection Small Water Pump Rate"),tr("Enter Injection Small Water Pump Rate [mL/min]"),LOOP[index].injectionSmallWaterPumpRate , -10000, 10000, 2, &ok,Qt::WindowFlags(), 1);
    
    /// update json config file
    writeJsonConfigFile();
}

void
MainWindow::
injectionBucket(int index)
{
    bool ok;
    LOOP[index].injectionBucket = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+ tr("Injection Bucket"),tr("Enter Injection Bucket [L]"),LOOP[index].injectionBucket , -10000, 10000, 2, &ok,Qt::WindowFlags(), 1);
     
    /// update json config file
    writeJsonConfigFile();
}

void
MainWindow::
injectionMark(int index)
{
    bool ok;
    LOOP[index].injectionMark = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+tr("Injection Mark"),tr("Enter Injection Mark [L]"), LOOP[index].injectionMark, -10000, 10000, 2, &ok,Qt::WindowFlags(), 1);
     
    /// update json config file
    writeJsonConfigFile();
}


void
MainWindow::
injectionMethod(int index)
{
    bool ok;
    LOOP[index].injectionMethod = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+tr("Injection Method"),tr("Enter Injection Method: 201"), LOOP[index].injectionMethod, -10000, 10000, 2, &ok,Qt::WindowFlags(), 1);

    /// update json config file
    writeJsonConfigFile();
}


void
MainWindow::
onActionPressureSensorSlope(int index)
{
    bool ok;
    LOOP[index].pressureSensorSlope = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+tr("Pressure Sensor Slope"),tr("Enter Pressure Sensor Slope: 1.0"), LOOP[index].pressureSensorSlope, -10000, 10000, 2, &ok,Qt::WindowFlags(), 1);

    /// update json config file
    writeJsonConfigFile();
}

void
MainWindow::
onMinRefTemp(int index)
{
    bool ok;
    LOOP[index].minRefTemp = QInputDialog::getInt(this,"[LOOP "+QString::number(index+1)+"] "+tr("Minimum Reference Temperature"),tr("Enter Minimum Reference Temperature: 20"), LOOP[index].minRefTemp, 0, 1000, 2, &ok);

    /// update json config file
    writeJsonConfigFile();
}

void
MainWindow::
onMaxRefTemp(int index)
{
    bool ok;
    LOOP[index].maxRefTemp = QInputDialog::getInt(this,"[LOOP "+QString::number(index+1)+"] "+tr("Maximum Reference Temperature"),tr("Enter Maximum Reference Temperature: 20"), LOOP[index].maxRefTemp, 0, 1000, 2, &ok);

    /// update json config file
    writeJsonConfigFile();
}

void
MainWindow::
onInjectionTemp(int index)
{
    bool ok;
    LOOP[index].injectionTemp = QInputDialog::getInt(this,"[LOOP "+QString::number(index+1)+"] "+tr("Injection Temperature"),tr("Enter Injection Temperature: 20"), LOOP[index].injectionTemp, 0, 1000, 2, &ok);

    /// update json config file
    writeJsonConfigFile();
}


void
MainWindow::
onXDelay(int index)
{
    bool ok;
    LOOP[index].xDelay = QInputDialog::getInt(this,"[LOOP "+QString::number(index+1)+"] "+tr("X Delay"),tr("Enter Delay Peroid (seconds): 10"), LOOP[index].xDelay, 0, 3600, 2, &ok);

    /// update json config file
    writeJsonConfigFile();
}

void
MainWindow::
onYFreq(int index)
{
    bool ok;
    LOOP[index].yFreq = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+tr("Y Delta Frequency"),tr("Enter Y Delta Fequency (KHz): 1000000"), LOOP[index].yFreq, 0, 1000000, 2, &ok,Qt::WindowFlags(), 1);

    /// update json config file
    writeJsonConfigFile();
}


void
MainWindow::
onZTemp(int index)
{
    bool ok;
    LOOP[index].zTemp = QInputDialog::getDouble(this,"[LOOP "+QString::number(index+1)+"] "+tr("Z Delta Temperature"),tr("Enter Z Delta Temperature (°C): 0.1"), LOOP[index].zTemp, 0, 1000, 2, &ok,Qt::WindowFlags(), 1);

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
    setupModbusPort_2();
    setupModbusPort_3();
    setupModbusPort_4();
    setupModbusPort_5();
    setupModbusPort_6();
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


int
MainWindow::
setupModbusPort_2()
{
    QSettings s;

    int portIndex = 0;
    int i = 0;
    ui->comboBox_6->disconnect();
    ui->comboBox_6->clear();
    foreach( QextPortInfo port, QextSerialEnumerator::getPorts() )
    {
        ui->comboBox_6->addItem( port.friendName );
        if( port.friendName == s.value( "serialinterface" ) )
        {
            portIndex = i;
        }
        ++i;
    }
    ui->comboBox_6->setCurrentIndex( portIndex );
    ui->comboBox_7->setCurrentIndex(0);
    ui->comboBox_8->setCurrentIndex(0);
    ui->comboBox_9->setCurrentIndex(0);
    ui->comboBox_10->setCurrentIndex(0);

    connect( ui->comboBox_6, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_2( int ) ) );
    connect( ui->comboBox_7, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_2( int ) ) );
    connect( ui->comboBox_8, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_2( int ) ) );
    connect( ui->comboBox_9, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_2( int ) ) );
    connect( ui->comboBox_10, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_2( int ) ) );

    changeSerialPort_2( portIndex );
    return portIndex;
}

int
MainWindow::
setupModbusPort_3()
{
    QSettings s;

    int portIndex = 0;
    int i = 0;
    ui->comboBox_11->disconnect();
    ui->comboBox_11->clear();
    foreach( QextPortInfo port, QextSerialEnumerator::getPorts() )
    {
#ifdef Q_OS_WIN
        ui->comboBox_11->addItem( port.friendName );
#else
        ui->comboBox_11->addItem( port.physName );
#endif
        if( port.friendName == s.value( "serialinterface" ) )
        {
            portIndex = i;
        }
        ++i;
    }
    ui->comboBox_11->setCurrentIndex( portIndex );
    ui->comboBox_12->setCurrentIndex(0);
    ui->comboBox_13->setCurrentIndex(0);
    ui->comboBox_14->setCurrentIndex(0);
    ui->comboBox_15->setCurrentIndex(0);

    connect( ui->comboBox_11, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_3( int ) ) );
    connect( ui->comboBox_12, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_3( int ) ) );
    connect( ui->comboBox_13, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_3( int ) ) );
    connect( ui->comboBox_14, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_3( int ) ) );
    connect( ui->comboBox_15, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_3( int ) ) );

    changeSerialPort_3( portIndex );
    return portIndex;
}

int
MainWindow::
setupModbusPort_4()
{
    QSettings s;

    int portIndex = 0;
    int i = 0;
    ui->comboBox_16->disconnect();
    ui->comboBox_16->clear();
    foreach( QextPortInfo port, QextSerialEnumerator::getPorts() )
    {
#ifdef Q_OS_WIN
        ui->comboBox_16->addItem( port.friendName );
#else
        ui->comboBox_16->addItem( port.physName );
#endif
        if( port.friendName == s.value( "serialinterface" ) )
        {
            portIndex = i;
        }
        ++i;
    }
    ui->comboBox_16->setCurrentIndex( portIndex );

    ui->comboBox_17->setCurrentIndex(0);
    ui->comboBox_18->setCurrentIndex(0);
    ui->comboBox_19->setCurrentIndex(0);
    ui->comboBox_20->setCurrentIndex(0);

    connect( ui->comboBox_16, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_4( int ) ) );
    connect( ui->comboBox_17, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_4( int ) ) );
    connect( ui->comboBox_18, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_4( int ) ) );
    connect( ui->comboBox_19, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_4( int ) ) );
    connect( ui->comboBox_20, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_4( int ) ) );

    changeSerialPort_4( portIndex );
    return portIndex;
}

int
MainWindow::
setupModbusPort_5()
{
    QSettings s;

    int portIndex = 0;
    int i = 0;
    ui->comboBox_21->disconnect();
    ui->comboBox_21->clear();
    foreach( QextPortInfo port, QextSerialEnumerator::getPorts() )
    {
#ifdef Q_OS_WIN
        ui->comboBox_21->addItem( port.friendName );
#else
        ui->comboBox_21->addItem( port.physName );
#endif
        if( port.friendName == s.value( "serialinterface" ) )
        {
            portIndex = i;
        }
        ++i;
    }
    ui->comboBox_21->setCurrentIndex( portIndex );
    ui->comboBox_22->setCurrentIndex(0);
    ui->comboBox_23->setCurrentIndex(0);
    ui->comboBox_24->setCurrentIndex(0);
    ui->comboBox_25->setCurrentIndex(0);

    connect( ui->comboBox_21, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_5( int ) ) );
    connect( ui->comboBox_22, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_5( int ) ) );
    connect( ui->comboBox_23, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_5( int ) ) );
    connect( ui->comboBox_24, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_5( int ) ) );
    connect( ui->comboBox_25, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_5( int ) ) );

    changeSerialPort_5( portIndex );
    return portIndex;
}

int
MainWindow::
setupModbusPort_6()
{
    QSettings s;

    int portIndex = 0;
    int i = 0;
    ui->comboBox_26->disconnect();
    ui->comboBox_26->clear();
    foreach( QextPortInfo port, QextSerialEnumerator::getPorts() )
    {
#ifdef Q_OS_WIN
        ui->comboBox_26->addItem( port.friendName );
#else
        ui->comboBox_26->addItem( port.physName );
#endif
        if( port.friendName == s.value( "serialinterface" ) )
        {
            portIndex = i;
        }
        ++i;
    }
    ui->comboBox_26->setCurrentIndex( portIndex );
    ui->comboBox_27->setCurrentIndex(0);
    ui->comboBox_28->setCurrentIndex(0);
    ui->comboBox_29->setCurrentIndex(0);
    ui->comboBox_30->setCurrentIndex(0);

    connect( ui->comboBox_26, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_6( int ) ) );
    connect( ui->comboBox_27, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_6( int ) ) );
    connect( ui->comboBox_28, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_6( int ) ) );
    connect( ui->comboBox_29, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_6( int ) ) );
    connect( ui->comboBox_30, SIGNAL( currentIndexChanged( int ) ),this, SLOT( changeSerialPort_6( int ) ) );

    changeSerialPort_6( portIndex );
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
    if( LOOP[loop].serialModbus )
    {
        modbus_close( LOOP[loop].serialModbus );
        modbus_free( LOOP[loop].serialModbus );
        LOOP[loop].serialModbus = NULL;
        updateLoopTabIcon(loop, false);
    }
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

        changeModbusInterface(port, parity, L1);
        onRtuPortActive(true,L1);
    }
    else emit connectionError( tr( "No serial port found at Loop_1" ) );
}


void
MainWindow::
changeSerialPort_2( int )
{
    const int iface = ui->comboBox_6->currentIndex();

    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    if( !ports.isEmpty() )
    {
        QSettings settings;
        settings.setValue( "serialinterface", ports[iface].friendName );
        settings.setValue( "serialbaudrate", ui->comboBox_7->currentText() );
        settings.setValue( "serialparity", ui->comboBox_8->currentText() );
        settings.setValue( "serialdatabits", ui->comboBox_9->currentText() );
        settings.setValue( "serialstopbits", ui->comboBox_10->currentText() );
        QString port = ports[iface].portName;

        // is it a serial port in the range COM1 .. COM9?
        if ( port.startsWith( "COM" ) )
        {
            // use windows communication device name "\\.\COMn"
            port = "\\\\.\\" + port;
        }

        char parity;
        switch( ui->comboBox_8->currentIndex() )
        {
            case 1: parity = 'O'; break;
            case 2: parity = 'E'; break;
            default:
            case 0: parity = 'N'; break;
        }

        changeModbusInterface(port, parity, L2);
        onRtuPortActive(true, L2);
    }
    else emit connectionError( tr( "No serial port found at Loop_2" ) );
}


void
MainWindow::
changeSerialPort_3( int )
{
    const int iface = ui->comboBox_11->currentIndex();

    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    if( !ports.isEmpty() )
    {
        QSettings settings;
        settings.setValue( "serialinterface", ports[iface].friendName );
        settings.setValue( "serialbaudrate", ui->comboBox_12->currentText() );
        settings.setValue( "serialparity", ui->comboBox_13->currentText() );
        settings.setValue( "serialdatabits", ui->comboBox_14->currentText() );
        settings.setValue( "serialstopbits", ui->comboBox_15->currentText() );
        QString port = ports[iface].portName;

        // is it a serial port in the range COM1 .. COM9?
        if ( port.startsWith( "COM" ) )
        {
            // use windows communication device name "\\.\COMn"
            port = "\\\\.\\" + port;
        }

        char parity;
        switch( ui->comboBox_13->currentIndex() )
        {
            case 1: parity = 'O'; break;
            case 2: parity = 'E'; break;
            default:
            case 0: parity = 'N'; break;
        }

        changeModbusInterface(port, parity, L3);
        onRtuPortActive(true,L3);
    }
    else emit connectionError( tr( "No serial port found" ) );
}


void
MainWindow::
changeSerialPort_4( int )
{
    const int iface = ui->comboBox_16->currentIndex();

    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    if( !ports.isEmpty() )
    {
        QSettings settings;
        settings.setValue( "serialinterface", ports[iface].friendName );
        settings.setValue( "serialbaudrate", ui->comboBox_17->currentText() );
        settings.setValue( "serialparity", ui->comboBox_18->currentText() );
        settings.setValue( "serialdatabits", ui->comboBox_19->currentText() );
        settings.setValue( "serialstopbits", ui->comboBox_20->currentText() );

        QString port = ports[iface].portName;

        // is it a serial port in the range COM1 .. COM9?
        if ( port.startsWith( "COM" ) )
        {
            // use windows communication device name "\\.\COMn"
            port = "\\\\.\\" + port;
        }

        char parity;
        switch( ui->comboBox_18->currentIndex() )
        {
            case 1: parity = 'O'; break;
            case 2: parity = 'E'; break;
            default:
            case 0: parity = 'N'; break;
        }

        changeModbusInterface(port, parity, L4);
        onRtuPortActive(true,L4);
    }
    else emit connectionError( tr( "No serial port found" ) );
}


void
MainWindow::
changeSerialPort_5( int )
{
    const int iface = ui->comboBox_21->currentIndex();

    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    if( !ports.isEmpty() )
    {
        QSettings settings;
        settings.setValue( "serialinterface", ports[iface].friendName );
        settings.setValue( "serialbaudrate", ui->comboBox_22->currentText() );
        settings.setValue( "serialparity", ui->comboBox_23->currentText() );
        settings.setValue( "serialdatabits", ui->comboBox_24->currentText() );
        settings.setValue( "serialstopbits", ui->comboBox_25->currentText() );

        QString port = ports[iface].portName;

        // is it a serial port in the range COM1 .. COM9?
        if ( port.startsWith( "COM" ) )
        {
            // use windows communication device name "\\.\COMn"
            port = "\\\\.\\" + port;
        }

        char parity;
        switch( ui->comboBox_23->currentIndex() )
        {
            case 1: parity = 'O'; break;
            case 2: parity = 'E'; break;
            default:
            case 0: parity = 'N'; break;
        }

        changeModbusInterface(port, parity, L5);
        onRtuPortActive(true,L5);
    }
    else emit connectionError( tr( "No serial port found" ) );
}


void
MainWindow::
changeSerialPort_6( int )
{
    const int iface = ui->comboBox_26->currentIndex();

    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    if( !ports.isEmpty() )
    {
        QSettings settings;
        settings.setValue( "serialinterface", ports[iface].friendName );
        settings.setValue( "serialbaudrate", ui->comboBox_27->currentText() );
        settings.setValue( "serialparity", ui->comboBox_28->currentText() );
        settings.setValue( "serialdatabits", ui->comboBox_29->currentText() );
        settings.setValue( "serialstopbits", ui->comboBox_30->currentText() );

        QString port = ports[iface].portName;

        // is it a serial port in the range COM1 .. COM9?
        if ( port.startsWith( "COM" ) )
        {
            // use windows communication device name "\\.\COMn"
            port = "\\\\.\\" + port;
        }

        char parity;
        switch( ui->comboBox_28->currentIndex() )
        {
            case 1: parity = 'O'; break;
            case 2: parity = 'E'; break;
            default:
            case 0: parity = 'N'; break;
        }

        changeModbusInterface(port, parity, L6);
        onRtuPortActive(true,L6);
    }
    else emit connectionError( tr( "No serial port found" ) );
}


void
MainWindow::
changeModbusInterface(const QString& port, char parity, const int loop)
{
    releaseSerialModbus(loop);
    if (loop == L1) LOOP[loop].serialModbus = modbus_new_rtu( port.toLatin1().constData(),ui->comboBox_2->currentText().toInt(),parity,ui->comboBox_3->currentText().toInt(),ui->comboBox_4->currentText().toInt() );
    if (loop == L2) LOOP[loop].serialModbus = modbus_new_rtu( port.toLatin1().constData(),ui->comboBox_7->currentText().toInt(),parity,ui->comboBox_8->currentText().toInt(),ui->comboBox_9->currentText().toInt() );
    if (loop == L3) LOOP[loop].serialModbus = modbus_new_rtu( port.toLatin1().constData(),ui->comboBox_12->currentText().toInt(),parity,ui->comboBox_13->currentText().toInt(),ui->comboBox_14->currentText().toInt() );
    if (loop == L4) LOOP[loop].serialModbus = modbus_new_rtu( port.toLatin1().constData(),ui->comboBox_17->currentText().toInt(),parity,ui->comboBox_18->currentText().toInt(),ui->comboBox_19->currentText().toInt() );
    if (loop == L5) LOOP[loop].serialModbus = modbus_new_rtu( port.toLatin1().constData(),ui->comboBox_22->currentText().toInt(),parity,ui->comboBox_23->currentText().toInt(),ui->comboBox_24->currentText().toInt() );
    if (loop == L6) LOOP[loop].serialModbus = modbus_new_rtu( port.toLatin1().constData(),ui->comboBox_27->currentText().toInt(),parity,ui->comboBox_28->currentText().toInt(),ui->comboBox_29->currentText().toInt() );
            
    if( modbus_connect( LOOP[loop].serialModbus ) == -1 )
    {
        emit connectionError( tr( "Could not connect serial port at LOOP " )+QString::number(loop) );
        releaseSerialModbus(loop);
    }
    else
        updateLoopTabIcon(loop, true);
}


void
MainWindow::
onCheckBoxChecked(bool checked)
{
    clearMonitors();

    if (ui->tabWidget_2->currentIndex() == 0)
    {
        if (checked) setupModbusPort();
        else releaseSerialModbus(L1);

        onRtuPortActive(checked,L1);
    }
    else if (ui->tabWidget_2->currentIndex() == 1)
    {
        if (checked) setupModbusPort_2();
        else releaseSerialModbus(L2);

        onRtuPortActive(checked,L2);
    }   
    else if (ui->tabWidget_2->currentIndex() == 2)
    {
        if (checked) setupModbusPort_3();
        else releaseSerialModbus(L3);

        onRtuPortActive(checked,L3);
    }   
    else if (ui->tabWidget_2->currentIndex() == 3)
    {
        if (checked) setupModbusPort_4();
        else releaseSerialModbus(L4);

        onRtuPortActive(checked,L4);
    }   
    else if (ui->tabWidget_2->currentIndex() == 4)
    {
        if (checked) setupModbusPort_5();
        else releaseSerialModbus(L5);

        onRtuPortActive(checked,L5);
    }
    else
    {
        if (checked) setupModbusPort_6();
        else releaseSerialModbus(L6);

        onRtuPortActive(checked,L6);
    }   
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
    if (pipe == L1P1) (isF) ? ui->progressBar->setValue(value) : ui->progressBar_2->setValue(value);
    else if (pipe == L1P2) (isF) ? ui->progressBar_4->setValue(value) : ui->progressBar_3->setValue(value);
    else if (pipe == L1P3) (isF) ? ui->progressBar_6->setValue(value) : ui->progressBar_5->setValue(value);
    else if (pipe == L2P1) (isF) ? ui->progressBar_7->setValue(value) : ui->progressBar_8->setValue(value);
    else if (pipe == L2P2) (isF) ? ui->progressBar_10->setValue(value) : ui->progressBar_9->setValue(value);
    else if (pipe == L2P3) (isF) ? ui->progressBar_12->setValue(value) : ui->progressBar_11->setValue(value);
    else if (pipe == L3P1) (isF) ? ui->progressBar_13->setValue(value) : ui->progressBar_14->setValue(value);
    else if (pipe == L3P2) (isF) ? ui->progressBar_16->setValue(value) : ui->progressBar_15->setValue(value);
    else if (pipe == L3P3) (isF) ? ui->progressBar_18->setValue(value) : ui->progressBar_17->setValue(value);
    else if (pipe == L4P1) (isF) ? ui->progressBar_19->setValue(value) : ui->progressBar_20->setValue(value);
    else if (pipe == L4P2) (isF) ? ui->progressBar_22->setValue(value) : ui->progressBar_21->setValue(value);
    else if (pipe == L4P3) (isF) ? ui->progressBar_24->setValue(value) : ui->progressBar_23->setValue(value);
    else if (pipe == L5P1) (isF) ? ui->progressBar_25->setValue(value) : ui->progressBar_26->setValue(value);
    else if (pipe == L5P2) (isF) ? ui->progressBar_28->setValue(value) : ui->progressBar_27->setValue(value);
    else if (pipe == L5P3) (isF) ? ui->progressBar_30->setValue(value) : ui->progressBar_29->setValue(value);
    else if (pipe == L6P1) (isF) ? ui->progressBar_31->setValue(value) : ui->progressBar_32->setValue(value);
    else if (pipe == L6P2) (isF) ? ui->progressBar_34->setValue(value) : ui->progressBar_33->setValue(value);
    else if (pipe == L6P3) (isF) ? ui->progressBar_36->setValue(value) : ui->progressBar_35->setValue(value);
 
    else if (pipe == ALL) 
    {
        ui->progressBar->setValue(value);
        ui->progressBar_2->setValue(value);
        ui->progressBar_4->setValue(value);
        ui->progressBar_3->setValue(value);
        ui->progressBar_6->setValue(value);
        ui->progressBar_5->setValue(value);

        ui->progressBar_7->setValue(value);
        ui->progressBar_8->setValue(value);
        ui->progressBar_10->setValue(value);
        ui->progressBar_9->setValue(value);
        ui->progressBar_12->setValue(value);
        ui->progressBar_11->setValue(value);

        ui->progressBar_13->setValue(value);
        ui->progressBar_14->setValue(value);
        ui->progressBar_16->setValue(value);
        ui->progressBar_15->setValue(value);
        ui->progressBar_18->setValue(value);
        ui->progressBar_17->setValue(value);

        ui->progressBar_19->setValue(value);
        ui->progressBar_20->setValue(value);
        ui->progressBar_22->setValue(value);
        ui->progressBar_21->setValue(value);
        ui->progressBar_24->setValue(value);
        ui->progressBar_23->setValue(value);

        ui->progressBar_25->setValue(value);
        ui->progressBar_26->setValue(value);
        ui->progressBar_28->setValue(value);
        ui->progressBar_27->setValue(value);
        ui->progressBar_30->setValue(value);
        ui->progressBar_29->setValue(value);
 
        ui->progressBar_31->setValue(value);
        ui->progressBar_32->setValue(value);
        ui->progressBar_34->setValue(value);
        ui->progressBar_33->setValue(value);
        ui->progressBar_36->setValue(value);
        ui->progressBar_35->setValue(value);
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
    if (ui->tabWidget_2->currentIndex() == 0)
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
    else if (ui->tabWidget_2->currentIndex() == 1)
    {
        ui->groupBox_71->setEnabled(!isLow); // salt
        ui->groupBox_126->setEnabled(!isLow); // oil temp
        ui->groupBox_23->setEnabled(!isLow); // oil run

        if (isLow)
        {
            ui->groupBox_11->setTitle("WATERCUT [%]");
            ui->label_23->setText("HIGH");
            ui->label_24->setText("LOW");
            ui->lineEdit_41->setText("4");
            ui->lineEdit_42->setText("0");
        }
        else
        {
            ui->groupBox_11->setTitle("WATER RUN [%]");
            ui->label_23->setText("START");
            ui->label_24->setText("STOP");
            ui->lineEdit_41->setText("99");
            ui->lineEdit_42->setText("60");
        }
    }
    else if (ui->tabWidget_2->currentIndex() == 2)
    {
        ui->groupBox_72->setEnabled(!isLow); // salt
        ui->groupBox_133->setEnabled(!isLow); // oil temp
        ui->groupBox_45->setEnabled(!isLow); // oil run

        if (isLow)
        {
            ui->groupBox_44->setTitle("WATERCUT [%]");
            ui->label_46->setText("HIGH");
            ui->label_47->setText("LOW");
            ui->lineEdit_45->setText("4");
            ui->lineEdit_46->setText("0");
        }
        else
        {
            ui->groupBox_44->setTitle("WATER RUN [%]");
            ui->label_46->setText("START");
            ui->label_47->setText("STOP");
            ui->lineEdit_45->setText("99");
            ui->lineEdit_46->setText("60");
        }
    }
    else if (ui->tabWidget_2->currentIndex() == 3)
    {
        ui->groupBox_73->setEnabled(!isLow); // salt
        ui->groupBox_135->setEnabled(!isLow); // oil temp
        ui->groupBox_55->setEnabled(!isLow); // oil run

        if (isLow)
        {
            ui->groupBox_54->setTitle("WATERCUT [%]");
            ui->label_70->setText("HIGH");
            ui->label_71->setText("LOW");
            ui->lineEdit_49->setText("4");
            ui->lineEdit_50->setText("0");
        }
        else
        {
            ui->groupBox_54->setTitle("WATER RUN [%]");
            ui->label_70->setText("START");
            ui->label_71->setText("STOP");
            ui->lineEdit_49->setText("99");
            ui->lineEdit_50->setText("60");
        }
    }
    else if (ui->tabWidget_2->currentIndex() == 4)
    {
        ui->groupBox_74->setEnabled(!isLow); // salt
        ui->groupBox_137->setEnabled(!isLow); // oil temp
        ui->groupBox_75->setEnabled(!isLow); // oil run

        if (isLow)
        {
            ui->groupBox_64->setTitle("WATERCUT [%]");
            ui->label_83->setText("HIGH");
            ui->label_84->setText("LOW");
            ui->lineEdit_53->setText("4");
            ui->lineEdit_54->setText("0");
        }
        else
        {
            ui->groupBox_64->setTitle("WATER RUN [%]");
            ui->label_83->setText("START");
            ui->label_84->setText("STOP");
            ui->lineEdit_53->setText("99");
            ui->lineEdit_54->setText("60");
        }
    }
    else if (ui->tabWidget_2->currentIndex() == 5)
    {
        ui->groupBox_83->setEnabled(!isLow); // salt
        ui->groupBox_139->setEnabled(!isLow); // oil temp
        ui->groupBox_85->setEnabled(!isLow); // oil run

        if (isLow)
        {
            ui->groupBox_84->setTitle("WATERCUT [%]");
            ui->label_91->setText("HIGH");
            ui->label_92->setText("LOW");
            ui->lineEdit_57->setText("4");
            ui->lineEdit_58->setText("0");
        }
        else
        {
            ui->groupBox_84->setTitle("WATER RUN [%]");
            ui->label_91->setText("START");
            ui->label_92->setText("STOP");
            ui->lineEdit_57->setText("99");
            ui->lineEdit_58->setText("60");
        }
    }
}

void
MainWindow::
connectModeChanged()
{
    connect(ui->radioButton_6, SIGNAL(toggled(bool)), this, SLOT(onModeChanged(bool)));
    connect(ui->radioButton_11, SIGNAL(toggled(bool)), this, SLOT(onModeChanged(bool)));
    connect(ui->radioButton_17, SIGNAL(toggled(bool)), this, SLOT(onModeChanged(bool)));
    connect(ui->radioButton_23, SIGNAL(toggled(bool)), this, SLOT(onModeChanged(bool)));
    connect(ui->radioButton_29, SIGNAL(toggled(bool)), this, SLOT(onModeChanged(bool)));
    connect(ui->radioButton_35, SIGNAL(toggled(bool)), this, SLOT(onModeChanged(bool)));
}


void
MainWindow::
onProductSelected(const bool prd)
{
    if (ui->tabWidget_2->currentIndex() == 0) updateRegisters(prd,L1);
    else if (ui->tabWidget_2->currentIndex() == 1) updateRegisters(prd,L2);
    else if (ui->tabWidget_2->currentIndex() == 2) updateRegisters(prd,L3);
    else if (ui->tabWidget_2->currentIndex() == 3) updateRegisters(prd,L4);
    else if (ui->tabWidget_2->currentIndex() == 4) updateRegisters(prd,L5);
    else if (ui->tabWidget_2->currentIndex() == 5) updateRegisters(prd,L6);
}


void
MainWindow::
connectProductBtnPressed()
{
    connect(ui->radioButton, SIGNAL(toggled(bool)), this, SLOT(onProductSelected(bool)));
    connect(ui->radioButton_15, SIGNAL(toggled(bool)), this, SLOT(onProductSelected(bool)));
    connect(ui->radioButton_21, SIGNAL(toggled(bool)), this, SLOT(onProductSelected(bool)));
    connect(ui->radioButton_27, SIGNAL(toggled(bool)), this, SLOT(onProductSelected(bool)));
    connect(ui->radioButton_33, SIGNAL(toggled(bool)), this, SLOT(onProductSelected(bool)));
    connect(ui->radioButton_39, SIGNAL(toggled(bool)), this, SLOT(onProductSelected(bool)));
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
        for (int i = 0; i < MAX_LOOP; i++) (connected) ? ui->tabWidget_2->setTabIcon(i,icon) : ui->tabWidget_2->setTabIcon(i,icoff);
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
    if (loop == L1) LOOP[loop].isCal ? ui->pushButton_4->setText("S T O P") : ui->pushButton_4->setText("S T A R T");
    else if (loop == L2) LOOP[loop].isCal ? ui->pushButton_9->setText("S T O P") : ui->pushButton_9->setText("S T A R T");
    else if (loop == L3) LOOP[loop].isCal ? ui->pushButton_12->setText("S T O P") : ui->pushButton_12->setText("S T A R T");
    else if (loop == L4) LOOP[loop].isCal ? ui->pushButton_15->setText("S T O P") : ui->pushButton_15->setText("S T A R T");
    else if (loop == L5) LOOP[loop].isCal ? ui->pushButton_18->setText("S T O P") : ui->pushButton_18->setText("S T A R T");
    else if (loop == L6) LOOP[loop].isCal ? ui->pushButton_21->setText("S T O P") : ui->pushButton_21->setText("S T A R T");
}

void
MainWindow::
initializeTabIcons()
{
    QIcon icon(QLatin1String(":/red.ico"));
    for (int i = 0; i < 6; i++) ui->tabWidget_2->setTabIcon(i,icon);
}


void
MainWindow::
createCalibrateFile(const int sn, const int pipe, const QString startValue, const QString stopValue, const QString saltValue, const QString filename)
{
    QDateTime currentDataTime = QDateTime::currentDateTime();

    /// headers 
    QString header0;
    LOOP[pipe/3].isEEA ? header0 = EEA_INJECTION_FILE : header0 = RAZ_INJECTION_FILE;
    QString header1("SN"+QString::number(sn)+" | "+LOOP[pipe/3].mode.split("\\").at(1) +" | "+currentDataTime.toString()+" | "+PIPE[pipe].pipeId+" | "+PROJECT+RELEASE_VERSION); 
    QString header2("INJECTION:  "+startValue+" % "+"to "+stopValue+" % "+"Watercut at "+saltValue+" % "+"Salinity\n");
    QString header3 = HEADER3;
    QString header4 = HEADER4;
    QString header5 = HEADER5;

	/// CALIBRAT, ADJUSTED, ROLLOVER (LOWCUT ONLY)
    if (LOOP[pipe/3].mode == LOW)
    {
        QString rolloverValue;
        QString header21;
        QString header22;

        /// create headers
        header2 = "TEMPERATURE:  "+startValue+" °C "+"to "+stopValue+" °C\n";
        header21 = "INJECTION:  "+LOOP[pipe/3].waterRunStop->text()+" % "+"to "+LOOP[pipe/3].waterRunStart->text()+" % Watercut\n";
        header22 = "ROLLOVER:  "+rolloverValue+" % "+"to "+"rollover\n";

		/// set filenames
		PIPE[pipe].fileCalibrate.setFileName(PIPE[pipe].mainDirPath+"\\"+QString("CALIBRAT").append(LOOP[pipe/3].calExt));
		PIPE[pipe].fileAdjusted.setFileName(PIPE[pipe].mainDirPath+"\\"+QString("ADJUSTED").append(LOOP[pipe/3].adjExt));
		PIPE[pipe].fileRollover.setFileName(PIPE[pipe].mainDirPath+"\\"+QString("ROLLOVER").append(LOOP[pipe/3].rolExt));

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
				updateFileList(QFileInfo(PIPE[pipe].fileCalibrate).fileName(), sn, LOOP[pipe/3].mode, pipe);
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
				updateFileList(QFileInfo(PIPE[pipe].fileAdjusted).fileName(), sn, LOOP[pipe/3].mode, pipe);
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
				updateFileList(QFileInfo(PIPE[pipe].fileRollover).fileName(), sn, LOOP[pipe/3].mode, pipe);
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

    LOOP[pipe/3].isEEA ? header0 = EEA_INJECTION_FILE : header0 = RAZ_INJECTION_FILE;
    QString header1("SN"+QString::number(sn)+" | "+LOOP[pipe/3].mode.split("\\").at(1) +" | "+currentDataTime.toString()+" | "+PIPE[pipe].pipeId+" | "+PROJECT+RELEASE_VERSION); 
 
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
    QString fileName = QString(startValue).append("_").append(stopValue).append(LOOP[pipe/3].filExt);

    /// headers 
    QString header0;
    LOOP[pipe/3].isEEA ? header0 = EEA_INJECTION_FILE : header0 = RAZ_INJECTION_FILE;
    QString header1("SN"+QString::number(sn)+" | "+LOOP[pipe/3].mode.split("\\").at(1) +" | "+currentDataTime.toString()+" | "+PIPE[pipe].pipeId+" | "+PROJECT+RELEASE_VERSION); 
    QString header2("INJECTION:  "+startValue+" % "+"to "+stopValue+" % "+"Watercut at "+saltValue+" % "+"Salinity\n");
    if (LOOP[pipe/3].mode == LOW) header2 = "TEMPERATURE:  "+startValue+" °C "+"to "+stopValue+" °C\n";
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
	/// set mode
	if (loop == L1)
	{
     	/// product
       	LOOP[L1].isEEA = ui->radioButton->isChecked();  
    
		/// mode
       	if (ui->radioButton_3->isChecked()) LOOP[L1].mode = HIGH;
       	else if (ui->radioButton_4->isChecked()) LOOP[L1].mode = FULL;
       	else if (ui->radioButton_5->isChecked()) LOOP[L1].mode = MID;
       	else if (ui->radioButton_6->isChecked()) LOOP[L1].mode = LOW;
	}
	else if (loop == L2)
	{
		/// product
	    LOOP[L2].isEEA = ui->radioButton_15->isChecked();  

		/// mode
       	if (ui->radioButton_12->isChecked()) LOOP[L2].mode = HIGH;
       	else if (ui->radioButton_13->isChecked()) LOOP[L2].mode = FULL;
       	else if (ui->radioButton_14->isChecked()) LOOP[L2].mode = MID;
     	else if (ui->radioButton_15->isChecked()) LOOP[L2].mode = LOW;
	}
	else if (loop == L3)
	{
       	/// product
       	LOOP[L3].isEEA = ui->radioButton_21->isChecked();  

 		/// mode
       	if (ui->radioButton_18->isChecked()) LOOP[L3].mode = HIGH;
       	else if (ui->radioButton_19->isChecked()) LOOP[L3].mode = FULL;
       	else if (ui->radioButton_20->isChecked()) LOOP[L3].mode = MID;
       	else if (ui->radioButton_17->isChecked()) LOOP[L3].mode = LOW;
	}
	else if (loop == L4)
	{
		/// product
       	LOOP[L4].isEEA = ui->radioButton_27->isChecked();  

       	/// mode
       	if (ui->radioButton_24->isChecked()) LOOP[L4].mode = HIGH;
       	else if (ui->radioButton_25->isChecked()) LOOP[L4].mode = FULL;
       	else if (ui->radioButton_26->isChecked()) LOOP[L4].mode = MID;
       	else if (ui->radioButton_23->isChecked()) LOOP[L4].mode = LOW;
	}
	else if (loop == L4)
	{
		/// product
       	LOOP[L5].isEEA = ui->radioButton_33->isChecked();  
        	
		/// mode
       	if (ui->radioButton_30->isChecked()) LOOP[L5].mode = HIGH;
       	else if (ui->radioButton_31->isChecked()) LOOP[L5].mode = FULL;
       	else if (ui->radioButton_32->isChecked()) LOOP[L5].mode = MID;
       	else if (ui->radioButton_29->isChecked()) LOOP[L5].mode = LOW;
	}
	else if (loop == L6)
	{
		/// product
       	LOOP[L6].isEEA = ui->radioButton_39->isChecked(); 

       	/// mode
       	if (ui->radioButton_36->isChecked()) LOOP[L6].mode = HIGH;
       	else if (ui->radioButton_37->isChecked()) LOOP[L6].mode = FULL;
       	else if (ui->radioButton_38->isChecked()) LOOP[L6].mode = MID;
       	else if (ui->radioButton_35->isChecked()) LOOP[L6].mode = LOW;
	}

	/// set file extensions
	if (LOOP[loop].mode == HIGH) 
	{
		LOOP[loop].filExt = ".HCI";
		LOOP[loop].calExt = ".HCI";
		LOOP[loop].adjExt = ".HCI";
		LOOP[loop].rolExt = ".HCR";
	}
	else if (LOOP[loop].mode == FULL) 
	{
		LOOP[loop].filExt = ".FCI";
		LOOP[loop].calExt = ".FCI";
		LOOP[loop].adjExt = ".FCI";
		LOOP[loop].rolExt = ".FCR";
	}
	else if (LOOP[loop].mode == MID) 
	{
		LOOP[loop].filExt = ".MCI";
		LOOP[loop].calExt = ".MCI";
		LOOP[loop].adjExt = ".MCI";
		LOOP[loop].rolExt = ".MCR";
	}
	else if (LOOP[loop].mode == LOW) 
	{
		LOOP[loop].filExt = ".LCT";
		LOOP[loop].calExt = ".LCI";
		LOOP[loop].adjExt = ".LCI";
		LOOP[loop].rolExt = ".LCR";
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

	for (int pipe = loop*3; pipe < loop*3+3; pipe++)
	{
		/// slave id exists
		if (PIPE[pipe].slave->text().isEmpty()) PIPE[pipe].isCal = false;
		else
		{
			/// set slave
   			memset( dest, 0, 1024 );
   			modbus_set_slave( serialModbus, PIPE[pipe].slave->text().toInt());

       		/// read pipe serial number
   			sendCalibrationRequest(FLOAT_R, serialModbus, FUNC_READ_INT, LOOP[loop].ID_SN_PIPE, BYTE_READ_INT, ret, dest, dest16, is16Bit, writeAccess, funcType);

			/// verify if serial number matches with pipe
   			if (*dest16 != PIPE[pipe].slave->text().toInt()) 
			{
       			displayMessage(QString("LOOP ")+QString::number(loop + 1),QString("LOOP ")+QString::number(loop + 1)+QString(" PIPE ")+QString::number(pipe + 1),"Invalid Serial Number!");

				/// lock FCT registers
   				modbus_write_register(serialModbus,999,0);

				/// cancel calibration
				LOOP[loop].isCal = false;

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
	/// pipe sn exists?
	if (PIPE[loop*3].slave->text().isEmpty() && PIPE[loop*3+1].slave->text().isEmpty() && PIPE[loop*3+2].slave->text().isEmpty()) 
	{
       	displayMessage(QString("LOOP ")+QString::number(loop + 1),QString("LOOP ")+QString::number(loop + 1),"No valid serial number found");
		return false;
	}

	/// toggle start button
    LOOP[loop].isCal = !LOOP[loop].isCal;

	/// calibration on
	if (LOOP[loop].isCal)
	{
		/// set product & calibration mode
		setProductAndCalibrationMode(loop);

    	/// update product specific register IDs
    	updateRegisters(LOOP[loop].isEEA,loop);

		/// pipe specific vars
		for (int pipe = loop*3; pipe < loop*3 + 3; pipe++)
		{
			PIPE[pipe].tempStability = 0;
   			PIPE[pipe].freqStability = 0;
   			PIPE[pipe].etimer->restart();
   			PIPE[pipe].mainDirPath = m_mainServer+LOOP[loop].mode+PIPE[pipe].slave->text(); 

			if (LOOP[loop].mode == LOW) prepareForNextFile(pipe, QString("AMB").append("_").append(QString::number(LOOP[loop].minRefTemp)).append(LOOP[loop].filExt));

			if (ui->radioButton_7->isChecked()) PIPE[pipe].osc = 1;
   			else if (ui->radioButton_8->isChecked()) PIPE[pipe].osc = 2;
   			else if (ui->radioButton_9->isChecked()) PIPE[pipe].osc = 3;
   			else PIPE[pipe].osc = 4;
		}

    	/// validate loop connect
    	if (LOOP[loop].modbus == NULL)
    	{
        	/// calibration fails
        	LOOP[loop].isCal = false;

        	/// update tab icon
        	updateStartButtonLabel(loop);

        	displayMessage(QString("LOOP ")+QString::number(loop + 1),QString("LOOP ")+QString::number(loop + 1),"Bad Serial Connection");

        	/// exit
        	return LOOP[loop].isCal;
    	}

		/// validate slave ids
    	if (!validateSerialNumber(loop, LOOP[loop].serialModbus))
		{
			/// reset LOOP[loop].isCal
			LOOP[loop].isCal = false;

        	/// update tab icon
        	updateStartButtonLabel(loop);

        	/// exit
        	return LOOP[loop].isCal;
		}
	}

	return true;
}


void
MainWindow::
onCalibrationButtonPressed(int loop)
{
    /// scan calibration variables
    if (!prepareCalibration(loop)) return;
	
    /// LOOP calibration on
    if (LOOP[loop].isCal)
    {
		for (int pipe = loop*3; pipe < loop*3+3; pipe++)
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
                   			break;
               			}
               			else fileCounter++;
           			}
       			}

				if (pipe == L1P1) 
				{
					future_L1P1 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L1P1.waitForFinished();
				}
				else if (pipe == L1P2) 
				{
					future_L1P2 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L1P2.waitForFinished();
				}
				else if (pipe == L1P3) 
				{
					future_L1P3 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L1P3.waitForFinished();
				}
				else if (pipe == L2P1) 
				{
					future_L2P1 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L2P1.waitForFinished();
				}
				else if (pipe == L2P2) 
				{
					future_L2P2 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L2P2.waitForFinished();
				}
				else if (pipe == L2P3) 
				{
					future_L2P3 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L2P3.waitForFinished();
				}
				else if (pipe == L3P1) 
				{
					future_L3P1 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L3P1.waitForFinished();
				}
				else if (pipe == L3P2) 
				{
					future_L3P2 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L3P2.waitForFinished();
				}
				else if (pipe == L3P3) 
				{
					future_L3P3 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L3P3.waitForFinished();
				}
				else if (pipe == L4P1) 
				{
					future_L4P1 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L4P1.waitForFinished();
				}
				else if (pipe == L4P2) 
				{
					future_L4P2 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L4P2.waitForFinished();
				}
				else if (pipe == L4P3) 
				{
					future_L4P3 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L4P3.waitForFinished();
				}
				else if (pipe == L5P1) 
				{
					future_L5P1 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L5P1.waitForFinished();
				}
				else if (pipe == L5P2) 
				{
					future_L5P2 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L5P2.waitForFinished();
				}
				else if (pipe == L5P3) 
				{
					future_L5P3 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L5P3.waitForFinished();
				}
				else if (pipe == L6P1) 
				{
					future_L6P1 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L6P1.waitForFinished();
				}
				else if (pipe == L6P2) 
				{
					future_L6P2 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L6P2.waitForFinished();
				}
				else if (pipe == L6P3) 
				{
					future_L6P3 = QtConcurrent::run([=]() {calibrate(pipe);});
					future_L6P3.waitForFinished();
				}
			}
    	}
	}
	else
    {
		/// LOOP[loop].isCal == FALSE
		stopCalibration(loop*3);
		stopCalibration(loop*3+1);
		stopCalibration(loop*3+2);
	}

	/// update pipe indicator 
    updateStartButtonLabel(loop);
}		


void
MainWindow::
stopCalibration(int pipe)
{
	if (pipe == L1P1) 
	{
		if (future_L1P1.isStarted()) future_L1P1.cancel();
	}
	else if (pipe == L1P2) 
	{
		if (future_L1P2.isStarted()) future_L1P2.cancel();
	}
	else if (pipe == L1P3) 
	{
		if (future_L1P3.isStarted()) future_L1P3.cancel();
	}
	else if (pipe == L2P1) 
	{
		if (future_L2P1.isStarted()) future_L2P1.cancel();
	}
	else if (pipe == L2P2) 
	{
		if (future_L2P2.isStarted()) future_L2P2.cancel();
	}
	else if (pipe == L2P3) 
	{
		if (future_L2P3.isStarted()) future_L2P3.cancel();
	}
	else if (pipe == L3P1) 
	{
		if (future_L3P1.isStarted()) future_L3P1.cancel();
	}
	else if (pipe == L3P2) 
	{
		if (future_L3P2.isStarted()) future_L3P2.cancel();
	}
	else if (pipe == L3P3) 
	{
		if (future_L3P3.isStarted()) future_L3P3.cancel();
	}
	else if (pipe == L4P1) 
	{
		if (future_L4P1.isStarted()) future_L4P1.cancel();
	}
	else if (pipe == L4P2) 
	{
		if (future_L4P2.isStarted()) future_L4P2.cancel();
	}
	else if (pipe == L4P3) 
	{
		if (future_L4P3.isStarted()) future_L4P3.cancel();
	}
	else if (pipe == L5P1) 
	{
		if (future_L5P1.isStarted()) future_L5P1.cancel();
	}
	else if (pipe == L5P2) 
	{
		if (future_L5P2.isStarted()) future_L5P2.cancel();
	}
	else if (pipe == L5P3) 
	{
		if (future_L5P3.isStarted()) future_L5P3.cancel();
	}
	else if (pipe == L6P1) 
	{
		if (future_L6P1.isStarted()) future_L6P1.cancel();
	}
	else if (pipe == L6P2) 
	{
		if (future_L6P2.isStarted()) future_L6P2.cancel();
	}
	else if (pipe == L6P3) 
	{
		if (future_L6P3.isStarted()) future_L6P3.cancel();
	}
}


void
MainWindow::
calibrate(const int pipe)
{
	static int injectionTime = 0;
	static int totalInjectionTime = 0;
	static double nextWatercut = 0;
	int loop = pipe/3;
    uint8_t dest[1024];
    uint16_t * dest16 = (uint16_t *) dest;
    int ret = -1;
    bool is16Bit = false;
    bool writeAccess = false;
    const QString funcType = descriptiveDataTypeName( FUNC_READ_FLOAT );

    /// reset connection
    memset( dest, 0, 1024 );
    modbus_set_slave( LOOP[loop].serialModbus, PIPE[pipe].slave->text().toInt() );
    
    /// get elapsed time
    int elapsedTime = PIPE[pipe].etimer->elapsed();

    /// update pipe tab icon and control variables
    updateStartButtonLabel(loop); 

    /// calibration enabled? 
    if (LOOP[loop].isCal)
    {
        /// get watercut 
        PIPE[pipe].watercut = sendCalibrationRequest(FLOAT_R, LOOP[loop].serialModbus, FUNC_READ_FLOAT, LOOP[loop].ID_WATERCUT, BYTE_READ_FLOAT, ret, dest, dest16, is16Bit, writeAccess, funcType);
        QThread::msleep(SLEEP_TIME);

        /// get temperature
        PIPE[pipe].temperature = sendCalibrationRequest(FLOAT_R, LOOP[loop].serialModbus, FUNC_READ_FLOAT, LOOP[loop].ID_TEMPERATURE, BYTE_READ_FLOAT, ret, dest, dest16, is16Bit, writeAccess, funcType);

		/// check temp stability
        if (PIPE[pipe].tempStability < 5) 
		{
			if (abs(PIPE[pipe].temperature - PIPE[pipe].temperature_prev) <= LOOP[loop].zTemp) PIPE[pipe].tempStability++;
			else PIPE[pipe].tempStability = 0;

			PIPE[pipe].temperature_prev = PIPE[pipe].temperature;
		}
		else PIPE[pipe].tempStability = 5;

        updateStability(T_BAR, pipe, PIPE[pipe].tempStability*20);
        QThread::msleep(SLEEP_TIME);

        /// get frequency
        PIPE[pipe].frequency = sendCalibrationRequest(FLOAT_R, LOOP[loop].serialModbus, FUNC_READ_FLOAT, LOOP[loop].ID_FREQ, BYTE_READ_FLOAT, ret, dest, dest16, is16Bit, writeAccess, funcType);

		/// check freq stability
		if (PIPE[pipe].freqStability < 5) 
		{
        	if (abs(PIPE[pipe].frequency - PIPE[pipe].frequency_prev) <= LOOP[loop].yFreq) PIPE[pipe].freqStability++;
			else PIPE[pipe].freqStability = 0;

        	PIPE[pipe].frequency_prev = PIPE[pipe].frequency;
		}
		else PIPE[pipe].freqStability = 5;

        updateStability(F_BAR, pipe, PIPE[pipe].freqStability*20);
        QThread::msleep(SLEEP_TIME);

        /// get oil_rp 
        PIPE[pipe].oilrp = sendCalibrationRequest(FLOAT_R, LOOP[loop].serialModbus, FUNC_READ_FLOAT, LOOP[loop].ID_OIL_RP, BYTE_READ_FLOAT, ret, dest, dest16, is16Bit, writeAccess, funcType);
        QThread::msleep(SLEEP_TIME);

        /// get measured ai
        PIPE[pipe].measai = sendCalibrationRequest(FLOAT_R, LOOP[loop].serialModbus, FUNC_READ_FLOAT, RAZ_MEAS_AI, BYTE_READ_FLOAT, ret, dest, dest16, is16Bit, writeAccess, funcType);
        QThread::msleep(SLEEP_TIME);

        /// get trimmed ai
        PIPE[pipe].trimai = sendCalibrationRequest(FLOAT_R, LOOP[loop].serialModbus, FUNC_READ_FLOAT, RAZ_TRIM_AI, BYTE_READ_FLOAT, ret, dest, dest16, is16Bit, writeAccess, funcType);
        QThread::msleep(SLEEP_TIME);

        /// update pipe reading
        updatePipeReading(pipe, PIPE[pipe].watercut, PIPE[pipe].frequency, PIPE[pipe].frequency, PIPE[pipe].temperature, PIPE[pipe].oilrp);

        /// read data 
        QString data_stream;
		data_stream = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14").arg(elapsedTime, 9, 'g', -1, ' ').arg(PIPE[pipe].watercut,7,'f',2,' ').arg(PIPE[pipe].osc, 4, 'g', -1, ' ').arg(" INT").arg(1, 7, 'g', -1, ' ').arg(PIPE[pipe].frequency,9,'f',2,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].oilrp,9,'f',2,' ').arg(PIPE[pipe].temperature,11,'f',2,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].measai,12,'f',2,' ').arg(PIPE[pipe].trimai,12,'f',2,' ').arg(0,10,'f',2,' ').arg(0,12,'f',2,' ');

		///////////////////////////
		///////////////////////////
        /// LOWCUT
		///////////////////////////
		///////////////////////////
		
        if (LOOP[loop].mode == LOW)
        {
			/// AMB_"LOOP[loop].minRefTemp".LCT
            if (QFileInfo(PIPE[pipe].file).fileName() == QString("AMB").append("_").append(QString::number(LOOP[loop].minRefTemp)).append(LOOP[loop].filExt))
            {
                /// validate run condition 
                if (PIPE[pipe].checkBox->isChecked() && ((PIPE[pipe].tempStability != 5) || (PIPE[pipe].freqStability != 5)))
                {
                    /// create a new file if needed
                    if (!QFileInfo(PIPE[pipe].file).exists()) 
                    {
						/// reset injectionTime
						injectionTime = 0;
						totalInjectionTime = 0;
						nextWatercut = 0.25;

                        /// enter measured initial watercut 
                        bool ok;
                        QString msg = PIPE[pipe].slave->text().append(": Enter Measured Initial Watercut");
                        LOOP[loop].waterRunStop->setText(QInputDialog::getText(this, QString("LOOP ")+QString::number(pipe/3 + 1)+QString(", PIPE ")+QString::number(pipe%3+1),tr(qPrintable(msg)), QLineEdit::Normal,"0.0", &ok));

						/// update data_stream with the user entered initial watercut
						data_stream = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14").arg(elapsedTime, 9, 'g', -1, ' ').arg(LOOP[loop].waterRunStop->text().toDouble(),7,'f',2,' ').arg(PIPE[pipe].osc, 4, 'g', -1, ' ').arg(" INT").arg(1, 7, 'g', -1, ' ').arg(PIPE[pipe].frequency,9,'f',2,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].oilrp,9,'f',2,' ').arg(PIPE[pipe].temperature,11,'f',2,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].measai,12,'f',2,' ').arg(PIPE[pipe].trimai,12,'f',2,' ').arg(0,10,'f',2,' ').arg(0,12,'f',2,' ');

                        /// indicate the operator to set temp to LOOP[loop].minRefTemp C degrees
                        if (getUserInputMessage(QString("LOOP ")+QString::number(loop + 1)+QString(", PIPE ")+QString::number(pipe%3+1), PIPE[pipe].slave->text().append(": Set The Heat Exchanger Temperature"), QString::number(LOOP[loop].minRefTemp).append("°C"))) createLoopFile(PIPE[pipe].slave->text().toInt(), LOOP[loop].mode, "AMB", QString::number(LOOP[loop].minRefTemp), LOOP[loop].saltStop->currentText(), pipe);
                        else 
						{
							stopCalibration(pipe);
							return;
						}

                        /// indicate the operator to fill the water container to the mark 
                		displayMessage(QString("LOOP ")+QString::number(pipe/3 + 1)+QString(", PIPE ")+QString::number(pipe%3+1),QString("Serial Number: ")+PIPE[pipe].slave->text()+QString("                              "),"Fill the water container to the mark");
                    }

                    writeToCalFile(pipe, data_stream);
                }
                else 
                {
                    prepareForNextFile(pipe, QString::number(LOOP[loop].minRefTemp).append("_").append(QString::number(LOOP[loop].maxRefTemp)).append(LOOP[loop].filExt));
                }
            }
            else if (QFileInfo(PIPE[pipe].file).fileName() == QString::number(LOOP[loop].minRefTemp).append("_").append(QString::number(LOOP[loop].maxRefTemp)).append(LOOP[loop].filExt))
            {
                if (PIPE[pipe].checkBox->isChecked() && ((PIPE[pipe].tempStability != 5) || (PIPE[pipe].freqStability != 5)))
                {
                    if (!QFileInfo(PIPE[pipe].file).exists()) 
                    {
                        if (getUserInputMessage(QString("LOOP ")+QString::number(pipe/3 + 1)+QString(", PIPE ")+QString::number(pipe%3+1), PIPE[pipe].slave->text().append(": Set Heat Exchanger Temperature"),QString::number(LOOP[loop].maxRefTemp).append("°C"))) createLoopFile(PIPE[pipe].slave->text().toInt(), LOOP[loop].mode, QString::number(LOOP[loop].minRefTemp), QString::number(LOOP[loop].maxRefTemp), LOOP[loop].saltStop->currentText(), pipe);
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
                    prepareForNextFile(pipe, QString::number(LOOP[loop].maxRefTemp).append("_").append(QString::number(LOOP[loop].injectionTemp)).append(LOOP[loop].filExt));
                }
            }
            else if (QFileInfo(PIPE[pipe].file).fileName() == QString::number(LOOP[loop].maxRefTemp).append("_").append(QString::number(LOOP[loop].injectionTemp)).append(LOOP[loop].filExt))
            {
                if (PIPE[pipe].checkBox->isChecked() && ((PIPE[pipe].tempStability != 5) || (PIPE[pipe].freqStability != 5)))
                {
                    if (!QFileInfo(PIPE[pipe].file).exists()) 
                    {
                        if (getUserInputMessage(QString("LOOP ")+QString::number(pipe/3 + 1)+QString(", PIPE ")+QString::number(pipe%3+1), PIPE[pipe].slave->text().append(": Set Heat Exchanger Temperature"),QString::number(LOOP[loop].injectionTemp).append("°C"))) createLoopFile(PIPE[pipe].slave->text().toInt(), LOOP[loop].mode, QString::number(LOOP[loop].maxRefTemp), QString::number(LOOP[loop].injectionTemp), LOOP[loop].saltStop->currentText(), pipe);
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
			/// start real calibration 
			///////////////////////////////////
			///////////////////////////////////
			
            else if (QFileInfo(PIPE[pipe].file).fileName() == "CALIBRAT.LCI") // CALIBRAT.LCI 
            {   
				/// waterRunStart is actually the MAX watercut value in LOWCUT mode 
                if (PIPE[pipe].checkBox->isChecked() && ((PIPE[pipe].tempStability != 5) || (PIPE[pipe].freqStability != 5))) // run?
                {
                    /// create a new file if needed
        			if (!QFileInfo(PIPE[pipe].file).exists()) 
                    {
                        bool ok;
                        QString msg = PIPE[pipe].slave->text().append(": Enter Measured Initial Watercut");
                        LOOP[loop].waterRunStop->setText(QInputDialog::getText(this, QString("LOOP ")+QString::number(pipe/3 + 1)+QString(", PIPE ")+QString::number(pipe%3+1),tr(qPrintable(msg)), QLineEdit::Normal,"0.0", &ok));

						data_stream = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14").arg(elapsedTime, 9, 'g', -1, ' ').arg(LOOP[loop].waterRunStop->text().toDouble(),7,'f',2,' ').arg(PIPE[pipe].osc, 4, 'g', -1, ' ').arg(" INT").arg(1, 7, 'g', -1, ' ').arg(PIPE[pipe].frequency,9,'f',2,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].oilrp,9,'f',2,' ').arg(PIPE[pipe].temperature,11,'f',2,' ').arg(0,8,'f',2,' ').arg(PIPE[pipe].measai,12,'f',2,' ').arg(PIPE[pipe].trimai,12,'f',2,' ').arg(0,10,'f',2,' ').arg(0,12,'f',2,' ');

						createCalibrateFile(PIPE[pipe].slave->text().toInt(), pipe, LOOP[pipe/3].waterRunStop->text(), LOOP[pipe/3].waterRunStart->text(), 0, "CALIBRAT");
                    }

                    writeToCalFile(pipe, data_stream);

					///
					/// calculate next injection time and accumulate totalInjectionTime
					///
					injectionTime = -(LOOP[loop].loopVolume->text().toDouble()/LOOP[loop].injectionWaterPumpRate)*log((1-(nextWatercut - LOOP[loop].waterRunStop->text().toDouble())/100)) - injectionTime;
					nextWatercut += 0.25;
					totalInjectionTime += injectionTime;

					///
					/// inject water to the pipe for "injectionTime" seconds
					///
				 	injectWater(true);					 // start injecting water
        			QThread::msleep(injectionTime*1000); // wait for "injectionTime" secs
					injectWater(false);					 // stop injecting water
                }
                else 
                {
					bool ok;
					QString msg;

					///
                    /// enter measured watercut
					///
                    msg = PIPE[pipe].slave->text().append(": Enter Measured Watercut");
                    double measuredWatercut = QInputDialog::getDouble(this, QString("LOOP ")+QString::number(pipe/3 + 1)+QString(", PIPE ")+QString::number(pipe%3+1),tr(qPrintable(msg)), 0.0, 0, 100, 2, &ok,Qt::WindowFlags(), 1);

					///
                    /// ask user total injected volume 
					///
					msg = PIPE[pipe].slave->text().append(": Enter Injected Volume");
	                double injectedVolume = QInputDialog::getDouble(this, QString("LOOP ")+QString::number(pipe/3 + 1)+QString(", PIPE ")+QString::number(pipe%3+1),tr(qPrintable(msg)), 0.0, 0, 10000, 6, &ok,Qt::WindowFlags(), 1);

					///
					/// compare injectedVolume with totalInjectedVolumeCalculated
					///
				 	if (abs(injectedVolume - (LOOP[loop].injectionWaterPumpRate)*totalInjectionTime) > 0) 
					{
						QFile::copy(PIPE[pipe].fileCalibrate.fileName(),PIPE[pipe].fileAdjusted.fileName());

						/// TODO
						// modify watercut in ADJUSTED.LCI here
					}

					///
                    /// finalize and close
					///
    				QDateTime currentDataTime = QDateTime::currentDateTime();
    				QString data_stream   = QString("Total injection time   = %1 s").arg(totalInjectionTime, 10, 'g', -1, ' ');
    				QString data_stream_2 = QString("Total injection volume = %1 mL").arg(injectedVolume, 10, 'g', -1, ' ');
    				QString data_stream_3 = QString("Loop volume            = %1 mL").arg(LOOP[loop].loopVolume->text().toDouble(), 10, 'g', -1, ' ');
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

					///
                    /// stop calibration
					///
                    LOOP[loop].isCal = false;
                    prepareForNextFile(pipe, NO_FILE);
                    updateStartButtonLabel(loop);
					stopCalibration(pipe);
					return;
                }
            }
			
            //////////////////////////////////////////////////
            /// restart singleshot timer or finish calibration
            //////////////////////////////////////////////////
            if (LOOP[loop].isCal && PIPE[pipe].checkBox->isChecked()) QTimer::singleShot(LOOP[loop].xDelay*1000, [this, pipe] () {calibrate(pipe);}); /// xDelay * 1000 = xDelay seconds
            else 
            {
				stopCalibration(pipe);
                updateStartButtonLabel(loop);
                PIPE[pipe].checkBox->setChecked(false);
                displayMessage(QString("LOOP ")+QString::number(pipe/3 + 1)+QString(", PIPE ")+QString::number(pipe%3+1),QString("Serial Number: ")+PIPE[pipe].slave->text()+QString("                                    "),"Calibration Finished Successfully!");
            }
        }
    }
} 

void
MainWindow::
injectWater(const bool isInjectWater)
{
	if (isInjectWater) return;
	else return;
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
        LOOP[loop].ID_SN_PIPE = 201;
        LOOP[loop].ID_WATERCUT = 3; // register id
        LOOP[loop].ID_TEMPERATURE = 5;
        LOOP[loop].ID_SALINITY = 9;
        LOOP[loop].ID_OIL_ADJUST = 15;
        LOOP[loop].ID_WATER_ADJUST = 17;
        LOOP[loop].ID_FREQ = 19;
        LOOP[loop].ID_OIL_RP = 61;

        if (loop==L1) ui->groupBox_6->setEnabled(true);
        else if (loop==L2) ui->groupBox_24->setEnabled(true);
        else if (loop==L3) ui->groupBox_46->setEnabled(true);
        else if (loop==L4) ui->groupBox_56->setEnabled(true);
        else if (loop==L5) ui->groupBox_76->setEnabled(true);
        else if (loop==L6) ui->groupBox_86->setEnabled(true);
    }
    else
    {
        LOOP[loop].ID_SN_PIPE = 201;
        LOOP[loop].ID_WATERCUT = 3; // register id
        LOOP[loop].ID_TEMPERATURE = 5;
        LOOP[loop].ID_SALINITY = 9;
        LOOP[loop].ID_OIL_ADJUST = 15;
        LOOP[loop].ID_WATER_ADJUST = 17;
        LOOP[loop].ID_FREQ = 19;
        LOOP[loop].ID_OIL_RP = 61;

        if (loop==L1) 
        {
            ui->radioButton_5->setChecked(true);
            ui->groupBox_6->setEnabled(false);
        }
        else if (loop==L2) 
        {
            ui->radioButton_14->setChecked(true);
            ui->groupBox_24->setEnabled(false);
        }
        else if (loop==L3) 
        {
            ui->radioButton_20->setChecked(true);
            ui->groupBox_46->setEnabled(false);
        }
        else if (loop==L4) 
        {
            ui->radioButton_26->setChecked(true);
            ui->groupBox_56->setEnabled(false);
        }
        else if (loop==L5) 
        {
            ui->radioButton_32->setChecked(true);
            ui->groupBox_76->setEnabled(false);
        }
        else if (loop==L6) 
        {
            ui->radioButton_38->setChecked(true);
            ui->groupBox_86->setEnabled(false);
        }
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
    ui->lineEdit_3->setValidator(serialNumberValidator);
    ui->lineEdit_8->setValidator(serialNumberValidator);
    ui->lineEdit_14->setValidator(serialNumberValidator);
    ui->lineEdit_5->setValidator(serialNumberValidator);
    ui->lineEdit_9->setValidator(serialNumberValidator);
    ui->lineEdit_15->setValidator(serialNumberValidator);
    ui->lineEdit_10->setValidator(serialNumberValidator);
    ui->lineEdit_11->setValidator(serialNumberValidator);
    ui->lineEdit_16->setValidator(serialNumberValidator);
    ui->lineEdit_17->setValidator(serialNumberValidator);
    ui->lineEdit_18->setValidator(serialNumberValidator);
    ui->lineEdit_19->setValidator(serialNumberValidator);
    ui->lineEdit_21->setValidator(serialNumberValidator);
    ui->lineEdit_22->setValidator(serialNumberValidator);
    ui->lineEdit_23->setValidator(serialNumberValidator);
}
