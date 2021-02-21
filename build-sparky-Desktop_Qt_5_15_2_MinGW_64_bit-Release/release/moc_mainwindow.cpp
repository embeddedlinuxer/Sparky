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
    QByteArrayData data[293];
    char stringdata0[6703];
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
QT_MOC_LITERAL(4, 32, 15), // "createLoopFiles"
QT_MOC_LITERAL(5, 48, 7), // "WINBOOL"
QT_MOC_LITERAL(6, 56, 6), // "QFile&"
QT_MOC_LITERAL(7, 63, 15), // "onRtuPortActive"
QT_MOC_LITERAL(8, 79, 17), // "onRtuPortActive_2"
QT_MOC_LITERAL(9, 97, 17), // "onRtuPortActive_3"
QT_MOC_LITERAL(10, 115, 17), // "onRtuPortActive_4"
QT_MOC_LITERAL(11, 133, 17), // "onRtuPortActive_5"
QT_MOC_LITERAL(12, 151, 17), // "onRtuPortActive_6"
QT_MOC_LITERAL(13, 169, 16), // "changeSerialPort"
QT_MOC_LITERAL(14, 186, 18), // "changeSerialPort_2"
QT_MOC_LITERAL(15, 205, 18), // "changeSerialPort_3"
QT_MOC_LITERAL(16, 224, 18), // "changeSerialPort_4"
QT_MOC_LITERAL(17, 243, 18), // "changeSerialPort_5"
QT_MOC_LITERAL(18, 262, 18), // "changeSerialPort_6"
QT_MOC_LITERAL(19, 281, 16), // "calibration_L1P1"
QT_MOC_LITERAL(20, 298, 16), // "calibration_L1P2"
QT_MOC_LITERAL(21, 315, 16), // "calibration_L1P3"
QT_MOC_LITERAL(22, 332, 16), // "calibration_L2P1"
QT_MOC_LITERAL(23, 349, 16), // "calibration_L2P2"
QT_MOC_LITERAL(24, 366, 16), // "calibration_L2P3"
QT_MOC_LITERAL(25, 383, 16), // "calibration_L3P1"
QT_MOC_LITERAL(26, 400, 16), // "calibration_L3P2"
QT_MOC_LITERAL(27, 417, 16), // "calibration_L3P3"
QT_MOC_LITERAL(28, 434, 16), // "calibration_L4P1"
QT_MOC_LITERAL(29, 451, 16), // "calibration_L4P2"
QT_MOC_LITERAL(30, 468, 16), // "calibration_L4P3"
QT_MOC_LITERAL(31, 485, 16), // "calibration_L5P1"
QT_MOC_LITERAL(32, 502, 16), // "calibration_L5P2"
QT_MOC_LITERAL(33, 519, 16), // "calibration_L5P3"
QT_MOC_LITERAL(34, 536, 16), // "calibration_L6P1"
QT_MOC_LITERAL(35, 553, 16), // "calibration_L6P2"
QT_MOC_LITERAL(36, 570, 16), // "calibration_L6P3"
QT_MOC_LITERAL(37, 587, 22), // "initializeToolbarIcons"
QT_MOC_LITERAL(38, 610, 24), // "initializeFrequencyGauge"
QT_MOC_LITERAL(39, 635, 26), // "initializeTemperatureGauge"
QT_MOC_LITERAL(40, 662, 22), // "initializeDensityGauge"
QT_MOC_LITERAL(41, 685, 17), // "initializeRPGauge"
QT_MOC_LITERAL(42, 703, 20), // "updateFrequencyGauge"
QT_MOC_LITERAL(43, 724, 22), // "updateTemperatureGauge"
QT_MOC_LITERAL(44, 747, 18), // "updateDensityGauge"
QT_MOC_LITERAL(45, 766, 13), // "updateRPGauge"
QT_MOC_LITERAL(46, 780, 16), // "onLoopTabChanged"
QT_MOC_LITERAL(47, 797, 13), // "clearMonitors"
QT_MOC_LITERAL(48, 811, 20), // "updateRequestPreview"
QT_MOC_LITERAL(49, 832, 18), // "updateRegisterView"
QT_MOC_LITERAL(50, 851, 13), // "enableHexView"
QT_MOC_LITERAL(51, 865, 17), // "sendModbusRequest"
QT_MOC_LITERAL(52, 883, 17), // "onSendButtonPress"
QT_MOC_LITERAL(53, 901, 16), // "pollForDataOnBus"
QT_MOC_LITERAL(54, 918, 18), // "openBatchProcessor"
QT_MOC_LITERAL(55, 937, 12), // "aboutQModBus"
QT_MOC_LITERAL(56, 950, 17), // "onCheckBoxChecked"
QT_MOC_LITERAL(57, 968, 11), // "resetStatus"
QT_MOC_LITERAL(58, 980, 14), // "setStatusError"
QT_MOC_LITERAL(59, 995, 20), // "onFloatButtonPressed"
QT_MOC_LITERAL(60, 1016, 22), // "onIntegerButtonPressed"
QT_MOC_LITERAL(61, 1039, 19), // "onCoilButtonPressed"
QT_MOC_LITERAL(62, 1059, 19), // "onReadButtonPressed"
QT_MOC_LITERAL(63, 1079, 20), // "onWriteButtonPressed"
QT_MOC_LITERAL(64, 1100, 23), // "onEquationButtonPressed"
QT_MOC_LITERAL(65, 1124, 11), // "loadCsvFile"
QT_MOC_LITERAL(66, 1136, 15), // "loadCsvTemplate"
QT_MOC_LITERAL(67, 1152, 16), // "onUploadEquation"
QT_MOC_LITERAL(68, 1169, 18), // "onDownloadEquation"
QT_MOC_LITERAL(69, 1188, 15), // "updateRegisters"
QT_MOC_LITERAL(70, 1204, 23), // "onDownloadButtonChecked"
QT_MOC_LITERAL(71, 1228, 11), // "saveCsvFile"
QT_MOC_LITERAL(72, 1240, 22), // "onEquationTableChecked"
QT_MOC_LITERAL(73, 1263, 32), // "onUnlockFactoryDefaultBtnPressed"
QT_MOC_LITERAL(74, 1296, 30), // "onLockFactoryDefaultBtnPressed"
QT_MOC_LITERAL(75, 1327, 29), // "onUpdateFactoryDefaultPressed"
QT_MOC_LITERAL(76, 1357, 20), // "onRadioButtonPressed"
QT_MOC_LITERAL(77, 1378, 22), // "onRadioButton_2Pressed"
QT_MOC_LITERAL(78, 1401, 22), // "onRadioButton_3Pressed"
QT_MOC_LITERAL(79, 1424, 22), // "onRadioButton_4Pressed"
QT_MOC_LITERAL(80, 1447, 22), // "onRadioButton_5Pressed"
QT_MOC_LITERAL(81, 1470, 22), // "onRadioButton_6Pressed"
QT_MOC_LITERAL(82, 1493, 22), // "onRadioButton_7Pressed"
QT_MOC_LITERAL(83, 1516, 22), // "onRadioButton_8Pressed"
QT_MOC_LITERAL(84, 1539, 22), // "onRadioButton_9Pressed"
QT_MOC_LITERAL(85, 1562, 23), // "onRadioButton_10Pressed"
QT_MOC_LITERAL(86, 1586, 23), // "onRadioButton_11Pressed"
QT_MOC_LITERAL(87, 1610, 23), // "onRadioButton_12Pressed"
QT_MOC_LITERAL(88, 1634, 23), // "onRadioButton_13Pressed"
QT_MOC_LITERAL(89, 1658, 23), // "onRadioButton_22Pressed"
QT_MOC_LITERAL(90, 1682, 23), // "onRadioButton_23Pressed"
QT_MOC_LITERAL(91, 1706, 23), // "onRadioButton_24Pressed"
QT_MOC_LITERAL(92, 1730, 23), // "onRadioButton_37Pressed"
QT_MOC_LITERAL(93, 1754, 23), // "onRadioButton_38Pressed"
QT_MOC_LITERAL(94, 1778, 23), // "onRadioButton_39Pressed"
QT_MOC_LITERAL(95, 1802, 23), // "onRadioButton_40Pressed"
QT_MOC_LITERAL(96, 1826, 23), // "onRadioButton_41Pressed"
QT_MOC_LITERAL(97, 1850, 23), // "onRadioButton_42Pressed"
QT_MOC_LITERAL(98, 1874, 23), // "onRadioButton_43Pressed"
QT_MOC_LITERAL(99, 1898, 23), // "onRadioButton_44Pressed"
QT_MOC_LITERAL(100, 1922, 23), // "onRadioButton_69Pressed"
QT_MOC_LITERAL(101, 1946, 23), // "onRadioButton_70Pressed"
QT_MOC_LITERAL(102, 1970, 23), // "onRadioButton_71Pressed"
QT_MOC_LITERAL(103, 1994, 23), // "onRadioButton_72Pressed"
QT_MOC_LITERAL(104, 2018, 23), // "onRadioButton_73Pressed"
QT_MOC_LITERAL(105, 2042, 23), // "onRadioButton_74Pressed"
QT_MOC_LITERAL(106, 2066, 23), // "onRadioButton_75Pressed"
QT_MOC_LITERAL(107, 2090, 23), // "onRadioButton_76Pressed"
QT_MOC_LITERAL(108, 2114, 23), // "onRadioButton_77Pressed"
QT_MOC_LITERAL(109, 2138, 23), // "onRadioButton_78Pressed"
QT_MOC_LITERAL(110, 2162, 23), // "onRadioButton_79Pressed"
QT_MOC_LITERAL(111, 2186, 23), // "onRadioButton_80Pressed"
QT_MOC_LITERAL(112, 2210, 23), // "onRadioButton_81Pressed"
QT_MOC_LITERAL(113, 2234, 23), // "onRadioButton_82Pressed"
QT_MOC_LITERAL(114, 2258, 23), // "onRadioButton_83Pressed"
QT_MOC_LITERAL(115, 2282, 23), // "onRadioButton_84Pressed"
QT_MOC_LITERAL(116, 2306, 23), // "onRadioButton_85Pressed"
QT_MOC_LITERAL(117, 2330, 23), // "onRadioButton_86Pressed"
QT_MOC_LITERAL(118, 2354, 23), // "onRadioButton_87Pressed"
QT_MOC_LITERAL(119, 2378, 23), // "onRadioButton_88Pressed"
QT_MOC_LITERAL(120, 2402, 23), // "onRadioButton_89Pressed"
QT_MOC_LITERAL(121, 2426, 23), // "onRadioButton_90Pressed"
QT_MOC_LITERAL(122, 2450, 23), // "onRadioButton_91Pressed"
QT_MOC_LITERAL(123, 2474, 23), // "onRadioButton_92Pressed"
QT_MOC_LITERAL(124, 2498, 23), // "onRadioButton_93Pressed"
QT_MOC_LITERAL(125, 2522, 23), // "onRadioButton_94Pressed"
QT_MOC_LITERAL(126, 2546, 23), // "onRadioButton_95Pressed"
QT_MOC_LITERAL(127, 2570, 23), // "onRadioButton_96Pressed"
QT_MOC_LITERAL(128, 2594, 23), // "onRadioButton_97Pressed"
QT_MOC_LITERAL(129, 2618, 23), // "onRadioButton_98Pressed"
QT_MOC_LITERAL(130, 2642, 23), // "onRadioButton_99Pressed"
QT_MOC_LITERAL(131, 2666, 24), // "onRadioButton_100Pressed"
QT_MOC_LITERAL(132, 2691, 24), // "onRadioButton_101Pressed"
QT_MOC_LITERAL(133, 2716, 24), // "onRadioButton_102Pressed"
QT_MOC_LITERAL(134, 2741, 24), // "onRadioButton_103Pressed"
QT_MOC_LITERAL(135, 2766, 24), // "onRadioButton_104Pressed"
QT_MOC_LITERAL(136, 2791, 24), // "onRadioButton_105Pressed"
QT_MOC_LITERAL(137, 2816, 24), // "onRadioButton_106Pressed"
QT_MOC_LITERAL(138, 2841, 24), // "onRadioButton_107Pressed"
QT_MOC_LITERAL(139, 2866, 24), // "onRadioButton_108Pressed"
QT_MOC_LITERAL(140, 2891, 24), // "onRadioButton_109Pressed"
QT_MOC_LITERAL(141, 2916, 24), // "onRadioButton_110Pressed"
QT_MOC_LITERAL(142, 2941, 24), // "onRadioButton_111Pressed"
QT_MOC_LITERAL(143, 2966, 24), // "onRadioButton_112Pressed"
QT_MOC_LITERAL(144, 2991, 24), // "onRadioButton_113Pressed"
QT_MOC_LITERAL(145, 3016, 24), // "onRadioButton_114Pressed"
QT_MOC_LITERAL(146, 3041, 24), // "onRadioButton_115Pressed"
QT_MOC_LITERAL(147, 3066, 24), // "onRadioButton_116Pressed"
QT_MOC_LITERAL(148, 3091, 24), // "onRadioButton_117Pressed"
QT_MOC_LITERAL(149, 3116, 24), // "onRadioButton_118Pressed"
QT_MOC_LITERAL(150, 3141, 24), // "onRadioButton_119Pressed"
QT_MOC_LITERAL(151, 3166, 24), // "onRadioButton_120Pressed"
QT_MOC_LITERAL(152, 3191, 24), // "onRadioButton_121Pressed"
QT_MOC_LITERAL(153, 3216, 24), // "onRadioButton_122Pressed"
QT_MOC_LITERAL(154, 3241, 24), // "onRadioButton_123Pressed"
QT_MOC_LITERAL(155, 3266, 24), // "onRadioButton_124Pressed"
QT_MOC_LITERAL(156, 3291, 24), // "onRadioButton_125Pressed"
QT_MOC_LITERAL(157, 3316, 24), // "onRadioButton_126Pressed"
QT_MOC_LITERAL(158, 3341, 24), // "onRadioButton_127Pressed"
QT_MOC_LITERAL(159, 3366, 24), // "onRadioButton_128Pressed"
QT_MOC_LITERAL(160, 3391, 24), // "onRadioButton_129Pressed"
QT_MOC_LITERAL(161, 3416, 24), // "onRadioButton_130Pressed"
QT_MOC_LITERAL(162, 3441, 24), // "onRadioButton_131Pressed"
QT_MOC_LITERAL(163, 3466, 24), // "onRadioButton_132Pressed"
QT_MOC_LITERAL(164, 3491, 24), // "onRadioButton_133Pressed"
QT_MOC_LITERAL(165, 3516, 24), // "onRadioButton_134Pressed"
QT_MOC_LITERAL(166, 3541, 24), // "onRadioButton_135Pressed"
QT_MOC_LITERAL(167, 3566, 24), // "onRadioButton_136Pressed"
QT_MOC_LITERAL(168, 3591, 24), // "onRadioButton_137Pressed"
QT_MOC_LITERAL(169, 3616, 24), // "onRadioButton_138Pressed"
QT_MOC_LITERAL(170, 3641, 24), // "onRadioButton_139Pressed"
QT_MOC_LITERAL(171, 3666, 24), // "onRadioButton_140Pressed"
QT_MOC_LITERAL(172, 3691, 24), // "onRadioButton_141Pressed"
QT_MOC_LITERAL(173, 3716, 24), // "onRadioButton_142Pressed"
QT_MOC_LITERAL(174, 3741, 24), // "onRadioButton_143Pressed"
QT_MOC_LITERAL(175, 3766, 24), // "onRadioButton_144Pressed"
QT_MOC_LITERAL(176, 3791, 24), // "onRadioButton_145Pressed"
QT_MOC_LITERAL(177, 3816, 24), // "onRadioButton_146Pressed"
QT_MOC_LITERAL(178, 3841, 24), // "onRadioButton_147Pressed"
QT_MOC_LITERAL(179, 3866, 24), // "onRadioButton_148Pressed"
QT_MOC_LITERAL(180, 3891, 24), // "onRadioButton_149Pressed"
QT_MOC_LITERAL(181, 3916, 24), // "onRadioButton_150Pressed"
QT_MOC_LITERAL(182, 3941, 24), // "onRadioButton_151Pressed"
QT_MOC_LITERAL(183, 3966, 24), // "onRadioButton_152Pressed"
QT_MOC_LITERAL(184, 3991, 24), // "onRadioButton_153Pressed"
QT_MOC_LITERAL(185, 4016, 24), // "onRadioButton_154Pressed"
QT_MOC_LITERAL(186, 4041, 24), // "onRadioButton_155Pressed"
QT_MOC_LITERAL(187, 4066, 24), // "onRadioButton_156Pressed"
QT_MOC_LITERAL(188, 4091, 24), // "onRadioButton_157Pressed"
QT_MOC_LITERAL(189, 4116, 24), // "onRadioButton_158Pressed"
QT_MOC_LITERAL(190, 4141, 24), // "onRadioButton_159Pressed"
QT_MOC_LITERAL(191, 4166, 24), // "onRadioButton_160Pressed"
QT_MOC_LITERAL(192, 4191, 24), // "onRadioButton_161Pressed"
QT_MOC_LITERAL(193, 4216, 24), // "onRadioButton_162Pressed"
QT_MOC_LITERAL(194, 4241, 24), // "onRadioButton_163Pressed"
QT_MOC_LITERAL(195, 4266, 24), // "onRadioButton_164Pressed"
QT_MOC_LITERAL(196, 4291, 24), // "onRadioButton_165Pressed"
QT_MOC_LITERAL(197, 4316, 24), // "onRadioButton_166Pressed"
QT_MOC_LITERAL(198, 4341, 24), // "onRadioButton_167Pressed"
QT_MOC_LITERAL(199, 4366, 24), // "onRadioButton_168Pressed"
QT_MOC_LITERAL(200, 4391, 24), // "onRadioButton_169Pressed"
QT_MOC_LITERAL(201, 4416, 24), // "onRadioButton_170Pressed"
QT_MOC_LITERAL(202, 4441, 24), // "onRadioButton_171Pressed"
QT_MOC_LITERAL(203, 4466, 24), // "onRadioButton_172Pressed"
QT_MOC_LITERAL(204, 4491, 24), // "onRadioButton_173Pressed"
QT_MOC_LITERAL(205, 4516, 24), // "onRadioButton_174Pressed"
QT_MOC_LITERAL(206, 4541, 24), // "onRadioButton_175Pressed"
QT_MOC_LITERAL(207, 4566, 24), // "onRadioButton_176Pressed"
QT_MOC_LITERAL(208, 4591, 24), // "onRadioButton_177Pressed"
QT_MOC_LITERAL(209, 4616, 24), // "onRadioButton_178Pressed"
QT_MOC_LITERAL(210, 4641, 24), // "onRadioButton_179Pressed"
QT_MOC_LITERAL(211, 4666, 24), // "onRadioButton_180Pressed"
QT_MOC_LITERAL(212, 4691, 24), // "onRadioButton_198Pressed"
QT_MOC_LITERAL(213, 4716, 24), // "onRadioButton_199Pressed"
QT_MOC_LITERAL(214, 4741, 24), // "onRadioButton_232Pressed"
QT_MOC_LITERAL(215, 4766, 24), // "onRadioButton_233Pressed"
QT_MOC_LITERAL(216, 4791, 24), // "onRadioButton_238Pressed"
QT_MOC_LITERAL(217, 4816, 24), // "onRadioButton_239Pressed"
QT_MOC_LITERAL(218, 4841, 24), // "onRadioButton_240Pressed"
QT_MOC_LITERAL(219, 4866, 24), // "onRadioButton_241Pressed"
QT_MOC_LITERAL(220, 4891, 24), // "onRadioButton_242Pressed"
QT_MOC_LITERAL(221, 4916, 24), // "onRadioButton_243Pressed"
QT_MOC_LITERAL(222, 4941, 24), // "onRadioButton_244Pressed"
QT_MOC_LITERAL(223, 4966, 24), // "onRadioButton_245Pressed"
QT_MOC_LITERAL(224, 4991, 24), // "onRadioButton_246Pressed"
QT_MOC_LITERAL(225, 5016, 24), // "onRadioButton_247Pressed"
QT_MOC_LITERAL(226, 5041, 24), // "onRadioButton_248Pressed"
QT_MOC_LITERAL(227, 5066, 24), // "onRadioButton_249Pressed"
QT_MOC_LITERAL(228, 5091, 24), // "onRadioButton_250Pressed"
QT_MOC_LITERAL(229, 5116, 24), // "onRadioButton_251Pressed"
QT_MOC_LITERAL(230, 5141, 24), // "onRadioButton_252Pressed"
QT_MOC_LITERAL(231, 5166, 24), // "onRadioButton_253Pressed"
QT_MOC_LITERAL(232, 5191, 24), // "onRadioButton_254Pressed"
QT_MOC_LITERAL(233, 5216, 24), // "onRadioButton_255Pressed"
QT_MOC_LITERAL(234, 5241, 24), // "onRadioButton_256Pressed"
QT_MOC_LITERAL(235, 5266, 24), // "onRadioButton_257Pressed"
QT_MOC_LITERAL(236, 5291, 24), // "onRadioButton_258Pressed"
QT_MOC_LITERAL(237, 5316, 24), // "onRadioButton_259Pressed"
QT_MOC_LITERAL(238, 5341, 24), // "onRadioButton_260Pressed"
QT_MOC_LITERAL(239, 5366, 24), // "onRadioButton_261Pressed"
QT_MOC_LITERAL(240, 5391, 24), // "onRadioButton_262Pressed"
QT_MOC_LITERAL(241, 5416, 24), // "onRadioButton_263Pressed"
QT_MOC_LITERAL(242, 5441, 24), // "onRadioButton_264Pressed"
QT_MOC_LITERAL(243, 5466, 24), // "onRadioButton_265Pressed"
QT_MOC_LITERAL(244, 5491, 24), // "onRadioButton_266Pressed"
QT_MOC_LITERAL(245, 5516, 24), // "onRadioButton_267Pressed"
QT_MOC_LITERAL(246, 5541, 24), // "onRadioButton_268Pressed"
QT_MOC_LITERAL(247, 5566, 24), // "onRadioButton_269Pressed"
QT_MOC_LITERAL(248, 5591, 24), // "onRadioButton_270Pressed"
QT_MOC_LITERAL(249, 5616, 24), // "onRadioButton_271Pressed"
QT_MOC_LITERAL(250, 5641, 24), // "onRadioButton_272Pressed"
QT_MOC_LITERAL(251, 5666, 24), // "onRadioButton_273Pressed"
QT_MOC_LITERAL(252, 5691, 24), // "onRadioButton_194Pressed"
QT_MOC_LITERAL(253, 5716, 24), // "onRadioButton_195Pressed"
QT_MOC_LITERAL(254, 5741, 24), // "onRadioButton_196Pressed"
QT_MOC_LITERAL(255, 5766, 24), // "onRadioButton_197Pressed"
QT_MOC_LITERAL(256, 5791, 24), // "onRadioButton_200Pressed"
QT_MOC_LITERAL(257, 5816, 24), // "onRadioButton_201Pressed"
QT_MOC_LITERAL(258, 5841, 24), // "onRadioButton_202Pressed"
QT_MOC_LITERAL(259, 5866, 24), // "onRadioButton_203Pressed"
QT_MOC_LITERAL(260, 5891, 24), // "onRadioButton_204Pressed"
QT_MOC_LITERAL(261, 5916, 24), // "onRadioButton_205Pressed"
QT_MOC_LITERAL(262, 5941, 24), // "onRadioButton_206Pressed"
QT_MOC_LITERAL(263, 5966, 24), // "onRadioButton_207Pressed"
QT_MOC_LITERAL(264, 5991, 24), // "onRadioButton_208Pressed"
QT_MOC_LITERAL(265, 6016, 24), // "onRadioButton_209Pressed"
QT_MOC_LITERAL(266, 6041, 24), // "onRadioButton_210Pressed"
QT_MOC_LITERAL(267, 6066, 24), // "onRadioButton_211Pressed"
QT_MOC_LITERAL(268, 6091, 24), // "onRadioButton_212Pressed"
QT_MOC_LITERAL(269, 6116, 24), // "onRadioButton_213Pressed"
QT_MOC_LITERAL(270, 6141, 24), // "onRadioButton_214Pressed"
QT_MOC_LITERAL(271, 6166, 24), // "onRadioButton_215Pressed"
QT_MOC_LITERAL(272, 6191, 24), // "onRadioButton_216Pressed"
QT_MOC_LITERAL(273, 6216, 24), // "onRadioButton_217Pressed"
QT_MOC_LITERAL(274, 6241, 24), // "onRadioButton_218Pressed"
QT_MOC_LITERAL(275, 6266, 24), // "onRadioButton_219Pressed"
QT_MOC_LITERAL(276, 6291, 24), // "onRadioButton_220Pressed"
QT_MOC_LITERAL(277, 6316, 24), // "onRadioButton_221Pressed"
QT_MOC_LITERAL(278, 6341, 24), // "onRadioButton_222Pressed"
QT_MOC_LITERAL(279, 6366, 24), // "onRadioButton_223Pressed"
QT_MOC_LITERAL(280, 6391, 24), // "onRadioButton_224Pressed"
QT_MOC_LITERAL(281, 6416, 24), // "onRadioButton_225Pressed"
QT_MOC_LITERAL(282, 6441, 24), // "onRadioButton_226Pressed"
QT_MOC_LITERAL(283, 6466, 24), // "onRadioButton_227Pressed"
QT_MOC_LITERAL(284, 6491, 24), // "onRadioButton_228Pressed"
QT_MOC_LITERAL(285, 6516, 24), // "onRadioButton_229Pressed"
QT_MOC_LITERAL(286, 6541, 24), // "onRadioButton_230Pressed"
QT_MOC_LITERAL(287, 6566, 24), // "onRadioButton_231Pressed"
QT_MOC_LITERAL(288, 6591, 24), // "onRadioButton_234Pressed"
QT_MOC_LITERAL(289, 6616, 24), // "onRadioButton_235Pressed"
QT_MOC_LITERAL(290, 6641, 24), // "onRadioButton_236Pressed"
QT_MOC_LITERAL(291, 6666, 24), // "onRadioButton_237Pressed"
QT_MOC_LITERAL(292, 6691, 11) // "updateGraph"

    },
    "MainWindow\0connectionError\0\0msg\0"
    "createLoopFiles\0WINBOOL\0QFile&\0"
    "onRtuPortActive\0onRtuPortActive_2\0"
    "onRtuPortActive_3\0onRtuPortActive_4\0"
    "onRtuPortActive_5\0onRtuPortActive_6\0"
    "changeSerialPort\0changeSerialPort_2\0"
    "changeSerialPort_3\0changeSerialPort_4\0"
    "changeSerialPort_5\0changeSerialPort_6\0"
    "calibration_L1P1\0calibration_L1P2\0"
    "calibration_L1P3\0calibration_L2P1\0"
    "calibration_L2P2\0calibration_L2P3\0"
    "calibration_L3P1\0calibration_L3P2\0"
    "calibration_L3P3\0calibration_L4P1\0"
    "calibration_L4P2\0calibration_L4P3\0"
    "calibration_L5P1\0calibration_L5P2\0"
    "calibration_L5P3\0calibration_L6P1\0"
    "calibration_L6P2\0calibration_L6P3\0"
    "initializeToolbarIcons\0initializeFrequencyGauge\0"
    "initializeTemperatureGauge\0"
    "initializeDensityGauge\0initializeRPGauge\0"
    "updateFrequencyGauge\0updateTemperatureGauge\0"
    "updateDensityGauge\0updateRPGauge\0"
    "onLoopTabChanged\0clearMonitors\0"
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
    "onDownloadEquation\0updateRegisters\0"
    "onDownloadButtonChecked\0saveCsvFile\0"
    "onEquationTableChecked\0"
    "onUnlockFactoryDefaultBtnPressed\0"
    "onLockFactoryDefaultBtnPressed\0"
    "onUpdateFactoryDefaultPressed\0"
    "onRadioButtonPressed\0onRadioButton_2Pressed\0"
    "onRadioButton_3Pressed\0onRadioButton_4Pressed\0"
    "onRadioButton_5Pressed\0onRadioButton_6Pressed\0"
    "onRadioButton_7Pressed\0onRadioButton_8Pressed\0"
    "onRadioButton_9Pressed\0onRadioButton_10Pressed\0"
    "onRadioButton_11Pressed\0onRadioButton_12Pressed\0"
    "onRadioButton_13Pressed\0onRadioButton_22Pressed\0"
    "onRadioButton_23Pressed\0onRadioButton_24Pressed\0"
    "onRadioButton_37Pressed\0onRadioButton_38Pressed\0"
    "onRadioButton_39Pressed\0onRadioButton_40Pressed\0"
    "onRadioButton_41Pressed\0onRadioButton_42Pressed\0"
    "onRadioButton_43Pressed\0onRadioButton_44Pressed\0"
    "onRadioButton_69Pressed\0onRadioButton_70Pressed\0"
    "onRadioButton_71Pressed\0onRadioButton_72Pressed\0"
    "onRadioButton_73Pressed\0onRadioButton_74Pressed\0"
    "onRadioButton_75Pressed\0onRadioButton_76Pressed\0"
    "onRadioButton_77Pressed\0onRadioButton_78Pressed\0"
    "onRadioButton_79Pressed\0onRadioButton_80Pressed\0"
    "onRadioButton_81Pressed\0onRadioButton_82Pressed\0"
    "onRadioButton_83Pressed\0onRadioButton_84Pressed\0"
    "onRadioButton_85Pressed\0onRadioButton_86Pressed\0"
    "onRadioButton_87Pressed\0onRadioButton_88Pressed\0"
    "onRadioButton_89Pressed\0onRadioButton_90Pressed\0"
    "onRadioButton_91Pressed\0onRadioButton_92Pressed\0"
    "onRadioButton_93Pressed\0onRadioButton_94Pressed\0"
    "onRadioButton_95Pressed\0onRadioButton_96Pressed\0"
    "onRadioButton_97Pressed\0onRadioButton_98Pressed\0"
    "onRadioButton_99Pressed\0"
    "onRadioButton_100Pressed\0"
    "onRadioButton_101Pressed\0"
    "onRadioButton_102Pressed\0"
    "onRadioButton_103Pressed\0"
    "onRadioButton_104Pressed\0"
    "onRadioButton_105Pressed\0"
    "onRadioButton_106Pressed\0"
    "onRadioButton_107Pressed\0"
    "onRadioButton_108Pressed\0"
    "onRadioButton_109Pressed\0"
    "onRadioButton_110Pressed\0"
    "onRadioButton_111Pressed\0"
    "onRadioButton_112Pressed\0"
    "onRadioButton_113Pressed\0"
    "onRadioButton_114Pressed\0"
    "onRadioButton_115Pressed\0"
    "onRadioButton_116Pressed\0"
    "onRadioButton_117Pressed\0"
    "onRadioButton_118Pressed\0"
    "onRadioButton_119Pressed\0"
    "onRadioButton_120Pressed\0"
    "onRadioButton_121Pressed\0"
    "onRadioButton_122Pressed\0"
    "onRadioButton_123Pressed\0"
    "onRadioButton_124Pressed\0"
    "onRadioButton_125Pressed\0"
    "onRadioButton_126Pressed\0"
    "onRadioButton_127Pressed\0"
    "onRadioButton_128Pressed\0"
    "onRadioButton_129Pressed\0"
    "onRadioButton_130Pressed\0"
    "onRadioButton_131Pressed\0"
    "onRadioButton_132Pressed\0"
    "onRadioButton_133Pressed\0"
    "onRadioButton_134Pressed\0"
    "onRadioButton_135Pressed\0"
    "onRadioButton_136Pressed\0"
    "onRadioButton_137Pressed\0"
    "onRadioButton_138Pressed\0"
    "onRadioButton_139Pressed\0"
    "onRadioButton_140Pressed\0"
    "onRadioButton_141Pressed\0"
    "onRadioButton_142Pressed\0"
    "onRadioButton_143Pressed\0"
    "onRadioButton_144Pressed\0"
    "onRadioButton_145Pressed\0"
    "onRadioButton_146Pressed\0"
    "onRadioButton_147Pressed\0"
    "onRadioButton_148Pressed\0"
    "onRadioButton_149Pressed\0"
    "onRadioButton_150Pressed\0"
    "onRadioButton_151Pressed\0"
    "onRadioButton_152Pressed\0"
    "onRadioButton_153Pressed\0"
    "onRadioButton_154Pressed\0"
    "onRadioButton_155Pressed\0"
    "onRadioButton_156Pressed\0"
    "onRadioButton_157Pressed\0"
    "onRadioButton_158Pressed\0"
    "onRadioButton_159Pressed\0"
    "onRadioButton_160Pressed\0"
    "onRadioButton_161Pressed\0"
    "onRadioButton_162Pressed\0"
    "onRadioButton_163Pressed\0"
    "onRadioButton_164Pressed\0"
    "onRadioButton_165Pressed\0"
    "onRadioButton_166Pressed\0"
    "onRadioButton_167Pressed\0"
    "onRadioButton_168Pressed\0"
    "onRadioButton_169Pressed\0"
    "onRadioButton_170Pressed\0"
    "onRadioButton_171Pressed\0"
    "onRadioButton_172Pressed\0"
    "onRadioButton_173Pressed\0"
    "onRadioButton_174Pressed\0"
    "onRadioButton_175Pressed\0"
    "onRadioButton_176Pressed\0"
    "onRadioButton_177Pressed\0"
    "onRadioButton_178Pressed\0"
    "onRadioButton_179Pressed\0"
    "onRadioButton_180Pressed\0"
    "onRadioButton_198Pressed\0"
    "onRadioButton_199Pressed\0"
    "onRadioButton_232Pressed\0"
    "onRadioButton_233Pressed\0"
    "onRadioButton_238Pressed\0"
    "onRadioButton_239Pressed\0"
    "onRadioButton_240Pressed\0"
    "onRadioButton_241Pressed\0"
    "onRadioButton_242Pressed\0"
    "onRadioButton_243Pressed\0"
    "onRadioButton_244Pressed\0"
    "onRadioButton_245Pressed\0"
    "onRadioButton_246Pressed\0"
    "onRadioButton_247Pressed\0"
    "onRadioButton_248Pressed\0"
    "onRadioButton_249Pressed\0"
    "onRadioButton_250Pressed\0"
    "onRadioButton_251Pressed\0"
    "onRadioButton_252Pressed\0"
    "onRadioButton_253Pressed\0"
    "onRadioButton_254Pressed\0"
    "onRadioButton_255Pressed\0"
    "onRadioButton_256Pressed\0"
    "onRadioButton_257Pressed\0"
    "onRadioButton_258Pressed\0"
    "onRadioButton_259Pressed\0"
    "onRadioButton_260Pressed\0"
    "onRadioButton_261Pressed\0"
    "onRadioButton_262Pressed\0"
    "onRadioButton_263Pressed\0"
    "onRadioButton_264Pressed\0"
    "onRadioButton_265Pressed\0"
    "onRadioButton_266Pressed\0"
    "onRadioButton_267Pressed\0"
    "onRadioButton_268Pressed\0"
    "onRadioButton_269Pressed\0"
    "onRadioButton_270Pressed\0"
    "onRadioButton_271Pressed\0"
    "onRadioButton_272Pressed\0"
    "onRadioButton_273Pressed\0"
    "onRadioButton_194Pressed\0"
    "onRadioButton_195Pressed\0"
    "onRadioButton_196Pressed\0"
    "onRadioButton_197Pressed\0"
    "onRadioButton_200Pressed\0"
    "onRadioButton_201Pressed\0"
    "onRadioButton_202Pressed\0"
    "onRadioButton_203Pressed\0"
    "onRadioButton_204Pressed\0"
    "onRadioButton_205Pressed\0"
    "onRadioButton_206Pressed\0"
    "onRadioButton_207Pressed\0"
    "onRadioButton_208Pressed\0"
    "onRadioButton_209Pressed\0"
    "onRadioButton_210Pressed\0"
    "onRadioButton_211Pressed\0"
    "onRadioButton_212Pressed\0"
    "onRadioButton_213Pressed\0"
    "onRadioButton_214Pressed\0"
    "onRadioButton_215Pressed\0"
    "onRadioButton_216Pressed\0"
    "onRadioButton_217Pressed\0"
    "onRadioButton_218Pressed\0"
    "onRadioButton_219Pressed\0"
    "onRadioButton_220Pressed\0"
    "onRadioButton_221Pressed\0"
    "onRadioButton_222Pressed\0"
    "onRadioButton_223Pressed\0"
    "onRadioButton_224Pressed\0"
    "onRadioButton_225Pressed\0"
    "onRadioButton_226Pressed\0"
    "onRadioButton_227Pressed\0"
    "onRadioButton_228Pressed\0"
    "onRadioButton_229Pressed\0"
    "onRadioButton_230Pressed\0"
    "onRadioButton_231Pressed\0"
    "onRadioButton_234Pressed\0"
    "onRadioButton_235Pressed\0"
    "onRadioButton_236Pressed\0"
    "onRadioButton_237Pressed\0updateGraph"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
     288,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1, 1454,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,   18, 1457,    2, 0x08 /* Private */,
       7,    1, 1494,    2, 0x08 /* Private */,
       8,    1, 1497,    2, 0x08 /* Private */,
       9,    1, 1500,    2, 0x08 /* Private */,
      10,    1, 1503,    2, 0x08 /* Private */,
      11,    1, 1506,    2, 0x08 /* Private */,
      12,    1, 1509,    2, 0x08 /* Private */,
      13,    1, 1512,    2, 0x08 /* Private */,
      14,    1, 1515,    2, 0x08 /* Private */,
      15,    1, 1518,    2, 0x08 /* Private */,
      16,    1, 1521,    2, 0x08 /* Private */,
      17,    1, 1524,    2, 0x08 /* Private */,
      18,    1, 1527,    2, 0x08 /* Private */,
      19,    0, 1530,    2, 0x08 /* Private */,
      20,    0, 1531,    2, 0x08 /* Private */,
      21,    0, 1532,    2, 0x08 /* Private */,
      22,    0, 1533,    2, 0x08 /* Private */,
      23,    0, 1534,    2, 0x08 /* Private */,
      24,    0, 1535,    2, 0x08 /* Private */,
      25,    0, 1536,    2, 0x08 /* Private */,
      26,    0, 1537,    2, 0x08 /* Private */,
      27,    0, 1538,    2, 0x08 /* Private */,
      28,    0, 1539,    2, 0x08 /* Private */,
      29,    0, 1540,    2, 0x08 /* Private */,
      30,    0, 1541,    2, 0x08 /* Private */,
      31,    0, 1542,    2, 0x08 /* Private */,
      32,    0, 1543,    2, 0x08 /* Private */,
      33,    0, 1544,    2, 0x08 /* Private */,
      34,    0, 1545,    2, 0x08 /* Private */,
      35,    0, 1546,    2, 0x08 /* Private */,
      36,    0, 1547,    2, 0x08 /* Private */,
      37,    0, 1548,    2, 0x08 /* Private */,
      38,    0, 1549,    2, 0x08 /* Private */,
      39,    0, 1550,    2, 0x08 /* Private */,
      40,    0, 1551,    2, 0x08 /* Private */,
      41,    0, 1552,    2, 0x08 /* Private */,
      42,    0, 1553,    2, 0x08 /* Private */,
      43,    0, 1554,    2, 0x08 /* Private */,
      44,    0, 1555,    2, 0x08 /* Private */,
      45,    0, 1556,    2, 0x08 /* Private */,
      46,    1, 1557,    2, 0x08 /* Private */,
      47,    0, 1560,    2, 0x08 /* Private */,
      48,    0, 1561,    2, 0x08 /* Private */,
      49,    0, 1562,    2, 0x08 /* Private */,
      50,    0, 1563,    2, 0x08 /* Private */,
      51,    0, 1564,    2, 0x08 /* Private */,
      52,    0, 1565,    2, 0x08 /* Private */,
      53,    0, 1566,    2, 0x08 /* Private */,
      54,    0, 1567,    2, 0x08 /* Private */,
      55,    0, 1568,    2, 0x08 /* Private */,
      56,    1, 1569,    2, 0x08 /* Private */,
      57,    0, 1572,    2, 0x08 /* Private */,
      58,    1, 1573,    2, 0x08 /* Private */,
      59,    1, 1576,    2, 0x08 /* Private */,
      60,    1, 1579,    2, 0x08 /* Private */,
      61,    1, 1582,    2, 0x08 /* Private */,
      62,    1, 1585,    2, 0x08 /* Private */,
      63,    1, 1588,    2, 0x08 /* Private */,
      64,    0, 1591,    2, 0x08 /* Private */,
      65,    0, 1592,    2, 0x08 /* Private */,
      66,    0, 1593,    2, 0x08 /* Private */,
      67,    0, 1594,    2, 0x08 /* Private */,
      68,    0, 1595,    2, 0x08 /* Private */,
      69,    2, 1596,    2, 0x08 /* Private */,
      70,    1, 1601,    2, 0x08 /* Private */,
      71,    0, 1604,    2, 0x08 /* Private */,
      72,    1, 1605,    2, 0x08 /* Private */,
      73,    0, 1608,    2, 0x08 /* Private */,
      74,    0, 1609,    2, 0x08 /* Private */,
      75,    0, 1610,    2, 0x08 /* Private */,
      76,    0, 1611,    2, 0x08 /* Private */,
      77,    0, 1612,    2, 0x08 /* Private */,
      78,    0, 1613,    2, 0x08 /* Private */,
      79,    0, 1614,    2, 0x08 /* Private */,
      80,    0, 1615,    2, 0x08 /* Private */,
      81,    0, 1616,    2, 0x08 /* Private */,
      82,    0, 1617,    2, 0x08 /* Private */,
      83,    0, 1618,    2, 0x08 /* Private */,
      84,    0, 1619,    2, 0x08 /* Private */,
      85,    0, 1620,    2, 0x08 /* Private */,
      86,    0, 1621,    2, 0x08 /* Private */,
      87,    0, 1622,    2, 0x08 /* Private */,
      88,    0, 1623,    2, 0x08 /* Private */,
      89,    0, 1624,    2, 0x08 /* Private */,
      90,    0, 1625,    2, 0x08 /* Private */,
      91,    0, 1626,    2, 0x08 /* Private */,
      92,    0, 1627,    2, 0x08 /* Private */,
      93,    0, 1628,    2, 0x08 /* Private */,
      94,    0, 1629,    2, 0x08 /* Private */,
      95,    0, 1630,    2, 0x08 /* Private */,
      96,    0, 1631,    2, 0x08 /* Private */,
      97,    0, 1632,    2, 0x08 /* Private */,
      98,    0, 1633,    2, 0x08 /* Private */,
      99,    0, 1634,    2, 0x08 /* Private */,
     100,    0, 1635,    2, 0x08 /* Private */,
     101,    0, 1636,    2, 0x08 /* Private */,
     102,    0, 1637,    2, 0x08 /* Private */,
     103,    0, 1638,    2, 0x08 /* Private */,
     104,    0, 1639,    2, 0x08 /* Private */,
     105,    0, 1640,    2, 0x08 /* Private */,
     106,    0, 1641,    2, 0x08 /* Private */,
     107,    0, 1642,    2, 0x08 /* Private */,
     108,    0, 1643,    2, 0x08 /* Private */,
     109,    0, 1644,    2, 0x08 /* Private */,
     110,    0, 1645,    2, 0x08 /* Private */,
     111,    0, 1646,    2, 0x08 /* Private */,
     112,    0, 1647,    2, 0x08 /* Private */,
     113,    0, 1648,    2, 0x08 /* Private */,
     114,    0, 1649,    2, 0x08 /* Private */,
     115,    0, 1650,    2, 0x08 /* Private */,
     116,    0, 1651,    2, 0x08 /* Private */,
     117,    0, 1652,    2, 0x08 /* Private */,
     118,    0, 1653,    2, 0x08 /* Private */,
     119,    0, 1654,    2, 0x08 /* Private */,
     120,    0, 1655,    2, 0x08 /* Private */,
     121,    0, 1656,    2, 0x08 /* Private */,
     122,    0, 1657,    2, 0x08 /* Private */,
     123,    0, 1658,    2, 0x08 /* Private */,
     124,    0, 1659,    2, 0x08 /* Private */,
     125,    0, 1660,    2, 0x08 /* Private */,
     126,    0, 1661,    2, 0x08 /* Private */,
     127,    0, 1662,    2, 0x08 /* Private */,
     128,    0, 1663,    2, 0x08 /* Private */,
     129,    0, 1664,    2, 0x08 /* Private */,
     130,    0, 1665,    2, 0x08 /* Private */,
     131,    0, 1666,    2, 0x08 /* Private */,
     132,    0, 1667,    2, 0x08 /* Private */,
     133,    0, 1668,    2, 0x08 /* Private */,
     134,    0, 1669,    2, 0x08 /* Private */,
     135,    0, 1670,    2, 0x08 /* Private */,
     136,    0, 1671,    2, 0x08 /* Private */,
     137,    0, 1672,    2, 0x08 /* Private */,
     138,    0, 1673,    2, 0x08 /* Private */,
     139,    0, 1674,    2, 0x08 /* Private */,
     140,    0, 1675,    2, 0x08 /* Private */,
     141,    0, 1676,    2, 0x08 /* Private */,
     142,    0, 1677,    2, 0x08 /* Private */,
     143,    0, 1678,    2, 0x08 /* Private */,
     144,    0, 1679,    2, 0x08 /* Private */,
     145,    0, 1680,    2, 0x08 /* Private */,
     146,    0, 1681,    2, 0x08 /* Private */,
     147,    0, 1682,    2, 0x08 /* Private */,
     148,    0, 1683,    2, 0x08 /* Private */,
     149,    0, 1684,    2, 0x08 /* Private */,
     150,    0, 1685,    2, 0x08 /* Private */,
     151,    0, 1686,    2, 0x08 /* Private */,
     152,    0, 1687,    2, 0x08 /* Private */,
     153,    0, 1688,    2, 0x08 /* Private */,
     154,    0, 1689,    2, 0x08 /* Private */,
     155,    0, 1690,    2, 0x08 /* Private */,
     156,    0, 1691,    2, 0x08 /* Private */,
     157,    0, 1692,    2, 0x08 /* Private */,
     158,    0, 1693,    2, 0x08 /* Private */,
     159,    0, 1694,    2, 0x08 /* Private */,
     160,    0, 1695,    2, 0x08 /* Private */,
     161,    0, 1696,    2, 0x08 /* Private */,
     162,    0, 1697,    2, 0x08 /* Private */,
     163,    0, 1698,    2, 0x08 /* Private */,
     164,    0, 1699,    2, 0x08 /* Private */,
     165,    0, 1700,    2, 0x08 /* Private */,
     166,    0, 1701,    2, 0x08 /* Private */,
     167,    0, 1702,    2, 0x08 /* Private */,
     168,    0, 1703,    2, 0x08 /* Private */,
     169,    0, 1704,    2, 0x08 /* Private */,
     170,    0, 1705,    2, 0x08 /* Private */,
     171,    0, 1706,    2, 0x08 /* Private */,
     172,    0, 1707,    2, 0x08 /* Private */,
     173,    0, 1708,    2, 0x08 /* Private */,
     174,    0, 1709,    2, 0x08 /* Private */,
     175,    0, 1710,    2, 0x08 /* Private */,
     176,    0, 1711,    2, 0x08 /* Private */,
     177,    0, 1712,    2, 0x08 /* Private */,
     178,    0, 1713,    2, 0x08 /* Private */,
     179,    0, 1714,    2, 0x08 /* Private */,
     180,    0, 1715,    2, 0x08 /* Private */,
     181,    0, 1716,    2, 0x08 /* Private */,
     182,    0, 1717,    2, 0x08 /* Private */,
     183,    0, 1718,    2, 0x08 /* Private */,
     184,    0, 1719,    2, 0x08 /* Private */,
     185,    0, 1720,    2, 0x08 /* Private */,
     186,    0, 1721,    2, 0x08 /* Private */,
     187,    0, 1722,    2, 0x08 /* Private */,
     188,    0, 1723,    2, 0x08 /* Private */,
     189,    0, 1724,    2, 0x08 /* Private */,
     190,    0, 1725,    2, 0x08 /* Private */,
     191,    0, 1726,    2, 0x08 /* Private */,
     192,    0, 1727,    2, 0x08 /* Private */,
     193,    0, 1728,    2, 0x08 /* Private */,
     194,    0, 1729,    2, 0x08 /* Private */,
     195,    0, 1730,    2, 0x08 /* Private */,
     196,    0, 1731,    2, 0x08 /* Private */,
     197,    0, 1732,    2, 0x08 /* Private */,
     198,    0, 1733,    2, 0x08 /* Private */,
     199,    0, 1734,    2, 0x08 /* Private */,
     200,    0, 1735,    2, 0x08 /* Private */,
     201,    0, 1736,    2, 0x08 /* Private */,
     202,    0, 1737,    2, 0x08 /* Private */,
     203,    0, 1738,    2, 0x08 /* Private */,
     204,    0, 1739,    2, 0x08 /* Private */,
     205,    0, 1740,    2, 0x08 /* Private */,
     206,    0, 1741,    2, 0x08 /* Private */,
     207,    0, 1742,    2, 0x08 /* Private */,
     208,    0, 1743,    2, 0x08 /* Private */,
     209,    0, 1744,    2, 0x08 /* Private */,
     210,    0, 1745,    2, 0x08 /* Private */,
     211,    0, 1746,    2, 0x08 /* Private */,
     212,    0, 1747,    2, 0x08 /* Private */,
     213,    0, 1748,    2, 0x08 /* Private */,
     214,    0, 1749,    2, 0x08 /* Private */,
     215,    0, 1750,    2, 0x08 /* Private */,
     216,    0, 1751,    2, 0x08 /* Private */,
     217,    0, 1752,    2, 0x08 /* Private */,
     218,    0, 1753,    2, 0x08 /* Private */,
     219,    0, 1754,    2, 0x08 /* Private */,
     220,    0, 1755,    2, 0x08 /* Private */,
     221,    0, 1756,    2, 0x08 /* Private */,
     222,    0, 1757,    2, 0x08 /* Private */,
     223,    0, 1758,    2, 0x08 /* Private */,
     224,    0, 1759,    2, 0x08 /* Private */,
     225,    0, 1760,    2, 0x08 /* Private */,
     226,    0, 1761,    2, 0x08 /* Private */,
     227,    0, 1762,    2, 0x08 /* Private */,
     228,    0, 1763,    2, 0x08 /* Private */,
     229,    0, 1764,    2, 0x08 /* Private */,
     230,    0, 1765,    2, 0x08 /* Private */,
     231,    0, 1766,    2, 0x08 /* Private */,
     232,    0, 1767,    2, 0x08 /* Private */,
     233,    0, 1768,    2, 0x08 /* Private */,
     234,    0, 1769,    2, 0x08 /* Private */,
     235,    0, 1770,    2, 0x08 /* Private */,
     236,    0, 1771,    2, 0x08 /* Private */,
     237,    0, 1772,    2, 0x08 /* Private */,
     238,    0, 1773,    2, 0x08 /* Private */,
     239,    0, 1774,    2, 0x08 /* Private */,
     240,    0, 1775,    2, 0x08 /* Private */,
     241,    0, 1776,    2, 0x08 /* Private */,
     242,    0, 1777,    2, 0x08 /* Private */,
     243,    0, 1778,    2, 0x08 /* Private */,
     244,    0, 1779,    2, 0x08 /* Private */,
     245,    0, 1780,    2, 0x08 /* Private */,
     246,    0, 1781,    2, 0x08 /* Private */,
     247,    0, 1782,    2, 0x08 /* Private */,
     248,    0, 1783,    2, 0x08 /* Private */,
     249,    0, 1784,    2, 0x08 /* Private */,
     250,    0, 1785,    2, 0x08 /* Private */,
     251,    0, 1786,    2, 0x08 /* Private */,
     252,    0, 1787,    2, 0x08 /* Private */,
     253,    0, 1788,    2, 0x08 /* Private */,
     254,    0, 1789,    2, 0x08 /* Private */,
     255,    0, 1790,    2, 0x08 /* Private */,
     256,    0, 1791,    2, 0x08 /* Private */,
     257,    0, 1792,    2, 0x08 /* Private */,
     258,    0, 1793,    2, 0x08 /* Private */,
     259,    0, 1794,    2, 0x08 /* Private */,
     260,    0, 1795,    2, 0x08 /* Private */,
     261,    0, 1796,    2, 0x08 /* Private */,
     262,    0, 1797,    2, 0x08 /* Private */,
     263,    0, 1798,    2, 0x08 /* Private */,
     264,    0, 1799,    2, 0x08 /* Private */,
     265,    0, 1800,    2, 0x08 /* Private */,
     266,    0, 1801,    2, 0x08 /* Private */,
     267,    0, 1802,    2, 0x08 /* Private */,
     268,    0, 1803,    2, 0x08 /* Private */,
     269,    0, 1804,    2, 0x08 /* Private */,
     270,    0, 1805,    2, 0x08 /* Private */,
     271,    0, 1806,    2, 0x08 /* Private */,
     272,    0, 1807,    2, 0x08 /* Private */,
     273,    0, 1808,    2, 0x08 /* Private */,
     274,    0, 1809,    2, 0x08 /* Private */,
     275,    0, 1810,    2, 0x08 /* Private */,
     276,    0, 1811,    2, 0x08 /* Private */,
     277,    0, 1812,    2, 0x08 /* Private */,
     278,    0, 1813,    2, 0x08 /* Private */,
     279,    0, 1814,    2, 0x08 /* Private */,
     280,    0, 1815,    2, 0x08 /* Private */,
     281,    0, 1816,    2, 0x08 /* Private */,
     282,    0, 1817,    2, 0x08 /* Private */,
     283,    0, 1818,    2, 0x08 /* Private */,
     284,    0, 1819,    2, 0x08 /* Private */,
     285,    0, 1820,    2, 0x08 /* Private */,
     286,    0, 1821,    2, 0x08 /* Private */,
     287,    0, 1822,    2, 0x08 /* Private */,
     288,    0, 1823,    2, 0x08 /* Private */,
     289,    0, 1824,    2, 0x08 /* Private */,
     290,    0, 1825,    2, 0x08 /* Private */,
     291,    0, 1826,    2, 0x08 /* Private */,
     292,    0, 1827,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString, 0x80000000 | 5, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, 0x80000000 | 6, 0x80000000 | 6, 0x80000000 | 6, 0x80000000 | 6, 0x80000000 | 6, 0x80000000 | 6, 0x80000000 | 6,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Bool,    2,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
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
        case 1: _t->createLoopFiles((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const WINBOOL(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6])),(*reinterpret_cast< const QString(*)>(_a[7])),(*reinterpret_cast< const QString(*)>(_a[8])),(*reinterpret_cast< const QString(*)>(_a[9])),(*reinterpret_cast< const QString(*)>(_a[10])),(*reinterpret_cast< const QString(*)>(_a[11])),(*reinterpret_cast< QFile(*)>(_a[12])),(*reinterpret_cast< QFile(*)>(_a[13])),(*reinterpret_cast< QFile(*)>(_a[14])),(*reinterpret_cast< QFile(*)>(_a[15])),(*reinterpret_cast< QFile(*)>(_a[16])),(*reinterpret_cast< QFile(*)>(_a[17])),(*reinterpret_cast< QFile(*)>(_a[18]))); break;
        case 2: _t->onRtuPortActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onRtuPortActive_2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onRtuPortActive_3((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->onRtuPortActive_4((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->onRtuPortActive_5((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->onRtuPortActive_6((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->changeSerialPort((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->changeSerialPort_2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->changeSerialPort_3((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->changeSerialPort_4((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->changeSerialPort_5((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->changeSerialPort_6((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->calibration_L1P1(); break;
        case 15: _t->calibration_L1P2(); break;
        case 16: _t->calibration_L1P3(); break;
        case 17: _t->calibration_L2P1(); break;
        case 18: _t->calibration_L2P2(); break;
        case 19: _t->calibration_L2P3(); break;
        case 20: _t->calibration_L3P1(); break;
        case 21: _t->calibration_L3P2(); break;
        case 22: _t->calibration_L3P3(); break;
        case 23: _t->calibration_L4P1(); break;
        case 24: _t->calibration_L4P2(); break;
        case 25: _t->calibration_L4P3(); break;
        case 26: _t->calibration_L5P1(); break;
        case 27: _t->calibration_L5P2(); break;
        case 28: _t->calibration_L5P3(); break;
        case 29: _t->calibration_L6P1(); break;
        case 30: _t->calibration_L6P2(); break;
        case 31: _t->calibration_L6P3(); break;
        case 32: _t->initializeToolbarIcons(); break;
        case 33: _t->initializeFrequencyGauge(); break;
        case 34: _t->initializeTemperatureGauge(); break;
        case 35: _t->initializeDensityGauge(); break;
        case 36: _t->initializeRPGauge(); break;
        case 37: _t->updateFrequencyGauge(); break;
        case 38: _t->updateTemperatureGauge(); break;
        case 39: _t->updateDensityGauge(); break;
        case 40: _t->updateRPGauge(); break;
        case 41: _t->onLoopTabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 42: _t->clearMonitors(); break;
        case 43: _t->updateRequestPreview(); break;
        case 44: _t->updateRegisterView(); break;
        case 45: _t->enableHexView(); break;
        case 46: _t->sendModbusRequest(); break;
        case 47: _t->onSendButtonPress(); break;
        case 48: _t->pollForDataOnBus(); break;
        case 49: _t->openBatchProcessor(); break;
        case 50: _t->aboutQModBus(); break;
        case 51: _t->onCheckBoxChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 52: _t->resetStatus(); break;
        case 53: _t->setStatusError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 54: _t->onFloatButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 55: _t->onIntegerButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 56: _t->onCoilButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 57: _t->onReadButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 58: _t->onWriteButtonPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 59: _t->onEquationButtonPressed(); break;
        case 60: _t->loadCsvFile(); break;
        case 61: _t->loadCsvTemplate(); break;
        case 62: _t->onUploadEquation(); break;
        case 63: _t->onDownloadEquation(); break;
        case 64: _t->updateRegisters((*reinterpret_cast< const bool(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 65: _t->onDownloadButtonChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 66: _t->saveCsvFile(); break;
        case 67: _t->onEquationTableChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 68: _t->onUnlockFactoryDefaultBtnPressed(); break;
        case 69: _t->onLockFactoryDefaultBtnPressed(); break;
        case 70: _t->onUpdateFactoryDefaultPressed(); break;
        case 71: _t->onRadioButtonPressed(); break;
        case 72: _t->onRadioButton_2Pressed(); break;
        case 73: _t->onRadioButton_3Pressed(); break;
        case 74: _t->onRadioButton_4Pressed(); break;
        case 75: _t->onRadioButton_5Pressed(); break;
        case 76: _t->onRadioButton_6Pressed(); break;
        case 77: _t->onRadioButton_7Pressed(); break;
        case 78: _t->onRadioButton_8Pressed(); break;
        case 79: _t->onRadioButton_9Pressed(); break;
        case 80: _t->onRadioButton_10Pressed(); break;
        case 81: _t->onRadioButton_11Pressed(); break;
        case 82: _t->onRadioButton_12Pressed(); break;
        case 83: _t->onRadioButton_13Pressed(); break;
        case 84: _t->onRadioButton_22Pressed(); break;
        case 85: _t->onRadioButton_23Pressed(); break;
        case 86: _t->onRadioButton_24Pressed(); break;
        case 87: _t->onRadioButton_37Pressed(); break;
        case 88: _t->onRadioButton_38Pressed(); break;
        case 89: _t->onRadioButton_39Pressed(); break;
        case 90: _t->onRadioButton_40Pressed(); break;
        case 91: _t->onRadioButton_41Pressed(); break;
        case 92: _t->onRadioButton_42Pressed(); break;
        case 93: _t->onRadioButton_43Pressed(); break;
        case 94: _t->onRadioButton_44Pressed(); break;
        case 95: _t->onRadioButton_69Pressed(); break;
        case 96: _t->onRadioButton_70Pressed(); break;
        case 97: _t->onRadioButton_71Pressed(); break;
        case 98: _t->onRadioButton_72Pressed(); break;
        case 99: _t->onRadioButton_73Pressed(); break;
        case 100: _t->onRadioButton_74Pressed(); break;
        case 101: _t->onRadioButton_75Pressed(); break;
        case 102: _t->onRadioButton_76Pressed(); break;
        case 103: _t->onRadioButton_77Pressed(); break;
        case 104: _t->onRadioButton_78Pressed(); break;
        case 105: _t->onRadioButton_79Pressed(); break;
        case 106: _t->onRadioButton_80Pressed(); break;
        case 107: _t->onRadioButton_81Pressed(); break;
        case 108: _t->onRadioButton_82Pressed(); break;
        case 109: _t->onRadioButton_83Pressed(); break;
        case 110: _t->onRadioButton_84Pressed(); break;
        case 111: _t->onRadioButton_85Pressed(); break;
        case 112: _t->onRadioButton_86Pressed(); break;
        case 113: _t->onRadioButton_87Pressed(); break;
        case 114: _t->onRadioButton_88Pressed(); break;
        case 115: _t->onRadioButton_89Pressed(); break;
        case 116: _t->onRadioButton_90Pressed(); break;
        case 117: _t->onRadioButton_91Pressed(); break;
        case 118: _t->onRadioButton_92Pressed(); break;
        case 119: _t->onRadioButton_93Pressed(); break;
        case 120: _t->onRadioButton_94Pressed(); break;
        case 121: _t->onRadioButton_95Pressed(); break;
        case 122: _t->onRadioButton_96Pressed(); break;
        case 123: _t->onRadioButton_97Pressed(); break;
        case 124: _t->onRadioButton_98Pressed(); break;
        case 125: _t->onRadioButton_99Pressed(); break;
        case 126: _t->onRadioButton_100Pressed(); break;
        case 127: _t->onRadioButton_101Pressed(); break;
        case 128: _t->onRadioButton_102Pressed(); break;
        case 129: _t->onRadioButton_103Pressed(); break;
        case 130: _t->onRadioButton_104Pressed(); break;
        case 131: _t->onRadioButton_105Pressed(); break;
        case 132: _t->onRadioButton_106Pressed(); break;
        case 133: _t->onRadioButton_107Pressed(); break;
        case 134: _t->onRadioButton_108Pressed(); break;
        case 135: _t->onRadioButton_109Pressed(); break;
        case 136: _t->onRadioButton_110Pressed(); break;
        case 137: _t->onRadioButton_111Pressed(); break;
        case 138: _t->onRadioButton_112Pressed(); break;
        case 139: _t->onRadioButton_113Pressed(); break;
        case 140: _t->onRadioButton_114Pressed(); break;
        case 141: _t->onRadioButton_115Pressed(); break;
        case 142: _t->onRadioButton_116Pressed(); break;
        case 143: _t->onRadioButton_117Pressed(); break;
        case 144: _t->onRadioButton_118Pressed(); break;
        case 145: _t->onRadioButton_119Pressed(); break;
        case 146: _t->onRadioButton_120Pressed(); break;
        case 147: _t->onRadioButton_121Pressed(); break;
        case 148: _t->onRadioButton_122Pressed(); break;
        case 149: _t->onRadioButton_123Pressed(); break;
        case 150: _t->onRadioButton_124Pressed(); break;
        case 151: _t->onRadioButton_125Pressed(); break;
        case 152: _t->onRadioButton_126Pressed(); break;
        case 153: _t->onRadioButton_127Pressed(); break;
        case 154: _t->onRadioButton_128Pressed(); break;
        case 155: _t->onRadioButton_129Pressed(); break;
        case 156: _t->onRadioButton_130Pressed(); break;
        case 157: _t->onRadioButton_131Pressed(); break;
        case 158: _t->onRadioButton_132Pressed(); break;
        case 159: _t->onRadioButton_133Pressed(); break;
        case 160: _t->onRadioButton_134Pressed(); break;
        case 161: _t->onRadioButton_135Pressed(); break;
        case 162: _t->onRadioButton_136Pressed(); break;
        case 163: _t->onRadioButton_137Pressed(); break;
        case 164: _t->onRadioButton_138Pressed(); break;
        case 165: _t->onRadioButton_139Pressed(); break;
        case 166: _t->onRadioButton_140Pressed(); break;
        case 167: _t->onRadioButton_141Pressed(); break;
        case 168: _t->onRadioButton_142Pressed(); break;
        case 169: _t->onRadioButton_143Pressed(); break;
        case 170: _t->onRadioButton_144Pressed(); break;
        case 171: _t->onRadioButton_145Pressed(); break;
        case 172: _t->onRadioButton_146Pressed(); break;
        case 173: _t->onRadioButton_147Pressed(); break;
        case 174: _t->onRadioButton_148Pressed(); break;
        case 175: _t->onRadioButton_149Pressed(); break;
        case 176: _t->onRadioButton_150Pressed(); break;
        case 177: _t->onRadioButton_151Pressed(); break;
        case 178: _t->onRadioButton_152Pressed(); break;
        case 179: _t->onRadioButton_153Pressed(); break;
        case 180: _t->onRadioButton_154Pressed(); break;
        case 181: _t->onRadioButton_155Pressed(); break;
        case 182: _t->onRadioButton_156Pressed(); break;
        case 183: _t->onRadioButton_157Pressed(); break;
        case 184: _t->onRadioButton_158Pressed(); break;
        case 185: _t->onRadioButton_159Pressed(); break;
        case 186: _t->onRadioButton_160Pressed(); break;
        case 187: _t->onRadioButton_161Pressed(); break;
        case 188: _t->onRadioButton_162Pressed(); break;
        case 189: _t->onRadioButton_163Pressed(); break;
        case 190: _t->onRadioButton_164Pressed(); break;
        case 191: _t->onRadioButton_165Pressed(); break;
        case 192: _t->onRadioButton_166Pressed(); break;
        case 193: _t->onRadioButton_167Pressed(); break;
        case 194: _t->onRadioButton_168Pressed(); break;
        case 195: _t->onRadioButton_169Pressed(); break;
        case 196: _t->onRadioButton_170Pressed(); break;
        case 197: _t->onRadioButton_171Pressed(); break;
        case 198: _t->onRadioButton_172Pressed(); break;
        case 199: _t->onRadioButton_173Pressed(); break;
        case 200: _t->onRadioButton_174Pressed(); break;
        case 201: _t->onRadioButton_175Pressed(); break;
        case 202: _t->onRadioButton_176Pressed(); break;
        case 203: _t->onRadioButton_177Pressed(); break;
        case 204: _t->onRadioButton_178Pressed(); break;
        case 205: _t->onRadioButton_179Pressed(); break;
        case 206: _t->onRadioButton_180Pressed(); break;
        case 207: _t->onRadioButton_198Pressed(); break;
        case 208: _t->onRadioButton_199Pressed(); break;
        case 209: _t->onRadioButton_232Pressed(); break;
        case 210: _t->onRadioButton_233Pressed(); break;
        case 211: _t->onRadioButton_238Pressed(); break;
        case 212: _t->onRadioButton_239Pressed(); break;
        case 213: _t->onRadioButton_240Pressed(); break;
        case 214: _t->onRadioButton_241Pressed(); break;
        case 215: _t->onRadioButton_242Pressed(); break;
        case 216: _t->onRadioButton_243Pressed(); break;
        case 217: _t->onRadioButton_244Pressed(); break;
        case 218: _t->onRadioButton_245Pressed(); break;
        case 219: _t->onRadioButton_246Pressed(); break;
        case 220: _t->onRadioButton_247Pressed(); break;
        case 221: _t->onRadioButton_248Pressed(); break;
        case 222: _t->onRadioButton_249Pressed(); break;
        case 223: _t->onRadioButton_250Pressed(); break;
        case 224: _t->onRadioButton_251Pressed(); break;
        case 225: _t->onRadioButton_252Pressed(); break;
        case 226: _t->onRadioButton_253Pressed(); break;
        case 227: _t->onRadioButton_254Pressed(); break;
        case 228: _t->onRadioButton_255Pressed(); break;
        case 229: _t->onRadioButton_256Pressed(); break;
        case 230: _t->onRadioButton_257Pressed(); break;
        case 231: _t->onRadioButton_258Pressed(); break;
        case 232: _t->onRadioButton_259Pressed(); break;
        case 233: _t->onRadioButton_260Pressed(); break;
        case 234: _t->onRadioButton_261Pressed(); break;
        case 235: _t->onRadioButton_262Pressed(); break;
        case 236: _t->onRadioButton_263Pressed(); break;
        case 237: _t->onRadioButton_264Pressed(); break;
        case 238: _t->onRadioButton_265Pressed(); break;
        case 239: _t->onRadioButton_266Pressed(); break;
        case 240: _t->onRadioButton_267Pressed(); break;
        case 241: _t->onRadioButton_268Pressed(); break;
        case 242: _t->onRadioButton_269Pressed(); break;
        case 243: _t->onRadioButton_270Pressed(); break;
        case 244: _t->onRadioButton_271Pressed(); break;
        case 245: _t->onRadioButton_272Pressed(); break;
        case 246: _t->onRadioButton_273Pressed(); break;
        case 247: _t->onRadioButton_194Pressed(); break;
        case 248: _t->onRadioButton_195Pressed(); break;
        case 249: _t->onRadioButton_196Pressed(); break;
        case 250: _t->onRadioButton_197Pressed(); break;
        case 251: _t->onRadioButton_200Pressed(); break;
        case 252: _t->onRadioButton_201Pressed(); break;
        case 253: _t->onRadioButton_202Pressed(); break;
        case 254: _t->onRadioButton_203Pressed(); break;
        case 255: _t->onRadioButton_204Pressed(); break;
        case 256: _t->onRadioButton_205Pressed(); break;
        case 257: _t->onRadioButton_206Pressed(); break;
        case 258: _t->onRadioButton_207Pressed(); break;
        case 259: _t->onRadioButton_208Pressed(); break;
        case 260: _t->onRadioButton_209Pressed(); break;
        case 261: _t->onRadioButton_210Pressed(); break;
        case 262: _t->onRadioButton_211Pressed(); break;
        case 263: _t->onRadioButton_212Pressed(); break;
        case 264: _t->onRadioButton_213Pressed(); break;
        case 265: _t->onRadioButton_214Pressed(); break;
        case 266: _t->onRadioButton_215Pressed(); break;
        case 267: _t->onRadioButton_216Pressed(); break;
        case 268: _t->onRadioButton_217Pressed(); break;
        case 269: _t->onRadioButton_218Pressed(); break;
        case 270: _t->onRadioButton_219Pressed(); break;
        case 271: _t->onRadioButton_220Pressed(); break;
        case 272: _t->onRadioButton_221Pressed(); break;
        case 273: _t->onRadioButton_222Pressed(); break;
        case 274: _t->onRadioButton_223Pressed(); break;
        case 275: _t->onRadioButton_224Pressed(); break;
        case 276: _t->onRadioButton_225Pressed(); break;
        case 277: _t->onRadioButton_226Pressed(); break;
        case 278: _t->onRadioButton_227Pressed(); break;
        case 279: _t->onRadioButton_228Pressed(); break;
        case 280: _t->onRadioButton_229Pressed(); break;
        case 281: _t->onRadioButton_230Pressed(); break;
        case 282: _t->onRadioButton_231Pressed(); break;
        case 283: _t->onRadioButton_234Pressed(); break;
        case 284: _t->onRadioButton_235Pressed(); break;
        case 285: _t->onRadioButton_236Pressed(); break;
        case 286: _t->onRadioButton_237Pressed(); break;
        case 287: _t->updateGraph(); break;
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
        if (_id < 288)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 288;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 288)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 288;
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
