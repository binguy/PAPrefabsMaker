#include "C:\Users\Mysel\source\repos\PAPrefabs\prefab.h"
#include <math.h>

const float PI = 4 * atan(1.);
const float BEAT = 60. / 138;
// Helper consts
const int num_lines = 50;
const float r = 12.;

// The entire prefab position will depend of this object's.
Object* parent = new Object(0., 15, shapes::circ, 1, false, true, false);

// To make a circle with spikes that moves from center to outside
void ball(float t, float size, float ang) {
    Object* circle = new Object(t, 13, shapes::circ, 0, false, true, false); // Create the circle
    circle->parent = parent;
    circle->posEvent(0., 0., 0.);
    circle->posEvent(16 * BEAT, 120 * cos(ang * PI / 180.), 120 * sin(ang * PI / 180.)); // Moves 120 units at certain angle in 16 beats
    circle->scaEvent(0., size, size); // Given size
    circle->rotEvent(0., 0.);
    circle->rotEvent(16 * BEAT, -4 * 360.); // Spins
    circle->colEvent(0., 0); // Pink

    Object* white = new Object(t, 12, shapes::circ, 0, false, true, false); // White circle effect
    white->parent = circle; // It will follow the main pink circle
    white->posEvent(0., 0., 0.);
    white->scaEvent(0., 2., 2.);
    white->scaEvent(BEAT, 0., 0., easeTypes::iSine); // Shrinks
    white->rotEvent(0., 0.0);
    white->colEvent(0., 2); // White
    white->colEvent(BEAT, 0, easeTypes::iSine);

    // Adding spikes
    static const int num_spikes = 8;
    for (int i = 0; i < num_spikes; i++) {
        float a = 360. / num_spikes * i; // Angle of spike
        Object* tr = new Object(t, 14, shapes::tr, 0, false, true, false);
        tr->parent = circle;
        tr->pt = "111"; // Follow position, scale and rotation of parent.
        tr->posEvent(0., 0.5 * cos(a * PI / 180), 0.5 * sin(a * PI / 180)); // Will be 0.5 from center
        tr->scaEvent(0., 0.2, 0.3);
        tr->rotEvent(0., a - 90.);
        tr->colEvent(0., 0);
        tr->colEvent(16 * BEAT, 0); // Lasts for 16 beats
    }
}

int main()
{
    prefab::name = "FOXBomb";
    prefab::offset = -4 * BEAT;
    prefab::type = 0;

    // Circle starts not appearing
    parent->posEvent(0., 0., 0.);
    parent->scaEvent(0., 0., 0.);
    parent->rotEvent(0., 0.);
    parent->colEvent(0., 0);
    // Remains not appearing and grows at 4th beat
    parent->scaEvent(3.9 * BEAT, 0., 0.);
    parent->scaEvent(4.0 * BEAT, r, r, easeTypes::oSine);
    parent->scaEvent(4.5 * BEAT, 0., 0., easeTypes::iSine);
    // Spins its children till then
    parent->rotEvent(4.0 * BEAT, -90.);
    // Goes white while growing
    parent->colEvent(3.0 * BEAT, 0);
    parent->colEvent(4.0 * BEAT, 2, easeTypes::oSine);
    parent->colEvent(4.25 * BEAT, 0, easeTypes::oSine);

    // Helpers
    for (float a = 0.; a < 360.; a += 360. / num_lines) {
        Object* line = new Object(0., 25, shapes::sq, 0, true, true, false);
        line->parent = parent;
        line->posEvent(0., r * cos(a * PI / 180.) / 2, r * sin(a * PI / 180.) / 2);
        line->scaEvent(0., .15, .85 * 2 * PI * r / num_lines);
        line->rotEvent(0., a);
        line->colEvent(0., 0);
        line->colEvent(4 * BEAT, 0);
    }
    // Creating the bullets during 3 beats
    for (float t = 4 * BEAT, ang = 90., size = 1.; t < 7 * BEAT; t += 0.02, ang -= 15.5, size += 0.03)
        ball(t, size, ang);
    prefab::generate();

}