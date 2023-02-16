//
// Created by spl211 on 31/12/2020.
//

#include "ServerReader.h"
ServerReader::ServerReader(ConnectionHandler &connectionHandler,mutex &mtx,condition_variable & waitCon,int &flag):connectionHandler(&connectionHandler),mtx(&mtx),waitCon(&waitCon),flag(&flag){
}
void ServerReader::run() {
    while (!connectionHandler->shouldTerminate()) {
        const short bufsize = 1024;
        char buf[bufsize];
        connectionHandler->getBytes(buf, bufsize); //ger from server
        short op = bytesToShort(buf); //converting first 2 bytes as my op(12/13)
        short msgOp = msgBytesToShort(buf); //function we wrote down to convert the message op to short
        if (op == 12) {
            std::cout << "ACK "<<  msgOp  ; //first print ACK and nsg op as asked
            if(msgOp==4) {

                connectionHandler->setTerminate(true);//ends the run for the threads and this is our sign
                *flag=1;
                unique_lock<mutex> lock(*mtx);
                waitCon->notify_all();
                break;
            }
            if(buf[4]!='\0')//if we need to add a string to the output we are going down a line
                    std::cout << std::endl;
            int i = 4;
            while (buf[i] != '\0') {
                if (buf[i]=='\n') //a sign for us to go down one more line
                    std::cout<<std::endl;
                else
                std::cout << buf[i];
                i++;
            }
            std::cout << std::endl;
        } else { //not ACK but error
            std::cout << "ERROR " << msgOp << std::endl;
            if(msgOp==4)
            {
                unique_lock<mutex> lock(*mtx);
               *flag=2;
                waitCon->notify_all();
            }

        }
    }
}
short ServerReader::bytesToShort(char *bytesArr)
{
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}
short ServerReader::msgBytesToShort(char *bytesArr)
{
    short result = (short)((bytesArr[2] & 0xff) << 8);
    result += (short)(bytesArr[3] & 0xff);
    return result;
}
