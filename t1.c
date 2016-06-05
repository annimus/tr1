#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_PKT 8192;
#define false 0;
#define true 1;

unsigned int SenderState = 0;
unsigned int ReceiverState = 0;
unsigned int TIMEunit = 0;
unsigned int timeout = 0;

_Bool receiverSend(int ack);
_Bool receiverReceive(int pkt);
_Bool senderSend();
_Bool senderReceive(int ack);
_Bool senderTimeoutResend(int pkt);
void limparVariaveis();

int main() {

	/*int pkt;
	int ack; */
	int option;
	int d, x;
	int distancia = 5000;
	int tamArq = 8000000;
	int numPKTs;
	int taxa = 10000000;
	int i = 0;
	int numPktLost = 0;
	_Bool pktLost;
	srand(time(NULL));

	numPKTs = tamArq / TAM_PKT;

	do{

		printf("\n\n --------- Trabalho de TR1 ----------- \n\n");
		printf("--> Menu <--\n\n");
		printf("1 - Simulacao Automatica.\n");
		printf("2 - Escolher Parametros.\n");
		printf("3 - Sair.\n");
		printf("\n");
		printf("Digite sua opcao :\n");
		scanf("%d",&option);

	
		switch(option){
			case 1:
				while (i < numPKTs) {

					pktLost = senderSend();
					if (!pktLost) {
						numPktLost++;
						i++;
					}

				}

				printf("\n\nTempo total decorrido: %d unidades de tempo\n",TIMEunit);
				printf("Numero de pacotes perdidos: %d\n", numPktLost);
				getchar();
				getchar();
				break;

			case 2: 
				printf("Digite o tamanho do arquivo a ser enviado (bits).\n");
				scanf("%d",&x);
				printf("Digite a distancia desejada (metros).\n");
				scanf("%d",&d);

				distancia = d;
				tamArq = x;
				numPKTs = tamArq / TAM_PKT;

				while (i < numPKTs) {

					pktLost = senderSend();
					if (!pktLost) {
						numPktLost++;
						i++;
					}

				}

				printf("\n\nTempo total decorrido: %d unidades de tempo\n",TIMEunit);
				printf("Numero de pacotes perdidos: %d\n", numPktLost);
				getchar();
				getchar();

				break;
		}
		
		SenderState = 0;
		ReceiverState = 0;
		TIMEunit = 0;
		timeout = 0;
		pktLost = true;
		numPktLost = 0;
		i = 0;
		d = 0;
		x = 0;
		
		system ("clear");
	}while (option != 3);


	return 0;
}

void limparVariaveis() {
	
}

/*Receiver functions*/
_Bool receiverSend(int ack) {
	int lose = 1+(rand()%10);
	if(lose < 9){
		return senderReceive(ack);
	}else{
		timeout++;
		TIMEunit++;
		printf("ACK perdido\n");
		if(ack == 0){
			SenderState = 0;
		}else if (ack == 1){
			SenderState = 2;
		}
		return true;
	}
}

_Bool receiverReceive(int pkt) {
	TIMEunit++;
	timeout++;
	if(timeout <= 2){
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
	}else{
		return senderTimeoutResend(pkt);
	}
	return true;
}

/*Sender functions*/
_Bool senderSend() {
	int lose = 1 + (rand()%10);
	if(lose < 9){
		switch (SenderState) {
		case 0:
			timeout = 0;
			printf("Enviado pkt 0\n");
			SenderState = 1;
			return receiverReceive(0);

		case 2:
			timeout = 0;
			printf("Enviado pkt 1\n");
			SenderState = 3;
			return receiverReceive(1);
		}
	}else{
		TIMEunit+=2;
		timeout+=2;
		printf("Pacote perdido\n");
	}
	return true;
}

_Bool senderTimeoutResend(int pkt){
	if(pkt==0){
		SenderState = 0;
		return senderSend();
	}else if(pkt == 1){
		SenderState = 2;
		return senderSend();
	}
	return true;
}

_Bool senderReceive(int ack) {
	TIMEunit++;
	timeout++;
	if(timeout <= 2){
		switch (SenderState) {
		case 1:
			SenderState = 2;
			if (ack == 0) {
				printf("Recebeu ack 0\n");
				return false;
			}
			printf("ERROR: PKT perdido\n");
			return true;
		case 3:
			SenderState = 0;
			if (ack == 1) {
				printf("Recebeu ack 1\n");
				return false;
			}
			printf("ERROR: PKT perdido\n");
			return true;
		}
	}else{
		return senderTimeoutResend(ack);
	}
	return true;
}

