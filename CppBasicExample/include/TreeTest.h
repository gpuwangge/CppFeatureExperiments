#ifndef __TREETEST_H__
#define __TREETEST_H__

#include "common.h"


// People often say ��tree�� when they mean ��binary search tree.��, L<M<R
// Heap, M>L, M>R; or M<L, M<R.  Heap is a complete tree (such as complete bianry tree)

/*k-ary tree */
//n graph theory, a k-ary tree is a rooted tree in which each node has no more than k children. 
//It is also sometimes known as a k-way tree, an N-ary tree, or an M-ary tree. 
//A binary tree is the special case where k=2.

/*Red-Black Tree*/
/*Index O(log(n))*/
/*1.A node is either red or black
2.The root is the same color as all leaves (Leaves is NULL itself)
3.Both children of every red node are black
4.Each simple path from a given node to any of its descendant leaves contains the same number of black nodes.*/
/*Red-black property is destroyed after operation from BST.Restore red-black tree need less than 3 rotations*/
/*Have a little more advantages in performance compared to AVL tree, so can replace AVL tree*/
/*Diffrence to AVL is that AVL seeks strict balance but RBT need local balance, and use color to represent height.*/

/*AVL Tree*/
/*Index O(log(n))*/
/*More rigidly balanced compared to RB tree*/
/*Height balanced, get height of each nodes*/

/*Trie*/
//a node's all sons have the same prefix, and they are the string of this node, and root is empty string
//trie is often used in search. such as entering a web url, auto all possible urls
//if there is no complete match, can return most similar result

/*Binary Search Tree*/
//if any node's left is non empty, then all left sub-tree's node's value is less or equal to its root node
//if any node's right is non empty, then all right sub-tree's node's value is larger or equal to its root node
//any left sub tree or right sub tree are also binary search tree
//no duplicate nodes
//in-order traverse will produce a sorted array

/*Binary Max Heap Node*/
class HeapNode{
public:
	int value;
	int index;
	HeapNode(int _value, int _index){
		value = _value;
		index = _index;
	}
};

/*Binary Max Heap*/
class Heap{
public:	
	vector<HeapNode> nodes; 
	int size;
	Heap(){
		size = 0;
	}

	void AddNode(int value){ //construct a heap
		nodes.push_back(HeapNode(value, size));
		size++;
	}

	void Tranverse(){
		cout<<"Traverse "<<endl;;
		for(int i = 0; i < size; i++){
			cout<<nodes[i].value<<" ";
			//if(i == 0) cout<<endl;
			//if(i == 2) cout<<endl;
			//if(i == 6) cout<<endl;
			//if(i == 14) cout<<endl;
		}
		cout<<endl;
	}

	int GetParentIndex(int index){
		return (index - 1) / 2;
	}
	int GetLeftChild(int index){
		return index * 2 + 1;
	}
	int GetRightChild(int index){
		return index * 2 + 2;
	}

	void MaxHeapify(int index){
		int l = GetLeftChild(index);
		int r = GetRightChild(index);
		int largest = index;

		if(l < size)
			if(nodes[largest].value < nodes[l].value)
				largest = l;

		if(r < size)
			if(nodes[largest].value < nodes[r].value)
				largest = r;

		if(index != largest){
			int tmp = nodes[index].value;
			nodes[index].value = nodes[largest].value;
			nodes[largest].value = tmp;

			MaxHeapify(largest);
		}
	}

	void BuildHeap(){ //turn a heap to max heap
		for(int i = (size / 2); i >= 0 ;i--) //!!!!!!!!!!!!!!
			MaxHeapify(i);
	}

	int Sort(){ //pop out a maximum value
		if(size >0){
			int returnValue = nodes[0].value;
			nodes[0].value = nodes[size-1].value;
			nodes.pop_back();
			size = nodes.size();
			MaxHeapify(0);
			return returnValue;
		}else{
			return 0;
		}
	}

};



/*Binary Search Node*/
class BinaryNode{
public:
	int value;
	BinaryNode *LChild;
	BinaryNode *RChild;
	BinaryNode(int _value){
		value = _value;
		LChild = NULL;
		RChild = NULL;
	}

