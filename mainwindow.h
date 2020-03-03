//!
//! \file mainwindow.h
//! \auther PAN Yunjie, YU Bohang, Team 03
//! \date 30-Nov-2018
//! \brief user interface
//!
//! get the signals of user interface and connect the movement of the mobileplatform
//!
//! \param "ui" pointer of the interface
//!
//! \pre
//! \post

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "namesanddefines.h"

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);//construktor
    ~MainWindow();//destruktor
signals://signal functions
    void sgn_btnForwardClicked();//button of Forward clicked
    void sgn_btnBackwardClicked();//button of Backward clicked
    void sgn_btnLeftClicked();//button of Turn Left clicked
    void sgn_btnRightClicked();//button of Turn Right clicked
    void sgn_btnDeacSensorClicked();//in manual model, suspend the sensors
    void sgn_btnResetSensorClicked();//in manual model, reset the value of sensors
    void sgn_btnActivSensorClicked();//in manual model, start sensors to record the data    
    void sgn_WindowDestroyed();//window destroyed
    void sgn_btnStopClicked();//button of Stopmotor clicked
    void sgn_btnStartClicked();
    void sgn_DSB_KPLValueChanged(double);
    void sgn_DSB_KILValueChanged(double);
    void sgn_DSB_KDLValueChanged(double);
    void sgn_DSB_KPRValueChanged(double);
    void sgn_DSB_KIRValueChanged(double);
    void sgn_DSB_KDRValueChanged(double);
    void sgn_DSB_setSpeedValueChanged(double);
    void sgn_DSB_setDistanceValueChanged(double);
    void sgn_btnTryClicked();
    void sgn_XMLPathChanged(const QString);
    void sgn_btnOKClicked();
    void sgn_btnWakeUpClicked();

public slots://slot functions
    void slot_btnStartClicked();
    void slot_btnStopClicked();//when button of 'Stop' clicked,call this function, stop the motors.   
    void slot_btnResetSensorClicked();//when button of 'Reset' clicked,call this function, refresh the value of sensor to 0
    void slot_btnActivSensorClicked();//when button of 'Activ' clicked,call this function, start to record the value of sensor
    void slot_btnDeacSensorClicked();//when button of 'Deac' clicked,call this function, stop recording the value of sensor

    void slot_btnCancelClicked();
    void slot_btnSureClicked();//when button of 'Sure' clicked,call this function, ensure the SollDistance of Robot

    void slot_showProzessInfo(QString,QString,QString);
    void slot_showOrderListInfo(QString, QString);
    void slot_showUltaschallInfo(int,double);
    void on_lineEditPathXML_textChanged(const QString);
    void on_btn_OKXML_clicked();
private:
    Ui::MainWindow *ui; //Pointer of Mainwindow
};

#endif // MAINWINDOW_H

