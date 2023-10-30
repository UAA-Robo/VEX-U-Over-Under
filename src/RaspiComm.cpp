#include "RaspiComm.hpp"
#include <iostream>
#include <stdlib.h>

RaspiComm::RaspiComm()
{
    

    
    vex::task thread = vex::task(startSendRecieveThread, this, 2);

}
void RaspiComm::sendMsg(std::string msg)
{
    msg_to_send = msg;
}

std::string RaspiComm::readMsg()
{
    return last_msg_recieved;
}

void RaspiComm::stopThreads()
{
    keepRunning = false;
}

int RaspiComm::startSendRecieveThread(void * param)
{
    RaspiComm * RC = static_cast<RaspiComm*>(param);
    RC->PORT = "dev/serial1";
    RC->last_send_ack = 0;
    RC->last_recieve_ack = 0;
    int rf = open(RC->PORT,O_RDWR);
    char *msg_recieved;

    // artifical way to stop sending message every loop
    int loopCounter = 0;
    while(RC->keepRunning == true)
    {
        int i = read(rf, msg_recieved, RC->MSG_SIZE);


        std::string  msg_recieved_string = std::string(msg_recieved);
        if(msg_recieved_string.substr(0,3) == "ACK")
        {
            
            RC->last_recieve_ack = atoi(msg_recieved_string.substr(4).c_str());
        }

        if(msg_recieved_string.substr(0,3) == "MSG")
        {
            size_t ack_length = msg_recieved_string.substr(4).find(':');
            
            int ack = atoi(msg_recieved_string.substr(4,ack_length).c_str());
            RC->last_msg_recieved = msg_recieved_string.substr(4+ack);
            int i = write(rf, msg_recieved_string.substr(4,ack_length).c_str(), RC->MSG_SIZE);
        }
        if(RC->last_recieve_ack == RC->last_send_ack)
        {
            RC->msg_to_send ="";
        }
        
        if(RC->msg_to_send != "" and loopCounter >= 1000)
        {
            if(RC->msg_to_send.length() <= RC->MSG_SIZE)
            {
                RC->last_send_ack++;
                const char * msg_to_send_char = RC->createMessage(RC->msg_to_send, RC->last_send_ack);
                int i = write(rf, msg_to_send_char, RC->MSG_SIZE);
                loopCounter = 0;
                //std::copy(msg_to_send->begin(), msg_to_send->end(), msg);
                
                
            }
            

            
        }
        loopCounter++;
    }
    return 0;
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
