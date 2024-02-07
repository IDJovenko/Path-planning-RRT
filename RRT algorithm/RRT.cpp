#pragma once
#include "RRT.h"
#include <stdexcept>

const std::pair<long, long> RRT::NullCnfg = { -1,-1 };

std::vector<config> RRT::run()
{
	for (auto i = 0; i < N; i++) {
		config QRand = RandomSample();
		config Qn = Nearest(QRand);
		config Qs = Steer(Qn, QRand);
		G.add_edge(Qn, Qs);

		if (!got_into_circle(QGoal, end_radius, Qs))
			continue;
		// if straight line between hitting_the_end_cnfg and end config collided with an obstacle
		if (Steer(QGoal, hitting_the_end_cnfg) != hitting_the_end_cnfg && hitting_the_end_cnfg != NullCnfg)
			continue;
		//// gag condition
		//if (length(guide_vector(QGoal, Qs)) < length(guide_vector(QGoal, hitting_the_end_cnfg)))
		hitting_the_end_cnfg = Qs;
	}
	//// if straight line between hitting_the_end_cnfg and end config collided with an obstacle
	//if (Steer(QGoal, hitting_the_end_cnfg) != hitting_the_end_cnfg
	//	&& hitting_the_end_cnfg != std::make_pair<long, long>(-1,-1)) !!!!!!!!!!
	//	throw std::logic_error("Obstacle too near to Qgoal. Replace the Qgoal or decrease the Qgoal radius");
	return get_shortest_path();
}

void RRT::delete_tree()
{
	G.clear();
}

long rand_long() {
	long A = rand();
	A <<= 15;
	A |= rand();
	A |= rand() & 0b11;
	return A;
}

bool RRT::IsFreeConfig(config p) {
	for (auto obs : *obstacles) {
		if (obs->inner_point(p))
			return false;
	}
	return true;
}

config RRT::RandomSample()
{
	config p;
	p.first = rand_long() % ConfigurationSize.first;
	p.second = rand_long() % ConfigurationSize.second;
	/*while (G.has_vertex(p) ) {
		p.first = rand_long() % ConfigurationSize.first;
		p.second = rand_long() % ConfigurationSize.second;
	}*/
	while (!IsFreeConfig(p)) {
		p.first = rand_long() % ConfigurationSize.first;
		p.second = rand_long() % ConfigurationSize.second;
	}
	return p;
}

bool operator==(const config& lh, const std::pair<double, double>& rh) {
	if (lh.first == (long)rh.first && lh.second == (long)rh.second)
		return true;
	return false;
};

