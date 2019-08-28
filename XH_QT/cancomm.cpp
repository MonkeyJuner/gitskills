#include"cancomm.h"


void* send_func(void *param);
void* send_func1(void *param);
void* receive_func(void *param);
void* send_func2(void *param);
void* send_func3(void *param);
void* send_func4(void *param);
void* send_func5(void *param);
UINT AutoModeFlag=0;
UINT AutoModeCount=0;

//*******************Receive*******************//
//**********VCU***********//
unsigned char VCU_WorkMode;
unsigned char GearPosition;
int GasPedalPosition;
int BrakePedalPosition;
unsigned char EPB_State;          //LJZ0312 add
double Speed;
//double GasPedalPercent;//20180603
//double BrakePedalPercent;//20180603
unsigned char MiddleDoorState;
unsigned char ButtonState;
unsigned char RotationSpdDir;
unsigned char VCU_State;
unsigned char FrontDoor;
unsigned char Horn;
unsigned char VCU_Accelaration;
//**********VCU2**********//
unsigned char Bfog_Lamp;
unsigned char Small_Lamp;
unsigned char High_Beam;
unsigned char Low_Beam;
unsigned char Left_Lamp;
unsigned char Right_Lamp;
unsigned char Fog_Lamp;
unsigned char Hazard_Lamp;
unsigned char Stop_Lamp;
//**********EPS***********//
unsigned char EPS_State;
int EPS_ActualAngle;
double EPS_ActualTorque;
unsigned char EPS_ControlState;
//**********EBS***********//
double Flwheelspeed;
double Frwheelspeed;
double Blwheelspeed;
double Brwheelspeed;
double AccurateSpeed;
//*******************Receive*******************//
//*******************Send*******************//
unsigned char throttle;
double fDeFLRWheelAverAngle;
double brake;
unsigned char HighBeam_turnedon;
unsigned char LowBeam_turnedon;
unsigned char leftlamp_turnedon;
unsigned char rightlamp_turnedon;
unsigned char shift;                        //档位，
bool lateralctrl_enabled;                       //横向控制有效
bool longitutdectrl_enabled;        //纵向控制有效
unsigned char Door_Control;
unsigned char Bfog_Lamp_turnedon;
unsigned char Small_Lamp_turnedon;
unsigned char Fog_Lamp_turnedon;
unsigned char Hazard_Light_turnedon;
unsigned char Stop_Lamp_turnedon;

unsigned char Horn_turnedon;
unsigned char FrontDoor_turnedon;

int MessageCounter=0;   //GHJ0914 EBS计数器
DWORD dwDeviceHandle;
typedef struct {
    int Run;
    DWORD ch;
} thread_arg_t;
CCanComm::CCanComm()
{

}

CCanComm::~CCanComm()
{
//	data_backup.close();
}

