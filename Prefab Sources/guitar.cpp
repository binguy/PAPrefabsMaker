
#include "C:\Users\Mysel\source\repos\PAPrefabs\prefab.h"
#include <math.h>
#include <stdlib.h>
#include <algorithm>

#define RIGHT 1
//Object *picker = new Object(0.,15,shapes::sq,0,false,true,true);

const float BEAT = 60. / 122;
const float win = 0.025;
const float w = 1.5;
const float H = 39.8 * 18 / 200;
const float d = 0.25;
const float h = H - d;
const float W = w + d / cos(atan(w / h));

const float x1 = 36., x2 = 14., x3 = 24.;
const float x2b = 1.5, x3b = 26, x4 = 32;

float r() {
	return x4 - (float)rand() / RAND_MAX * (x4 - x3);
}

float r2() {
	return 1.5 * BEAT + (float)rand() / RAND_MAX * 0.5 * BEAT;
}

std::vector<float> posVect;
std::vector<float> timeVect;

#if RIGHT
std::vector<float> v = { 0.,2.,4.,5.,6.,7. };
#else
std::vector<float> v = { 1.,3.,4.,5.,6.,7. };
#endif
void newVects() {
	timeVect.clear();
	posVect.clear();
	for (float f : v) {
		float t = 8 * f * BEAT;
		bool is2 = (f - 2 < .1) && (f - 2 > -.1);
		bool is7 = (f - 7 < .1) && (f - 7 > -.1);
		timeVect.push_back(t);
		posVect.push_back(x1);
		timeVect.push_back(t + 0.5 * BEAT - win);
		posVect.push_back(x1);
		timeVect.push_back(t + 0.5 * BEAT + win);
		posVect.push_back(is2 ? x2b : x2);
		timeVect.push_back(t + BEAT - win);
		posVect.push_back(x3b);
		timeVect.push_back(t + BEAT + win);
		posVect.push_back(is2 ? x2b : x2);
		float a = r2();
		float b = r2();
		timeVect.push_back(t + std::min(a, b));
		posVect.push_back(r());
		timeVect.push_back(t + std::max(a, b));
		posVect.push_back(r());
		timeVect.push_back(t + (is7 ? 3 : 2) * BEAT);
		posVect.push_back(is7 ? 1.6 * x1 : r());
		if (!is7) {
			timeVect.push_back(t + 2 * BEAT + 0.01);
			posVect.push_back(x1);
		}
	}
}

void whiteShape(float y) {
	Object* parent = new Object(0., 20, shapes::rTr, 2, false, true);
	Object* child = new Object(0., 20, shapes::rTr, 3, false, true);

	//parent->parent = picker;
	parent->setCenter(0.5, 0.);

	/*parent->posEvent(0.,x1,y);
	parent->posEvent(0.5*BEAT-win,x1,y);
	parent->posEvent(0.5*BEAT+win,x2,y);
	parent->posEvent(BEAT-win,x3,y);
	parent->posEvent(BEAT+win,x2,y);*/
	for (int i = 0; i < timeVect.size(); i++)
		parent->posEvent(timeVect[i], posVect[i], y);

	parent->scaEvent(0., w, -h);
	parent->rotEvent(0., 0.);
	parent->colEvent(0., 2);


	child->parent = parent;
	child->setCenter(0.5, 0.);
	child->posEvent(0., 0., 0.);
	child->scaEvent(0., -w, h);
	child->rotEvent(0., 0.);
	child->colEvent(0., 2);

	for (float f : v) {
		bool is7 = (f - 7 < .1) && (f - 7 > -.1);
		float t = 8 * f * BEAT;
		parent->colEvent(t + 1.5 * BEAT, 2);
		if (!is7) parent->colEvent(t + 2. * BEAT, 3, easeTypes::iSine);
		parent->colEvent(t + 3. * BEAT, 2);
		child->colEvent(t + 1.5 * BEAT, 2);
		if (!is7) child->colEvent(t + 2. * BEAT, 3, easeTypes::iSine);
		child->colEvent(t + 3. * BEAT, 2);
	}
}

