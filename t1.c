#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_PKT = 100;

int SenderState = 0;
int ReceiverState = 0;

int main() {
	int distancia;
	int tamArq = 200;
	int numPKTs;
	int taxa;
	int i = 0;
	bool pktLost;
	srand(unsigned time(null))

	numPKTs = tamArq/TAM_PKT;
	
	while(i<tamArq){

		pktLost = senderSend();
		if (!pktLost)
		{
			i++;
		}

	}

	return 0;
}


//Receiver functions
bool receiverSend(int ack) {
	return senderReceive(ack);
}

bool receiverReceive(int pkt) {
	switch(ReceiveState) {
		case 0:
			if(pkt == 0) {
				ReceiverState = 1;
				printf("Enviado ack 0\n");
				return receiverSend(0);
			} else {
				printf("Enviado ack 1\n");
				return receiverSend(1);
			}
		case 1:
			if(pkt == 1) {
				ReceiverState = 0;
				printf("Enviado ack 1\n");
				return receiverSend(1);
			} else {
				printf("Enviado ack 0\n");
				return receiverSend(0);
			}
	}
}

//Sender functions
bool senderSend(int pkt) {
	switch(SenderState) {
		case 0:
			
			printf("Enviado pkt 0\n");
			SenderState = 1;
			return receiverReceive(pkt);

		case 2:
			
			printf("Enviado pkt 1\n");
			SenderState = 3;
			return receiverReceive(pkt);
	}	
}

bool senderReceive(int ack) {
	switch(SenderState) {
		case 1:
			if(ack == 0) {
				SenderState = 2;
				printf("Recebeu ack 0\n");
				return false;
			}
			printf("PKT perdido\n");
			return true;
		case 3:
			if(ack ==1) {
				SenderState = 0;
				printf("Recebeu ack 1\n");
				return false;
			}
			printf("PKT perdido\n");
			return true;
	}
}
