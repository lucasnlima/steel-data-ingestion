#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <vector>

using namespace std;

int main() {

	HANDLE hFile = 0;
	HANDLE hPipe;
	DWORD dwBytesRead;
	BOOL bStatus;

	hFile = CreateFile("D:\\Users\\beatr\\Documents\\Faculdade\\6 Periodo\\Automação em Tempo Real\\steel-data-ingestion\\Dados.txt",
		GENERIC_READ,
		0,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	

	//bStatus = ReadFile(hFile, );

	cout << "Processo de display de dados funcionando!";
	int a;
	cin >> a;
}