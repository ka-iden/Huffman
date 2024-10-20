#include <iostream>
#include <sstream>

#include "Tree.h"

static std::vector<uint8_t> encode(const std::string& text, const std::unordered_map<char, std::string>& huffmanCode, size_t& bitLength)
{
	std::string encodedString;
	for (char ch : text) encodedString += huffmanCode.at(ch);
	bitLength = encodedString.size();
	std::vector<uint8_t> byteArray((bitLength + 7) / 8, 0);
	for (size_t i = 0; i < bitLength; ++i) if (encodedString[i] == '1') byteArray[i / 8] |= (1 << (7 - (i % 8)));
	return byteArray;
}

std::string decode(const std::vector<uint8_t>& byteArray, Node* root)
{
	std::string decodedString;
	Node* currentNode = root;
	for (size_t i = 16; i < byteArray.size() * 8 - 1; ++i)
	{
		uint8_t byte = byteArray[i / 8];
		bool bit = byte & (1 << (7 - (i % 8)));

		currentNode = bit ? currentNode->right : currentNode->left;
		if (!currentNode->left && !currentNode->right) // when this breaks, im going to kill someone
		{
			decodedString += currentNode->ch;
			currentNode = root;
		}
	}
	return decodedString;
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
		return 1;
	}
	if (strcmp(strrchr(argv[1], '.'), ".txt") == 0 && strcmp(strrchr(argv[2], '.'), ".huff") == 0)
	{
		std::ifstream inputFile(argv[1]); // ex: input.txt
		if (!inputFile.is_open())
		{
			std::cerr << "Failed to open input file." << std::endl;
			return 1;
		}
		std::string text((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
		std::cout << "Unencoded text size: " << text.size() << " bytes" << std::endl;
		inputFile.close();
		Node* root = buildHuffmanTree(text);
		std::unordered_map<char, std::string> huffmanCode;
		buildHuffmanCodes(root, "", huffmanCode);
		size_t bitLength = 0;
		std::vector<uint8_t> encodedBytes = encode(text, huffmanCode, bitLength);
		std::cout << "Encoded text size: " << encodedBytes.size() << " bytes" << std::endl;

		std::ofstream outputFile(argv[2], std::ios::binary);
		if (!outputFile.is_open()) // ex: output.huff
		{
			std::cerr << "Failed to open output file." << std::endl;
			return 1;
		}
		saveHuffmanTree(outputFile, root);
		outputFile << "\n";
		outputFile.write(reinterpret_cast<const char*>(&bitLength), sizeof(bitLength));
		outputFile << "\n";
		outputFile.write(reinterpret_cast<const char*>(encodedBytes.data()), encodedBytes.size());
		outputFile.close();
		float percentReduction = (float)(text.size() - encodedBytes.size()) / text.size() * 100; // dont ask how long it took
		std::cout << "File compressed successfully. " << percentReduction << "% reduction" << std::endl; // too long
		return 0;
	}
	if (strcmp(strrchr(argv[1], '.'), ".huff") == 0 && strcmp(strrchr(argv[2], '.'), ".txt") == 0)
	{
		std::ifstream encodedFile(argv[1], std::ios::binary);
		if (!encodedFile.is_open()) // ex: input.huff
		{
			std::cerr << "Failed to open encoded file." << std::endl;
			return 1;
		}
		Node* loadedRoot = loadHuffmanTree(encodedFile);

		std::string line;
		std::getline(encodedFile, line); // skips first line :DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD im going to kms
		//size_t bitLength; // unneeded now because for some reason, skipping specifically 7 bytes does the trick
		//encodedFile.read(reinterpret_cast<char*>(&bitLength), sizeof(bitLength)); // making me angry holy moly
		encodedFile.ignore(7); // Ignore the additional newline after the bit length, why 7? i dont FUCKING KNOW

		std::vector<uint8_t> loadedEncodedBytes((std::istreambuf_iterator<char>(encodedFile)), std::istreambuf_iterator<char>());
		encodedFile.close();
		std::string decodedString = decode(loadedEncodedBytes, loadedRoot);
		std::ofstream outputFile(argv[2]);
		if (!outputFile.is_open()) // ex: output.txt
		{
			std::cerr << "Failed to open output file." << std::endl;
			return 1;
		}
		outputFile << decodedString;
		outputFile.close();
		std::cout << "Decoded string: " << decodedString << std::endl;
		return 0;
	}
	else
	{
		std::cerr << "Invalid file extensions. only accepting .txt and .huff" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
		return 1;
	}
	return 0;
}