bool CCanComm::Init()
{
    CAN_InitConfig config;
    thread_arg_t rcv_thread_arg0;
    int RCV_ret,VCU_ret,EPS_ret,EBS_ret,EPB_ret;//20180608
    if ((dwDeviceHandle = CAN_DeviceOpen(ACUSB_132B, 0, 0)) == 0)
    //CAN_DeviceOpen是打开设备的函数，函数的第一个参数是设备类型，第二个是设备端口号，第三个是设备信息可为空
    //返回值0打开失败，否则返回设备句柄
    {
        printf("open deivce error\n");
    }
    else
        printf("open deivce success\n");
    config.dwAccCode = 0; //验收码
    config.dwAccMask = 0xffffffff; //屏蔽码
    config.nFilter = 0;     // 滤波方式(0表示未设置滤波功能,1表示双滤波,2表示单滤波)
    config.bMode = 0;       // 工作模式(0表示正常模式,1表示只听模式)
    config.nBtrType = 1;    // 位定时参数模式(1表示SJA1000,0表示LPC21XX)
    config.dwBtr[0] = 0x01; // BTR0   0014 -1M 0016-800K 001C-500K 011C-250K 031C-12K 041C-100K 091C-50K 181C-20K 311C-10K BFFF-5K
    config.dwBtr[1] = 0x1C; // BTR1    / /guan 修改为福田项目中的250kbs
    config.dwBtr[2] = 0;
    config.dwBtr[3] = 0;

    rcv_thread_arg0.Run=1;
    rcv_thread_arg0.ch=0;

    if (CAN_ChannelStart(dwDeviceHandle, 0, &config) != CAN_RESULT_OK)
    //CAN_ChannelStart用于初始化并启动指定的通道，函数的第一个参数是设备的句柄，第二参数是设备的通道号，第三个是初始化的结构
    {
        printf("Start CAN 0 error\n");
    }
    else
        {
        printf("Start CAN 0 success!\n");
        }

    RCV_ret = pthread_create(&rcv_threadid0, NULL, receive_func, &rcv_thread_arg0);
    //pthread_create创建线程函数，函数的第一个参数是线程的标识符，第二个是线程的属性的设置，第三个是线程的起始地址，第四个是传递给线程函数的参数
    ////返回值 成功0，出错-1
    VCU_ret = pthread_create(&snd_threadid0, NULL, send_func, (void *) 0);
    EPS_ret = pthread_create(&snd_threadid1, NULL, send_func1, (void *) 0);
    EBS_ret = pthread_create(&snd_threadid1, NULL, send_func2, (void *) 0);
    EPB_ret = pthread_create(&snd_threadid1, NULL, send_func3, (void *) 0);

    if(RCV_ret==0)
       printf("create RCV_thread\n");
    if(VCU_ret==0)
        printf("create VCU_thread\n");
    if(EPS_ret==0)
        printf("create EPS_thread\n");
    if(EBS_ret==0)
       printf("create EBS_thread\n");
//   pthread_join(rcv_threadid0, NULL);
//   pthread_join(snd_threadid0, NULL);
    //pthread_join是用来等一个线程结束，保证线程同步操作的函数。
    //函数的第一个参数是等待结束的线程的ID，第二个参数是用户自己定义的指针用来储存等待线程的返回值。
    //返回值 成功0，出错返回错误号。
    //	CAN_DeviceClose(dwDeviceHandle);
    //该函数用于关闭设备
    //函数的参数为设备的句柄
    //返回值  成功CAN_RESULT_OK，失败CAN_RESULT_ERROR(这两个在函数头已经被预定义了)
    return true;
}

bool CCanComm::shutdown()
{
    CAN_ChannelStop(dwDeviceHandle, 0);
    //该函数用来停止指定的CAN通道
    //函数的第一个参数是设备的句柄，第二个参数设备的通道号
    //返回值  成功CAN_RESULT_OK，失败CAN_RESULT_ERROR(这两个在函数头已经被预定义了)
    int a=CAN_DeviceClose(dwDeviceHandle);
    if (a== CAN_RESULT_ERROR)
    //CAN_DeviceClose是关闭设备的函数，函数参数是设备句柄
    //返回值0打开失败
        printf("close deivce error\n");
    else
        printf("close deivce success\n");
    return true;
}

