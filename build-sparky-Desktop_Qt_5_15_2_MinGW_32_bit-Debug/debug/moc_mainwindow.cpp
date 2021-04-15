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
    QByteArrayData data[73];
    char stringdata0[1273];
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
QT_MOC_LITERAL(4, 32, 17), // "toggleLineView_P1"
QT_MOC_LITERAL(5, 50, 17), // "toggleLineView_P2"
QT_MOC_LITERAL(6, 68, 17), // "toggleLineView_P3"
QT_MOC_LITERAL(7, 86, 18), // "injectionPumpRates"
QT_MOC_LITERAL(8, 105, 15), // "injectionBucket"
QT_MOC_LITERAL(9, 121, 13), // "injectionMark"
QT_MOC_LITERAL(10, 135, 15), // "injectionMethod"
QT_MOC_LITERAL(11, 151, 27), // "onActionPressureSensorSlope"
QT_MOC_LITERAL(12, 179, 12), // "onMinRefTemp"
QT_MOC_LITERAL(13, 192, 12), // "onMaxRefTemp"
QT_MOC_LITERAL(14, 205, 15), // "onInjectionTemp"
QT_MOC_LITERAL(15, 221, 8), // "onXDelay"
QT_MOC_LITERAL(16, 230, 7), // "onYFreq"
QT_MOC_LITERAL(17, 238, 7), // "onZTemp"
QT_MOC_LITERAL(18, 246, 18), // "onActionMainServer"
QT_MOC_LITERAL(19, 265, 19), // "onActionLocalServer"
QT_MOC_LITERAL(20, 285, 19), // "onIntervalSmallPump"
QT_MOC_LITERAL(21, 305, 17), // "onIntervalBigPump"
QT_MOC_LITERAL(22, 323, 12), // "onLoopNumber"
QT_MOC_LITERAL(23, 336, 17), // "onActionMinMaster"
QT_MOC_LITERAL(24, 354, 17), // "onActionMaxMaster"
QT_MOC_LITERAL(25, 372, 19), // "onActionDeltaMaster"
QT_MOC_LITERAL(26, 392, 24), // "onActionDeltaMasterFinal"
QT_MOC_LITERAL(27, 417, 13), // "onActionWater"
QT_MOC_LITERAL(28, 431, 11), // "onActionOil"
QT_MOC_LITERAL(29, 443, 13), // "onModeChanged"
QT_MOC_LITERAL(30, 457, 19), // "onMasterPipeToggled"
QT_MOC_LITERAL(31, 477, 12), // "runInjection"
QT_MOC_LITERAL(32, 490, 10), // "runTempRun"
QT_MOC_LITERAL(33, 501, 15), // "stopCalibration"
QT_MOC_LITERAL(34, 517, 14), // "updateFileList"
QT_MOC_LITERAL(35, 532, 19), // "createInjectionFile"
QT_MOC_LITERAL(36, 552, 26), // "onCalibrationButtonPressed"
QT_MOC_LITERAL(37, 579, 15), // "onRtuPortActive"
QT_MOC_LITERAL(38, 595, 16), // "changeSerialPort"
QT_MOC_LITERAL(39, 612, 17), // "createTempRunFile"
QT_MOC_LITERAL(40, 630, 22), // "initializeToolbarIcons"
QT_MOC_LITERAL(41, 653, 13), // "clearMonitors"
QT_MOC_LITERAL(42, 667, 20), // "updateRequestPreview"
QT_MOC_LITERAL(43, 688, 18), // "updateRegisterView"
QT_MOC_LITERAL(44, 707, 13), // "enableHexView"
QT_MOC_LITERAL(45, 721, 17), // "sendModbusRequest"
QT_MOC_LITERAL(46, 739, 17), // "onSendButtonPress"
QT_MOC_LITERAL(47, 757, 16), // "pollForDataOnBus"
QT_MOC_LITERAL(48, 774, 12), // "aboutQModBus"
QT_MOC_LITERAL(49, 787, 17), // "onCheckBoxChecked"
QT_MOC_LITERAL(50, 805, 17), // "onCheckBoxClicked"
QT_MOC_LITERAL(51, 823, 11), // "resetStatus"
QT_MOC_LITERAL(52, 835, 14), // "setStatusError"
QT_MOC_LITERAL(53, 850, 20), // "onFloatButtonPressed"
QT_MOC_LITERAL(54, 871, 22), // "onIntegerButtonPressed"
QT_MOC_LITERAL(55, 894, 19), // "onCoilButtonPressed"
QT_MOC_LITERAL(56, 914, 19), // "onReadButtonPressed"
QT_MOC_LITERAL(57, 934, 20), // "onWriteButtonPressed"
QT_MOC_LITERAL(58, 955, 23), // "onEquationButtonPressed"
QT_MOC_LITERAL(59, 979, 11), // "loadCsvFile"
QT_MOC_LITERAL(60, 991, 15), // "loadCsvTemplate"
QT_MOC_LITERAL(61, 1007, 16), // "onUploadEquation"
QT_MOC_LITERAL(62, 1024, 18), // "onDownloadEquation"
QT_MOC_LITERAL(63, 1043, 17), // "onUpdateRegisters"
QT_MOC_LITERAL(64, 1061, 23), // "onDownloadButtonChecked"
QT_MOC_LITERAL(65, 1085, 11), // "saveCsvFile"
QT_MOC_LITERAL(66, 1097, 22), // "onEquationTableChecked"
QT_MOC_LITERAL(67, 1120, 32), // "onUnlockFactoryDefaultBtnPressed"
QT_MOC_LITERAL(68, 1153, 30), // "onLockFactoryDefaultBtnPressed"
QT_MOC_LITERAL(69, 1184, 29), // "onUpdateFactoryDefaultPressed"
QT_MOC_LITERAL(70, 1214, 19), // "updatePipeStability"
QT_MOC_LITERAL(71, 1234, 18), // "readJsonConfigFile"
QT_MOC_LITERAL(72, 1253, 19) // "writeJsonConfigFile"

    },
    "MainWindow\0connectionError\0\0msg\0"
    "toggleLineView_P1\0toggleLineView_P2\0"
    "toggleLineView_P3\0injectionPumpRates\0"
    "injectionBucket\0injectionMark\0"
    "injectionMethod\0onActionPressureSensorSlope\0"
    "onMinRefTemp\0onMaxRefTemp\0onInjectionTemp\0"
    "onXDelay\0onYFreq\0onZTemp\0onActionMainServer\0"
    "onActionLocalServer\0onIntervalSmallPump\0"
    "onIntervalBigPump\0onLoopNumber\0"
    "onActionMinMaster\0onActionMaxMaster\0"
    "onActionDeltaMaster\0onActionDeltaMasterFinal\0"
    "onActionWater\0onActionOil\0onModeChanged\0"
    "onMasterPipeToggled\0runInjection\0"
    "runTempRun\0stopCalibration\0updateFileList\0"
    "createInjectionFile\0onCalibrationButtonPressed\0"
    "onRtuPortActive\0changeSerialPort\0"
    "createTempRunFile\0initializeToolbarIcons\0"
    "clearMonitors\0updateRequestPreview\0"
    "updateRegisterView\0enableHexView\0"
    "sendModbusRequest\0onSendButtonPress\0"
    "pollForDataOnBus\0aboutQModBus\0"
    "onCheckBoxChecked\0onCheckBoxClicked\0"
    "resetStatus\0setStatusError\0"
    "onFloatButtonPressed\0onIntegerButtonPressed\0"
    "onCoilButtonPressed\0onReadButtonPressed\0"
    "onWriteButtonPressed\0onEquationButtonPressed\0"
    "loadCsvFile\0loadCsvTemplate\0"
    "onUploadEquation\0onDownloadEquation\0"
    "onUpdateRegisters\0onDownloadButtonChecked\0"
    "saveCsvFile\0onEquationTableChecked\0"
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
      70,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  364,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,  367,    2, 0x08 /* Private */,
       5,    1,  370,    2, 0x08 /* Private */,
       6,    1,  373,    2, 0x08 /* Private */,
       7,    0,  376,    2, 0x08 /* Private */,
       8,    0,  377,    2, 0x08 /* Private */,
       9,    0,  378,    2, 0x08 /* Private */,
      10,    0,  379,    2, 0x08 /* Private */,
      11,    0,  380,    2, 0x08 /* Private */,
      12,    0,  381,    2, 0x08 /* Private */,
      13,    0,  382,    2, 0x08 /* Private */,
      14,    0,  383,    2, 0x08 /* Private */,
      15,    0,  384,    2, 0x08 /* Private */,
      16,    0,  385,    2, 0x08 /* Private */,
      17,    0,  386,    2, 0x08 /* Private */,
      18,    0,  387,    2, 0x08 /* Private */,
      19,    0,  388,    2, 0x08 /* Private */,
      20,    0,  389,    2, 0x08 /* Private */,
      21,    0,  390,    2, 0x08 /* Private */,
      22,    0,  391,    2, 0x08 /* Private */,
      23,    0,  392,    2, 0x08 /* Private */,
      24,    0,  393,    2, 0x08 /* Private */,
      25,    0,  394,    2, 0x08 /* Private */,
      26,    0,  395,    2, 0x08 /* Private */,
      27,    0,  396,    2, 0x08 /* Private */,
      28,    0,  397,    2, 0x08 /* Private */,
      29,    1,  398,    2, 0x08 /* Private */,
      30,    1,  401,    2, 0x08 /* Private */,
      31,    0,  404,    2, 0x08 /* Private */,
      32,    0,  405,    2, 0x08 /* Private */,
      33,    0,  406,    2, 0x08 /* Private */,
      34,    3,  407,    2, 0x08 /* Private */,
      35,    6,  414,    2, 0x08 /* Private */,
      36,    0,  427,    2, 0x08 /* Private */,
      37,    1,  428,    2, 0x08 /* Private */,
      38,    1,  431,    2, 0x08 /* Private */,
      39,    5,  434,    2, 0x08 /* Private */,
      40,    0,  445,    2, 0x08 /* Private */,
      41,    0,  446,    2, 0x08 /* Private */,
      42,    0,  447,    2, 0x08 /* Private */,
      43,    0,  448,    2, 0x08 /* Private */,
      44,    0,  449,    2, 0x08 /* Private */,
      45,    0,  450,    2, 0x08 /* Private */,
      46,    0,  451,    2, 0x08 /* Private */,
      47,    0,  452,    2, 0x08 /* Private */,
      48,    0,  453,    2, 0x08 /* Private */,
      49,    1,  454,    2, 0x08 /* Private */,
      50,    1,  457,    2, 0x08 /* Private */,
      51,    0,  460,    2, 0x08 /* Private */,
      52,    1,  461,    2, 0x08 /* Private */,
      53,    1,  464,    2, 0x08 /* Private */,
      54,    1,  467,    2, 0x08 /* Private */,
      55,    1,  470,    2, 0x08 /* Private */,
      56,    1,  473,    2, 0x08 /* Private */,
      57,    1,  476,    2, 0x08 /* Private */,
      58,    0,  479,    2, 0x08 /* Private */,
      59,    0,  480,    2, 0x08 /* Private */,
      60,    0,  481,    2, 0x08 /* Private */,
      61,    0,  482,    2, 0x08 /* Private */,
      62,    0,  483,    2, 0x08 /* Private */,
      63,    1,  484,    2, 0x08 /* Private */,
      64,    1,  487,    2, 0x08 /* Private */,
      65,    0,  490,    2, 0x08 /* Private */,
      66,    1,  491,    2, 0x08 /* Private */,
      67,    0,  494,    2, 0x08 /* Private */,
      68,    0,  495,    2, 0x08 /* Private */,
      69,    0,  496,    2, 0x08 /* Private */,
      70,    3,  497,    2, 0x08 /* Private */,
      71,    0,  504,    2, 0x08 /* Private */,
      72,    0,  505,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
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
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    2,    2,    2,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    2,
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
    QMetaType::Void, QMetaType::Bool,    2,
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
        case 1: _t->toggleLineView_P1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->toggleLineView_P2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->toggleLineView_P3((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->injectionPumpRates(); break;
        case 5: _t->injectionBucket(); break;
        case 6: _t->injectionMark(); break;
        case 7: _t->injectionMethod(); break;
        case 8: _t->onActionPressureSensorSlope(); break;
        case 9: _t->onMinRefTemp(); break;
        case 10: _t->onMaxRefTemp(); break;
        case 11: _t->onInjectionTemp(); break;
        case 12: _t->onXDelay(); break;
        case 13: _t->onYFreq(); break;
        case 14: _t->onZTemp(); break;
        case 15: _t->onActionMainServer(); break;
        case 16: _t->onActionLocalServer(); break;
        case 17: _t->onIntervalSmallPump(); break;
        case 18: _t->onIntervalBigPump(); break;
        case 19: _t->onLoopNumber(); break;
        case 20: _t->onActionMinMaster(); break;
        case 21: _t->onActionMaxMaster(); break;
        case 22: _t->onActionDeltaMaster(); break;
        case 23: _t->onActionDeltaMasterFinal(); break;
        case 24: _t->onActionWater(); break;
        case 25: _t->onActionOil(); break;
        case 26: _t->onModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 27: _t->onMasterPipeToggled((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 28: _t->runInjection(); break;
        case 29: _t->runTempRun(); break;
        case 30: _t->stopCalibration(); break;
        case 31: _t->updateFileList((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 32: _t->createInjectionFile((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6]))); break;
        case 33: _t->onCalibrationButtonPressed(); break;
        case 34: _t->onRtuPortActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 35: _t->changeSerialPort((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->createTempRunFile((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const int(*)>(_a[5]))); break;
        case 37: _t->initializeToolbarIcons(); break;
        case 38: _t->clearMonitors(); break;
        case 39: _t->updateRequestPreview(); break;
        case 40: _t->updateRegisterView(); break;
        case 41: _t->enableHexView(); break;
        case 42: _t->sendModbusRequest(); break;
        case 43: _t->onSendButtonPress(); break;
        case 44: _t->pollForDataOnBus(); break;
        case 45: _t->aboutQModBus(); break;
        case 46: _t->onCheckBoxChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 47: _t->onCheckBoxClicked((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 48: _t->resetStatus(); break;
        case 49: _t->setStatusError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 50: _t->onFloatButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 51: _t->onIntegerButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 52: _t->onCoilButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 53: _t->onReadButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 54: _t->onWriteButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 55: _t->onEquationButtonPressed(); break;
        case 56: _t->loadCsvFile(); break;
        case 57: _t->loadCsvTemplate(); break;
        case 58: _t->onUploadEquation(); break;
        case 59: _t->onDownloadEquation(); break;
        case 60: _t->onUpdateRegisters((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 61: _t->onDownloadButtonChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 62: _t->saveCsvFile(); break;
        case 63: _t->onEquationTableChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 64: _t->onUnlockFactoryDefaultBtnPressed(); break;
        case 65: _t->onLockFactoryDefaultBtnPressed(); break;
        case 66: _t->onUpdateFactoryDefaultPressed(); break;
        case 67: _t->updatePipeStability((*reinterpret_cast< const bool(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 68: _t->readJsonConfigFile(); break;
        case 69: _t->writeJsonConfigFile(); break;
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
        if (_id < 70)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 70;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 70)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 70;
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
