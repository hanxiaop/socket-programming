#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <typeinfo>


#define AWS_TCPMONI   26047
#define BUFFERLENGTH 500

using namespace std;

int main() {

    int cliSocket;
    cliSocket = socket(AF_INET, SOCK_STREAM, 0);
    if ((cliSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	perror("Cant't get socket");
    }
    struct sockaddr_in cliSocketAddr;
    memset(&cliSocketAddr, 0, sizeof(cliSocketAddr));
    cliSocketAddr.sin_family =AF_INET;
    cliSocketAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    cliSocketAddr.sin_port = htons(AWS_TCPMONI);
    
    // port
    int getsock_check;
    socklen_t sockLength = sizeof(cliSocketAddr);
    
    cout << "The monitor is up and running \n";
    if (connect(cliSocket, (struct sockaddr *)&cliSocketAddr, sockLength) < 0) {
    perror("Can't get connection.\n");
    } 
    while(1){
    
    
    
    int readData;
    char bufferClient[BUFFERLENGTH] = {0};
    readData = read(cliSocket, bufferClient, BUFFERLENGTH);
    
    bufferClient[readData] = '\0';
    string clientInput;
    clientInput = bufferClient;
    memset(bufferClient, 0, BUFFERLENGTH);
    if (readData > 0) {
        istringstream ssss(clientInput);
        vector<string> delayresults;
        string singleResult;
        while(getline(ssss, singleResult, ',')){
            delayresults.push_back(singleResult);
        }
	cout << "The monitor received input=<" << delayresults[0] << ">, size=<" << delayresults[1] << ">, and power =<" << delayresults[2] << "> from the AWS \n";
        readData = read(cliSocket, bufferClient, BUFFERLENGTH);
        if (readData > 0) {
	    bufferClient[readData] = '\0';
	    string finalResult;
	    finalResult = bufferClient;
	    if (finalResult != "0") {
		    istringstream sssss(finalResult);
		    vector<string> fnalresults;
		    string singleResultF;
		    while(getline(sssss, singleResultF, ',')){
		        fnalresults.push_back(singleResultF);
		    }
		    cout << "The result for link <" << delayresults[0] << ">: \n";
		    cout << "Tt = <" << fnalresults[0] << ">ms \n";
		    cout << "Tp = <" << fnalresults[1] << ">ms \n";
		    cout << "Delay = <" << fnalresults[2] << ">ms \n";
	    } else {
		cout << "Found no matches for link <" << delayresults[0] << "> \n";
	    }
	    
        }else {
		//close(cliSocket);
	}
        
    }
    
    //send(cliSocket, buffer, sizeof(buffer), 0);
    //string valread;
    //valread = write(cliSocket, buffer, BUFFERLENGTH);
    //cout << buffer;
     
    //send(cliSocket, argv
    }


   close(cliSocket);
   



    return 0;
}


