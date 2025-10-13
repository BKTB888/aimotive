//
// Created by bktivadar on 10/7/25.
//

#include "Field.h"

#include <algorithm>
#include <list>
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
    const int current_i = coord.first;
    const int current_j = coord.second;

    // Megnézzük, hogy nyitva vannak-e a megfelelő irányból
    const auto possibles = {
        make_pair(TOP, make_pair(current_i + 1, current_j)),//Alsó mező, de neki én felül vagyok
        make_pair(LEFT, make_pair(current_i, current_j + 1)), //Jobb oldali mező, de neki én bal oldalt vagyok
        make_pair(BOTTOM, make_pair(current_i - 1, current_j)), //...
        make_pair(RIGHT, make_pair(current_i, current_j - 1)) // ...
    };
    auto result = vector<Coordinate>();
    const auto N = field.size();
    const auto M = field[0].size();
    for (const auto& [direction, tile_coord]: possibles) {
        const auto i = tile_coord.first;
        const auto j = tile_coord.second;
        if (i < N && j < M && i >= 0 && j >= 0 && field[i][j].isOpenFrom(direction)) {
            result.emplace_back(tile_coord);
        }
    }

    return result;
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

static void print_set(const std::set<std::pair<size_t, Coordinate>>& s) {
    std::cout << "{\n";
    for (const auto& [id, coord] : s) {
        std::cout << "  (" << id
                  << ", (" << coord.first << ", " << coord.second << "))\n";
    }
    std::cout << "}\n";
}

size_t Field::countPathToCs(const function<pair<size_t, Coordinate>(list<pair<int, Coordinate>>)>& node_selector) const {
    auto current = make_pair(-1, start);
    auto discovered_coords = set{start};
    auto open = list{current};
    auto found_cs = set<pair<size_t, Coordinate>>();

    while (found_cs.size() != Cs.size() && !open.empty()) {
        open.remove(current);
        const auto distance = current.first;
        const auto current_coord = current.second;
        const auto connected_coords = getConnectedCoordinates(current_coord);
        for (const auto& connected_coord: connected_coords) {
            const auto new_distance = distance + 1;
            if (!discovered_coords.contains(connected_coord)) {
                discovered_coords.insert(connected_coord);
                const auto connected = make_pair(new_distance, connected_coord);
                if (Cs.contains(connected_coord)) {
                    found_cs.insert(connected);
                } else {
                    open.push_back(connected);
                }
            }
        }
        const auto next = node_selector(open);
        current = next;
    }
    //For debuging
    //print_set(found_cs);
    return ranges::fold_left(found_cs, 0, [](const auto acc, const auto& c) {
        return acc + c.first;
    });
}