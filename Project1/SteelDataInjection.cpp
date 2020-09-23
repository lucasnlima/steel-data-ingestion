#include <iostream>
#include <windows.h>
#include "CheckForError.h"

using namespace std;

int main(char args[]) {
	STARTUPINFO si;				    // StartUpInformation para novo processo
	PROCESS_INFORMATION ProcessDataDisplay;	// Informações sobre novo processo criado
	cout << "Main task";
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&ProcessDataDisplay, sizeof(ProcessDataDisplay));

	if (!CreateProcess("C:\\Windows\\System32\\calc.exe", NULL, NULL, NULL,FALSE, 0, NULL,NULL, &si, &ProcessDataDisplay)) {
		printf("CreateProcess failed (%d).\n", GetLastError());
	};
	// Wait until child process exits.
	WaitForSingleObject(ProcessDataDisplay.hProcess, INFINITE);
	
	// Close process and thread handles. 
	CloseHandle(ProcessDataDisplay.hProcess);
	CloseHandle(ProcessDataDisplay.hThread);




	return 0;
}