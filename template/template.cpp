#include <iostream>
#include <fstream>
#include <memory>

using namespace std;

void solve_sample(istream& in);

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