void* receive_func(void *param)  //接收线程的处理函数
{
    int reclen = 0;
    thread_arg_t *thread_arg = (thread_arg_t *) param;
    int ind = thread_arg->ch;
    CAN_DataFrame rec[60];
    int i;
    CAN_ErrorInformation err;
    printf("receive thread running....%d\n", ind);
    while (1)
    {
        reclen = CAN_ChannelReceive(dwDeviceHandle, ind, rec,50, 200);
        // CAN_ChannelReceive是从指定CAN通道读取数据
        //函数的第一个参数是设备的句柄，第二个参数是设备通道号，第三个是接受数据的首指针，第四是接收数据的数据长度，第五是等待超时时间，单位毫秒
        //返回值是实际读到的帧数，如果返回值是0x0,则表示有错误，错误可用CAN_GetErrorinfo获取错误码
        if (reclen > 0)
        {
             for(i=0;i<reclen;i++)
             {
                 if(rec[i].uID == 0x0C18E5D0)
                     {

                         int GearPositiontemp=(rec[i].arryData[3])&0x03;

                             switch(GearPositiontemp)
                                  {

                                       case 0x00:
                                           {

                                                GearPosition=0x00;          // N
                                                break;
                                           }


                                       case 0x01:
                                           {
                                                GearPosition=0x03;          // D
                                                break;
                                           }
                                       case 0x02:
                                           {
                                                GearPosition=0x04;			// R
                                                break;
                                           }
                                       case 0x03:
                                           {
                                                GearPosition=0x02;			// S
                                                break;
                                           }


                                  }
                     }
                 if (rec[i].uID == 0X0C18E5D0)
                 {
                                        //VehicleState=(rec[i].arryData[0])&0x07;//LJZ0308  00无信号，01启动，02就绪，03熄火;
                                        VCU_WorkMode=rec[i].arryData[0]&0x03; // VCU工作模式 1手动模式 2自动模式应答 3进入自动模式
                                        GasPedalPosition=(unsigned int)rec[i].arryData[1];//20180603
                                        BrakePedalPosition=(unsigned int)rec[i].arryData[2];//20180603
                                         AccurateSpeed=((unsigned int)rec[i].arryData[6]+(unsigned int)rec[i].arryData[7]*256)/256;
                                         ButtonState=(rec[i].arryData[0]>>4)&0x01;
                 }
 //               if (rec[i].uID == 0x0CFE0CAE)
 //               {
 //                   VCU_WorkMode=rec[i].arryData[0]&0x03; //GHJ20170901 VCU工作模式 1手动模式 2自动模式应答 3进入自动模式

//                    int GearPositiontemp=(rec[i].arryData[0]>>3)&0x07; //GHJ20170901 档位状态 001 N档 010 D档 011 R档
//                    switch(GearPositiontemp)
//                    {
//                        case 0x01:
//                        {
//                            GearPosition=0x03;            //GHJ20170901 N档
//                            break;
//                        }
//                        case 0x02:
//                        {
//                            GearPosition=0x04;			//GHJ20170901 D档
//                            break;
//                        }
//                        case 0x03:
//                        {
//                            GearPosition=0x02;			//GHJ20170901 R档
//                            break;
//                        }
//                    }

//                    Speed=(double)rec[i].arryData[1];  //GHJ20170901 车速

//                    VCU_State=(rec[i].arryData[5]>>2)&0x03;    //GHJ20170901
//                    //00整车正在运行，不能切换模式 01可以进入无人驾驶模式 10整车控制器请求停车 11整车控制器请求退出无人驾驶模式

//                    //VehicleState=(rec[i].arryData[5]>>4)&0x03;  //GHJ20170901
//                    //00on档 01st档（表示已拧过st）10ready（整车已上高压，当前状态下才允许行车）11整车正在或准备充电或充电已完成（表示有线充电、禁止行车,充电枪已经插上）

//                    //FailureLevel=(rec[i].arryData[5]>>6)&0x03;  //GHJ20170901
//                    //00正常 01一级故障（提示）10二级故障（保护、降功率）11三级故障（严重故障，需要停车）

//                    VCU_Accelaration=rec[i].arryData[6]; //GHJ20170901 电子油门反馈
 //               }

//                if(rec[i].uID == 0x0CFC00D0)
//                {
//                   AccurateSpeed=(unsigned int)rec[i].arryData[5]*0.5; //LJZ0302 实际车速
//                }
//                if(rec[i].uID == 0x0CFB00D0)
//                {
//                   GasPedalPosition=(rec[i].arryData[0]>>4)&0x01;//LJZ0302 油门踏板状态
//                   BrakePedalPosition=(rec[i].arryData[0]>>3)&0x01;//LJZ0302 制动踏板状态
//                }



                if(rec[i].uID == 0x0C19E5D0)
                  {
                    // Bfog_Lamp=(rec[i].arryData[0]>>3)&0x01;   //GHJ20170902 后雾灯使能 0关闭 1开启

                     //Small_Lamp=(rec[i].arryData[2]>>5)&0x01;  //GHJ20170902 小灯使能 0关闭 1开启
                     Horn=(rec[i].arryData[0]>>6)&0x01;  //20180603
                     EPB_State=(rec[i].arryData[2])&0x01;

                     High_Beam=(rec[i].arryData[0]>>3)&0x01;   //GHJ20170902 远光灯使能 0关闭 1开启


                     Low_Beam=(rec[i].arryData[0]>>4)&0x01;    //GHJ20170902 近光灯使能 0关闭 1开启

                     Left_Lamp=(rec[i].arryData[0])&0x01;   //GHJ20170902 左转灯使能 0关闭 1开启

                     Right_Lamp=(rec[i].arryData[0]>>1)&0x01;  //GHJ20170902 右转灯使能 0关闭 1开启

                    // Fog_Lamp=(rec[i].arryData[0]>>2)&0x01;    //GHJ20170902 前雾灯使能 0关闭 1开启

                     Stop_Lamp=(rec[i].arryData[0]>>2)&0x01;   //GHJ20170902 刹车灯使能 0关闭 1开启
                    // ButtonState=(rec[i].arryData[1]>>5)&0x01;//LJZ0302自动驾驶按钮 0关 1开
                     MiddleDoorState=(rec[i].arryData[1]>>1)&0x01;
                     FrontDoor=(rec[i].arryData[1])&0x01;//LJZ0302 前门状态
                  }

                if(rec[i].uID == 0x0C02A0A2)
                {
                    EPS_State=(rec[i].arryData[6])&0x0F; //EPS工作状态 0x01手动 0x04自动

                    EPS_ActualAngle=((unsigned int)rec[i].arryData[0]+rec[i].arryData[1]*256)*0.1-1575; //LJZ0319 EPS实际转角  左打为正，右打为负

                    EPS_ActualTorque=(rec[i].arryData[5]-128)*0.07;  //LJZ 0319

                }



             }
            CAN_ClearReceiveBuffer(dwDeviceHandle, ind);
            //该函数用来清空底层接口库缓冲区数据gg
            //函数的第一个参数是设备的句柄，第二个参数是设备通道号
            //返回值  CAN_RESULT_OK表示成功， CAN_RESULT_ERROR表示失败
        }
        else
        {
            if (CAN_GetErrorInfo(dwDeviceHandle, ind, &err) == CAN_RESULT_OK)
            //该函数用于获取最后一次错误信息
            //函数的第一个参数是设备的句柄，第二个参数是设备的通道号，第三个参数是用来存数错误信息的PCAN_ErrorInformation结构体指针
            //返回值  CAN_RESULT_OK表示获取成功， CAN_RESULT_ERROR表示失败
            {
                printf("CAN error");
            }
        }
}
    return 0;//退出线程
}

