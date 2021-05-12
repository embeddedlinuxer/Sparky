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
    QByteArrayData data[84];
    char stringdata0[1445];
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
QT_MOC_LITERAL(7, 86, 15), // "onViewYAxisData"
QT_MOC_LITERAL(8, 102, 14), // "readMasterPipe"
QT_MOC_LITERAL(9, 117, 14), // "isUserInputYes"
QT_MOC_LITERAL(10, 132, 18), // "injectionPumpRates"
QT_MOC_LITERAL(11, 151, 15), // "injectionBucket"
QT_MOC_LITERAL(12, 167, 13), // "injectionMark"
QT_MOC_LITERAL(13, 181, 15), // "injectionMethod"
QT_MOC_LITERAL(14, 197, 27), // "onActionPressureSensorSlope"
QT_MOC_LITERAL(15, 225, 12), // "onMinRefTemp"
QT_MOC_LITERAL(16, 238, 12), // "onMaxRefTemp"
QT_MOC_LITERAL(17, 251, 15), // "onInjectionTemp"
QT_MOC_LITERAL(18, 267, 8), // "onXDelay"
QT_MOC_LITERAL(19, 276, 7), // "onYFreq"
QT_MOC_LITERAL(20, 284, 7), // "onZTemp"
QT_MOC_LITERAL(21, 292, 18), // "onActionMainServer"
QT_MOC_LITERAL(22, 311, 19), // "onActionLocalServer"
QT_MOC_LITERAL(23, 331, 19), // "onIntervalSmallPump"
QT_MOC_LITERAL(24, 351, 17), // "onIntervalBigPump"
QT_MOC_LITERAL(25, 369, 12), // "onLoopNumber"
QT_MOC_LITERAL(26, 382, 17), // "onActionMinMaster"
QT_MOC_LITERAL(27, 400, 17), // "onActionMaxMaster"
QT_MOC_LITERAL(28, 418, 19), // "onActionDeltaMaster"
QT_MOC_LITERAL(29, 438, 24), // "onActionDeltaMasterFinal"
QT_MOC_LITERAL(30, 463, 13), // "onActionWater"
QT_MOC_LITERAL(31, 477, 11), // "onActionOil"
QT_MOC_LITERAL(32, 489, 16), // "onActionSettings"
QT_MOC_LITERAL(33, 506, 13), // "onActionStart"
QT_MOC_LITERAL(34, 520, 12), // "onActionStop"
QT_MOC_LITERAL(35, 533, 19), // "onActionStopPressed"
QT_MOC_LITERAL(36, 553, 12), // "onActionSkip"
QT_MOC_LITERAL(37, 566, 13), // "onActionPause"
QT_MOC_LITERAL(38, 580, 21), // "onActionStopInjection"
QT_MOC_LITERAL(39, 602, 22), // "onActionStartInjection"
QT_MOC_LITERAL(40, 625, 13), // "onModeChanged"
QT_MOC_LITERAL(41, 639, 19), // "onMasterPipeToggled"
QT_MOC_LITERAL(42, 659, 12), // "runInjection"
QT_MOC_LITERAL(43, 672, 10), // "runTempRun"
QT_MOC_LITERAL(44, 683, 15), // "stopCalibration"
QT_MOC_LITERAL(45, 699, 14), // "updateFileList"
QT_MOC_LITERAL(46, 714, 19), // "createInjectionFile"
QT_MOC_LITERAL(47, 734, 16), // "startCalibration"
QT_MOC_LITERAL(48, 751, 15), // "onRtuPortActive"
QT_MOC_LITERAL(49, 767, 16), // "changeSerialPort"
QT_MOC_LITERAL(50, 784, 17), // "createTempRunFile"
QT_MOC_LITERAL(51, 802, 22), // "initializeToolbarIcons"
QT_MOC_LITERAL(52, 825, 13), // "clearMonitors"
QT_MOC_LITERAL(53, 839, 20), // "updateRequestPreview"
QT_MOC_LITERAL(54, 860, 18), // "updateRegisterView"
QT_MOC_LITERAL(55, 879, 13), // "enableHexView"
QT_MOC_LITERAL(56, 893, 17), // "sendModbusRequest"
QT_MOC_LITERAL(57, 911, 17), // "onSendButtonPress"
QT_MOC_LITERAL(58, 929, 16), // "pollForDataOnBus"
QT_MOC_LITERAL(59, 946, 12), // "aboutQModBus"
QT_MOC_LITERAL(60, 959, 17), // "onCheckBoxChecked"
QT_MOC_LITERAL(61, 977, 17), // "onCheckBoxClicked"
QT_MOC_LITERAL(62, 995, 11), // "resetStatus"
QT_MOC_LITERAL(63, 1007, 14), // "setStatusError"
QT_MOC_LITERAL(64, 1022, 20), // "onFloatButtonPressed"
QT_MOC_LITERAL(65, 1043, 22), // "onIntegerButtonPressed"
QT_MOC_LITERAL(66, 1066, 19), // "onCoilButtonPressed"
QT_MOC_LITERAL(67, 1086, 19), // "onReadButtonPressed"
QT_MOC_LITERAL(68, 1106, 20), // "onWriteButtonPressed"
QT_MOC_LITERAL(69, 1127, 23), // "onEquationButtonPressed"
QT_MOC_LITERAL(70, 1151, 11), // "loadCsvFile"
QT_MOC_LITERAL(71, 1163, 15), // "loadCsvTemplate"
QT_MOC_LITERAL(72, 1179, 16), // "onUploadEquation"
QT_MOC_LITERAL(73, 1196, 18), // "onDownloadEquation"
QT_MOC_LITERAL(74, 1215, 17), // "onUpdateRegisters"
QT_MOC_LITERAL(75, 1233, 23), // "onDownloadButtonChecked"
QT_MOC_LITERAL(76, 1257, 11), // "saveCsvFile"
QT_MOC_LITERAL(77, 1269, 22), // "onEquationTableChecked"
QT_MOC_LITERAL(78, 1292, 32), // "onUnlockFactoryDefaultBtnPressed"
QT_MOC_LITERAL(79, 1325, 30), // "onLockFactoryDefaultBtnPressed"
QT_MOC_LITERAL(80, 1356, 29), // "onUpdateFactoryDefaultPressed"
QT_MOC_LITERAL(81, 1386, 19), // "updatePipeStability"
QT_MOC_LITERAL(82, 1406, 18), // "readJsonConfigFile"
QT_MOC_LITERAL(83, 1425, 19) // "writeJsonConfigFile"

    },
    "MainWindow\0connectionError\0\0msg\0"
    "toggleLineView_P1\0toggleLineView_P2\0"
    "toggleLineView_P3\0onViewYAxisData\0"
    "readMasterPipe\0isUserInputYes\0"
    "injectionPumpRates\0injectionBucket\0"
    "injectionMark\0injectionMethod\0"
    "onActionPressureSensorSlope\0onMinRefTemp\0"
    "onMaxRefTemp\0onInjectionTemp\0onXDelay\0"
    "onYFreq\0onZTemp\0onActionMainServer\0"
    "onActionLocalServer\0onIntervalSmallPump\0"
    "onIntervalBigPump\0onLoopNumber\0"
    "onActionMinMaster\0onActionMaxMaster\0"
    "onActionDeltaMaster\0onActionDeltaMasterFinal\0"
    "onActionWater\0onActionOil\0onActionSettings\0"
    "onActionStart\0onActionStop\0"
    "onActionStopPressed\0onActionSkip\0"
    "onActionPause\0onActionStopInjection\0"
    "onActionStartInjection\0onModeChanged\0"
    "onMasterPipeToggled\0runInjection\0"
    "runTempRun\0stopCalibration\0updateFileList\0"
    "createInjectionFile\0startCalibration\0"
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
      81,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  419,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,  422,    2, 0x08 /* Private */,
       5,    1,  425,    2, 0x08 /* Private */,
       6,    1,  428,    2, 0x08 /* Private */,
       7,    1,  431,    2, 0x08 /* Private */,
       8,    0,  434,    2, 0x08 /* Private */,
       9,    2,  435,    2, 0x08 /* Private */,
      10,    0,  440,    2, 0x08 /* Private */,
      11,    0,  441,    2, 0x08 /* Private */,
      12,    0,  442,    2, 0x08 /* Private */,
      13,    0,  443,    2, 0x08 /* Private */,
      14,    0,  444,    2, 0x08 /* Private */,
      15,    0,  445,    2, 0x08 /* Private */,
      16,    0,  446,    2, 0x08 /* Private */,
      17,    0,  447,    2, 0x08 /* Private */,
      18,    0,  448,    2, 0x08 /* Private */,
      19,    0,  449,    2, 0x08 /* Private */,
      20,    0,  450,    2, 0x08 /* Private */,
      21,    0,  451,    2, 0x08 /* Private */,
      22,    0,  452,    2, 0x08 /* Private */,
      23,    0,  453,    2, 0x08 /* Private */,
      24,    0,  454,    2, 0x08 /* Private */,
      25,    0,  455,    2, 0x08 /* Private */,
      26,    0,  456,    2, 0x08 /* Private */,
      27,    0,  457,    2, 0x08 /* Private */,
      28,    0,  458,    2, 0x08 /* Private */,
      29,    0,  459,    2, 0x08 /* Private */,
      30,    0,  460,    2, 0x08 /* Private */,
      31,    0,  461,    2, 0x08 /* Private */,
      32,    0,  462,    2, 0x08 /* Private */,
      33,    0,  463,    2, 0x08 /* Private */,
      34,    0,  464,    2, 0x08 /* Private */,
      35,    0,  465,    2, 0x08 /* Private */,
      36,    0,  466,    2, 0x08 /* Private */,
      37,    0,  467,    2, 0x08 /* Private */,
      38,    0,  468,    2, 0x08 /* Private */,
      39,    0,  469,    2, 0x08 /* Private */,
      40,    1,  470,    2, 0x08 /* Private */,
      41,    1,  473,    2, 0x08 /* Private */,
      42,    0,  476,    2, 0x08 /* Private */,
      43,    0,  477,    2, 0x08 /* Private */,
      44,    0,  478,    2, 0x08 /* Private */,
      45,    3,  479,    2, 0x08 /* Private */,
      46,    6,  486,    2, 0x08 /* Private */,
      47,    0,  499,    2, 0x08 /* Private */,
      48,    1,  500,    2, 0x08 /* Private */,
      49,    1,  503,    2, 0x08 /* Private */,
      50,    5,  506,    2, 0x08 /* Private */,
      51,    0,  517,    2, 0x08 /* Private */,
      52,    0,  518,    2, 0x08 /* Private */,
      53,    0,  519,    2, 0x08 /* Private */,
      54,    0,  520,    2, 0x08 /* Private */,
      55,    0,  521,    2, 0x08 /* Private */,
      56,    0,  522,    2, 0x08 /* Private */,
      57,    0,  523,    2, 0x08 /* Private */,
      58,    0,  524,    2, 0x08 /* Private */,
      59,    0,  525,    2, 0x08 /* Private */,
      60,    1,  526,    2, 0x08 /* Private */,
      61,    1,  529,    2, 0x08 /* Private */,
      62,    0,  532,    2, 0x08 /* Private */,
      63,    1,  533,    2, 0x08 /* Private */,
      64,    1,  536,    2, 0x08 /* Private */,
      65,    1,  539,    2, 0x08 /* Private */,
      66,    1,  542,    2, 0x08 /* Private */,
      67,    1,  545,    2, 0x08 /* Private */,
      68,    1,  548,    2, 0x08 /* Private */,
      69,    0,  551,    2, 0x08 /* Private */,
      70,    0,  552,    2, 0x08 /* Private */,
      71,    0,  553,    2, 0x08 /* Private */,
      72,    0,  554,    2, 0x08 /* Private */,
      73,    0,  555,    2, 0x08 /* Private */,
      74,    1,  556,    2, 0x08 /* Private */,
      75,    1,  559,    2, 0x08 /* Private */,
      76,    0,  562,    2, 0x08 /* Private */,
      77,    1,  563,    2, 0x08 /* Private */,
      78,    0,  566,    2, 0x08 /* Private */,
      79,    0,  567,    2, 0x08 /* Private */,
      80,    0,  568,    2, 0x08 /* Private */,
      81,    2,  569,    2, 0x08 /* Private */,
      82,    0,  574,    2, 0x08 /* Private */,
      83,    0,  575,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,    2,    2,
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
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    2,    2,
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
        case 4: _t->onViewYAxisData((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->readMasterPipe(); break;
        case 6: { bool _r = _t->isUserInputYes((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->injectionPumpRates(); break;
        case 8: _t->injectionBucket(); break;
        case 9: _t->injectionMark(); break;
        case 10: _t->injectionMethod(); break;
        case 11: _t->onActionPressureSensorSlope(); break;
        case 12: _t->onMinRefTemp(); break;
        case 13: _t->onMaxRefTemp(); break;
        case 14: _t->onInjectionTemp(); break;
        case 15: _t->onXDelay(); break;
        case 16: _t->onYFreq(); break;
        case 17: _t->onZTemp(); break;
        case 18: _t->onActionMainServer(); break;
        case 19: _t->onActionLocalServer(); break;
        case 20: _t->onIntervalSmallPump(); break;
        case 21: _t->onIntervalBigPump(); break;
        case 22: _t->onLoopNumber(); break;
        case 23: _t->onActionMinMaster(); break;
        case 24: _t->onActionMaxMaster(); break;
        case 25: _t->onActionDeltaMaster(); break;
        case 26: _t->onActionDeltaMasterFinal(); break;
        case 27: _t->onActionWater(); break;
        case 28: _t->onActionOil(); break;
        case 29: _t->onActionSettings(); break;
        case 30: _t->onActionStart(); break;
        case 31: _t->onActionStop(); break;
        case 32: _t->onActionStopPressed(); break;
        case 33: _t->onActionSkip(); break;
        case 34: _t->onActionPause(); break;
        case 35: _t->onActionStopInjection(); break;
        case 36: _t->onActionStartInjection(); break;
        case 37: _t->onModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 38: _t->onMasterPipeToggled((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 39: _t->runInjection(); break;
        case 40: _t->runTempRun(); break;
        case 41: _t->stopCalibration(); break;
        case 42: _t->updateFileList((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 43: _t->createInjectionFile((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6]))); break;
        case 44: _t->startCalibration(); break;
        case 45: _t->onRtuPortActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 46: _t->changeSerialPort((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 47: _t->createTempRunFile((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const int(*)>(_a[5]))); break;
        case 48: _t->initializeToolbarIcons(); break;
        case 49: _t->clearMonitors(); break;
        case 50: _t->updateRequestPreview(); break;
        case 51: _t->updateRegisterView(); break;
        case 52: _t->enableHexView(); break;
        case 53: _t->sendModbusRequest(); break;
        case 54: _t->onSendButtonPress(); break;
        case 55: _t->pollForDataOnBus(); break;
        case 56: _t->aboutQModBus(); break;
        case 57: _t->onCheckBoxChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 58: _t->onCheckBoxClicked((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 59: _t->resetStatus(); break;
        case 60: _t->setStatusError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 61: _t->onFloatButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 62: _t->onIntegerButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 63: _t->onCoilButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 64: _t->onReadButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 65: _t->onWriteButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 66: _t->onEquationButtonPressed(); break;
        case 67: _t->loadCsvFile(); break;
        case 68: _t->loadCsvTemplate(); break;
        case 69: _t->onUploadEquation(); break;
        case 70: _t->onDownloadEquation(); break;
        case 71: _t->onUpdateRegisters((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 72: _t->onDownloadButtonChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 73: _t->saveCsvFile(); break;
        case 74: _t->onEquationTableChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 75: _t->onUnlockFactoryDefaultBtnPressed(); break;
        case 76: _t->onLockFactoryDefaultBtnPressed(); break;
        case 77: _t->onUpdateFactoryDefaultPressed(); break;
        case 78: _t->updatePipeStability((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        case 79: _t->readJsonConfigFile(); break;
        case 80: _t->writeJsonConfigFile(); break;
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
        if (_id < 81)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 81;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 81)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 81;
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
