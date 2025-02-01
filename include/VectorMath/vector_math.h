#pragma once
#include <vector>
#include <algorithm>

#define point std::pair<double, double>

#define LIES_INSIDE 0
#define LIES_ON_A (char)'A'
#define LIES_ON_B (char)'B'

/// <param name="A"> starting point</param>
/// <param name="B"> end point</param>
/// <returns> guide vector coordinates</returns>
point guide_vector(std::pair<double, double> A, std::pair<double, double> B);

point mult_by_scalar(std::pair<double, double> v, double scalar);

/// <summary>scalar product</summary>
double dot(point v1, point v2);
/// <summary>
/// square scalar product for vector itself
/// </summary>
double dot(point v);

double cross(point v1, point v2);

double length(point v);

/// <returns>point on segment AB closest to point P</returns>
point closest(point A, point B, point P);

/// <param name="c"> 'A'(=65) or 'B'(=66) will be written to the param "c",
/// if they are closest to point P, otherwise 0 (NOP) will be written</param>
/// <returns>point on segment AB closest to point P</returns>
point closest(point A, point B, point P, char& c);

// TODO: remake on the same line segments lying handling
/// <summary>
/// point of intersection of two segments
/// </summary>
/// <param name="s1_1"> beginning of ferst segment</param>
/// <param name="s1_2"> end of ferst segment</param>
/// <param name="s2_1"> beginning of second segment</param>
/// <param name="s2_2"> end of second segment</param>
/// <param name="out_cross_point"> pointer to point of intersection of two segments </param>
/// <returns> do two segments intersect (if the segments lie on the same line - returns false)</returns>
bool intersection_point(point s1_1, point s1_2, point s2_1, point s2_2, point& out_cross_point);

/// <summary>
/// check if some point p is inside a circle.
/// </summary>
/// <param name="center"> center of cercle</param>
/// <param name="radius"> radius of cercle</param>
/// <param name="p"> being verified point</param>
/// <returns> true, if p is inside of circle</returns>
bool got_into_circle(point center, double radius, point p);

point operator+(const point& l, const point& r);
point operator-(const point& l, const point& r);
