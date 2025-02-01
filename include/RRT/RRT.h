#pragma once
#include "../Common/build_config.h"

#include "../Containers/Tree.h"
#include "../Containers/Set.h"
#include "../GraphicObjects/Obstacle.h"
#include "../VectorMath/vector_math.h"
#include <vector>
#include <stack>
#include <map>
#include <iterator>
//#include <utility>

using config = std::pair<long, long>;
using obstacle = std::tuple<config, config, config, config>;

class RRT
{
private:
	Tree G;
	config QInit{}, QGoal{};
	/// <summary> range indicated hitting the end</summary>
	double end_radius{};
	static const std::pair<long, long> NullCnfg;
	config hitting_the_end_cnfg{ NullCnfg };
	std::vector<config> path;
	/// <summary> configuration space size </summary>
	config ConfigurationSize;
	/// <summary> Set of obstacles </summary>
	Set<Obstacle>* obstacles{ new Set<Obstacle> };
	/// <summary> number of samples </summary>
	unsigned N{};

	/// <param name="p"> any config (point)</param>
	/// <returns> is config p from free configuration space</returns>
	bool IsFreeConfig(config p);
	/// <returns> returns a new random configuration from the space of valid configurations </returns>
	config RandomSample();
	/*/// <summary>
	/// checks that a straight line path from configuration X to Y does not lead to collisions,
	/// i.e. segment XY does not intersect with obstacles from the space of invalid configurations
	/// </summary>
	/// <param name="X">configuration (point)</param>
	/// <param name="Y">configuration (point)</param>
	/// <returns> if a straight line path from X to Y does not lead to collisions</returns>
	bool CollitionFree(config X, config Y);*/

#if RELEASE
	/// <summary>
	/// returns the vertex (configuration) closest to the configuration X.
	/// If it turns out that X is close not to one vertex,
	/// but to an edge (i.e., that the configuration closest to X lies on the segment between any two configurations),
	/// then at this point, the segment is divided into two parts and a new vertex is added, which is then returned.
	/// </summary>
	/// <param name="X">configuration (point)</param>
	/// <returns>returns the vertex (configuration) closest to the X</returns>
	config Nearest(config X);
	/// <param name="X">configuration (point)</param>
	/// <param name="Y">configuration (point)</param>
	/// <returns> returns a new configuration Z from the space of acceptable configurations,
	/// lying on the segment XY and closest to Y,
	/// such that the straight path from configuration X to Y does not lead to collisions</returns>
	config Steer(config X, config Y);

	std::vector<config> get_shortest_path();
#endif

public:
	RRT() {};
	/// <param name="N"> number of samples </param>
	/// <param name="Qinit"> initial configuration/point </param>
	/// <param name="Qgoal"> goal configuration/point </param>
	/// <param name="ConfigurationSize"> configuration space size </param>
	RRT(unsigned N, config Qinit, config Qgoal, double Qgoal_radius, config ConfigurationSize) :
		N(N), QInit(Qinit), QGoal(Qgoal), end_radius(Qgoal_radius), ConfigurationSize(ConfigurationSize)
	{};

	std::vector<config> run();

	void delete_tree();

	void set_seed(unsigned seed)
	{
		srand(seed);
	}

	TreeIterator begin() {
		return G.begin();
	}
	TreeIterator end() {
		return G.end();
	}

#if DEBAG
	void set_tree(Tree& G) {
		this->G = G;
	}
	void set_obstacles(Set<Obstacle>& obs) {
		obstacles = &obs;
	}
	/// <summary>
	/// returns the vertex (configuration) closest to the configuration X.
	/// If it turns out that X is close not to one vertex,
	/// but to an edge (i.e., that the configuration closest to X lies on the segment between any two configurations),
	/// then at this point, the segment is divided into two parts and a new vertex is added, which is then returned.
	/// </summary>
	/// <param name="X">configuration (point)</param>
	/// <returns>returns the vertex (configuration) closest to the X</returns>
	config Nearest(config X);
	/// <param name="X">configuration (point) doesn't belong to any obstacle</param>
	/// <param name="Y">configuration (point)</param>
	/// <returns> returns a new configuration Z from the space of acceptable configurations,
	/// lying on the segment XY and closest to Y,
	/// such that the straight path from configuration X to Y does not lead to collisions</returns>
	config Steer(config X, config Y);

	std::vector<config> get_shortest_path();
	std::vector<config> get_path_to_start(config end);
#endif
};
