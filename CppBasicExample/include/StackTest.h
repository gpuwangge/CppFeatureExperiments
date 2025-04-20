#include <iostream>
using namespace std;

#define EMPTY -99999

class Node{
public:
	int value;
	Node *next;
	Node (int _value){
		value = _value;
		next = NULL;
	}
};

/*Implement queue using linklist*/
/*first-in-first-out*/
class Queue{
public:
	Node *first, *last;
	Queue(){
		first = new Node(0);
		last = new Node(0);
	}

	int Dequeue(){//same as stack
		if(first->next != NULL){
			int x = first->next->value;
			Node *tmpNode = first->next;
			first->next = tmpNode->next;
			delete tmpNode;
			return x;
		}
		return EMPTY;

	}

	void Enqueue(int value){
		Node *n = new Node(value);
		if(first->next == NULL){
			first->next = n;
			last->next = n;
		}else{
			last->next->next = n;
			last->next = n;
		}
	}
};

/*Implement stack using linklist*/
/*last-in-first-out*/


class Stack{
public:
	Node *head;
	Stack(){
		head= new Node(0);
	}

	int Pop(){
		if(head->next != NULL){
			int x = head->next->value;
			Node *tmpNode = head->next;
			head->next = tmpNode->next;
			delete tmpNode;
			return x;
		}
		return EMPTY;
	}

	void Push(int value){
		Node *n = new Node(value);
		n->next = head->next;
		head->next = n;
	}

	void PrintAll(){
		cout<<"Print all:"<<endl;
		Node *headCopy = head;
		while(head->next!=NULL){
			cout<<head->next->value<<" ";
			head = head->next;
		}
		cout<<endl;
		head = headCopy;
	}
};

void main(){
	cout<<"Stack test:"<<endl;
	Stack stack;
	
	cout<<"Input sequence:"<<endl;
	for(int i = 0; i < 10; i ++){
		stack.Push(i*100);
		cout<<i*100<<" ";
	}
	cout<<endl;

	cout<<"Pop test;"<<endl;
	int returnValue;
	while((returnValue = stack.Pop()) != EMPTY)
		cout<<returnValue<<" ";
	cout<<"Over"<<endl;

	cout<<"Pop test2;"<<endl;
	while((returnValue = stack.Pop()) != EMPTY)
		cout<<returnValue<<" ";
	cout<<"Over"<<endl;

	//---------------------

	cout<<"Queue test:"<<endl;
	Queue queue;
	cout<<"Input sequence:"<<endl;
	for(int i = 0; i < 10; i ++){
		queue.Enqueue(i*100+i);
		cout<<i*100+i<<" ";
	}
	cout<<endl;

	cout<<"Dequeue test;"<<endl;
	while((returnValue = queue.Dequeue()) != EMPTY)
		cout<<returnValue<<" ";
	cout<<"Over"<<endl;

	cout<<"Dequeue test2;"<<endl;
	while((returnValue = queue.Dequeue()) != EMPTY)
		cout<<returnValue<<" ";
	cout<<"Over"<<endl;

	getchar();

	/*
	010101
	100110

	0100
	1000

	101010

	11
	00
	10*/
}