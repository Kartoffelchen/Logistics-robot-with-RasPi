#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)//distribute memory address for Pointer ui
{
    ui->setupUi(this);

    ui->btn_Sure->setEnabled(false);
    ui->btn_Cancel->setEnabled(true);
    ui->DSB_KPL->setEnabled(false);
    ui->DSB_KIL->setEnabled(false);
    ui->DSB_KDL->setEnabled(false);
    ui->DSB_KPR->setEnabled(false);
    ui->DSB_KIR->setEnabled(false);
    ui->DSB_KDR->setEnabled(false);
    ui->DSB_setDistance->setEnabled(false);
    ui->DSB_setSpeed->setEnabled(false);
    ui->btn_Stop->setEnabled(false);

    ui->DSB_KPL->setValue(0.15);
    ui->DSB_KIL->setValue(0.40);
    ui->DSB_KDL->setValue(0.1);
    ui->DSB_KPR->setValue(0.20);
    ui->DSB_KIR->setValue(0.4);
    ui->DSB_KDR->setValue(0.1);
    ui->DSB_setDistance->setValue(100);
    ui->DSB_setSpeed->setValue(12);
    ui->lineEditPathXML->setText("/home/pi/Desktop/MyCar/Challenge/WallE/example_deliveryPlan.xml");
    ui->btn_deactivateSensor->setEnabled(false);
    ui->btn_resetSensor->setEnabled(false);

    connect(ui->btn_Forward,  SIGNAL(pressed()),this,SIGNAL(sgn_btnForwardClicked()));
    connect(ui->btn_Backward, SIGNAL(pressed()),this,SIGNAL(sgn_btnBackwardClicked()));
    connect(ui->btn_Links,    SIGNAL(pressed()),this,SIGNAL(sgn_btnLeftClicked()));
    connect(ui->btn_Rechts,   SIGNAL(pressed()),this,SIGNAL(sgn_btnRightClicked()));

    connect(ui->btn_deactivateSensor,  SIGNAL(pressed()),this,SLOT(slot_btnDeacSensorClicked()));
    connect(ui->btn_resetSensor,       SIGNAL(pressed()),this,SLOT(slot_btnResetSensorClicked()));
    connect(ui->btn_activateSensor,    SIGNAL(pressed()),this,SLOT(slot_btnActivSensorClicked()));

    connect(ui->DSB_KPL,SIGNAL(valueChanged(double)),this,SIGNAL(sgn_DSB_KPLValueChanged(double)));
    connect(ui->DSB_KPR,SIGNAL(valueChanged(double)),this,SIGNAL(sgn_DSB_KPRValueChanged(double)));
    connect(ui->DSB_KIL,SIGNAL(valueChanged(double)),this,SIGNAL(sgn_DSB_KILValueChanged(double)));
    connect(ui->DSB_KIR,SIGNAL(valueChanged(double)),this,SIGNAL(sgn_DSB_KIRValueChanged(double)));
    connect(ui->DSB_KDL,SIGNAL(valueChanged(double)),this,SIGNAL(sgn_DSB_KDLValueChanged(double)));
    connect(ui->DSB_KDR,SIGNAL(valueChanged(double)),this,SIGNAL(sgn_DSB_KDRValueChanged(double)));
    connect(ui->DSB_setSpeed,   SIGNAL(valueChanged(double)),this,SIGNAL(sgn_DSB_setSpeedValueChanged(double)));
    connect(ui->DSB_setDistance,SIGNAL(valueChanged(double)),this,SIGNAL(sgn_DSB_setDistanceValueChanged(double)));

    connect(ui->btn_Sure,  SIGNAL(pressed()),this,SLOT(slot_btnSureClicked()));
    connect(ui->btn_Cancel,SIGNAL(pressed()),this,SLOT(slot_btnCancelClicked()));

    connect(ui->btn_Stop,  SIGNAL(pressed()),this,SLOT(slot_btnStopClicked()));
    connect(ui->btn_Start, SIGNAL(pressed()),this,SLOT(slot_btnStartClicked()));
    connect(ui->btn_Try,   SIGNAL(pressed()),this,SIGNAL(sgn_btnTryClicked()));
    connect(ui->btn_WakeUp,SIGNAL(pressed()),this,SIGNAL(sgn_btnWakeUpClicked()));
}

