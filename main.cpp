#include <fstream>
#include <iostream>
#include <ranges>

#include "Field.h"
#include <algorithm>


int main() {
    using namespace std;

    auto fs = ifstream("input.txt");
    auto& is = cin;

    //Field and node_selectors
    auto field = Field();

    constexpr auto bfs_node_select = [] (const auto& open_set) {
        const auto it = open_set.begin();
        return *it;
    };

    const auto heuristic = [&field] (const auto& coord) {
        static const auto& Cs = field.getCs();
        const auto min_it = ranges::min_element(Cs, {},[&coord] (const auto& c) {
            return abs(coord.first - c.first) + abs(coord.second - c.second);
        });
        return min_it->first;
    };
    const auto a_start_select = [&heuristic] (const auto& open_set) {
        return *ranges::min_element(open_set,{}, [&heuristic] (const auto& tile) {
            return heuristic(tile.second) + tile.first;
        });
    };



    while (is >> field) {
        //cout << field << endl;
        cout << field.countPathToCs(a_start_select) << endl;
    }

    return 0;
}
