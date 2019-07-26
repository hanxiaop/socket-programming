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
#define PORT 21047


int main(){

ifstream databaseA("database_a.csv");
	if (!databaseA.is_open()){
		std::cout << "Can't open File" << '\n';
	};

	vector< vector<string> > alldata;
	string eachDataLine;
	int i = 0;
	while(databaseA.good()){
	


		getline(databaseA, eachDataLine);

		istringstream singleLine(eachDataLine);
		vector<string> singleLineData;
		string aWord;

		while (getline(singleLine, aWord, ',')){
			singleLineData.push_back(aWord);

		}
		alldata.push_back(singleLineData);
	
		// cout << alldata[i][0] << "\n";
		i++;
	
	}

	databaseA.close();

	vector<string> linkIDs;
	for (int j = 0; j < alldata.size(); j++) {
		linkIDs.push_back(alldata[j][0]);
		//cout << alldata[i][0] << "\n";
	}
	
	
	//for (int k = 0; k < linkIDs.size(); k++) {
	//	cout << "links: " << linkIDs[k] << "\n";
	//}

	//cout << linkIDs;


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
		cout << "The ServerA is up and running using UDP on port <21047>. \n";
	} else {
		perror("Can't bind the socket");
		return 0;
	}	
	//cout << "bind successfully" << "\n";

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
		//string receivedAWS;
		//receivedAWS = createdBuffer;
		string awsSentData = createdBuffer;
		cout << "The ServerA received input <" << awsSentData << "> \n";
    		//memcpy(awsSentData, createdBuffer, strlen(createdBuffer)+1);
		memset(createdBuffer, 0, BUFFERLENGTH);
		string singleLinkID;
		string result;
		result = "-1";
		//cout << awsSentData;
		for (int i=0; i < linkIDs.size(); i++) {
			singleLinkID = linkIDs[i];
			if (singleLinkID == awsSentData) {
				result = alldata[i][0]+","+alldata[i][1]+","+alldata[i][2]+","+alldata[i][3]+","+alldata[i][4];
			
			}
			
		}
		// cout << result;
		if (result != "-1"){
			strcpy(createdBuffer, result.c_str());
			/*strcpy(createdBuffer, alldata[theIndex][0]);
   			strcat(createdBuffer, ",");
   			strcat(createdBuffer, alldata[theIndex][1]);
    			strcat(createdBuffer, ",");
   			strcat(createdBuffer, alldata[theIndex][2]);
    			strcat(createdBuffer, ",");
   			strcat(createdBuffer, alldata[theIndex][3]);
    			strcat(createdBuffer, ",");
   			strcat(createdBuffer, alldata[theIndex][4]);*/
			
   			// memcpy(createdBuffer, buffer, strlen(buffer)+1);
			cout << "The Server A has found <1> matches \n";
			if (sendto(servSockA, createdBuffer, strlen(createdBuffer), 0, (struct sockaddr      	 					*)&aws_addr,sockSize) < 0) {
       			 perror("sendto");
   			} 
			cout << "The Server A finished sending the output to AWS \n" ;
			//cout << "send information to AWS2: " << result;
			
			
			memset(createdBuffer, 0, BUFFERLENGTH);
			
		}else {
			cout << "The Server A has found <0> matches \n";
			strcpy(createdBuffer, "0");
			if (sendto(servSockA, createdBuffer, strlen(createdBuffer), 0, (struct sockaddr      	 					*)&aws_addr,sockSize) < 0) {
       			 perror("sendto");
   			}
			cout << "The Server A finished sending the output to AWS \n" ;
			//cout << "send information to AWS: " << result;
			memset(createdBuffer, 0, BUFFERLENGTH);
			
		}
		

		}
	}

    
	close(servSockA);


    return 0;
}

