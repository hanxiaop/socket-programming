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
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

#define BUFFERLENGTH 500
#define PORTSEV_B     22047
#define PORTSEV_A     21047
#define PORTSEV_C     23047

#define AWS_UDP       24047
#define AWS_TCPCLI    25047
#define AWS_TCPMONI   26047

int main() {

    int serverAsoc;
    if ( (serverAsoc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
        perror( "Can't create socket." );
        return 0;
    }
    int serverBsoc;
    if ( (serverBsoc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
        perror( "Can't create socket." );
        return 0;
    }
    int serverCsoc;
    if ( (serverCsoc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
        perror( "Can't create socket." );
        return 0;
    }



    int clientSoc;
    clientSoc = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSoc == -1 ) {
	perror("Can't create socket");
	exit(1);
    }
    struct sockaddr_in clientAddr;
    memset( &clientAddr, 0, sizeof(clientAddr) );
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons( AWS_TCPCLI );              
    clientAddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );  

    int monitorSoc;
    monitorSoc = socket(AF_INET, SOCK_STREAM, 0);
    if (monitorSoc == -1 ) {
	perror("Can't create socket");
	exit(1);
    }
    struct sockaddr_in monitorSocAddr;
    memset( &monitorSocAddr, 0, sizeof(monitorSocAddr) );
    monitorSocAddr.sin_family = AF_INET;
    monitorSocAddr.sin_port = htons( AWS_TCPMONI );              
    monitorSocAddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );  


    if (bind(clientSoc, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) < 0) {
	perror("Bind failed");
	exit(1);
    }
    
    if (listen(clientSoc, 5) < 0 ) {
	perror("listen");
	exit(1);
    }

    if (bind(monitorSoc, (struct sockaddr *)&monitorSocAddr, sizeof(monitorSocAddr)) < 0) {
	perror("Bind failed");
	exit(1);
    }
    
    if (listen(monitorSoc, 5) < 0 ) {
	perror("listen");
	exit(1);
    }




    struct sockaddr_in serverAaddr;
    memset( &serverAaddr, 0, sizeof(serverAaddr) );
    serverAaddr.sin_family = AF_INET;
    serverAaddr.sin_port = htons( PORTSEV_A );              
    serverAaddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );  
    socklen_t serverAaddrLen = sizeof(serverAaddr);
    struct sockaddr_in serverBaddr;
    memset( &serverBaddr, 0, sizeof(serverBaddr) );
    serverBaddr.sin_family = AF_INET;
    serverBaddr.sin_port = htons( PORTSEV_B );              
    serverBaddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );  
    socklen_t serverBaddrLen = sizeof(serverBaddr);
    struct sockaddr_in serverCaddr;
    memset( &serverCaddr, 0, sizeof(serverCaddr) );
    serverCaddr.sin_family = AF_INET;
    serverCaddr.sin_port = htons( PORTSEV_C );              
    serverCaddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );  
    socklen_t serverCaddrLen = sizeof(serverCaddr);

    
    struct sockaddr_in clientserA;
    socklen_t clientserALen = sizeof(clientserA);
    //char bufferToSerA[BUFFERLENGTH] = {0};
    struct sockaddr_in clientserB;
    socklen_t clientserBLen = sizeof(clientserB);
    //char bufferToSerB[BUFFERLENGTH] = {0};
    struct sockaddr_in clientserC;
    socklen_t clientserCLen = sizeof(clientserC);
    char bufferForC[BUFFERLENGTH];

    //int readData;
    //char buffer[BUFFERLENGTH] = {0};
    //readData = read(createdSocket, buffer, BUFFERLENGTH);
   
    
    cout << "The AWS is up and running \n";
      int createdSocketMonitor;   
    struct sockaddr_in fromMOnitorAddr;
    int sizeofMonitor = sizeof(fromMOnitorAddr);
    if ((createdSocketMonitor = accept(monitorSoc, (struct sockaddr *)&fromMOnitorAddr, (socklen_t*)&fromMOnitorAddr)) < 0) {
	perror("accept");
	exit(1);
    }

