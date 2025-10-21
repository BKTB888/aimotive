#include <fstream>
#include <iostream>
#include <ranges>

#include "Field.h"
#include <algorithm>

size_t manhattanDistance(const Coordinate& from, const Coordinate& to) {
    return abs(from.first - to.first) + abs(from.second - to.second);
}

int main() {
    using namespace std;

    auto fs = ifstream("input.txt");
    auto& is = fs;

    //Field and node_selectors
    auto field = Field();

    const NodeSelector bfs_node_select = [] (const auto& open_set, const auto&) {
        const auto it = open_set.begin();
        return *it;
    };


    const auto heuristic = [&field] (const auto& coord, const auto& found_cs) {
        static const auto& Cs = field.getCs();


        //A legközelebbi C-vel szomszédos mezőnek a távolsaága, ami még nincs meg
        return ranges::min(
        Cs | views::filter([&found_cs] (const auto& c) {
            return !found_cs.contains(c);
        }) | views::transform([&field] (const auto& c) {
            return field.getConnectedCoordinates(c);
        }) | views::join
        | views::transform([&coord](const auto& c) {
            return manhattanDistance(c, coord);
        })
    );

    };
    const NodeSelector a_star_select = [&heuristic] (const auto& open_set, const auto& found_cs) {
        return *ranges::min_element(open_set,{}, [&heuristic, &found_cs] (const auto& tile) {
            return heuristic(tile.first, found_cs) + tile.second;
        });
    };

    const auto selections = {
        make_pair("A*", a_star_select),
        make_pair("BFS", bfs_node_select)
    };
    while (is >> field) {
        for (const auto& [name, selection] : selections) {
            cout << name << ':' << endl;
            field.countPathToCsDebug(selection);
            cout << endl;
            cout << "Non Debug:" << field.countPathToCs(selection) << endl;
        }
    }

    return 0;
}
