#include <string>
#include <Windows.h>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS

struct MESSAGE_TYPE_1 {
	long nseq  = 0;
	int tipo = 0;
	int caldeira = 0;
	int gravidade = 0;
	int classe = 0;
	std::string idFoto;
	SYSTEMTIME tempo;
};

struct MESSAGE_TYPE_2 {
	long nseq = 0;
	int tipo = 0;
	int caldeira = 0;
	std::string id;
	float temp = 0;
	float vel = 0;
	SYSTEMTIME tempo;
};

SYSTEMTIME  now;

char* GenerateMessageType1(long index) {

	GetLocalTime(&now);

	struct MESSAGE_TYPE_1 msg;
	
	msg.nseq = index;
	msg.tipo = 11;
	msg.caldeira = rand() % 7;
	msg.gravidade = rand() % 99;
	msg.classe = rand() % 10;
	msg.idFoto = "ABC88F";
	msg.tempo = now;

	std::string msg_str = std::to_string(msg.nseq) + "/" +
		std::to_string(msg.tipo) + "/" + std::to_string(msg.caldeira) +
		"/" + std::to_string(msg.gravidade) + "/" +
		std::to_string(msg.classe) + "/" +
		msg.idFoto + "/" +
		std::to_string(msg.tempo.wHour) +
		":" + std::to_string(msg.tempo.wMinute) +
		":" + std::to_string(msg.tempo.wSecond) +
		":" + std::to_string(msg.tempo.wMilliseconds);

		char* ch_prt = (char*)calloc(msg_str.length() + 1, sizeof(char*));
		strcpy(ch_prt, msg_str.c_str());

	return ch_prt;
}

char* GenerateMessageType2(long index) {

	GetLocalTime(&now);

	struct MESSAGE_TYPE_2 msg;

	msg.nseq = index;
	msg.tipo = 22;
	msg.caldeira = rand() % 7;
	msg.id = "E4";
	msg.temp = 2700.4;
	msg.vel = 4000.7;
	msg.tempo = now;

	std::string msg_str = std::to_string(msg.nseq) +
		"/" + std::to_string(msg.tipo) + "/" + std::to_string(msg.caldeira) +
		"/" + msg.id + "/" +
		"/" + std::to_string(msg.temp) +
		"/" + std::to_string(msg.vel) +
		"/" + std::to_string(msg.tempo.wHour) +
		":" + std::to_string(msg.tempo.wMinute) +
		":" + std::to_string(msg.tempo.wSecond) +
		":" + std::to_string(msg.tempo.wMilliseconds);

	char* ch_prt = (char*)calloc(msg_str.length() + 1, sizeof(char*));
	std::strcpy(ch_prt, msg_str.c_str());

	return ch_prt;
}