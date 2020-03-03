//!
//! \file encoder.h
//! \auther PAN Yunjie, YU Bohang, Team 03
//! \date 30-Dec-2018
//! \brief record distance of mobileplatform
//!
//! this part define variables, which can be used in "encoder.cpp", to calculate the distance of mobileplatform running
//!
//! \param "m_nPinEncoderA" is pin number of encoder A
//! \param "m_nPinEncoderB" is pin number of encoder B
//! \param "m_nNumSteps" record the total steps of enconder
//! \param "m_Mutex_Steps" represent the Mutex for m_nNUmSteps
//!
//! \pre
//! \post

#ifndef ENCODER_H
#define ENCODER_H

#include "abstractsensor.h"

class Encoder: public AbstractSensor
{
    Q_OBJECT
public:
    Encoder(int,int,int);//constructor
    ~Encoder();//destructor
    void resetSensor();//function reset the sensor
    bool initSensor();//function initialize the sensor Class
    double getEncoderSteps();//function to get steps of this encoder
public slots:
    void slot_getSensorValue();//function to get angle value of this encoder
signals:
    void sgn_newMotorSpeed(int,double);//function to send sensor value
private:
    int m_nPinEncoderA;//number of PinA
    int m_nPinEncoderB;//number of PinB
    long double m_nNumSteps;//number of steps
    QMutex  m_Mutex_Steps;//mutex to protect the steps value of the Encoder
};

#endif // ENCODER_H