	void AddLeft(int value){//not used
		if(LChild != NULL){
			delete LChild;
		}
		LChild = new BinaryNode(value);
	}

	void AddRight(int value){//not used
		if(RChild != NULL){
			delete RChild;
		}
		RChild = new BinaryNode(value);
	}

	void AddBinary(int _value){
		if(_value <= value){
			if(LChild == NULL)
				LChild = new BinaryNode(_value);
			else
				LChild->AddBinary(_value);
		}else{
			if(RChild ==NULL){
				RChild = new BinaryNode(_value);
			}else{
				RChild->AddBinary(_value);
			}
		}
	}

	void AVLRotation(){
		/*Rotation doesn't change the InorderTraversal*/
		/*There are to rotations: left rotation and right rotation. They are reversable*/
/*
		+---+                          +---+
        | Q |                          | P |
        +---+                          +---+
       /     \     right rotation     /     \
    +---+   +---+  ------------->  +---+   +---+
    | P |   | Z |                  | X |   | Q |
    +---+   +---+  <-------------  +---+   +---+
   /     \          left rotation         /     \
+---+   +---+                          +---+   +---+
| X |   | Y |                          | Y |   | Z |
+---+   +---+                          +---+   +---+
                                                                  __
                                                                 /  \
                                     +---+                      /  +---+
                                     | Q |                     /   | Q |
                           +---+     +---+              +---+ /    +---+
        +---+              | P |    /     \      R1     | P |/    /     \              +---+
        | Q |     R0       +---+   /     +---+ ----->   +---+    /     +---+   R2      | P |
        +---+   ----->    /     \ /      | Z |         /        /      | Z | ----->    +---+
       /     \         +---+   +---+     +---+      +---+    +---+     +---+          /     \
    +---+   +---+      | X |   | Y |                | X |    | Y |                 +---+   +---+
    | P |   | Z |      +---+   +---+                +---+    +---+                 | X |   | Q |
    +---+   +---+              __                                                  +---+   +---+
   /     \                    /  \                                                        /     \
+---+   +---+     L2       +---+  \                       +---+                L0      +---+   +---+
| X |   | Y |   <-----     | P |   \                      | P |              <-----    | Y |   | Z |
+---+   +---+              +---+    \ +---+      L1       +---+     +---+              +---+   +---+
                          /     \    \| Q |    <-----    /     \    | Q |
                       +---+     \    +---+           +---+     \   +---+
                       | X |      \        \          | X |      \ /     \
                       +---+     +---+    +---+       +---+     +---+   +---+
                                 | Y |    | Z |                 | Y |   | Z |
                                 +---+    +---+                 +---+   +---+
		*/
		/*Need work...*/
	}

	void PreorderTranversal(){
		cout<<value<<" ";
		if(LChild != NULL) LChild->PreorderTranversal();
		if(RChild != NULL) RChild->PreorderTranversal();
	}

	void InorderTranversal(){
		if(LChild != NULL) LChild->InorderTranversal();
		cout<<value<<" ";
		if(RChild != NULL) RChild->InorderTranversal();
	}

	void PostorderTranversal(){	
		if(LChild != NULL) LChild->PostorderTranversal();
		if(RChild != NULL) RChild->PostorderTranversal();
		cout<<value<<" ";
	}
};

/*General Node*/
class Node{
public:
	int value;
	list<Node> children;
	bool visited;
	Node(int _value){
		value = _value;
		visited = false;
	}
	void AddChild(Node node){//add from left to right
		children.push_back(node);
	}
	int ChildrenCount(){
		return children.size();
	}
	Node* GetChild(int index){
		list<Node>::iterator iter = children.begin();
		for(int i = 0; i < ChildrenCount(); i++, iter++){
			if(i==index){
				return &(*iter);
			}
		}
		return NULL;
	}

	Node* GetLeft(){
		list<Node>::iterator iter = children.begin();
		if(iter!=children.end()){
			return &(*iter);
		}
		return NULL;
	}

