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
    QByteArrayData data[66];
    char stringdata0[1149];
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
QT_MOC_LITERAL(4, 32, 9), // "calibrate"
QT_MOC_LITERAL(5, 42, 14), // "updateFileList"
QT_MOC_LITERAL(6, 57, 19), // "createCalibrateFile"
QT_MOC_LITERAL(7, 77, 15), // "stopCalibration"
QT_MOC_LITERAL(8, 93, 26), // "onCalibrationButtonPressed"
QT_MOC_LITERAL(9, 120, 15), // "onRtuPortActive"
QT_MOC_LITERAL(10, 136, 16), // "changeSerialPort"
QT_MOC_LITERAL(11, 153, 17), // "toggleLineView_P1"
QT_MOC_LITERAL(12, 171, 17), // "toggleLineView_P2"
QT_MOC_LITERAL(13, 189, 17), // "toggleLineView_P3"
QT_MOC_LITERAL(14, 207, 17), // "onProductSelected"
QT_MOC_LITERAL(15, 225, 13), // "onModeChanged"
QT_MOC_LITERAL(16, 239, 18), // "injectionPumpRates"
QT_MOC_LITERAL(17, 258, 15), // "injectionBucket"
QT_MOC_LITERAL(18, 274, 13), // "injectionMark"
QT_MOC_LITERAL(19, 288, 15), // "injectionMethod"
QT_MOC_LITERAL(20, 304, 27), // "onActionPressureSensorSlope"
QT_MOC_LITERAL(21, 332, 12), // "onMinRefTemp"
QT_MOC_LITERAL(22, 345, 12), // "onMaxRefTemp"
QT_MOC_LITERAL(23, 358, 15), // "onInjectionTemp"
QT_MOC_LITERAL(24, 374, 8), // "onXDelay"
QT_MOC_LITERAL(25, 383, 7), // "onYFreq"
QT_MOC_LITERAL(26, 391, 7), // "onZTemp"
QT_MOC_LITERAL(27, 399, 18), // "onActionMainServer"
QT_MOC_LITERAL(28, 418, 19), // "onActionLocalServer"
QT_MOC_LITERAL(29, 438, 21), // "onIntervalCalibration"
QT_MOC_LITERAL(30, 460, 18), // "onIntervalRollover"
QT_MOC_LITERAL(31, 479, 14), // "createLoopFile"
QT_MOC_LITERAL(32, 494, 22), // "initializeToolbarIcons"
QT_MOC_LITERAL(33, 517, 16), // "onLoopTabChanged"
QT_MOC_LITERAL(34, 534, 13), // "clearMonitors"
QT_MOC_LITERAL(35, 548, 20), // "updateRequestPreview"
QT_MOC_LITERAL(36, 569, 18), // "updateRegisterView"
QT_MOC_LITERAL(37, 588, 13), // "enableHexView"
QT_MOC_LITERAL(38, 602, 17), // "sendModbusRequest"
QT_MOC_LITERAL(39, 620, 17), // "onSendButtonPress"
QT_MOC_LITERAL(40, 638, 16), // "pollForDataOnBus"
QT_MOC_LITERAL(41, 655, 18), // "openBatchProcessor"
QT_MOC_LITERAL(42, 674, 12), // "aboutQModBus"
QT_MOC_LITERAL(43, 687, 17), // "onCheckBoxChecked"
QT_MOC_LITERAL(44, 705, 11), // "resetStatus"
QT_MOC_LITERAL(45, 717, 14), // "setStatusError"
QT_MOC_LITERAL(46, 732, 20), // "onFloatButtonPressed"
QT_MOC_LITERAL(47, 753, 22), // "onIntegerButtonPressed"
QT_MOC_LITERAL(48, 776, 19), // "onCoilButtonPressed"
QT_MOC_LITERAL(49, 796, 19), // "onReadButtonPressed"
QT_MOC_LITERAL(50, 816, 20), // "onWriteButtonPressed"
QT_MOC_LITERAL(51, 837, 23), // "onEquationButtonPressed"
QT_MOC_LITERAL(52, 861, 11), // "loadCsvFile"
QT_MOC_LITERAL(53, 873, 15), // "loadCsvTemplate"
QT_MOC_LITERAL(54, 889, 16), // "onUploadEquation"
QT_MOC_LITERAL(55, 906, 18), // "onDownloadEquation"
QT_MOC_LITERAL(56, 925, 15), // "updateRegisters"
QT_MOC_LITERAL(57, 941, 23), // "onDownloadButtonChecked"
QT_MOC_LITERAL(58, 965, 11), // "saveCsvFile"
QT_MOC_LITERAL(59, 977, 22), // "onEquationTableChecked"
QT_MOC_LITERAL(60, 1000, 32), // "onUnlockFactoryDefaultBtnPressed"
QT_MOC_LITERAL(61, 1033, 30), // "onLockFactoryDefaultBtnPressed"
QT_MOC_LITERAL(62, 1064, 29), // "onUpdateFactoryDefaultPressed"
QT_MOC_LITERAL(63, 1094, 15), // "updateStability"
QT_MOC_LITERAL(64, 1110, 18), // "readJsonConfigFile"
QT_MOC_LITERAL(65, 1129, 19) // "writeJsonConfigFile"

    },
    "MainWindow\0connectionError\0\0msg\0"
    "calibrate\0updateFileList\0createCalibrateFile\0"
    "stopCalibration\0onCalibrationButtonPressed\0"
    "onRtuPortActive\0changeSerialPort\0"
    "toggleLineView_P1\0toggleLineView_P2\0"
    "toggleLineView_P3\0onProductSelected\0"
    "onModeChanged\0injectionPumpRates\0"
    "injectionBucket\0injectionMark\0"
    "injectionMethod\0onActionPressureSensorSlope\0"
    "onMinRefTemp\0onMaxRefTemp\0onInjectionTemp\0"
    "onXDelay\0onYFreq\0onZTemp\0onActionMainServer\0"
    "onActionLocalServer\0onIntervalCalibration\0"
    "onIntervalRollover\0createLoopFile\0"
    "initializeToolbarIcons\0onLoopTabChanged\0"
    "clearMonitors\0updateRequestPreview\0"
    "updateRegisterView\0enableHexView\0"
    "sendModbusRequest\0onSendButtonPress\0"
    "pollForDataOnBus\0openBatchProcessor\0"
    "aboutQModBus\0onCheckBoxChecked\0"
    "resetStatus\0setStatusError\0"
    "onFloatButtonPressed\0onIntegerButtonPressed\0"
    "onCoilButtonPressed\0onReadButtonPressed\0"
    "onWriteButtonPressed\0onEquationButtonPressed\0"
    "loadCsvFile\0loadCsvTemplate\0"
    "onUploadEquation\0onDownloadEquation\0"
    "updateRegisters\0onDownloadButtonChecked\0"
    "saveCsvFile\0onEquationTableChecked\0"
    "onUnlockFactoryDefaultBtnPressed\0"
    "onLockFactoryDefaultBtnPressed\0"
    "onUpdateFactoryDefaultPressed\0"
    "updateStability\0readJsonConfigFile\0"
    "writeJsonConfigFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      63,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  329,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,  332,    2, 0x08 /* Private */,
       5,    4,  335,    2, 0x08 /* Private */,
       6,    6,  344,    2, 0x08 /* Private */,
       7,    1,  357,    2, 0x08 /* Private */,
       8,    1,  360,    2, 0x08 /* Private */,
       9,    2,  363,    2, 0x08 /* Private */,
      10,    1,  368,    2, 0x08 /* Private */,
      11,    1,  371,    2, 0x08 /* Private */,
      12,    1,  374,    2, 0x08 /* Private */,
      13,    1,  377,    2, 0x08 /* Private */,
      14,    1,  380,    2, 0x08 /* Private */,
      15,    1,  383,    2, 0x08 /* Private */,
      16,    1,  386,    2, 0x08 /* Private */,
      17,    1,  389,    2, 0x08 /* Private */,
      18,    1,  392,    2, 0x08 /* Private */,
      19,    1,  395,    2, 0x08 /* Private */,
      20,    1,  398,    2, 0x08 /* Private */,
      21,    1,  401,    2, 0x08 /* Private */,
      22,    1,  404,    2, 0x08 /* Private */,
      23,    1,  407,    2, 0x08 /* Private */,
      24,    1,  410,    2, 0x08 /* Private */,
      25,    1,  413,    2, 0x08 /* Private */,
      26,    1,  416,    2, 0x08 /* Private */,
      27,    0,  419,    2, 0x08 /* Private */,
      28,    0,  420,    2, 0x08 /* Private */,
      29,    1,  421,    2, 0x08 /* Private */,
      30,    1,  424,    2, 0x08 /* Private */,
      31,    6,  427,    2, 0x08 /* Private */,
      32,    0,  440,    2, 0x08 /* Private */,
      33,    1,  441,    2, 0x08 /* Private */,
      34,    0,  444,    2, 0x08 /* Private */,
      35,    0,  445,    2, 0x08 /* Private */,
      36,    0,  446,    2, 0x08 /* Private */,
      37,    0,  447,    2, 0x08 /* Private */,
      38,    0,  448,    2, 0x08 /* Private */,
      39,    0,  449,    2, 0x08 /* Private */,
      40,    0,  450,    2, 0x08 /* Private */,
      41,    0,  451,    2, 0x08 /* Private */,
      42,    0,  452,    2, 0x08 /* Private */,
      43,    1,  453,    2, 0x08 /* Private */,
      44,    0,  456,    2, 0x08 /* Private */,
      45,    1,  457,    2, 0x08 /* Private */,
      46,    1,  460,    2, 0x08 /* Private */,
      47,    1,  463,    2, 0x08 /* Private */,
      48,    1,  466,    2, 0x08 /* Private */,
      49,    1,  469,    2, 0x08 /* Private */,
      50,    1,  472,    2, 0x08 /* Private */,
      51,    0,  475,    2, 0x08 /* Private */,
      52,    0,  476,    2, 0x08 /* Private */,
      53,    0,  477,    2, 0x08 /* Private */,
      54,    0,  478,    2, 0x08 /* Private */,
      55,    0,  479,    2, 0x08 /* Private */,
      56,    2,  480,    2, 0x08 /* Private */,
      57,    1,  485,    2, 0x08 /* Private */,
      58,    0,  488,    2, 0x08 /* Private */,
      59,    1,  489,    2, 0x08 /* Private */,
      60,    0,  492,    2, 0x08 /* Private */,
      61,    0,  493,    2, 0x08 /* Private */,
      62,    0,  494,    2, 0x08 /* Private */,
      63,    3,  495,    2, 0x08 /* Private */,
      64,    0,  502,    2, 0x08 /* Private */,
      65,    0,  503,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString, QMetaType::Int,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    2,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,    2,    2,    2,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
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
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,    2,    2,
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
        case 1: _t->calibrate((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 2: _t->updateFileList((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 3: _t->createCalibrateFile((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6]))); break;
        case 4: _t->stopCalibration((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onCalibrationButtonPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onRtuPortActive((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->changeSerialPort((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->toggleLineView_P1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->toggleLineView_P2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->toggleLineView_P3((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->onProductSelected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->onModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->injectionPumpRates((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->injectionBucket((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->injectionMark((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->injectionMethod((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->onActionPressureSensorSlope((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->onMinRefTemp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->onMaxRefTemp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->onInjectionTemp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->onXDelay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->onYFreq((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->onZTemp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->onActionMainServer(); break;
        case 25: _t->onActionLocalServer(); break;
        case 26: _t->onIntervalCalibration((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 27: _t->onIntervalRollover((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 28: _t->createLoopFile((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const int(*)>(_a[6]))); break;
        case 29: _t->initializeToolbarIcons(); break;
        case 30: _t->onLoopTabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->clearMonitors(); break;
        case 32: _t->updateRequestPreview(); break;
        case 33: _t->updateRegisterView(); break;
        case 34: _t->enableHexView(); break;
        case 35: _t->sendModbusRequest(); break;
        case 36: _t->onSendButtonPress(); break;
        case 37: _t->pollForDataOnBus(); break;
        case 38: _t->openBatchProcessor(); break;
        case 39: _t->aboutQModBus(); break;
        case 40: _t->onCheckBoxChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 41: _t->resetStatus(); break;
        case 42: _t->setStatusError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 43: _t->onFloatButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 44: _t->onIntegerButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 45: _t->onCoilButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 46: _t->onReadButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 47: _t->onWriteButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 48: _t->onEquationButtonPressed(); break;
        case 49: _t->loadCsvFile(); break;
        case 50: _t->loadCsvTemplate(); break;
        case 51: _t->onUploadEquation(); break;
        case 52: _t->onDownloadEquation(); break;
        case 53: _t->updateRegisters((*reinterpret_cast< const bool(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 54: _t->onDownloadButtonChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 55: _t->saveCsvFile(); break;
        case 56: _t->onEquationTableChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 57: _t->onUnlockFactoryDefaultBtnPressed(); break;
        case 58: _t->onLockFactoryDefaultBtnPressed(); break;
        case 59: _t->onUpdateFactoryDefaultPressed(); break;
        case 60: _t->updateStability((*reinterpret_cast< const bool(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 61: _t->readJsonConfigFile(); break;
        case 62: _t->writeJsonConfigFile(); break;
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
        if (_id < 63)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 63;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 63)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 63;
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
