#include <iostream>
#include <Windows.h>
using namespace std;

/*synchronized*/
//1 synchronized aMethod(){} ���Է�ֹ����߳�ͬʱ ������������synchronized����
//���һ�������ж��synchronized������ֻҪһ���̷߳��������е�һ��synchronized������
//�����̲߳���ͬʱ��������������κ�һ��synchronized���� 
//�ܵ���˵���������������������������ڵĶ��� 
//2 synchronized static aStaticMethod{} ��ֹ����߳�ͬʱ����������е�synchronized static ���� 
//3 synchronized(this){/*����*/}������������������ڵĶ��� 
//synchronized�ؼ��ֻ��������ڷ����е�ĳ�������У���ʾֻ������������Դʵ�л������
//4 synchronized�ؼ����ǲ��ܼ̳е�
//5 ����synchronized�ؼ��ּ��ڷ����ϻ��Ƕ����ϣ���ȡ�õ������Ƕ��󣬶����ǰ�һ�δ������������
//6 һ��û�й���Ķ�����Ϊ����û�������
//7 ����
//Ϊ�˱�֤�����˻��İ�ȫ�����Բ����˻��ķ������£�

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


void main(){
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
	return;

}