#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <vector>
using namespace std;

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
	
	result.push_back(message);
	return result;
}

int main() {

	SetConsoleTitle("Exibicao de Defeitos");
	HANDLE hSemaphorePipe;
	HANDLE hEventPauseDefectDisplay;
	char mensagemRecebida[256];
	HANDLE hPipe;
	BOOL bStatus;
	DWORD dwBytesRead;
	vector<string> splitedMessage;

	hEventPauseDefectDisplay = OpenEvent(SYNCHRONIZE, FALSE, TEXT("EventPausaDefectDisplay"));
	if (hEventPauseDefectDisplay == NULL) {
		cout << "Erro ao abrir o handle de evento. COD: " << GetLastError();
	}
	hSemaphorePipe = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, "SemPipe");


	hPipe = CreateNamedPipe(
		"\\\\.\\pipe\\PipeMensagem",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_BYTE | PIPE_WAIT,
		1,
		0,
		0,
		1000,
		NULL
	);
	ReleaseSemaphore(hSemaphorePipe, 1, NULL);

	bStatus = ConnectNamedPipe(hPipe, NULL);
	if (!bStatus) {
		cout << "error";
	}

	while (true) {

		WaitForSingleObject(hEventPauseDefectDisplay, INFINITE);

		bStatus = ReadFile(hPipe, &mensagemRecebida, 256, &dwBytesRead, NULL);

		splitedMessage = splitMessage(mensagemRecebida);

		if (!bStatus) {			
			cout << "error";
		}	

		string time = splitedMessage[6];
		string nseq = splitedMessage[0];
		string cad = splitedMessage[2];
		string idFoto = splitedMessage[5];
		string grav = splitedMessage[3];
		string classe = splitedMessage[4];

		cout << time << " NSEQ: " << nseq << " CAD: " << cad << " ID FOTO: " << idFoto	<< " GRAV: " << grav << " CLASSE: " << classe << endl;
	}
}