config RRT::Nearest(config X)
{
	if (G.get_outgoing_vertices(QInit).empty())
		return QInit;
	using edge = std::pair<config, config>;
	std::map<edge, bool> vizited_edges;
	std::stack<edge> stck;
	edge first_edge = std::make_pair(QInit, G.get_outgoing_vertices(QInit)[0]);
	stck.push(first_edge);
	vizited_edges.insert(make_pair(first_edge, true));
	vizited_edges.insert(make_pair(make_pair(first_edge.second, first_edge.first), true));
	char status_c{};
	point result = closest(first_edge.first, first_edge.second, X, status_c);
	/// the edge on which the nearest point lies
	edge AB = first_edge;
	char tmp_status_c;
	point tmp;
	while (!stck.empty()) {
		edge e = stck.top();
		stck.pop();
#if CONSOLE_LOG
		std::cout << "A = { "
			<< e.first.first << ';'
			<< e.first.second
			<< " }, B = { "
			<< e.second.first << ';'
			<< e.second.second
			<< " }"
			<< std::endl;
#endif

		tmp = closest(e.first, e.second, X, tmp_status_c);
		// if length of found in this iteration vector is less then length previously found vector (result)
		// then reset result vector and remember the edge
		if (dot(guide_vector(X, tmp), guide_vector(X, tmp)) < dot(guide_vector(X, result), guide_vector(X, result))) {
			result = tmp;
			status_c = tmp_status_c;
			AB = e;
		}

		/// <summary>
		/// Iterates over adjacent by edges edges incident to passed vertex.
		/// </summary>
		/// <param name="vertex">: one of two vertices adjacent to an edge</param>
		/// <param name="adj_vert">: vector of adjacent by passed vertex vertex</param>
		/// <returns></returns>
		auto adj_edge_traversal = [&stck, &vizited_edges](config vertex, const std::vector<config>& adj_vert) -> void
		{
			for (auto& it : adj_vert) {
				edge adj_e = make_pair(vertex, it);
				if (vizited_edges.contains(adj_e))
					continue;
				stck.push(adj_e);
				vizited_edges.insert(make_pair(adj_e, true));
				vizited_edges.insert(make_pair(make_pair(adj_e.second, adj_e.first), true));
			}
		};

		adj_edge_traversal(e.first, G.get_outgoing_vertices(e.first));
		adj_edge_traversal(e.second, G.get_outgoing_vertices(e.second));
	}
	if (status_c != LIES_INSIDE) {
		return result;
	}
	// adding finding config to graph

	// this condition is similar to the condition of existing result in tree
	//if (G.get_incoming_vertex(result) != EmptyParent)
	//	return result;
	if (AB.first == result || result == AB.second)
		return result;
	if (G.find(result) != G.end())
		return result;
	if (!IsFreeConfig(result))
		return result;
	G.insert_vertex(AB.first, result, AB.second);

	if (!got_into_circle(QGoal, end_radius, result))
		return result;
	// if straight line between hitting_the_end_cnfg and end config collided with an obstacle
	if (Steer(QGoal, hitting_the_end_cnfg) != hitting_the_end_cnfg)
		return result;
	// gag condition
	if (length(guide_vector(QGoal, result)) < length(guide_vector(QGoal, hitting_the_end_cnfg)))
		hitting_the_end_cnfg = result;
	return result;
}

void make_indent(std::pair<long, long> X, std::pair<double, double>* Z, double XZ_length) {
	// x-axis ident
	if (dot(guide_vector(X, std::make_pair(Z->first + 1, Z->second))) < XZ_length)
		Z->first++;
	else 
	if (dot(guide_vector(X, std::make_pair(Z->first - 1, Z->second))) < XZ_length)
		Z->first--;
	// y-axis ident
	if (dot(guide_vector(X, std::make_pair(Z->first, Z->second + 1))) < XZ_length)
		Z->second++;
	else
	if (dot(guide_vector(X, std::make_pair(Z->first, Z->second - 1))) < XZ_length)
		Z->second--;
}
config RRT::Steer(config X, config Y)
{
	point result{ Y };
	double result_length = length(guide_vector(X, Y));
	if (obstacles->is_empty())
		return result;
	for (auto obs : *obstacles) {
		if (obs->inner_point(std::make_pair(287, 235))) {
			int k = 0;
			k++;
		}
		point Z;
		double XZ_length{};
		Z = obs->Steer(X, Y, XZ_length);
		make_indent(X, &Z, XZ_length);
		if (XZ_length < result_length) {
			result_length = std::move(XZ_length);
			result = std::move(Z);
		}
	}
	if (!IsFreeConfig(result))
		return X;
	return result;
}

std::vector<config> RRT::get_shortest_path()
{
	if (hitting_the_end_cnfg == NullCnfg)
		return {};
	std::vector<config> path{};
	// if there isn't exact hit
	if (hitting_the_end_cnfg != QGoal)
		path.push_back(QGoal);
	config cur_conf = hitting_the_end_cnfg;
	path.push_back(cur_conf);
	while (cur_conf != QInit)
	{
		cur_conf = G.get_incoming_edge(cur_conf).first;
		path.push_back(cur_conf);
	}
	return path;
}

std::vector<config> RRT::get_path_to_start(config end)
{
	std::vector<config> path{};
	path.push_back(this->QGoal);
	config cur_conf = end;
	path.push_back(cur_conf);
	while (cur_conf != QInit)
	{
		cur_conf = G.get_incoming_edge(cur_conf).first;
		path.push_back(cur_conf);
	}
	return path;
}
