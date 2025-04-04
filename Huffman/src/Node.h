#pragma once

class Node
{
public:
	Node(Node* left, Node* right) // catch null reads for freq
		: frequency((left ? left->frequency : 0) + (right ? right->frequency : 0)), left(left), right(right) {}
	virtual ~Node() = default; // add virtual destructor to make the class polymorphic
	int getFrequency() const { return frequency; } // get frequency of the node
	Node* left;
	Node* right;
protected:
	int frequency;
};

class Leaf : public Node
{
public: // can't access base class constructor without this.xxx
	Leaf(int frequency, char character) : Node(nullptr, nullptr), character(character) { this->frequency = frequency; }
	char character;
};

struct NodeComparator
{
	bool operator()(Node* left, Node* right)
	{
		return left->getFrequency() > right->getFrequency();
	}
};