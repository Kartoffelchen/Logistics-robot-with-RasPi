#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <softPwm.h>
#include <QThread>
#include <QTimer>
#include <wiringPi.h>
#include <QDebug>
#include <QApplication>
#include <QCoreApplication>
#include "namesanddefines.h"

class LEDControl: public QObject
{
    Q_OBJECT
public:
    LEDControl(int,int,int);
    ~LEDControl();
    void turnOnLED(int);
    void turnOffLED(int);
private:
    bool initLED();
    int  m_nPinRed;
    int  m_nPinGreen;
    int  m_nPinYellow;
    int  m_nBlinkColor;
    int  m_nBlinkFrequenz;
signals:
    void sgn_finished();
public slots:
    void slot_startBlink(int);
    void slot_stopBlinking();
    void slot_turnOff();
private:
    void blinkLED();
    bool m_bIsBlinking;
};

#endif // LEDCONTROL_H
