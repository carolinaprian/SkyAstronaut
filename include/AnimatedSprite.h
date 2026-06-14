#pragma once

#include <SFML/Graphics.hpp>
#include <string>

struct AnimatedSprite
{
    sf::Texture texture;
    sf::Sprite sprite;
    bool loaded = false;
    int frameWidth = 0;
    int frameHeight = 0;
    int numFrames = 1;
    int currentFrame = 0;
    float frameTime = 0.12f;
    sf::Clock frameClock;

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
