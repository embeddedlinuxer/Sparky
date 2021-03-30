/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../Sparky/src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[72];
    char stringdata0[1260];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 15), // "connectionError"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 3), // "msg"
QT_MOC_LITERAL(4, 32, 19), // "onMasterPipeToggled"
QT_MOC_LITERAL(5, 52, 9), // "calibrate"
QT_MOC_LITERAL(6, 62, 15), // "stopCalibration"
QT_MOC_LITERAL(7, 78, 14), // "updateFileList"
QT_MOC_LITERAL(8, 93, 19), // "createCalibrateFile"
QT_MOC_LITERAL(9, 113, 26), // "onCalibrationButtonPressed"
QT_MOC_LITERAL(10, 140, 15), // "onRtuPortActive"
QT_MOC_LITERAL(11, 156, 16), // "changeSerialPort"
QT_MOC_LITERAL(12, 173, 17), // "toggleLineView_P1"
QT_MOC_LITERAL(13, 191, 17), // "toggleLineView_P2"
QT_MOC_LITERAL(14, 209, 17), // "toggleLineView_P3"
QT_MOC_LITERAL(15, 227, 13), // "onModeChanged"
QT_MOC_LITERAL(16, 241, 18), // "injectionPumpRates"
QT_MOC_LITERAL(17, 260, 15), // "injectionBucket"
QT_MOC_LITERAL(18, 276, 13), // "injectionMark"
QT_MOC_LITERAL(19, 290, 15), // "injectionMethod"
QT_MOC_LITERAL(20, 306, 27), // "onActionPressureSensorSlope"
QT_MOC_LITERAL(21, 334, 12), // "onMinRefTemp"
QT_MOC_LITERAL(22, 347, 12), // "onMaxRefTemp"
QT_MOC_LITERAL(23, 360, 15), // "onInjectionTemp"
QT_MOC_LITERAL(24, 376, 8), // "onXDelay"
QT_MOC_LITERAL(25, 385, 7), // "onYFreq"
QT_MOC_LITERAL(26, 393, 7), // "onZTemp"
QT_MOC_LITERAL(27, 401, 18), // "onActionMainServer"
QT_MOC_LITERAL(28, 420, 19), // "onActionLocalServer"
QT_MOC_LITERAL(29, 440, 21), // "onIntervalCalibration"
QT_MOC_LITERAL(30, 462, 18), // "onIntervalRollover"
QT_MOC_LITERAL(31, 481, 12), // "onLoopNumber"
QT_MOC_LITERAL(32, 494, 17), // "onActionMinMaster"
QT_MOC_LITERAL(33, 512, 17), // "onActionMaxMaster"
QT_MOC_LITERAL(34, 530, 19), // "onActionDeltaMaster"
QT_MOC_LITERAL(35, 550, 24), // "onActionDeltaMasterFinal"
QT_MOC_LITERAL(36, 575, 13), // "onActionWater"
QT_MOC_LITERAL(37, 589, 11), // "onActionOil"
QT_MOC_LITERAL(38, 601, 14), // "createLoopFile"
QT_MOC_LITERAL(39, 616, 22), // "initializeToolbarIcons"
QT_MOC_LITERAL(40, 639, 13), // "clearMonitors"
QT_MOC_LITERAL(41, 653, 20), // "updateRequestPreview"
QT_MOC_LITERAL(42, 674, 18), // "updateRegisterView"
QT_MOC_LITERAL(43, 693, 13), // "enableHexView"
QT_MOC_LITERAL(44, 707, 17), // "sendModbusRequest"
QT_MOC_LITERAL(45, 725, 17), // "onSendButtonPress"
QT_MOC_LITERAL(46, 743, 16), // "pollForDataOnBus"
QT_MOC_LITERAL(47, 760, 18), // "openBatchProcessor"
QT_MOC_LITERAL(48, 779, 12), // "aboutQModBus"
QT_MOC_LITERAL(49, 792, 17), // "onCheckBoxChecked"
QT_MOC_LITERAL(50, 810, 11), // "resetStatus"
QT_MOC_LITERAL(51, 822, 14), // "setStatusError"
QT_MOC_LITERAL(52, 837, 20), // "onFloatButtonPressed"
QT_MOC_LITERAL(53, 858, 22), // "onIntegerButtonPressed"
QT_MOC_LITERAL(54, 881, 19), // "onCoilButtonPressed"
QT_MOC_LITERAL(55, 901, 19), // "onReadButtonPressed"
QT_MOC_LITERAL(56, 921, 20), // "onWriteButtonPressed"
QT_MOC_LITERAL(57, 942, 23), // "onEquationButtonPressed"
QT_MOC_LITERAL(58, 966, 11), // "loadCsvFile"
QT_MOC_LITERAL(59, 978, 15), // "loadCsvTemplate"
QT_MOC_LITERAL(60, 994, 16), // "onUploadEquation"
QT_MOC_LITERAL(61, 1011, 18), // "onDownloadEquation"
QT_MOC_LITERAL(62, 1030, 17), // "onUpdateRegisters"
QT_MOC_LITERAL(63, 1048, 23), // "onDownloadButtonChecked"
QT_MOC_LITERAL(64, 1072, 11), // "saveCsvFile"
QT_MOC_LITERAL(65, 1084, 22), // "onEquationTableChecked"
QT_MOC_LITERAL(66, 1107, 32), // "onUnlockFactoryDefaultBtnPressed"
QT_MOC_LITERAL(67, 1140, 30), // "onLockFactoryDefaultBtnPressed"
QT_MOC_LITERAL(68, 1171, 29), // "onUpdateFactoryDefaultPressed"
QT_MOC_LITERAL(69, 1201, 19), // "updatePipeStability"
QT_MOC_LITERAL(70, 1221, 18), // "readJsonConfigFile"
QT_MOC_LITERAL(71, 1240, 19) // "writeJsonConfigFile"

    },
    "MainWindow\0connectionError\0\0msg\0"
    "onMasterPipeToggled\0calibrate\0"
    "stopCalibration\0updateFileList\0"
    "createCalibrateFile\0onCalibrationButtonPressed\0"
    "onRtuPortActive\0changeSerialPort\0"
    "toggleLineView_P1\0toggleLineView_P2\0"
    "toggleLineView_P3\0onModeChanged\0"
    "injectionPumpRates\0injectionBucket\0"
    "injectionMark\0injectionMethod\0"
    "onActionPressureSensorSlope\0onMinRefTemp\0"
    "onMaxRefTemp\0onInjectionTemp\0onXDelay\0"
    "onYFreq\0onZTemp\0onActionMainServer\0"
    "onActionLocalServer\0onIntervalCalibration\0"
    "onIntervalRollover\0onLoopNumber\0"
    "onActionMinMaster\0onActionMaxMaster\0"
    "onActionDeltaMaster\0onActionDeltaMasterFinal\0"
    "onActionWater\0onActionOil\0createLoopFile\0"
    "initializeToolbarIcons\0clearMonitors\0"
    "updateRequestPreview\0updateRegisterView\0"
    "enableHexView\0sendModbusRequest\0"
    "onSendButtonPress\0pollForDataOnBus\0"
    "openBatchProcessor\0aboutQModBus\0"
    "onCheckBoxChecked\0resetStatus\0"
    "setStatusError\0onFloatButtonPressed\0"
    "onIntegerButtonPressed\0onCoilButtonPressed\0"
    "onReadButtonPressed\0onWriteButtonPressed\0"
    "onEquationButtonPressed\0loadCsvFile\0"
    "loadCsvTemplate\0onUploadEquation\0"
    "onDownloadEquation\0onUpdateRegisters\0"
    "onDownloadButtonChecked\0saveCsvFile\0"
    "onEquationTableChecked\0"
    "onUnlockFactoryDefaultBtnPressed\0"
    "onLockFactoryDefaultBtnPressed\0"
    "onUpdateFactoryDefaultPressed\0"
    "updatePipeStability\0readJsonConfigFile\0"
    "writeJsonConfigFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      69,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  359,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,  362,    2, 0x08 /* Private */,
       5,    0,  365,    2, 0x08 /* Private */,
       6,    0,  366,    2, 0x08 /* Private */,
       7,    3,  367,    2, 0x08 /* Private */,
       8,    6,  374,    2, 0x08 /* Private */,
       9,    0,  387,    2, 0x08 /* Private */,
      10,    1,  388,    2, 0x08 /* Private */,
      11,    1,  391,    2, 0x08 /* Private */,
      12,    1,  394,    2, 0x08 /* Private */,
      13,    1,  397,    2, 0x08 /* Private */,
      14,    1,  400,    2, 0x08 /* Private */,
      15,    1,  403,    2, 0x08 /* Private */,
      16,    0,  406,    2, 0x08 /* Private */,
      17,    0,  407,    2, 0x08 /* Private */,
      18,    0,  408,    2, 0x08 /* Private */,
      19,    0,  409,    2, 0x08 /* Private */,
      20,    0,  410,    2, 0x08 /* Private */,
      21,    0,  411,    2, 0x08 /* Private */,
      22,    0,  412,    2, 0x08 /* Private */,
      23,    0,  413,    2, 0x08 /* Private */,
      24,    0,  414,    2, 0x08 /* Private */,
      25,    0,  415,    2, 0x08 /* Private */,
      26,    0,  416,    2, 0x08 /* Private */,
      27,    0,  417,    2, 0x08 /* Private */,
      28,    0,  418,    2, 0x08 /* Private */,
      29,    0,  419,    2, 0x08 /* Private */,
      30,    0,  420,    2, 0x08 /* Private */,
      31,    0,  421,    2, 0x08 /* Private */,
      32,    0,  422,    2, 0x08 /* Private */,
      33,    0,  423,    2, 0x08 /* Private */,
      34,    0,  424,    2, 0x08 /* Private */,
      35,    0,  425,    2, 0x08 /* Private */,
      36,    0,  426,    2, 0x08 /* Private */,
      37,    0,  427,    2, 0x08 /* Private */,
      38,    5,  428,    2, 0x08 /* Private */,
      39,    0,  439,    2, 0x08 /* Private */,
      40,    0,  440,    2, 0x08 /* Private */,
      41,    0,  441,    2, 0x08 /* Private */,
      42,    0,  442,    2, 0x08 /* Private */,
      43,    0,  443,    2, 0x08 /* Private */,
      44,    0,  444,    2, 0x08 /* Private */,
      45,    0,  445,    2, 0x08 /* Private */,
      46,    0,  446,    2, 0x08 /* Private */,
      47,    0,  447,    2, 0x08 /* Private */,
      48,    0,  448,    2, 0x08 /* Private */,
      49,    1,  449,    2, 0x08 /* Private */,
      50,    0,  452,    2, 0x08 /* Private */,
      51,    1,  453,    2, 0x08 /* Private */,
      52,    1,  456,    2, 0x08 /* Private */,
      53,    1,  459,    2, 0x08 /* Private */,
      54,    1,  462,    2, 0x08 /* Private */,
      55,    1,  465,    2, 0x08 /* Private */,
      56,    1,  468,    2, 0x08 /* Private */,
      57,    0,  471,    2, 0x08 /* Private */,
      58,    0,  472,    2, 0x08 /* Private */,
      59,    0,  473,    2, 0x08 /* Private */,
      60,    0,  474,    2, 0x08 /* Private */,
      61,    0,  475,    2, 0x08 /* Private */,
      62,    1,  476,    2, 0x08 /* Private */,
      63,    1,  479,    2, 0x08 /* Private */,
      64,    0,  482,    2, 0x08 /* Private */,
      65,    1,  483,    2, 0x08 /* Private */,
      66,    0,  486,    2, 0x08 /* Private */,
      67,    0,  487,    2, 0x08 /* Private */,
      68,    0,  488,    2, 0x08 /* Private */,
      69,    3,  489,    2, 0x08 /* Private */,
      70,    0,  496,    2, 0x08 /* Private */,
      71,    0,  497,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    2,    2,    2,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,    2,    2,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int, QMetaType::Int,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectionError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->onMasterPipeToggled((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 2: _t->calibrate(); break;
        case 3: _t->stopCalibration(); break;
        case 4: _t->updateFileList((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 5: _t->createCalibrateFile((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6]))); break;
        case 6: _t->onCalibrationButtonPressed(); break;
        case 7: _t->onRtuPortActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->changeSerialPort((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->toggleLineView_P1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->toggleLineView_P2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->toggleLineView_P3((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->onModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->injectionPumpRates(); break;
        case 14: _t->injectionBucket(); break;
        case 15: _t->injectionMark(); break;
        case 16: _t->injectionMethod(); break;
        case 17: _t->onActionPressureSensorSlope(); break;
        case 18: _t->onMinRefTemp(); break;
        case 19: _t->onMaxRefTemp(); break;
        case 20: _t->onInjectionTemp(); break;
        case 21: _t->onXDelay(); break;
        case 22: _t->onYFreq(); break;
        case 23: _t->onZTemp(); break;
        case 24: _t->onActionMainServer(); break;
        case 25: _t->onActionLocalServer(); break;
        case 26: _t->onIntervalCalibration(); break;
        case 27: _t->onIntervalRollover(); break;
        case 28: _t->onLoopNumber(); break;
        case 29: _t->onActionMinMaster(); break;
        case 30: _t->onActionMaxMaster(); break;
        case 31: _t->onActionDeltaMaster(); break;
        case 32: _t->onActionDeltaMasterFinal(); break;
        case 33: _t->onActionWater(); break;
        case 34: _t->onActionOil(); break;
        case 35: _t->createLoopFile((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const int(*)>(_a[5]))); break;
        case 36: _t->initializeToolbarIcons(); break;
        case 37: _t->clearMonitors(); break;
        case 38: _t->updateRequestPreview(); break;
        case 39: _t->updateRegisterView(); break;
        case 40: _t->enableHexView(); break;
        case 41: _t->sendModbusRequest(); break;
        case 42: _t->onSendButtonPress(); break;
        case 43: _t->pollForDataOnBus(); break;
        case 44: _t->openBatchProcessor(); break;
        case 45: _t->aboutQModBus(); break;
        case 46: _t->onCheckBoxChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 47: _t->resetStatus(); break;
        case 48: _t->setStatusError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 49: _t->onFloatButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 50: _t->onIntegerButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 51: _t->onCoilButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 52: _t->onReadButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 53: _t->onWriteButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 54: _t->onEquationButtonPressed(); break;
        case 55: _t->loadCsvFile(); break;
        case 56: _t->loadCsvTemplate(); break;
        case 57: _t->onUploadEquation(); break;
        case 58: _t->onDownloadEquation(); break;
        case 59: _t->onUpdateRegisters((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 60: _t->onDownloadButtonChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 61: _t->saveCsvFile(); break;
        case 62: _t->onEquationTableChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 63: _t->onUnlockFactoryDefaultBtnPressed(); break;
        case 64: _t->onLockFactoryDefaultBtnPressed(); break;
        case 65: _t->onUpdateFactoryDefaultPressed(); break;
        case 66: _t->updatePipeStability((*reinterpret_cast< const bool(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 67: _t->readJsonConfigFile(); break;
        case 68: _t->writeJsonConfigFile(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::connectionError)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 69)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 69;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 69)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 69;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::connectionError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
