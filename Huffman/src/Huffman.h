#pragma once
#include "Node.h"

#include <iostream>
#include <unordered_map>

class Huffman
{
public:
	Huffman(std::string text);
	std::string encode();
	std::string decode(std::string encodedText);

	void printCodes();

private:
	std::string text;
	std::unordered_map<char, int> frequencies;
	std::unordered_map<char, std::string> codes;
	Node* root = nullptr;

	void generateHuffmanCodes(Node* node, std::string code);
};