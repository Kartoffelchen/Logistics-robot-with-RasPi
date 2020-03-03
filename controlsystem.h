//!
//! \file conrtolsystem.h
//! \auther PAN Yunjie, YU Bohang, Team 03
//! \date 30-Nov-2018
//! \brief connect ui and mobileplatform
//!
//! this part use the "connect" function to connect the Commends on the user interface and the movement of the mobileplatform
//!
//! \param "m_pMyWindow" signalsender or signalreceiver in the "connect" function
//! \param "m_pMyCar" signalreceiver or signalsender in the "connect" function
//!
//! \return
//!
//! \pre
//! \post

#ifndef CONTROLSYSTEM_H
#define CONTROLSYSTEM_H

#include <QObject>
#include <QStateMachine>
#include "ledcontrol.h"
#include "mainwindow.h"
#include "mobileplatform.h"
#include "xmlparsen.h"

class ControlSystem: public QObject
{
    Q_OBJECT
public:
    ControlSystem();  //Constructor
    ~ControlSystem();//Destructor
public slots:
    void slot_StartToYellow();
    void slot_StartToGreen();
    void slot_StartToRed();

    void slot_loadArticel();

    void slot_YellowToClerkA();
    void slot_YellowToClerkB();

    void slot_unloadArticel();

    void slot_ClerkAToYellow();
    void slot_ClerkBToYellow();

    void slot_ClerkAToRed();
    void slot_ClerkBToRed();

    void slot_RedToClerkA();
    void slot_RedToClerkB();

    void slot_ClerkAToGreen();
    void slot_ClerkBToGreen();

    void slot_GreenToClerkA();
    void slot_GreenToClerkB();

    void slot_ClerkAToStart();
    void slot_ClerkBToStart();

    void slot_StandBy();
    void slot_StateGoingOn();
    void slot_StateQuit();
    void slot_StartWorking();
    void slot_exitGoingOn();

    void slot_setLagerDistance(double);

    void slot_btnOKClicked();

    void slot_LEDAufZu();

    void slot_doLoop();

signals:
    void sgn_startBlink(int);
    void sgn_stopBlink();
    void sgn_turnOff();

    void sgn_LoopFinished();

    void sgn_ArriveYellow();
    void sgn_ArriveClerkA();
    void sgn_ArriveClerkB();
    void sgn_ArriveRed();
    void sgn_ArriveGreen();
    void sgn_ArriveStart();

    void sgn_YellowGoingOn();
    void sgn_RedGoingOn();
    void sgn_GreenGoingOn();

    void sgn_LoadingFinishedRedClerkA();
    void sgn_LoadingFinishedRedClerkB();

    void sgn_LoadingFinishedGreenClerkA();
    void sgn_LoadingFinishedGreenClerkB();

    void sgn_LoadingFinishedYellowClerkA();
    void sgn_LoadingFinishedYellowClerkB();

    void sgn_UnloadingFinishedClerkARed();
    void sgn_UnloadingFinishedClerkBRed();

    void sgn_UnloadingFinishedClerkAYellow();
    void sgn_UnloadingFinishedClerkBYellow();

    void sgn_UnloadingFinishedClerkAGreen();
    void sgn_UnloadingFinishedClerkBGreen();

    void sgn_UnloadingFinishedClerkAStart();
    void sgn_UnloadingFinishedClerkBStart();

    void sgn_sendProzessInfo(QString,QString,QString);
    void sgn_sendOrderListInfo(QString,QString);    

    void sgn_btnWakeUpClicked();
private:
    LEDControl* m_pLedControl;
    QThread* m_pLedControl_Thread;
    XMLParsen* m_pXmlParsen;

    MainWindow *m_pMyWindow; //define the Object MyWindow

    MobilePlatform *m_pMyCar;//define the Object MyCar

    QStateMachine* m_pTopMachine;
    QState* m_pStandBy;
    QState* m_pStateGoingOn;
    QState* m_pStateQuit;
    QState* m_pdoLoop;

    QState* m_pStartWorking;

    QState* m_pUnloading;
    QState* m_pLoading;

    QState* m_pStartYellow;
    QState* m_pStartRed;
    QState* m_pStartGreen;

    QState* m_pYellowClerkA;
    QState* m_pYellowClerkB;

    QState* m_pClerkARed;
    QState* m_pClerkBRed;

    QState* m_pRedClerkA;
    QState* m_pRedClerkB;

    QState* m_pClerkAYellow;
    QState* m_pClerkBYellow;

    QState* m_pClerkAGreen;
    QState* m_pClerkBGreen;

    QState* m_pGreenClerkA;
    QState* m_pGreenClerkB;

    QState* m_pClerkAStart;
    QState* m_pClerkBStart;

    QList<XMLInfo> m_QlXMLInfo;
    int     m_nTaskNum;
    int     m_nDoingTaskNr;
    XMLInfo m_XMLInfo;
    double  m_dLagerDistance;

    void sendProzessInfo(int);
    void getXMLInfo();
};

#endif // CONTROLSYSTEM_H
