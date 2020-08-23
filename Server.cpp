/*
 * Server.cpp
 *
 *  Created on: 20-Aug-2020
 *      Author: Tarang
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <unistd.h>
#include "Common.h"

int Ack_Init;
int Window_size = 5;
int base;
int flag = 0;
int countHead;
int seqNumber;


using namespace omnetpp;

class Server: public cSimpleModule {
public:
    simtime_t timeOut;
    cMessage *timeOutEvent;
    cMessage *message[Message_Count]; // message that has to be re-sent on timeOut
    Server();
    virtual ~Server();
    virtual cMessage* generateNewMessage(int type);
    virtual void sendBufferMsg(cMessage *msg, int type);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Server);

Server::Server() {

    timeOutEvent = nullptr;

}

Server::~Server() {

    cancelAndDelete(timeOutEvent);

}
void Server::initialize() {

    timeOut = 1.0;
    timeOutEvent = new cMessage("timeOutEvent");

    EV << "Sending Control message\n";

    Ack_Init = 1;

    if (Ack_Init == 1) {

        message[0] = generateNewMessage(INIT);

        sendBufferMsg(message[0], INIT);

    }
    scheduleAt(simTime() + timeOut, timeOutEvent);
}

void Server::handleMessage(cMessage *msg) {

    if (msg == timeOutEvent) {
        int i;

        EV << "timeOut expired, resending message and restarting timer\n";
        i = base;
        countHead = base;

        while (i < Window_size) {
            flag = 8;
            sendBufferMsg(message[i], NORMAL);
            i++;
        }

        scheduleAt(simTime() + timeOut, timeOutEvent);

    } else {
        EV << getName();

        if (strcmp("Window_size", msg->getName()) == 0) {

            EV << "window size is: " << Window_size << "\n";
            delete msg;
        }

        EV << "Timer cancelled, New Window Frame Initiated.\n";

        cancelEvent(timeOutEvent);

        for (int i = 0; i < Window_size; i++) {

            message[i] = generateNewMessage(NORMAL);
            sendBufferMsg(message[i], NORMAL);
        }
        scheduleAt(simTime() + timeOut, timeOutEvent);
    }

}
cMessage* Server::generateNewMessage(int type) {

    if (type == NORMAL) {
        char msgname[20];
        sprintf(msgname, "tic-%d", ++seqNumber);
        seqNumber = seqNumber % 256;
        cMessage *msg = new cMessage(msgname);
        return msg;
    } else if (type == INIT) {
        char msgname1[20];
        sprintf(msgname1, "Control Message");

        cMessage *msg = new cMessage(msgname1);
        return msg;
    } else {
        EV << "Not " << endl;
        return NULL;
    }

}

void Server::sendBufferMsg(cMessage *msg, int type) {
    if (type == NORMAL) {
        cMessage *copy = (cMessage*) msg->dup();
        send(copy, "out");

    } else if (type == INIT) {
        cMessage *copy = (cMessage*) msg->dup();
        send(copy, "out");

    }
}

