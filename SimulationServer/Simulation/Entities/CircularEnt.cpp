#include "CircularEnt.h"
#include "RectangularEnt.h"
#include "LinearEnt.h"
#include "../Math/MathLib.h"
#include <iostream>

CircularEnt::CircularEnt(uint16_t id, uint32_t weight, bool movable, double x, double y,
	double radius) : SimEnt(id, SimEnt::CIRCLE, weight, movable), _radius(radius)
{
	_center = new Point(x, y);
}

CircularEnt::CircularEnt(std::ifstream& file, bool readBinary) : SimEnt(file, readBinary, SimEnt::CIRCLE)
{
	double x, y;
    if (readBinary)
    {
        file.read(reinterpret_cast<char*>(&x), sizeof(x));
        file.read(reinterpret_cast<char*>(&y), sizeof(y));
        file.read(reinterpret_cast<char*>(&_radius), sizeof(_radius));
    }
    else
        file >> x >> y >> _radius;
    _center = new Point(x, y);
}

CircularEnt::CircularEnt(const CircularEnt& other) : SimEnt(other)
{
    _center = new Point(*other._center);
    _radius = other._radius;
}

double CircularEnt::collisionLength(SimEnt& other, Point& proj)
{
	int other_shape = other.getShapeID();

	if (other_shape == LINE)
	{
		/*  If the distance between circle center and orthogonal projection is shorter than the radius and if this projection belongs to
			the line segment (0 <= u <= 1), there is a collision.

			There is also a possibility that projection doesn't belong to the line segment and there is a collision.
			To check it we just have to calculate distance between both ends of the line segment and the center of
			CircularEnt. If any of these distances is shorter than the radius, there is a collision.
		*/

		LinearEnt &conv = *dynamic_cast<LinearEnt*>(&other);
		bool belongs;
		Point orth_proj = orthogonalProjection(*_center, conv.getBeg(), conv.getEnd(), &belongs);
		double ovr_dist = orth_proj.getDistance(*_center);
        /*if (_shapeID == SimEnt::KHEPERA_ROBOT && conv.getID() == 1004)
        {
            std::cout << "dist to orth_proj: " << ovr_dist << std::endl;
            double A = conv.getEnd().getY() - conv.getBeg().getY(), B = conv.getBeg().getX() - conv.getEnd().getX(),
                C = conv.getEnd().getX() * conv.getBeg().getY() - conv.getBeg().getX() * conv.getEnd().getY();
            double top = abs(A * _center->getX() + B * _center->getY() + C);
            double bott = sqrt(A * A + B * B);
            double res = top / bott;
            std::cout << "dist - 2. approach: " << res << std::endl;
        }*/

		if (ovr_dist <= _radius)
		{
			if (belongs)
			{
				proj.setCoords(orth_proj);
                return _radius - ovr_dist;
			}

			double dist_to_beg = conv.getBeg().getDistance(*_center), dist_to_end = conv.getEnd().getDistance(*_center);
			double dist_to_vertex = min(dist_to_beg, dist_to_end);
			if (dist_to_vertex <= _radius)
			{
				proj.setCoords(dist_to_beg == dist_to_vertex ? conv.getBeg() : conv.getEnd());
                return _radius - dist_to_vertex;
			}
		}
	}

	else if (other_shape == RECTANGLE)
		return other.collisionLength(*this, proj);

	else if (other_shape == CIRCLE || other_shape == KHEPERA_ROBOT)
	{
		CircularEnt &converted = *dynamic_cast<CircularEnt*>(&other);
		double radiuses_sum = _radius + converted.getRadius();
		double centres_diff = _center->getDistance(converted.getCenter());

        return radiuses_sum - centres_diff;
	}

	return NO_COLLISION;
}

void CircularEnt::translate(double x, double y)
{
	_center->translate(x, y);
}

/*
		 Serialization format (integers in network-byte-order, doubles and floats in host-byte-order)

             +-------------------+--------------------------------------+-------------------+
             |                   |                                      |                   |
             |   SHAPE_ID        |              ENTITY_ID               |    MOVABLE        |
			 |    8 bytes        |               16 bytes               |    8 bytes        |
			 +-------------------+--------------------------------------+-------------------+
			 |                                                                              |
			 |                                WEIGHT                                        |
			 |                                32 bytes                                      |
			 +------------------------------------------------------------------------------+
		   	 |                                                                              |
			 |                                                                              |
			 |                                X COORD                                       |
			 |                                64 bytes                                      |
			 |                                                                              |
			 |                                                                              |
			 +------------------------------------------------------------------------------+
			 |                                                                              |
			 |                                                                              |
			 |                                Y COORD                                       |
			 |                                64 bytes                                      |
			 |                                                                              |
			 |                                                                              |
			 +------------------------------------------------------------------------------+
			 |                                                                              |
			 |                                                                              |
			 |                                 RADIUS                                       | 
			 |                                64 bytes                                      |
			 |                                                                              |
			 |                                                                              |
			 +------------------------------------------------------------------------------+

			 DATA_LENGTH = 256 bytes

*/

void CircularEnt::serialize(Buffer& buffer)
{
	SimEnt::serialize(buffer);

	buffer.pack(getX());
	buffer.pack(getY());
	buffer.pack(_radius);
}

void CircularEnt::serialize(std::ofstream& file)
{
	double x = getX();
	double y = getY();

	SimEnt::serialize(file);
	file.write(reinterpret_cast<const char*>(&x), sizeof(x));
	file.write(reinterpret_cast<const char*>(&y), sizeof(y));
	file.write(reinterpret_cast<const char*>(&_radius), sizeof(_radius));
}