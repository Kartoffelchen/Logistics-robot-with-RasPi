//!
//! \file abstractsensor.h
//! \auther PAN Yunjie, YU Bohang, Team 03
//! \date 30-Dec-2018
//! \brief to define a class for all sensors
//!
//! this part creat a class, that can represent all sensor with different IDs
//!
//! \param "m_bIsRunning" represent the statement of sensor
//! \param "m_dSensorValue" represent the values of sensors
//! \param "m_nSensorID" represent the ID of sensor
//! \param "m_nAbtastFrequenz" represent the frequenz of Qtimer of one sensor
//! \param "m_Mutex_Value" represent the Mutex for sensor`s value
//! \param "m_pAbtastTimer" represent the QTimer of one sensor
//!
//! \pre
//! \post

#ifndef ABSTRACTSENSOR_H
#define ABSTRACTSENSOR_H

#include <QMutex>
#include <QTimer>
#include <QThread>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <QApplication>
#include "namesanddefines.h"
#include <QDebug>

class AbstractSensor: public QObject
{
    Q_OBJECT
public:
    AbstractSensor();//constructor
    virtual ~AbstractSensor();//destructor
    virtual void activateSensor();//function activate the sensor
    virtual void deactivateSensor();//function deactivate the sensor
    virtual void resetSensor();//function reset the sensor
    virtual double sendSensorValue();//send new value of this sensor
    virtual double getEncoderSteps();//get Encoders' TICs
protected:
    bool    m_bIsRunning;//true means running,false not running
    double  m_dSensorValue;//value of this sensor
    int     m_nSensorID;//IDnumber of this sensor
    int     m_nAbtastFrequenz;//frequenz of Qtimer of this sensor
    QMutex  m_Mutex_Value;// mutex to protect the value of sensors
    QTimer* m_pAbtastTimer;//Qtimer of this sensor
public slots:
    virtual void slot_getSensorValue();//get new value of this sensor
signals:
    void sgn_newSensorData(int,double);//call sendSensorValue()
    void sgn_finished();//stop thread
};

#endif // ABSTRACTSENSOR_H
