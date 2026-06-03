#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>
#include <iostream>

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

struct Player
{
    sf::RectangleShape shape;
    AnimatedSprite animated;
    bool useSprite = false;
    float x = 150.f;
    float y = 0.f;
    float width = 64.f;
    float height = 64.f;
    float velocityY = 0.f;
    float gravity = 1500.f;
    float jumpSpeed = -780.f;
    float groundY = 520.f;
    bool onGround = true;

    Player()
        : shape(sf::Vector2f(width, height))
    {
        shape.setFillColor(sf::Color(200, 230, 255));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.f);
        y = groundY - height;
        shape.setPosition(x, y);
    }

    void reset()
    {
        x = 150.f;
        y = groundY - height;
        velocityY = 0.f;
        onGround = true;
        shape.setPosition(x, y);
        if (useSprite)
            animated.sprite.setPosition(x, y);
    }

    bool loadTexture(const std::string &path)
    {
        if (animated.load(path, 4, 64, 64))
        {
            useSprite = true;
            animated.sprite.setPosition(x, y);
            return true;
        }

        useSprite = false;
        return false;
    }

    void jump()
    {
        if (onGround)
        {
            velocityY = jumpSpeed;
            onGround = false;
        }
    }

    void update(float deltaTime)
    {
        velocityY += gravity * deltaTime;
        y += velocityY * deltaTime;

        if (y >= groundY - height)
        {
            y = groundY - height;
            velocityY = 0.f;
            onGround = true;
        }

        if (useSprite)
        {
            animated.update();
            animated.sprite.setPosition(x, y);
        }
        else
        {
            shape.setPosition(x, y);
        }
    }

    void draw(sf::RenderWindow &window)
    {
        if (useSprite)
            window.draw(animated.sprite);
        else
            window.draw(shape);
    }

    sf::FloatRect getBounds() const
    {
        if (useSprite)
            return animated.sprite.getGlobalBounds();

        return shape.getGlobalBounds();
    }
};

struct Obstacle
{
    sf::RectangleShape shape;
    float speed = 360.f;

    Obstacle(float startX, float groundY)
    {
        float width = 40.f + static_cast<float>(rand() % 40);
        float height = 50.f + static_cast<float>(rand() % 80);
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color(220, 80, 80));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.f);
        shape.setPosition(startX, groundY - height);
    }

    void update(float deltaTime)
    {
        shape.move(-speed * deltaTime, 0.f);
    }
};

struct Collectible
{
    sf::CircleShape shape;
    float speed = 360.f;

