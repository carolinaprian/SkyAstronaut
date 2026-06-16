#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <algorithm>
#include <ctime>

#include "AnimatedSprite.hpp"
#include "Player.hpp"
#include "Obstacle.hpp"
#include "Collectible.hpp"

// Juego runner donde el astronauta corre automáticamente,
// salta para evitar obstáculos y recoge objetos dorados para sumar puntos.
// El programa inicializa la ventana, carga las texturas y la fuente,
// genera el fondo estelar, crea los objetos del juego y ejecuta el bucle principal.

using namespace std;
using namespace sf;

enum class GameState
{
    COVER,
    PLAYING,
    GAMEOVER
};

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

    // Fondo principal del juego: un rectángulo oscuro que simula el cielo.
    // Se dibujan estrellas sobre este fondo antes de los obstáculos y el jugador.
    Texture coverTexture;
    bool coverTextureLoaded = coverTexture.loadFromFile("assets/images/cover.png");
    Sprite coverSprite;
    if (coverTextureLoaded)
    {
        coverSprite.setTexture(coverTexture);
        float scaleX = 800.f / coverTexture.getSize().x;
        float scaleY = 600.f / coverTexture.getSize().y;
        coverSprite.setScale(scaleX, scaleY);
    }

    RectangleShape sky(Vector2f(800.f, 520.f));
    sky.setFillColor(Color(6, 18, 45));

    Texture alienTexture;
    bool alienTextureLoaded = alienTexture.loadFromFile("assets/images/alien.png");
    Sprite alienSprite;
    if (alienTextureLoaded)
    {
        alienSprite.setTexture(alienTexture);
        alienSprite.setScale(60.f / alienTexture.getSize().x, 60.f / alienTexture.getSize().y);
        alienSprite.setPosition(700.f, 16.f);
    }

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

    // Almacenan los objetos en pantalla: obstáculos que matan y coleccionables que suman puntos.
    vector<Obstacle> obstacles;
    vector<Collectible> collectibles;

    float obstacleTimer = 0.f;
    float collectibleTimer = 0.f;
    float obstacleInterval = 2.8f; // intervalo entre apariciones de obstáculos
    float collectibleInterval = 1.4f; // intervalo entre apariciones de coleccionables
    int score = 0;
    int highScore = 0;
    bool gameOver = false;
    GameState gameState = GameState::COVER;

    Clock deltaClock;

    Text titleText;
    Text infoText;
    Text scoreText;
    Text gameOverText;
    Text coverText;
    Text creditText;

    // Texturas de assets usados en el HUD y en los objetos del juego.
    Texture jetpackIconTex;
    bool jetpackIconLoaded = jetpackIconTex.loadFromFile("assets/images/jetpack_icon.png");
    Sprite jetpackIconSprite;
    if (jetpackIconLoaded)
    {
        jetpackIconSprite.setTexture(jetpackIconTex);
        jetpackIconSprite.setScale(100.f / jetpackIconTex.getSize().x, 100.f / jetpackIconTex.getSize().y);
    }

    Texture collectibleTexture;
    bool collectibleTextureLoaded = collectibleTexture.loadFromFile("assets/images/jetpack_collectible.png");

    Texture obstacleTexture;
    bool obstacleTextureLoaded = obstacleTexture.loadFromFile("assets/images/planet_obstacule.png");

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

        coverText.setFont(font);
        coverText.setCharacterSize(22);
        coverText.setFillColor(Color(235, 235, 255));
        coverText.setString("Presiona cualquier tecla para comenzar");
        FloatRect coverBounds = coverText.getLocalBounds();
        coverText.setOrigin(coverBounds.left + coverBounds.width / 2.f, coverBounds.top + coverBounds.height / 2.f);
        coverText.setPosition(400.f, 520.f);

        creditText.setFont(font);
        creditText.setCharacterSize(16);
        creditText.setFillColor(Color(180, 190, 235));
        creditText.setString("Creado por: Carolina Prian y Karla Hernandez");
        creditText.setPosition(16.f, 568.f);

        gameOverText.setFont(font);
        gameOverText.setCharacterSize(32);
        gameOverText.setFillColor(Color(255, 180, 80));
        gameOverText.setPosition(120.f, 260.f);
        gameOverText.setString("GAME OVER\nPresiona R para reiniciar");
    }

    // Bucle principal del juego.
    // En cada iteración procesa entradas del jugador, actualiza la lógica y renderiza la escena.
    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();
        Event event;
        while (window.pollEvent(event))
        {
            // Cerrar la ventana si el jugador pulsa la X o el evento de cierre se activa.
            if (event.type == Event::Closed)
                window.close();

            // Procesar las teclas dentro del juego.
            if (event.type == Event::KeyPressed)
            {
                if (gameState == GameState::COVER)
                {
                    gameState = GameState::PLAYING;
                    continue;
                }

                // Espacio provoca el salto si el juego ya empezó y no terminó.
                if (event.key.code == Keyboard::Space && gameState == GameState::PLAYING && !gameOver)
                {
                    astronaut.boost();
                }

                // R reinicia el juego después de perder.
                if (event.key.code == Keyboard::R && gameState == GameState::GAMEOVER)
                {
                    gameOver = false;
                    gameState = GameState::PLAYING;
                    score = 0;
                    obstacleTimer = 0.f;
                    collectibleTimer = 0.f;
                    obstacles.clear();
                    collectibles.clear();
                    astronaut.reset();
                }
            }
        }

        if (gameState == GameState::PLAYING && !gameOver)
        {
            // Actualiza la posición del jugador y su animación según el tiempo transcurrido.
            astronaut.update(deltaTime);

            // Sumar tiempo para decidir cuándo generar nuevos objetos.
            obstacleTimer += deltaTime;
            collectibleTimer += deltaTime;

            // Genera obstáculos de forma periódica con una altura aleatoria.
            // El rango evita que aparezcan demasiado cerca del techo o del suelo.
            if (obstacleTimer >= obstacleInterval)
            {
                float spawnY = 80.f + static_cast<float>(rand() % 440);
                obstacles.emplace_back(820.f, spawnY, obstacleTextureLoaded ? &obstacleTexture : nullptr);
                obstacleTimer = 0.f;
            }

            // Genera coleccionables con mayor frecuencia; son premios que el jugador puede recoger.
            if (collectibleTimer >= collectibleInterval)
            {
                float spawnY = 80.f + static_cast<float>(rand() % 440);
                collectibles.emplace_back(820.f, spawnY, collectibleTextureLoaded ? &collectibleTexture : nullptr);
                collectibleTimer = 0.f;
            }

            // Mueve cada obstáculo y coleccionable hacia la izquierda.
            for (auto &obstacle : obstacles)
                obstacle.update(deltaTime);

            for (auto &collectible : collectibles)
                collectible.update(deltaTime);

            // Elimina objetos que ya salieron completamente de la parte izquierda de la pantalla.
            obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](const Obstacle &obs) {
                return obs.shape.getPosition().x + obs.shape.getSize().x < -20.f;
            }), obstacles.end());

            collectibles.erase(std::remove_if(collectibles.begin(), collectibles.end(), [](const Collectible &item) {
                return item.shape.getPosition().x + item.shape.getRadius() * 2.f < -20.f;
            }), collectibles.end());

            // Ajusta las cajas de colisión para que sean un poco más pequeñas.
            // Esto crea un margen de tolerancia y evita colisiones demasiado estrictas.
            auto shrinkBounds = [](const FloatRect &r, float scale) {
                float nw = r.width * scale;
                float nh = r.height * scale;
                return FloatRect(r.left + (r.width - nw) / 2.f, r.top + (r.height - nh) / 2.f, nw, nh);
            };

            FloatRect playerBounds = shrinkBounds(astronaut.getBounds(), 0.8f);

            // Comprueba colisiones con cada obstáculo; si hay impacto, termina el juego.
            for (const auto &obstacle : obstacles)
            {
                if (playerBounds.intersects(shrinkBounds(obstacle.shape.getGlobalBounds(), 0.8f)))
                {
                    gameOver = true;
                    gameState = GameState::GAMEOVER;
                    highScore = max(highScore, score);
                    break;
                }
            }

            // Recoge los coleccionables que el jugador toca y suma puntos.
            // Cada coleccionable también otorga una carga de salto adicional.
            for (auto it = collectibles.begin(); it != collectibles.end();)
            {
                if (playerBounds.intersects(shrinkBounds(it->shape.getGlobalBounds(), 0.8f)))
                {
                    astronaut.charges += 1;
                    score += 10;
                    it = collectibles.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            // Si el jugador sale por la parte superior o inferior de la pantalla, pierde.
            if (astronaut.y <= 0.f || astronaut.y + astronaut.height >= 600.f)
            {
                gameOver = true;
                gameState = GameState::GAMEOVER;
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

        if (gameState == GameState::COVER)
        {
            if (coverTextureLoaded)
                window.draw(coverSprite);
            else
            {
                window.draw(sky);
                for (const auto &star : stars)
                    window.draw(star);
            }

            if (loadedFont)
            {
                window.draw(titleText);
                window.draw(coverText);
                window.draw(creditText);
            }

            window.display();
            continue;
        }

        if (coverTextureLoaded)
            window.draw(coverSprite);
        else
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