#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TAM_PKT 8000;
#define false 0;
#define true 1;

unsigned int SenderState = 0;
unsigned int ReceiverState = 0;
unsigned int TIMEunit = 0;
double timeout = 0;
double time_time = 0;

_Bool receiverSend(int ack);
_Bool receiverReceive(int pkt);
_Bool senderSend();
_Bool senderReceive(int ack);
_Bool senderTimeoutResend(int pkt);

void sendPktPrint();
void sendAckPrint();
void sendPkttimeoutPrint();
void sendAckTimeoutPrint();
void TimeoutPrint();

int main() {

	int option;
	int d, x;
	int tamPKT = TAM_PKT;
	double distancia = 5000;/*Distancia em metros (5 Km)*/
	double tamArq = 8000000;/*Tamanho do arquivo em bits (1 MB)*/
	double taxa = 10000000;/*Taxa de transmissão em bits/s (10 Mb)*/
	unsigned int numPKTs;
	unsigned int i = 0;
	unsigned int numPktLost = 0;
	_Bool pktLost;
	double tempototal = 0;
	/*time_time = 1;*/
	time_time = (tamPKT/taxa)+(distancia/300000000);
	srand(time(NULL));

	numPKTs = tamArq / TAM_PKT;

	do{

		printf("\n\n --------- Trabalho de TR1 ----------- \n\n");
		printf("--> Menu <--\n\n");
		printf("1 - Simulacao Automatica.\n");
		printf("2 - Escolher Parametros.\n");
		printf("3 - Sair.\n");
		printf("\n");
		printf("Digite sua opcao : ");
		scanf("%d",&option);
		getchar();


		switch(option){
			case 1:
				while (i < numPKTs) {

					pktLost = senderSend();
					if (!pktLost) {
						i++;
					}else{
						numPktLost++;
					}

				}

				tempototal = TIMEunit*((tamPKT/taxa)+(distancia/300000000));

				printf("\n\nTempo total decorrido: %f segundos\n",tempototal);
				printf("Numero de pacotes: %d\n",numPKTs);
				printf("Numero de retransmissao de pacote: %d\n", numPktLost);
				getchar();
				break;

			case 2:
				printf("Digite o tamanho do arquivo a ser enviado (bits): ");
				scanf("%d",&x);
				getchar();
				printf("Digite a distancia desejada (metros): ");
				scanf("%d",&d);
				getchar();

				distancia = d;
				tamArq = x;
				numPKTs = tamArq / TAM_PKT;
				time_time = (tamPKT/taxa)+(distancia/300000000);

				while (i < numPKTs) {

					pktLost = senderSend();
					if (!pktLost) {
						i++;
					}else{
						numPktLost++;
					}

				}

				tempototal = TIMEunit*((tamPKT/taxa)+(distancia/300000000));

				printf("\n\nTempo total decorrido: %f segundos\n",tempototal);
				printf("Numero de pacotes: %d\n",numPKTs);
				printf("Numero de retransmissao de pacote: %d\n", numPktLost);
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

/*Receiver functions*/
_Bool receiverSend(int ack) {
	int lose = 1+(rand()%20);
	if(lose < 20){
		sendAckPrint();
		return senderReceive(ack);
	}else{
		timeout++;
		TIMEunit++;
		sendAckTimeoutPrint();
		TimeoutPrint();
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
				printf("Pacote duplicado\n");
				printf("Enviado ack 1\n");
				return receiverSend(1);
			}
		case 1:
			if (pkt == 1) {
				ReceiverState = 0;
				printf("Enviado ack 1\n");
				return receiverSend(1);
			} else {
				printf("Pacote duplicado\n");
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
	int lose = 1 + (rand()%20);
	if(lose < 20){
		switch (SenderState) {
		case 0:
			timeout = 0;
			printf("Enviado pkt 0\n");
			sendPktPrint();
			SenderState = 1;
			return receiverReceive(0);

		case 2:
			timeout = 0;
			printf("Enviado pkt 1\n");
			sendPktPrint();
			SenderState = 3;
			return receiverReceive(1);
		}
	}else{
		TIMEunit+=2;
		timeout+=2;
		sendPkttimeoutPrint();
		TimeoutPrint();
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

/*Apresentacao*/
void sendPktPrint(){
	printf("    SENDER   \t\t\t\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDERPKT\t\t\t\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\tPKT\t\t\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\tPKT\t\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\tPKT\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\t\tPKT\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\t\t\tPKT\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\t\t\t     PKTRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\t\t\t\tRECEIVER\tOK\n");
	sleep(time_time/9);
}

void sendAckPrint(){
	printf("    SENDER   \t\t\t\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\t\t\t     ACKRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\t\t\tACK\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\t\tACK\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\tACK\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\tACK\t\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\tACK\t\t\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDERACK\t\t\t\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("OK  SENDER   \t\t\t\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
}

void sendPkttimeoutPrint(){
	printf("    SENDER   \t\t\t\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDERPKT\t\t\t\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\tPKT\t\t\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\tPKT\t\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\t X \t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\t\t   \t\tRECEIVER\t\n");
	sleep((13*time_time)/9);
}

void sendAckTimeoutPrint(){
	printf("    SENDER   \t\t\t\t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\t\t\t     ACKRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\t\t\tACK\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\t\tACK\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t\t X \t\t\tRECEIVER\t\n");
	sleep(time_time/9);
	printf("    SENDER\t\t   \t\t\t\tRECEIVER\t\n");
	sleep((4*time_time)/9);
}

void TimeoutPrint(){
	printf("\t\t\tTIMEOUT\t\t\t              \t\n");
}