void pinkShape1(float x, float y, float st, float ft) {
	Object* parent = new Object(st, 21, shapes::rTr, 0, false, true);
	Object* child = new Object(st, 21, shapes::rTr, 1, false, true);

	//parent->parent = picker;
	parent->setCenter(0.5, 0.);
	parent->posEvent(0., x, y);
	parent->scaEvent(0., w, -h);
	parent->rotEvent(0., 0.);
	parent->colEvent(0., 0);
	parent->colEvent(ft - st, 0);

	//child->parent = picker;
	child->setCenter(0.5, 0.);
	child->posEvent(0., x, y);
	child->scaEvent(0., -w, h);
	child->rotEvent(0., 0.);
	child->colEvent(0., 0);
	child->colEvent(ft - st, 0);
}

void pinkShape2(float x, float y) {
	Object* parent = new Object(0., 21, shapes::rTr, 1, false, true);
	Object* child = new Object(0., 21, shapes::rTr, 1, false, true);

	parent->setCenter(0.5, 0.);
	//parent->parent = picker;

	parent->posEvent(0., x1, y);

	bool izq = false;
	for (int i = 1; i < timeVect.size(); i++) {
		float t1 = timeVect[i - 1];
		float t2 = timeVect[i];
		float _x1 = posVect[i - 1];
		float _x2 = posVect[i];
		if (_x2 < x == izq)
			continue;
		float t;
		if (izq) {//_x2 estará a la derecha
			t = (x - _x1) * (t2 - t1) / (_x2 - _x1) + t1;
			parent->posEvent(t, x1, y, easeTypes::inst);
		}
		else {//_x2 estará a la izquierda
			t = (x - _x1) * (t2 - t1) / (_x2 - _x1) + t1;
			parent->posEvent(t, x, y, easeTypes::inst);
		}
		izq ^= 1;
	}

	parent->scaEvent(0., w, -h);
	parent->rotEvent(0., 0.);
	parent->colEvent(0., 0);

	child->setCenter(0.5, 0.);
	child->parent = parent;
	child->posEvent(0., 0., 0.);
	child->scaEvent(0., -w, h);
	child->rotEvent(0., 0.);
	child->colEvent(0., 0);

	for (float f : v) {
		bool is7 = (f - 7 < .1) && (f - 7 > -.1);
		float t = 8 * f * BEAT;
		parent->colEvent(t + 1.5 * BEAT, 0);
		if (!is7) parent->colEvent(t + 2. * BEAT, 3, easeTypes::iSine);
		parent->colEvent(t + 3. * BEAT, 0);
		child->colEvent(t + 1.5 * BEAT, 0);
		if (!is7) child->colEvent(t + 2. * BEAT, 3, easeTypes::iSine);
		child->colEvent(t + 3. * BEAT, 0);
	}
}

int main() {
#if RIGHT
	prefab::name = "Guitar1";
	srand(10);
#else
	prefab::name = "Guitar2";
	srand(20);
#endif
	prefab::offset = -0.5 * BEAT;

	/*picker->posEvent(0.,0.,0.,easeTypes::line,1,0.,-10*H,H);
	picker->scaEvent(0.,1.,1.);
	picker->rotEvent(0.,0.);
	picker->colEvent(0.,0);*/

	for (int i = 0; i < 10; i++) {
		float y = (-4.5 + i) * H;
		newVects();
		whiteShape(y);
#if RIGHT
		for (float x = x1; x > x2b; x -= W)
#else
		for (float x = x1; x > x2; x -= W)
#endif
			pinkShape2(x, y);
		/*
		#if RIGHT
		for(float x=x1;x>x2b;x-=W){
			if(x>x2)
				continue;
			float t = 2*8*BEAT;
			float t1 = t+(x1-x)*2*win/(x1-x2b)+0.5*BEAT-win;
			float t2 = t-(x2b-x)*(0.5*BEAT-2*win)/(x3b-x2b)+0.5*BEAT+win;
			float t3 = t+(x3b-x)*2*win/(x3b-x2b)+BEAT-win;
			float x4b = posVect[14];
			float t4 = t-(x2b-x)*(timeVect[14]-(t+BEAT+win))/(x4b-x2b)+BEAT+win;
			pinkShape1(x,y,t1,t2);
			pinkShape1(x,y,t3,t4);
		}
		#endif
		/**/
	}
	prefab::generate();
	return 0;
}