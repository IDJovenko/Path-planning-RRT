#pragma once
#include "FilledRectangle.h"
#include "../../Obstacle.h"
#include "../../../VectorMath/vector_math.h"

#include <vector>



class RectObstacle : public FilledRectangle, public Obstacle
{
public:
    RectObstacle(HWND hwnd, POINT a = { 0, 0 }, POINT c = { 0, 0 },
        COLORREF color_pen = RGB(255, 255, 255), COLORREF color_brush = RGB(255, 141, 0)):
        FilledRectangle(hwnd, a, c, color_pen, color_brush) {};
	RectObstacle(const Obstacle& rv) : Obstacle(rv), FilledRectangle(nullptr) {};
    RectObstacle(const FilledRectangle& rv) : FilledRectangle(rv) {};
    
    // TODO: below implementation doesn't supports oblique rectangle, need to replace with a trace method or an affine transformation
    /// <returns> return true if pair is inside the rectangle</returns>
    virtual bool inner_point(std::pair<long, long> pair) {
        if (((pair.first > ppt[0].x && pair.first < ppt[2].x) || (pair.first > ppt[2].x && pair.first < ppt[0].x)) &&
            ((pair.second > ppt[0].y && pair.second < ppt[2].y) || (pair.second > ppt[2].y && pair.second < ppt[0].y)))
            return true;
        return false;
    }
    /// <returns>
    /// returns a new configuration Z lying on segment XY and closest to Y,
    /// such that a straight line path from configuration X to Y does not lead to collisions.
    /// </returns>
    virtual std::pair<long, long> Steer(std::pair<long, long> X, std::pair<long, long> Y, double& XZ_length) {
        std::pair<double, double>  intersection = Y;
        XZ_length = length(guide_vector(X, Y));
        for (auto i = 0; i != 4; i++) {
            std::pair<double, double>* new_intersection = new std::pair<double, double>;
            if (!intersection_point(X, Y, std::make_pair(ppt[i].x, ppt[i].y),
                std::make_pair(ppt[(i + 1) % 4].x, ppt[(i + 1) % 4].y), *new_intersection)) {
                continue;
            }
            double new_length = length(guide_vector(*new_intersection, X));
            if (new_length < XZ_length) {
                intersection = *new_intersection;
                XZ_length = new_length;
            }
        }
        return intersection;
    }
};

// Need to satisfy the condition from Set 
bool operator==(const Obstacle& lh, const RectObstacle& rh) {
    return false;
};