void* send_func(void *param)   //发送线程的处理函数
{
    while (1)
    {
        int ch = (long) param;
        //CAN_DataFrame send[SEND_FRAMES];
        CAN_DataFrame send1;//GHJ20170829 XH[襄旅海梁项目]发送至VCU的一帧数据
        send1.uID = 0x0C1AD0E5;  // GHJ20170829修改为XH项目的发送至VCUID
        send1.nSendType = 0;  // 0-正常发送;1-单次发送;2-自发自收;3-单次自发自收
        send1.bRemoteFlag = 0;  // 0-数据帧；1-远程帧
        send1.bExternFlag = 1;  // 0-标准帧；1-扩展帧 GHJ20170829根据XH项目修改为扩展帧
        send1.nDataLen = 8;     // DLC

        CAN_DataFrame send2;//GHJ20170829 XH[襄旅海梁项目]发送至VCU的二帧数据
        send2.uID = 0x0C1BD0E5;  // GHJ20170829修改为XH项目的发送至VCUID
        send2.nSendType = 0;  // 0-正常发送;1-单次发送;2-自发自收;3-单次自发自收
        send2.bRemoteFlag = 0;  // 0-数据帧；1-远程帧
        send2.bExternFlag = 1;  // 0-标准帧；1-扩展帧 GHJ20170829根据XH项目修改为扩展帧
        send2.nDataLen = 8;     // DLC


       // double braketemp=brake/1000;   //LJZ0305 制动分辨率0.5
        //********send1********//

        if(AutoModeFlag==1)
        {

            if(longitutdectrl_enabled==1)
            {
                if(VCU_WorkMode==0x01)
                {
                    send1.arryData[0]=0x02;
                }
                else if(VCU_WorkMode==0x02)
                {
                    send1.arryData[0]=0x03;
                }
                else if(VCU_WorkMode==0x03)
                {
                    send1.arryData[0]=0x03;
                    //send2.arryData[2]&=0x00;    //LJZ0313 门控使能为1

                }
                else
                {
                    send1.arryData[0]=0x01;
                  //  send1.arryData[4]&=0xDF;   //LJZ0313 门控使能为0
                    //AutoModeFlag=0;
                    longitutdectrl_enabled=0;

                }

//                if(braketemp>=0&&braketemp<100)//LJZ0305 科密制动
//                {

//                    send1.arryData[2]=(int)braketemp;

//                    //send1.arryData[1] = braketemp;
//                    Stop_Lamp_turnedon=1;
//                }
//                else
//                {
//                    Stop_Lamp_turnedon=0;
//                }


            }
            else
            {
                send1.arryData[3]=0x01;
                //send1.arryData[4]&=0xDF;    // 门控使能为0
                //AutoModeFlag=0;
                longitutdectrl_enabled=0;

            }
        }
        else
        {
            send1.arryData[3]=0x01;
           // send1.arryData[4]&=0xDF;        // 门控使能为0
            //longitutdectrl_enabled=0;
            AutoModeFlag=0;

        }






        send1.arryData[1] = throttle;	//修改为XH项目 设置油门开度 VCU_ACCELERATION

        UINT temp =shift;              //修改为XH项目 设置档位信息  VCU_Shift
        if (temp==0)				    // N档
        {
            send1.arryData[3]=0x00;
        }
        if (temp==0x01)					// D档
        {
            send1.arryData[3]=0x01;
        }
        if (temp==0x02)					// R档
        {
            send1.arryData[3]=0x02;
        }

        temp=Door_Control;
        if(temp==1)
        {
            send2.arryData[1]|=0x03;
        }
        else
        {
            send2.arryData[1]&=0xFC;
        }

        //********send1********//

        //********send2********//
        temp=Horn_turnedon;
        if(temp==1)
          send2.arryData[0]|=0x40;
        else
            send2.arryData[0]&=0xBF;
//        temp=Bfog_Lamp_turnedon;            // 设置后雾灯
//        if(temp==1)
//            send2.arryData[5]|=0x80;
//        else
//            send2.arryData[5]&=0x7F;

//        temp=Small_Lamp_turnedon; 			// 设置小灯
//        if(temp==1)
//            send2.arryData[5]|=0x20;
//        else
//            send2.arryData[5]&=0xDF;

        temp=HighBeam_turnedon;            // 设置远光灯
        if(temp==1)
            send2.arryData[0]|=0x08;
        else
            send2.arryData[0]&=0xF7;

        temp=LowBeam_turnedon; 			   // 设置近光灯
        if(temp==1)
            send2.arryData[0]|=0x10;
        else
            send2.arryData[0]&=0xEF;

        temp =leftlamp_turnedon;		  // 设置左转灯
        if (temp==1)
            send2.arryData[0]|=0x01;
        else
            send2.arryData[0]&=0xFE;

        temp = rightlamp_turnedon;        // 设置右转灯
        if (temp==1)
            send2.arryData[0]|=0x02;
        else
            send2.arryData[0]&=0xFD;

//        temp=Fog_Lamp_turnedon;          // 设置前雾灯
//        if(temp==1)
//            send2.arryData[5]|=0x01;
//        else
//            send2.arryData[5]&=0xFE;

//        temp=Hazard_Light_turnedon;      // 设置双闪灯
//        if(temp==1)
//            send2.arryData[6]|=0x80;
//        else
//            send2.arryData[6]&=0x7F;

        temp=Stop_Lamp_turnedon;	     // 设置刹车灯
        if(temp==1)
            send2.arryData[0]|=0x04;
        else
            send2.arryData[0]&=0xFB;

        //********send2********//


        //*************************特殊情况的处理****************************//


//        if(EPS_State==0x04)
//        {
//            send3.arryData[2]|=0x05;
//        }
//        if(EPS_State==0x05)
//        {
//            send3.arryData[2]|=0x01;
//        }



//        if(MiddleDoorState==0x01)
//        {
//            send1.arryData[4]=0;		//GHJ20170904中门打开，油门开度置为0
//        }


//        if(BrakePedalPosition!=0)		//GHJ20170904制动踏板踩下退出无人驾驶模式
//        {
//            send1.arryData[0]=0x01;     // LJZ20180316 VCU退出自动驾驶
//            //longitutdectrl_enabled=0;
//            AutoModeFlag=0;
//        }


        //*************************特殊情况的处理****************************//

        int VCU_Checksum=0;
        for(int i=0;i<7;i++)
        {
            VCU_Checksum+=send1.arryData[i];
        }
  //      send1.arryData[7]=VCU_Checksum;

        int Light_checksum=send2.arryData[0];
        for(int i=1;i<7;i++)
        {
            Light_checksum^=send2.arryData[i];
        }
  //      send2.arryData[7]=Light_checksum;

       if(ButtonState==0)
        {
           send1.arryData[0]=0x01;

            AutoModeFlag=0;
            if(AutoModeCount<50)
                AutoModeCount++;
            else
                AutoModeCount=50;
        }
        if(ButtonState==1&&AutoModeCount>10)
        {
            AutoModeFlag=1;
            AutoModeCount=0;
        }  //GHJ20170904按钮必须关闭再开启才能再次进入自动驾驶模式

        CAN_ChannelSend(dwDeviceHandle, ch, &send1,1);
        CAN_ChannelSend(dwDeviceHandle, ch, &send2,1);
//		该函数用来指定CAN通道发送的数据
//		函数的第一个参数是设备的句柄，第二个参数是函数的通道号。第三个参数是发送的数据的首指针，第四个参数是发送数据帧数组的长度
//		返回值为发送成功的帧数
    usleep(50000);//挂起线程一段时间，满足发送周期，参数单位毫秒
}
    CAN_DeviceClose(dwDeviceHandle);
    return 0;
}

