#include <math.h>

typedef struct Coordinate2 Coordinate2;
// typedef struct Coordinate3 Coordinate3;

const float PI = 3.1415;


struct Coordinate2 {
	float x;
	float y;
};

/*
struct Coordinate3 {
	float x;
	float y;
	float z;
};
*/

Coordinate2 coord2(float x, float y) {
	Coordinate2 returned;
	returned.x = x;
	returned.y = y;

	return returned;
}

/*
Coordinate3 coord3(float x, float y, float z) {
	Coordinate3 returned;
	returned.x = x;
	returned.y = y;
	returned.z = z;
	return returned;
}
*/

float distance(Coordinate2 coord_one, Coordinate2 coord_two) {
	return sqrt(fabs(coord_one.x - coord_two.x) * fabs(coord_one.x - coord_two.x) + fabs(coord_one.y - coord_two.y) * fabs(coord_one.y - coord_two.y));

}

void move_coord_rad(Coordinate2* coord, float rad, float distance) {
	float y_delta = sin(rad) * distance;
	float x_delta = cos(rad) * distance;

	coord -> x += x_delta;
	coord -> y += y_delta;
}

Coordinate2 find_coord_rad(Coordinate2 coord, float rad, float distance) {
	float y_delta = sin(rad) * distance;
	float x_delta = cos(rad) * distance;
	return coord2(x_delta + coord.x, y_delta + coord.y);
}
float normalize_rad(float rad) {
	return fmod(rad, 2.0 * PI);
}

float add_rad(float rhs, float lhs) {
	return normalize_rad(rhs + lhs);
}

float subtract_rad(float rhs, float lhs) {
	return normalize_rad(rhs + (2 * PI - normalize_rad(lhs)));
}

float find_rad_coord(Coordinate2 coord1, Coordinate2 coord2) {
	/*
	coord1.x *= 100;
	coord1.y *= 100;
	coord2.x *= 100;
	coord2.y *= 100;
	*/
	if (coord1.x == coord2.x) {
		if (coord1.y > coord2.y) {
			return 1.5 * PI;
		}
		else {
			return 0.5 * PI;
		}
	}
	if (coord1.y == coord2.y) {
		if (coord1.x > coord2.x) {
			return PI;
		}
		else {
			return 0.0;
		}
	}

	if (coord2.x < coord1.x) {
		if (coord2.y < coord1.y) {
			return PI + atan(fabs(coord1.y - coord2.y) / fabs(coord1.x - coord2.x));
		}
		else {
			return 0.5 * PI + (subtract_rad(0.5 * PI, atan(fabs(coord1.y - coord2.y) / fabs(coord1.x - coord2.x))));
		}
	}
	else if (coord2.y < coord1.y) {
		return (1.5 * PI + (subtract_rad(0.5 * PI, atan(fabs(coord1.y - coord2.y) / fabs(coord1.x - coord2.x)))));
	}
	
	return atan(fabs(coord1.y - coord2.y) / fabs(coord1.x - coord2.x));
}

void print_coord2(Coordinate2 coord) {

}
