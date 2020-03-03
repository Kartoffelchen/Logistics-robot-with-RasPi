#include "abstractsensor.h"

AbstractSensor::AbstractSensor()
{
    m_bIsRunning = false;//false, loop can not run
    m_pAbtastTimer = new QTimer();//distribute memory address for Pointer m_pAbtastTimer
    m_nAbtastFrequenz = DEFAULTABTASTFREQUENZ;//set value of interval
    m_pAbtastTimer->setInterval(m_nAbtastFrequenz);//set interval of Qtimer
    m_pAbtastTimer->setSingleShot(false);// Timer always running,until stopTimer

    connect(m_pAbtastTimer, SIGNAL(timeout()), this, SLOT(slot_getSensorValue()));
}

void AbstractSensor::activateSensor()
{
    m_bIsRunning = true;
    m_pAbtastTimer->start();
}
void AbstractSensor::deactivateSensor()//stop Timer,stop getting new value,stop loop
{    
    m_bIsRunning = false;
    m_pAbtastTimer->stop();
}
void AbstractSensor::resetSensor()//reset sensor
{
    m_Mutex_Value.lock();
    m_dSensorValue = 0;//m_dSensorValue called,other sensor cannot use this parameter
    m_Mutex_Value.unlock();
    emit sgn_newSensorData(m_nSensorID,sendSensorValue());
}
double AbstractSensor::getEncoderSteps()
{
    return 0.0;
}
void AbstractSensor::slot_getSensorValue()//get new value of this sensor
{
}
double AbstractSensor::sendSensorValue()//send new value of this sensor
{
    double value;
    m_Mutex_Value.lock();
    value = m_dSensorValue;//m_dSensorValue called,other sensor cann not use this parameter
    m_Mutex_Value.unlock();
    return value;
}
AbstractSensor::~AbstractSensor()
{
    resetSensor();
    delete m_pAbtastTimer;
    emit sgn_finished();
}
