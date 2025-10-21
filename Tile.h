//
// Created by bktivadar on 10/7/25.
//

#ifndef AIMOTIVE_TILE_H
#define AIMOTIVE_TILE_H
#include <iostream>
#include <unordered_set>

enum class TileType {
    EMPTY,
    T,
    X,
    C
};

enum Direction {
    RIGHT = 0x1,
    TOP = 0x2,
    LEFT = 0x4,
    BOTTOM = 0x8
};

Direction oppositeOf(Direction direction);



class Tile {
    unsigned char value;

    static bool isHelperLetter(unsigned char c);
    static unsigned char CharToValue(unsigned char c);
    [[nodiscard]] unsigned char toChar() const;
public:
    Tile() = default;
    explicit Tile(char representation);

    void setValue(unsigned char representation);
    [[nodiscard]] TileType getTileType() const;

    [[nodiscard]] bool isOpenFrom(Direction direction) const;
    [[nodiscard]] std::unordered_set<Direction> getOpenFrom() const;

    friend std::ostream& operator<<(std::ostream& os, const Tile& t) {
        return os << t.toChar();
    }

    friend std::istream& operator>>(std::istream& is, Tile& t) {
        unsigned char representation;
        is >> representation;
        t.setValue(representation);
        return is;
    }
};

#endif //AIMOTIVE_TILE_H