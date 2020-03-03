#include "controlsystem.h"

ControlSystem::ControlSystem()
{
    m_nDoingTaskNr = 0;
    m_dLagerDistance = 0;
    m_XMLInfo.subOrderID = "Error";
    m_XMLInfo.subOrderDescription = "Error";
    m_XMLInfo.subOrderStorageRack = "Error";
    m_XMLInfo.subOrderWarehouseClerk = "Error";
    /*************************************************************************************/

    m_pLedControl  = new LEDControl(LEDYELLOW_PIN,LEDRED_PIN,LEDGREEN_PIN);
    m_pLedControl_Thread = new QThread();
    m_pLedControl->moveToThread(m_pLedControl_Thread );
    connect(m_pLedControl, SIGNAL(sgn_finished()),    m_pLedControl_Thread , SLOT(quit()));//
    connect(m_pLedControl, SIGNAL(sgn_finished()),    m_pLedControl   , SLOT(deleteLater()));//
    connect(m_pLedControl_Thread, SIGNAL(finished()), m_pLedControl_Thread, SLOT(deleteLater()));//
    m_pLedControl_Thread->start();
//    m_pLedControl->blinkLED(LEDYELLOW_PIN);

    /*************************************************************************************/

    m_pXmlParsen = new XMLParsen();
    /*************************************************************************************/

    m_pMyCar = new MobilePlatform();//distribute memory address for Pointer m_pMyCar

    /*************************************************************************************/

    m_pMyWindow = new MainWindow(); //distribute memory address for Pointer MyWindow
    m_pMyWindow->show();//display the controlwindow

    /*************************************************************************************/

    connect(m_pMyWindow,SIGNAL(sgn_btnActivSensorClicked()),             m_pMyCar,SLOT(activateSensor()));//sensor begin to work
    connect(m_pMyWindow,SIGNAL(sgn_btnDeacSensorClicked()),              m_pMyCar,SLOT(deactivateSensor()));//pause the Sensors
    connect(m_pMyWindow,SIGNAL(sgn_btnResetSensorClicked()),             m_pMyCar,SLOT(resetSensor()));//reset the values of sensors

    connect(m_pMyWindow,SIGNAL(sgn_btnForwardClicked()),                 m_pMyCar,SLOT(slot_moveForward()));
    connect(m_pMyWindow,SIGNAL(sgn_btnBackwardClicked()),                m_pMyCar,SLOT(slot_moveBackward()));
    connect(m_pMyWindow,SIGNAL(sgn_btnLeftClicked()),                    m_pMyCar,SLOT(slot_turnLeft()));
    connect(m_pMyWindow,SIGNAL(sgn_btnRightClicked()),                   m_pMyCar,SLOT(slot_turnRight()));

    connect(m_pMyWindow,SIGNAL(sgn_DSB_KPLValueChanged(double)),         m_pMyCar,SLOT(slot_DSB_KPLValueChanged(double)));
    connect(m_pMyWindow,SIGNAL(sgn_DSB_KILValueChanged(double)),         m_pMyCar,SLOT(slot_DSB_KILValueChanged(double)));
    connect(m_pMyWindow,SIGNAL(sgn_DSB_KDLValueChanged(double)),         m_pMyCar,SLOT(slot_DSB_KDLValueChanged(double)));
    connect(m_pMyWindow,SIGNAL(sgn_DSB_KPRValueChanged(double)),         m_pMyCar,SLOT(slot_DSB_KPRValueChanged(double)));
    connect(m_pMyWindow,SIGNAL(sgn_DSB_KIRValueChanged(double)),         m_pMyCar,SLOT(slot_DSB_KIRValueChanged(double)));
    connect(m_pMyWindow,SIGNAL(sgn_DSB_KDRValueChanged(double)),         m_pMyCar,SLOT(slot_DSB_KDRValueChanged(double)));
    connect(m_pMyWindow,SIGNAL(sgn_DSB_setSpeedValueChanged(double)),    m_pMyCar,SLOT(slot_DSB_setSpeed(double)));
    connect(m_pMyWindow,SIGNAL(sgn_DSB_setDistanceValueChanged(double)), m_pMyCar,SLOT(slot_DSB_setDistance(double)));
    connect(m_pMyWindow,SIGNAL(sgn_XMLPathChanged(QString)),             m_pXmlParsen,SLOT(slot_setXMLPath(QString)));
    connect(m_pMyWindow,SIGNAL(sgn_btnOKClicked()),                      this,        SLOT(slot_btnOKClicked()));
    connect(this, SIGNAL(sgn_sendProzessInfo(QString,QString,QString)),  m_pMyWindow,SLOT(slot_showProzessInfo(QString,QString,QString)));
    connect(this, SIGNAL(sgn_sendOrderListInfo(QString,QString)),        m_pMyWindow,SLOT(slot_showOrderListInfo(QString,QString)));
    connect(this, SIGNAL(sgn_startBlink(int)),  m_pLedControl,SLOT(slot_startBlink(int)));
    connect(this, SIGNAL(sgn_stopBlink()),       m_pLedControl,SLOT(slot_stopBlinking()));
    connect(this, SIGNAL(sgn_turnOff()),        m_pLedControl,SLOT(slot_turnOff()));

    //connect(m_pMyWindow,SIGNAL(sgn_btnTryClicked()),this,SLOT(slot_LEDAufZu()));

    connect(m_pMyCar, SIGNAL(sgn_showSensorValue(int,double)), m_pMyWindow,SLOT(slot_showUltaschallInfo(int,double)));
    connect(m_pMyCar, SIGNAL(sgn_realLagerDistance(double)),   this,SLOT(slot_setLagerDistance(double)));//get Distances between lager and colorpunkt
    /*************************************************************************************/

    m_pTopMachine   = new QStateMachine();
    m_pStateGoingOn = new QState();
    m_pStateQuit    = new QState();
    m_pStandBy      = new QState();
    m_pdoLoop       = new QState();

    m_pStartWorking = new QState(m_pStateGoingOn);

    m_pLoading      = new QState(m_pStateGoingOn);

    m_pUnloading    = new QState(m_pStateGoingOn);

    m_pStartYellow  = new QState(m_pStateGoingOn);
    m_pStartRed     = new QState(m_pStateGoingOn);
    m_pStartGreen   = new QState(m_pStateGoingOn);

    m_pYellowClerkA = new QState(m_pStateGoingOn);
    m_pYellowClerkB = new QState(m_pStateGoingOn);

    m_pClerkAYellow = new QState(m_pStateGoingOn);
    m_pClerkBYellow = new QState(m_pStateGoingOn);

    m_pRedClerkA    = new QState(m_pStateGoingOn);
    m_pRedClerkB    = new QState(m_pStateGoingOn);

    m_pClerkARed    = new QState(m_pStateGoingOn);
    m_pClerkBRed    = new QState(m_pStateGoingOn);

    m_pGreenClerkA  = new QState(m_pStateGoingOn);
    m_pGreenClerkB  = new QState(m_pStateGoingOn);

    m_pClerkAGreen  = new QState(m_pStateGoingOn);
    m_pClerkBGreen  = new QState(m_pStateGoingOn);

    m_pClerkAStart  = new QState(m_pStateGoingOn);
    m_pClerkBStart  = new QState(m_pStateGoingOn);

    /*************************************************************************************/

    m_pStandBy->addTransition(m_pMyWindow,SIGNAL(sgn_btnStartClicked()),m_pStateGoingOn);
    connect(m_pStateGoingOn,SIGNAL(entered()),this,SLOT(slot_StateGoingOn()));
    connect(m_pStartWorking,SIGNAL(entered()),this,SLOT(slot_StartWorking()));
    connect(m_pStateGoingOn,SIGNAL(exited()), this,SLOT(slot_exitGoingOn()));

    m_pStandBy->addTransition(m_pMyWindow,SIGNAL(sgn_btnTryClicked()),m_pdoLoop);
    connect(m_pdoLoop,SIGNAL(entered()),this,SLOT(slot_doLoop()));    

    m_pStandBy->addTransition(m_pMyWindow,SIGNAL(sgn_btnStopClicked()),m_pStateQuit);

    m_pdoLoop->addTransition(m_pMyWindow,SIGNAL(sgn_btnStopClicked()),m_pStateQuit);
    connect(m_pStateQuit,SIGNAL(entered()),this,SLOT(slot_StateQuit()));

    m_pStateQuit->addTransition(m_pMyWindow,SIGNAL(sgn_btnWakeUpClicked()),m_pStandBy);

    m_pdoLoop->addTransition(this,SIGNAL(sgn_LoopFinished()),m_pStandBy);
    m_pStateGoingOn->addTransition(m_pMyWindow,SIGNAL(sgn_btnStopClicked()),m_pStateQuit);

    /*************************************************************************************/

    m_pStartWorking->addTransition(this,SIGNAL(sgn_YellowGoingOn()),m_pStartYellow);
    connect(m_pStartYellow,SIGNAL(entered()),this,SLOT(slot_StartToYellow()));

    m_pStartWorking->addTransition(this,SIGNAL(sgn_RedGoingOn()),m_pStartRed);
    connect(m_pStartRed,SIGNAL(entered()),this,SLOT(slot_StartToRed()));

    m_pStartWorking->addTransition(this,SIGNAL(sgn_GreenGoingOn()),m_pStartGreen);
    connect(m_pStartGreen,SIGNAL(entered()),this,SLOT(slot_StartToGreen()));
    /*************************************************************************************/

    m_pStartYellow->addTransition(this,SIGNAL(sgn_ArriveYellow()),m_pLoading);
    m_pClerkAYellow->addTransition(this,SIGNAL(sgn_ArriveYellow()),m_pLoading);
    m_pClerkBYellow->addTransition(this,SIGNAL(sgn_ArriveYellow()),m_pLoading);

    m_pStartRed->addTransition(this,SIGNAL(sgn_ArriveRed()),m_pLoading);
    m_pClerkARed->addTransition(this,SIGNAL(sgn_ArriveRed()),m_pLoading);
    m_pClerkBRed->addTransition(this,SIGNAL(sgn_ArriveRed()),m_pLoading);

    m_pStartGreen->addTransition(this,SIGNAL(sgn_ArriveGreen()),m_pLoading);
    m_pClerkAGreen->addTransition(this,SIGNAL(sgn_ArriveGreen()),m_pLoading);
    m_pClerkBGreen->addTransition(this,SIGNAL(sgn_ArriveGreen()),m_pLoading);

    connect(m_pLoading,SIGNAL(entered()),this,SLOT(slot_loadArticel()));
    /*************************************************************************************/

    m_pLoading->addTransition(this,SIGNAL(sgn_LoadingFinishedYellowClerkA()),m_pYellowClerkA);
    connect(m_pYellowClerkA,SIGNAL(entered()),this,SLOT(slot_YellowToClerkA()));

    m_pLoading->addTransition(this,SIGNAL(sgn_LoadingFinishedYellowClerkB()),m_pYellowClerkB);
    connect(m_pYellowClerkB,SIGNAL(entered()),this,SLOT(slot_YellowToClerkB()));    

    m_pLoading->addTransition(this,SIGNAL(sgn_LoadingFinishedRedClerkA()),m_pRedClerkA);
    connect(m_pRedClerkA,SIGNAL(entered()),this,SLOT(slot_RedToClerkA()));

    m_pLoading->addTransition(this,SIGNAL(sgn_LoadingFinishedRedClerkB()),m_pRedClerkB);
    connect(m_pRedClerkB,SIGNAL(entered()),this,SLOT(slot_RedToClerkB()));

    m_pLoading->addTransition(this,SIGNAL(sgn_LoadingFinishedGreenClerkA()),m_pGreenClerkA);
    connect(m_pGreenClerkA,SIGNAL(entered()),this,SLOT(slot_GreenToClerkA()));

    m_pLoading->addTransition(this,SIGNAL(sgn_LoadingFinishedGreenClerkB()),m_pGreenClerkB);
    connect(m_pGreenClerkB,SIGNAL(entered()),this,SLOT(slot_GreenToClerkB()));
    /*************************************************************************************/

    m_pYellowClerkA->addTransition(this,SIGNAL(sgn_ArriveClerkA()),m_pUnloading);
    m_pYellowClerkB->addTransition(this,SIGNAL(sgn_ArriveClerkB()),m_pUnloading);

    m_pRedClerkA->addTransition(this,SIGNAL(sgn_ArriveClerkA()),m_pUnloading);
    m_pRedClerkB->addTransition(this,SIGNAL(sgn_ArriveClerkB()),m_pUnloading);

    m_pGreenClerkA->addTransition(this,SIGNAL(sgn_ArriveClerkA()),m_pUnloading);
    m_pGreenClerkB->addTransition(this,SIGNAL(sgn_ArriveClerkB()),m_pUnloading);

    connect(m_pUnloading,SIGNAL(entered()),this,SLOT(slot_unloadArticel()));
    /*************************************************************************************/

    m_pUnloading->addTransition(this,SIGNAL(sgn_UnloadingFinishedClerkARed()),m_pClerkARed);
    connect(m_pClerkARed,SIGNAL(entered()),this,SLOT(slot_ClerkAToRed()));

    m_pUnloading->addTransition(this,SIGNAL(sgn_UnloadingFinishedClerkBRed()),m_pClerkBRed);
    connect(m_pClerkBRed,SIGNAL(entered()),this,SLOT(slot_ClerkBToRed()));

    m_pUnloading->addTransition(this,SIGNAL(sgn_UnloadingFinishedClerkAYellow()),m_pClerkAYellow);
    connect(m_pClerkAYellow,SIGNAL(entered()),this,SLOT(slot_ClerkAToYellow()));

    m_pUnloading->addTransition(this,SIGNAL(sgn_UnloadingFinishedClerkBYellow()),m_pClerkBYellow);
    connect(m_pClerkBYellow,SIGNAL(entered()),this,SLOT(slot_ClerkBToYellow()));

    m_pUnloading->addTransition(this,SIGNAL(sgn_UnloadingFinishedClerkAGreen()),m_pClerkAGreen);
    connect(m_pClerkAGreen,SIGNAL(entered()),this,SLOT(slot_ClerkAToGreen()));

    m_pUnloading->addTransition(this,SIGNAL(sgn_UnloadingFinishedClerkBGreen()),m_pClerkBGreen);
    connect(m_pClerkBGreen,SIGNAL(entered()),this,SLOT(slot_ClerkBToGreen()));

    m_pUnloading->addTransition(this,SIGNAL(sgn_UnloadingFinishedClerkAStart()),m_pClerkAStart);
    connect(m_pClerkAStart,SIGNAL(entered()),this,SLOT(slot_ClerkAToStart()));

    m_pUnloading->addTransition(this,SIGNAL(sgn_UnloadingFinishedClerkBStart()),m_pClerkBStart);
    connect(m_pClerkBStart,SIGNAL(entered()),this,SLOT(slot_ClerkBToStart()));
    /*************************************************************************************/

    m_pClerkAStart->addTransition(this,SIGNAL(sgn_ArriveStart()),m_pStandBy);
    m_pClerkBStart->addTransition(this,SIGNAL(sgn_ArriveStart()),m_pStandBy);
    connect(m_pStandBy,SIGNAL(entered()),this,SLOT(slot_StandBy()));

    /*************************************************************************************/
    m_pTopMachine->addState(m_pStateGoingOn);
    m_pTopMachine->addState(m_pStateQuit);
    m_pTopMachine->addState(m_pStandBy);
    m_pTopMachine->addState(m_pdoLoop);

    m_pStateGoingOn->setInitialState(m_pStartWorking);
    m_pTopMachine->setInitialState(m_pStandBy);
    m_pTopMachine->start();
}
/*************************************************************************************/
void ControlSystem::slot_LEDAufZu()
{
//     m_pLedControl->blinkLED(LEDGREEN_PIN);
}
/*************************************************************************************/
void ControlSystem::slot_setLagerDistance(double dLagerDistance)
{
    m_dLagerDistance = dLagerDistance;
}
void ControlSystem::slot_btnOKClicked()
{
    getXMLInfo();
}
/*************************************************************************************/
void ControlSystem::getXMLInfo()
{
    m_QlXMLInfo = m_pXmlParsen->getXMLInfo();
    m_nTaskNum = m_QlXMLInfo.size();
    emit sgn_sendOrderListInfo(m_pXmlParsen->sendOrderListID(),m_pXmlParsen->sendOrderListDescription());
    for(int idx = 0;idx<m_nTaskNum;idx++)
    {
        qDebug()<<m_QlXMLInfo.at(idx).subOrderID<<"|"
                <<m_QlXMLInfo.at(idx).subOrderDescription<<"|"
                <<m_QlXMLInfo.at(idx).subOrderStorageRack<<"|"
                <<m_QlXMLInfo.at(idx).subOrderWarehouseClerk;
    }
}
/*****************************************************/
void ControlSystem::slot_doLoop()
{
    m_pMyCar -> setKUL(0);

//    m_pLedControl->blinkLED(LEDGREEN_PIN);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKA_CLERKALEFT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_LEFTBOTTOM_CLERKALEFT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_STARTFRONT_LEFTBOTTOM);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_STARTFRONT_RIGHTBOTTOM);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RIGHTBOTTOM_CLERKBRIGHT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKB_CLERKBRIGHT);

//    m_pLedControl->stopBlinking();

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKB_CLERKBRIGHT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKBRIGHT_RIGHTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RIGHTTOP_GREEN);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RED_GREEN);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_YELLOW_RED);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_LEFTTOP_YELLOW);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKALEFT_LEFTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKA_CLERKALEFT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();
    m_pMyCar->turnLeft();
    /*************************************************************************************/
