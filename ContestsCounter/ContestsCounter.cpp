#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>
using namespace std;

void solve_sample(istream& in) {
	int settersCount, problemsPerDay, maxDays;
	in >> settersCount >> problemsPerDay >> maxDays;
	int problemsCount = 0;
	for (int i = 0; i != settersCount; ++i) {
		int setterProblems;
		cin >> setterProblems;
		problemsCount += setterProblems;
	}
	cout << min(problemsCount / problemsPerDay, maxDays) << endl;
}

void solve(istream& in) {
	int samplesCount;
	cin >> samplesCount;
	for (int sample = 0; sample != samplesCount; ++sample) {
		solve_sample(in);
	}
}

int main(int argc, char** argv)
{
	if (argc > 1) {
		auto_ptr<ifstream> in(new ifstream(argv[1]));
		solve(*in.get());
	}
	else {
		solve(cin);
	}
}
