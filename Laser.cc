#include "Laser.h"
#include <iostream>
#include <allegro5/allegro_primitives.h>

__BEGIN_API

Laser::Laser(Point p, ALLEGRO_COLOR c, Vector s):centre(p), color(c), speed(s)
{

   centre = centre + speed * 0.1;
   live = true;
}

Laser::~Laser() {
   
}

void Laser::update(double dt) {
   centre = centre + speed * dt;
   if (!in_bound())
      live = false;
}

void Laser::draw() {
   Point tracer = centre + speed * (-0.05);
   al_draw_line(centre.x, centre.y,
		tracer.x, tracer.y,
		color, 3);		
}

bool Laser::in_bound() {
   if ((centre.x > 800) ||
       (centre.x < 0) ||
       (centre.y > 600) ||
       (centre.y < 0))
      return false;
   return true;
}
__END_API