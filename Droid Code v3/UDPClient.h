
#include <WinSock2.h>


#include <stdio.h>

#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#pragma comment(lib, "Ws2_32.lib")


//CLIENT: UDP
class UDPClient {
	SOCKET clientSock;
	sockaddr_in default_destination;
	int default_destination_size;
	sockaddr_in last_address; //last address received from
	bool default_destination_set;
	WSADATA wsaData;
public:
	UDPClient();
	UDPClient(int local_port);
	UDPClient(char* host_addr, int host_port);
	UDPClient(SOCKET sock);
	~UDPClient();

	//establishing client:
	void setDefaultDestination(char* host, int port);
	void setDefaultDestinationToLastReceived(); //TODO - fix this
	int bindTo(int local_port);

	//send data:
	int sendData(char* buffer, int len); //send data to default host
	int sendData(char* buffer, int len, char* host, int port); //send data to
	int sendString(char* string); //send string to default host
	int sendString(char* string, char* host, int port); //send to host
	int sendInt(int i); //send int to default host
	int sendInt(int i, char* host, int port); //send to host
	int reply(char* buffer, int len);

	//receive data:
	int receiveData(char* buffer, int len);
	char* recvLine();
	int recvInt();

	//meta info:
	//char* getDestinationAddr();
	//int getDestinationPort();
	char* getLocalAddr();
	int getLocalPort();
};


//CONSTRUCTOR: Create new socket, bound locally to ANY (random) port:
//THROWS: Exception upon ecountering an error.
UDPClient::UDPClient() {
	int result = WSAStartup(MAKEWORD(2, 2), (LPWSADATA)&wsaData);
	if (result < 0) {
		throw "WSAStartup ERROR.";
		return;
	}

	clientSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (clientSock == INVALID_SOCKET) {
		throw "Invalid Socket ERROR.";
		return;
	}

	//bind to (local) address - default properties:
	//setup local address info
	sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;//inet_addr("127.0.0.1");
	local.sin_port = 0; //assign given port
	result = bind(clientSock, (sockaddr*)&local, sizeof(local));

	default_destination_set = false;
}


//CONSTRUCTOR: Create new socket, bound locally to the given port:
//THROWS: Exception upon ecountering an error.
UDPClient::UDPClient(int local_port) {
	int result = WSAStartup(MAKEWORD(2, 2), (LPWSADATA)&wsaData);
	if (result < 0) {
		throw "WSAStartup ERROR.";
		return;
	}

	clientSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (clientSock == INVALID_SOCKET) {
		throw "Invalid Socket ERROR.";
		return;
	}

	//bind to (local) address - given port:
	sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;//inet_addr("127.0.0.1");
	local.sin_port = htons(local_port); //assign given port
										//bind: //NOTE: always returns -1?
	result = bind(clientSock, (sockaddr*)&local, sizeof(local));
	if (result < 0) {
		throw "Socket Bind ERROR";
		return;
	}

	default_destination_set = false;
}


//CONSTRUCTOR: Create a new socket, establishing a default destination to the
//	given host and port.
//THROWS: Exception upon encountering an error.
UDPClient::UDPClient(char* host_addr, int host_port) {
	int result = WSAStartup(MAKEWORD(2, 2), (LPWSADATA)&wsaData);
	if (result < 0) {
		throw "WSAStartup ERROR.";
		return;
	}

	clientSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (clientSock == INVALID_SOCKET) {
		throw "Invalid Socket ERROR.";
		return;
	}

	//bind to (local) address - default properties:
	//setup local address info
	sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr("localhost");
	local.sin_port = 0; //randomly selected port
						//bind: //NOTE: always returns -1?
	bind(clientSock, (sockaddr*)&local, sizeof(local));

	//set up destination variables:
	default_destination.sin_family = AF_INET;
	default_destination.sin_addr.s_addr = inet_addr(host_addr);
	default_destination.sin_port = htons(host_port);
	default_destination_size = sizeof(default_destination);

	default_destination_set = true;
}