/*
    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKA_CLERKALEFT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKALEFT_LEFTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();
    
    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_LEFTTOP_YELLOW);
    
    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_YELLOW_RED);
    
    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RED_GREEN);
    
    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RIGHTTOP_GREEN);
    
    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();
    
    
    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RIGHTTOP_CLERKBRIGHT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKB_CLERKBRIGHT);

    m_pLedControl->stopBlinking();

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();
    m_pMyCar->turnLeft();
   

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKB_CLERKBRIGHT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RIGHTBOTTOM_CLERKBRIGHT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_STARTFRONT_RIGHTBOTTOM);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_STARTFRONT_LEFTBOTTOM);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKALEFT_LEFTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKA_CLERKALEFT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();
    m_pMyCar->turnLeft();*/
    emit sgn_LoopFinished();

}
/*************************************************************************************/
void ControlSystem::slot_StartToYellow()
{
   // qDebug()<<"I am StartToYellow!";
    m_pMyCar -> setKUL(0);
    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_START_STARTFRONT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_STARTFRONT_LEFTBOTTOM);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_LEFTBOTTOM_CLERKALEFT);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKALEFT_LEFTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_LEFTTOP_YELLOW);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    emit sgn_ArriveYellow();
}
void ControlSystem::slot_StartToRed()
{
   // qDebug()<<"I am StartToRed!";
     m_pMyCar -> setKUL(0);
    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_START_STARTFRONT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_STARTFRONT_LEFTBOTTOM);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_LEFTBOTTOM_CLERKALEFT);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKALEFT_LEFTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_LEFTTOP_YELLOW);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_YELLOW_RED);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();
    emit sgn_ArriveRed();
}
void ControlSystem::slot_StartToGreen()
{
    //qDebug()<<"I am StartToGreen!";
    m_pMyCar -> setKUL(0);
    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_START_STARTFRONT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_STARTFRONT_RIGHTBOTTOM);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RIGHTBOTTOM_CLERKBRIGHT);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RIGHTTOP_CLERKBRIGHT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RIGHTTOP_GREEN);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    emit sgn_ArriveGreen();
}
/*************************************************************************************/
void ControlSystem::slot_loadArticel()
{
    //qDebug()<<"I am loadArticel!";
    if(m_XMLInfo.subOrderStorageRack == "Yellow")
    {
//        m_pLedControl->blinkLED(LEDYELLOW_PIN);//blinken
        emit sgn_startBlink(LEDYELLOW_PIN);
    }
    else if(m_XMLInfo.subOrderStorageRack == "Red")
    {
//        m_pLedControl->blinkLED(LEDRED_PIN);//blinken
        emit sgn_startBlink(LEDRED_PIN);
    }
    else if(m_XMLInfo.subOrderStorageRack == "Green")
    {
//        m_pLedControl->blinkLED(LEDGREEN_PIN);//blinken
        emit sgn_startBlink(LEDGREEN_PIN);
    }
    else
    {
        qDebug()<<"XML Info Error! Loading cann not be finished!";
    }

    m_pMyCar->setKUL(1);//1 means using info of Ultraschallsensor to stop robot
    m_pMyCar->setKLLKLR(1,1);
    m_pMyCar->moveForward(DISTANCE_COLOR_ENDE);

    m_pMyCar->setKUL(0);
    m_pMyCar->setKLLKLR(1,1);
    m_pMyCar->moveForward(12.0);//cm

//    m_pLedControl->stopBlinking();//blinken
    emit sgn_stopBlink();
    //??????????????????

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();
    m_pMyCar->turnLeft();

    m_pMyCar->setKUL(0);
    m_pMyCar->setKLLKLR(1,1);
    m_pMyCar->moveForward(m_dLagerDistance + 12.0);//12 means using info of Colorsensor to stop robot,m_dLagerDistance

    if(m_XMLInfo.subOrderStorageRack == "Yellow")
    {
        if(m_XMLInfo.subOrderWarehouseClerk == "ClerkA")
        {
            emit sgn_LoadingFinishedYellowClerkA();
        }
        else if(m_XMLInfo.subOrderWarehouseClerk == "ClerkB")
        {
            emit sgn_LoadingFinishedYellowClerkB();
        }
        else
        {
            qDebug()<<"XML Info Error! Loading cann not be finished!";
        }
    }
    else if(m_XMLInfo.subOrderStorageRack == "Red")
    {
        if(m_XMLInfo.subOrderWarehouseClerk == "ClerkA")
        {
            emit sgn_LoadingFinishedRedClerkA();
        }
        else if(m_XMLInfo.subOrderWarehouseClerk == "ClerkB")
        {
            emit sgn_LoadingFinishedRedClerkB();
        }
        else
        {
            qDebug()<<"XML Info Error! Loading cann not be finished!";
        }
    }
    else if(m_XMLInfo.subOrderStorageRack == "Green")
    {
        if(m_XMLInfo.subOrderWarehouseClerk == "ClerkA")
        {
            emit sgn_LoadingFinishedGreenClerkA();
        }
        else if(m_XMLInfo.subOrderWarehouseClerk == "ClerkB")
        {

            emit sgn_LoadingFinishedGreenClerkB();
        }
        else
        {
            qDebug()<<"XML Info Error! Loading cann not be finished!";
        }
    }
    else
    {
        qDebug()<<"XML Info Error! Loading cann not be finished!";
    }    
    emit sgn_sendProzessInfo(m_XMLInfo.subOrderID,m_XMLInfo.subOrderDescription,m_XMLInfo.subOrderWarehouseClerk);
}
/*************************************************************************************/
void ControlSystem::slot_YellowToClerkA()
{    
   // qDebug()<<"I am YellowToClerkA!";
    m_pMyCar -> setKUL(0);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_LEFTTOP_YELLOW);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKALEFT_LEFTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();
    emit sgn_ArriveClerkA();
}
void ControlSystem::slot_YellowToClerkB()
{
    //qDebug()<<"I am YellowToClerkB!";
    m_pMyCar -> setKUL(0);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_YELLOW_RED);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RED_GREEN);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RIGHTTOP_GREEN);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RIGHTTOP_CLERKBRIGHT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();
    emit sgn_ArriveClerkB();
}

