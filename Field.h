//
// Created by bktivadar on 10/7/25.
//

#ifndef AIMOTIVE_FIELD_H
#define AIMOTIVE_FIELD_H

#include <vector>
#include <format>
#include <memory>
#include "Tile.h"


class Field {
    std::vector<std::vector<Tile>> field;
    std::pair<uint16_t, uint16_t> start;
    std::vector<std::pair<uint16_t, uint16_t>> Cs;

    void initializeRest();
public:
    explicit Field(std::string_view field_str);
    friend std::ostream& operator<<(std::ostream& os, const Field& f) {
        for (const auto& row : f.field) {
            for (const auto& cell : row) {
                os << cell;
            }
            os << std::endl;
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Field& f) {
        unsigned N, M;
        is >> N >> M;
        //ignore a new line character
        is.ignore();
        f.field.resize(N);

        auto i = 0;
        for (std::string line; i < N && std::getline(is, line); ++i) {
            if (const auto line_size = line.length(); line_size != M)
                throw std::invalid_argument(
                    std::format("Expected M = {} got {} in line: {}.{}!", M, line_size, i, line)
                );

            auto& tiles = f.field[i];
            tiles.resize(M);
            for (auto j = 0; j < M; ++j) {
                const auto new_tile = Tile(line[j]);
                tiles[j] = new_tile;
            }
        }

        if (i < N)
            throw std::invalid_argument(
                std::format("Expected N = {} got {}!", N, i)
            );

        f.initializeRest();
        return is;
    }
};


#endif //AIMOTIVE_FIELD_H