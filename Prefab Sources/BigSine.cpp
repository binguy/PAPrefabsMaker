#include "C:\Users\Mysel\source\repos\PAPrefabs\prefab.h"

const float BEAT = 60. / 122;
const float DIST = 71.2 * 18 / 20 / 16;

Object* helper = new Object(0., 26, shapes::sq, 0, true, true, false);
Object* squares[16];

int main()
{
    prefab::name = "BigSine1";
    prefab::offset = 0.;
    for (int i = 0; i < 16; i++) {
        squares[i] = new Object(0., 2, shapes::sq, 1, false, true, false);
        squares[i]->parent = helper;
        squares[i]->scaOffset = 4 * BEAT;
        float x = DIST * (-7.5 + i);
        squares[i]->posEvent(0., x, 0.);
        squares[i]->scaEvent(0., 0., 0.);
        squares[i]->rotEvent(0., 0.);
        squares[i]->colEvent(0., 2);
        for (int k = 0; k < 72; k++) {
            squares[i]->posEvent(0.25 * BEAT + BEAT * k / 1 + BEAT * i / 4, x, k % 2 ? -4 : 4, easeTypes::ioSine);
        }
    }

    helper->posEvent(0., 0., 2., easeTypes::line, 1, 0., 12.);
    helper->posEvent(8 * BEAT, 0., -2., easeTypes::inst, 1, 0., -12.);
    helper->posEvent(16 * BEAT, 0., -2., easeTypes::inst, 1, 0., -12.);
    helper->posEvent(24 * BEAT, 0., -2., easeTypes::inst, 1, 0., -12.);

    helper->rotEvent(0., 0., easeTypes::inst);
    helper->rotEvent(8 * BEAT, 180., easeTypes::inst);
    //helper->rotEvent(16*BEAT,180.,easeTypes::inst);

    helper->scaEvent(0.03, 80., 0.);

    for (int i = 0; i < 4; i++) {
        helper->scaEvent(8 * BEAT * i, 80., 0.);
        helper->scaEvent(8 * BEAT * i + 4 * BEAT, 80., 12., easeTypes::oSine);
        helper->scaEvent(8 * BEAT * i + 4.5 * BEAT, 80., 0., easeTypes::iSine);

        //helper->rotEvent(8*BEAT*i,i?180.:0.,easeTypes::inst);

        helper->colEvent(8 * BEAT * i, 3);
        for (int j = 1; j <= 8; j++)
            helper->colEvent(8 * BEAT * i + 0.5 * BEAT * j, !(j % 2), easeTypes::ioSine);

        for (int j = 0; j < 16; j++) {
            Object* sq = squares[j];
            float x = DIST * (-7.5 + j);
            float fact = 16;
            sq->scaEvent(8 * BEAT * i + 4 * BEAT + BEAT * j / fact, 1.2 * DIST, 1.2 * DIST, easeTypes::inst);
            sq->scaEvent(8 * BEAT * i + 4.5 * BEAT + BEAT * j / fact, 0.9 * DIST, 0.9 * DIST, easeTypes::iSine);
            sq->scaEvent(8 * BEAT * i + 7 * BEAT + BEAT * j / fact, 0.9 * DIST, 0.9 * DIST, easeTypes::inst);
            sq->scaEvent(8 * BEAT * i + 7.5 * BEAT + BEAT * j / fact, 0., 0., easeTypes::iSine);

            sq->colEvent(8 * BEAT * i + 4 * BEAT + BEAT * j / fact, 2, easeTypes::line);
            sq->colEvent(8 * BEAT * i + 4.25 * BEAT + BEAT * j / fact, 0, easeTypes::iSine);
            sq->colEvent(8 * BEAT * i + 9 * BEAT + BEAT * j / fact, 2, easeTypes::inst);
        }
    }
    prefab::generate();
}