void* send_func1(void *param)   //EPS发送线程的处理函数
{
    while (1)
    {
        int ch = (long) param;
        CAN_DataFrame send3;//GHJ20170901 XH[襄旅海梁项目]发送至EPS的一帧数据
        send3.uID = 0x0C02A2A0;  // GHJ20170901修改为XH项目的发送至EPSID
        send3.nSendType = 0;  // 0-正常发送;1-单次发送;2-自发自收;3-单次自发自收
        send3.bRemoteFlag = 0;  // 0-数据帧；1-远程帧
        send3.bExternFlag = 1;  // 0-标准帧；1-扩展帧 GHJ20170829根据XH项目修改为扩展帧
        send3.nDataLen = 8;     // DLC

        CAN_DataFrame send4;//GHJ20170901 XH[襄旅海梁项目]发送至EPS的一帧数据
        send4.uID = 0x18FEF117;  // GHJ20170901修改为XH项目的发送至EPSID
        send4.nSendType = 0;  // 0-正常发送;1-单次发送;2-自发自收;3-单次自发自收
        send4.bRemoteFlag = 0;  // 0-数据帧；1-远程帧
        send4.bExternFlag = 1;  // 0-标准帧；1-扩展帧 GHJ20170829根据XH项目修改为扩展帧
        send4.nDataLen = 8;     // DLC





        if(AutoModeFlag==1)
        {
            if(lateralctrl_enabled==1)
            {

                send3.arryData[2]=0x31;//LJZ0305
            }
            else
            {
                send3.arryData[2]=0x34;	//LJZ0309 设置EPS控制模式

            }

        }
        else
        {
            send3.arryData[2]=0x74;		//GHJ20170901 设置EPS控制模式

        }

        int StrAnlgetemp=fDeFLRWheelAverAngle; //GHJ20170901 设置EPS期望转角 注意最后转化为前轮偏角

//        if(StrAnlgetemp<700&&StrAnlgetemp>=0)
//        {

        StrAnlgetemp=StrAnlgetemp/0.1+15750;   //LJZ0306 分辨率为0.1   20180320 add
        send3.arryData[0]=StrAnlgetemp&0x00FF;
        send3.arryData[1]=(StrAnlgetemp&0xFF00)>>8;
//       }
//        else
//        {
//          StrAnlgetemp=22750;
//          send3.arryData[0]=StrAnlgetemp&0x00FF;
//          send3.arryData[1]=(StrAnlgetemp&0xFF00)>>8;
//        }

//        if(StrAnlgetemp>-700&&StrAnlgetemp<=0)
//        {

//        StrAnlgetemp=StrAnlgetemp/0.1+15750;   //LJZ0306 分辨率为0.1
//        send3.arryData[0]=StrAnlgetemp&0x00FF;
//        send3.arryData[1]=(StrAnlgetemp&0xFF00)>>8;
//        }
//        else
//        {
//          StrAnlgetemp=8750;
//          send3.arryData[0]=StrAnlgetemp&0x00FF;
//          send3.arryData[1]=(StrAnlgetemp&0xFF00)>>8;
//        }
        if(ButtonState==0)
         {
            send3.arryData[2]=0x35;//LJZ0329
             AutoModeFlag=0;
             if(AutoModeCount<50)
                 AutoModeCount++;
             else
                 AutoModeCount=50;
         }
         if(ButtonState==1&&AutoModeCount>10)
         {
             AutoModeFlag=1;
             AutoModeCount=0;
         }
        send3.arryData[6]=0x00;//LJZ0313 add
        send3.arryData[7]=0x00;//LJZ0313 add
        send3.arryData[4]=0x0C;  //LJZ0313 add

        send3.arryData[3]=0x80;//LJZ0313

        int Speedtemp=AccurateSpeed/256;
        send4.arryData[0]=0XFF;
        send4.arryData[1]=Speedtemp&0X00FF;
        send4.arryData[2]=(Speedtemp&0XFF00)>>8;
        send4.arryData[3]=0XFF;
        send4.arryData[4]=0XFF;
        send4.arryData[5]=0XFF;
        send4.arryData[6]=0XFF;
        send4.arryData[7]=0XFF;
        send3.arryData[5]=0X80;

        //********send3********//

//        if(BrakePedalPosition!=1)		//GHJ20170904制动踏板踩下退出无人驾驶模式
//        {
//            send3.arryData[0]=0x01;     //EPS退出自动驾驶
//            AutoModeFlag=0;
//        }

 //       if(EPS_ActualTorque>3||EPS_ActualTorque<-3)
//        {
//              send3.arryData[2]=0x74;
//              send3.arryData[2]=0x75;
 //             AutoModeFlag=0;
 //             lateralctrl_enabled=0;     //LJZ0319

//        }

        int EPS_checksum=0;
        for(int i=0;i<7;i++)
        {
            EPS_checksum+=send3.arryData[i];
        }
        send3.arryData[7]=EPS_checksum;
        CAN_ChannelSend(dwDeviceHandle, ch, &send3,1);
        CAN_ChannelSend(dwDeviceHandle, ch, &send4,1);
        usleep(10000);//挂起线程一段时间，满足发送周期，参数单位毫秒
    }
    CAN_DeviceClose(dwDeviceHandle);
    return 0;
}


