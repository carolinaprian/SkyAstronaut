#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdlib>

// Obstacle representa un obstáculo móvil que el jugador debe evitar.
struct Obstacle
{
    sf::RectangleShape shape; // forma del obstáculo
    float speed = 360.f; // velocidad de desplazamiento hacia la izquierda

    // Crea un obstáculo con tamaño aleatorio y lo sitúa en la posición inicial.
    // startX es la coordenada de aparición horizontal; startY es la altura aproximada.
    // Si se proporciona una textura, la aplica al obstáculo.
    Obstacle(float startX, float startY, const sf::Texture *texture = nullptr)
    {
        float width = 64.f + static_cast<float>(std::rand() % 48);
        float height = width; // square-ish obstacles
        shape.setSize(sf::Vector2f(width, height));
        if (texture)
            shape.setTexture(texture);
        shape.setFillColor(sf::Color(220, 80, 80));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.f);
        shape.setPosition(startX, std::max(48.f, startY - height / 2.f));
    }

    // Desplaza el obstáculo hacia la izquierda según su velocidad y el tiempo.
    void update(float deltaTime)
    {
        shape.move(-speed * deltaTime, 0.f);
    }
};
