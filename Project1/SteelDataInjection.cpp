#include <iostream>
#include <stdio.h>
#include <process.h>
#include <windows.h>
#include <vector>
#include <time.h>
#include <string>
#include "CheckForError.h"
#include "CatchProcessData.h"
#include "MessageGenerate.h"

#define MAX_POSICOES 200


typedef unsigned (WINAPI* CAST_FUNCTION)(LPVOID);

using namespace std;

// Definindo a lista circular como ma
string listaMensagens[200];
long currentIndex = 0;

HANDLE hSemaphoreLVazia;
HANDLE hSemaphoreLCheia;

DWORD WINAPI CatchProcessData();
DWORD WINAPI CapturaDeMensagensTipo11();
DWORD WINAPI CapturaDeMensagensTipo22();

int main(char args[]) {
	STARTUPINFOA siDataDisplay;				    // StartUpInformation para novo processo
	PROCESS_INFORMATION ProcessDataDisplay;	// Informações sobre novo processo criado

	STARTUPINFOA siDefectDisplay;
	PROCESS_INFORMATION StripDefectDisplay;
	
	
	ZeroMemory(&siDataDisplay, sizeof(siDataDisplay));
	siDataDisplay.cb = sizeof(siDataDisplay);
	ZeroMemory(&ProcessDataDisplay, sizeof(ProcessDataDisplay));


	ZeroMemory(&siDefectDisplay, sizeof(siDefectDisplay));
	siDefectDisplay.cb = sizeof(siDefectDisplay);
	ZeroMemory(&StripDefectDisplay, sizeof(StripDefectDisplay));


	if (!CreateProcessA("D:\\Users\\beatr\\Documents\\Faculdade\\6 Periodo\\Automação em Tempo Real\\steel-data-ingestion\\Release\\ProcessDataDisplay.exe", NULL, NULL, NULL,FALSE, CREATE_NEW_CONSOLE, NULL,NULL, &siDataDisplay, &ProcessDataDisplay)) {
		printf("CreateProcess failed (%d).\n", GetLastError());
	};

	if (!CreateProcessA("D:\\Users\\beatr\\Documents\\Faculdade\\6 Periodo\\Automação em Tempo Real\\steel-data-ingestion\\Release\\StreepDefectDisplay.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &siDefectDisplay, &StripDefectDisplay)) {
		printf("CreateProcess failed (%d).\n", GetLastError());
	};


	unsigned int Thread1Id;
	unsigned int Thread2Id;
	unsigned int Thread3Id;


	hSemaphoreLVazia = CreateSemaphore(NULL, MAX_POSICOES, MAX_POSICOES, (LPCWSTR)"SemListaVazia");
	hSemaphoreLCheia = CreateSemaphore(NULL, 0, MAX_POSICOES, (LPCWSTR)"SemListaCheia");

	HANDLE h_Thread1 = (HANDLE)_beginthreadex(
		NULL,
		0,
		(CAST_FUNCTION) CatchProcessData,	// casting necessário
		NULL,
		0,
		&Thread1Id
	);

	HANDLE h_thread2 = (HANDLE)_beginthreadex(
		NULL,
		0,
		(CAST_FUNCTION)CapturaDeMensagensTipo11,
		NULL,
		0,
		&Thread2Id

	);

	//HANDLE h_thread3 = (HANDLE)_beginthreadex(
	//	NULL,
	//	0,
	//	(CAST_FUNCTION)CapturaDeMensagensTipo22,
	//	NULL,
	//	0,
	//	&Thread3Id
	//);

	char a = 0;
	cout << "Main task";
	while (a!= 13) {
	cin >> a;
	}


	return 0;
}

DWORD WINAPI CapturaDeMensagensTipo11() {
	while (true) {
		for (int i = 0; i < 200; i++) {
			WaitForSingleObject(hSemaphoreLCheia, INFINITE);
				if (listaMensagens[i].find("/11/")) {						
					cout << "mensagem tipo 11 encontrada. [MENSAGEM]: " << listaMensagens[i] << endl;
					//Função do processo de display que exibe na tela a
					//string
					listaMensagens[i] = "";	
					ReleaseSemaphore(hSemaphoreLVazia, 1, NULL);
				}				
			}		
	}
	return 0;
}

DWORD WINAPI CapturaDeMensagensTipo22() {
	while (true) {
		for (int i = 0; i < 200; i++) {
			WaitForSingleObject(hSemaphoreLCheia, INFINITE);
			if (listaMensagens[i].find("/22/")) {
				
				cout << "mensagem tipo 22 encontrada. [MENSAGEM]: " << listaMensagens[i] << endl;
				listaMensagens[i] = "";
				ReleaseSemaphore(hSemaphoreLVazia, 1, NULL);
			}
			
		}
	}
	return 0;
}

DWORD WINAPI CatchProcessData() {

	struct itemLista {
		string mensagem;
		struct itemLista* next;
	};

	while (true)
	{
		clock_t tick;
		double tempo = 0;
		int maior = 2000;
		int menor = 100;
		double reference = (double)(rand() % (maior - menor + 1) + menor);
		tick = clock();
		int a = 0;
		int currentNSEQTipo1 = 1;
		int currentNSEQTipo2 = 1;
		std::string msg1;
		std::string msg2;

		std::cout << '\n' << "Thread1";
		while (true) {
			tempo = (clock() - tick) * 1000 / CLOCKS_PER_SEC;
			if (tempo >= reference) {

				msg1 = GenerateMessageType1(currentNSEQTipo1);
				WaitForSingleObject(hSemaphoreLVazia, INFINITE);
				listaMensagens[currentIndex] = msg1;
				std::cout << '\n' << msg1;				
				currentNSEQTipo1++;
				currentIndex++;
				if (currentIndex == 200) currentIndex = 0;
				ReleaseSemaphore(hSemaphoreLCheia, NULL, NULL);
				reference = (double)(rand() % (maior - menor + 1) + menor);
				tick = clock();
			}
			if (tempo == 500) {

				msg2 = GenerateMessageType2(currentNSEQTipo2);
					
				listaMensagens[currentIndex] = msg2;

				std::cout << '\n' << msg2;
				currentNSEQTipo2++;
				
				currentIndex++;
				if (currentIndex == 200) currentIndex = 0;

			}
			if (listaMensagens->size() == 200) {
				std::cout << "a lista esta cheia";
				system("pause");
			}
		}

	}
}