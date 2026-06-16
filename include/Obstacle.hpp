#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdlib>

using namespace std;
using namespace sf;

struct Obstacle
{
    RectangleShape shape;
    float speed = 306.f; // velocidad reducida 15%: antes 360, ahora 306

    Obstacle(float startX, float startY, const Texture *texture = nullptr)
    {
        float size = 72.f; // tamaño fijo de obstáculo 72x72 px
        shape.setSize(Vector2f(size, size));
        if (texture)
        {
            shape.setTexture(texture);
            shape.setTextureRect(IntRect(0, 0, texture->getSize().x, texture->getSize().y));
        }
        shape.setFillColor(Color(220, 80, 80));
        shape.setOutlineColor(Color::White);
        shape.setOutlineThickness(2.f);
        shape.setPosition(startX, max(48.f, startY - size / 2.f));
    }

    void update(float deltaTime)
    {
        shape.move(-speed * deltaTime, 0.f);
    }
};
