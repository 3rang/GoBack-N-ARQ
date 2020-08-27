/*
 * Common.h
 *
 *  Created on: 20-Aug-2020
 *      Author: Tarang
 */

#ifndef COMMON_H_
#define COMMON_H_

#include"Packet_m.h"


#define Message_Count  100
#define INIT 1
#define NORMAL 0
//#define SOURCE_ADDR "172.20.0.1"
//#define DESTINATION_ADDR "172.20.0.29"

enum ADDR{
    SOURCE_ADDR ,
    DESTINATION_ADDR
};


extern int countMsg;
extern int Ack_Init;
extern int Window_size;
extern int base;
extern int flag;
extern int countHead;
extern int seqNumber;



#endif /* COMMON_H_ */
