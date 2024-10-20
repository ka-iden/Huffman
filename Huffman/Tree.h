#pragma once
#include <fstream>
#include <unordered_map>
#include <queue>

struct Node
{
	char ch;
	int freq;
	Node* left;
	Node* right;

	Node(char ch, int freq, Node* left = nullptr, Node* right = nullptr)
		: ch(ch), freq(freq), left(left), right(right) {}
};

struct Compare
{
	bool operator()(Node* l, Node* r) { return l->freq > r->freq; }
};

void buildHuffmanCodes(Node* root, std::string str, std::unordered_map<char, std::string>& huffmanCode)
{
	if (root == nullptr) return;

	if (!root->left && !root->right) huffmanCode[root->ch] = str;

	buildHuffmanCodes(root->left, str + "0", huffmanCode);
	buildHuffmanCodes(root->right, str + "1", huffmanCode);
}

Node* buildHuffmanTree(const std::string& text)
{
	std::unordered_map<char, int> freq;
	for (char ch : text) freq[ch]++;

	std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

	for (const auto& pair : freq) pq.push(new Node(pair.first, pair.second));

	while (pq.size() > 1)
	{
		Node* left = pq.top(); pq.pop();
		Node* right = pq.top(); pq.pop();

		int sum = left->freq + right->freq;
		pq.push(new Node('\0', sum, left, right));
	}

	return pq.top();
}

void saveHuffmanTree(std::ofstream& outFile, Node* root)
{
	if (root == nullptr)
	{
		outFile << "# ";
		return;
	}

	switch (root->ch)
	{
	case ' ':
		outFile << '~'; break;
	case '\n':
		outFile << '}'; break;
	default:
		outFile << root->ch; break;
	}
	outFile << " " << root->freq << " ";
	saveHuffmanTree(outFile, root->left);
	saveHuffmanTree(outFile, root->right);
}

Node* loadHuffmanTree(std::ifstream& inFile)
{
	char ch;
	if (!(inFile >> ch)) return nullptr;
	if (ch == '#') return nullptr;
	if (ch == '~') ch = ' ';
	if (ch == '}') ch = '\n';

	int freq;
	if (!(inFile >> freq)) return nullptr;
	Node* node = new Node(ch, freq);

	node->left = loadHuffmanTree(inFile);
	node->right = loadHuffmanTree(inFile);

	return node;
}