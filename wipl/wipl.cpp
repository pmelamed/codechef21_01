#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

const int NOT_FOUND = 10000;
typedef vector<int> blocks_t;
typedef blocks_t::iterator block_ptr_t;

bool check_diffs_compensation(blocks_t &diffs, int range_min, int range_max) {
	int compensation = 0;
	for ( block_ptr_t dptr = diffs.begin(); dptr != diffs.end() && compensation < range_min; ++dptr ) {
		compensation += *dptr;
	}
	return compensation <= range_max;
}

bool try_compensate(blocks_t greater, blocks_t smaller, int range_min, int range_max) {
	if ( greater.size() == 0 || smaller.size() == 0 ) {
		return false;
	}
	block_ptr_t gptr = greater.begin();
	block_ptr_t lptr = smaller.begin();
	blocks_t diffs;
	int compensation = 0;
	for ( ; gptr != greater.end() && lptr != smaller.end(); gptr++, lptr++ ) {
		int diff = *gptr - *lptr;
		if ( diff > 0 ) {
			diffs.push_back(diff);
			compensation += diff;
		}
	}
	if ( compensation < range_min || *min_element(diffs.begin(), diffs.end()) > range_max ) {
		return false;
	}
	sort(diffs.begin(), diffs.end());
	do {
		if ( check_diffs_compensation(diffs, range_min, range_max) ) {
			return true;
		}
	} while ( next_permutation(diffs.begin(), diffs.end()) );
	return false;
}

void solve_sample(istream &in) {
	int height, blocksCount;
	in >> blocksCount >> height;
	blocks_t blocks;
	int sum = 0;
	for ( int i = 0; i != blocksCount; ++i ) {
		int block;
		in >> block;
		blocks.push_back(block);
		sum += block;
	}
	if ( sum < 2 * height ) {
		cout << -1 << endl;
		return;
	}
	sort(blocks.begin(), blocks.end(), greater<>());
	int remainder1 = height;
	int remainder2 = height;
	blocks_t tower1;
	blocks_t tower2;
	block_ptr_t blockPtr = blocks.begin();
	int last_tower = 0,
		last_block = 0;
	while ( blockPtr != blocks.end() && (remainder1 > 0 || remainder2 > 0) ) {
		int block = *blockPtr++;
		if ( remainder1 >= remainder2 ) {
			remainder1 -= block;
			tower1.push_back(block);
			last_tower = 1;
		} else {
			remainder2 -= block;
			tower2.push_back(block);
			last_tower = 2;
		}
		last_block = block;
	}
	if ( remainder1 == 0 || remainder2 == 0 ) {
		cout << blockPtr - blocks.begin() << endl;
		return;
	}
	int result = NOT_FOUND;
	result = blockPtr - blocks.begin();
	if ( remainder1 < 0 && remainder2 < 0 ) {
		// Try to remove last and compensate
		if ( last_tower == 1 ) {
			tower1.pop_back();
			remainder1 += last_block;
			if ( try_compensate(tower2, tower1, remainder1, -remainder2) ) {
				--result;
			}
		} else {
			tower2.pop_back();
			remainder2 += last_block;
			if ( try_compensate(tower1, tower2, remainder2, -remainder1) ) {
				--result;
			}
		}
	} else {
		if ( remainder1 > 0 ) {
			if ( !try_compensate(tower2, tower1, remainder1, -remainder2) ) {
				result = NOT_FOUND;
			}
		} else {
			if ( !try_compensate(tower1, tower2, remainder2, -remainder1) ) {
				result = NOT_FOUND;
			}
		}
	}
	cout << (result >= NOT_FOUND ? -1 : result) << endl;
}

void solve(istream &in) {
	int samplesCount;
	in >> samplesCount;
	for ( int sample = 0; sample != samplesCount; ++sample ) {
		solve_sample(in);
	}
}

int main(int argc, char **argv) {
	if ( argc > 1 ) {
		unique_ptr<ifstream> in(new ifstream(argv[1]));
		solve(*in);
	} else {
		solve(cin);
	}
}
