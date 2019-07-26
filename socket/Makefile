all: serverAoutput serverBoutput serverCoutput awsoutput monitoroutput client

serverAoutput: serverA.cpp
	g++ -o serverAoutput serverA.cpp
serverBoutput: serverB.cpp
	g++ -o serverBoutput serverB.cpp
serverCoutput: serverC.cpp
	g++ -o serverCoutput serverC.cpp
awsoutput: aws.cpp
	g++ -o awsoutput aws.cpp
monitoroutput: monitor.cpp
	g++ -o monitoroutput monitor.cpp 
client: client.cpp
	g++ -o client client.cpp

clean: 
	$(RM) serverAoutput serverBoutput serverCoutput AWSoutput monitoroutput client

.PHONY: serverA

serverA: serverA

	./serverAoutput

.PHONY: serverB

serverB: serverB

	./serverBoutput

.PHONY: serverC

serverC: serverC

	./serverCoutput

.PHONY: monitor

monitor: monitor

	./monitoroutput

.PHONY: aws

aws: aws

	./awsoutput