//CONSTRUCTOR: Create from an existing socket.
//	Attempts to set default to the socket's originating host/port.
UDPClient::UDPClient(SOCKET sock) {
	clientSock = sock;

	//TODO: extract local info and set it here

	//EXTRACT THE IP AND PORT AND SET IT HERE
	default_destination_set = false;
}


//DECONSTRUCTOR: Cleanup WSA and die:
UDPClient::~UDPClient() {
	WSACleanup();
}


//SET_DEFAULT_DESTINATION: Attempts to set the default host/port.
void UDPClient::setDefaultDestination(char* host, int port) {
	//set up destination variables:
	default_destination.sin_family = AF_INET;
	default_destination.sin_addr.s_addr = inet_addr(host);
	default_destination.sin_port = htons(port);
	default_destination_size = sizeof(default_destination);

	default_destination_set = true;
}


//SET_DEFAULT_DESTINATION: Attempts to set the default host/port
//	to the last address a packet was received from.
void UDPClient::setDefaultDestinationToLastReceived() {

	//NOTE: this needs to work as "memcpy" or something
	//CHANGE THIS SO THAT IT DOES NOT MESS UP THE POINTERS.

	default_destination = last_address;
	default_destination_size = sizeof(default_destination);
	default_destination_set = true;
}


//BIND_TO: Binds the socket to the given LOCAL port (accepting on local port).
//	RETURNS result of bind()
int UDPClient::bindTo(int local_port) {
	//bind to (local) address - given port:
	sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;//inet_addr("127.0.0.1");
	local.sin_port = htons(local_port); //assign given port
										//bind: //NOTE: always returns -1?
	return bind(clientSock, (sockaddr*)&local, sizeof(local));
}


//SEND_DATA: attempts to send data to the default host/port.
//	RETURNS: results of (private) sendData(), or
//	-1 if failed or if default destination is not set.
int UDPClient::sendData(char* buffer, int len) {
	if (!default_destination_set) {
		printf("No default set.\n");
		return -1;
	}

	return sendto(clientSock, buffer, len, 0,
		(sockaddr*)&default_destination, default_destination_size);
}


//SEND_DATA: attempts to send data to the given host/port.
//	RETURNS: results of (private) sendData().
int UDPClient::sendData(char* buffer, int len, char* host, int port) {
	sockaddr_in destination;
	destination.sin_family = AF_INET;
	destination.sin_addr.s_addr = inet_addr(host);
	destination.sin_port = htons(port);

	return sendto(clientSock, buffer, len, 0, (sockaddr*)&destination, sizeof(destination));
}


//SEND_STRING: calculates the length of the string, and sends it
// over the socket using standard/default sendData() function.
//RETURNS results of sendData(buffer, len).
int UDPClient::sendString(char* string) {
	return sendData(string, strlen(string));
}


//SEND_STRING (to): calculates the length of the string, and sends it
// over the socket using specified destination sendData() function.
//RETURNS results of sendData(buffer, len, host, port).
int UDPClient::sendString(char* string, char* host, int port) {
	return sendData(string, strlen(string), host, port);
}


//SEND_INT: converts the given int into a char* and sends it
// over the socket using standard/default sendData() function.
//RETURNS results of sendData(buffer, len).
int UDPClient::sendInt(int i) {
	return sendData((char*)&i, sizeof(int));
}


//SEND_INT (to): calculates the length of the string, and sends it
// over the socket using specified destination sendData() function.
//RETURNS results of sendData(buffer, len, host, port).
int UDPClient::sendInt(int i, char* host, int port) {
	return sendData((char*)&i, sizeof(int), host, port);
}


//REPLY: sends the buffer to the last address that was read from.
// Results may be unpredictable if no message was read before using
// this function.
//RETURNS results of sendto
int UDPClient::reply(char* buffer, int len) {
	return sendto(clientSock, buffer, len, 0, (sockaddr*)&last_address,
		sizeof(last_address));
}


