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

    // artifical way to stop sending message every loop
    int loopCounter = 0;
    while(*keepRunning == true)
    {
        int i = read(rf, msg_recieved, MSG_SIZE);


        std::string  msg_recieved_string = std::string(msg_recieved);
        if(msg_recieved_string.substr(0,3) == "ACK")
        {
            
            *last_recieve_ack = atoi(msg_recieved_string.substr(4).c_str());
        }

        if(msg_recieved_string.substr(0,3) == "MSG")
        {
            size_t ack_length = msg_recieved_string.substr(4).find(':');
            
            int ack = atoi(msg_recieved_string.substr(4,ack_length).c_str());
            *last_msg_recieved = msg_recieved_string.substr(4+ack);
            int i = write(rf, msg_recieved_string.substr(4,ack_length).c_str(), MSG_SIZE);
        }
        if(*last_recieve_ack == *last_send_ack)
        {
            *msg_to_send ="";
        }
        
        if(*msg_to_send != "" and loopCounter >= 1000)
        {
            if(msg_to_send->length() <= MSG_SIZE)
            {
                last_send_ack++;
                const char * msg_to_send_char = createMessage(*msg_to_send, *last_send_ack);
                int i = write(rf, msg_to_send_char, MSG_SIZE);
                loopCounter = 0;
                //std::copy(msg_to_send->begin(), msg_to_send->end(), msg);
                
                
            }
            

            
        }
        loopCounter++;
    }
    return;
}


bool RaspiComm::handShake()
{
    return false; 
}


const char * RaspiComm::createACK(int ack)
{
    std::string ack_msg;
    ack_msg.append("ACK:");


    char * ack_char;
    itoa(ack, ack_char, 8);
    
    ack_msg.append(std::string(ack_char));

    const char * msg_char = ack_msg.c_str();
    return msg_char; 
}
const char * RaspiComm::createMessage(std::string msgToCreate, int ack)
{
    std::string msg;
    msg.append("MSG:");
    char * ack_char;
    itoa(ack, ack_char, 8);
    msg.append(std::string(ack_char));
    msg.append(std::string(":"));
    msg.append(msgToCreate);
    const char * msg_char = msg.c_str();
    return msg_char; 
}
