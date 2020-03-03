#include "encoder.h"

Encoder::Encoder(int nPinEncoderA, int nPinEncoderB, int nSensorID) : AbstractSensor()
{
    m_nPinEncoderA = nPinEncoderA;
    m_nPinEncoderB = nPinEncoderB;
    m_nSensorID    = nSensorID;
    m_nNumSteps = 0.0;
    m_dSensorValue = 0.0;
    if(initSensor())
    {
        m_pAbtastTimer->setSingleShot(true);//Timer works just once
        m_nAbtastFrequenz = ABTASTFREQUENZENCODER;
        m_pAbtastTimer->setInterval(m_nAbtastFrequenz);
    }    
}
bool Encoder::initSensor()
{
    if ((m_nPinEncoderA>=0) && (m_nPinEncoderA<32)&&(m_nPinEncoderB>=0) &&
            (m_nPinEncoderB<32)&&(m_nPinEncoderA!=m_nPinEncoderB)&&m_nSensorID<=6&&m_nSensorID>=1)//
    {
        pinMode(m_nPinEncoderA,INPUT);
        pinMode(m_nPinEncoderB,INPUT);
        return true;
    }
    else
    {
        return false;
    }
}

void Encoder::slot_getSensorValue()
{
    bool firstSignalA   = digitalRead(m_nPinEncoderA);//get first value and assign the value to "firstSignalA"
    bool secondSignalA  = digitalRead(m_nPinEncoderA);
    bool firstSignalB   = digitalRead(m_nPinEncoderB);//get first value and assign the value to "firstSignalB"
    bool secondSignalB  = digitalRead(m_nPinEncoderB);
    while(m_bIsRunning)
    {
        secondSignalA  = digitalRead(m_nPinEncoderA);//get second value and assign the value to "secondSignalA"
        secondSignalB  = digitalRead(m_nPinEncoderB);//get second value and assign the value to "secondSignalB"

        while(secondSignalA == firstSignalA && secondSignalB == firstSignalB && m_bIsRunning)//in this case means that the mobileplatform does not move
        {
            secondSignalA = digitalRead(m_nPinEncoderA);//set the value from second signal A
            secondSignalB = digitalRead(m_nPinEncoderB);//set the value from second signal B
            QCoreApplication::processEvents();
        }
        if (firstSignalA == secondSignalB)//in this case means that the mobileplatform moves forward (Übung5,Seite 10)
        {
            m_nNumSteps+=1;//step minus 1
        }
        else if (firstSignalB == secondSignalA)//in this case means that the mobileplatform moves backward (Übung5,Seite 10)
        {
            m_nNumSteps-=1;//step adds 1
        }
        firstSignalA = secondSignalA;//set the value of second signal as the value of first signal A in next loop
        firstSignalB = secondSignalB;//set the value of second signal as the value of first signal B in next loop
        m_Mutex_Value.lock();
        m_dSensorValue = (double)m_nNumSteps/TICS*GESAMTDREHUNGSGRAD;
        m_Mutex_Value.unlock();
        emit sgn_newSensorData(m_nSensorID,sendSensorValue());
        QCoreApplication::processEvents();
    }
}
double Encoder::getEncoderSteps()
{    
    double value;
    m_Mutex_Steps.lock();
    value = m_nNumSteps;//m_dSensorValue called,other sensor cann not use this parameter
    m_Mutex_Steps.unlock();
    if(m_nNumSteps<0)
        return -value;
    else
        return value;
}

void Encoder::resetSensor()//reset sensor
{
    m_Mutex_Value.lock();
    m_dSensorValue = 0.0;//m_dSensorValue called,other sensor cann not use this parameter
    m_Mutex_Value.unlock();
    m_Mutex_Steps.lock();
    m_nNumSteps = 0;//m_dSensorValue called,other sensor cann not use this parameter
    m_Mutex_Steps.unlock();
    emit sgn_newSensorData(m_nSensorID,sendSensorValue());
}
Encoder::~Encoder()
{
    resetSensor();
    delete m_pAbtastTimer;
    emit sgn_finished();
}
