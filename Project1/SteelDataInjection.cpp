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
int i,j;
LONG semCount;
LONG semCount11;
LONG semCount22;

HANDLE hSemaphoreLVazia;
HANDLE hSemaphoreLCheiaTipo11;
HANDLE hSemaphoreLCheiaTipo22;

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
	hSemaphoreLCheiaTipo11 = CreateSemaphore(NULL, 0, MAX_POSICOES, (LPCWSTR)"SemListaCheia11");
	hSemaphoreLCheiaTipo22 = CreateSemaphore(NULL, 0, MAX_POSICOES, (LPCWSTR)"SemListaCheia22");

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

	HANDLE h_thread3 = (HANDLE)_beginthreadex(
		NULL,
		0,
		(CAST_FUNCTION)CapturaDeMensagensTipo22,
		NULL,
		0,
		&Thread3Id
	);

	char a = 0;
	std::cout << "Main task";
	while (a!= 13) {
	cin >> a;
	}


	return 0;
}

DWORD WINAPI CapturaDeMensagensTipo11() {
	while (true) {
		for (i = 0; i < 200; i++) {
			WaitForSingleObject(hSemaphoreLCheiaTipo11, INFINITE);
			if (listaMensagens[i].find("\/11\/", 0) != string::npos) {
				std::cout << "\n[MENSAGEM TIPO 11]: " << listaMensagens[i] << endl;
				//Função do processo de display que exibe na tela a
				//string
				listaMensagens[i] = "";
				ReleaseSemaphore(hSemaphoreLVazia, 1, &semCount);
				std::cout << "[QTD PREENCHIDA] >> " << semCount << endl;
			}
		}
		return 0;
	}
}

	// criar um mutex pros consumidores
DWORD WINAPI CapturaDeMensagensTipo22() {
	while (true) {		
		for (j = 0; j < 200; j++) {
			WaitForSingleObject(hSemaphoreLCheiaTipo22, INFINITE);
			if (listaMensagens[j].find("\/22\/", 0) != string::npos) {

				cout << "\n[MENSAGEM TIPO 22]: " << listaMensagens[j] << endl;
				listaMensagens[j] = "";
				ReleaseSemaphore(hSemaphoreLVazia, 1, &semCount);
				cout << "[QTD PREENCHIDA] >> " << semCount << endl;
				cout << "[QTD PREENCHIDA22] >> " << semCount22 << endl;
			}
		}
		return 0;

	}
}

DWORD WINAPI CatchProcessData() {

	struct itemLista {
		string mensagem;
		struct itemLista* next;
	};

	while (true)
	{
	
		int a = 0;
		int currentNSEQTipo1 = 1;
		int currentNSEQTipo2 = 1;
		std::string msg1;
		std::string msg2;

		std::cout << '\n' << "Thread1";
		while (true) {
				msg2 = GenerateMessageType2(currentNSEQTipo2);		
				listaMensagens[currentIndex] = msg2;
				std::cout << '\n' << msg2;
				currentNSEQTipo2++;				
				currentIndex++;
				if (currentIndex == 200) cout << "lista cheia";
				if (currentIndex == 200) currentIndex = 0;
				ReleaseSemaphore(hSemaphoreLCheiaTipo22, 1, &semCount22);

				msg1 = GenerateMessageType1(currentNSEQTipo1);
				WaitForSingleObject(hSemaphoreLVazia, INFINITE);
				listaMensagens[currentIndex] = msg1;
				std::cout << '\n' << msg1;				
				currentNSEQTipo1++;
				currentIndex++;
				if (currentIndex == 200) currentIndex = 0;
				ReleaseSemaphore(hSemaphoreLCheiaTipo11, 1, &semCount11);
				cout << "[QTD PREENCHIDA11] >> " << semCount11 << endl;
			
				Sleep(1000);
			if (listaMensagens->size() == 200) {
				std::cout << "a lista esta cheia";
				//system("pause");
			}
		}

	}
}