//
// Created by spl211 on 31/12/2020.
//

#ifndef BOOST_ECHO_CLIENT_KEYBOARDREADER_H
#define BOOST_ECHO_CLIENT_KEYBOARDREADER_H
#include <iostream>
#include <mutex>
#include "condition_variable"
#include <boost/lexical_cast.hpp>


using namespace std;

class KeyBoardReader {
public:
    KeyBoardReader(ConnectionHandler &connectionHandler,mutex &mtx,condition_variable& waitCon,int&flag);
    void run();
    void regLogStuStatHandle(short op,char*bytesArr,int &start,char* tempBuf);
    void CourseNumberHandle(short op,char*bytesArr,int &start,char* tempBuf);
    void shortToBytes(short num, char* bytesArr);
private:

    ConnectionHandler *connectionHandler ;
    mutex *mtx;
    condition_variable *waitCon;
    int *flag;
    bool terminate;

};


#endif //BOOST_ECHO_CLIENT_KEYBOARDREADER_H
