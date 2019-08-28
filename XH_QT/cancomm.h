#ifndef CANCOMM_H
#define CANCOMM_H
#include"icancmd.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
class CCanComm
{
public:
    CCanComm();
    ~CCanComm();
    bool Init();
    bool shutdown();
    //  void Update();
private:
    pthread_t rcv_threadid0;
    pthread_t rcv_threadid1;//20180603
    pthread_t snd_threadid0;
    pthread_t snd_threadid1;
    pthread_t snd_threadid2;
    pthread_t snd_threadid3;//20180605
    pthread_t snd_threadid4;
    pthread_t snd_threadid5;

//    std::fstream data_backup;
};
#endif // CANCOMM_H
