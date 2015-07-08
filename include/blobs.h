#ifndef BLOBS_H
#define BLOBS_H

#include "visuals.h"

class CoordVect
{
 public:
	double x;
	double y;

	CoordVect(double x_new = 0, double y_new = 0);
	void set(double x_new, double y_new);
	void set(CoordVect &set_vect);
	void set_x(double x_new);
	void set_x(CoordVect &set_vect);
	void set_y(double y_new);
	void set_y(CoordVect &set_vect);
	void add(double x_add, double y_add);
	void add(CoordVect &add_vect);
	void sub(double x_sub, double y_sub);
	void sub(CoordVect &sub_vect);
	void mul(double x_mul, double y_mul);
	void mul(double xy_mul);
	void mul(CoordVect &mul_vect);
	void div(double x_div, double y_div);
	void div(double xy_div);
	void div(CoordVect &div_vect);
	double rads(void);
};

class Brain
{
 public:
	static constexpr int inNum_reg = 19;
	static constexpr int outNum_reg = 2;
	static constexpr int memNum = 6;
	static const unsigned int memOffset[memNum];
	static constexpr int inNum = inNum_reg + memNum;
	static constexpr int outNum = outNum_reg + memNum;

	double weights[inNum][outNum];
	double outs[outNum];
	double *mem[memNum];

	Brain(double weights_init[inNum][outNum] = nullptr);
	~Brain(void);
	void feedforward(double ins[inNum_reg]);
};

class Blob
{
public:
	/* Standard size of a blob */
	static constexpr double stdSize = 16.0;
	/* How much larger one blob than another to consume it.
	   Bigger blob must be *at least* this percent larger.
	 */
	static constexpr double howLrg = 0.10;
	/* How much one blob must cover another to consume it.
	   Bigger blob must cover *at least* this percent of a smaller blob.
	 */
	static constexpr double howCover = 0.50;
	/* How far a blob can see (as a multiple of its size). A blob's FOV is
	   a square whose sides are size*(1+2*seeMult) long each.
	 */
	static constexpr double seeMult = 8.0;
	/* How fast a blob can accelerate in any direction */
	static constexpr double accel = 5.0;
	/* What percentage of a blob's size is lost each round.
	   This decay is applied to a blob's size, not its mass.
	   Thus, the amount of mass a blob loses each iteration
	   will increase exponentially as it grows.
	 */
	static constexpr double decayRate = 0.001;
	/* The minimum size of a blob before it dies off. */
	static constexpr double minSize = 5;

	static constexpr double fit_pow_size = 4.0;

	CoordVect pos;
	CoordVect vel;
	double size;
	double maxVel;
	Brain brain;
	Uint32 color;
	/* The greatest size a blob has reached */
	double peakSize;
	/* Number of ticks the blob has been alive */
	int timeAlive;
	double fitness;

	Blob(double size_new = stdSize, double x_new = 0.0, double y_new = 0.0,
	     double weights[Brain::inNum][Brain::outNum] = nullptr);
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
	void boundsCorrect(void);
	void perceive(void);
	void act(void);
	void calcFitness(void);
	void update(void);
};

class Food {
 public:
	static constexpr double size = 5;
	CoordVect pos;

	Food(double x_new = 0.0, double y_new = 0.0);
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

double sigmoid(double x);

#endif
