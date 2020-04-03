#ifndef BST_H
#define BST_H

template<typename Comparable>
class BSTNode{
public:
	BSTNode() = default;
	BSTNode(Comparable data, BSTNode* parent = nullptr);

	BSTNode *parent = nullptr;
	BSTNode *leftChild = nullptr;
	BSTNode *rightChild = nullptr;
	Comparable data;
};

template<typename Comparable, typename Comp>
class BST
{
public:
	BST() = default;

	using Node = BSTNode<Comparable>;
	Node* head = nullptr;

	void insert(Comparable data);
	void remove();
	Comparable getMin();
	Comparable getMax();
};

#endif // BST_H
