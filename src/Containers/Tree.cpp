#pragma once
#include "../../include/Containers/Tree.h"

#include <stdexcept>
#include <exception>

using namespace std;

Tree::Tree(initializer_list<tuple<point, point, double>> edges) {
    for (const auto& e : edges) {
        add_edge(get<0>(e), get<1>(e), get<2>(e));
    }
}

void Tree::add_vertex(point vertex) {
    if (!has_vertex(vertex)) {
        vertices[vertex] = node();
    }
}

void Tree::add_edge(point start_vertex, point end_vertex, double weight) {
    if (start_vertex == end_vertex) return;
    add_vertex(start_vertex);
    add_vertex(end_vertex);
    // if end_vertex have parent vertex
    if (vertices[end_vertex].parent.first != EmptyParent) {
        throw logic_error("adding a child vertex that already has a parent vertex");
    }
    vertices[end_vertex].parent.first = start_vertex;
    vertices[end_vertex].parent.second = weight;
    vertices[start_vertex].children[end_vertex] = weight;
}

void Tree::insert_vertex(
    point start_vertex, point inserted_vertex, point end_vertex,
    double weight_1, double weight_2)
{
    remove_edge(start_vertex, end_vertex);
    add_edge(start_vertex, inserted_vertex, weight_1);
    add_edge(inserted_vertex, end_vertex, weight_2);
}

std::vector<point> Tree::get_vertices() const {
    std::vector<point> result;
    for (const auto& p : vertices) {
        result.push_back(p.first);
    }
    return result;
}

std::vector<point> Tree::get_outgoing_vertices(point src_vertex) const {
    std::map<std::pair<long, long>, node>::const_iterator it = vertices.find(src_vertex);
    if (it == vertices.end()) {
        return std::vector<point> {};
    }
    vector<point> result;
    for (const auto& p : it->second.children) {
        result.push_back(p.first);
    }
    return result;
}

point Tree::get_incoming_vertex(point src_vertex) const
{
    const auto it = vertices.find(src_vertex);
    if (it == vertices.end()) {
        return point{};
    }
    return it->second.parent.first;
}

vector<pair<point, double>> Tree::get_outgoing_edges(point src_vertex) const {
    const auto it = vertices.find(src_vertex);
    if (it == vertices.end()) {
        return vector<pair<point, double>> {};
    }
    vector<pair<point, double>> result;
    for (const auto& p : it->second.children) {
        result.push_back(make_pair(p.first, p.second));
    }
    return result;
}

std::pair<point, double> Tree::get_incoming_edge(point src_vertex) const
{
    const auto it = vertices.find(src_vertex);
    if (it == vertices.end()) {
        return pair<point, double>{};
    }
    const auto& p = it->second.parent;
    if (p == make_pair(EmptyParent, 0.0))
        throw logic_error("referencing an non-existent parent vertex");
    return make_pair(p.first, p.second);
}

bool Tree::has_vertex(point vertex) const {
    return (vertices.find(vertex) != vertices.end());
}

bool Tree::has_edge(point start_vertex, point end_vertex) const {
    const auto it = vertices.find(start_vertex);
    if (it == vertices.end()) {
        return false;
    }
    return (it->second.children.find(end_vertex) != it->second.children.end());
}

// TODO: there is no neccessary to call get_adjacent_edges
bool Tree::has_edge() const
{
    for (auto it = vertices.begin()++; it != vertices.end(); it++) {
        std::vector<std::pair<point, double>> adj_verteces = get_outgoing_edges((*it).first);
        if (!adj_verteces.empty()) {
            return true;
        }
    }
    return false;
}

double Tree::edge_weight(point start_vertex, point end_vertex) const {
    const auto it_s = vertices.find(start_vertex);
    if (it_s == vertices.end()) {
        throw invalid_argument("Edge doesn't exist");
    }
    const auto it_e = it_s->second.children.find(end_vertex);
    if (it_e == it_s->second.children.end()) {
        throw invalid_argument("Edge doesn't exist");
    }
    return it_e->second;
}

void Tree::clear()
{
    this->vertices.clear();
}

void Tree::remove_vertex(point vertex) {
    /// Remove outgoing edges.
    auto adjacent_vertices = get_outgoing_vertices(vertex);
    for (const auto& adj_v : adjacent_vertices) {
        remove_edge(vertex, adj_v);
    }
    /// Remove incoming (coming from parent) edges.
    remove_edge(get_incoming_vertex(vertex), vertex);
    /// Remove the vertex itself.
    vertices.erase(vertex);
}

void Tree::remove_edge(point start_vertex, point end_vertex) {
    auto it_s = vertices.find(start_vertex);
    // if start_vertex doesn't exist
    if (it_s == vertices.end())
        return;
    // if start_vertex->end_vertex doesn't exist
    if (it_s->second.children.find(end_vertex) == it_s->second.children.end())
        return;
    it_s->second.children.erase(end_vertex);

    it_s = vertices.find(end_vertex);
    it_s->second.parent = make_pair(EmptyParent, 0.0);
}
