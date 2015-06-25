#ifndef BLOBS_H
#define BLOBS_H

class CoordVect
{
 public:
	double x;
	double y;

	CoordVect(double x_new = 0, double y_new = 0);
	void set(double x_new, double y_new);
	void set(CoordVect *set_vect);
	void add(double x_add, double y_add);
	void add(CoordVect *add_vect);
	void sub(double x_sub, double y_sub);
	void sub(CoordVect *sub_vect);
	double rads(void);
};

class Blob
{
public:
	CoordVect pos;
	CoordVect vel;
	double size;

	Blob(double size_new = 1, double x_new = 0, double y_new = 0);
	void setSize(double size_set);
	void addSize(double size_add);
	void setPos(double x_new, double y_new);
	void setPos(CoordVect *set_vect);
	void addPos(double x_add, double y_add);
	void addPos(CoordVect *add_vect);
	void setVel(double x_new, double y_new);
	void setVel(CoordVect *set_vect);
	void addVel(double x_add, double y_add);
	void addVel(CoordVect *add_vect);
	void update(void);
};

struct Food {
	CoordVect pos;
	double size;
};

#endif
