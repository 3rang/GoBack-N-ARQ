/*
 * Server.cpp
 *  Application to demo GoBack-N ARQ Protocol
 *  Created on: 20-Aug-2020
 *      Author: Tarang
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <unistd.h>

#include "Common.h"

int Ack_Init;
int Window_size ;
int base;
int flag = 0;
int countHead;
int seqNumber;


using namespace omnetpp;


/*
 * Class Server with cSimpleModule inheritance
 */

class Server: public cSimpleModule {
public:
    simtime_t timeOut;
    cMessage *timeOutEvent;
    cMessage *message1[Message_Count];
    Pck *message[Message_Count]; // message that has to be re-sent on timeOut
    Server();
    virtual ~Server();
    virtual Pck* generateNewMessage(int type);
    virtual cMessage* generateINITMessgae(int type);
    virtual void sendBufferMsg(Pck *msg, cMessage *msgg,int type);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Server);

/*
 * Class constructor  and destructor
 */

Server::Server() {


    timeOutEvent = nullptr;

}

Server::~Server() {

    cancelAndDelete(timeOutEvent);

}


/*
 * Function: initialize :- init Function for initialization connections
 * ----------------------------
 *   Return :void
 *
 *   Arg :1. pointer of cMessage
 *
 */


void Server::initialize() {

    timeOut = 1.0;
    timeOutEvent = new cMessage("timeOutEvent");


    EV << "Sending Control message \n"<< par("Window_Size").intValue();

    Window_size = par("Window_Size").intValue();

    EV << "Window_Size Init"<<endl;

    Ack_Init = 1;

    if (Ack_Init == 1) {

        message1[0] = generateINITMessgae(INIT);

        sendBufferMsg(NULL,message1[0], INIT);

    }
    scheduleAt(simTime() + timeOut, timeOutEvent);
}


/*
 * Function: handleMessage :- Handler Function for handle the messages from the client Packets
 * ----------------------------
 *   Return :void
 *
 *   Arg :1. pointer of cMessage
 *
 */

void Server::handleMessage(cMessage *msg) {


    if (msg == timeOutEvent) {
        int i;

        EV << "timeOut expired, resending message and restarting timer\n";
        i = base;
        countHead = base;

        while (i < Window_size) {
            flag = 8;
            sendBufferMsg(message[i],NULL, NORMAL);
            i++;
        }

        scheduleAt(simTime() + timeOut, timeOutEvent);

    } else {
        EV << getName();

        if (strcmp("Window_size", msg->getName()) == 0) {

            EV << "window size is: " << Window_size << "\n";
            delete msg;
        }

        EV << "Timer Off, New Window Frame Initiated.\n";

        cancelEvent(timeOutEvent);

        for (int i = 0; i < Window_size; i++) {

            message[i] = generateNewMessage(NORMAL);
            sendBufferMsg(message[i],NULL, NORMAL);
        }
        scheduleAt(simTime() + timeOut, timeOutEvent);
    }

}

/*
 * Function: generateNewMessage :- Use for Normal Packets Generate
 * ----------------------------
 *   Return :pointer of Pck
 *
 *   Arg :1. Type of Msg need to generate (Normal  or INIT message)
 *
 */


Pck* Server::generateNewMessage(int type) {

    if (type == NORMAL) {
        char msgname[20];
        char SourceAddress;
        sprintf(msgname, "tic-%d", ++seqNumber);
        seqNumber = seqNumber % 256;
        Pck *msg = new Pck(msgname);
        msg->setSourceAddress(SOURCE_ADDR);
        msg->setDestinationAddress(DESTINATION_ADDR);
        msg->setSeq(seqNumber);
        return msg;
    } else {
        EV << "Not " << endl;
        return NULL;
    }

}
/*
 * Function: generateINITMessgae :- Use for INIT Message generate
 * ----------------------------
 *   Return :pointer of cMessage
 *
 *   Arg :1. Type of Msg need to generate (Normal  or INIT message)
 *
 *
 */


cMessage* Server::generateINITMessgae(int type) {

      if (type == INIT) {
        char msgname1[20];
        sprintf(msgname1, "INIT Message");

        cMessage *msg = new cMessage(msgname1);

        return msg;
    } else {
        EV << "Not " << endl;
        return NULL;
    }

}


/*
 * Function: sendBufferMsg :- Use for Sending Message to Client (Receiver Part)
 * ----------------------------
 *   Return : Void
 *   Arg :1. pointr of PCK
 *        2. pointer of cMessage
 *        3. Type of Msg (Normal Pcket or INIT messaage)
 *
 */

void Server::sendBufferMsg(Pck *msg,cMessage *msgg, int type) {
    if (type == NORMAL) {
        cMessage *ttmsg = check_and_cast<Pck *>(msg);
        cMessage *copy = (cMessage*) ttmsg->dup();

        send(copy, "out");

    } else if (type == INIT) {


                cMessage *copy = (cMessage*) msgg->dup();
        send(copy, "out");

    }
}

/*
 * END OF FILE
 */

