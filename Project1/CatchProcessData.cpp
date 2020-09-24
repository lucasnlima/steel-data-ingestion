#include <iostream>
#include <windows.h>

DWORD WINAPI CatchProcessData() {
	while (true)
	{
		std::cout << "Thread1";
	}
	return 0;
}