void MainWindow::slot_btnStopClicked()
{
    ui->btn_Start->setEnabled(true);
    ui->btn_Stop->setEnabled(false);
    emit sgn_btnStopClicked();
}
void MainWindow::slot_btnStartClicked()
{
    ui->btn_Start->setEnabled(false);
    ui->btn_Stop->setEnabled(true);
    emit sgn_btnStartClicked();
}
void MainWindow::slot_btnSureClicked()
{
    ui->btn_Sure->setEnabled(false);
    ui->btn_Cancel->setEnabled(true);
    ui->DSB_KPL->setEnabled(false);
    ui->DSB_KIL->setEnabled(false);
    ui->DSB_KDL->setEnabled(false);
    ui->DSB_KPR->setEnabled(false);
    ui->DSB_KIR->setEnabled(false);
    ui->DSB_KDR->setEnabled(false);
    ui->DSB_setDistance->setEnabled(false);
    ui->DSB_setSpeed->setEnabled(false);
}
void MainWindow::slot_btnCancelClicked()
{
    ui->btn_Sure->setEnabled(true);
    ui->btn_Cancel->setEnabled(false);
    ui->DSB_KPL->setEnabled(true);
    ui->DSB_KIL->setEnabled(true);
    ui->DSB_KDL->setEnabled(true);
    ui->DSB_KPR->setEnabled(true);
    ui->DSB_KIR->setEnabled(true);
    ui->DSB_KDR->setEnabled(true);
    ui->DSB_setDistance->setEnabled(true);
    ui->DSB_setSpeed->setEnabled(true);
    ui->lineEditPathXML->setEnabled(true);
}

void MainWindow::slot_btnResetSensorClicked()
{
    ui->btn_activateSensor->setEnabled(true);
    ui->btn_deactivateSensor->setEnabled(false);
    ui->btn_resetSensor->setEnabled(false);
    emit sgn_btnResetSensorClicked();
}
void MainWindow::slot_btnActivSensorClicked()
{
    ui->btn_activateSensor->setEnabled(false);
    ui->btn_deactivateSensor->setEnabled(true);
    ui->lcdN_CurrentDistance->setEnabled(true);
    emit sgn_btnActivSensorClicked();
}
void MainWindow::slot_btnDeacSensorClicked()
{    
    ui->btn_activateSensor->setEnabled(true);
    ui->btn_deactivateSensor->setEnabled(false);
    ui->btn_resetSensor->setEnabled(true);
    emit sgn_btnDeacSensorClicked();
}

void MainWindow::slot_showOrderListInfo(QString qID, QString qDescription)
{
    ui->textOrderListID->setText(qID);
    ui->textOrderListDescription->setText(qDescription);
}

void MainWindow::slot_showUltaschallInfo(int nID, double SensorValue)
{
     ui->lcdN_CurrentDistance->display(SensorValue);
}
void MainWindow::slot_showProzessInfo(QString qID, QString qDescription, QString qProzess)
{
    ui->textOrderID->setText(qID);
    ui->textOrderDescription->setText(qDescription);
    ui->textProzess->setText(qProzess);
}
void MainWindow::on_lineEditPathXML_textChanged(const QString qXMLPath)
{
    emit sgn_XMLPathChanged(qXMLPath);
}
void MainWindow::on_btn_OKXML_clicked()
{
    ui->lineEditPathXML->setEnabled(false);
    ui->btn_OKXML->setEnabled(false);
    emit sgn_btnOKClicked();
}
MainWindow::~MainWindow()
{
    delete ui;//release the address of ui in memory
}


