#include "xh_test.h"
#include "ui_xh_test.h"
#include "cancomm.h"
#include"icancmd.h"
#include <QTimer>
#include<QMessageBox>
#include<QFileInfo>
#include <QDebug>
//#include <QTime>
#include <QVector>
XH_Test::XH_Test(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XH_Test)
{
    ui->setupUi(this);
    QObject::connect(this, SIGNAL(GasSliderReturn(int)), this, SLOT(on_GasSlider_SetValue(int)));
    QObject::connect(this, SIGNAL(AcclerationSliderReturn(int)), this, SLOT(on_AcclerationSlider_SetValue(int)));
    QObject::connect(this, SIGNAL(StrAngleSliderReturn(int)), this, SLOT(on_StrAngleSlider_SetValue(int)));
}

XH_Test::~XH_Test()
{
    delete ui;
}

void XH_Test::on_CanButton_toggled(bool checked)
{
    CCanComm canreceive;
    if(checked==1)
    {
        canreceive.Init();
        QTimer *timer = new QTimer(this);
        timer->start(50);
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(VCU_WorkMode_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(GearPosition_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(GasPedal_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(BrakePedal_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Speed_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(MiddleDoor_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(FailureLevel_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(VehicleState_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(VCU_State_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(RotationDir_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Gas_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Bfog_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(SmallLight_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(HighBeam_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(LowBeam_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(LeftLight_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(RightLight_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Fog_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Hazard_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(BrakeLight_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(EPS_WorkMode_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(StrAngle_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Torque_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(AngleSensor_SetValue()));
//        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(EBS_WorkMode_SetValue()));
//        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(DeAccelaration_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Button_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Flwheelspeed_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Frwheelspeed_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Blwheelspeed_SetValue()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Brwheelspeed_SetValue()));//GHJ20170915 轮速显示
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(AccurateSpeed_SetValue()));//GHJ20170915 轮速显示
    }
    else
    {
        canreceive.shutdown();
    }

}

void XH_Test::VCU_WorkMode_SetValue()
{
    ui->VCU_WorkMode->setText(QString::number(VCU_WorkMode));
}

void XH_Test::GearPosition_SetValue()
{
    if(GearPosition==0x00)
        ui->GearPosition->setText(QString("N"));//20180605
    else if(GearPosition==0x03)
        ui->GearPosition->setText(QString("D"));
    else if(GearPosition==0x04)
        ui->GearPosition->setText(QString("R"));
    else if(GearPosition==0x02)
        ui->GearPosition->setText(QString("P"));
    else
        ui->GearPosition->setText(QString("No Feeback"));
}

void XH_Test::GasPedal_SetValue()
{
    ui->GasPedal->setText(QString::number(GasPedalPosition));
}

void XH_Test::BrakePedal_SetValue()
{
    ui->BrakePedal->setText(QString::number(BrakePedalPosition));
}

void XH_Test::Speed_SetValue()
{
    ui->Speed->setText(QString::number(Speed));
}

void XH_Test::MiddleDoor_SetValue()
{
    ui->MiddleDoor->setText(QString::number(MiddleDoorState));
}

void XH_Test::FailureLevel_SetValue()
{
    ui->FailureLevel->setText(QString::number(Horn));
}

void XH_Test::VehicleState_SetValue()
{
    ui->VehicleState->setText(QString::number(FrontDoor));
}

void XH_Test::VCU_State_SetValue()
{
    if(VCU_State==0)
        ui->VCU_State->setText("No Change");
    else if(VCU_State==1)
        ui->VCU_State->setText("Mode Change");
    else if(VCU_State==2)
        ui->VCU_State->setText("Stop");
    else if(VCU_State==3)
        ui->VCU_State->setText("Must Change");
    else
        ui->GearPosition->setText(QString("No Feeback"));
}

void XH_Test::RotationDir_SetValue()
{
    if(RotationSpdDir==0)
        ui->RotationDir->setText(QString("Forward"));
    else if(RotationSpdDir==1)
        ui->RotationDir->setText(QString("Backward"));
    else
        ui->GearPosition->setText(QString("No Feeback"));
}

void XH_Test::Gas_SetValue()
{
    ui->Gas->setText(QString::number(VCU_Accelaration));
}

void XH_Test::Bfog_SetValue()
{
    ui->Bfog->setText(QString::number(Bfog_Lamp));
}

void XH_Test::SmallLight_SetValue()
{
    ui->SmallLight->setText(QString::number(Small_Lamp));
}

void XH_Test::HighBeam_SetValue()
{
    ui->HighBeam->setText(QString::number(High_Beam));
}

void XH_Test::LowBeam_SetValue()
{
    ui->LowBeam->setText(QString::number(Low_Beam));
}

void XH_Test::LeftLight_SetValue()
{
    ui->LeftLight->setText(QString::number(Left_Lamp));
}

void XH_Test::RightLight_SetValue()
{
    ui->RightLight->setText(QString::number(Right_Lamp));
}

void XH_Test::Fog_SetValue()
{
    ui->Fog->setText(QString::number(Fog_Lamp));
}

void XH_Test::Hazard_SetValue()
{
    ui->Hazard->setText(QString::number(Hazard_Lamp));
}

void XH_Test::BrakeLight_SetValue()
{
    ui->BrakeLight->setText(QString::number(Stop_Lamp));
}

void XH_Test::EPS_WorkMode_SetValue()
{
    ui->EPS_WorkMode->setText(QString::number(EPS_State));
}

void XH_Test::StrAngle_SetValue()
{
    ui->StrAngle->setText(QString::number(EPS_ActualAngle));
}

void XH_Test::Torque_SetValue()
{
    ui->Torque->setText(QString::number(EPS_ActualTorque));
}

void XH_Test::AngleSensor_SetValue()
{
    ui->AngleSensor->setText(QString::number(EPS_ControlState));
}

void XH_Test::Button_SetValue()
{
    ui->Button->setText(QString::number(ButtonState));
}

void XH_Test::Flwheelspeed_SetValue()
{
    ui->Flwheelspeed->setText(QString::number(Flwheelspeed));
}

void XH_Test::Frwheelspeed_SetValue()
{
    ui->Frwheelspeed->setText(QString::number(Frwheelspeed));
}

void XH_Test::Blwheelspeed_SetValue()
{
    ui->Blwheelspeed->setText(QString::number(Blwheelspeed));
}

void XH_Test::Brwheelspeed_SetValue()
{
    ui->Brwheelspeed->setText(QString::number(Brwheelspeed));
}

void XH_Test::AccurateSpeed_SetValue()
{
    ui->AccurateSpeed->setText(QString::number(AccurateSpeed));
}

void XH_Test::on_longitutdectrl_enabled_toggled(bool checked)
{
    if(checked==1)
    {
        longitutdectrl_enabled=1;
    }
    else
    {
        longitutdectrl_enabled=0;
    }
}

void XH_Test::on_lateralctrl_enabled_toggled(bool checked)
{
    if(checked==1)
    {
        lateralctrl_enabled=1;
    }
    else
    {
        lateralctrl_enabled=0;
    }
}

void XH_Test::on_N_clicked()
{
    shift=0;
}

void XH_Test::on_D_clicked()
{
    shift=0x01;
}

void XH_Test::on_R_clicked()
{
    shift=0x02;
}

void XH_Test::on_GasEdit_editingFinished()
{
    QString str=ui->GasEdit->text();
    double a=str.toDouble();
    if(a>0&&a<100)
        throttle=a;
    else
        throttle=0;
}

void XH_Test::on_GasSlider_valueChanged(int value)
{
    throttle=value;
    emit GasSliderReturn(throttle);
}

void XH_Test::on_GasSlider_SetValue(int temp)
{
    ui->Gastext->setText(QString::number(temp));
}



void XH_Test::on_AcclerationEdit_editingFinished()
{
    QString str=ui->AcclerationEdit->text();
    double a=str.toDouble();
    if(a>0&&a<1000)
        brake=a;
    else
        brake=0;
}

void XH_Test::on_AcclerationSlider_valueChanged(int value)
{
    brake=value;
    emit AcclerationSliderReturn(brake);
}

void XH_Test::on_AcclerationSlider_SetValue(int temp)
{
    ui->AccelarateText->setText(QString::number(temp));
}

void XH_Test::on_StrAngleEdit_editingFinished()
{
    QString str=ui->StrAngleEdit->text();
    double a=str.toDouble();
    if(a>-720&&a<720)
        fDeFLRWheelAverAngle=a;
    else
        fDeFLRWheelAverAngle=0;
}

void XH_Test::on_StrAngleSlider_valueChanged(int value)
{
    fDeFLRWheelAverAngle=value;
    emit StrAngleSliderReturn(fDeFLRWheelAverAngle);
}

void XH_Test::on_StrAngleSlider_SetValue(int temp)
{
    ui->StrAngleText->setText(QString::number(temp));
}


void XH_Test::on_BfogControl_toggled(bool checked)
{
    if(checked==1)
    {
        Bfog_Lamp_turnedon=1;
    }
    else
    {
        Bfog_Lamp_turnedon=0;
    }
}

void XH_Test::on_SmallLightControl_toggled(bool checked)
{
    if(checked==1)
    {
        Small_Lamp_turnedon=1;
    }
    else
    {
        Small_Lamp_turnedon=0;
    }
}

void XH_Test::on_HighBeamControl_toggled(bool checked)
{
    if(checked==1)
    {
        HighBeam_turnedon=1;
    }
    else
    {
        HighBeam_turnedon=0;
    }
}



void XH_Test::on_LowBeamControl_toggled(bool checked)
{
    if(checked==1)
    {
        LowBeam_turnedon=1;
    }
    else
    {
        LowBeam_turnedon=0;
    }
}

void XH_Test::on_LeftLightControl_toggled(bool checked)
{
    if(checked==1)
    {
        leftlamp_turnedon=1;
    }
    else
    {
        leftlamp_turnedon=0;
    }
}

void XH_Test::on_RightLightControl_toggled(bool checked)
{
    if(checked==1)
    {
        rightlamp_turnedon=1;
    }
    else
    {
        rightlamp_turnedon=0;
    }
}

void XH_Test::on_FogControl_toggled(bool checked)
{
    if(checked==1)
    {
        Fog_Lamp_turnedon=1;
    }
    else
    {
        Fog_Lamp_turnedon=0;
    }
}

void XH_Test::on_HazardControl_toggled(bool checked)
{
    if(checked==1)
    {
        Hazard_Light_turnedon=1;
    }
    else
    {
        Hazard_Light_turnedon=0;
    }
}



void XH_Test::on_BrakeLightControl_toggled(bool checked)
{
    if(checked==1)
    {
        Stop_Lamp_turnedon=1;
    }
    else
    {
        Stop_Lamp_turnedon=0;
    }
}


void XH_Test::on_MiddleDoorControl_toggled(bool checked)
{
    if(checked==1)
    {
        Door_Control=1;
    }
    else
    {
        Door_Control=0;
    }
}

QFile file("DataSave.txt");
void XH_Test::on_DataSave_clicked()
{
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,"Tips","DatasaveBegin",QMessageBox::Yes);
        QTextStream out(&file);
        out<<QString("AccurateSpeed")<<' '<<QString("EPS_ActualAngle")<<QString("fDeFLRWheelAverAngle")<<' '<<QString("brake")<<' '<<QString("GearPositions")<<'\n';
        m_nTimerId1 = startTimer(50);
    }
}

void XH_Test::timerEvent(QTimerEvent*event)
{

    if(event->timerId()==m_nTimerId1)
    {
         //file.open(QIODevice::WriteOnly  | QIODevice::Text);
         QTextStream out(&file);
         out<<AccurateSpeed<<'\t'<<EPS_ActualAngle<<'\t'<<fDeFLRWheelAverAngle<<'\t'<<brake<<'\t'<<GearPosition<<'\n';
    }
}

void XH_Test::on_DataSaveFinished_clicked()
{
    file.close();
    QMessageBox::warning(this,"Tips","DatasaveFinished",QMessageBox::Yes);
}


void XH_Test::on_Paint_clicked()
{
    QTimer *timer1 = new QTimer(this);
    timer1->start(50);
    QObject::connect(timer1, SIGNAL(timeout()), this, SLOT(Graph_Show()));

}

void XH_Test::Graph_Show()
{

//    QTime t;
//    t=QTime::currentTime();
//    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
//    n=qrand()%50;
    Graph_Show(ui->paint);
}

void XH_Test::Graph_Show(QCustomPlot *CustomPlot)
{
    QVector<double> t(10);
    QVector<double> speed(10);

    for(int i=0; i<9; i++)
    {
        num[i]=num[i+1];
    }
    num[9]=Speed;
    for(int i=0;i<10;i++)
    {
        t[i] = i;
        speed[i] =num[i];
    }
    CustomPlot->addGraph();
    CustomPlot->graph(0)->setPen(QPen(Qt::red));
    CustomPlot->graph(0)->setData(t,speed);
    CustomPlot->xAxis->setLabel("t");
    CustomPlot->yAxis->setLabel("Speed");
    CustomPlot->xAxis->setRange(0,10);
    CustomPlot->yAxis->setRange(0,40);
    CustomPlot->replot();
}

void XH_Test::on_HornControl_clicked(bool checked)
{
    if(checked==1)
    {
        Horn_turnedon=1;
    }
    else
    {
        Horn_turnedon=0;
    }
}

void XH_Test::on_FrontDoor_clicked(bool checked)
{
    if(checked==1)
    {
        FrontDoor_turnedon=1;
    }
    else
    {
        FrontDoor_turnedon=0;
    }
}
