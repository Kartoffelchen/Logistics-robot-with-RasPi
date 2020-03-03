//!
//! \file mobileplatform.h
//! \auther PAN Yunjie, YU Bohang, Team 03
//! \date 30-Nov-2018
//! \brief driving programm of the mobileplatform
//!
//! control the movement of the mobileplatform with the signals from the user interface and the linesensor
//!
//! \param "m_motorLeft" object left motor
//! \param "m_motorRight" object right motor
//!
//! \param "m_pLineSensorL" object left line sensor
//! \param "m_pLineSensorR" object right line sensor
//! \param "m_pColorSensor" object color sensor
//! \param "m_pEncoderL" object left encoder
//! \param "m_pEncoderR" object right encoder
//! \param "m_pUltraschallSensor" object ultraschall sensor
//!
//! \param "m_pLineL_Thread" object of thread for left linesensor
//! \param "m_pLineR_Thread" object of thread for right linesensor
//! \param "m_pColor_Thread" object of thread for color sensor
//! \param "m_pEncoderL_Thread" object of thread for left encoder
//! \param "m_pEncoderR_Thread" object of thread for right encoder
//! \param "m_pUltraschall_Thread" object of thread for ultraschall sensor
//!
//! \param "m_bIstLineFollow" used to coontrol the movementmode of platform (auto or manuell)
//! \param "m_bIstGoStraight" used to control Odometrie (go straightly or not)
//! \param "m_dSollSpeed" theoretical speed of two motors
//! \param "m_dPosition" real position of platform
//! \param "m_dSollDistance" theoretical distance,that platform should move
//! \param "m_dSollPWML" theoretical PWM of left motor, to move with a certain speed
//! \param "m_dSollPWMR" heoretical PWM of right motor, to move with a certain speed

//!
//! \pre
//! \post

#ifndef MOBILEPLATFORM_H
#define MOBILEPLATFORM_H

#include "dcmotor.h"
#include "linesensor.h"
#include "encoder.h"
#include "ultraschallsensor.h"
#include <QMainWindow>
#include <QCoreApplication>
#include <QDebug>

class MobilePlatform: public QObject
{
    Q_OBJECT
public:
    MobilePlatform();//Construktor
    ~MobilePlatform();//Destruktor
    void moveForward(double);//Platform moves forward
    void moveBackward(double);//Platform moves backward
    void turnLeft();//Platform turns left
    void turnRight();//Platform turns right
    void stopMotion();//stop Platform
    void stopWithUltraschallSensor();
    int  positioningRobot();//???
    void setKLLKLR(double,double);
    void setKUL(double);

public slots://slot functions
    void slot_moveForward();//Platform moves forward
    void slot_moveBackward();//Platform moves backward
    void slot_turnLeft();//Platform turns left
    void slot_turnRight();//Platform turns right
    void slot_stopMotion();//stop Platform

    void slot_stopAll();//when Mainwindow destroyed,call this function to stop all

    void slot_DSB_KPLValueChanged(double);
    void slot_DSB_KILValueChanged(double);
    void slot_DSB_KDLValueChanged(double);
    void slot_DSB_KPRValueChanged(double);
    void slot_DSB_KIRValueChanged(double);
    void slot_DSB_KDRValueChanged(double);
    void slot_DSB_setSpeed(double);
    void slot_DSB_setDistance(double);
    void slot_doSpeedPIDControl();

    void resetSensor();//reset sensor value
    void deactivateSensor();//deactivate sensor
    void activateSensor();//activate sensor

signals:
    void sgn_showSensorValue(int,double);//send all sensors values to GUI
    void sgn_showMoveInfo(int,double);//information of movement,like speed,position ...
                                      //'int' is ID of Info,'double' is Info`s value
    void sgn_showMotorSpeed(int,double);//send motors speeds to GUI
    void sgn_showMotorPosition(int,double);//send motors positions to GUI
    void sgn_finished();
    void sgn_realLagerDistance(double);
private:
    bool   m_bIsControling;//parameter,used to judge,if PIDControl should stop or not
    double m_dSollSpeed;//speed,with that platform moves ,cm per seconds
    double m_dSollSpeedGUI;//sollspeed from GUI
    double m_dSollDistance;//distance in a movement,that platform should go through,cm

    double m_dIstPositionL;//position of left whell in a movement,cm
    double m_dIstPositionR;//position of right wheel in a movement,cm
    double m_dIstPosition;//position in a movement,used to judge,if platform should stop or not,cm

    double m_dKLL;
    double m_dKLR;

    double m_dKUL;

    double m_dKPL;
    double m_dKIL;
    double m_dKDL;

    double m_dKPR;
    double m_dKIR;
    double m_dKDR;

    DcMotor* m_pMotorL;//object of left motor
    DcMotor* m_pMotorR;//object of right motor
    AbstractSensor* m_pEncoderL;//object of left encoder
    QThread* m_pEncoderL_Thread;//object of thread for left encoder
    AbstractSensor* m_pEncoderR;//object of right encoder
    QThread* m_pEncoderR_Thread;//object of thread for right encoder
    AbstractSensor* m_pUltraschallSensor;//object of ultraschall sensor
    QThread* m_pUltraschall_Thread;//object of thread for ultraschall sensor

    void initSensorThread(AbstractSensor*, QThread*);//function to initialize threads for sensors
};

#endif // MOBILEPLATFORM_H
