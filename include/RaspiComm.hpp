#pragma once
#include "vex.h"
#include <string>
#include <vector>
#include <vex_thread.h>


class RaspiComm
{
    public:
        void sendMsg(std::string msg);
        std::string readMsg();
        bool messageRecieved();
        void stopThreads();
        RaspiComm();
        static int startSendRecieveThread(void * param);
    private:
        const int REFRESHRATE = 1000; // in msec


        char * PORT;
        int MSG_SIZE = 100;
        std::string msg_to_send;
        std::string last_msg_recieved;
        //static void startRecieveThread();
        
        std::string last_message; 
        int last_send_ack;
        int last_recieve_ack;
        bool mesg_recieved = true;
        bool keepRunning;  
        bool handShake();
        const char * createACK(int ack);
        const char * createMessage(std::string msg, int ack);

    
        
        
        



};

