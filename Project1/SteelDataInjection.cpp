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
HANDLE hEventPausaInspect;
HANDLE hEventPausaDefectDisplay;
HANDLE hEventPausaProcessDisplay;
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

void gotoxy(int x, int y) {
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

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


	if (!CreateProcess("..\\ProcessDataDisplay.exe",
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

	if (!CreateProcess("..\\StreepDefectDisplay.exe",
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
	hEventPausa11 = CreateEvent(NULL, true, true, TEXT("Evento11"));
	hEventPausa22 = CreateEvent(NULL, true, true, TEXT("Evento22"));
	hEventPausaInspect = CreateEvent(NULL, true, true, TEXT("EventoInspect"));
	hEventPausaDefectDisplay = CreateEvent(NULL, true, true, TEXT("EventPausaDefectDisplay"));
	hEventPausaProcessDisplay = CreateEvent(NULL, true, true, TEXT("EventPausaProcessDisplay"));


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
	int logcount = 0;
	bool event11 = false;
	bool event22 = false;
	bool eventPausaInspect = false;
	bool eventPausaDisplayDefect = false;
	bool eventPausaProcessDisplay = false;

	std::cout << "________________________________________________________________\n" <<
		"                   Steel Data Injection  v2.0             \n" <<
		"----------------------------------------------------------------\n" <<
		" Programa para coleta de dados de laminacaoo de aco\n Opcoes:\n" <<
		"  <i> Set/Reset Inspecao de defeitos.\n" <<
		"  <d> Set/Reset Captura de mensagens de defeito.\n" <<
		"  <e> Set/Reset Captura de mensagens de dados do processo.\n" <<
		"  <a> Set/Reset Exeibicao de defeitos.\n" <<
		"  <l> Set/Reset Exibicao de dados do processo.\n" <<
		"  <c> Set/Reset Limpar janela de exibicao de dados do processo.\n" <<
		"  <ESC> Finalizar aplicacao.\n\n" <<
		" Escolha uma opcao:";
		gotoxy(0, 15);
		std::cout << "[LOG DE COMANDOS INSERIDOS]" << endl;
		gotoxy(19, 13);

	while (a != 32) {
		cin >> a;
		switch (a)
		{
		case 105:
			eventPausaInspect = !eventPausaInspect;
			if (eventPausaInspect) {
				logcount += 1;
				gotoxy(1, 16+logcount);
				std::cout << logcount << " - Pausa Inspecao de defeitos" << endl;
				gotoxy(19, 13);
				ResetEvent(hEventPausaInspect);
			}
			else {
				logcount += 1;
				gotoxy(1, 16 + logcount);
				std::cout << logcount << " - Continua Inspecao de defeitos" << endl;
				gotoxy(19, 13);
				SetEvent(hEventPausaInspect);
			}
			break;
		case 100:
			event11 = !event11;
			if (event11) {
				logcount += 1;
				gotoxy(1, 16 + logcount);
				std::cout << logcount << " - Pausa no captura de mensagens tipo 11" << endl;
				gotoxy(19, 13);
				ResetEvent(hEventPausa11);
			}
			else {
				logcount += 1;
				gotoxy(1, 16 + logcount);
				std::cout << logcount << " - Continua captura de mensagens tipo 11" << endl;
				gotoxy(19, 13);
				SetEvent(hEventPausa11);
			}
			break;
		case 101:
			event22 = !event22;
			if (event22) {
				logcount += 1;
				gotoxy(1, 16 + logcount);
				std::cout << logcount << " - Pausa no captura de mensagens tipo 22" << endl;
				gotoxy(19, 13);
				ResetEvent(hEventPausa22);
			}
			else {
				logcount += 1;
				gotoxy(1, 16 + logcount);
				std::cout << logcount << " - Continua captura de mensagens tipo 22" << endl;
				gotoxy(19, 13);
				SetEvent(hEventPausa22);
			}
			break;
		case 97:
			eventPausaDisplayDefect = !eventPausaDisplayDefect;
			if (eventPausaDisplayDefect) {
				logcount += 1;
				gotoxy(1, 16 + logcount);
				std::cout << logcount << " - Pausa Processo de exibição de Defeitos" << endl;
				gotoxy(19, 13);
				ResetEvent(hEventPausaDefectDisplay);
			}
			else {
				logcount += 1;
				gotoxy(1, 16 + logcount);
				std::cout << logcount << " - Continua Processo de exibição de Defeitos" << endl;
				gotoxy(19, 13);
				SetEvent(hEventPausaDefectDisplay);
			}
			break;
		case 108:
			eventPausaProcessDisplay = !eventPausaProcessDisplay;
			if (eventPausaProcessDisplay) {
				logcount += 1;
				gotoxy(1, 16 + logcount);
				std::cout << logcount << " - Pausa exibição de dados do processo" << endl;
				gotoxy(19, 13);
				ResetEvent(hEventPausaProcessDisplay);
			}
			else {
				logcount += 1;
				gotoxy(1, 16 + logcount);
				std::cout << logcount << " - Continua exibição de dados do processo" << endl;
				gotoxy(19, 13);
				SetEvent(hEventPausaProcessDisplay);
			}
			break;
		default:
			gotoxy(19, 13);
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
				//std::cout << "[CONSUMIDA MENSAGEM TIPO 11]: " << listaMensagens[i] << endl;

				if (WaitNamedPipe("\\\\.\\pipe\\PipeMensagem", NMPWAIT_USE_DEFAULT_WAIT) == 0) {
					//cout << "Esperando por uma instancia do pipe..." << endl;
				}

				int status = WriteFile(hPipe, listaMensagens[i].c_str(), sizeof(char) * (listaMensagens[i].length() + 1), dwBytesWritten, NULL);
				if (status != TRUE) {
					cout << "Erro ao escrever no arquivo COD:" << GetLastError() << endl;
				}

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
	char msgToFile[46] = "";
	string msg;
	int linhasArquivo = 0;
	LONG filePos = 0L;
	BOOL bStatus;

	hFile = CreateFile("..\\Dados.txt",
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
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
				//cout << "[CONSUMIDA MENSAGEM TIPO 22]: " << listaMensagens[j] << endl;

				ZeroMemory(msgToFile, sizeof(msgToFile));

				msg = listaMensagens[j] + "\n";
				strcpy(msgToFile, msg.c_str());

				if (linhasArquivo == 99) {
					linhasArquivo = 0;
					SetFilePointer(hFile, filePos, NULL, FILE_BEGIN);
					//cout << "VOLTANDO PARA O INICIO DO ARQUIVO";
				}
				else {
					linhasArquivo++;
				}

				LockFile(hFile, 0, 0, 4600, 0);
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

		while (true) {

			WaitForSingleObject(hEventPausaInspect, INFINITE);

			if (currentIndex == 200) {
				//cout << "lista cheia";
				currentIndex = 0;
				ReleaseSemaphore(hSemaphoreLCheiaTipo22, 1, &semCount22);
			}

			msg1 = GenerateMessageType1(currentNSEQTipo1);

			WaitForSingleObject(hMutexVarLista, INFINITE);
			//if (--listCount == 0) {
			//	//cout << "-------- A LISTA ESTA CHEIA ------------" << endl;
			//}
			//else {
			//	//cout << "[--- COUNT]: " << listCount << endl;
			//}
			ReleaseMutex(hMutexVarLista);
			WaitForSingleObject(hSemaphoreLVazia, INFINITE);

			listaMensagens[currentIndex] = msg1;
			//cout << "[DEPOSITADA MENSAGEM TIPO 11]: " << msg1 << endl;
						
			currentNSEQTipo1++;
			currentIndex++;
			if (currentIndex == 200) currentIndex = 0;
			ReleaseSemaphore(hSemaphoreLCheiaTipo11, 1, &semCount11);


			msg2 = GenerateMessageType2(currentNSEQTipo2);
			WaitForSingleObject(hMutexVarLista, INFINITE);
			//if (--listCount == 0) {
			//	//cout << "-------- A LISTA ESTA CHEIA ------------" << endl;
			//}
			
			ReleaseMutex(hMutexVarLista);
			WaitForSingleObject(hSemaphoreLVazia, INFINITE);
			listaMensagens[currentIndex] = msg2;
			//cout << "[DEPOSITADA MENSAGEM TIPO 22]: " << msg2 << endl;

			currentNSEQTipo2++;
			currentIndex++;
			if (currentIndex == 200) currentIndex = 0;
			ReleaseSemaphore(hSemaphoreLCheiaTipo22, 1, &semCount22);
			Sleep(1000);
		}

	}

}

