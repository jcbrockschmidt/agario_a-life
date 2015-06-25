/* DOIT:
 * - Use ampersand references instead of pointer references for
 *   add and subtract functions for CoordVect.
 * - Enable CoordVect to store ints as well as doubles.
 */

#define _USE_MATH_DEFINES
#include "math.h"
#include "blobs.h"
#include "general.h"
#include "simulation.h"

CoordVect::CoordVect(double x_new, double y_new)
{
	CoordVect::set(x_new, y_new);
}

void CoordVect::set(double x_new, double y_new)
{
	x = x_new;
	y = y_new;
}

void CoordVect::set(CoordVect &set_vect)
{
	CoordVect::set(set_vect.x, set_vect.y);
}

void CoordVect::add(double x_add, double y_add)
{
	x += x_add;
	y += y_add;
}

void CoordVect::add(CoordVect &add_vect)
{
	CoordVect::add(add_vect.x, add_vect.y);
}

void CoordVect::sub(double x_sub, double y_sub)
{
	x -= x_sub;
	y -= y_sub;
}

void CoordVect::sub(CoordVect &sub_vect)
{
	CoordVect::sub(sub_vect.x, sub_vect.y);
}

double CoordVect::rads(void)
{
	if (x == 0.0) {
		if (y > 0.0)
			return 0.5*M_PI;
		else if (y < 0.0)
			return 1.5*M_PI;
		else
			return 0.0;
	}
	double r;
	r = atan(y/x);
	if (x < 0.0 && y >= 0.0)
		r += M_PI;
	else if (x < 0.0 && y < 0.0)
		r += M_PI;
	else if (x > 0.0 && y < 0.0)
		r = 2.0*M_PI + r;
	return r;
}



Blob::Blob(double size_new, double x_new, double y_new)
: pos(x_new, y_new), vel()
{
	size = size_new;
}

void Blob::setSize(double size_set)
{
	size = size_set;
}

void Blob::addSize(double size_add)
{
	size += size_add;
}

void Blob::setPos(double x_new, double y_new)
{
 	pos.set(x_new, y_new);
}

void Blob::setPos(CoordVect &set_vect)
{
	pos.set(set_vect);
}

void Blob::addPos(double x_add, double y_add)
{
	pos.add(x_add, y_add);
}

void Blob::addPos(CoordVect &add_vect)
{
	pos.add(add_vect);
}

void Blob::setVel(double x_new, double y_new)
{
	vel.set(x_new, y_new);
}

void Blob::setVel(CoordVect &add_vect)
{
	vel.set(add_vect);
}

void Blob::addVel(CoordVect &add_vect)
{
	vel.add(add_vect);
}

void Blob::update(void)
{
	/* Apply friction */
	double rads = vel.rads();
	CoordVect fric(sim::friction*cos(rads), sim::friction*sin(rads));
	CoordVect newVel(vel);
	newVel.sub(fric);
	if (vel.x > 0.0)
		newVel.x = max(0.0, newVel.x);
	else if (vel.x <= 0.0)
		newVel.x = min(0.0, newVel.x);
	if (vel.y > 0.0)
		newVel.y = max(0.0, newVel.y);
	else if (vel.y <= 0.0)
		newVel.y = min(0.0, newVel.y);
	vel.set(newVel);

	/* Update position */
	pos.add(vel);
}
