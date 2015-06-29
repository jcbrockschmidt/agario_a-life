#ifndef BLOBS_H
#define BLOBS_H

class CoordVect
{
 public:
	double x;
	double y;

	CoordVect(double x_new = 0, double y_new = 0);
	void set(double x_new, double y_new);
	void set(CoordVect &set_vect);
	void set_x(double x_new);
	void set_y(double y_new);
	void add(double x_add, double y_add);
	void add(CoordVect &add_vect);
	void sub(double x_sub, double y_sub);
	void sub(CoordVect &sub_vect);
	double rads(void);
};

class Blob
{
public:
	/* Standard size of a blob */
	static constexpr double stdSize = 1.0;
	/* How much larger one blob than another to consume it.
	   Bigger blob must be *at least* this percent larger.
	 */
	static constexpr double howLrg = 0.10;
	/* How much one blob must cover another to consume it.
	   Bigger blob must cover *at least* this percent of a smaller blob.
	 */
	static constexpr double howCover = 0.50;

	CoordVect pos;
	CoordVect vel;
	double size;

	Blob(double size_new = stdSize, double x_new = 0.0, double y_new = 0.0);
	void setSize(double size_set);
	void addSize(double size_add);
	void setPos(double x_new, double y_new);
	void setPos(CoordVect &set_vect);
	void addPos(double x_add, double y_add);
	void addPos(CoordVect &add_vect);
	void setVel(double x_new, double y_new);
	void setVel(CoordVect &set_vect);
	void addVel(double x_add, double y_add);
	void addVel(CoordVect &add_vect);
	void update(void);
};

struct Food {
	CoordVect pos;
	double size;
};

/**
Checks for collision between two rectangles.

@param x1 x position of first rectangle.
@param y1 y position of first rectangle.
@param w1 width of first rectangle.
@param h1 height of first rectangle.
@param x2 x position of second rectangle.
@param y2 y position of second rectangle.
@param w2 width of second rectangle.
@param h2 height of second rectangle.
@return true if rectangles collide, false if they do not.
 */
bool testAABBAABB(double x1, double y1, double w1, double h1,
	  double x2, double y2, double w2, double h2);
bool testAABBAABB(CoordVect &vec1, double side1, CoordVect &vec2, double side2);

#endif
