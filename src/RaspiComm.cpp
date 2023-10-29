#include "RaspiComm.hpp"

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

void RaspiComm::startSendThread()
{
    while(*keepRunning == true)
    {
        if(*msg_to_send != "")
        {
            std::ofstream 
        }
    }
}


