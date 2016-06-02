#include <stdio.h>

#define TAM_PKT = x;

int SenderState = 0;
int ReceiverState = 0;

int main() {
	int distancia;
	int tamArq;
	int numPKTs;
	int taxa;

	numPKTs = tamArq/TAM_PKT;
	//

	return 0;
}


//Receiver functions
void receiverSend(int ack) {
	senderReceive(ack);
}

void receiverReceive(int pkt) {
	switch(ReceiveState) {
		case 0:
			if(pkt == 0) {
				receiverSend(0);
				ReceiverState = 1;
				printf("Enviado ack 0\n");
			} else {
				receiverSend(1);
				printf("Enviado ack 1\n");
			}
			break;
		case 1:
			if(pkt == 1) {
				receiverSend(1);
				ReceiverState = 0;
				printf("Enviado ack 1\n");
			} else {
				receiverSend(0);
				printf("Enviado ack 0\n");
			}
			break;
	}
}

//Sender functions
void senderSend(int pkt) {
	switch(SenderState) {
		case 0:
			receiverReceive(pkt);
			printf("Enviado pkt 0\n");
			SenderState = 1;
			break;

		case 2:
			receiverReceive(pkt);
			printf("Enviado pkt 1\n");
			SenderState = 3;
			break;
	}	
}

void senderReceive(int ack) {
	switch(SenderState) {
		case 1:
			if(ack == 0) {
				SenderState = 2;
				printf("Recebeu ack 0\n");
			}
			break;
		case 3:
			if(ack ==1) {
				SenderState = 0;
				printf("Recebeu ack 1\n");
			}
			break;	
	}
}
