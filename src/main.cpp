#include "Huffman.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::vector<uint8_t> packBitsToBytes(const std::string& bitString) {
	std::vector<uint8_t> bytes;
	uint8_t currentByte = 0;
	int bitCount = 0;

	for (char bitChar : bitString) {
		currentByte <<= 1;
		if (bitChar == '1') currentByte |= 1;
		bitCount++;

		if (bitCount == 8) {
			bytes.push_back(currentByte);
			currentByte = 0;
			bitCount = 0;
		}
	}

	if (bitCount > 0) { // pad remaining bits
		currentByte <<= (8 - bitCount);
		bytes.push_back(currentByte);
	}

	return bytes;
}

std::string unpackBytesToBits(const std::vector<uint8_t>& bytes, size_t originalBitLength) {
	std::string bitString;
	for (uint8_t byte : bytes) {
		for (int i = 7; i >= 0; --i) {
			bitString += ((byte >> i) & 1) ? '1' : '0';
		}
	}
	return bitString.substr(0, originalBitLength); // remove padding
}

void writeBytesToFile(const std::string& filename, const std::vector<uint8_t>& bytes) {
	std::ofstream file(filename, std::ios::binary);
	if (!file) throw std::runtime_error("Failed to open file for writing");
	file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}

std::vector<uint8_t> readBytesFromFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file) throw std::runtime_error("Failed to open file for reading");

	return std::vector<uint8_t>(
		std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>()
	);
}

int main()
{
	std::string unencodedText = R"(A grasshopper spent the summer hopping about in the sun and singing to his heart's content. One day, an ant went hurrying by, looking very hot and weary.

"Why are you working on such a lovely day?" said the grasshopper.

"I'm collecting food for the winter," said the ant, "and I suggest you do the same." And off she went, helping the other ants to carry food to their store. The grasshopper carried on hopping and singing. When winter came the ground was covered with snow. The grasshopper had no food and was hungry. So he went to the ants and asked for food.

"What did you do all summer when we were working to collect our food?" said one of the ants.

"I was busy hopping and singing," said the grasshopper.

"Well," said the ant, "if you hop and sing all summer, and do no work, then you must starve in the winter.


Testing ASCII - (char)32 to (char)126 is expected to work.
 !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
 !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~)";

	Huffman huffman(unencodedText);

	std::string encodedText = huffman.encode();
	std::string nameOfFile = "encoded.huff";

	size_t originalBitLength = encodedText.size();
	std::vector<uint8_t> packedBytes = packBitsToBytes(encodedText);
	writeBytesToFile(nameOfFile, packedBytes);


	huffman.printCodes();
	
	auto readBytes = readBytesFromFile(nameOfFile);
	std::string unpackedBits = unpackBytesToBits(readBytes, originalBitLength);
	std::string decoded = huffman.decode(unpackedBits);
	std::cout << "Decoded text: " << decoded << std::endl;

	std::cout << "Size of original text: " << unencodedText.size() << " bytes" << std::endl;
	std::cout << "Size of encoded text: " << (encodedText.size() + 7) / 8 << " bytes" << std::endl;
	float percentReduction = ((float)unencodedText.size() - (float)(encodedText.size() + 7) / 8) / (float)((encodedText.size() + 7) / 8) * 100;
	std::cout << "Percent reduction: " << percentReduction << "%" << std::endl;
	std::cout << "Size compared to original: " << (float)100 - percentReduction << "%" << std::endl;
}