/*.............HAIGE...............*/
void* send_func2(void *param)   //EBS发送线程的处理函数
{
    while (1)
    {
        int ch = (long) param;

        CAN_DataFrame send6;//GHJ20170901 XH[襄旅海梁项目]发送至EPS的一帧数据
        send6.uID = 0x0C04CB0A;  // GHJ20170901修改为XH项目的发送至EPSID
        send6.nSendType = 0;  // 0-正常发送;1-单次发送;2-自发自收;3-单次自发自收
        send6.bRemoteFlag = 0;  // 0-数据帧；1-远程帧
        send6.bExternFlag = 1;  // 0-标准帧；1-扩展帧 GHJ20170829根据XH项目修改为扩展帧
        send6.nDataLen = 8;     // DLC

        CAN_DataFrame send5;//GHJ20170901 XH[襄旅海梁项目]发送至EPS的一帧数据
        send5.uID = 0x0C04DB0A;  // GHJ20170901修改为XH项目的发送至EPSID
        send5.nSendType = 0;  // 0-正常发送;1-单次发送;2-自发自收;3-单次自发自收
        send5.bRemoteFlag = 0;  // 0-数据帧；1-远程帧
        send5.bExternFlag = 1;  // 0-标准帧；1-扩展帧 GHJ20170829根据XH项目修改为扩展帧
        send5.nDataLen = 8;     // DLC

//        CAN_DataFrame send7;//GHJ20170901 XH[襄旅海梁项目]发送至EPS的一帧数据
//        send7.uID = 0x18FE12BD;  // GHJ20170901修改为XH项目的发送至EPSID
//        send7.nSendType = 0;  // 0-正常发送;1-单次发送;2-自发自收;3-单次自发自收
//        send7.bRemoteFlag = 0;  // 0-数据帧；1-远程帧
//        send7.bExternFlag = 1;  // 0-标准帧；1-扩展帧 GHJ20170829根据XH项目修改为扩展帧
//        send7.nDataLen = 8;     // DLC

//        //********send3********//
//        if(AccurateSpeed==0)         //LJZ20180614
//        {
//            if(VCU_WorkMode==1)
//            {
//                  if(ButtonState==0)
//                  {
//                      printf("33333333\n");
//                      send7.arryData[0]=0X01;
//                  }
//            }

//        }
//        else
//        {
//            send7.arryData[0]=0X02;
//        }





        double braketemp=brake;
        send6.arryData[0]=0X0A;
        send6.arryData[1]=0X0C;
        send6.arryData[2]=0XEB;
        send6.arryData[3]=0X4F;
        send6.arryData[4]=0X09;
        send6.arryData[5]=0X0C;
        send5.arryData[0]=0X0A;
        send5.arryData[1]=0X0C;
        send5.arryData[2]=0XEB;
        send5.arryData[3]=0X4F;
        send5.arryData[4]=0X09;
        send5.arryData[5]=0X0C;

        if(braketemp>=0&&braketemp<1000)
                        {

                            send6.arryData[6]=(int)braketemp&0x00FF;
                            send6.arryData[7]=((int)braketemp&0xFF00)>>8;
                            send5.arryData[6]=(int)braketemp&0x00FF;
                            send5.arryData[7]=((int)braketemp&0xFF00)>>8;

                            Stop_Lamp_turnedon=1;
                        }
                        else
                        {
                            Stop_Lamp_turnedon=0;
                        }



        //********send3********//

//        if(BrakePedalPosition!=1)		//GHJ20170904制动踏板踩下退出无人驾驶模式
//        {
//            send3.arryData[0]=0x01;     //EPS退出自动驾驶
//            AutoModeFlag=0;
//        }

 //       if(EPS_ActualTorque>3||EPS_ActualTorque<-3)
//        {
//              send3.arryData[2]=0x74;
//              send3.arryData[2]=0x75;
 //             AutoModeFlag=0;
 //             lateralctrl_enabled=0;     //LJZ0319

//        }


        CAN_ChannelSend(dwDeviceHandle, ch, &send5,1);
        CAN_ChannelSend(dwDeviceHandle, ch, &send6,1);
        usleep(10000);//挂起线程一段时间，满足发送周期，参数单位毫秒
    }
    CAN_DeviceClose(dwDeviceHandle);
    return 0;
}


