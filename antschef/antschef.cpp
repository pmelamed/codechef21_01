#include <iostream>
#include <fstream>
#include <memory>
#include <list>
#include <math.h>
#include <algorithm>

using namespace std;

const float ZERO_DELTA = 1E-6f;
const float OUT_OF_LINE = 1E10f;

inline float sign(float x) {
	return (x > ZERO_DELTA) ? 1.0f : (x < ZERO_DELTA ? -1.0f : 0.0f);
}

inline bool is_equal(float a, float b) {
	return fabs(a - b) < ZERO_DELTA;
}
inline bool at_same_distance(float pos1, float pos2) {
	return is_equal(fabs(pos1), fabs(pos2));
}

class ant_t {
public:
	float dx;
	float pos;
	int collision;
	bool moves_to_center;
	bool moves_outside_down;
	bool moves_outside_up;
	bool purged;

	ant_t(int _line, float _pos):
		pos(_pos),
		dx(-sign(_pos)),
		collision(-1),
		moves_to_center(true),
		purged(false),
		moves_outside_down(false),
		moves_outside_up(false) {
	}

	void update_collision(int _collision) {
		if ( _collision > collision ) {
			collision = _collision;
		}
	}

	void reset_collision() {
		collision = -1;
	}

	void update_pos(float time) {
		pos += time * dx;
	}

	void update_flags() {
		moves_to_center = !is_equal(pos, 0.0f) && (pos * dx) < 0;
		moves_outside_down = (pos < ZERO_DELTA &&dx < 0);
		moves_outside_up = (pos > ZERO_DELTA && dx > 0);
	}

	void turn_around() {
		dx = -dx;
	}
};

typedef list<ant_t *> ants_list_t;
typedef ants_list_t::iterator ant_ptr_t;
typedef ants_list_t::reverse_iterator ant_rptr_t;
typedef list<ants_list_t> lines_list_t;
typedef lines_list_t::iterator line_ptr_t;

void mark_line_collisions(ants_list_t &line, int &collision_mark, float &collision_time, int &collisions_count) {
	if ( line.size() < 2 ) {
		return;
	}
	ant_ptr_t ant1_ptr = line.begin();
	ant_ptr_t ant2_ptr = ant1_ptr;
	++ant2_ptr;
	while ( ant2_ptr != line.end() ) {
		if ( (*ant1_ptr)->dx == 1 && (*ant2_ptr)->dx == -1 ) {
			float t = ((*ant2_ptr)->pos - (*ant1_ptr)->pos) / 2.0f;
			int mark = 0;
			if ( is_equal(t, collision_time) ) {
				mark = collision_mark;
				++collisions_count;
			} else if ( t < collision_time ) {
				collision_time = t;
				collisions_count = 1;
				mark = ++collision_mark;
			}
			(*ant1_ptr)->update_collision(mark);
			(*ant2_ptr)->update_collision(mark);
		}
		++ant1_ptr;
		++ant2_ptr;
	}
}

void mark_center_collisions(ants_list_t &all, int &collision_mark, float &collision_time, int &collisions_count) {
	if ( all.size() < 2 ) {
		return;
	}
	ants_list_t candidates;
	copy_if(
		all.begin(),
		all.end(),
		back_inserter(candidates),
		[collision_time](ant_t *a) {return a->moves_to_center && a->pos <= collision_time; }
	);
	candidates.sort([](ant_t *a1, ant_t *a2) { return fabs(a1->pos) < fabs(a2->pos); });
	ant_ptr_t ant1_ptr = candidates.begin();
	ant_ptr_t ant2_ptr = ant1_ptr;
	++ant2_ptr;
	float distance1 = fabs((*ant1_ptr)->pos);
	float distance2 = fabs((*ant2_ptr)->pos);
	while ( ant2_ptr != candidates.end() && !is_equal(distance1, fabs((*ant2_ptr)->pos)) ) {
		distance1 = fabs((*ant2_ptr)->pos);
		++ant1_ptr;
		++ant2_ptr;
	}
	if ( distance1 <= collision_time && ant2_ptr != candidates.end() ) {
		int mark = 0;
		if ( is_equal(distance1, collision_time) ) {
			mark = collision_mark;
			++collisions_count;
		} else if ( distance1 < collision_time ) {
			collision_time = distance1;
			mark = ++collision_mark;
			collisions_count = 1;
		}
		(*ant1_ptr)->update_collision(mark);
		while ( ant2_ptr != candidates.end() && is_equal(distance1, fabs((*ant2_ptr)->pos)) ) {
			(*ant2_ptr)->update_collision(mark);
			++ant2_ptr;
		}
	}
}

void purge_line(ants_list_t &line) {
	ant_ptr_t ant_ptr = line.begin();
	while ( !line.empty() && (*ant_ptr)->moves_outside_down ) {
		(*ant_ptr)->purged = true;
		line.pop_front();
		ant_ptr = line.begin();
	}
	ant_rptr_t ant_rptr = line.rbegin();
	while ( !line.empty() && (*ant_rptr)->moves_outside_up ) {
		(*ant_ptr)->purged = true;
		line.pop_back();
		ant_rptr = line.rbegin();
	}
}

void purge_list(ants_list_t &all) {
	for ( ant_ptr_t ant_ptr = all.begin(); ant_ptr != all.end(); ) {
		ant_t *ant = (*ant_ptr);
		if ( ant->purged ) {
			ant_ptr_t erased = ant_ptr++;
			all.erase(erased);
			delete ant;
		} else {
			++ant_ptr;
		}
	}
}

void solve_sample(istream &in) {
	ants_list_t ants;
	lines_list_t lines;
	int lines_count;
	in >> lines_count;
	for ( int line_index = 0; line_index < lines_count; ++line_index ) {
		lines.push_back(ants_list_t());
		ants_list_t &line = lines.back();
		int ants_count;
		in >> ants_count;
		for ( int ant_index = 0; ant_index < ants_count; ++ant_index ) {
			int pos;
			in >> pos;
			ant_t *a = new ant_t(line_index, (float) pos);
			ants.push_back(a);
			line.push_back(a);
			line.sort([](ant_t *a1, ant_t *a2) { return a1->pos < a2->pos; });
		}
	}
	int collisions = 0;
	do {
		int collisions_count = 0;
		float collision_time = OUT_OF_LINE;
		int collision_mark = 0;
		for_each(
			lines.begin(),
			lines.end(),
			[&](ants_list_t &line) {
				mark_line_collisions(line, collision_mark, collision_time, collisions_count);
			}
		);
		// mark_center_collisions(ants, collision_mark, collision_time, collisions_count);
		collisions += collisions_count;
		for_each(
			ants.begin(),
			ants.end(),
			[=](ant_t *ant) {
				ant->update_pos(collision_time);
				if ( ant->collision == collision_mark ) {
					ant->turn_around();
				}
				ant->update_flags();
				ant->reset_collision();
			}
		);
		for_each(lines.begin(), lines.end(), purge_line);
		purge_list(ants);
	} while ( !ants.empty() );
	cout << collisions << endl;
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