    Collectible(float startX, float groundY)
    {
        float radius = 14.f;
        shape.setRadius(radius);
        shape.setFillColor(sf::Color(240, 220, 90));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.f);
        float yPosition = groundY - 140.f - static_cast<float>(rand() % 120);
        shape.setPosition(startX, std::max(groundY - 260.f, yPosition));
    }

    void update(float deltaTime)
    {
        shape.move(-speed * deltaTime, 0.f);
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Sky Astronaut Runner");
    window.setFramerateLimit(60);

    Player astronaut;
    bool loadedAstronaut = astronaut.loadTexture("assets/images/astronaut.png");
    if (!loadedAstronaut)
    {
        loadedAstronaut = astronaut.loadTexture("assets/images/pikachu.png");
        if (!loadedAstronaut)
        {
            std::cout << "No se encontró astronaut.png, usando forma simple como reserva.\n";
        }
    }

    sf::Font font;
    bool loadedFont = font.loadFromFile("assets/fonts/Minecraft.ttf");
    if (!loadedFont)
    {
        std::cout << "No se encontró el archivo de fuente: assets/fonts/Minecraft.ttf\n";
    }

    sf::RectangleShape ground(sf::Vector2f(800.f, 80.f));
    ground.setPosition(0.f, 520.f);
    ground.setFillColor(sf::Color(30, 30, 40));

    sf::RectangleShape sky(sf::Vector2f(800.f, 520.f));
    sky.setFillColor(sf::Color(6, 18, 45));

    std::vector<sf::CircleShape> stars;
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_real_distribution<float> starX(0.f, 800.f);
    std::uniform_real_distribution<float> starY(0.f, 500.f);
    for (int i = 0; i < 100; i++)
    {
        sf::CircleShape star(static_cast<float>(1 + rand() % 3));
        star.setFillColor(sf::Color(200, 220, 255, 200));
        star.setPosition(starX(rng), starY(rng));
        stars.push_back(star);
    }

    std::vector<Obstacle> obstacles;
    std::vector<Collectible> collectibles;

    float obstacleTimer = 0.f;
    float collectibleTimer = 0.f;
    float obstacleInterval = 1.6f;
    float collectibleInterval = 2.2f;
    int score = 0;
    int highScore = 0;
    bool gameOver = false;

    sf::Clock deltaClock;

    sf::Text titleText;
    sf::Text infoText;
    sf::Text scoreText;
    sf::Text gameOverText;

    if (loadedFont)
    {
        titleText.setFont(font);
        titleText.setCharacterSize(24);
        titleText.setFillColor(sf::Color::White);
        titleText.setString("Sky Astronaut Runner");
        titleText.setPosition(16.f, 12.f);

        infoText.setFont(font);
        infoText.setCharacterSize(18);
        infoText.setFillColor(sf::Color(200, 200, 255));
        infoText.setPosition(16.f, 46.f);

        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(16.f, 520.f + 18.f);

        gameOverText.setFont(font);
        gameOverText.setCharacterSize(32);
        gameOverText.setFillColor(sf::Color(255, 180, 80));
        gameOverText.setPosition(120.f, 260.f);
        gameOverText.setString("GAME OVER\nPresiona R para reiniciar");
    }

    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space && !gameOver)
                {
                    astronaut.jump();
                }

                if (event.key.code == sf::Keyboard::R && gameOver)
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

            if (obstacleTimer >= obstacleInterval)
            {
                obstacles.emplace_back(820.f, 520.f);
                obstacleTimer = 0.f;
            }

            if (collectibleTimer >= collectibleInterval)
            {
                collectibles.emplace_back(820.f, 520.f);
                collectibleTimer = 0.f;
            }

            for (auto &obstacle : obstacles)
            {
                obstacle.update(deltaTime);
            }

            for (auto &collectible : collectibles)
            {
                collectible.update(deltaTime);
            }

            obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](const Obstacle &obs) {
                return obs.shape.getPosition().x + obs.shape.getSize().x < -20.f;
            }), obstacles.end());

            collectibles.erase(std::remove_if(collectibles.begin(), collectibles.end(), [](const Collectible &item) {
                return item.shape.getPosition().x + item.shape.getRadius() * 2.f < -20.f;
            }), collectibles.end());

            for (const auto &obstacle : obstacles)
            {
                if (astronaut.getBounds().intersects(obstacle.shape.getGlobalBounds()))
                {
                    gameOver = true;
                    highScore = std::max(highScore, score);
                    break;
                }
            }

            for (auto it = collectibles.begin(); it != collectibles.end();)
            {
                if (astronaut.getBounds().intersects(it->shape.getGlobalBounds()))
                {
                    score += 10;
                    it = collectibles.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        if (loadedFont)
        {
            std::string info = "Espacio = Saltar  |  R = Reiniciar";
            if (gameOver)
                info = "Juego terminado. Presiona R para intentar otra vez.";

            infoText.setString(info);
            scoreText.setString("Puntaje: " + std::to_string(score) + "   Mejor: " + std::to_string(highScore));
        }

        window.clear();
        window.draw(sky);

        for (const auto &star : stars)
            window.draw(star);

        window.draw(ground);

        for (const auto &collectible : collectibles)
            window.draw(collectible.shape);

        for (const auto &obstacle : obstacles)
            window.draw(obstacle.shape);

        astronaut.draw(window);

        if (loadedFont)
        {
            window.draw(titleText);
            window.draw(infoText);
            window.draw(scoreText);
            if (gameOver)
                window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}
