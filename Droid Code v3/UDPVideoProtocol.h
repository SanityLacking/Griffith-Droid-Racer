//core UDP network system:
#include "UDPClient.h"

#define FRAME_SEGS_WIDTH 5 //22
#define FRAME_SEGS_HEIGHT 5 //22
#define FRAME_SEGS_TOTAL 25 //484

#define CURRENT_PROTOCOL_VERSION 1

#define FRAME_NUMBER_SIZE 1
#define INDEX_NUMBER_SIZE 2
#define PACKET_SIZE 2
#define VIDEO_HEADER_SIZE 5


#define PACKET_SIZE 1024
#define HEADER_SIZE 5

struct header{
	char frame_number;
	short position_index;
	short data_length;
};

struct admin{
	int width;
	int height;
};


class UDPVideoProtocol: public UDPClient{
public:
	//client bind:
	UDPVideoProtocol(char* host_addr, int host_port):
	  UDPClient(host_addr, host_port){};
	//server bind:
	UDPVideoProtocol(int local_port):
	  UDPClient(local_port){};

	//admininstartive data:
	int sendAdminInfo(admin *ad);
	int recvAdminInfo(admin *ad);

	//send video segment:
	int sendSegment(int frame_num, int segment_pos, char *data, int len);

	//receive video segment:
	int receiveHeader(header *head);
	int receiveSegment(header *head, char *data);
};


int UDPVideoProtocol::sendAdminInfo(admin *ad){
	char *buff = (char*)malloc(8);
	memcpy(&buff[0], &ad->width, 4);
	memcpy(&buff[4], &ad->height, 4);
	return sendData(buff, 8);
}

int UDPVideoProtocol::recvAdminInfo(admin *ad){
	char *buff = (char*)malloc(8);
	int result = receiveData(buff, 8);
	if(result >= 0){
		memcpy(&ad->width, &buff[0], 4);
		memcpy(&ad->height, &buff[4], 4);
	}
	return result;
}

int UDPVideoProtocol::sendSegment(int frame_num, int segment_pos, char *data, int len){
	if(len > (PACKET_SIZE - HEADER_SIZE))
		return -1;
	
	char *buff = (char*)malloc(PACKET_SIZE);
	buff[0] = (char)frame_num;
	memcpy(&buff[1], &segment_pos, 2);
	memcpy(&buff[3], &len, 2);
	memcpy(&buff[5], &data[0], len);
	int result = sendData(buff, PACKET_SIZE);
	delete buff;
	return result;
}


/* RETURNS -1 if failed, else
 * RETURNS the segment number.
 */
int UDPVideoProtocol::receiveSegment(header *head, char *buff){
	int result = receiveData(buff, PACKET_SIZE);
	if(result < 0)
		return -1;

	head->frame_number = buff[0];
	memcpy(&head->position_index, &buff[1], 2);
	memcpy(&head->data_length, &buff[3], 2);

	return PACKET_SIZE;
}

















/*
//READ HEADER (and apply proper read afterwards)
int UDPVideoProtocol::receiveHeader(header *head){
	char *buff = (char*)malloc(HEADER_SIZE);
	int result = receiveData(buff, HEADER_SIZE);
	if(result < 0){
		return -1;
	}
	
	head->frame_number = buff[0];
	memcpy(&head->position_index, &buff[1], 2);
	memcpy(&head->data_length, &buff[3], 2);
	//head->packet_num = buff[5];
	//head->packet_max = buff[6];

	return result;
}*/

	/*
	int result;
	int packet = 0;
	int maxPackets = len / MAX_SIZE;
	if(len % MAX_SIZE != 0) maxPackets++;
	while(packet < maxPackets){
		packet++;
		int bufsz;
		if(len <= MAX_SIZE){
			bufsz = len;
		}else{
			bufsz = MAX_SIZE;
		}
		char *buff = (char*)malloc(HEADER_SIZE + bufsz);
		
		buff[0] = (char)frame_num; //char frame_number;
		memcpy(&buff[1], &segment_pos, 2); //short position_index;
		memcpy(&buff[3], &len, 2); //short data_length;
		buff[5] = (char)packet; //char packet_num;
		buff[6] = (char)maxPackets; //char packet_max;

		len -= MAX_SIZE;

		result = sendData(buff, bufsz+HEADER_SIZE);
		if(result < 0)
			return -1;

		delete buff;
	}

	return len;
	*/

	/*
	do{
		int bufsz;
		if(len <= MAX_SIZE){
			bufsz = len;
		}else{
			bufsz = MAX_SIZE;
		}
		char *buff = (char*)malloc(HEADER_SIZE + bufsz);
		
		buff[0] = (char)frame_num; //char frame_number;
		memcpy(&buff[1], &segment_pos, 2); //short position_index;
		memcpy(&buff[3], &len, 2); //short data_length;
		buff[5] = (char)packet; //char packet_num;
		buff[6] = (char)2; //char packet_max;

		len -= MAX_SIZE;
	}while(len > MAX_SIZE);


	*/