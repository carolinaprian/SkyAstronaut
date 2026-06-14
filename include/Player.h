#pragma once

#include "AnimatedSprite.h"
#include <SFML/Graphics.hpp>
#include <string>

// Player representa al astronauta jugable, sus animaciones y su física vertical.
// También maneja el reinicio del estado y el control de cargas de salto.
struct Player
{
    sf::RectangleShape shape; // forma de respaldo si no se encuentra el sprite del astronauta
    AnimatedSprite idleAnim; // animación cuando el jugador está en tierra o sin moverse verticalmente
    AnimatedSprite jumpAnim; // animación usada mientras el jugador sube en el salto
    AnimatedSprite fallAnim; // animación usada cuando el jugador está cayendo
    bool useSprite = false; // determina si se debe dibujar la animación o la forma simple
    float x = 150.f; // coordenada horizontal del jugador
    float y = 0.f; // coordenada vertical del jugador
    float width = 30.f; // ancho del rectángulo de respaldo del jugador
    float height = 91.f; // alto del rectángulo de respaldo del jugador
    float velocityY = 0.f; // velocidad vertical actual del jugador
    float gravity = 400.f; // aceleración hacia abajo aplicada cada segundo
    float boostSpeed = -420.f; // velocidad inicial del salto (valor negativo hacia arriba)
    int charges = 4; // número de saltos disponibles antes de recargar

    Player()
        : shape(sf::Vector2f(width, height))
    {
        shape.setFillColor(sf::Color(200, 230, 255));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.f);
        y = 240.f;
        shape.setPosition(x, y);
    }

    // Restablece al jugador a su posición inicial, velocidad cero y carga de saltos completa.
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

    // Intenta cargar animaciones para el jugador desde archivos específicos.
    // Retorna true si se cargó al menos una animación útil.
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
    // Ejecuta un impulso de salto si quedan cargas.
    // Este método reduce una carga y modifica la velocidad vertical.
    void boost()
    {
        if (charges > 0)
        {
            velocityY = boostSpeed;
            charges -= 1;
        }
    }

    // Calcula la nueva posición vertical y horizontal del jugador,
    // aplica la gravedad y actualiza la animación activa según la velocidad.
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

    // Dibuja al jugador en la ventana, seleccionando el sprite de salto,
    // caída o estado quieto según su velocidad vertical actual.
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

    // Devuelve el rectángulo de colisión actual del jugador,
    // usando el sprite activo si está disponible o la forma simple.
    sf::FloatRect getBounds() const
    {
        if (useSprite)
            return idleAnim.sprite.getGlobalBounds();

        return shape.getGlobalBounds();
    }
};
