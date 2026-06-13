#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <algorithm>
#include <ctime>

using namespace std;
using namespace sf;

struct AnimatedSprite
{
    Texture texture;
    Sprite sprite;
    bool loaded = false;
    int frameWidth = 0;
    int frameHeight = 0;
    int numFrames = 1;
    int currentFrame = 0;
    float frameTime = 0.12f;
    Clock frameClock;

    bool load(const string &filePath, int frames = 1, int width = 0, int height = 0)
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
            sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
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
            sprite.setTextureRect(IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
            frameClock.restart();
        }
    }
};

struct Player
{
    RectangleShape shape;
    AnimatedSprite idleAnim;
    AnimatedSprite jumpAnim;
    AnimatedSprite fallAnim;
    bool useSprite = false;
    float x = 150.f;
    float y = 0.f;
    float width = 30.f;
    float height = 91.f;
    float velocityY = 0.f;
    float gravity = 400.f; // weak gravity
    float boostSpeed = -420.f;
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
            fallAnim.sprite.setPosition(x, y);
        }
    }

    bool loadTexture(const string &path)
    {
        // Try loading three separate state sheets (idle, jump, fall)
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
        // weak gravity and vertical movement
        velocityY += gravity * deltaTime;
        y += velocityY * deltaTime;

        // gentle auto-move right up to a cap
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

    void draw(RenderWindow &window)
    {
        if (useSprite)
        {
            // determine state by vertical velocity
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

    FloatRect getBounds() const
    {
        if (useSprite)
            return idleAnim.sprite.getGlobalBounds();

        return shape.getGlobalBounds();
    }
};

struct Obstacle
{
    RectangleShape shape;
    float speed = 360.f;

    Obstacle(float startX, float startY)
    {
        float width = 64.f + static_cast<float>(rand() % 48);
        float height = width; // square-ish obstacles
        shape.setSize(Vector2f(width, height));
        shape.setFillColor(Color(220, 80, 80));
        shape.setOutlineColor(Color::White);
        shape.setOutlineThickness(2.f);
        shape.setPosition(startX, std::max(48.f, startY - height / 2.f));
    }

    void update(float deltaTime)
    {
        shape.move(-speed * deltaTime, 0.f);
    }
};

struct Collectible
{
    CircleShape shape;
    float speed = 360.f;

    Collectible(float startX, float startY)
    {
        float radius = 20.f;
        shape.setRadius(radius);
        shape.setFillColor(Color(240, 220, 90));
        shape.setOutlineColor(Color::White);
        shape.setOutlineThickness(2.f);
        shape.setPosition(startX, std::max(48.f, startY - radius));
    }

    void update(float deltaTime)
    {
        shape.move(-speed * deltaTime, 0.f);
    }
};

int main()
{
    RenderWindow window(VideoMode(800, 600), "Sky Astronaut Runner");
    window.setFramerateLimit(60);

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

    // removed ground layer per new mechanics

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