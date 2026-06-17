#pragma once

#include "AnimatedSprite.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>

using namespace std;
using namespace sf;
namespace fs = std::filesystem;

struct Player
{
    RectangleShape shape;
    AnimatedSprite idleAnim;
    AnimatedSprite jumpAnim;
    bool useSprite = false;
    float x = 150.f;
    float y = 0.f;
    float width = 64.f; // tamaño reducido del astronauta a 64x64px
    float height = 64.f;
    float velocityY = 0.f;
    float gravity = 400.f;
    float boostSpeed = -252.f; // fuerza de salto reducida 40%: antes -420, ahora -252
    int charges = 4;

    Player()
        : shape(Vector2f(width, height))
    {
        shape.setFillColor(Color(200, 230, 255));
        shape.setOutlineColor(Color::White);
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
        }
    }

    bool loadTexture(const fs::path &basePath)
    {
        bool any = false;
        const float displaySize = 64.f;
        auto applyScale = [&](AnimatedSprite &anim)
        {
            float texW = anim.frameWidth > 0 ? static_cast<float>(anim.frameWidth) : static_cast<float>(anim.texture.getSize().x);
            float texH = anim.frameHeight > 0 ? static_cast<float>(anim.frameHeight) : static_cast<float>(anim.texture.getSize().y);
            if (texW > 0.f && texH > 0.f)
                anim.sprite.setScale(displaySize / texW, displaySize / texH);
        };

        fs::path root = basePath;
        if (fs::exists(root) && fs::is_regular_file(root))
            root = root.parent_path();
        if (!fs::exists(root) || !fs::is_directory(root))
            root = fs::current_path();

        fs::path imagesRoot = root;
        if (root.filename() == "assets")
            imagesRoot = root / "images";
        else if (fs::exists(root / "assets"))
            imagesRoot = root / "assets" / "images";
        else
            imagesRoot = root / "images";

        auto imagePath = [&](const string &name) {
            return (imagesRoot / name).string();
        };

        if (idleAnim.load(imagePath("astronaut_idle.png"), 2, 96, 96))
        {
            any = true;
            idleAnim.frameTime = 0.18f;
            applyScale(idleAnim);
        }
        else if (idleAnim.load(imagePath("astronaut_idle 2 frame.png"), 2, 96, 96))
        {
            any = true;
            idleAnim.frameTime = 0.18f;
            applyScale(idleAnim);
        }

        if (jumpAnim.load(imagePath("astronaut_jump.png"), 3, 96, 96))
        {
            any = true;
            jumpAnim.frameTime = 0.1f;
            applyScale(jumpAnim);
        }

        if (!any)
        {
            if (idleAnim.load(imagePath("astronaut.png")))
            {
                any = true;
                idleAnim.frameTime = 0.18f;
                applyScale(idleAnim);
            }
        }

        if (!any)
        {
            if (idleAnim.load(imagePath("pikachu.png")))
            {
                any = true;
                idleAnim.frameTime = 0.18f;
                applyScale(idleAnim);
            }
        }

        useSprite = any;
        if (useSprite)
        {
            idleAnim.sprite.setPosition(x, y);
            jumpAnim.sprite.setPosition(x, y);
        }

        return useSprite;
    }

    void boost()
    {
        if (charges > 0)
        {
            velocityY = boostSpeed;
            charges -= 1;
        }
    }

    void update(float deltaTime)
    {
        velocityY += gravity * deltaTime;
        y += velocityY * deltaTime;

        x += 80.f * deltaTime;
        if (x > 340.f)
            x = 340.f;

        if (useSprite)
        {
            idleAnim.update();
            jumpAnim.update();
            idleAnim.sprite.setPosition(x, y);
            jumpAnim.sprite.setPosition(x, y);
        }
        else
        {
            shape.setPosition(x, y);
        }
    }

    void draw(RenderWindow &window)
    {
        if (useSprite)
        {
            const float thresh = 20.f;
            if (velocityY < -thresh && jumpAnim.loaded)
                window.draw(jumpAnim.sprite);
            else
                window.draw(idleAnim.sprite);
        }
        else
        {
            window.draw(shape);
        }
    }

    FloatRect getBounds() const
    {
        if (useSprite)
            return idleAnim.sprite.getGlobalBounds();

        return shape.getGlobalBounds();
    }
};
