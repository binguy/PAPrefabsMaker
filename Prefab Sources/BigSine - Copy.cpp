#include "C:\Users\Mysel\source\repos\PAPrefabs\prefab.h"

const float BEAT = 120. / 122;
const float DIST = 142.4 * 18 / 20 / 16;

Object* helper = new Object(0., 26, shapes::sq, 0, true, true, false);
Object* squares[32];

int main()
{
    prefab::name = "BigSineDouble";
    prefab::offset = 0.;
    for (int i = 0; i < 32; i++) {
        squares[i] = new Object(0., 2, shapes::sq, 1, false, true, false);
        squares[i]->parent = helper;
        squares[i]->scaOffset = 8 * BEAT;
        float x = DIST * (-15 + i);
        squares[i]->posEvent(0., x, 0.);
        squares[i]->scaEvent(0., 0., 0.);
        squares[i]->rotEvent(0., 0.);
        squares[i]->colEvent(0., 2);
        for (int k = 0; k < 144; k++) {
            squares[i]->posEvent(0.5 * BEAT + BEAT * k / 2 + BEAT * i / 8, x, k % 4 ? -8 : 8, easeTypes::ioSine);
        }
    }

    helper->posEvent(0., 0., 4., easeTypes::line, 2, 0., 24.);
    helper->posEvent(16 * BEAT, 0., -4., easeTypes::inst, 1, 0., -24.);
    helper->posEvent(32 * BEAT, 0., -4., easeTypes::inst, 1, 0., -24.);
    helper->posEvent(48 * BEAT, 0., -4., easeTypes::inst, 1, 0., -24.);

    helper->rotEvent(0., 0., easeTypes::inst);
    helper->rotEvent(16 * BEAT, 360., easeTypes::inst);
    //helper->rotEvent(16*BEAT,180.,easeTypes::inst);

    helper->scaEvent(0.06, 160., 0.);

    for (int i = 0; i < 8; i++) {
        helper->scaEvent(16 * BEAT * i, 160., 0.);
        helper->scaEvent(16 * BEAT * i + 8 * BEAT, 160., 24., easeTypes::oSine);
        helper->scaEvent(16 * BEAT * i + 9 * BEAT, 160., 0., easeTypes::iSine);

        //helper->rotEvent(8*BEAT*i,i?180.:0.,easeTypes::inst);

        helper->colEvent(16 * BEAT * i, 6);
        for (int j = 1; j <= 16; j++)
            helper->colEvent(16 * BEAT * i + 1 * BEAT * j, !(j % 4), easeTypes::ioSine);

        for (int j = 0; j < 32; j++) {
            Object* sq = squares[j];
            float x = DIST * (-15 + j);
            float fact = 32;
            sq->scaEvent(16 * BEAT * i + 8 * BEAT + BEAT * j / fact, 2.4 * DIST, 2.4 * DIST, easeTypes::inst);
            sq->scaEvent(16 * BEAT * i + 9 * BEAT + BEAT * j / fact, 1.8 * DIST, 1.8 * DIST, easeTypes::iSine);
            sq->scaEvent(16 * BEAT * i + 14 * BEAT + BEAT * j / fact, 1.8 * DIST, 1.8 * DIST, easeTypes::inst);
            sq->scaEvent(16 * BEAT * i + 15 * BEAT + BEAT * j / fact, 0., 0., easeTypes::iSine);

            sq->colEvent(16 * BEAT * i + 8 * BEAT + BEAT * j / fact, 4, easeTypes::line);
            sq->colEvent(16 * BEAT * i + 8.5 * BEAT + BEAT * j / fact, 0, easeTypes::iSine);
            sq->colEvent(16 * BEAT * i + 18 * BEAT + BEAT * j / fact, 4, easeTypes::inst);
        }
    }
    prefab::generate();
}
