#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// AnimatedSprite gestiona una animación simple basada en un conjunto de frames.
// Se usa para mostrar animaciones de jugador como idle, salto y caída.
struct AnimatedSprite
{
    sf::Texture texture; // textura cargada desde archivo
    sf::Sprite sprite; // sprite que se dibuja en pantalla
    bool loaded = false; // indica si la imagen se cargó correctamente
    int frameWidth = 0; // ancho de cada frame de animación
    int frameHeight = 0; // alto de cada frame
    int numFrames = 1; // número de frames en la animación
    int currentFrame = 0; // frame actualmente visible
    float frameTime = 0.12f; // tiempo entre frames
    sf::Clock frameClock; // reloj para controlar la animación

    // Carga una imagen y opcionalmente configura una animación por frames.
    bool load(const std::string &filePath, int frames = 1, int width = 0, int height = 0)
    {
        if (!texture.loadFromFile(filePath))
            return false;

        loaded = true;
        texture.setSmooth(true);
        sprite.setTexture(texture);

        if (frames > 1 && width > 0 && height > 0)
        {
            frameWidth = width;
            frameHeight = height;
            numFrames = frames;
            sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        }

        return true;
    }

    void update()
    {
        if (!loaded || numFrames <= 1)
            return;

        if (frameClock.getElapsedTime().asSeconds() >= frameTime)
        {
            currentFrame = (currentFrame + 1) % numFrames;
            sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
            frameClock.restart();
        }
    }
};
