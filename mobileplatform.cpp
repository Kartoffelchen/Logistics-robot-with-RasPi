#include "mobileplatform.h"
#include "namesanddefines.h"

MobilePlatform::MobilePlatform()
{
    m_bIsControling = false;
    m_dSollSpeed = 0;//cm per second
    m_dSollSpeedGUI = 0;//cm per second
    m_dSollDistance = 0;//cm
    m_dIstPosition = 0;//cm
    m_dIstPositionL = 0;//cm
    m_dIstPositionR = 0;//cm
    m_dKLL = 0.0;
    m_dKLR = 0.0;
    m_dKUL = 0.0;

    m_pUltraschallSensor  = new UltraschallSensor(UltraschallPin, UltraschallSensor_ID);
    m_pUltraschall_Thread = new QThread();
    m_pEncoderL           = new Encoder(EncoderPin_AL, EncoderPin_BL, EncoderL_ID);
    m_pEncoderL_Thread    = new QThread();
    m_pEncoderR           = new Encoder(EncoderPin_AR, EncoderPin_BR, EncoderR_ID);
    m_pEncoderR_Thread    = new QThread();

    m_pMotorL             = new DcMotor(MotorPin_AL,MotorPin_BL,MotorPin_EL);//left motor initialize
    m_pMotorR             = new DcMotor(MotorPin_AR,MotorPin_BR,MotorPin_ER);//right motor initialize

    initSensorThread(m_pUltraschallSensor, m_pUltraschall_Thread);
    initSensorThread(m_pEncoderL,    m_pEncoderL_Thread);
    initSensorThread(m_pEncoderR,    m_pEncoderR_Thread);
    connect(m_pUltraschallSensor,SIGNAL(sgn_newSensorData(int,double)),this,SIGNAL(sgn_showSensorValue(int,double)));
}
void MobilePlatform::initSensorThread(AbstractSensor* sensor, QThread *thread)
{
    sensor->moveToThread(thread);
    connect(sensor, SIGNAL(sgn_finished()), thread, SLOT(quit()));//
    connect(sensor, SIGNAL(sgn_finished()), sensor, SLOT(deleteLater()));//
    connect(thread, SIGNAL(finished()),     thread, SLOT(deleteLater()));//
    thread->start();
}
void MobilePlatform::activateSensor()
{
    m_pEncoderL -> activateSensor();
    m_pEncoderR -> activateSensor();
    m_pUltraschallSensor -> activateSensor();
}
void MobilePlatform::deactivateSensor()
{
    m_pEncoderL -> deactivateSensor();
    m_pEncoderR -> deactivateSensor();
    m_pUltraschallSensor -> deactivateSensor();
}
void MobilePlatform::resetSensor()
{
    m_pEncoderL -> resetSensor();
    m_pEncoderR -> resetSensor();
    m_pUltraschallSensor -> resetSensor();
}
void MobilePlatform::moveForward(double dDistance)//left and right motor forward
{
    m_dSollDistance = dDistance;//cm
    m_pMotorL->forward();
    m_pMotorR->forward();
    activateSensor();
    m_bIsControling = true;
    slot_doSpeedPIDControl();
}
void MobilePlatform::slot_moveForward()
{
    m_pMotorL->forward();
    m_pMotorR->forward();
    m_pMotorL->setPWM(10);
    m_pMotorR->setPWM(10);
}
void MobilePlatform::moveBackward(double dDistance)//left and right motor backward
{
    m_dSollDistance = dDistance;//cm
    m_pMotorL->backward();
    m_pMotorR->backward();
    activateSensor();
    m_bIsControling = true;
    slot_doSpeedPIDControl();
}
void MobilePlatform::slot_moveBackward()
{
    m_pMotorL->stopMotor();
    m_pMotorR->stopMotor();
    m_pMotorL->setPWM(10);
    m_pMotorR->setPWM(10);
}
void MobilePlatform::turnLeft()//left backward and right forward
{
    m_dSollDistance = 0.25*PI*L_WHEEL;//cm
    m_pMotorL->backward();
    m_pMotorR->forward();
    activateSensor();
    m_bIsControling = true;
    slot_doSpeedPIDControl();
}
void MobilePlatform::slot_turnLeft()
{
    m_pMotorL->backward();
    m_pMotorR->forward();
    m_pMotorL->setPWM(10);
    m_pMotorR->setPWM(10);
}
void MobilePlatform::turnRight()//left forward and right backward
{
    m_dSollDistance = 0.25*PI*L_WHEEL;//cm
    m_pMotorL->forward();
    m_pMotorR->backward();
    activateSensor();
    m_bIsControling = true;
    slot_doSpeedPIDControl();
}
void MobilePlatform::slot_turnRight()
{
    m_pMotorL->forward();
    m_pMotorR->backward();
    m_pMotorL->setPWM(10);
    m_pMotorR->setPWM(10);
}
void MobilePlatform::stopMotion()//left and right motor stop
{
    m_bIsControling = false;
    m_pMotorL->stopMotor();
    m_pMotorR->stopMotor();
//    m_pEncoderL -> deactivateSensor();
//    m_pEncoderR -> deactivateSensor();
    m_pEncoderL -> resetSensor();
    m_pEncoderR -> resetSensor();
    m_dSollDistance = 0;
    m_dIstPosition = 0;
    m_dIstPositionL = 0;
    m_dIstPositionR = 0;
    m_dSollSpeed = 0;
}
void MobilePlatform::slot_stopMotion()
{
    m_pMotorL->stopMotor();
    m_pMotorR->stopMotor();
}
void MobilePlatform::slot_stopAll()
{
    stopMotion();
    deactivateSensor();
    resetSensor();
}
void MobilePlatform::setKLLKLR(double dKLL,double dKLR)
{
    m_dKLL = dKLL;
    m_dKLR = dKLR;
}
void MobilePlatform::setKUL(double dKUL)
{
    m_dKUL = dKUL;
}
void MobilePlatform::slot_doSpeedPIDControl()
{
    /*
    double dEncoderStepsR_new  = 0;//no unit
    double dEncoderStepsR_last = 0;//no unit
    double dEncoderStepsL_last = 0;//no unit
    double dEncoderStepsL_new  = 0;//no unit
    double dIstSpeedL_new = 0;//cm per second
    double dIstSpeedL_last = 0;//cm per second
    double dIstSpeedR_new = 0;//cm per second
    double dIstSpeedR_last = 0;//cm per second
    double dDifferenceSpeedL_new = 0;//cm per second
    double dDifferenceSpeedR_new = 0;//cm per second
    double dDifferenceSpeedDiffL = 0;
    double dDifferenceSpeedDiffR = 0;
    double dSpeedSumL = 0;//cm per second
    double dSpeedSumR = 0;//cm per second
    double dSollPWML = 0;
    double dSollPWMR = 0;
    int    nUltra = 0;//parameter,used to stop mobileplatform,wenn it arrives at right lager
    while(m_bIsControling)
    {
        dEncoderStepsL_last = m_pEncoderL ->getEncoderSteps();
        dEncoderStepsR_last = m_pEncoderR ->getEncoderSteps();
        delayMicroseconds(ABTASTZEITSPEED);
        dEncoderStepsL_new  = m_pEncoderL ->getEncoderSteps();
        dEncoderStepsR_new  = m_pEncoderR ->getEncoderSteps();
        dIstSpeedL_new = (dEncoderStepsL_new - dEncoderStepsL_last)*2*PI*RADIUSWHEEL*DELTATS/TICS;
        dIstSpeedR_new = (dEncoderStepsR_new - dEncoderStepsR_last)*2*PI*RADIUSWHEEL*DELTATS/TICS;//  get real speeds of two motors
//        cout<<"dEncoderStepsL_new:"<<dIstSpeedL_new<<endl;
//        cout<<"dEncoderStepsR_new:"<<dIstSpeedR_new<<endl;
        if(m_dIstPosition <= 1.0)
        {
            m_dSollSpeed = 0.5*m_dSollSpeedGUI;
        }
        else
        {
            m_dSollSpeed = m_dSollSpeedGUI;
        }
        if(m_dIstPosition + 10>= m_dSollDistance)
        {
            //m_dSollSpeed = m_dSollSpeedGUI*0.7;
            m_dKLL = 0;//parameter,used to decide,if two linesensoren are used or not
            m_dKLR = 0;
        }
        if(m_dSollDistance <=25)
        {
            m_dSollSpeed = 7;
        }
        dIstSpeedL_new  *= (1+m_dKLR*digitalRead(LinePin_L));
        dIstSpeedR_new  *= (1+m_dKLL*digitalRead(LinePin_R));
        m_dIstPositionL = dEncoderStepsL_new*2*PI*RADIUSWHEEL/TICS;
        m_dIstPositionR = dEncoderStepsR_new*2*PI*RADIUSWHEEL/TICS;//define PI/TICS
        m_dIstPosition = (m_dIstPositionL + m_dIstPositionR)*0.5;
//        emit sgn_showMotorPositionL(EncoderL_ID,dPositionL);
//        emit sgn_showMotorPositionR(EncoderR_ID,dPositionR);
//        cout<<"dIstSpeedL_new:"<<dIstSpeedL_new<<endl;
//        cout<<"dIstSpeedR_new:"<<dIstSpeedR_new<<endl;
        dDifferenceSpeedL_new  = m_dSollSpeed - dIstSpeedL_new;
        dDifferenceSpeedR_new  = m_dSollSpeed - dIstSpeedR_new;
//        cout<<"dDifferenceSpeedL_new:"<<dDifferenceSpeedL_new<<endl;
//        cout<<"dDifferenceSpeedR_new:"<<dDifferenceSpeedR_new<<endl;
//        dDifferenceSpeedL_last = m_dSollSpeed - dIstSpeedL_last;
//        dDifferenceSpeedR_last = m_dSollSpeed - dIstSpeedR_last;
        dDifferenceSpeedDiffL  = dIstSpeedL_last - dIstSpeedL_new;//dDifferenceSpeedL_new - dDifferenceSpeedL_last
        dDifferenceSpeedDiffR  = dIstSpeedR_last - dIstSpeedR_new;//dDifferenceSpeedR_new - dDifferenceSpeedR_last
//        cout<<"m_dSollSpeed"<<m_dSollSpeed<<endl;
        dSpeedSumL += dDifferenceSpeedL_new;
        dSpeedSumR += dDifferenceSpeedR_new;
        dSollPWML = KPSL * dDifferenceSpeedL_new + KISL * dSpeedSumL + KDSL * (dDifferenceSpeedDiffL);
        dSollPWMR = KPSR * dDifferenceSpeedR_new + KISR * dSpeedSumR + KDSR * (dDifferenceSpeedDiffR);
//        cout<<"m_dSollPWML:"<<dSollPWML<<endl;
//        cout<<"m_dSollPWMR:"<<dSollPWMR<<endl;
        if(dSollPWML<0)
        {
            dSollPWML = 0;
        }
        else if(dSollPWML>99)
        {
            dSollPWML = 99;
        }
        if(dSollPWMR<0)
        {
            dSollPWMR = 0;
        }
        else if(dSollPWMR>99)
        {
            dSollPWMR= 99;
        }
//        you dian wen ti
//        zhi jiao wan linesensor hui gan rao,xu yao xiu gai
//        she zhi pan duan tiao jian,zui hou 10 cm jiang can shu she zhi wei 0,tong shi jiang su
//        cout<<"m_dSollPWML:"<<dSollPWML<<endl;
//        cout<<"m_dSollPWMR:"<<dSollPWMR<<endl;

        m_pMotorL->setPWM((int)(dSollPWML+0.5));
        m_pMotorR->setPWM((int)(dSollPWMR+0.5));
//        cout<<"m_dIstPosition:"<<m_dIstPosition<<endl;
        dIstSpeedL_last = dIstSpeedL_new;
        dIstSpeedR_last = dIstSpeedR_new;
//        dDifferenceSpeedL_last = dDifferenceSpeedL_new;
//        dDifferenceSpeedR_last = dDifferenceSpeedR_new;
//        emit sgn_showMotorSpeedL(EncoderL_ID,dIstSpeedL_new);
//        emit sgn_showMotorSpeedR(EncoderR_ID,dIstSpeedR_new);
       if(m_dIstPosition >= m_dSollDistance)
       {
           m_bIsControling = false;
           stopMotion();
           break;
       }
       if(m_dKUL == 1)
       {
         if(m_pUltraschallSensor->sendSensorValue() <= 30)//30cm,need to be tested and modified
         {
             nUltra++;
             if(nUltra == 5)//5 or 2, need to be tested
             {
                 m_bIsControling = false;
                 emit sgn_realLagerDistance(m_dIstPosition);
                 stopMotion();
                 break;
             }
          }
       }
       QCoreApplication::processEvents();//processes GUI-Events
    }
    */
    double dEncoderStepsR_new  = 0;//no unit
    double dEncoderStepsR_last = 0;//no unit
    double dEncoderStepsL_last = 0;//no unit
    double dEncoderStepsL_new  = 0;//no unit
    double dIstSpeedL_new = 0;//cm per second
    double dIstSpeedL_last = 0;//cm per second
    double dIstSpeedR_new = 0;//cm per second
    double dIstSpeedR_last = 0;//cm per second
    double dDifferenceSpeedL_new = 0;//cm per second
    double dDifferenceSpeedR_new = 0;//cm per second
    double dDifferenceSpeedDiffL = 0;
    double dDifferenceSpeedDiffR = 0;
    double dSpeedSumL = 0;//cm per second
    double dSpeedSumR = 0;//cm per second
    double dSollPWML = 0;//no unit
    double dSollPWMR = 0;//no unit
    while(m_bIsControling)
    {
        dEncoderStepsL_last = m_pEncoderL ->getEncoderSteps();
        dEncoderStepsR_last = m_pEncoderR ->getEncoderSteps();
        delayMicroseconds(ABTASTZEITSPEED);
        dEncoderStepsL_new  = m_pEncoderL ->getEncoderSteps();
        dEncoderStepsR_new  = m_pEncoderR ->getEncoderSteps();
        dIstSpeedL_new = (dEncoderStepsL_new - dEncoderStepsL_last)*2*PI*RADIUSWHEEL*DELTATS/TICS;
        dIstSpeedR_new = (dEncoderStepsR_new - dEncoderStepsR_last)*2*PI*RADIUSWHEEL*DELTATS/TICS;//  get real speeds of two motors
        if(m_dIstPosition <= 1.0)
        {
            m_dSollSpeed = 0.5*m_dSollSpeedGUI;
        }
        else
        {
            m_dSollSpeed = m_dSollSpeedGUI;
        }
        if(m_dIstPosition + 10>= m_dSollDistance)
        {
            m_dKLL = 0;//parameter,used to decide,if two linesensoren are used or not
            m_dKLR = 0;
        }
        if(m_dSollDistance <=25)
        {
            m_dSollSpeed = 6;
        }
        dIstSpeedL_new  *= (1+m_dKLL*digitalRead(LinePin_L));
        dIstSpeedR_new  *= (1+m_dKLR*digitalRead(LinePin_R));
        m_dIstPositionL = dEncoderStepsL_new*2*PI*RADIUSWHEEL/TICS;
        m_dIstPositionR = dEncoderStepsR_new*2*PI*RADIUSWHEEL/TICS;//define PI/TICS
        m_dIstPosition = (m_dIstPositionL + m_dIstPositionR)*0.5;
        dDifferenceSpeedL_new  = m_dSollSpeed - dIstSpeedL_new;
        dDifferenceSpeedR_new  = m_dSollSpeed - dIstSpeedR_new;
        dDifferenceSpeedDiffL  = dIstSpeedL_last - dIstSpeedL_new;//dDifferenceSpeedL_new - dDifferenceSpeedL_last
        dDifferenceSpeedDiffR  = dIstSpeedR_last - dIstSpeedR_new;//dDifferenceSpeedR_new - dDifferenceSpeedR_last
        dSpeedSumL += dDifferenceSpeedL_new;
        dSpeedSumR += dDifferenceSpeedR_new;
        dSollPWML = KPSL * dDifferenceSpeedL_new + KISL * dSpeedSumL + KDSL * (dDifferenceSpeedDiffL);
        dSollPWMR = KPSR * dDifferenceSpeedR_new + KISR * dSpeedSumR + KDSR * (dDifferenceSpeedDiffR);
        if(dSollPWML<0)
        {
            dSollPWML = 0;
        }
        else if(dSollPWML>99)
        {
            dSollPWML = 99;
        }
        if(dSollPWMR<0)
        {
            dSollPWMR = 0;
        }
        else if(dSollPWMR>99)
        {
            dSollPWMR= 99;
        }
        m_pMotorL->setPWM((int)(dSollPWML+0.5));
        m_pMotorR->setPWM((int)(dSollPWMR+0.5));
        dIstSpeedL_last = dIstSpeedL_new;
        dIstSpeedR_last = dIstSpeedR_new;
       if(m_dIstPosition >= m_dSollDistance)
       {
           m_bIsControling = false;
           stopMotion();
           break;
       }
       if(m_dKUL == 1)
       {
         if(m_pUltraschallSensor->sendSensorValue() <= 30)//30cm,need to be tested and modified
         {
             m_bIsControling = false;
             emit sgn_realLagerDistance(m_dIstPosition);
             stopMotion();
             break;
          }
       }
       QCoreApplication::processEvents();//processes GUI-Events
    }
}
MobilePlatform::~MobilePlatform()//destructor
{
    stopMotion();
    deactivateSensor();
    resetSensor();
    emit sgn_finished();
}
void MobilePlatform::slot_DSB_KPLValueChanged(double dKPL)
{
     m_dKPL = dKPL;
     qDebug()<<"m_dKPL"<<m_dKPL;
}
void MobilePlatform::slot_DSB_KILValueChanged(double dKIL)
{
    m_dKIL = dKIL;
    qDebug()<<"m_dKIL"<<m_dKIL;
}
void MobilePlatform::slot_DSB_KDLValueChanged(double dKDL)
{
    m_dKDL = dKDL;
    qDebug()<<"m_dKDL"<<m_dKDL;
}
void MobilePlatform::slot_DSB_KPRValueChanged(double dKPR)
{
    m_dKPR = dKPR;
    qDebug()<<"m_dKPR"<<m_dKPR;
}
void MobilePlatform::slot_DSB_KIRValueChanged(double dKIR)
{
    m_dKIR = dKIR;
    qDebug()<<"m_dKIR"<<m_dKIR;
}
void MobilePlatform::slot_DSB_KDRValueChanged(double dKDR)
{
    m_dKDR = dKDR;
    qDebug()<<"m_dKDR"<<m_dKDR;
}
void MobilePlatform::slot_DSB_setSpeed(double dSpeed)
{
    m_dSollSpeedGUI = dSpeed;
    qDebug()<<"m_dSollSpeedGUI"<<m_dSollSpeedGUI;
}

void MobilePlatform::slot_DSB_setDistance(double dDistance)
{
    m_dSollDistance = dDistance;
    qDebug()<<"m_dSollDistance"<<m_dSollDistance;
}