//RECEIVE_DATA: attempts to receive data on the bound PORT.
//		RETURNS -1 if failed to receive.
//Read socket data up to "len" size from socket into
// ELSE RETURNS length of data that was received (in bytes).
int UDPClient::receiveData(char* buffer, int len) {
	int received = 0;

	sockaddr_in source;
	int source_size = sizeof(source);

	if ((received = recvfrom(clientSock, buffer, len, 0,
		(sockaddr*)&source, &source_size)) == SOCKET_ERROR) {
		printf("ERROR #%d\n", WSAGetLastError());
		return SOCKET_ERROR;
	}

	last_address = source;
	return received;
	/*
	int received = 0;

	sockaddr_in source;
	int source_size = sizeof(source);

	for(int i=0; i<len; i += received){
	if((received = recvfrom(clientSock, buffer+i, len-i, 0,
	(sockaddr*) &source, &source_size))
	== SOCKET_ERROR){
	printf("ERROR #%d\n", WSAGetLastError());
	return SOCKET_ERROR;
	}
	}

	last_address = source;

	return received;
	*/
}


//RECEIVE_LINE: attempts to read one line (string) on the bound port.
//Read socket data up to a newline "\n" terminator.
//		 RETURNS NULL if failed to receive.
//	ELSE RETURNS the string read up to "\n" newline.
char* UDPClient::recvLine() {
	int len = 512;
	char* buffer;
	buffer = (char*)malloc(len);
	int i = 0;
	do {
		//if len is exceeded, extend array:
		if (i == len) {
			char* temp;
			len *= 2;
			temp = (char*)malloc(len);
			memcpy(temp, buffer, i);
			delete buffer;
			buffer = (char*)malloc(len);
			memcpy(buffer, temp, i);
		}
		int res = receiveData(buffer + i, 1);
		if (res != 1)
			return NULL;
	} while (buffer[i++] != '\n');

	char* result;
	result = (char*)malloc(i + 1);
	memcpy(result, buffer, i);
	result[i] = '\0';
	delete buffer;
	return result;
}


//RECEIVE_INT: attempts to read an integer on the bound port.
//Read socket data for 4 bytes (int) and returns the int.
//		 RETURNS NULL if failed to receive.
//	ELSE RETURNS the integer.
int UDPClient::recvInt() {
	char buffer[4];
	int result = receiveData(buffer, 4);
	if (result != 4)
		return NULL;
	else {
		result = 0;
		//NOTE: this does not take into account "endian" form
		result = (result << 8) + buffer[0];
		result = (result << 8) + buffer[1];
		result = (result << 8) + buffer[2];
		result = (result << 8) + buffer[3];
		return result;
	}
}

/*
char* UDPClient::getLocalAddr(){
struct sockaddr_in local;
int addrlen = sizeof(local);
if(getsockname(clientSock, (struct sockaddr*)&local, &addrlen) == 0 &&
local.sin_family == AF_INET &&
addrlen == sizeof(local)){
printf("RETURNING ADDR: %s: len = %d\n", inet_ntoa(local.sin_addr),
strlen(inet_ntoa(local.sin_addr)));
return inet_ntoa(local.sin_addr);
}
return "0.0.0.0";
}*/
char* UDPClient::getLocalAddr() {
	struct sockaddr_in sin;
	int addrlen = sizeof(sin);
	if (getsockname(clientSock, (struct sockaddr *)&sin, &addrlen) == 0 &&
		sin.sin_family == AF_INET &&
		addrlen == sizeof(sin)) {
		printf("RETURNING ADDR: %s: len = %d\n", inet_ntoa(sin.sin_addr),
			strlen(inet_ntoa(sin.sin_addr)));
		return inet_ntoa(sin.sin_addr);

		//printf("RETURNING PORT: %d\n", ntohs(sin.sin_port));
		//return "ok";
	}
	else
		return "0.0.0.1";
}

int UDPClient::getLocalPort() {
	struct sockaddr_in sin;
	int addrlen = sizeof(sin);
	if (getsockname(clientSock, (struct sockaddr *)&sin, &addrlen) == 0 &&
		sin.sin_family == AF_INET &&
		addrlen == sizeof(sin)) {
		printf("RETURNING PORT: %d\n", ntohs(sin.sin_port));
		return  ntohs(sin.sin_port);
	}
	else
		return -1;
}


#endif