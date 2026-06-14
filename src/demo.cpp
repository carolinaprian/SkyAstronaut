#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <algorithm>
#include <ctime>

#include "AnimatedSprite.h"
#include "Player.h"
#include "Obstacle.h"
#include "Collectible.h"

// Juego runner simple con astronauta que salta y recoge objetos.
// El código principal inicializa la ventana, carga los assets, crea
// los obstáculos y coleccionables, y controla la lógica de juego.

using namespace std;
using namespace sf;

int main()
{
    // Crear la ventana del juego con tamaño fijo y nombre del título.
    RenderWindow window(VideoMode(800, 600), "Sky Astronaut Runner");
    window.setFramerateLimit(60);

    // Crear el jugador y preparar sus animaciones o forma simple.
    Player astronaut;
    bool loadedAstronaut = astronaut.loadTexture("assets/images/astronaut.png");
    if (!loadedAstronaut)
    {
        loadedAstronaut = astronaut.loadTexture("assets/images/pikachu.png");
        if (!loadedAstronaut)
        {
            std::cout << "No se encontr� astronaut.png, usando forma simple como reserva.\n";
        }
    }

    Font font;
    bool loadedFont = font.loadFromFile("assets/fonts/Minecraft.ttf");
    if (!loadedFont)
    {
        std::cout << "No se encontr� el archivo de fuente: assets/fonts/Minecraft.ttf\n";
    }

    // Fondo del cielo y estrellas que se dibujan detrás de los objetos del juego.
    RectangleShape sky(Vector2f(800.f, 520.f));
    sky.setFillColor(Color(6, 18, 45));

    vector<CircleShape> stars;
    mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    uniform_real_distribution<float> starX(0.f, 800.f);
    uniform_real_distribution<float> starY(0.f, 500.f);
    for (int i = 0; i < 100; i++)
    {
        CircleShape star(static_cast<float>(1 + rand() % 3));
        star.setFillColor(Color(200, 220, 255, 200));
        star.setPosition(starX(rng), starY(rng));
        stars.push_back(star);
    }

    // Listas de obstáculos y coleccionables que aparecerán durante el juego.
    vector<Obstacle> obstacles;
    vector<Collectible> collectibles;

    float obstacleTimer = 0.f;
    float collectibleTimer = 0.f;
    float obstacleInterval = 2.8f; // low spawn rate
    float collectibleInterval = 1.4f; // generous spawn frequency
    int score = 0;
    int highScore = 0;
    bool gameOver = false;

    Clock deltaClock;

    Text titleText;
    Text infoText;
    Text scoreText;
    Text gameOverText;

    // jetpack icon
    Texture jetpackIconTex;
    bool jetpackIconLoaded = jetpackIconTex.loadFromFile("assets/images/jetpack_icon.png");
    Sprite jetpackIconSprite;
    if (jetpackIconLoaded)
    {
        jetpackIconSprite.setTexture(jetpackIconTex);
        jetpackIconSprite.setScale(100.f / jetpackIconTex.getSize().x, 100.f / jetpackIconTex.getSize().y);
    }

    if (loadedFont)
    {
        titleText.setFont(font);
        titleText.setCharacterSize(24);
        titleText.setFillColor(Color::White);
        titleText.setString("Sky Astronaut Runner");
        titleText.setPosition(16.f, 12.f);

        infoText.setFont(font);
        infoText.setCharacterSize(18);
        infoText.setFillColor(Color(200, 200, 255));
        infoText.setPosition(16.f, 46.f);

        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(Color::White);
        scoreText.setPosition(16.f, 520.f + 18.f);

        gameOverText.setFont(font);
        gameOverText.setCharacterSize(32);
        gameOverText.setFillColor(Color(255, 180, 80));
        gameOverText.setPosition(120.f, 260.f);
        gameOverText.setString("GAME OVER\nPresiona R para reiniciar");
    }

    // Bucle principal de juego: procesa eventos, actualiza el estado y dibuja todo.
    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Space && !gameOver)
                {
                    astronaut.boost();
                }

                if (event.key.code == Keyboard::R && gameOver)
                {
                    gameOver = false;
                    score = 0;
                    obstacleTimer = 0.f;
                    collectibleTimer = 0.f;
                    obstacles.clear();
                    collectibles.clear();
                    astronaut.reset();
                }
            }
        }

        if (!gameOver)
        {
            astronaut.update(deltaTime);

            obstacleTimer += deltaTime;
            collectibleTimer += deltaTime;

            // spawn obstacles at random heights (not too close to top/bottom)
            if (obstacleTimer >= obstacleInterval)
            {
                float spawnY = 80.f + static_cast<float>(rand() % 440);
                obstacles.emplace_back(820.f, spawnY);
                obstacleTimer = 0.f;
            }

            // spawn collectibles generously at random heights
            if (collectibleTimer >= collectibleInterval)
            {
                float spawnY = 80.f + static_cast<float>(rand() % 440);
                collectibles.emplace_back(820.f, spawnY);
                collectibleTimer = 0.f;
            }

            for (auto &obstacle : obstacles)
                obstacle.update(deltaTime);

            for (auto &collectible : collectibles)
                collectible.update(deltaTime);

            obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](const Obstacle &obs) {
                return obs.shape.getPosition().x + obs.shape.getSize().x < -20.f;
            }), obstacles.end());

            collectibles.erase(std::remove_if(collectibles.begin(), collectibles.end(), [](const Collectible &item) {
                return item.shape.getPosition().x + item.shape.getRadius() * 2.f < -20.f;
            }), collectibles.end());

            // forgiving hitbox scale
            auto shrinkBounds = [](const FloatRect &r, float scale) {
                float nw = r.width * scale;
                float nh = r.height * scale;
                return FloatRect(r.left + (r.width - nw) / 2.f, r.top + (r.height - nh) / 2.f, nw, nh);
            };

            FloatRect playerBounds = shrinkBounds(astronaut.getBounds(), 0.8f);

            for (const auto &obstacle : obstacles)
            {
                if (playerBounds.intersects(shrinkBounds(obstacle.shape.getGlobalBounds(), 0.8f)))
                {
                    gameOver = true;
                    highScore = max(highScore, score);
                    break;
                }
            }

            for (auto it = collectibles.begin(); it != collectibles.end();)
            {
                if (playerBounds.intersects(shrinkBounds(it->shape.getGlobalBounds(), 0.8f)))
                {
                    // give one charge and a small score bump
                    astronaut.charges += 1;
                    score += 10;
                    it = collectibles.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            // death if touching top or bottom of screen
            if (astronaut.y <= 0.f || astronaut.y + astronaut.height >= 600.f)
            {
                gameOver = true;
                highScore = max(highScore, score);
            }
        }

        if (loadedFont)
        {
            string info = "Espacio = Saltar  |  R = Reiniciar";
            if (gameOver)
                info = "Juego terminado. Presiona R para intentar otra vez.";

            infoText.setString(info);
            scoreText.setString("Puntaje: " + to_string(score) + "   Mejor: " + to_string(highScore));
        }

        window.clear();

        window.draw(sky);

        for (const auto &star : stars)
            window.draw(star);

        for (const auto &collectible : collectibles)
            window.draw(collectible.shape);

        for (const auto &obstacle : obstacles)
            window.draw(obstacle.shape);

        astronaut.draw(window);

        // HUD and text
        if (loadedFont)
        {
            window.draw(titleText);
            window.draw(infoText);
            window.draw(scoreText);

            // draw charges top-left: e.g. "4x [icon]"
            string chargeStr = to_string(astronaut.charges) + " x ";
            Text chargesText;
            chargesText.setFont(font);
            chargesText.setCharacterSize(20);
            chargesText.setFillColor(Color::White);
            chargesText.setPosition(16.f, 80.f);
            chargesText.setString(chargeStr);
            window.draw(chargesText);

            if (jetpackIconLoaded)
            {
                jetpackIconSprite.setPosition(16.f + 36.f, 80.f - 4.f);
                window.draw(jetpackIconSprite);
            }

            if (gameOver)
                window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}