#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>

// Collectible representa un objeto que el jugador puede recoger para puntaje extra.
struct Collectible
{
    sf::CircleShape shape; // forma del coleccionable
    float speed = 360.f; // velocidad de desplazamiento hacia la izquierda

    // Crea un objeto coleccionable con radio fijo y lo coloca en la altura deseada.
    // startX es la coordenada horizontal de aparición; startY es la altura aproximada.
    Collectible(float startX, float startY)
    {
        float radius = 20.f;
        shape.setRadius(radius);
        shape.setFillColor(sf::Color(240, 220, 90));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.f);
        shape.setPosition(startX, std::max(48.f, startY - radius));
    }

    // Desplaza el coleccionable hacia la izquierda cada fotograma.
    void update(float deltaTime)
    {
        shape.move(-speed * deltaTime, 0.f);
    }
};
