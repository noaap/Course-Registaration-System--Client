//
// Created by spl211 on 31/12/2020.
//
#include "connectionHandler.h"
#include "KeyBoardReader.h"
#include <mutex>
#include <boost/lexical_cast.hpp>

KeyBoardReader::KeyBoardReader(ConnectionHandler& connectionHandler, mutex& mutx,condition_variable & waitCon,int &flag):connectionHandler(&connectionHandler),mtx(&mutx),waitCon(&waitCon),flag(&flag),terminate(false){
}
void KeyBoardReader::run() {
    while (!this->terminate) { //indicate when we logged out
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        short op = 0; //the op of the right command the user asked for
        char c = buf[0];
        int counter = 0;
        string msgOP = ""; //string of the keyword of the asked command
        char tempBuf[1024]; //the buf we eventually return
        while (c != ' ' && c != '\0') { //getting the string
            msgOP = msgOP + c;
            counter++;
            c = buf[counter];
        }
        counter = counter + 1;//to start after the space char
        if (msgOP == "ADMINREG") {
            op = 1;
            regLogStuStatHandle(op, buf, counter,tempBuf);//a function we wrote below to handle different commands with same protocol
        } else if (msgOP == "STUDENTREG") {
            op = 2;
            regLogStuStatHandle(op, buf, counter, tempBuf);
        } else if (msgOP == "LOGIN") {
            op = 3;
            regLogStuStatHandle(op, buf, counter, tempBuf);
        } else if (msgOP == "LOGOUT") {
            op = 4;
            counter = 2; //we need to send only 2 bytes
            shortToBytes(op, tempBuf);

        } else if (msgOP == "MYCOURSES") {
            op = 11;
            counter = 2;//same as op=4
            shortToBytes(op, tempBuf);
        } else if (msgOP == "STUDENTSTAT") {
            op = 8;
            regLogStuStatHandle(op, buf, counter, tempBuf);
        } else {//handling the same way for the last op's
            if (msgOP == "COURSEREG")
                op = 5;
            if (msgOP == "KDAMCHECK")
                op = 6;
            if (msgOP == "COURSESTAT")
                op = 7;
            if (msgOP == "ISREGISTERED")
                op = 9;
            if (msgOP == "UNREGISTER")
                op = 10;
            CourseNumberHandle(op, buf, counter, tempBuf);//function we wrote below to handle a 4 bytes narray to send
            counter = 3;//need to write only until place 3 bytes from tempBuf to send
        }

        counter++;//to send the number of bytes and not the last place

        if (!connectionHandler->sendBytes(tempBuf, counter)) {//we updated the counter we need to send
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        if (op == 4) {
                std::unique_lock<std::mutex> lock(*mtx);
                while (*flag==0)
                    waitCon->wait(lock);
                if (*flag==1)
                    terminate = true;
                else
                    *flag=0;
        }
    }
}

    void KeyBoardReader::regLogStuStatHandle(short op, char *buf, int &start,char *tempBuf) {//handling few cases  for username/passwords cases
        int tempCounter = 2; //counter that we will save to return the number of bytes(and writing the tempbuf)
        char c = buf[start];
        shortToBytes(op, tempBuf);
        while (c != '\0') { //while we still have bytes we got from the screen
            if (c == ' ')
                tempBuf[tempCounter] = '\0';//replace space with '\0' as asked
            else
                tempBuf[tempCounter] = c;
            start++;
            c = buf[start];
            tempCounter++;
        }
        tempBuf[tempCounter] = '\0';//adding to the end
        start = tempCounter;//to return the number of bytes to send(updates the counter in the run function)

    }
    void KeyBoardReader::CourseNumberHandle(short op, char *buf, int &start, char *tempBuf) {
        string courseNum = "";
        while (buf[start] != '\0') {//getting the course number we wish to check
            courseNum += buf[start];
            start++;
        }
        short k =(short)(std::stoi(courseNum));
        shortToBytes(op, tempBuf);
        char by[2];
        shortToBytes(k, by); //convert again to bytes then add them to tempbuf
        tempBuf[2] = by[0];
        tempBuf[3] = by[1];

    }
    void KeyBoardReader::shortToBytes(short num, char *bytesArr) {
        bytesArr[0] = ((num >> 8) & 0xFF);
        bytesArr[1] = (num & 0xFF);
    }




