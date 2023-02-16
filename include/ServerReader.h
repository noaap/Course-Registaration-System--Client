//
// Created by spl211 on 31/12/2020.
//

#ifndef BOOST_ECHO_CLIENT_SERVERREADER_H
#define BOOST_ECHO_CLIENT_SERVERREADER_H
#include <mutex>
#include <iostream>
#include <thread>
#include "../include/connectionHandler.h"
#include "condition_variable"
using namespace std;
class ServerReader {
public:
    ServerReader(ConnectionHandler &connectionHandler,mutex &mtx,condition_variable& waitCon,int&flag);
    void run();
    short bytesToShort(char* bytesArr);
    short msgBytesToShort(char* bytesArr);

private:
    ConnectionHandler *connectionHandler ;
    mutex *mtx;
    condition_variable *waitCon;
    int* flag;
};


#endif //BOOST_ECHO_CLIENT_SERVERREADER_H
