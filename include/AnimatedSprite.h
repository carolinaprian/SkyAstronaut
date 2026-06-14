#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// AnimatedSprite gestiona un sprite y su reproducción por frames cuando se cargan hojas de animación.
// Permite avanzar el frame mostrando solo un rectángulo de la textura en cada ciclo.
struct AnimatedSprite
{
    sf::Texture texture; // textura completa que contiene uno o más frames
    sf::Sprite sprite; // sprite que se dibuja en la pantalla con la textura cargada
    bool loaded = false; // true cuando la textura se cargó correctamente
    int frameWidth = 0; // ancho de cada frame individual
    int frameHeight = 0; // alto de cada frame individual
    int numFrames = 1; // total de frames disponibles en la hoja de animación
    int currentFrame = 0; // índice del frame que se está mostrando
    float frameTime = 0.12f; // tiempo en segundos entre cambios de frame
    sf::Clock frameClock; // temporizador para controlar el avance de frames

    // Carga una imagen desde disco y, si se pasan parámetros de frames,
    // divide la textura en un número fijo de frames de tamaño width x height.
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

    // Actualiza el frame visible cuando ha pasado el tiempo suficiente.
    // Si solo hay un frame o la textura no se cargó, no hace nada.
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
