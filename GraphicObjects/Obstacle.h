#pragma once

#include <utility>

class Obstacle {
public:
	/// <param name="A">a point</param>
	/// <returns>true, if A point is inner point</returns>
	virtual bool inner_point(std::pair<long, long> A) = 0;

	/// <param name="X">configuration (point)</param>
	/// <param name="Y">configuration (point)</param>
	/// <returns> returns a new configuration Z,
	/// lying on the segment XY and closest to Y,
	/// such that the straight path from configuration X to Z does not lead to collisions
	/// with this obstacle</returns>
	virtual std::pair<long, long> Steer(
		std::pair<long, long> X,
		std::pair<long, long> Y,
		double& XZ_length
	) = 0;
};