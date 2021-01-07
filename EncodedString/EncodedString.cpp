#include <iostream>
#include <sstream>
using namespace std;

int shiftedBit(char code, int shift) {
	return (code - '0') << shift;
}
int decodeQuartet(const char *code) {
	return shiftedBit(code[0], 3) + shiftedBit(code[1], 2) + shiftedBit(code[2], 1) + shiftedBit(code[3], 0) + 'a';
}

int main()
{
	int samplesCount;
	cin >> samplesCount;
	for (int sample = 0; sample < samplesCount; ++sample) {
		int codeLength;
		cin >> codeLength;
		string code;
		cin >> code;
		const char* const codeStart = code.c_str();
		const char* const codeEnd = codeStart + codeLength;
		char* const result = new char[codeLength / 4 + 1];
		char* resultPtr = result;
		for (const char* codePtr = codeStart; codePtr != codeEnd; codePtr += 4) {
			*(resultPtr++) = decodeQuartet(codePtr);
		}
		*resultPtr = 0;
		cout << result << endl;
		delete[] result;
	}
}
