#ifndef XH_TEST_H
#define XH_TEST_H

#include <QWidget>
#include "qcustomplot.h"
namespace Ui {
class XH_Test;
}

class XH_Test : public QWidget
{
    Q_OBJECT

public:
    explicit XH_Test(QWidget *parent = 0);
    ~XH_Test();

    void timerEvent(QTimerEvent *);

    void Graph_Show(QCustomPlot *CustomPlot);

    double num[10];

public slots:
    void Graph_Show();

signals:
    void GasSliderReturn(int);

    void AcclerationSliderReturn(int);

    void StrAngleSliderReturn(int);

private slots:
    void on_CanButton_toggled(bool checked);

    void VCU_WorkMode_SetValue();

    void GearPosition_SetValue();

    void GasPedal_SetValue();

    void BrakePedal_SetValue();

    void Speed_SetValue();

    void MiddleDoor_SetValue();

    void FailureLevel_SetValue();

    void VehicleState_SetValue();

    void VCU_State_SetValue();

    void RotationDir_SetValue();

    void Gas_SetValue();

    void Bfog_SetValue();

    void SmallLight_SetValue();

    void HighBeam_SetValue();

    void LowBeam_SetValue();

    void LeftLight_SetValue();

    void RightLight_SetValue();

    void Fog_SetValue();

    void Hazard_SetValue();

    void BrakeLight_SetValue();

    void EPS_WorkMode_SetValue();

    void StrAngle_SetValue();

    void Torque_SetValue();

    void AngleSensor_SetValue();

//    void EBS_WorkMode_SetValue();

//    void DeAccelaration_SetValue();

    void Button_SetValue();

    void Flwheelspeed_SetValue();

    void Frwheelspeed_SetValue();

    void Blwheelspeed_SetValue();

    void Brwheelspeed_SetValue();

    void AccurateSpeed_SetValue();

    void on_longitutdectrl_enabled_toggled(bool checked);

    void on_lateralctrl_enabled_toggled(bool checked);

    void on_GasEdit_editingFinished();

    void on_GasSlider_valueChanged(int value);

    void on_GasSlider_SetValue(int);

    void on_AcclerationEdit_editingFinished();

    void on_AcclerationSlider_valueChanged(int value);

    void on_AcclerationSlider_SetValue(int);

    void on_StrAngleEdit_editingFinished();

    void on_StrAngleSlider_valueChanged(int value);

    void on_StrAngleSlider_SetValue(int);

    void on_BfogControl_toggled(bool checked);

    void on_SmallLightControl_toggled(bool checked);

    void on_HighBeamControl_toggled(bool checked);

    void on_LowBeamControl_toggled(bool checked);

    void on_LeftLightControl_toggled(bool checked);

    void on_RightLightControl_toggled(bool checked);

    void on_FogControl_toggled(bool checked);

    void on_HazardControl_toggled(bool checked);

    void on_BrakeLightControl_toggled(bool checked);

    void on_MiddleDoorControl_toggled(bool checked);

    void on_DataSave_clicked();

    void on_DataSaveFinished_clicked();

    void on_N_clicked();

    void on_D_clicked();

    void on_R_clicked();

    void on_Paint_clicked();


    void on_HornControl_clicked(bool checked);

    void on_FrontDoor_clicked(bool checked);

private:
    Ui::XH_Test *ui;
    int m_nTimerId1;
};

#endif // XH_TEST_H
