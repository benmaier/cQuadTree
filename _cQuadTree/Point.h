// minimal clone of openFrameworks 2d vector,
// only containing the methods needed in QuadTree.h

#ifndef minimal_clone_Point_h
#define minimal_clone_Point_h

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;

class Point {

    public:

        double x, y;

        Point()
        {
            x = 0.f;
            y = 0.f;
        }


        Point(const double &_x,
              const double &_y
             )
        {
            x = _x;
            y = _y;
        }

        Point& operator+=( const Point& vec );
        Point  operator+( const Point& vec ) const;
        Point  operator-( const Point& vec ) const;

        Point  operator*( const double f ) const;
        Point  operator/( const double f ) const;

        friend ostream& operator<<(ostream& os, const Point& vec);
        friend istream& operator>>(istream& is, const Point& vec);

        double length() const;
        double length2() const;

        string tostr() const;
        bool is_null() const;
};


// define header for non-member operators
Point operator*( double f, const Point& vec );


// implementations

inline Point Point::operator+( const Point& vec ) const {
	return Point( x+vec.x, y+vec.y);
}

inline Point& Point::operator+=( const Point& vec ) {
	x += vec.x;
	y += vec.y;
	return *this;
}

inline Point Point::operator-( const Point& vec ) const {
	return Point(x-vec.x, y-vec.y);
}


inline Point Point::operator*( const double f ) const {
	return Point(x*f, y*f);
}

inline Point Point::operator/( const double f ) const {
	if(f == 0) return Point(x, y);

	return Point(x/f, y/f);
}

inline Point operator*( double f, const Point& vec ) {
    return Point( f*vec.x, f*vec.y);
}

inline ostream& operator<<(ostream& os, const Point& vec) {
	os << vec.x << ", " << vec.y;
	return os;
}

inline istream& operator>>(istream& is, Point& vec) {
	is >> vec.x;
	is.ignore(2);
	is >> vec.y;
	return is;
}


inline double Point::length() const {
	return (double)sqrt( length2() );
}

inline double Point::length2() const {
	return (double) (x*x + y*y);
}

inline bool Point::is_null() const {
	return isnan(x) && isnan(y);
}

inline string Point::tostr() const {
    ostringstream ss;
    ss << "Point(x=" << x << ",y=" << y << ")";
    return ss.str();
}

#endif /* minimal_clone_Point_h */

