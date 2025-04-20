#ifndef __BITTEST_H__
#define __BITTEST_H__

#include "common.h"
#include <bitset>
using namespace std;


/*Complement notion for negative: flip each bit and add 1*/
bool GetBit(int num, int i){
	int mask = 1<<i;
	return (num & mask) != 0;
}

void PrintAllBits(int num){
	for(int i = 15 ; i >=0; i--){
		cout<<GetBit(num,i);
		if(i == 8) cout<< "_";
	}
	cout<<endl;
}

int SetBit(int num, int i){
	int mask = 1<<i;
	return num | mask;
}

int ClearBit(int num, int i){
	int mask = ~(1<<i);
	return num & mask;
}

int ClearBitMSBthroughI(int num, int i){
	int mask = (1<<i) - 1;
	return num & mask;
}

int ClearBitsIthough0(int num, int i){
	int mask = ~((1 << (i+1)) - 1);
	return num & mask;
}

int UpdateBit(int num, int i, int v){ //merge setbit and clearbit
	int mask = ~(1<<i);
	return (num & mask) | (v << i);
}

void BitTest(){
	cout<<"=====Bit Test====="<<endl;

	int x = 2;
	cout<<"x = "<<bitset<8>(x)<<", flip all bits ~x = "<<bitset<8>(~x)<<endl;

	int y = 3;
	cout<<"y = "<<bitset<8>(y)<<endl;
	cout<<"x&y = "<<bitset<8>(x&y)<<", ";
	cout<<"x|y = "<<bitset<8>(x|y)<<", ";
	cout<<"x^y = "<<bitset<8>(x^y)<<" (if bit is different, get 1; otherwise get 0)"<<endl;

	cout<<"x<<1 = "<<bitset<8>(x<<1)<<endl;
	cout<<"x>>1 = "<<bitset<8>(x>>1)<<endl;

	cout<<"Swap x and y using only ^: "<<endl;
	cout<<"  x = "<<x<<", y = "<<y<<endl;
	y = x^y;
	x = x^y;
	y = x^y;
	cout<<"  x = "<<x<<", y = "<<y<<endl;
	
	int z = 6;
	cout<<"z = "<<z<<" = ";
	PrintAllBits(z);

	cout<<"Set 3rd bit of z:"<<endl;
	z = SetBit(z, 3);
	cout<<"z = "<<z<<" = ";
	PrintAllBits(z);

	cout<<"Clear 2nd bit of z:"<<endl;
	z = ClearBit(z,2);
	cout<<"z = "<<z<<" = ";
	PrintAllBits(z);

	cout<<"ClearBitMSBthroughI of z: "<<endl;
	z = ClearBitMSBthroughI(z,2);//clear 2 ~ 15
	cout<<"z = "<<z<<" = ";
	PrintAllBits(z);

	cout<<"Update 3 from 9th bit of z:"<<endl;
	z = UpdateBit(z, 9, 3);
	cout<<"z = "<<z<<" = ";
	PrintAllBits(z);

	cout<<"ClearBitsIthough0 of z:"<<endl;
	z = ClearBitsIthough0(z, 5);//clear 5 ~ 0
	cout<<"z = "<<z<<" = ";
	PrintAllBits(z);

	cout<<endl;
}

#endif