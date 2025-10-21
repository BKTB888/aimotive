//
// Created by bktivadar on 10/7/25.
//

#include "Field.h"

#include <algorithm>
#include <list>
#include <map>
#include <ranges>
#include <numeric>

using namespace std;

void Field::initializeRest() {
    const auto M = field[0].size();
    auto T_count = 0;
    Cs.clear();
    for (auto i = 0; i < field.size(); i++) {
        for (auto j = 0; j < M; j++) {
            if (const auto tile = field[i][j]; tile.getTileType() == TileType::C)
                Cs.emplace(i, j);
            else if (tile.getTileType() == TileType::T) {
                start.first = i;
                start.second = j;
                ++T_count;
            }
        }
    }
    if (T_count != 1)
        throw invalid_argument(format("There should be only 1 T, found {}", T_count));
}

vector<Coordinate> Field::getConnectedCoordinates(const Coordinate &coord) const {
    const auto& [current_i, current_j] = coord;
    const auto N = field.size();
    const auto M = field[0].size();
    const auto& current_tile = field[current_i][current_j];

    // Megnézzük, hogy nyitva vannak-e a megfelelő irányból
    return map<Direction, pair<uint16_t, uint16_t>>{
        {TOP, {current_i + 1, current_j}},//Alsó mező, de neki én felül vagyok
        {LEFT, {current_i, current_j + 1}}, //Jobb oldali mező, de neki én bal oldalt vagyok
        {BOTTOM, {current_i - 1, current_j}}, //...
        {RIGHT, {current_i, current_j - 1}} // ...
    } | views::filter([this, &N, &M, &current_tile] (const auto& direction_coord) {
        const auto [direction, coord] = direction_coord;
        const auto& [i, j] = coord;
        return  i < N && j < M && field[i][j].isOpenFrom(direction) && current_tile.isOpenFrom(oppositeOf(direction));
    }) | views::transform([] (const auto& direction_coord) {
        return direction_coord.second;
    })| ranges::to<vector<Coordinate>>();
}

Field::Field(string_view field_str) {
    auto lines = field_str
       | views::split('\n');

    const auto M = lines.front().size();

    field = lines
       | views::transform([M] (const auto& line) {

           if (const auto line_size = line.size(); line_size != M)
               throw invalid_argument(format("Every line should have length {}, but current has {}!", M, line_size));

           return line | views::transform([] (const auto& ch) {
               return Tile(ch);
           }) | ranges::to<vector>();
       })
       | ranges::to<vector>();

    this->initializeRest();
}

const Coordinate& Field::getStart() const {
    return start;
}

const std::set<Coordinate>& Field::getCs() const {
    return Cs;
}

static void print_found(const std::map<Coordinate, size_t>& s) {
    std::cout << "{\n";
    for (const auto& [coord, num] : s) {
        std::cout << "  (" << num
                  << ", (" << coord.first << ", " << coord.second << "))\n";
    }
    std::cout << "}\n";
}

size_t Field::countPathToCs(const NodeSelector& node_selector) const {
    auto current = make_pair(start, -1);
    auto discovered_coords = set{start};
    auto open = list{current};
    auto found_cs = map<Coordinate, size_t>();

    while (found_cs.size() != Cs.size() && !open.empty()) {
        const auto& [current_coord, distance] = current;
        open.remove(current);

        for (
            const auto& connected_coord: getConnectedCoordinates(current_coord)
        ) {
            const auto new_distance = distance + 1;
            if (!discovered_coords.contains(connected_coord)) {
                discovered_coords.insert(connected_coord);
                const auto connected = make_pair(connected_coord, new_distance);
                if (Cs.contains(connected_coord)) {
                    found_cs.insert(connected);
                } else {
                    open.push_back(connected);
                }
            }
        }
        const auto next = node_selector(open, found_cs);
        current = next;
    }
    return ranges::fold_left(found_cs, 0, [](const auto acc, const auto& c) {
        return acc + c.second;
    });
}
void Field::countPathToCsDebug(const NodeSelector &node_selector) const {
    auto current = make_pair(start, vector<Coordinate>());
    auto discovered_coords = set{start};
    auto open = list{
        current
    };
    auto found_cs = map<Coordinate, vector<Coordinate>>();

    auto i = 0;
    while (found_cs.size() != Cs.size() && !open.empty()) {
        ++i;
        const auto& current_coord = current.first;
        open.remove(current);

        for (
            const auto& connected_coord: getConnectedCoordinates(current_coord)
        ) {
            if (!discovered_coords.contains(connected_coord)) {
                auto new_path = current.second;
                new_path.push_back(connected_coord);
                discovered_coords.insert(connected_coord);

                const auto connected = make_pair(connected_coord, new_path);
                if (Cs.contains(connected_coord)) {
                    found_cs.insert(connected);
                } else {
                    open.push_back(connected);
                }
            }
        }
        const auto& converted = open | views::transform([] (const auto& coord_and_path) {
            const auto& [coord, path] = coord_and_path;
            return make_pair(coord, static_cast<int>(path.size()));
        }) | ranges::to<list>();
        const auto& converted_cs = found_cs | views::transform([] (const auto& coord_and_path) {
            const auto& [coord, path] = coord_and_path;
            return make_pair(coord, path.size());
        }) | ranges::to<map>();
        const auto next_coord = node_selector(converted, converted_cs).first;
        const auto current_it = ranges::find_if(open, [&next_coord](const auto& coord_and_path) {
            const auto coord = coord_and_path.first;
            return coord == next_coord;
        });
        if (current_it != open.end()) {current = *current_it;}
    }

    cout << "Visited nodes: " << i << endl;
    const auto all_paths = found_cs | views::transform([](const auto& coord_and_path) {
        return coord_and_path.second;
    }) | views::join | ranges::to<vector>();
    const auto all_paths_set = set(all_paths.begin(), all_paths.end());
    for (auto i = 0; i < field.size(); ++i) {
        for (auto j = 0; j < field[0].size(); ++j) {
            const auto wasVisited = all_paths_set.contains(make_pair(i, j));
            if (wasVisited) {cout << "\o{33}[31m"; }
            cout << field[i][j];
            if (wasVisited) {cout << "\o{33}[0m";}
        }
        cout << endl;
    }
    cout << "Result:" << all_paths.size() - found_cs.size() << endl;
}
