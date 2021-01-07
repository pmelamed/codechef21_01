#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>

using namespace std;

void solve_sample(istream& in) {
	int jacksonPacksCount,
		johnsonPacksCount;
	in >> jacksonPacksCount >> johnsonPacksCount;

	int* jacksonPacks = new int[jacksonPacksCount],
		* jacksonPacksEnd = jacksonPacks + jacksonPacksCount,
		* jacksonsPtr,
		jacksonVotes = 0,

		* johnsonPacks = new int[johnsonPacksCount],
		* johnsonPacksEnd = johnsonPacks + johnsonPacksCount,
		* johnsonsPtr,
		johnsonVotes = 0;
	for (jacksonsPtr = jacksonPacks; jacksonsPtr != jacksonPacksEnd; ++jacksonsPtr) {
		in >> *jacksonsPtr;
		jacksonVotes += *jacksonsPtr;
	}
	for (johnsonsPtr = johnsonPacks; johnsonsPtr != johnsonPacksEnd; ++johnsonsPtr) {
		in >> *johnsonsPtr;
		johnsonVotes += *johnsonsPtr;
	}
	sort(jacksonPacks, jacksonPacksEnd);
	sort(johnsonPacks, johnsonPacksEnd, greater<int>());
	jacksonsPtr = jacksonPacks;
	johnsonsPtr = johnsonPacks;
	while (jacksonVotes <= johnsonVotes && jacksonsPtr != jacksonPacksEnd && johnsonsPtr != johnsonPacksEnd && *jacksonsPtr < *johnsonsPtr) {
		int delta = *(johnsonsPtr++) - *(jacksonsPtr++);
		jacksonVotes += delta;
		johnsonVotes -= delta;
	}
	if (jacksonVotes>johnsonVotes) {
		cout << jacksonsPtr - jacksonPacks << endl;
	}
	else {
		cout << -1 << endl;
	}
	delete[] johnsonPacks;
	delete[] jacksonPacks;
}

void solve(istream& in) {
	int samplesCount;
	in >> samplesCount;
	for (int sample = 0; sample != samplesCount; ++sample) {
		solve_sample(in);
	}
}

int main(int argc, char** argv)
{
	if (argc > 1) {
		unique_ptr<ifstream> in(new ifstream(argv[1]));
		solve(*in);
	}
	else {
		solve(cin);
	}
}
