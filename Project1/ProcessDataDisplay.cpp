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

using namespace std;
typedef unsigned (WINAPI* CAST_FUNCTION)(LPVOID);

HANDLE hPipe;

vector<string> splitMessage(string message) {
	string delimiter = "/";
	size_t pos = 0;
	string token;
	vector<string> result;

	while ((pos = message.find(delimiter)) != string::npos) {
		token = message.substr(0, pos);
		message.erase(0, pos + delimiter.length());
		result.push_back(token);
	}

	result.push_back(message.substr(0, 11));
	return result;
}

int WatchKeyboard() {

	BOOL bStatus;
	DWORD dwBytesRead;
	char buffer[1];
	HANDLE hSemaphorePipe;
	hSemaphorePipe = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, "SemPipeLimpa");
	cout << GetLastError();

	hPipe = CreateNamedPipe(
		"\\\\.\\pipe\\teste",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_BYTE | PIPE_WAIT,
		1,
		0,
		0,
		1000,
		NULL
	);
	int b = GetLastError();
	ReleaseSemaphore(hSemaphorePipe, 1, NULL);

	bStatus = ConnectNamedPipe(hPipe, NULL);
	int a = GetLastError();
	if (!bStatus) {
		cout << "error";
	}

	while (true) {

		ZeroMemory(buffer, sizeof(buffer));
		bStatus = ReadFile(hPipe, &buffer, 1, &dwBytesRead, NULL);
		if (!bStatus) {
			cout << "error";
		}

		cout << buffer[0] << endl;
		string opcao = to_string(buffer[0]);

		if (opcao == "99") {
			system("cls");

		}

	}



	return 0;
}


int main() {

	SetConsoleTitle("Exibicao de Dados");

	unsigned int Thread4Id;


	HANDLE h_Thread4 = (HANDLE)_beginthreadex(
		NULL,
		0,
		(CAST_FUNCTION)WatchKeyboard,	// casting necessário
		NULL,
		0,
		&Thread4Id
	);

	HANDLE hSemArquivoAtualizado;
	HANDLE hFile;
	HANDLE hMutexArquivo;
	DWORD dwBytesRead = 0;
	BOOL bStatus;
	BOOL MutexStatus;
	LONG filePos = 0L;

	char buffer[46];
	int linhasArquivo = 0;
	hMutexArquivo = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, "MutexArquivo");
	cout << "ERRO AO ABRIR O SEMAFORO: " << GetLastError() << endl;


	hSemArquivoAtualizado = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, "SemAtArquivo");


	hFile = CreateFile("..\\Dados.txt",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	cout << GetLastError();

	while (true) {
		ZeroMemory(buffer, 46);
		vector<string> splittedMessage;
		WaitForSingleObject(hSemArquivoAtualizado, INFINITE);

		//WaitForSingleObject(hMutexArquivo, INFINITE);
		Sleep(1000);
		//LockFile(hFile, 0, 0, 4600, 0);
		if (linhasArquivo == 99) {
			linhasArquivo = 0;
			SetFilePointer(hFile, filePos, NULL, FILE_BEGIN);
		}
		else {
			linhasArquivo++;
		}
		bStatus = ReadFile(hFile, buffer, sizeof(buffer), &dwBytesRead, NULL);
		if (!bStatus) {
			cout << GetLastError();
		}
		//UnlockFile(hFile, 0, NULL, 4600, 0);
		ReleaseSemaphore(hMutexArquivo, 1, NULL);

		splittedMessage = splitMessage(buffer);

		if (splittedMessage.size() == 7) {
			cout << splittedMessage[6] << " NSEQ: " << splittedMessage[0] << " CAD: " << splittedMessage[2]
				<< " ID PLACA: " << splittedMessage[3] << " TEMP: " << splittedMessage[4]
				<< " VEL: " << splittedMessage[5] << endl;

		}
		else {
			cout << "array invalido" << endl;
		}

	}



}