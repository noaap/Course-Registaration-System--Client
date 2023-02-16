//
// Created by spl211 on 31/12/2020.
//
#include <stdlib.h>
#include <iostream>
#include "../include/connectionHandler.h"
#include "../include/KeyBoardReader.h"
#include "../include/ServerReader.h"
#include <thread>
#include <mutex>
#include "condition_variable"

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/

int main (int argc, char *argv[]) {

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }

    std::string host =argv[1];
    short port =atoi(argv[2]);
    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
   std::mutex mtx;
    std:: condition_variable waitCon;
    int flag=0; //flag for knowing the state of logout(we block the keyboard reader until we got an answer for logout)

    KeyBoardReader keyReader(connectionHandler,mtx,waitCon,flag);  //create runable object for reading from screen and sending the right command to server
   std::thread k(&KeyBoardReader::run,&keyReader);
   ServerReader ServerRead(connectionHandler,mtx,waitCon,flag);  //create runable object for getting response from server and print the asked response
    std::thread s(&ServerReader::run,&ServerRead);
   k.join();
   s.join();

    return 0;
}