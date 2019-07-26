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


#define AWS_TCPCLI    25047
#define BUFFERLENGTH 500

using namespace std;

int main(int argc, char *argv[]) {


    if (argc != 4) {
        cout << "The input is not correct. Need 3 inputs." << argc;
        return 0;
    }

    int cliSocket;
    cliSocket = socket(AF_INET, SOCK_STREAM, 0);
    if ((cliSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	perror("Cant't get socket");
    }
    struct sockaddr_in cliSocketAddr;
    memset(&cliSocketAddr, 0, sizeof(cliSocketAddr));
    cliSocketAddr.sin_family =AF_INET;
    cliSocketAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    cliSocketAddr.sin_port = htons(AWS_TCPCLI);
    
    // port
    int getsock_check;
    socklen_t sockLength = sizeof(cliSocketAddr);
    
    //getsock_check = getsockname(cliSocket, (struct sockaddr *)&cliSocketAddr, &sockLength);
    //if (getsock_check == -1) {
    //    perror("getsockname");
	//exit(1);
    //}
    cout << "The client is up and running \n";
    if (connect(cliSocket, (struct sockaddr *)&cliSocketAddr, sockLength) < 0) {
	perror("Can't get connection.\n");
    }

    
    char buffer[BUFFERLENGTH];
    char* linkId = argv[1];
    char* size = argv[2];
    char* power = argv[3];
    
    strcpy(buffer, linkId);
    strcat(buffer, ",");
    strcat(buffer, size);
    strcat(buffer, ",");
    strcat(buffer, power);
   
    
    send(cliSocket, buffer, sizeof(buffer), 0);
    cout << "The client sent link ID=<" <<linkId<<">, size=<"<< size << ">, and power=<" << power << "> to AWS \n";
    memset(buffer, 0, BUFFERLENGTH);
    //string valread;
    //valread = write(cliSocket, buffer, BUFFERLENGTH);
    //cout << buffer;

    //send(cliSocket, argv
    int readData;
    readData = read(cliSocket, buffer, BUFFERLENGTH);
    buffer[readData] = '\0';
    string results = buffer;
    istringstream ssss(results);
    vector<string> delayresults;
    string singleResult;
    while(getline(ssss, singleResult, ',')){
        delayresults.push_back(singleResult);
    }
    if (delayresults[0] == "0") {
	cout << "Found no matches for link <" << linkId << "> \n";
    }else {
	cout << "The delay for link <" << linkId<< "> is <" << delayresults[2] << ">ms \n";
    }

    


    close(cliSocket);
   



    return 0;
}


