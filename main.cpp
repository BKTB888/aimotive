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

    const NodeSelector bfs_node_select = [] (const auto& open_set) {
        const auto it = open_set.begin();
        return *it;
    };

    const auto heuristic = [&field] (const auto& coord) {
        static const auto& Cs = field.getCs();

        //A legközelebbi C-nek a távolsaága
        return ranges::min(
        Cs | std::views::transform([&coord](const auto& c) {
            return manhattanDistance(c, coord);
        })
    );

    };
    const NodeSelector a_start_select = [&heuristic] (const auto& open_set) {
        return *ranges::min_element(open_set,{}, [&heuristic] (const auto& tile) {
            return heuristic(tile.second) + tile.first;
        });
    };

    const auto selections = {
        make_pair("A*", a_start_select),
        make_pair("BFS", bfs_node_select)
    };
    while (is >> field) {
        for (const auto& [name, selection] : selections) {
            cout << name << ':' << endl;
            field.countPathToCsDebug(selection);
            cout << endl;
        }
    }

    return 0;
}
