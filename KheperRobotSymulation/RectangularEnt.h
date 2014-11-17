#ifndef RECTANGULAR_ENT_H
#define RECTANGULAR_ENT_H

#include "SymEnt.h"
#include "CircularEnt.h"
#include "Symulation.h"
#include "Point.h"
#include "MathLib.h"

class RectangularEnt : public SymEnt
{
	public:
		// x, y -> coord of left-top corner
		RectangularEnt(uint16_t id, uint32_t weight, bool movable, double x,
			double y, double width, double height, double angle = 0);

		// points should be in clockwise order
		/*RectangularEnt(uint16_t id, uint32_t weight, bool movable, double x1,
			double y1, double x2, double y2, double x3, double y3,
			double x4, double y4);*/

		virtual ~RectangularEnt() { delete _bottLeft; delete _center; }

		double CollisionLength(SymEnt& other, Point& proj);
		Point& GetBottLeft() { return *_bottLeft; }
		Point& GetCenter() { return *_center; }

		virtual void Translate(int x, int y);

		virtual void Serialize(Buffer& buffer);

	protected:
		double check_and_divide(CircularEnt& other, Point& bottLeft, double width, double height, int level);

		Point* _bottLeft;
		Point* _center;
		double _width;
		double _height;
		double _angle; // in degrees
};

#endif
