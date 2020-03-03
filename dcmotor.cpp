#include "dcmotor.h"
#include <iostream>
using namespace std;
DcMotor::DcMotor(int pinMotorA, int pinMotorB, int pinMotorE)//Construktor
{
    m_nPinMotorA = pinMotorA;//set PinA of motor
    m_nPinMotorB = pinMotorB;//set PinB of motor
    m_nPinMotorE = pinMotorE;//set PinE of motor
    m_nPWM = 0;
    if(initPins())
    {
        digitalWrite(m_nPinMotorA,LOW);
        digitalWrite(m_nPinMotorB,LOW);
        softPwmWrite(m_nPinMotorE,m_nPWM);
    }
}
bool DcMotor::initPins()//initialize Pinmode of motor
{
    if(m_nPinMotorA<32&&m_nPinMotorB<32&&m_nPinMotorE<32&&//Pinnumber not bigger than 32 and not smaller than 0,and pinnumbers must different
       m_nPinMotorA>=0&&m_nPinMotorB>=0&&m_nPinMotorE>=0&&
       m_nPinMotorA!=m_nPinMotorB&&m_nPinMotorA!=m_nPinMotorE&&
       m_nPinMotorB!=m_nPinMotorE)
    {
        pinMode(m_nPinMotorA,OUTPUT);//set Pinmode OUTPUT
        pinMode(m_nPinMotorB,OUTPUT);//set Pinmode OUTPUT
        pinMode(m_nPinMotorE,OUTPUT);//set Pinmode OUTPUT
        softPwmCreate(m_nPinMotorE,0,100);//set range of PWM
        return true;// Initialization succeeds
    }
    else
    {
        return false;//Initialization failed
    }
}
void DcMotor::forward()//Motor moves forward
{
    digitalWrite(m_nPinMotorA,HIGH);
    digitalWrite(m_nPinMotorB,LOW);
}
void DcMotor::backward()// Motor moves backward
{
    digitalWrite(m_nPinMotorA,LOW);
    digitalWrite(m_nPinMotorB,HIGH);
}
void DcMotor::stopMotor()// stop Motor
{
    digitalWrite(m_nPinMotorA,LOW);
    digitalWrite(m_nPinMotorB,LOW);
    m_nPWM = 0;
    softPwmWrite(m_nPinMotorE,m_nPWM);
}
void DcMotor::setPWM(int nPWM) //set PWM of motor
{
    if (nPWM>=0&&nPWM<=100)
       {
        m_nPWM = nPWM;
        softPwmWrite(m_nPinMotorE,m_nPWM);
       }
    else
       {
        qDebug()<<"PWM Error,PWM:"<<m_nPWM<<endl;
       }
}
int DcMotor::getPWM()// get PWM of motor
{
    return m_nPWM;
}
DcMotor::~DcMotor()//destructor
{
    digitalWrite(m_nPinMotorA,LOW);
    digitalWrite(m_nPinMotorB,LOW);
    m_nPWM = 0;
    softPwmWrite(m_nPinMotorE,m_nPWM);
}
