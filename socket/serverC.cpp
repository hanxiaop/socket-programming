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
#include <stdlib.h>
#include <math.h>
using namespace std;

#define BUFFERLENGTH 500
#define PORT 23047


int main(){

	int servSockA;
	servSockA = socket(AF_INET, SOCK_DGRAM, 0);
	if (servSockA < 0){
		cout << "Can not create socket";
		return 0;

	}
	struct sockaddr_in sockA_addr;
	memset(&sockA_addr, 0, sizeof(sockA_addr));
	sockA_addr.sin_family = AF_INET;
	sockA_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockA_addr.sin_port = htons(PORT); //converts small to big
	socklen_t sockA_addrLen = sizeof(sockA_addr);
	
	if( bind(servSockA, (struct sockaddr*)&sockA_addr, sizeof(sockA_addr)) == 0 ){
		cout << "The ServerC is up and running using UDP on port <23047>. \n";
	} else {
		perror("Can't bind the socket");
		return 0;
	}	
	// cout << "bind successfully" << "\n";

	struct sockaddr_in aws_addr;
	socklen_t sockSize = sizeof(aws_addr);
	

	//create buffers
	int messageSize;
	char createdBuffer[BUFFERLENGTH] = {0};

	while(1){

	messageSize = recvfrom(servSockA, createdBuffer, BUFFERLENGTH, 0, (struct sockaddr*)&aws_addr, &sockSize);
	if (messageSize < 0) {
		cout << "can't receive data" << "\n";
		return 0;
	}
	if (messageSize > 0) {
	createdBuffer[messageSize] = '\0';
        
	string resultFromAWS;
        resultFromAWS = createdBuffer;
        memset(createdBuffer, 0, BUFFERLENGTH);
    	vector<string> changeDataToVec;
    	istringstream ss(resultFromAWS);
	string singleData;
   	while(getline(ss, singleData, ',')){
        	changeDataToVec.push_back(singleData);
    	}
        
   	
	//istringstream sss;

	float linkID = atoi(changeDataToVec[0].c_str());
	float size = atoi(changeDataToVec[1].c_str());  // bits
	float power = atoi(changeDataToVec[2].c_str()); // dBm
	float bandwidth = atoi(changeDataToVec[3].c_str()); // Mhz, change to HZ
	float length = atoi(changeDataToVec[4].c_str()); // km
	float velocity = atoi(changeDataToVec[5].c_str());  // m/s
	float noisePower = atoi(changeDataToVec[6].c_str()); // dbm
	cout << "The ServerC received link information of link<" << linkID << ">, file size <" << size << ">, and signal power <" << power <<"> \n";
	float bitRate = bandwidth * 1000000 * log2(1+ (power/noisePower));
	float tt = roundf((size/bitRate * 1000)*100)/100; // in ms
	float tp = roundf((length *1000 / (velocity * 10000000))*100)/100;
	float etoe = roundf((tt + tp)*100)/100;
	
	stringstream sstt;
	stringstream sstp;
	stringstream ssete;
	sstt <<tt;
	string ttstr = sstt.str();
	sstp << tp;
	string tpstr = sstp.str();
	ssete << etoe;
	string etestr = ssete.str();
	
	string totalResut = ttstr+","+tpstr+","+etestr;
	char sendbackBuffer[BUFFERLENGTH];
	strcpy(sendbackBuffer, totalResut.c_str());
	cout << "The ServerC finished the calculation for link <" << linkID << "> \n";
	if (sendto(servSockA, sendbackBuffer, strlen(sendbackBuffer), 0, (struct sockaddr      	 	*)&aws_addr,sockSize) < 0) {
        perror("sendto");
        }
        memset(sendbackBuffer, 0, BUFFERLENGTH);
	
	
	cout << "The Server C finished sending the output to AWS \n";
	
	
	//char tpBuffer[BUFFERLENGTH];
	//char etoeBuffer[BUFFERLENGTH];

	/*
	cout << "Tt: " << tt << "\n";
	cout << "Tp: " << tp << "\n";
	cout << "End to End: " << etoe << "\n";
	string sendBackResult;
	char resultsSent[BUFFERLENGTH];
	*/
	//sendBackResult = to_string(tt)+","+to_string(tp)+","+to_string(etoe);
	//string ttt = to_string(tt);
	
	/*
	if (changeDataToVec.size() == 7){
	 	istringstream sss(changeDataToVec[0]);
		sss >> linkID;
		cout << linkID << "\n";
	 	istringstream sss2(changeDataToVec[1]);
		sss2 >> size;
		cout << size << "\n";
	}*/
 	


	
      
		//string receivedAWS;
		//receivedAWS = createdBuffer;
		//string awsSentData = createdBuffer;
    		//memcpy(awsSentData, createdBuffer, strlen(createdBuffer)+1);
		//memset(createdBuffer, 0, BUFFERLENGTH);
		//cout << awsSentData << "\n";
		//cout << awsSentData;
	/*
		if (sendto(servSockA, createdBuffer, strlen(createdBuffer), 0, (struct sockaddr      	 					*)&aws_addr,sockSize) < 0) {
		 perror("sendto");
		} 
		cout << "data receive from: " << awsSentData << "\n" ;
		//cout << "send information to AWS2: " << result;
		
		
		memset(createdBuffer, 0, BUFFERLENGTH);
		
	
		strcpy(createdBuffer, "0");
		if (sendto(servSockA, createdBuffer, strlen(createdBuffer), 0, (struct sockaddr      	 					*)&aws_addr,sockSize) < 0) {
		 perror("sendto");
		}
		cout << "data receive from: " << awsSentData << "\n" ;
		//cout << "send information to AWS: " << result;
		memset(createdBuffer, 0, BUFFERLENGTH);
		*/
	
		

		}
	}

    
	close(servSockA);


    return 0;
}
