#include "ultraschallsensor.h"

UltraschallSensor::UltraschallSensor(int nPinUltraschall, int nSensorID) : AbstractSensor()
{
    m_nPinUltraschallSensor = nPinUltraschall;
    m_nSensorID = nSensorID;
    if(initSensor())
    {
        m_nAbtastFrequenz = ABTASTFREQUENZULTRASCHALLSENSOR;
        m_pAbtastTimer->setInterval(m_nAbtastFrequenz);
    }

}

bool UltraschallSensor::initSensor(){
    if ((m_nPinUltraschallSensor>=0) && (m_nPinUltraschallSensor<32)&&m_nSensorID<=6&&m_nSensorID>=1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void UltraschallSensor::slot_getSensorValue()
{
    int nFirstTime = 0;
    int nSecondTime = 0;
    int nDiff = 0;
    m_bIsRunning = true;
    digitalWrite(m_nPinUltraschallSensor,LOW);//set status of GPIO LOW
    pinMode(m_nPinUltraschallSensor,OUTPUT);// set GPIO output
    digitalWrite(m_nPinUltraschallSensor,HIGH);//set status of GPIO High
    delayMicroseconds(10);//10us TTL to signal pin (initate)
    digitalWrite(m_nPinUltraschallSensor,LOW);//set status of GPIO LOW
    pinMode(m_nPinUltraschallSensor,INPUT);//set GPIO input (echo back)
    digitalWrite(m_nPinUltraschallSensor,HIGH);//set status of GPIO High
    // get the Time of the echoback-process
    while(!digitalRead(m_nPinUltraschallSensor)&&m_bIsRunning)
    {}
    nFirstTime = micros();//time of rising edge
    while(digitalRead(m_nPinUltraschallSensor)&&m_bIsRunning)
    {}
    nSecondTime = micros();//time of falling edge
    nDiff = nSecondTime - nFirstTime;
    if (nDiff > 38000)//38 ms if no obstacle
        nDiff = 0;
    m_Mutex_Value.lock();
    m_dSensorValue = ABSTANDZWISHENSENSORBODEN+(double) (nDiff / ECHOBACKPARAMETER);
    m_Mutex_Value.unlock();
    emit sgn_newSensorData(m_nSensorID,sendSensorValue());
}

UltraschallSensor::~UltraschallSensor()
{
}
