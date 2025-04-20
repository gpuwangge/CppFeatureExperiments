#include <iostream>
//#include <hash_map>
#include <unordered_map>
#include <list>
#include <string>
#include <vector>
using namespace std;

/*
-Since there was no hash table defined in the C++ standard library, 
different implementors of the standard libraries would provide a 
non-standard hash table often named hash_map. Because these 
implementations were not written following a standard they all 
had subtle differences in functionality and performance guarantees.

-Starting with C++11 a hash table implementation has been added 
to the C++ standard library standard. It was decided to use an 
alternate name for the class to prevent collisions with these 
non-standard implementations and to prevent inadvertent use of 
the new class by developers who had hash_table in their code.

-The chosen alternate name is unordered_map which really is 
more descriptive as it hints at the class's map interface and the 
unordered nature of its elements.
*/

/*NP困难(NP - hard)*/
//某个问题被称作NP困难，当且仅当存在一个NP完全问题可以在多项式时间图灵归约到这个问题。
//指问题NP困难的答案可用于解决NP完全问题.因此解决NP完全问题不会难于解决NP困难问题.
//栗子：旅行推销员问题（Travelling Salesman Problem)
//有n个城市，一个推销员要从其中某一个城市出发，唯一走遍所有的城市，再回到他出发的城市，求最短的路线。
//也即求一个最短的哈密顿回路。
//即使用dp也要指数时间

/*NP Problem完全问题*/
//是NP（非决定性多项式时间）中最难的决定性问题.
//因此NP完备问题应该是最不可能被化简为P（多项式时间可决定）的决定性问题的集合。
//栗子：背包问题（Knapsack problem）
//给定一组物品，每种物品都有自己的重量和价格，在限定的总重量内，我们如何选择，才能使得物品的总价格最高。
//利用DP，背包问题存在一个伪多项式时间算法

/*伪多项式时间*/
//若一个数值算法的时间复杂度可以表示为输入数值N的多项式，则称其时间复杂度为伪多项式时间

/*try-finally语句*/
//这就是try-finally语句最具有魔幻能力的地方，即不管在何种情况下，在离开当前的作用域时，
//finally块区域内的代码都将会被执行到。

/*final关键字*/
//final关键字可以用于以下两种情况：
//1. 一个类或结构体不希望被继承。
//2. 一个虚函数不希望被override。


/*List and Hash Table*/
void BuildHashMap(){
	cout<<"Build Hash Map:"<<endl;
	unordered_map<string, int> hashMap;
	list<string> lst;
	for(int i = 0;i < 100; i++){
		cout<<i<<" ";
		char* c= new char;
		itoa(i,c,10);
		string s = c;
		lst.push_back(s);
		hashMap.insert(make_pair(s, i));
	}

	cout<<endl;

	hashMap["test"] = 10000;
	cout<<"Add [test,10000]"<<endl;

	cout<<"Size is:"<<hashMap.size()<<endl;

	cout<<"Remove"<<endl;
	for(list<string>::iterator iter = lst.begin(); iter != lst.end(); ++iter){
		cout<<hashMap[*iter]<<" ";
		hashMap.erase(*iter);
	}

	cout<<endl;

	cout<<"Size is:"<<hashMap.size()<<endl;

	cout<<"Left is:"<<hashMap["test"]<<endl;

	cout<<endl;
}

/*Vector*/
/*Each double allocation need O(n), but happen so few so it is still O(1)*/
/*Compare to list, no 'insert' in vector*/
void BuildVector(){
	cout<<"Build Vector:"<<endl;

	vector<int> v;
	for(int i = 0;i < 100; i++){
		cout<<i<<" ";
		v.push_back(i);
	}

	cout<<endl;

	cout<<"Size is: "<<v.size()<<endl;

	for(int i = 0;i < 10; i++){
		v.pop_back();
	}

	cout<<"Size is: "<<v.size()<<endl;

	for(int i = 0;i < 10; i++){
		cout<<v[i]<<" ";
	}
	cout<<endl;

	cout<<"Size is: "<<v.size()<<endl;

	cout<<endl;
}

/*String Buffer*/
/*Save time in stirng copy*/
/*
写时才拷贝（Copy-On-Write）技术，就是编程界“懒惰行为”——拖延战术的产物。
举个例子，比如我们有个程序要写文件，不断地根据网络传来的数据写，
如果每一次fwrite或是fprintf都要进行一个磁盘的I/O操作的话，
都简直就是性能上巨大的损失，因此通常的做法是，每次写文件操作都写在特定大小的一块内存中（磁盘缓存），
只有当我们关闭文件时，才写到磁盘上（这就是为什么如果文件不关闭，所写的东西会丢失的原因）。
更有甚者是文件关闭时都不写磁盘，而一直等到关机或是内存不够时才写磁盘，Unix就是这样一个系统，
如果非正常退出，那么数据就会丢失，文件就会损坏。
String STL use Copy-On-Write
Reason of String Buffer:
Create a buffer array
string.append(x)
rather then string = string + x
*/


void StringBuffer(){
	cout<<"StringBuffer:"<<endl;

	cout<<endl;
}


void main(){

	BuildHashMap();

	BuildVector();

	StringBuffer();

	getchar();
	return;
}