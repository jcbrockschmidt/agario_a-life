/* DOIT:
 * - Change size before (not after) boundary correction.
 */

#define _USE_MATH_DEFINES
#include <math.h>
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

void CoordVect::set_x(double x_new)
{
	x = x_new;
}

void CoordVect::set_y(double y_new)
{
	y = y_new;
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

void CoordVect::mul(double x_mul, double y_mul)
{
	x *= x_mul;
	y *= y_mul;
}

void CoordVect::mul(double xy_mul)
{
	CoordVect::mul(xy_mul, xy_mul);
}

void CoordVect::mul(CoordVect &mul_vect)
{
	CoordVect::mul(mul_vect.x, mul_vect.y);
}

void CoordVect::div(double x_div, double y_div)
{
	x /= x_div;
	y /= y_div;
}

void CoordVect::div(double xy_div)
{
	CoordVect::div(xy_div, xy_div);
}

void CoordVect::div(CoordVect &div_vect)
{
	CoordVect::div(div_vect.x, div_vect.y);
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
	Blob::setSize(size_new);
}

void Blob::setSize(double size_set)
{
	size = size_set;
	maxVel = 25*pow(0.9830, size) + 1;
}

void Blob::addSize(double size_add)
{
	Blob::setSize(size + size_add);
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
	CoordVect newPos(pos);
	newVel.sub(fric);
	if (vel.x >= 0.0)
		newVel.x = max(0.0, newVel.x);
	else if (vel.x < 0.0)
		newVel.x = min(0.0, newVel.x);
	if (vel.y >= 0.0)
		newVel.y = max(0.0, newVel.y);
	else if (vel.y < 0.0)
		newVel.y = min(0.0, newVel.y);

	/* Make sure velocity's magnitude is not too great */
	if (newVel.x*newVel.x + newVel.y*newVel.y > maxVel*maxVel)
		newVel.set(maxVel*cos(rads), maxVel*sin(rads));

	/* Apply velocity */
	vel.set(newVel);
	newPos.add(vel);

	/* Bounds correction */
	if (newPos.x < 0.0) {
		newPos.set_x(0.0);
		if (vel.x < 0.0) vel.set_x(0.0);
		vel.set_x(0.0);
	} else if (newPos.x + size > sim::bounds.x) {
		newPos.set_x(sim::bounds.x - size);
		if (vel.x > 0.0) vel.set_x(0.0);
	}
	if (newPos.y < 0.0) {
		newPos.set_y(0.0);
		if (vel.y < 0.0) vel.set_y(0.0);
		vel.set_y(0.0);
	} else if (newPos.y + size > sim::bounds.y) {
		newPos.set_y(sim::bounds.y - size);
		if (vel.y > 0.0) vel.set_y(0.0);
	}

	/* Apply changes to position */
	pos.set(newPos);
}

Food::Food(double x_new, double y_new)
{
	pos = CoordVect(x_new, y_new);
}

bool testAABBAABB(double x1, double y1, double w1, double h1,
	  double x2, double y2, double w2, double h2)
{
	if (x1 + w1 < x2)
		return false;
	else if (x2 + w2 < x1)
		return false;
	else if (y1 + w1 < y2)
		return false;
	else if (y2 + w2 < y1)
		return false;
	else
		return true;
}

bool testAABBAABB(CoordVect &vec1, double side1, CoordVect &vec2, double side2)
{
	return testAABBAABB(vec1.x, vec1.y, side1, side1, vec2.x, vec2.y, side2, side2);
}
