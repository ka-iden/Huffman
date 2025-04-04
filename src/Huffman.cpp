#include "Huffman.h"

#include <queue>
#include <functional>
#include <stack>


Huffman::Huffman(std::string text)
	: text(text)
{
	for (char c : text)
		frequencies[c] += 1;
}

std::string Huffman::encode()
{
	std::priority_queue<Node*, std::vector<Node*>, NodeComparator> queue; // create a priority queue of nodes
	for (auto pair : frequencies) // create a leaf node for each character and add it to the priority queue
		queue.push(new Leaf(pair.second, pair.first));
	while (queue.size() > 1) // combine nodes and pop them from the queue until only the root remains
	{
		Node* left = queue.top();
		queue.pop();
		Node* right = queue.top();
		queue.pop();
		Node* parent = new Node(left, right);

		queue.push(parent); // add the new node to the queue
	}
	generateHuffmanCodes(root = queue.top(), ""); // set the root node and generate codes

	std::string encodedText;
	for (char c : text)
		encodedText += codes[c];
	return encodedText;
}

void Huffman::generateHuffmanCodes(Node* node, std::string code)
{
	if (!root)
		return;

	std::stack<std::pair<Node*, std::string>> stack; // somehow this avoids recursive stack overflow?
	stack.push(std::make_pair(root, "")); // use std::make_pair to initialize the stack

	while (!stack.empty()) // while there are nodes to process
	{
		auto pair = stack.top(); // get the top node and code
		stack.pop();

		Node* node = pair.first;
		std::string code = pair.second;

		if (Leaf* leaf = dynamic_cast<Leaf*>(node))
			codes[leaf->character] = code; // store huffman code for the character
		else
		{
			if (node->right) stack.push(std::make_pair(node->right, code + "1"));
			if (node->left) stack.push(std::make_pair(node->left, code + "0"));
		}
	}
}

std::string Huffman::decode(std::string encodedText)
{
	std::string decodedText;
	Node* currentNode = root;
	for (char c : encodedText)
	{
		currentNode = (c == '0') ? currentNode->left : currentNode->right;
		if (dynamic_cast<Leaf*>(currentNode))
		{
			decodedText += static_cast<Leaf*>(currentNode)->character;
			currentNode = root;
		}
	}
	return decodedText;
}

void Huffman::printCodes()
{
	for (auto pair : codes)
		std::cout << pair.first << ": " << pair.second << std::endl;
}