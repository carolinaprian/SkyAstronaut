#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>

struct Collectible
{
    sf::CircleShape shape;
    float speed = 360.f;

    Collectible(float startX, float startY)
    {
        float radius = 20.f;
        shape.setRadius(radius);
        shape.setFillColor(sf::Color(240, 220, 90));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.f);
        shape.setPosition(startX, std::max(48.f, startY - radius));
    }

    void update(float deltaTime)
    {
        shape.move(-speed * deltaTime, 0.f);
    }
};
