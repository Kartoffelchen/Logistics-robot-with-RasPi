#include "linesensor.h"

LineSensor::LineSensor(int LinePin, int SensorID) : AbstractSensor()
{
    m_nLineSensorPin = LinePin;
    m_nSensorID = SensorID;
    if(initSensor())
    {
        m_nAbtastFrequenz = ABTASTFREQUENZLINESENSOR;
        m_pAbtastTimer->setInterval(m_nAbtastFrequenz);
    }
}
bool LineSensor::initSensor()
{
    if ((m_nLineSensorPin>=0) && (m_nLineSensorPin<32))
    {
        pinMode(m_nLineSensorPin, INPUT);
        return true;
    }
    else
    {
        return false;
    }
}
LineSensor::~LineSensor()
{}
void LineSensor::slot_getSensorValue()
{
    m_Mutex_Value.lock();
    m_dSensorValue = digitalRead(m_nLineSensorPin);
    m_Mutex_Value.unlock();
    emit sgn_newSensorData(m_nSensorID,sendSensorValue());
}