while(1){
    int createdSocket;   
    struct sockaddr_in fromclientAddr;
    int sizeofClient = sizeof(fromclientAddr);
    if ((createdSocket = accept(clientSoc, (struct sockaddr *)&fromclientAddr, (socklen_t*)&sizeofClient)) < 0) {
	perror("accept");
	exit(1);
    }

  


    int readData;
    char bufferClient[BUFFERLENGTH] = {0};

    readData = read(createdSocket, bufferClient, BUFFERLENGTH);
if (readData > 0) {
    bufferClient[readData] = '\0';
    string clientInput;
    clientInput = bufferClient;
    //cout << "sssssssssssssssss: " << clientInput << "\n";
    memset(bufferClient, 0, BUFFERLENGTH);
    string userInput;
    vector<string> userInp;
    istringstream ss(clientInput);
    while(getline(ss, userInput, ',')){
        userInp.push_back(userInput);
    }
    
    char clientInputData[BUFFERLENGTH];
    string test;
    test = userInp[0];
    strcpy(clientInputData, test.c_str());
    char clientInputDataBB[BUFFERLENGTH];
    string testB;
    testB = userInp[0];//"38";
    strcpy(clientInputDataBB, testB.c_str());

    cout << "The AWS received link ID=<" << userInp[0] << ">, size=<" << userInp[1] << ">, and power=<" << userInp[2] << "> from the client using TCP over port <25047> \n";
    char buffersendInput[BUFFERLENGTH];
    string resultTomNI = clientInput;
    strcpy(buffersendInput, resultTomNI.c_str());
    send(createdSocketMonitor, buffersendInput, BUFFERLENGTH, 0);
    memset(buffersendInput, 0, BUFFERLENGTH);
    cout << "The AWS sent link ID=<" << userInp[0] << ">, size=<" << userInp[1] << ">, and power=<" << userInp[2] << "> to the monitor using TCP over port <26047> \n";
    
    if (sendto(serverAsoc, clientInputData, strlen(clientInputData), 0, (struct sockaddr      	 	*)&serverAaddr,serverAaddrLen) < 0) {
        perror("sendto");
    }
    cout << "The AWS sent link ID=<" << userInp[0] << "> to Backend-Server A using UDP over port <24047> \n";
    memset(clientInputData, 0, BUFFERLENGTH);
    //cout << clientInputData << "\n";
    if (sendto(serverBsoc, clientInputDataBB, strlen(clientInputDataBB), 0, (struct sockaddr      	 	*)&serverBaddr,serverBaddrLen) < 0) {
        perror("sendto");
    }
    cout << "The AWS sent link ID=<" << userInp[0] << "> to Backend-Server B using UDP over port <24047> \n";
    memset(clientInputDataBB, 0, BUFFERLENGTH);
    //cout << clientInputData << "\n";

    int recFromA = recvfrom(serverAsoc, clientInputData,BUFFERLENGTH,0,(struct sockaddr *)&clientserA,&clientserALen);
    clientInputData[recFromA] = '\0';
    //cout << "message from server A: " << clientInputData<< "\n";
    int recFromB = recvfrom(serverBsoc, clientInputDataBB,BUFFERLENGTH,0,(struct sockaddr *)&clientserB,&clientserBLen);
    clientInputDataBB[recFromB] = '\0';
    //cout << "message from serverB: " << clientInputDataBB << "\n";
  


    string resultA;
    string resultB;
    resultA = clientInputData;
    resultB = clientInputDataBB;
    //cout << resultA << "-----" << resultB << "-----\n";
    memset(clientInputData, 0, BUFFERLENGTH);
    memset(clientInputDataBB, 0, BUFFERLENGTH);
    if (resultA == "0" && resultB == "0") {
        cout << "The AWS received <0> matches from Backend-Server <A> using UDP over port <24047> \n";
	cout << "The AWS received <0> matches from Backend-Server <B> using UDP over port <24047> \n";

	char buffersendResultBack[BUFFERLENGTH];
	
	string totalresult ="0";
       
	
	strcpy(buffersendResultBack, totalresult.c_str());
	send(createdSocket, buffersendResultBack, sizeof(buffersendResultBack), 0);
	send(createdSocketMonitor, buffersendResultBack, sizeof(buffersendResultBack), 0);
	cout << "The AWS sent No Match to the monitor and the client using TCP over ports <25047> and <26047>, respectively \n";
	memset(buffersendResultBack, 0, BUFFERLENGTH);
	

    }else if (resultA == "0") {
	cout << "The AWS received <0> matches from Backend-Server <A> using UDP over port <24047> \n";
	cout << "The AWS received <1> matches from Backend-Server <B> using UDP over port <24047> \n";
        // recev no from a
        string resultBLInk;
    	vector<string> linkBdataBase;
    	istringstream ss(resultB);
   	while(getline(ss, resultBLInk, ',')){
        	linkBdataBase.push_back(resultBLInk);
    	}
   	
        string strSendToCFB;
        strSendToCFB = userInp[0]+","+userInp[1]+","+userInp[2]+","+linkBdataBase[1]+","+linkBdataBase[2]+","+linkBdataBase[3]+","+linkBdataBase[4];
	strcpy(bufferForC, strSendToCFB.c_str());
        if (sendto(serverCsoc, bufferForC, strlen(bufferForC), 0, (struct sockaddr      	 	*)&serverCaddr,serverCaddrLen) < 0) {
        perror("sendto");
        }
 	 cout << "The AWS sent link ID=<" << userInp[0] << ">, size=<" << userInp[1] << ">, and power=<" << userInp[2] << "> and link information to Backend-Server C using UDP over port <24047> \n";
        memset(bufferForC, 0, BUFFERLENGTH);
	int msgFromC;
        msgFromC = recvfrom(serverCsoc, bufferForC, BUFFERLENGTH, 0, (struct sockaddr*)&clientserC, &clientserCLen);
	bufferForC[msgFromC] = '\0';
	cout << "The AWS received outputs from Backend-Server C using UDP over port <24047> \n";
	string resultfC = bufferForC;
	memset(bufferForC, 0, BUFFERLENGTH);
	
	char buffersendResultBack[BUFFERLENGTH];
	istringstream ssss(resultfC);
	vector<string> results;
	string singleResult;
        while(getline(ssss, singleResult, ',')){
            results.push_back(singleResult);
        }
	


	strcpy(buffersendResultBack, resultfC.c_str());
	send(createdSocket, buffersendResultBack, sizeof(buffersendResultBack), 0);
	cout << "The AWS sent delay=<" << results[2] << ">ms to the client using TCP over port <25047> \n";
	send(createdSocketMonitor, buffersendResultBack, sizeof(buffersendResultBack), 0);
	cout << "The AWS sent detailed results to the monitor using TCP over port <26047> \n";
	memset(buffersendResultBack, 0, BUFFERLENGTH);
	//close(createdSocket);
	//close(createdSocketMonitor);
	


    }else if (resultB == "0") {
	cout << "The AWS received <1> matches from Backend-Server <A> using UDP over port <24047> \n";
	cout << "The AWS received <0> matches from Backend-Server <B> using UDP over port <24047> \n";
	// recev no from A
        string resultALInk;
    	vector<string> linkAdataBase;
    	istringstream ss(resultA);
   	while(getline(ss, resultALInk, ',')){
        	linkAdataBase.push_back(resultALInk);
    	}
   	
        
	string strSendToCFA;
        strSendToCFA = userInp[0]+","+userInp[1]+","+userInp[2]+","+linkAdataBase[1]+","+linkAdataBase[2]+","+linkAdataBase[3]+","+linkAdataBase[4];
        strcpy(bufferForC, strSendToCFA.c_str());
        if (sendto(serverCsoc, bufferForC, strlen(bufferForC), 0, (struct sockaddr      	 	*)&serverCaddr,serverCaddrLen) < 0) {
        perror("sendto");
        }
	cout << "The AWS sent link ID=<" << userInp[0] << ">, size=<" << userInp[1] << ">, and power=<" << userInp[2] << "> and link information to Backend-Server C using UDP over port <24047> \n";
        memset(bufferForC, 0, BUFFERLENGTH);
	int msgFromC;
        msgFromC = recvfrom(serverCsoc, bufferForC, BUFFERLENGTH, 0, (struct sockaddr*)&clientserC, &clientserCLen);
	bufferForC[msgFromC] = '\0';
	cout << "The AWS received outputs from Backend-Server C using UDP over port <24047> \n";
	string resultfC = bufferForC;
	memset(bufferForC, 0, BUFFERLENGTH);
	
	char buffersendResultBack[BUFFERLENGTH];
	istringstream ssss(resultfC);
	vector<string> results;
	string singleResult;
        while(getline(ssss, singleResult, ',')){
            results.push_back(singleResult);
        }
	
	strcpy(buffersendResultBack, resultfC.c_str());
	send(createdSocket, buffersendResultBack, sizeof(buffersendResultBack), 0);
	cout << "The AWS sent delay=<" << results[2] << ">ms to the client using TCP over port <25047> \n";
	send(createdSocketMonitor, buffersendResultBack, sizeof(buffersendResultBack), 0);
	cout << "The AWS sent detailed results to the monitor using TCP over port <26047> \n";
	memset(buffersendResultBack, 0, BUFFERLENGTH);
	//close(createdSocket);
	//close(createdSocketMonitor);
    }



}
	
close(createdSocket);

}
    close(createdSocketMonitor);
    close(clientSoc);
    close(serverAsoc);
    close(serverBsoc);
    close(serverCsoc);
    close(monitorSoc);





    return 0;
}
