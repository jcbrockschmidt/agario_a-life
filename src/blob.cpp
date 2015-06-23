#include "blobs.h"

using namespace std;

CoordVect::CoordVect(double x_new, double y_new)
{
	CoordVect::set(x_new, y_new);
}

void CoordVect::set(double x_new, double y_new)
{
	x = x_new;
	y = y_new;
}

void CoordVect::set(CoordVect *set_vect)
{
	CoordVect::set(set_vect->x, set_vect->y);
}

void CoordVect::add(double x_add, double y_add)
{
	x += x_add;
	y += y_add;
}

void CoordVect::add(CoordVect *add_vect)
{
	CoordVect::add(add_vect->x, add_vect->y);
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

void Blob::setPos(CoordVect *set_vect)
{
	pos.set(set_vect);
}

void Blob::addPos(double x_add, double y_add)
{
	pos.add(x_add, y_add);
}

void Blob::addPos(CoordVect *add_vect)
{
	pos.add(add_vect);
}

void Blob::setVel(double x_new, double y_new)
{
	vel.set(x_new, y_new);
}

void Blob::setVel(CoordVect *add_vect)
{
	vel.set(add_vect);
}

void Blob::addVel(CoordVect *add_vect)
{
	vel.add(add_vect);
}
