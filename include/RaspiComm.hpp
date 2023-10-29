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

    private:
        const int REFRESHRATE = 1000; // in msec
        const std::string PORT = "dev/serial1";

        static std::string *msg_to_send;
        static std::string *last_msg_recieved;
        static void startRecieveThread();
        static void startSendThread();
        std::string last_message; 
        int last_send_ack = 0;
        int last_recieve_ack = 0;
        bool mesg_recieved = true;
        static bool *keepRunning;  


        
        



};

