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
int listCount = 200;
LONG semCount;
LONG semCount11;
LONG semCount22;

HANDLE hSemaphoreLVazia;
HANDLE hSemaphoreLCheiaTipo11;
HANDLE hSemaphoreLCheiaTipo22;
HANDLE hMutexLista;
HANDLE hMutexVarLista;

DWORD WINAPI CatchProcessData();
DWORD WINAPI CapturaDeMensagensTipo11();
DWORD WINAPI CapturaDeMensagensTipo22();

int main(char args[]) {
	STARTUPINFO siDataDisplay;				    // StartUpInformation para novo processo
	PROCESS_INFORMATION ProcessDataDisplay;	// Informações sobre novo processo criado

	STARTUPINFO siDefectDisplay;
	PROCESS_INFORMATION StripDefectDisplay;
	
	
	ZeroMemory(&siDataDisplay, sizeof(siDataDisplay));
	siDataDisplay.cb = sizeof(siDataDisplay);
	ZeroMemory(&ProcessDataDisplay, sizeof(ProcessDataDisplay));


	ZeroMemory(&siDefectDisplay, sizeof(siDefectDisplay));
	siDefectDisplay.cb = sizeof(siDefectDisplay);
	ZeroMemory(&StripDefectDisplay, sizeof(StripDefectDisplay));


	if (!CreateProcess("ProcessDataDisplay.exe", NULL, NULL, NULL,FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &siDataDisplay, &ProcessDataDisplay)) {
		printf("CreateProcess failed (%d).\n", GetLastError());
	};

	if (!CreateProcess("D:\\Users\\beatr\\Documents\\Faculdade\\6 Periodo\\Automação em Tempo Real\\steel-data-ingestion\\StreepDefectDisplay.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &siDefectDisplay, &StripDefectDisplay)) {
		printf("CreateProcess failed (%d).\n", GetLastError());
	};

	unsigned int Thread1Id;
	unsigned int Thread2Id;
	unsigned int Thread3Id;

	hSemaphoreLVazia = CreateSemaphore(NULL, MAX_POSICOES, MAX_POSICOES, "SemListaVazia");
	hSemaphoreLCheiaTipo11 = CreateSemaphore(NULL, 0, MAX_POSICOES, "SemListaCheia11");
	hSemaphoreLCheiaTipo22 = CreateSemaphore(NULL, 0, MAX_POSICOES, "SemListaCheia22");
	hMutexLista = CreateMutex(NULL, false, "hMutexLista");
	hMutexVarLista = CreateMutex(NULL, false, "hMutexVarLista");

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
		WaitForSingleObject(hMutexLista, INFINITE);
		WaitForSingleObject(hSemaphoreLCheiaTipo11, INFINITE);
		for (i = 0; i < 200; i++) {			
			if (listaMensagens[i].find("\/11\/", 0) != string::npos) {
				std::cout << "\n[CONSUMIDA MENSAGEM TIPO 11]: " << listaMensagens[i] << endl;
				//Função do processo de display que exibe na tela a
				//string
				listaMensagens[i] = "";
				WaitForSingleObject(hMutexVarLista, INFINITE);
				++listCount;
				cout << "[--- COUNT]: " << listCount << endl;
				ReleaseMutex(hMutexVarLista);				
				ReleaseSemaphore(hSemaphoreLVazia, 1, &semCount);	
				break;
			}
			ReleaseMutex(hMutexLista);
		}
	}
	return 0;
}

	// criar um mutex pros consumidores
DWORD WINAPI CapturaDeMensagensTipo22() {
	while (true) {	
		WaitForSingleObject(hMutexLista, INFINITE);
		WaitForSingleObject(hSemaphoreLCheiaTipo22, INFINITE);
		for (j = 0; j < 200; j++) {			
			if (listaMensagens[j].find("\/22\/", 0) != string::npos) {
				cout << "\n[CONSUMIDA MENSAGEM TIPO 22]: " << listaMensagens[j] << endl;
				listaMensagens[j] = "";
				
				WaitForSingleObject(hMutexVarLista, INFINITE);
				++listCount;
				cout << "[--- COUNT]: " << listCount << endl;
				ReleaseMutex(hMutexVarLista);				
				ReleaseSemaphore(hSemaphoreLVazia, 1, &semCount);
				break;
			}
		}
		ReleaseMutex(hMutexLista);		
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
	
		int a = 0;
		int currentNSEQTipo1 = 1;
		int currentNSEQTipo2 = 1;
		std::string msg1;
		std::string msg2;

		std::cout << '\n' << "Thread1";
		while (true) {

				if (currentIndex == 200) {
					cout << "lista cheia";
					currentIndex = 0;
					ReleaseSemaphore(hSemaphoreLCheiaTipo22, 1, &semCount22);
				}

				msg1 = GenerateMessageType1(currentNSEQTipo1);

				WaitForSingleObject(hMutexVarLista, INFINITE);
				if (--listCount == 0) {
					cout << "-------- A LISTA ESTA CHEIA ------------";
				}
				else {
					cout << "[--- COUNT]: " << listCount << endl;
				}
				ReleaseMutex(hMutexVarLista);
				WaitForSingleObject(hSemaphoreLVazia, INFINITE);
				
				listaMensagens[currentIndex] = msg1;
				cout << "[DEPOSITADA MENSAGEM TIPO 11]: " << msg1 << endl;
				//std::cout << '\n' << msg1;				
				currentNSEQTipo1++;
				currentIndex++;
				if (currentIndex == 200) currentIndex = 0;
				ReleaseSemaphore(hSemaphoreLCheiaTipo11, 1, &semCount11);
			

				msg2 = GenerateMessageType2(currentNSEQTipo2);
				WaitForSingleObject(hMutexVarLista, INFINITE);
				if (--listCount == 0) {
					cout << "-------- A LISTA ESTA CHEIA ------------";
				}
				else {
					cout << "[--- COUNT]: " << listCount << endl;
				}
				ReleaseMutex(hMutexVarLista);
				WaitForSingleObject(hSemaphoreLVazia, INFINITE);								
				listaMensagens[currentIndex] = msg2;
				cout << "[DEPOSITADA MENSAGEM TIPO 22]: " << msg2 << endl;

				currentNSEQTipo2++;				
				currentIndex++;
				if (currentIndex == 200) currentIndex = 0;
				ReleaseSemaphore(hSemaphoreLCheiaTipo22, 1, &semCount22);
 				Sleep(1000);
			}
			
		}

	}

