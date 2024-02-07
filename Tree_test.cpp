#pragma once
#include "../catch.hpp"

#include "Tree.h"

#include <algorithm>
#include <set>
#include <sstream>

using namespace std;

template <typename Container>
set<typename Container::value_type> to_set(const Container& c) {
    return set<typename Container::value_type>(c.begin(), c.end());
}

template <typename Container>
Container sorted(const Container& c) {
    Container copy = c;
    sort(begin(copy), end(copy));
    return copy;
}

namespace Catch {
    template<>
    struct StringMaker<pair<long, double>> {
        static string convert(const pair<point, double>& p) {
            ostringstream out;
            out << "{ {" << p.first.first << ", " << p.first.second << "}" << ", " << p.second << "}";
            return out.str();
        }
    };
}

TEST_CASE("[Tree] Empty Tree", "[Tree]") {
    Tree g;
    CHECK_FALSE(g.has_vertex({ 0,0 }));
    CHECK_FALSE(g.has_edge({ 0,0 }, { 1,1 }));
}

TEST_CASE("[Tree] One edge", "[Tree]") {
    Tree g;
    g.add_edge({ 0,0 }, { 1,1 }, 5.0);
    CHECK(g.has_vertex({ 0,0 }));
    CHECK(g.has_vertex({ 1,1 }));
    CHECK(g.has_edge({ 0,0 }, { 1,1 }));
    CHECK_FALSE(g.has_edge({ 1,1 }, { 0,0 }));
    CHECK(g.edge_weight({ 0,0 }, { 1,1 }) == 5.0);
    CHECK_FALSE(g.has_edge({ 0,0 }, { 0,0 }));
}

TEST_CASE("[Tree] Two edges", "[Tree]") {
    Tree g;
    g.add_edge({ 0,0 }, { 1,1 });
    g.add_edge({ 0,0 }, { 3,3 });
    CHECK(g.has_vertex({ 0,0 }));
    CHECK(g.has_vertex({ 1,1 }));
    CHECK(g.has_vertex({ 3,3 }));
    CHECK(g.has_edge({ 0,0 }, { 1,1 }));
    CHECK(g.has_edge({ 0,0 }, { 3,3 }));
    CHECK_FALSE(g.has_edge({ 1,1 }, { 0,0 }));
    CHECK_FALSE(g.has_edge({ 3,3 }, { 0,0 }));
    CHECK_FALSE(g.has_edge({ 1,1 }, { 3,3 }));
}

TEST_CASE("[Tree] Get vertices", "[Tree]") {
    Tree g;
    g.add_edge({ 0,0 }, { 1,1 });
    g.add_edge({ 0,0 }, { 3,3 });
    CHECK(sorted(g.get_vertices()) == vector<point> { { 0, 0 }, { 1,1 }, { 3,3 }});
}

TEST_CASE("[Tree] Get adjacent vertices", "[Tree]") {
    Tree g;
    g.add_edge({ 0,0 }, { 1,1 });
    g.add_edge({ 0,0 }, { 2,2 }, 4.5);
    CHECK_THROWS(g.add_edge({ 1,1 }, { 2,2 }));
    g.add_edge({ 1,1 }, { 3,3 }, 3.0);
    g.add_edge({ 1,1 }, { 4,4 });
    CHECK(sorted(g.get_outgoing_vertices({ 0,0 })) == vector<point> { { 1, 1 }, { 2,2 }});
    CHECK(sorted(g.get_outgoing_vertices({ 1,1 })) == vector<point> { { 3, 3 }, { 4,4 } });
    CHECK(g.get_incoming_vertex({ 2,2 }) == point{ 0,0 });
    CHECK(g.get_incoming_vertex({ 3,3 }) == point{ 1,1 });
    CHECK(sorted(g.get_outgoing_vertices({ 4,4 })) == vector<point> {});
    CHECK(g.edge_weight({ 1,1 }, { 3,3 }) == 3.0);
    CHECK_THROWS(g.edge_weight({ 2,2 }, { 0,0 }) == 4.5);
    CHECK_THROWS(g.edge_weight({ 0,0 }, { 3,3 }));
}

TEST_CASE("[Tree] Get adjacent edges", "[Tree]") {
    Tree g;
    g.add_edge({ 0,0 }, { 1,1 });
    g.add_edge({ 0,0 }, { 2,2 }, 4.5);
    CHECK_THROWS(g.add_edge({ 1,1 }, { 2,2 }, 3.0));
    g.add_edge({ 1,1 }, { 3,3 }, 3.0);
    CHECK_THROWS(g.add_edge({ 3,3 }, { 1,1 }, 7.2));
    g.add_edge({ 4,4 }, { 0,0 }, 7.2);

    using ESet = set<pair<point, double>>;
    CHECK(to_set(g.get_outgoing_edges({ 0,0 })) == ESet{ make_pair(make_pair(1,1), 1.0), make_pair(make_pair(2,2), 4.5) });
    CHECK(to_set(g.get_outgoing_edges({ 1,1 })) == ESet{ make_pair(make_pair(3,3), 3.0) });
    CHECK(to_set(g.get_outgoing_edges({ 2,2 })) == ESet{});
    CHECK(to_set(g.get_outgoing_edges({ 3,3 })) == ESet{});
    CHECK(to_set(g.get_outgoing_edges({ 4,4 })) == ESet{ make_pair(make_pair(0,0), 7.2) });
    CHECK(g.get_incoming_edge({ 0,0 }) == make_pair(make_pair<long, long>(4, 4), 7.2));
    CHECK_THROWS(g.get_incoming_edge({ 4,4 }));
}


TEST_CASE("[Tree] Insert vertex", "[Tree]") {
    Tree g;
    g.add_edge({ 0,0 }, { 2,2 });
    g.add_edge({ 0,0 }, { 3,3 });
    g.add_edge({ 0,0 }, { 4,4 });
    g.add_edge({ 4,4 }, { 6,6 });
    g.insert_vertex({ 0,0 }, { 1,1 }, { 2,2 });
    CHECK(g.has_vertex({ 1,1 }));
    CHECK_FALSE(g.has_edge({ 0,0 }, { 2,2 }));
    CHECK(g.has_edge({ 0,0 }, { 1,1 }));
    CHECK(g.has_edge({ 1,1 }, { 2,2 }));
    CHECK_THROWS(g.insert_vertex({ 0,0 }, { 5,5 }, { 6,6 }));
}
