#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_PKT 100;
#define false 0;
#define true 1;

int SenderState = 0;
int ReceiverState = 0;

_Bool receiverSend(int ack);
_Bool receiverReceive(int pkt);
_Bool senderSend();
_Bool senderReceive(int ack);

int main() {
	//int distancia;
	int tamArq = 300;
	int numPKTs;
	//int taxa;
	int i = 0;
	_Bool pktLost;
	srand(time(NULL));

	numPKTs = tamArq / TAM_PKT
	;

	while (i < numPKTs) {

		pktLost = senderSend();
		if (!pktLost) {
			i++;
		}

	}

	return 0;
}

//Receiver functions
_Bool receiverSend(int ack) {
	return senderReceive(ack);
}

_Bool receiverReceive(int pkt) {
	switch (ReceiverState) {
	case 0:
		if (pkt == 0) {
			ReceiverState = 1;
			printf("Enviado ack 0\n");
			return receiverSend(0);
		} else {
			printf("Enviado ack 1\n");
			return receiverSend(1);
		}
	case 1:
		if (pkt == 1) {
			ReceiverState = 0;
			printf("Enviado ack 1\n");
			return receiverSend(1);
		} else {
			printf("Enviado ack 0\n");
			return receiverSend(0);
		}
	}
	return false;
}

//Sender functions
_Bool senderSend() {
	switch (SenderState) {
	case 0:

		printf("Enviado pkt 0\n");
		SenderState = 1;
		return receiverReceive(0);

	case 2:

		printf("Enviado pkt 1\n");
		SenderState = 3;
		return receiverReceive(1);
	}
	return false;
}

_Bool senderReceive(int ack) {
	switch (SenderState) {
	case 1:
		if (ack == 0) {
			SenderState = 2;
			printf("Recebeu ack 0\n");
			return false;

		}
		printf("PKT perdido\n");
		return true;
	case 3:
		if (ack == 1) {
			SenderState = 0;
			printf("Recebeu ack 1\n");
			return false;
		}
		printf("PKT perdido\n");
		return true;
	}
	return false;
}

