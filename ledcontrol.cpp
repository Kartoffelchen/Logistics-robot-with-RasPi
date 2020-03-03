#include "ledcontrol.h"

LEDControl::LEDControl(int nYellowPin,int nRedPin,int nGreenPin)
{
    m_nPinYellow = nYellowPin;
    m_nPinRed = nRedPin;
    m_nPinGreen = nGreenPin;
    m_nBlinkColor = 0;
    m_bIsBlinking = false;
    if(initLED())
    {
        digitalWrite(m_nPinYellow,LOW);
        digitalWrite(m_nPinRed,LOW);
        digitalWrite(m_nPinGreen,LOW);
    }
}
bool LEDControl::initLED()
{
    if(m_nPinYellow < 32 && m_nPinRed < 32 && m_nPinGreen <32 &&//Pinnumber not bigger than 32 and not smaller than 0,and pinnumbers must different
       m_nPinYellow >=0 && m_nPinRed >=0 && m_nPinGreen >=0 &&
       m_nPinYellow != m_nPinRed && m_nPinYellow != m_nPinGreen && m_nPinRed != m_nPinGreen)
    {
        pinMode(m_nPinYellow,OUTPUT);//set Pinmode OUTPUT
        pinMode(m_nPinRed,OUTPUT);//set Pinmode OUTPUT
        pinMode(m_nPinGreen,OUTPUT);//set Pinmode OUTPUT
        return true;
    }
    else
    {
        qDebug()<<"LED init Error!";
        return false;
    }
}
void LEDControl::turnOnLED(int nColor)
{
    digitalWrite(nColor,HIGH);
}
void LEDControl::turnOffLED(int nColor)
{
    digitalWrite(nColor,LOW);
}
void LEDControl::slot_turnOff()
{
    digitalWrite(m_nPinYellow,LOW);
    digitalWrite(m_nPinRed,LOW);
    digitalWrite(m_nPinGreen,LOW);
}
void LEDControl::slot_startBlink(int nColor)
{
    m_nBlinkColor = nColor;
    m_bIsBlinking = true;
    blinkLED();
}
void LEDControl::blinkLED()
{
    while(m_bIsBlinking)
    {
        turnOffLED(m_nBlinkColor);
        delayMicroseconds(400000);//0.4s
        turnOnLED(m_nBlinkColor);
        delayMicroseconds(400000);//0.4s
        QCoreApplication::processEvents();
    }
}
void LEDControl::slot_stopBlinking()
{
    m_bIsBlinking = false;
    digitalWrite(m_nPinYellow,LOW);
    digitalWrite(m_nPinRed,LOW);
    digitalWrite(m_nPinGreen,LOW);

    delayMicroseconds(100000);

    digitalWrite(m_nBlinkColor,HIGH);
}
LEDControl::~LEDControl()
{
    digitalWrite(m_nPinYellow,LOW);
    digitalWrite(m_nPinRed,LOW);
    digitalWrite(m_nPinGreen,LOW);
    emit sgn_finished();
}
