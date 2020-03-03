//!
//! \file dcmotor.h
//! \auther PAN Yunjie, YU Bohang, Team 03
//! \date 12-Nov-2018
//! \brief to control the rotation of the motor
//!
//! this part is used to set the values of output (high or low of GPIO) and the value of the PWM
//!
//! \param "m_nPinMotorA","m_nPinMotorB" control the rotation direction of the motor
//! \param "m_nPinMotorE"                control the speed of the motor
//!
//! \pre
//! \post

#ifndef DCMOTOR_H
#define DCMOTOR_H

#include <softPwm.h>
#include <wiringPi.h>
#include "namesanddefines.h"
#include <QDebug>

class DcMotor // declaration von Class DcMotor
{
public:
    DcMotor(int,int,int);//construktor
    ~DcMotor();//destruktor
    bool initPins(); //set Pinmode
    void forward();// motor moves forward
    void backward();//motor moves backward
    void stopMotor();//stop Motor
    void setPWM(int);//set PWM of Motor
    int  getPWM();//get PWM of Motor
private:
    int m_nPinMotorA; //number of PinA
    int m_nPinMotorB; //number of PinB
    int m_nPinMotorE; //number of PinE
    int m_nPWM;// PWM of Motor
};

#endif // DCMOTOR_H