void ControlSystem::slot_RedToClerkA()
{
    //qDebug()<<"I am RedToClerkA!";
     m_pMyCar -> setKUL(0);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_YELLOW_RED);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_LEFTTOP_YELLOW);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKALEFT_LEFTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();
    emit sgn_ArriveClerkA();
}
void ControlSystem::slot_RedToClerkB()
{
   // qDebug()<<"I am RedToClerkB!";
    m_pMyCar -> setKUL(0);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RED_GREEN);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RIGHTTOP_GREEN);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RIGHTTOP_CLERKBRIGHT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();
   // qDebug()<<"I am RedToClerkB!";
    emit sgn_ArriveClerkB();
}
void ControlSystem::slot_GreenToClerkA()
{
    //qDebug()<<"I am GreenToClerkA!";
    m_pMyCar -> setKUL(0);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RED_GREEN);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_YELLOW_RED);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_LEFTTOP_YELLOW);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKALEFT_LEFTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();
    emit sgn_ArriveClerkA();
}
void ControlSystem::slot_GreenToClerkB()
{
   // qDebug()<<"I am GreenToClerkB!";
     m_pMyCar -> setKUL(0);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_GREEN_RIGHTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->moveForward(DISTANCE_RIGHTTOP_CLERKBRIGHT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();
    emit sgn_ArriveClerkB();
}
/*************************************************************************************/
void ControlSystem::slot_unloadArticel()
{   
    // qDebug()<<"I am unloadArticel!";
    m_pMyCar -> setKUL(0);
    if(m_XMLInfo.subOrderWarehouseClerk == "ClerkA")
    {
        m_pMyCar->setKLLKLR(KLL,KLR);
        m_pMyCar->moveForward(DISTANCE_CLERKA_CLERKALEFT);//
        delayMicroseconds(250000);
//        if(m_XMLInfo.subOrderStorageRack == "Yellow")
//        {
//            m_pLedControl->turnOffLED(LEDYELLOW_PIN);
        emit sgn_turnOff();

        delayMicroseconds(250000);
//        }
//        else if(m_XMLInfo.subOrderStorageRack == "Red")
//        {
//            m_pLedControl->turnOffLED(LEDRED_PIN);
//            emit sgn_turnOff();
//        }
//        else if(m_XMLInfo.subOrderStorageRack == "Green")
//        {
//            m_pLedControl->turnOffLED(LEDGREEN_PIN);
//            emit sgn_turnOff();
//        }
//        else
//        {
//            qDebug()<<"XML Info Error! Unloading cann not be finished!!!";
//        }

        m_pMyCar->moveBackward(DISTANCE_CLERKA_CLERKALEFT);

        if(m_nDoingTaskNr < m_nTaskNum)
        {
            m_XMLInfo = m_QlXMLInfo.at(m_nDoingTaskNr);
            if(m_XMLInfo.subOrderStorageRack == "Yellow")
            {
                emit sgn_UnloadingFinishedClerkAYellow();
            }
            else if(m_XMLInfo.subOrderStorageRack == "Red")
            {
                emit sgn_UnloadingFinishedClerkARed();
            }
            else if(m_XMLInfo.subOrderStorageRack == "Green")
            {
                emit sgn_UnloadingFinishedClerkAGreen();
            }
            else
            {
                qDebug()<<"XML Info Error! Unloading is Wrong!";
            }
            emit sgn_sendProzessInfo(m_XMLInfo.subOrderID,m_XMLInfo.subOrderDescription,m_XMLInfo.subOrderStorageRack);
        }
        else if(m_nDoingTaskNr == m_nTaskNum)
        {
            emit sgn_UnloadingFinishedClerkAStart();
            emit sgn_sendProzessInfo("no work to do","all finished","Start/Stop");
        }
        else
        {
            qDebug()<<"XML Info Error! Unloading is Wrong!";
        }
    }
    else if(m_XMLInfo.subOrderWarehouseClerk == "ClerkB")
    {

        m_pMyCar->setKLLKLR(KLL,KLR);
        m_pMyCar->moveForward(DISTANCE_CLERKB_CLERKBRIGHT);
        delayMicroseconds(500000);
        if(m_XMLInfo.subOrderStorageRack == "Yellow")
        {
            m_pLedControl->turnOffLED(LEDYELLOW_PIN);
        }
        else if(m_XMLInfo.subOrderWarehouseClerk == "Red")
        {
            m_pLedControl->turnOffLED(LEDRED_PIN);
        }
        else if(m_XMLInfo.subOrderStorageRack == "Green")
        {
            m_pLedControl->turnOffLED(LEDGREEN_PIN);
        }
        else
        {
            qDebug()<<"XML Info Error! Unloading cann not be finished!";
        }

        m_pMyCar->moveBackward(DISTANCE_CLERKB_CLERKBRIGHT);

        if(m_nDoingTaskNr < m_nTaskNum)
        {
            m_XMLInfo = m_QlXMLInfo.at(m_nDoingTaskNr);
            if(m_XMLInfo.subOrderStorageRack == "Yellow")
            {
                emit sgn_UnloadingFinishedClerkBYellow();
            }
            else if(m_XMLInfo.subOrderStorageRack == "Red")
            {
                emit sgn_UnloadingFinishedClerkBRed();
            }
            else if(m_XMLInfo.subOrderStorageRack == "Green")
            {
                emit sgn_UnloadingFinishedClerkBGreen();
            }
            else
            {
                qDebug()<<"XML Info Error! Unloading is Wrong!";
            }
            emit sgn_sendProzessInfo(m_XMLInfo.subOrderID,m_XMLInfo.subOrderDescription,m_XMLInfo.subOrderStorageRack);
        }
        else if(m_nDoingTaskNr == m_nTaskNum)
        {
            emit sgn_UnloadingFinishedClerkBStart();
            emit sgn_sendProzessInfo("no work to do","all finished","Start/Stop");
        }
        else
        {
            qDebug()<<"XML Info Error! Unloading is Wrong!";
        }
    }
    else
    {
        qDebug()<<"XML Info Error! Unloading cann not be finished!";
    }
    m_nDoingTaskNr++;
}
/*************************************************************************************/
void ControlSystem::slot_ClerkAToYellow()
{
   // qDebug()<<"I am ClerkAToYellow!";
     m_pMyCar -> setKUL(0);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKALEFT_LEFTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_LEFTTOP_YELLOW);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();
    emit sgn_ArriveYellow();
}
void ControlSystem::slot_ClerkBToYellow()
{
   // qDebug()<<"I am ClerkBToYellow!";
     m_pMyCar -> setKUL(0);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKBRIGHT_RIGHTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_GREEN_RIGHTTOP);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RED_GREEN);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_YELLOW_RED);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();
    emit sgn_ArriveYellow();
}
void ControlSystem::slot_ClerkAToRed()
{
  //  qDebug()<<"I am ClerkAToRed!";
     m_pMyCar -> setKUL(0);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKALEFT_LEFTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_LEFTTOP_YELLOW);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_YELLOW_RED);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();
    emit sgn_ArriveRed();
}
void ControlSystem::slot_ClerkBToRed()
{
   // qDebug()<<"I am ClerkBToRed!";
    m_pMyCar -> setKUL(0);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKBRIGHT_RIGHTTOP);


    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_GREEN_RIGHTTOP);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RED_GREEN);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();
    emit sgn_ArriveRed();
}
void ControlSystem::slot_ClerkAToGreen()
{
  //  qDebug()<<"I am ClerkAToGreen!";
    m_pMyCar -> setKUL(0);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKALEFT_LEFTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_LEFTTOP_YELLOW);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_YELLOW_RED);

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RED_GREEN);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();
    emit sgn_ArriveGreen();
}
void ControlSystem::slot_ClerkBToGreen()
{
  //  qDebug()<<"I am ClerkBToGreen!";
     m_pMyCar -> setKUL(0);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_CLERKBRIGHT_RIGHTTOP);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RIGHTTOP_GREEN);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();
    emit sgn_ArriveGreen();
}
void ControlSystem::slot_ClerkAToStart()
{
   // qDebug()<<"I am ClerkAToStart!";
    m_pMyCar -> setKUL(0);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_LEFTBOTTOM_CLERKALEFT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_STARTFRONT_LEFTBOTTOM);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_START_STARTFRONT);
    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();
    m_pMyCar->turnRight();
    emit sgn_ArriveStart();
}
void ControlSystem::slot_ClerkBToStart()
{
  //  qDebug()<<"I am ClerkBToStart!";
    m_pMyCar -> setKUL(0);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_RIGHTBOTTOM_CLERKBRIGHT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnRight();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_STARTFRONT_RIGHTBOTTOM);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();

    m_pMyCar->setKLLKLR(KLL,KLR);
    m_pMyCar->moveForward(DISTANCE_START_STARTFRONT);

    m_pMyCar->setKLLKLR(0,0);
    m_pMyCar->turnLeft();
    m_pMyCar->turnLeft();
    emit sgn_ArriveStart();
}
/*************************************************************************************/
void ControlSystem::slot_StandBy()
{
    qDebug()<<"I am Standby!";
    emit sgn_sendProzessInfo("Work not started!","I am waiting!","Order please!");
}
void ControlSystem::slot_StateGoingOn()
{
//    qDebug()<<"I am StateGoingOn!";
    m_pMyCar->activateSensor();
    emit sgn_sendOrderListInfo(m_pXmlParsen->sendOrderListID(),m_pXmlParsen->sendOrderListDescription());    
}
void ControlSystem::slot_StartWorking()
{
   // qDebug()<<"I am StartWorking!";
    m_XMLInfo = m_QlXMLInfo.at(0);
    m_nDoingTaskNr = 1;
    emit sgn_sendProzessInfo(m_XMLInfo.subOrderID,m_XMLInfo.subOrderDescription,m_XMLInfo.subOrderStorageRack);
    if(m_XMLInfo.subOrderStorageRack == "Yellow")
    {
        emit sgn_YellowGoingOn();
    }
    else if(m_XMLInfo.subOrderStorageRack == "Red")
    {
        emit sgn_RedGoingOn();
    }
    else if(m_XMLInfo.subOrderStorageRack == "Green")
    {
        emit sgn_GreenGoingOn();
    }
    else
    {
        qDebug()<<"Something is wrong! Work quitted! Please check me!";
    }
}
void ControlSystem::slot_exitGoingOn()
{
    m_pMyCar->deactivateSensor();
    m_pMyCar->resetSensor();
}
void ControlSystem::slot_StateQuit()
{
    m_pMyCar->stopMotion();
    qDebug()<<"I am StateQuit!";
    emit sgn_sendProzessInfo("Something wrong!","Work quitted!","Please check me!");
}
/*************************************************************************************/

ControlSystem::~ControlSystem()
{
    delete m_pLedControl;
    delete m_pLedControl_Thread;

    delete m_pXmlParsen;

    delete m_pMyWindow;

    delete m_pMyCar;

    delete m_pTopMachine;

    delete m_pStandBy;
    delete m_pStateGoingOn;
    delete m_pStateQuit;
    //delele m_pdoLoop;

    delete m_pUnloading;
    delete m_pLoading;

    delete m_pStartYellow;
    delete m_pStartGreen;
    delete m_pStartRed;

    delete m_pYellowClerkA;
    delete m_pYellowClerkB;

    delete m_pClerkARed;
    delete m_pClerkBRed;

    delete m_pClerkAYellow;
    delete m_pClerkBYellow;

    delete m_pRedClerkA;
    delete m_pRedClerkB;

    delete m_pClerkAGreen;
    delete m_pClerkBGreen;

    delete m_pGreenClerkA;
    delete m_pGreenClerkB;

    delete m_pClerkAStart;
    delete m_pClerkBStart;
}

