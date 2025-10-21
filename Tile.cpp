//
// Created by bktivadar on 10/7/25.
//

#include "Tile.h"

#include <cmath>

constexpr auto allDirections = std::array{
    RIGHT,
    TOP,
    LEFT,
    BOTTOM
};

Direction oppositeOf(const Direction direction) {
    return allDirections[abs((std::bit_width(static_cast<unsigned>(direction)) - 3) % 4)];
}

bool Tile::isHelperLetter(const unsigned char c) {
    return c == '-' || c == 'X' || c == 'T';
}

unsigned char Tile::CharToValue(const unsigned char c) {
    if (isHelperLetter(c)) return c;
    if (isdigit(c) && c != '0') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    throw std::invalid_argument(
        std::format("Invalid character '{}'", c)
    );
}

unsigned char Tile::toChar() const {
    if (isHelperLetter(value)) return value;
    if (value <= 9) return value + '0';
    return 'A' + (value - 10);
}

Tile::Tile(const char representation): value(CharToValue(representation)) {}

void Tile::setValue(const unsigned char representation) {
    value = CharToValue(representation);
}

TileType Tile::getTileType() const {
    using enum TileType;
    return
       (value == '-') ? EMPTY :
       (value == 'X') ? X :
       (value == 'T') ? T :
        C;
}

bool Tile::isOpenFrom(const Direction direction) const {
    return value == '-' || (value != 'X' && (value == 'T' || value & direction));
}

std::unordered_set<Direction> Tile::getOpenFrom() const {
    switch (getTileType()) {
        using enum TileType;
        case EMPTY: return std::unordered_set(allDirections.begin(), allDirections.end());
        case T: [[fallthrough]];
        case X: return {};
        case C:
            auto directions = std::unordered_set<Direction>();
            for (const auto direction: allDirections) {
                if (value & direction) directions.insert( direction);
            }
            return directions;
    }
}
