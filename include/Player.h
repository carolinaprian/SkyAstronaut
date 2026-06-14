#pragma once

#include "AnimatedSprite.h"
#include <SFML/Graphics.hpp>
#include <string>

// Player representa al astronauta jugable y su comportamiento físico.
struct Player
{
    sf::RectangleShape shape; // forma usada cuando no hay sprite cargado
    AnimatedSprite idleAnim; // animación al estar en el suelo
    AnimatedSprite jumpAnim; // animación al saltar
    AnimatedSprite fallAnim; // animación al caer
    bool useSprite = false; // si se usa animación de sprite o forma simple
    float x = 150.f; // posición horizontal
    float y = 0.f; // posición vertical
    float width = 30.f; // ancho del jugador cuando se usa la forma simple
    float height = 91.f; // alto del jugador en forma simple
    float velocityY = 0.f; // velocidad vertical actual
    float gravity = 400.f; // gravedad aplicada cada fotograma
    float boostSpeed = -420.f; // fuerza de salto
    int charges = 4; // cargas de salto disponibles

    Player()
        : shape(sf::Vector2f(width, height))
    {
        shape.setFillColor(sf::Color(200, 230, 255));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.f);
        y = 240.f;
        shape.setPosition(x, y);
    }

    void reset()
    {
        x = 150.f;
        y = 240.f;
        velocityY = 0.f;
        shape.setPosition(x, y);
        charges = 4;
        if (useSprite)
        {
            idleAnim.sprite.setPosition(x, y);
            jumpAnim.sprite.setPosition(x, y);
            fallAnim.sprite.setPosition(x, y);
        }
    }

    // Intenta cargar texturas para los diferentes estados del jugador.
    // Devuelve true si se cargó al menos una animación.
    bool loadTexture(const std::string &path)
    {
        bool any = false;
        if (idleAnim.load("assets/images/astronaut_idle.png", 2, 96, 96))
        {
            any = true;
            idleAnim.frameTime = 0.18f;
        }
        if (jumpAnim.load("assets/images/astronaut_jump.png", 3, 96, 96))
        {
            any = true;
            jumpAnim.frameTime = 0.1f;
        }
        if (fallAnim.load("assets/images/astronaut_fall.png", 2, 96, 96))
        {
            any = true;
            fallAnim.frameTime = 0.14f;
        }

        useSprite = any;
        if (useSprite)
        {
            idleAnim.sprite.setPosition(x, y);
            jumpAnim.sprite.setPosition(x, y);
            fallAnim.sprite.setPosition(x, y);
        }

        return useSprite;
    }

    // Ejecuta el salto si hay cargas disponibles.
    void boost()
    {
        if (charges > 0)
        {
            velocityY = boostSpeed;
            charges -= 1;
        }
    }

    // Actualiza la posición y la animación del jugador.
    void update(float deltaTime)
    {
        velocityY += gravity * deltaTime;
        y += velocityY * deltaTime;

        x += 80.f * deltaTime; // auto-move right
        if (x > 340.f)
            x = 340.f;

        if (useSprite)
        {
            idleAnim.update();
            jumpAnim.update();
            fallAnim.update();
            idleAnim.sprite.setPosition(x, y);
            jumpAnim.sprite.setPosition(x, y);
            fallAnim.sprite.setPosition(x, y);
        }
        else
        {
            shape.setPosition(x, y);
        }
    }

    void draw(sf::RenderWindow &window)
    {
        if (useSprite)
        {
            const float thresh = 20.f;
            if (velocityY < -thresh && jumpAnim.loaded)
                window.draw(jumpAnim.sprite);
            else if (velocityY > thresh && fallAnim.loaded)
                window.draw(fallAnim.sprite);
            else
                window.draw(idleAnim.sprite);
        }
        else
        {
            window.draw(shape);
        }
    }

    sf::FloatRect getBounds() const
    {
        if (useSprite)
            return idleAnim.sprite.getGlobalBounds();

        return shape.getGlobalBounds();
    }
};
