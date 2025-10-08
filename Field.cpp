//
// Created by bktivadar on 10/7/25.
//

#include "Field.h"
#include <ranges>

void Field::initializeRest() {
    const auto M = field[0].size();
    auto T_count = 0;
    for (auto i = 0; i < field.size(); i++) {
        for (auto j = 0; j < M; j++) {
            if (const auto tile = field[i][j]; tile.getTileType() == C)
                Cs.push_back(std::make_pair(i, j));
            else if (tile.getTileType() == T) {
                start = std::make_pair(i, j);
                ++T_count;
            }
        }
    }
    if (T_count != 1)
        throw std::invalid_argument(std::format("There should be only 1 T, found {}", T_count));
}

Field::Field(std::string_view field_str) {
    auto lines = field_str
       | std::views::split('\n');

    const auto M = lines.front().size();

    field = lines
       | std::views::transform([M] (const auto& line) {

           if (const auto line_size = line.size(); line_size != M)
               throw std::invalid_argument(std::format("Every line should have length {}, but current has {}!", M, line_size));

           return line | std::views::transform([] (const auto& ch) {
               return Tile(ch);
           }) | std::ranges::to<std::vector>();
       })
       | std::ranges::to<std::vector>();

    this->initializeRest();
}
