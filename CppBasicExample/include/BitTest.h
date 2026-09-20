#ifndef __BITTEST_H__
#define __BITTEST_H__

#include "common.h"
#include <bitset>

/*Complement notion for negative: flip each bit and add 1*/
bool GetBit(int num, int i){
	int mask = 1<<i;
	return (num & mask) != 0;
}

void PrintAllBits(int num){
	for(int i = 15 ; i >=0; i--){
		std::cout<<GetBit(num,i);
		if(i == 8) std::cout<< "_";
	}
	std::cout<<std::endl;
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

void BitOperationTest(){
	std::cout<<"=====Bit Test====="<<std::endl;

	int x = 2;
	std::cout<<"x = "<<std::bitset<8>(x)<<", flip all bits ~x = "<<std::bitset<8>(~x)<<std::endl;

	int y = 3;
	std::cout<<"y = "<<std::bitset<8>(y)<<std::endl;
	std::cout<<"x&y = "<<std::bitset<8>(x&y)<<", ";
	std::cout<<"x|y = "<<std::bitset<8>(x|y)<<", ";
	std::cout<<"x^y = "<<std::bitset<8>(x^y)<<" (if bit is different, get 1; otherwise get 0)"<<std::endl;

	std::cout<<"x<<1 = "<<std::bitset<8>(x<<1)<<std::endl;
	std::cout<<"x>>1 = "<<std::bitset<8>(x>>1)<<std::endl;

	std::cout<<"Swap x and y using only ^: "<<std::endl;
	std::cout<<"  x = "<<x<<", y = "<<y<<std::endl;
	y = x^y;
	x = x^y;
	y = x^y;
	std::cout<<"  x = "<<x<<", y = "<<y<<std::endl;
	
	int z = 6;
	std::cout<<"z = "<<z<<" = ";
	PrintAllBits(z);

	std::cout<<"Set 3rd bit of z:"<<std::endl;
	z = SetBit(z, 3);
	std::cout<<"z = "<<z<<" = ";
	PrintAllBits(z);

	std::cout<<"Clear 2nd bit of z:"<<std::endl;
	z = ClearBit(z,2);
	std::cout<<"z = "<<z<<" = ";
	PrintAllBits(z);

	std::cout<<"ClearBitMSBthroughI of z: "<<std::endl;
	z = ClearBitMSBthroughI(z,2);//clear 2 ~ 15
	std::cout<<"z = "<<z<<" = ";
	PrintAllBits(z);

	std::cout<<"Update 3 from 9th bit of z:"<<std::endl;
	z = UpdateBit(z, 9, 3);
	std::cout<<"z = "<<z<<" = ";
	PrintAllBits(z);

	std::cout<<"ClearBitsIthough0 of z:"<<std::endl;
	z = ClearBitsIthough0(z, 5);//clear 5 ~ 0
	std::cout<<"z = "<<z<<" = ";
	PrintAllBits(z);

	std::cout<<std::endl;
}

#endif