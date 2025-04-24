#ifndef __STACKTEST_H__
#define __STACKTEST_H__

#include "common.h"

#define EMPTY -99999

class StackNode{
public:
	int value;
	StackNode *next;
	StackNode (int _value){
		value = _value;
		next = NULL;
	}
};

/*Implement queue using linklist*/
/*first-in-first-out*/
class Queue{
public:
	StackNode *first, *last;
	Queue(){
		first = new StackNode(0);
		last = new StackNode(0);
	}

	int Dequeue(){//same as stack
		if(first->next != NULL){
			int x = first->next->value;
			StackNode *tmpNode = first->next;
			first->next = tmpNode->next;
			delete tmpNode;
			return x;
		}
		return EMPTY;

	}

	void Enqueue(int value){
		StackNode *n = new StackNode(value);
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
	StackNode *head;
	Stack(){
		head= new StackNode(0);
	}

	int Pop(){
		if(head->next != NULL){
			int x = head->next->value;
			StackNode *tmpNode = head->next;
			head->next = tmpNode->next;
			delete tmpNode;
			return x;
		}
		return EMPTY;
	}

	void Push(int value){
		StackNode *n = new StackNode(value);
		n->next = head->next;
		head->next = n;
	}

	void PrintAll(){
		cout<<"Print all:"<<endl;
		StackNode *headCopy = head;
		while(head->next!=NULL){
			cout<<head->next->value<<" ";
			head = head->next;
		}
		cout<<endl;
		head = headCopy;
	}
};

void StackTest(){
	cout << "=====Stack Test=====" << endl;

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

	cout<<endl;

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

#endif