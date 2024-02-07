#pragma once

#include <map>
#include <vector>
#include <tuple>
#include <initializer_list>
#include <iterator>
#define point std::pair<long,long>
static const point EmptyParent = { -1,-1 };

struct node {
    friend class TreeIterator;
    friend class Tree;
    node() : parent({ EmptyParent, 0 }), children() {}
    node(std::pair<point, double> parent, std::map<point, double> children) :
        parent(parent), children(children) {}
    std::pair<point, double> parent;
    std::map<point, double> children;
};

class TreeIterator : public std::iterator<
    std::forward_iterator_tag,
    std::map<point, node>::iterator
> {
public:
    TreeIterator() : p() {};
    TreeIterator(const std::map<point, node>::iterator& p) : p(p) {};

    bool operator!=(TreeIterator const& other) const
    {
        return this->p != other.p;
    }
    bool operator==(TreeIterator const& other) const
    {
        return this->p == other.p;
    }

    /// <returns> pair of current vertex and vector of outgoings vertex</returns>
    std::pair<point, std::vector<point>> operator*() const
    {
        std::vector<point> result;
        for (const auto& it : p->second.children) {
            result.push_back(it.first);
        }
        return std::make_pair(p->first, result);
    }
    TreeIterator& operator++() {
        this->p++;
        return *this;
    }
    TreeIterator& operator++(int) {
        TreeIterator temp(this->p);
        this->p++;
        return temp;
    }
private:
    std::map<point, node>::iterator p;
};

/**
 * Oriented positive-weighted graph without cycles.
**/
class Tree {
public:
    Tree() {};
    Tree(std::initializer_list<std::tuple<point, point, double>> edges);

    /// Add vertices (if necessary) and an edge between them to the tree.
    void add_edge(point start_vertex, point end_vertex, double weight = 1.0);

    /// Delete start->end edge and add start->inserted and inserted->end edges to the tree.
    void insert_vertex(
        point start_vertex, point inserted_vertex, point end_vertex,
        double weight_1 = 1.0, double weight_2 = 1.0);

    /// Return all vertices of the tree.
    std::vector<point> get_vertices() const;

    /// Return all outgoing vertices for the specified vertex.
    std::vector<point> get_outgoing_vertices(point src_vertex) const;

    /// Return incoming (parent) vertex for the specified vertex.
    point get_incoming_vertex(point src_vertex) const;

    /// Get adjacent edges for the vertex as vector of (end vertex, edge weight).
    std::vector<std::pair<point, double>> get_outgoing_edges(point src_vertex) const;

    /// Get incoming (parent) edge for the vertex as pair (end vertex, edge weight).
    std::pair<point, double> get_incoming_edge(point src_vertex) const;

    /// Return true if the vertex exists in the graph, false otherwise.
    bool has_vertex(point vertex) const;

    /// Return true if vertices exist and have an edge between them, false otherwise.
    bool has_edge(point start_vertex, point end_vertex) const;

    /// return true if the tree has any edges.
    bool has_edge() const;

    /// Return weight of an edge between vertices (if there is any), throw an exception otherwise.
    double edge_weight(point start_vertex, point end_vertex) const;

    void clear();

    // TODO: Recursive removes the vertex and tree below it.
    //void remove_branch(point vertex);

    TreeIterator begin() {
        return vertices.begin();
    }
    TreeIterator end() {
        return vertices.end();
    }
    TreeIterator find(point p) {
        return TreeIterator(vertices.find(p));
    }

private:
    /// Add single vertex to the tree.
    void add_vertex(point vertex);

    /// Remove the edge from the tree (but not the vertices).
    void remove_edge(point start_vertex, point end_vertex);

    /// Remove the vertex and all its adjacent edges from the tree.
    void remove_vertex(point vertex);

private:
    std::map<point, node> vertices;
};
