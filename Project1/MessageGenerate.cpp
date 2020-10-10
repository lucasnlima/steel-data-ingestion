#include <string>
#include <Windows.h>
#include <iostream>
#include <string>


using namespace std;
struct MESSAGE_TYPE_1 {
	char nseq[5];
	char tipo[2];
	char caldeira[2];
	char gravidade[2];
	char classe[1];
	char idFoto[6];
	SYSTEMTIME tempo;
};

struct MESSAGE_TYPE_2 {
	char nseq[5];
	char tipo[2];
	char caldeira[2];
	char id[8];
	char tempInt[3];
	char tempDec[1];
	char velInt[3];
	char velDec[1];
	SYSTEMTIME tempo;
};

SYSTEMTIME  now;

char* GenerateAlphanumeric() {
	char alphanumeric[10];
	int aux;

	for (int i = 0; i<10; i++) {
		aux = rand() % 2;
		if (aux) {
			alphanumeric[i] = (char)48 + rand() % 10;
		}
		else {
			alphanumeric[i] = (char)65 + rand() % 26;
		}
	}
	return alphanumeric;
}

char* GenerateNumbers() {
	char numbers[10];

	for (int i = 0; i < 10; i++) {
		numbers[i] = (char)48 + rand() % 10;
	}
	return numbers;
}

char* GenerateMessageType1(long index) {

	GetLocalTime(&now);
	char* mensagem;
	mensagem = (char*)malloc(36);
	ZeroMemory(mensagem, sizeof(mensagem));
	struct MESSAGE_TYPE_1 msg;

	const string barra = "/";
	sprintf(msg.nseq,"%05d", index);
	memcpy(msg.tipo, "11", sizeof(msg.tipo));
	sprintf(msg.caldeira,"%02d",rand()%7);
	sprintf(msg.gravidade,"%02d",rand()%100);
	sprintf(msg.classe,"%1d",rand()%10);
	memcpy(msg.idFoto, GenerateAlphanumeric(),sizeof(msg.idFoto));

	strncat(mensagem, msg.nseq,5);
	strncat(mensagem, "/", 1);
	strncat(mensagem, msg.tipo, 2);
	strncat(mensagem, "/", 1);
	strncat(mensagem, msg.caldeira, 2);
	strncat(mensagem, "/", 1);
	strncat(mensagem, msg.gravidade, 2);
	strncat(mensagem, "/", 1);
	strncat(mensagem, msg.classe, 1);
	strncat(mensagem, "/", 1);
	strncat(mensagem, msg.idFoto, 6);
	strncat(mensagem, "/", 1);
	strncat(mensagem, to_string(now.wHour).c_str(), 2);
	strncat(mensagem, ":", 1);
	strncat(mensagem, to_string(now.wMinute).c_str(), 2);
	strncat(mensagem, ":", 1);
	strncat(mensagem, to_string(now.wSecond).c_str(), 2);
	strncat(mensagem, ":", 1);
	strncat(mensagem, to_string(now.wMilliseconds).c_str(), 3);
	
	return (char*)mensagem;
}

char* GenerateMessageType2(long index) {


	GetLocalTime(&now);
	char* mensagem;
	mensagem = (char*)malloc(36);
	ZeroMemory(mensagem, sizeof(mensagem));
	struct MESSAGE_TYPE_2 msg;

	sprintf(msg.nseq, "%05d", index);
	memcpy(msg.tipo, "22", sizeof(msg.tipo));
	sprintf(msg.caldeira, "%02d", rand() % 7);
	memcpy(msg.id, GenerateAlphanumeric(), sizeof(msg.id));
	sprintf(msg.tempInt, "%03d", (rand() % 1000));
	sprintf(msg.tempDec, "%01d", (rand() % 100));
	sprintf(msg.velInt, "%03d", (rand() % 1000));
	sprintf(msg.velDec, "%01d", (rand() % 100));

	strncat(mensagem, msg.nseq, 5);
	strncat(mensagem, "/", 1);
	strncat(mensagem, msg.tipo, 2);
	strncat(mensagem, "/", 1);
	strncat(mensagem, msg.caldeira, 2);
	strncat(mensagem, "/", 1);
	strncat(mensagem, msg.id, 8);
	strncat(mensagem, "/", 1);
	strncat(mensagem, msg.tempInt, 3);
	strncat(mensagem, ".", 1);
	strncat(mensagem, msg.tempDec, 1);
	strncat(mensagem, "/", 1);
	strncat(mensagem, msg.velInt, 3);
	strncat(mensagem, ".", 1);
	strncat(mensagem, msg.velDec, 1);
	strncat(mensagem, "/", 1);
	strncat(mensagem, to_string(now.wHour).c_str(), 2);
	strncat(mensagem, ":", 1);
	strncat(mensagem, to_string(now.wMinute).c_str(), 2);
	strncat(mensagem, ":", 1);
	strncat(mensagem, to_string(now.wSecond).c_str(), 2);
	strncat(mensagem, ":", 1);
	strncat(mensagem, to_string(now.wMilliseconds).c_str(), 3);

	return (char*)mensagem;
}