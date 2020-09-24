#include <iostream>
#include <stdio.h>
#include <process.h>
#include <windows.h>
#include "CheckForError.h"
#include "CatchProcessData.h"

typedef unsigned (WINAPI* CAST_FUNCTION)(LPVOID);

using namespace std;

int main(char args[]) {
	//STARTUPINFOA si;				    // StartUpInformation para novo processo
	//PROCESS_INFORMATION ProcessDataDisplay;	// Informações sobre novo processo criado
	

	
	//
	//ZeroMemory(&si, sizeof(si));
	//si.cb = sizeof(si);
	//ZeroMemory(&ProcessDataDisplay, sizeof(ProcessDataDisplay));

	//if (!CreateProcessA(NULL, NULL, NULL, NULL,FALSE, DEBUG_PROCESS, NULL,NULL, &si, &ProcessDataDisplay)) {
	//	printf("CreateProcess failed (%d).\n", GetLastError());
	//};
	//// Wait until child process exits.
	//WaitForSingleObject(ProcessDataDisplay.hProcess, INFINITE);
	//
	//// Close process and thread handles. 
	//CloseHandle(ProcessDataDisplay.hProcess);
	//CloseHandle(ProcessDataDisplay.hThread);
	unsigned int Thread1Id;

	HANDLE h_Thread1 = (HANDLE)_beginthreadex(
		NULL,
		0,
		(CAST_FUNCTION) CatchProcessData,	// casting necessário
		NULL,
		0,
		&Thread1Id
	);

	while (true) {
	cout << "Main task";

	}


	return 0;
}