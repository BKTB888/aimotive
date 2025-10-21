//
// Created by bktivadar on 10/7/25.
//

#ifndef AIMOTIVE_FIELD_H
#define AIMOTIVE_FIELD_H

#include <vector>
#include <format>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <memory>

#include "Tile.h"

using Coordinate = std::pair<uint16_t, uint16_t>;
using NodeSelector = std::function<std::pair<Coordinate, size_t>(const std::list<std::pair<Coordinate, int>>&, const std::map<Coordinate, size_t>&)>;

class Field {
    std::vector<std::vector<Tile>> field;
    Coordinate start;
    std::set<Coordinate> Cs;

    void initializeRest();
public:
    Field() = default;
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

    [[nodiscard]] const Coordinate& getStart() const;
    [[nodiscard]] const std::set<Coordinate>& getCs() const;
    [[nodiscard]] std::vector<Coordinate> getConnectedCoordinates(const Coordinate &coord) const;

    [[nodiscard]] size_t countPathToCs(const NodeSelector& node_selector) const;
    void countPathToCsDebug(const NodeSelector& node_selector) const;

    friend std::istream& operator>>(std::istream& is, Field& f) {
        using namespace std;
        if (unsigned N, M; is >> N >> M) {
            //ignore a new line character
            is.ignore();
            f.field.resize(N);

            auto i = 0;
            for (string line; i < N && getline(is, line); ++i) {
                if (const auto line_size = line.length(); line_size != M)
                    throw invalid_argument(
                        format("Expected M = {} got {} in line: {}.{}!", M, line_size, i, line)
                    );

                auto& tiles = f.field[i];
                tiles.resize(M);
                for (auto j = 0; j < M; ++j) {
                    tiles[j].setValue(line[j]);
                }
            }

            if (i < N)
                throw invalid_argument(
                    format("Expected N = {} got {}!", N, i)
                );

            f.initializeRest();
        }
        return is;
    }
};


#endif //AIMOTIVE_FIELD_H