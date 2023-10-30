#include "vex.h"
#include <string>
#include <vector>


class RaspiComm
{
    public:
        void sendMsg(std::string msg);
        std::string readMsg();
        bool messageRecieved();
        void stopThreads();
        RaspiComm(){

            PORT = "dev/serial1";
            *last_send_ack = 0;
            *last_recieve_ack = 0;

        }
    private:
        const int REFRESHRATE = 1000; // in msec


        static const char * PORT;
        static const int MSG_SIZE = 100;
        static std::string *msg_to_send;
        static std::string *last_msg_recieved;
        //static void startRecieveThread();
        static void startSendRecieveThread();
        std::string last_message; 
        static int * last_send_ack;
        static int * last_recieve_ack;
        bool mesg_recieved = true;
        static bool *keepRunning;  
        bool handShake();
        char * createACK(int ack);
        char * createMessage(std::string msg);

        
        



};

