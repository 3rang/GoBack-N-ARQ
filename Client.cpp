/*
 * Client.cpp
 * Application to demo GoBack-N ARQ Protocol
 *  Created on: 20-Aug-2020
 *      Author: Tarang
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <unistd.h>
#include "Common.h"

using namespace omnetpp;

int countMsg;
int AD;


/*
 * Class Client with cSimpleModule inheritance
 */


class Client: public cSimpleModule {

public:
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Client);

/*
 * Function: handleMessage :- Handler Function for handle the messages from the client Packets and send back to Server
 * ----------------------------
 *   Return :void
 *
 *   Arg :1. pointer of cMessage
 *
 */

void Client::handleMessage(cMessage *msg) {

    char Ack_Count[10];

    if ((rand() % 10 == 0)) {
        AD=1;
        EV <<"2"<<endl;
        EV << "Lost message " << msg << endl;
        bubble("Message Lost");
        if (flag == 0) {
            base = countMsg;
        } else if (flag == 8) {
            base = countHead;
        }
        flag++;
        delete msg;
    } else if (Ack_Init == 1) {
        cMessage *msg1 = new cMessage("Window_size");
        send(msg1, "out");
        bubble("Received Window Size");
        Ack_Init = 0;
    } else if (flag == 8) {
        EV<<"retransmission of Frames :" <<endl;
        AD=1;
        countHead++;
        if (countHead > (Window_size - 1)) {
            delete msg, countMsg = 0;
            flag = 0;
            EV << msg << " lost, sending back an acknowledgement.\n";
            sprintf(Ack_Count, "ack-%d", seqNumber);
            cMessage *msg2 = new cMessage(Ack_Count);
            send(msg2, "out");
        }
        AD=0;
    }
    else {
        countMsg += 1;
    //    EV << "1" <<endl;
        if (countMsg == Window_size) {
          //  EV <<"1.1"<<endl;
            if (flag == 0) {
            //    EV <<"1.2"<<endl;
                EV << msg << " received, sending back an acknowledgement.\n";
                delete msg;
                sprintf(Ack_Count, "ack till -%d ", seqNumber);
                cMessage *msg2 = new cMessage(Ack_Count);
                send(msg2, "out");
                countMsg = 0;
                flag = 0;
            }

        }
        else if(flag !=8 && countMsg !=0 && AD==0)
        {
            EV << "ACk for CLI"<<endl;

       }


        }

    }
