//!
//! \file ultraschallsensor.h
//! \auther PAN Yunjie, YU Bohang, Team 03
//! \date 01-Dec-2018
//! \brief define a class for ultrasound wave sensor
//!
//! when users need the data of ultrasonicsensor,send the value of ultrasonicsensor in certain frequence
//! \return the value of distance between ultrasonicsensor and objects
//!
//! \param "m_nPinUltraschallSensor" the pin of ultrasonicsensor
//!
//! \pre
//! \post

#ifndef ULTRASCHALLSENSOR_H
#define ULTRASCHALLSENSOR_H

#include "abstractsensor.h"

class UltraschallSensor: public AbstractSensor
{
    Q_OBJECT
public:
    UltraschallSensor(int, int);//constructor
    ~UltraschallSensor();//destructor
    bool initSensor();//function to initialize the sensor
public slots:
    void slot_getSensorValue();//function to get the value of the sensor
private:
    int m_nPinUltraschallSensor;//number of Pin
};

#endif // ULTRASCHALLSENSOR_H
