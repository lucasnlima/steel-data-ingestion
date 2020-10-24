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
#include <locale>

#define MAX_POSICOES 200

typedef unsigned (WINAPI* CAST_FUNCTION)(LPVOID);

using namespace std;

// Definindo a lista circular como ma
string listaMensagens[200];
long currentIndex = 0;
int i, j;
int listCount = 200;
LONG semCount;
LONG semCount11;
LONG semCount22;

HANDLE hSemaphoreLVazia;
HANDLE hSemaphoreLCheiaTipo11;
HANDLE hSemaphoreLCheiaTipo22;
HANDLE hSemaphorePipe;
HANDLE hSemaphoreArquivo;
HANDLE hMutexLista;
HANDLE hMutexVarLista;
HANDLE hEventPausa11;
HANDLE hEventPausa22;
HANDLE hFile;
HANDLE hSemAtualizaArquivo;
HANDLE hSemAbreArquivo;

DWORD WINAPI CatchProcessData();
DWORD WINAPI CapturaDeMensagensTipo11();
DWORD WINAPI CapturaDeMensagensTipo22();

DWORD dwBytesRead;
LPDWORD dwBytesWritten;
DWORD dwRegLength;
HANDLE hPipe;

int main(char args[]) {
	//setlocale(LC_ALL, "utf-8");

	SetConsoleTitle("Processo Principal");

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

	hSemaphorePipe = CreateSemaphore(NULL, 0, 1, "SemPipe");
	hSemAtualizaArquivo = CreateSemaphore(NULL, 0, 1, "SemAtArquivo");


	if (!CreateProcess("D:\\Users\\beatr\\Documents\\Faculdade\\6 Periodo\\Automação em Tempo Real\\steel-data-ingestion\\ProcessDataDisplay.exe",
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&siDataDisplay,
		&ProcessDataDisplay))
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
	};

	if (!CreateProcess("D:\\Users\\beatr\\Documents\\Faculdade\\6 Periodo\\Automação em Tempo Real\\steel-data-ingestion\\StreepDefectDisplay.exe",
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&siDefectDisplay,
		&StripDefectDisplay)) {
		printf("CreateProcess failed (%d).\n", GetLastError());
	};

	unsigned int Thread1Id;
	unsigned int Thread2Id;
	unsigned int Thread3Id;

	//---Incialização dos Semaforos
	hSemaphoreLVazia = CreateSemaphore(NULL, MAX_POSICOES, MAX_POSICOES, "SemListaVazia");
	hSemaphoreLCheiaTipo11 = CreateSemaphore(NULL, 0, MAX_POSICOES, "SemListaCheia11");
	hSemaphoreLCheiaTipo22 = CreateSemaphore(NULL, 0, MAX_POSICOES, "SemListaCheia22");
	hSemaphoreLCheiaTipo22 = CreateSemaphore(NULL, 0, MAX_POSICOES, "SemListaCheia22");

	//---Incialização do Mutexes
	hMutexLista = CreateMutex(NULL, false, "hMutexLista");
	hMutexVarLista = CreateMutex(NULL, false, "hMutexVarLista");

	//--Inicizalização dos Eventos
	hEventPausa11 = CreateEvent(NULL, true, true, "Evento11");
	hEventPausa22 = CreateEvent(NULL, true, true, "Evento22");

	HANDLE h_Thread1 = (HANDLE)_beginthreadex(
		NULL,
		0,
		(CAST_FUNCTION)CatchProcessData,	// casting necessário
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

	//---Aguarda leitura do teclada
	char a = 0;
	bool event11 = false;
	bool event22 = false;
	std::cout << "Main task" << endl;
	while (a != 32) {
		cin >> a;
		switch (a)
		{
		case 49:
			event11 = !event11;
			if (event11) {
				std::cout << "Pausa no consumo de mensagens tipo 11" << endl;
				ResetEvent(hEventPausa11);
			}
			else {
				std::cout << "Continua consumo de mensagens tipo 11" << endl;
				SetEvent(hEventPausa11);
			}
			break;
		case 50:
			event22 = !event22;
			if (event22) {
				std::cout << "Pausa no consumo de mensagens tipo 22" << endl;
				ResetEvent(hEventPausa22);
			}
			else {
				std::cout << "Continua consumo de mensagens tipo 22" << endl;
				SetEvent(hEventPausa22);
			}
			break;
		default:
			break;
		}
	}


	return 0;
}

