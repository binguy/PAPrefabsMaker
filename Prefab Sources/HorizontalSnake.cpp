#include "C:\Users\Mysel\source\repos\PAPrefabs\prefab.h"
#include <math.h>

const float BEAT = 60. / 122;
const float DIST = 12.;

Object* parent1 = new Object(0., 15, shapes::sq, 0, false, true, true);
Object* parent2 = new Object(0., 15, shapes::sq, 0, false, true, true);

void square(float st, float x, float y) {
	Object* sq = new Object(st, 3, shapes::sq, 0, false, true);
	sq->parent = parent2;
	sq->rotEvent(0., 0.);
	sq->colEvent(0., 2);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			sq->posEvent((4 * i + j) * 2 * BEAT + 0., x - j * 2 * DIST, y, (4 * i + j) ? easeTypes::inst : easeTypes::line);

			sq->scaEvent((4 * i + j) * 2 * BEAT + 0., 1.8, 1.8, easeTypes::inst);
			sq->scaEvent((4 * i + j) * 2 * BEAT + 0.1, 1., 1., easeTypes::iSine);
			sq->colEvent((4 * i + j) * 2 * BEAT + 0.025, 2, easeTypes::line);
			sq->colEvent((4 * i + j) * 2 * BEAT + 0.1, 0, easeTypes::iSine);

			sq->scaEvent((4 * i + j) * 2 * BEAT + 0. + BEAT, 1., 1., easeTypes::line);
			sq->scaEvent((4 * i + j) * 2 * BEAT + 0.2 + BEAT, 0., 0., easeTypes::oSine);
			sq->colEvent((4 * i + j) * 2 * BEAT + 0.3 + BEAT, 2, easeTypes::inst);
		}
}

int main() {
	prefab::name = "HorizontalSnake";
	prefab::offset = 0.;
	parent1->posEvent(0., 0., 0., easeTypes::line, 1, 0., 16.);
	parent1->scaEvent(0., 1., 1.);
	parent1->rotEvent(0., 0.);
	parent1->colEvent(0., 0);

	parent2->parent = parent1;
	parent1->scaEvent(0., 1., 1.);
	parent1->rotEvent(0., 0.);
	parent1->colEvent(0., 0);
	for (int i = 0; i <= 32; i++)
		parent2->posEvent(i * BEAT, 0., (i % 2) ? -0.3 : 0.3, easeTypes::ioSine);

	for (float f = 0; f < 2.01; f += 0.1)
		square(f * BEAT, -DIST * (f - 4.), -2 * cos(3.14159265 * f));

	parent1->posEvent(8.5 * BEAT, 0., 0., easeTypes::inst, 1, 0., -16.);
	parent1->rotEvent(8.5 * BEAT, 180., easeTypes::inst);
	parent1->posEvent(16.5 * BEAT, 0., 0., easeTypes::inst, 1, 0., 16.);
	parent1->rotEvent(16.5 * BEAT, 180., easeTypes::inst);
	parent1->posEvent(24.5 * BEAT, 0., 0., easeTypes::inst, 1, 0., -16.);
	parent1->rotEvent(24.5 * BEAT, 180., easeTypes::inst);

	prefab::generate();
	return 0;
}