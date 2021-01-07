#include <iostream>
#include <fstream>
#include <memory>

using namespace std;

void solve_sample(istream& in) {
	int dimension,
		collisionsCount,
		x, y;

	in >> dimension >> collisionsCount >> x >> y;
	collisionsCount = 4 + collisionsCount % 4;
	int dx = 1,
		dy = 1;

	while (collisionsCount-- > 0) {
		int distanceX = dx > 0 ? dimension - x : x;
		int distanceY = dy > 0 ? dimension - y : y;
		if (distanceX == distanceY) {
			x += dx * distanceX;
			y += dy * distanceY;
			break;
		}
		else if (distanceX < distanceY) {
			x = dx > 0 ? dimension : 0;
			y += dy * distanceX;
			dx = -dx;
		}
		else {
			y = dy > 0 ? dimension : 0;
			x += dx * distanceY;
			dy = -dy;
		}
	}
	cout << x << ' ' << y << endl;
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
