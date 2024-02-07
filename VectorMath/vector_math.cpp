#include "vector_math.h"

#ifndef EPS
#define EPS 1.0e-06
#endif // EPS

point guide_vector(std::pair<double, double> A, std::pair<double, double> B)
{
    return point(B.first - A.first, B.second - A.second);
}

point mult_by_scalar(std::pair<double, double> v, double scalar)
{
    return point(v.first * scalar, v.second * scalar);
}

double dot(point v1, point v2)
{
    return v1.first * v2.first + v1.second * v2.second;
}
// returns dot product vector v with itself
double dot(point v)
{
    return v.first * v.first + v.second * v.second;
}

double cross(point v1, point v2)
{
    return v1.first * v2.second + v1.second * v2.first;
}

double length(point v)
{
    return sqrt(dot(v, v));
}

point closest(point A, point B, point P)
{
    double d = dot(guide_vector(A, P), guide_vector(A, B)) / dot(guide_vector(A, B), guide_vector(A, B));
    d = std::clamp(d, 0.0, 1.0);
    return A + mult_by_scalar((B - A), d);
}

// TODO: idk if a matching between d = 0.0 and 'A' is strict.
point closest(point A, point B, point P, char& c)
{
    double d = dot(guide_vector(A, P), guide_vector(A, B)) / dot(guide_vector(A, B), guide_vector(A, B));
    d = std::clamp(d, 0.0, 1.0);
    if (d < EPS && d > -EPS) {
        c = 'A';
        return A;
    }
    if (d < 1 + EPS && d > 1 - EPS) {
        c = 'B';
        return B;
    }
    c = 0;
    return A + mult_by_scalar((B - A), d);
}

bool intersection_point(point s1_1, point s1_2, point s2_1, point s2_2, point& CrossP)
{
    point v1{ s1_2.first - s1_1.first, s1_2.second - s1_1.second };
    point v2{ s2_2.first - s2_1.first, s2_2.second - s2_1.second };
    // parallelism check
    if (cross(v1, v2) < EPS && cross(v1, v2) > -EPS)
        return false;

    point w{ s2_1.first - s1_1.first, s2_1.second - s1_1.second };

    double t2 = (v1.first * w.second - w.first * v1.second) / (v2.first * v1.second - v1.first * v2.second);
    double t1 = (w.first + v2.first * t2) / (v1.first);

    // segments intersection check
    if (t2 < 0 || t2 > 1 || t1 < 0 || t1 > 1) {
        return false;
    }

    CrossP.first = s2_1.first + v2.first * t2;
    CrossP.second = s2_1.second + v2.second * t2;

    return true;
}

bool got_into_circle(point center, double radius, point p)
{
    if (length(guide_vector(center, p)) <= radius)
        return true;
    else
        return false;
}

point operator+(const point& l, const point& r)
{
    return point(l.first + r.first, l.second + r.second);
}

point operator-(const point& l, const point& r)
{
    return point(l.first - r.first, l.second - r.second);
}
