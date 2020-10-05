#include <iostream>
#include <windows.h>
#include <time.h>
#include <vector>
#include <string>

//DWORD WINAPI CatchProcessData(std::string listaMensagens[200]) {
//
//	int currentIndex = 0;
//	std::string Lista[200];
//
//	struct mensagemTipo1 {
//		long nseq;
//		int tipo;
//		int cadeira;
//		int gravidade;
//		int classe;
//		std::string idFoto;
//		SYSTEMTIME tempo;
//	};
//
//	struct mensagemTipo2 {
//		long nseq;
//		int tipo;
//		int cadeira;
//		std::string id;
//		float temp;
//		float vel;
//		SYSTEMTIME tempo;
//	};
//
//	while (true)
//	{
//		clock_t tick;
//		double tempo = 0;
//		int maior = 2000;
//		int menor = 100;
//		double reference = (double)(rand() % (maior - menor + 1) + menor);
//		tick = clock();
//		int a = 0;
//		int b = 0;
//		char error_message[36];
//		char data_message[45];
//		struct mensagemTipo1 mensagemTipo1;
//		struct mensagemTipo2 mensagemTipo2;
//		int currentNSEQTipo1 = 1;
//		int currentNSEQTipo2 = 1;
//		std::string msg1;
//		std::string msg2;
//		//char mensagemTipo1[] = "[NNNNN][11][NN][N][AAAAAA] ";
//		//char mensagemTipo2[] = "[NNNNN][22][NNNN][AAAAAAAAA][NNN.N][NNN.N] ";
//		
//		SYSTEMTIME  now;
//
//		std::cout << '\n' << "Thread1";
//		while (true) {
//			tempo = (clock() - tick)* 1000/ CLOCKS_PER_SEC;
//			if(tempo >= reference) {
//						GetLocalTime(&now);
//
//						mensagemTipo1.nseq = currentNSEQTipo1;
//						mensagemTipo1.tipo = 11;
//						mensagemTipo1.cadeira = rand() % 7;
//						mensagemTipo1.gravidade = rand() % 99;
//						mensagemTipo1.classe = rand() % 10;
//						mensagemTipo1.idFoto = "ABC88F";
//						mensagemTipo1.tempo = now;
//
//						msg1 = std::to_string(mensagemTipo1.nseq) + "/" + std::to_string(mensagemTipo1.tipo) + "/" + std::to_string(mensagemTipo1.cadeira) + "/" + std::to_string(mensagemTipo1.gravidade) + "/" + std::to_string(mensagemTipo1.classe) + "/" + mensagemTipo1.idFoto + "/" + std::to_string(mensagemTipo1.tempo.wHour) + ":" + std::to_string(mensagemTipo1.tempo.wMinute) + ":" + std::to_string(mensagemTipo1.tempo.wSecond) + ":" + std::to_string(mensagemTipo1.tempo.wMilliseconds);
//						Lista[a] = msg1;
//						listaMensagens[a] = msg1;
//						std::cout << '\n' << msg1;
//						a++;
//						currentNSEQTipo1++;
//						reference = (double)(rand() % (maior - menor + 1) + menor);
//						tick = clock();
//			}
//			if (tempo == 500) {
//				GetLocalTime(&now);
//				mensagemTipo2.nseq = currentNSEQTipo2;
//				mensagemTipo2.tipo = 22;
//				mensagemTipo2.cadeira = rand() % 7;
//				mensagemTipo2.id = "E4";
//				mensagemTipo2.temp = 2700.4;
//				mensagemTipo2.vel = 4000.7;
//				mensagemTipo2.tempo = now;
//				
//				msg2 = std::to_string(mensagemTipo2.nseq) + "/" + std::to_string(mensagemTipo2.tipo) + "/" + std::to_string(mensagemTipo2.cadeira) + "/" + mensagemTipo2.id + "/" + std::to_string(mensagemTipo2.temp) + "/" + std::to_string(mensagemTipo2.vel) + "/" + std::to_string(mensagemTipo2.tempo.wHour) + ":" + std::to_string(mensagemTipo2.tempo.wMinute) + ":" + std::to_string(mensagemTipo2.tempo.wSecond) + ":" + std::to_string(mensagemTipo2.tempo.wMilliseconds);
//				listaMensagens[a] = msg2;
//				//std::cout << '\n' << mensagemTipo2 << a << " Tempo: " << tempo << " ";
//				std::cout << '\n' <<msg2;
//				a++;
//				currentNSEQTipo2++;
//
//			}
//			/*if (*listaMensagens->size() == 200) {
//				std::cout << "a lista esta cheia";
//				system("pause");
//			}*/
//		}
//				
//	}
//	return 0;
//}