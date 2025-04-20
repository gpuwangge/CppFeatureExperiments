#include <iostream>
#include <Windows.h>
using namespace std;

/*synchronized*/
//1 synchronized aMethod(){} 可以防止多个线程同时 访问这个对象的synchronized方法
//如果一个对象有多个synchronized方法，只要一个线程访问了其中的一个synchronized方法，
//其它线程不能同时访问这个对象中任何一个synchronized方法 
//总的来说，这种情况，锁就是这个方法所在的对象 
//2 synchronized static aStaticMethod{} 防止多个线程同时访问这个类中的synchronized static 方法 
//3 synchronized(this){/*区块*/}，锁就是这个方法所在的对象 
//synchronized关键字还可以用于方法中的某个区块中，表示只对这个区块的资源实行互斥访问
//4 synchronized关键字是不能继承的
//5 无论synchronized关键字加在方法上还是对象上，它取得的锁都是对象，而不是把一段代码或函数当作锁
//6 一个没有共享的对象作为锁是没有意义的
//7 例子
//为了保证银行账户的安全，可以操作账户的方法如下：

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