DWORD WINAPI CapturaDeMensagensTipo11() {

	WaitForSingleObject(hSemaphorePipe, INFINITE); // usar o waitnamedpipe
	hPipe = CreateFile(
		"\\\\.\\pipe\\PipeMensagem",
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	while (true) {
		WaitForSingleObject(hEventPausa11, INFINITE);
		WaitForSingleObject(hMutexLista, INFINITE);
		WaitForSingleObject(hSemaphoreLCheiaTipo11, INFINITE);
		for (i = 0; i < 200; i++) {
			if (listaMensagens[i].find("\/11\/", 0) != string::npos) {
				std::cout << "[CONSUMIDA MENSAGEM TIPO 11]: " << listaMensagens[i] << endl;

				if (WaitNamedPipe("\\\\.\\pipe\\PipeMensagem", NMPWAIT_USE_DEFAULT_WAIT) == 0) {
					cout << "Esperando por uma instancia do pipe..." << endl;
				}

				int status = WriteFile(hPipe, listaMensagens[i].c_str(), sizeof(char) * (listaMensagens[i].length() + 1), dwBytesWritten, NULL);
				cout << GetLastError() << endl;

				listaMensagens[i] = "";
				WaitForSingleObject(hMutexVarLista, INFINITE);
				++listCount;
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

	DWORD nOut;
	char msgToFile[46] = "alou amigos \n";
	string msg;
	int linhasArquivo = 0;
	LONG filePos = 0L;
	BOOL bStatus;

	hFile = CreateFile("D:\\Users\\beatr\\Documents\\Faculdade\\6 Periodo\\Automação em Tempo Real\\steel-data-ingestion\\Dados.txt",
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);


	while (true) {
		WaitForSingleObject(hEventPausa22, INFINITE);
		WaitForSingleObject(hMutexLista, INFINITE);
		WaitForSingleObject(hSemaphoreLCheiaTipo22, INFINITE);
		for (j = 0; j < 200; j++) {
			if (listaMensagens[j].find("\/22\/", 0) != string::npos) {
				cout << "[CONSUMIDA MENSAGEM TIPO 22]: " << listaMensagens[j] << endl;

				ZeroMemory(msgToFile, sizeof(msgToFile));

				msg = listaMensagens[j] + "\n";
				strcpy(msgToFile, msg.c_str());

				if (linhasArquivo == 99) {
					linhasArquivo = 0;
					SetFilePointer(hFile, filePos, NULL, FILE_BEGIN);
					cout << "VOLTANDO PARA O INICIO DO ARQUIVO";
				}
				else {
					linhasArquivo++;
				}

				LockFile(hFile, 0, filePos, 46, 0);
				//WaitForSingleObject(hSemaphoreArquivo, INFINITE);

				bStatus = WriteFile(hFile, msgToFile, 46, &nOut, NULL);
				if (!bStatus) {
					cout << GetLastError();
				}

				//ReleaseSemaphore(hSemaphoreArquivo, 1, NULL);

				UnlockFile(hFile, 0, NULL, 4600, 0);

				listaMensagens[j] = "";
				ReleaseSemaphore(hSemAtualizaArquivo, 1, NULL);

				WaitForSingleObject(hMutexVarLista, INFINITE);
				++listCount;
				ReleaseMutex(hMutexVarLista);
				ReleaseSemaphore(hSemaphoreLVazia, 1, &semCount);
				break;
			}
		}
		ReleaseMutex(hMutexLista);
	}

	CloseHandle(hFile);
	return 0;
}

DWORD WINAPI CatchProcessData() {

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
				cout << "-------- A LISTA ESTA CHEIA ------------" << endl;
			}
			else {
				cout << "[--- COUNT]: " << listCount << endl;
			}
			ReleaseMutex(hMutexVarLista);
			WaitForSingleObject(hSemaphoreLVazia, INFINITE);

			listaMensagens[currentIndex] = msg1;
			cout << "[DEPOSITADA MENSAGEM TIPO 11]: " << msg1 << endl;
						
			currentNSEQTipo1++;
			currentIndex++;
			if (currentIndex == 200) currentIndex = 0;
			ReleaseSemaphore(hSemaphoreLCheiaTipo11, 1, &semCount11);


			msg2 = GenerateMessageType2(currentNSEQTipo2);
			WaitForSingleObject(hMutexVarLista, INFINITE);
			if (--listCount == 0) {
				cout << "-------- A LISTA ESTA CHEIA ------------" << endl;
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

