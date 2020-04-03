#include "bst.h"

template<typename Comparable>
BSTNode<Comparable>::BSTNode(Comparable data, BSTNode *parent):
	data(data), parent(parent)
{
}

template<typename Comparable, typename Comp>
void BST<Comparable, Comp>::insert(Comparable data)
{
	if(head == nullptr){
		head = new Node(data);
	}else{
		Node* temp = head;
		for(;;){
			if(temp->data < data){
				if(temp->leftChild != nullptr){
					temp = temp->leftChild;
				}
				else {
					temp->leftChild = new Node(data, temp);
				}
			}else{
				temp = temp->rightChild;
			}
		}
	}
}
