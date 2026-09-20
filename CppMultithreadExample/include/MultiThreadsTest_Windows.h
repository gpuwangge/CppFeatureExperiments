#define pragma once
#include "common.h"
#include <Windows.h>

HANDLE hMutex;
int GlobalThreadData;

DWORD WINAPI SubThread(LPVOID lpParamter){
	while(1){
		WaitForSingleObject(hMutex, INFINITE);
		GlobalThreadData += 1000;
		std::cout<<"Sub Thread!"<<GlobalThreadData<<std::endl;
		Sleep(500);
		ReleaseMutex(hMutex);
	}
}


void MultiThreadsTest_Windows(){
	HANDLE hThread = CreateThread(NULL, 0 , SubThread, NULL, 0, NULL);
	hMutex = CreateMutex(NULL, FALSE, NULL);
	GlobalThreadData = 0;
	CloseHandle(hThread);
	while(1){
		WaitForSingleObject(hMutex, INFINITE);
		GlobalThreadData += 1;
		std::cout<<"Main Thread!"<<GlobalThreadData<<std::endl;
		Sleep(1000);
		ReleaseMutex(hMutex);
	}

	getchar();
}



