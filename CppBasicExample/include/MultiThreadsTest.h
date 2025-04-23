#ifndef __MULTITHREADSTEST_H__
#define __MULTITHREADSTEST_H__

#include "common.h"

#include <Windows.h>


HANDLE hMutex;
int GlobalThreadData;

DWORD WINAPI SubThread(LPVOID lpParamter){
	while(1){
		WaitForSingleObject(hMutex, INFINITE);
		GlobalThreadData += 1000;
		cout<<"Sub Thread!"<<GlobalThreadData<<endl;
		Sleep(500);
		ReleaseMutex(hMutex);
	}
}


void MultiThreadsTest(){
	HANDLE hThread = CreateThread(NULL, 0 , SubThread, NULL, 0, NULL);
	hMutex = CreateMutex(NULL, FALSE, NULL);
	GlobalThreadData = 0;
	CloseHandle(hThread);
	while(1){
		WaitForSingleObject(hMutex, INFINITE);
		GlobalThreadData += 1;
		cout<<"Main Thread!"<<GlobalThreadData<<endl;
		Sleep(1000);
		ReleaseMutex(hMutex);
	}

	getchar();
}


#endif

