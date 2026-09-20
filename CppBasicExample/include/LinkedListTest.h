#ifndef __LINKEDLISTTEST_H__
#define __LINKEDLISTTEST_H__

#include "common.h"

class LinkListElement{
public:
	int data;
	LinkListElement *next;
public:
	LinkListElement(int _data){
		data = _data;
		next = NULL;
	}
};

void InsertLinkListElement_Front(LinkListElement *head, int data){
	LinkListElement* newElem = new LinkListElement(data);
	newElem->next = head->next;
	head->next = newElem;//head->next is a pointer, address of head->next is not changed, but head->next is changed
}

void InsertLinkListElement_Bottom(LinkListElement *head, int data){
	while(head->next !=NULL){
		head = head->next;
	}
	LinkListElement* newElem = new LinkListElement(data);
	newElem->next = NULL;
	head->next = newElem;//head->next is a INPUT pointer, address of head->next is not changed, but head->next is changed
}

void InsertAfterLinkListElement(LinkListElement *head, int referenceData, int data){
	while(head->next != NULL){
		if(head->data == referenceData){
			LinkListElement *newElem = new LinkListElement(data);
			newElem->next = head->next;
			head->next = newElem;
			return;
		}
		head = head->next;
	}
}

void DeleteLinkListElement(LinkListElement *head, int data){
	while(head->next != NULL){
		if(head->next->data == data){
			LinkListElement *deleteElement = head->next;
			head->next = deleteElement->next;
			delete deleteElement;
			return;
		}
		head = head->next;
	}
}

LinkListElement* FindLinkListElement(LinkListElement* head, int data){
	while(head->next != NULL){
		if (head->next->data == data)
			return head->next;
		else
			head = head->next;//head is a INPUT pointer, address of head is not changed, so head is not changed outsite
	}
	return head;
}

void TraverseLinkList(LinkListElement *head){
	std::cout<<"Traverse Linked List Data: ";
	while(head->next!= NULL){
		std::cout<<head->next->data<<" ";
		head = head->next;
	}
	std::cout<<std::endl;
}

void ClearLinkList(LinkListElement *head){
	while(head->next != NULL){
		LinkListElement *deleteElem = head->next;
		head->next = deleteElem->next;
		delete deleteElem;
	}

}

void LinkedListTest(){
	std::cout<<"=====Linked List Test====="<<std::endl;

	LinkListElement *head = new LinkListElement(0);

	std::cout<<"Print Empty Linked List: ";
	TraverseLinkList(head);

	for(int i = 0;i < 5; i++){
		InsertLinkListElement_Front(head, 100+i);
	}

	TraverseLinkList(head);

	LinkListElement *result = FindLinkListElement(head,112);

	std::cout<<"Find element 112, result is "<<result->data<<std::endl;

	InsertLinkListElement_Front(head, 9999);
	std::cout<<"Insert 9999 to Linked List: ";
	TraverseLinkList(head);

	std::cout<<"Delete Test:"<<std::endl;
	DeleteLinkListElement(head, 103);

	TraverseLinkList(head);

	DeleteLinkListElement(head, 10854345);

	TraverseLinkList(head);

	InsertLinkListElement_Bottom(head, 99990000);

	TraverseLinkList(head);

	InsertAfterLinkListElement(head, 101, 100000);

	TraverseLinkList(head);

	ClearLinkList(head);

	TraverseLinkList(head);

	std::cout<<std::endl;
}

#endif

