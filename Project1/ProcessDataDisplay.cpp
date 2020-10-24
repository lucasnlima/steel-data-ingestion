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
	
	result.push_back(message.substr(0, 11));
	return result;
}

int main() {

	SetConsoleTitle("Exibicao de Dados");

	HANDLE hSemArquivoAtualizado;
	HANDLE hFile;
	HANDLE hPipe;
	HANDLE hEventPauseProcessDisplay;
	DWORD dwBytesRead = 0;
	BOOL bStatus;
	LONG filePos = 0L;
	char buffer[46];
	int linhasArquivo = 0;

	hEventPauseProcessDisplay = OpenEvent(SYNCHRONIZE, FALSE, TEXT("EventPausaProcessDisplay"));
	if (hEventPauseProcessDisplay == NULL) {
		cout << "Erro ao abrir o handle de evento. COD: " << GetLastError();
	}
	hSemArquivoAtualizado = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, "SemAtArquivo");
	if (hSemArquivoAtualizado == NULL) {
		cout << "Erro ao abrir o semaforo. COD: " << GetLastError();
	}
	
	
	hFile = CreateFile("..\\Dados.txt",
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);		

	cout << GetLastError();

 	while (true) {
		WaitForSingleObject(hEventPauseProcessDisplay, INFINITE);
		ZeroMemory(buffer, 46);
		vector<string> splittedMessage;
		WaitForSingleObject(hSemArquivoAtualizado, INFINITE);

		LockFile(hFile, 0, filePos, 46, 0);
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
		UnlockFile(hFile, 0, NULL, 4600, 0);
		
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