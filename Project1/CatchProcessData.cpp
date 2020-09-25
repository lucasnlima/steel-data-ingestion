#include <iostream>
#include <windows.h>
#include <time.h>

DWORD WINAPI CatchProcessData() {
	while (true)
	{
		clock_t tick;
		double tempo = 0;
		int maior = 2000;
		int menor = 200;
		double reference = (double)(rand() % (maior - menor + 1) + menor);
		tick = clock();
		int a = 0;
		char error_message[36];
		char data_message[45];
		
		SYSTEMTIME  now;

		std::cout << '\n' << "Thread1";
		while (true) {
			tempo = (clock() - tick)* 1000/ CLOCKS_PER_SEC;
			if(tempo >= reference) {
						GetLocalTime(&now);
						std::cout << '\n' << "[NNNNN][11][NN][N][AAAAAA] " << a <<  " Tempo: ";
						std::cout << now.wHour << ":" << now.wMinute << ":" << now.wSecond << ":" << now.wMilliseconds;
						a++;
						reference = (double)(rand() % (maior - menor + 1) + menor);
						tick = clock();
			}
			if (tempo == 500) {
				GetLocalTime(&now);
				std::cout << '\n' << "[NNNNN][22][NNNN][AAAAAAAAA][NNN.N][NNN.N] " << a << " Tempo: " << tempo;
				std::cout << now.wHour << ":" << now.wMinute << ":" << now.wSecond << ":" << now.wMilliseconds;
				a++;

			}

		}
				
	}
	return 0;
}