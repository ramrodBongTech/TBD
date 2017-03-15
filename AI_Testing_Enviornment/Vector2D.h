#pragma once
#include <cfloat>
#include <cmath>
#include <climits>
#include "box2d\Box2D.h"


/*The Vector2d class is an object consisting of simply an x and
y value. Certain operators are overloaded to make it for
easier vector math to be performed.*/

class Vector2D 
{
public:
	/*The x and y values are public to give easier access for
	outside funtions. Accessors and mutators are not really
	necessary*/
	float x;
	float &width = x;
	float &w = x;
	float y;
	float &height = y;
	float &h = y;
	~Vector2D(void);
	//Constructor assigns the inputs to x and y.
	Vector2D();
	//creates a unit vector based on a radian angle
	Vector2D(float angle);
	Vector2D(b2Vec2 pos);
	Vector2D(float, float);
	Vector2D(const Vector2D& v);
	void limit(float maxMagnitude);
	b2Vec2 toBox2DVector();

	/*The following operators simply return Vector2ds that
	have operations performed on the relative (x, y) values*/
	Vector2D operator+(const Vector2D&) const;
	Vector2D operator-(const Vector2D&) const;
	Vector2D operator*(const Vector2D&) const;
	Vector2D operator/(const Vector2D&) const;
	Vector2D operator-(const b2Vec2&);
	Vector2D operator+=(const Vector2D&);
	Vector2D operator-=(const Vector2D&);
	Vector2D operator=(const Vector2D & v);
	// nromalise its self
	Vector2D Normalize();
	//normalise a value
	static Vector2D Normalize(Vector2D t);
	//Check if the Vectors have the same values.
	bool operator==(const Vector2D&) const;

	/*Check which Vectors are closer or further from the
	origin.*/
	bool operator>(const Vector2D&) const;
	bool operator<(const Vector2D&) const;
	bool operator>=(const Vector2D&) const;
	bool operator<=(const Vector2D&) const;

	//Negate both the x and y values.
	Vector2D operator-() const;

	//Apply scalar operations.
	Vector2D operator*(const float&) const;
	Vector2D operator/(const float&) const;

	static Vector2D DirectionVectorBetweenTwoPoints(const Vector2D&, const Vector2D&);
	//Product functions
	static float DotProduct(const Vector2D&, const Vector2D&);
	float DotProduct(const Vector2D&);
	static float CrossProduct(const Vector2D&, const Vector2D&);

	//Returns the length of the vector from the origin.
	static float Magnitude(const Vector2D&);
	//gets the length/mag of its self
	float Magnitude();
	//Return the unit vector of the input
	static Vector2D Normal(const Vector2D&);

	//Return a vector perpendicular to the left.
	static Vector2D Perpendicular(const Vector2D&);

	//Return true if two line segments intersect.
	static bool Intersect(const Vector2D&, const Vector2D&, const Vector2D&, const Vector2D&);

	//Return the point where two lines intersect.
	static Vector2D GetIntersect(const Vector2D&, const Vector2D&, const Vector2D&, const Vector2D&);
	//get the distance between 2 points retuns as float
	static float Distance(Vector2D & one, Vector2D &two);

	static float DistanceSq(Vector2D & one, Vector2D & two);

	//get the angle between 2 points returns as float in degrees
	static float AngleDeg(Vector2D & one, Vector2D & two);
	//get the angle between 2 points returns as float in radians
	static float AngleRad(Vector2D & one, Vector2D & two);

	static const Vector2D UP;
	static const Vector2D LEFT; 
	static const Vector2D DOWN; 
	static const Vector2D RIGHT;
	static const Vector2D ZERO;
};