	Node* GetRight(){
		list<Node>::iterator iter = children.begin();
		if(iter!=children.end()){
			iter++;
			if(iter!=children.end()){
				return &(*iter);
			}
		}
		return NULL;
	}

	void PreorderTranversal(){//m->l->r
		cout<<value<<" ";
		for(list<Node>::iterator iter = children.begin();
			iter != children.end(); iter++){
			(*iter).PreorderTranversal();
		}
		
	}
	void InorderTranversal(){//l->m->r
		Node *L = GetLeft();
		if(L!= NULL) L->InorderTranversal();
		
		cout<<value<<" ";

		Node *R= GetRight();
		if(R!= NULL) R->InorderTranversal();
	}
	void PostorderTranversal(){//l->r->m
		for(list<Node>::iterator iter = children.begin();
			iter != children.end(); iter++){
			(*iter).PostorderTranversal();
		}
		cout<<value<<" ";
	}
	
};

list<Node> stack;

void BFSLoop(){
	if(stack.size() != 0){
		list<Node>::iterator xiter = stack.begin();
		Node x(0);
		x = *xiter;

		cout<<x.value<<" ";
		x.visited = true;

		stack.pop_front();

		for(list<Node>::iterator iter = x.children.begin();iter!= x.children.end(); iter++){
			if((*iter).visited == false)
				stack.push_back(*iter);
		}

		BFSLoop();
	}
}

void BFS(Node x){
	stack.clear();
	stack.push_back(x);
	BFSLoop();
}

void DFSLoop(){
	if(stack.size() != 0){
		list<Node>::iterator xiter = stack.begin();
		Node x(0);
		x = *xiter;

		cout<<x.value<<" ";
		x.visited = true;

		stack.pop_front();

		for(list<Node>::iterator iter = x.children.begin();iter!= x.children.end(); iter++){
			if((*iter).visited == false){
				stack.push_back(*iter);
				DFSLoop();
			}
		}
	}
}

void DFS(Node x){
	stack.clear();
	stack.push_back(x);
	DFSLoop();
}

void PrintAll(BinaryNode root){
	cout<<"PreorderTranversal"<<endl;
	root.PreorderTranversal();
	cout<<endl;

	cout<<"InorderTranversal"<<endl;
	root.InorderTranversal();
	cout<<endl;

	cout<<"PostorderTranversal"<<endl;
	root.PostorderTranversal();
	cout<<endl;
}

void PrintAll(Node root){
	cout<<"PreorderTranversal"<<endl;
	root.PreorderTranversal();
	cout<<endl;

	cout<<"InorderTranversal"<<endl;
	root.InorderTranversal();
	cout<<endl;

	cout<<"PostorderTranversal"<<endl;
	root.PostorderTranversal();
	cout<<endl;

	cout<<"BFS"<<endl;
	BFS(root);
	cout<<endl;

	cout<<"DFS"<<endl;
	DFS(root);
	cout<<endl;
}

void TreeTest(){
	cout << "=====Tree Test=====" << endl;

	cout<<"General tree:"<<endl;
	Node root(0);
	root.AddChild(1);
	root.AddChild(2);
	root.GetChild(0)->AddChild(3);
	root.GetChild(0)->AddChild(4);
	root.GetChild(1)->AddChild(5);
	root.GetChild(1)->AddChild(6);
	root.GetChild(0)->GetChild(0)->AddChild(7);
	PrintAll(root);

	cout<<endl;

	//=============================================
	cout<<"Binary Search tree:"<<endl;

	srand(45);

	BinaryNode binaryRoot(50);
	for(int i =0;i < 20; i++){
		binaryRoot.AddBinary(rand()%100);
	}
	PrintAll(binaryRoot);

	cout<<endl;
	
	//===============================================
	cout<<"Heap: "<<endl;
	Heap heap;
	for(int i =0;i < 20; i++){
		heap.AddNode(rand()%100);
	}
	heap.Tranverse();
	heap.BuildHeap();
	heap.Tranverse();
	cout<<"Sort"<<endl;
	for(int i = 0; i < 19; i++){
		cout<<heap.Sort()<<" ";
	}
	cout<<endl;
	heap.Tranverse();
	cout<<endl;
	
}


#endif