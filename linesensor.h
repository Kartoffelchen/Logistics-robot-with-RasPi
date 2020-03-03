//!
//! \file linesensor.h
//! \auther PAN Yunjie, YU Bohang, Team 03
//! \date 12-Nov-2018
//! \brief get the status of linearsensor
//!
//! use this part to receive the signal from sensor (in the line or not)
//!
//! \param "m_nPin" this pin receive signal
//!
//! \pre
//! \post

#ifndef LINESENSOR_H
#define LINESENSOR_H

#include "abstractsensor.h"

class LineSensor: public AbstractSensor
{
   Q_OBJECT
public:
    LineSensor(int, int);//constructor
    ~LineSensor();//destructor

public slots:
    void slot_getSensorValue();//function to get value of the linesensor
private:
    int  m_nLineSensorPin;//number of Pin
    bool initSensor();// to check if GPIO of linesensor is right
};


#endif // LINESENSOR_H
