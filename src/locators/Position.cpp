//
// Created by Oliver Bailey on 05/07/2025.
//

#include "locators/Position.hpp"
#include "locators/Move.hpp"
#include <cmath>

Position::Position(const sf::Vector2i posToConvert)
    : x(posToConvert.x), y(posToConvert.y) {}

double Position::distanceBetween(const Position otherPosition) const {
    return std::sqrt(std::pow(otherPosition.x - x, 2) + std::pow(otherPosition.y - y, 2));
}

std::string Position::str() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}


// std::ostream& operator<<(std::ostream& os, const Position& position) {
//     return os << ;
// }