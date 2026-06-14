#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdlib>

// Obstacle representa un obstáculo móvil que el jugador debe evitar.
struct Obstacle
{
    sf::RectangleShape shape; // forma del obstáculo
    float speed = 360.f; // velocidad de desplazamiento hacia la izquierda

    Obstacle(float startX, float startY)
    {
        float width = 64.f + static_cast<float>(std::rand() % 48);
        float height = width; // square-ish obstacles
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color(220, 80, 80));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.f);
        shape.setPosition(startX, std::max(48.f, startY - height / 2.f));
    }

    void update(float deltaTime)
    {
        shape.move(-speed * deltaTime, 0.f);
    }
};
