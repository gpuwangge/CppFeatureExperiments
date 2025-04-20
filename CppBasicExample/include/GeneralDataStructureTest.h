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

/*NP����(NP - hard)*/
//ĳ�����ⱻ����NP���ѣ����ҽ�������һ��NP��ȫ��������ڶ���ʽʱ��ͼ���Լ��������⡣
//ָ����NP���ѵĴ𰸿����ڽ��NP��ȫ����.��˽��NP��ȫ���ⲻ�����ڽ��NP��������.
//���ӣ���������Ա���⣨Travelling Salesman Problem)
//��n�����У�һ������ԱҪ������ĳһ�����г�����Ψһ�߱����еĳ��У��ٻص��������ĳ��У�����̵�·�ߡ�
//Ҳ����һ����̵Ĺ��ܶٻ�·��
//��ʹ��dpҲҪָ��ʱ��

/*NP Problem��ȫ����*/
//��NP���Ǿ����Զ���ʽʱ�䣩�����ѵľ���������.
//���NP�걸����Ӧ��������ܱ�����ΪP������ʽʱ��ɾ������ľ���������ļ��ϡ�
//���ӣ��������⣨Knapsack problem��
//����һ����Ʒ��ÿ����Ʒ�����Լ��������ͼ۸����޶����������ڣ��������ѡ�񣬲���ʹ����Ʒ���ܼ۸���ߡ�
//����DP�������������һ��α����ʽʱ���㷨

/*α����ʽʱ��*/
//��һ����ֵ�㷨��ʱ�临�Ӷȿ��Ա�ʾΪ������ֵN�Ķ���ʽ�������ʱ�临�Ӷ�Ϊα����ʽʱ��

/*try-finally���*/
//�����try-finally��������ħ�������ĵط����������ں�������£����뿪��ǰ��������ʱ��
//finally�������ڵĴ��붼���ᱻִ�е���

/*final�ؼ���*/
//final�ؼ��ֿ��������������������
//1. һ�����ṹ�岻ϣ�����̳С�
//2. һ���麯����ϣ����override��


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
дʱ�ſ�����Copy-On-Write�����������Ǳ�̽硰������Ϊ����������ս���Ĳ��
�ٸ����ӣ����������и�����Ҫд�ļ������ϵظ������紫��������д��
���ÿһ��fwrite����fprintf��Ҫ����һ�����̵�I/O�����Ļ���
����ֱ���������Ͼ޴����ʧ�����ͨ���������ǣ�ÿ��д�ļ�������д���ض���С��һ���ڴ��У����̻��棩��
ֻ�е����ǹر��ļ�ʱ����д�������ϣ������Ϊʲô����ļ����رգ���д�Ķ����ᶪʧ��ԭ�򣩡�
�����������ļ��ر�ʱ����д���̣���һֱ�ȵ��ػ������ڴ治��ʱ��д���̣�Unix��������һ��ϵͳ��
����������˳�����ô���ݾͻᶪʧ���ļ��ͻ��𻵡�
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