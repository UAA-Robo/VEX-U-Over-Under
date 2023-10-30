#include "RaspiComm.hpp"
#include <iostream>
#include <stdlib.h>

void RaspiComm::sendMsg(std::string msg)
{
    *msg_to_send = msg;
}

std::string RaspiComm::readMsg()
{
    return *last_msg_recieved;
}

void RaspiComm::stopThreads()
{
    *keepRunning = false;
}

void RaspiComm::startSendRecieveThread()
{
    
    int rf = open(PORT,O_RDWR);
    char msg_recieved[MSG_SIZE];
    
    while(*keepRunning == true)
    {
        int i = read(rf, msg_recieved, MSG_SIZE);


        std::string  msg_recieved_string = std::string(msg_recieved);
        if(msg_recieved_string.substr(0,3) == "ACK")
        {
            *last_recieve_ack = atoi(msg_recieved_string.substr(3,8).c_str());
        }

        if(msg_recieved_string.substr(0,3) == "MSG")
        {
            int i = write(rf, msg_recieved_string.substr(3,8).c_str(), MSG_SIZE);
        }

        




        



        if(*msg_to_send != "")
        {
            if(msg_to_send->length() <= MSG_SIZE)
            {
                char * msg = new char[msg_to_send->length()];
                std::copy(msg_to_send->begin(), msg_to_send->end(), msg);

            }
            

            
        }
    }
    return;
}


bool RaspiComm::handShake()
{
    return false; 
}