void* send_func3(void *param)   //EBS发送线程的处理函数
{
    while (1)
    {
        int ch = (long) param;

        CAN_DataFrame send7;//GHJ20170901 XH[襄旅海梁项目]发送至EPS的一帧数据
        send7.uID = 0x18FE12BD;  // GHJ20170901修改为XH项目的发送至EPSID
        send7.nSendType = 0;  // 0-正常发送;1-单次发送;2-自发自收;3-单次自发自收
        send7.bRemoteFlag = 0;  // 0-数据帧；1-远程帧
        send7.bExternFlag = 1;  // 0-标准帧；1-扩展帧 GHJ20170829根据XH项目修改为扩展帧
        send7.nDataLen = 8;     // DLC

        //********send3********//
        if(AccurateSpeed==0)         //LJZ20180614
        {
            if(VCU_WorkMode==3)
            {
                  if(throttle==0)
                  {
                      if(ButtonState==1)
                      {
                          send7.arryData[0]=0X01;
                      }
                      else
                      {
                          send7.arryData[0]=0X02;
                      }
                  }
                  else
                  {
                      send7.arryData[0]=0X02;
                  }
            }
            else
            {
                send7.arryData[0]=0X02;
            }

        }
        else
        {
            send7.arryData[0]=0X02;
        }


        CAN_ChannelSend(dwDeviceHandle, ch, &send7,1);

        usleep(10000);//挂起线程一段时间，满足发送周期，参数单位毫秒
    }
    CAN_DeviceClose(dwDeviceHandle);
    return 0;
}



