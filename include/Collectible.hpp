#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>

using namespace std;
using namespace sf;

struct Collectible
{
    CircleShape shape;
    float speed = 306.f; // velocidad reducida 15% para igualar el scroll más lento

    Collectible(float startX, float startY, const Texture *texture = nullptr)
    {
        float radius = 20.f;
        shape.setRadius(radius);
        if (texture)
            shape.setTexture(texture);
        shape.setFillColor(Color(240, 220, 90));
        shape.setOutlineColor(Color::White);
        shape.setOutlineThickness(2.f);
        shape.setPosition(startX, max(48.f, startY - radius));
    }

    void update(float deltaTime)
    {
        shape.move(-speed * deltaTime, 0.f);
    }
};
