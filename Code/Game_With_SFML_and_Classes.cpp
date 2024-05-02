#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <windows.h>

using namespace std;

const int SCREEN_HEIGHT = 80;
const int SCREEN_WIDTH = 120;
const int MAX_BULLETS = 5;
const int MAX_ENEMIES = 5;
const int MAX_LIVES = 3;

class Bullet {
public:
    float x, y;
    bool active;
    bool hit;

    Bullet() : x(0), y(0), active(false), hit(false) {}

    void move() {
        if (active) {
            y -= 1.0f;
            if (y < 0) {
                active = false;
            }
        }
    }
};

class Enemy {
public:
    float x, y;
    bool active;
    bool hit;

    Enemy() : x(0), y(0), active(false), hit(false) {}

    void move(float& enemySpeed) {
        if (active) {
            y += enemySpeed;
            if (y > SCREEN_HEIGHT) {
                active = false;
            }
        }
    }
};

class Game {
private:
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::RectangleShape backgroundRect;

    sf::Texture playerTexture;
    sf::RectangleShape spaceshipShape;

    sf::Texture bulletTexture;
    sf::RectangleShape bulletShape;

    sf::Texture enemyTexture;
    sf::RectangleShape enemyShape;

    sf::Font font;
    sf::Text livesText;
    sf::Text scoreText;
    sf::Text levelText;

    sf::Music sound;

    int spaceshipX, spaceshipY;
    int score, speed, lives, level;
    bool fireFromLeft;

    vector<Bullet> bullets;
    vector<Enemy> enemies;

public:
    Game() : window(sf::VideoMode(SCREEN_WIDTH * 10, SCREEN_HEIGHT * 10), "Game"),
        backgroundTexture(), backgroundRect(sf::Vector2f(window.getSize().x, window.getSize().y)),
        playerTexture(), spaceshipShape(sf::Vector2f(100, 110)),
        bulletTexture(), bulletShape(sf::Vector2f(40, 80)),
        enemyTexture(), enemyShape(sf::Vector2f(60, 70)),
        font(), livesText(), scoreText(), levelText(),
        sound(), spaceshipX(SCREEN_WIDTH / 2), spaceshipY(SCREEN_HEIGHT - 11),
        score(0), speed(40), lives(MAX_LIVES), level(1), fireFromLeft(true),
        bullets(MAX_BULLETS), enemies(MAX_ENEMIES) {
        initialize();
    }

    void initialize() {
        loadTextures();
        setupBackground();
        setupSpaceship();
        setupBullet();
        setupEnemy();
        setupText();
        setupMusic();
    }

    void loadTextures() {
        backgroundTexture.loadFromFile("Background.JPEG");
        playerTexture.loadFromFile("Spaceship.JPEG");
        bulletTexture.loadFromFile("bullet.JPEG");
        enemyTexture.loadFromFile("Enemy.JPEG");
    }

    void setupBackground() {
        backgroundRect.setTexture(&backgroundTexture);
    }

    void setupSpaceship() {
        spaceshipShape.setPosition(spaceshipX * 10 - 5, spaceshipY * 10 - 14);
        spaceshipShape.setFillColor(sf::Color::Cyan);
        spaceshipShape.setTexture(&playerTexture);
    }

    void setupBullet() {
        bulletShape.setFillColor(sf::Color::Black);
        bulletShape.setTexture(&bulletTexture);
    }

    void setupEnemy() {
        enemyShape.setFillColor(sf::Color::Magenta);
        enemyShape.setTexture(&enemyTexture);
    }

    void setupText() {
        font.loadFromFile("arial.ttf");

        livesText.setFont(font);
        livesText.setCharacterSize(50);
        livesText.setFillColor(sf::Color::White);
        livesText.setPosition(10, 10);

        scoreText.setFont(font);
        scoreText.setCharacterSize(50);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(SCREEN_WIDTH * 10 - 250, 10);

        levelText.setFont(font);
        levelText.setCharacterSize(60);
        levelText.setFillColor(sf::Color::Red);
        levelText.setPosition(SCREEN_WIDTH * 10 - 700, 10);
    }

    void setupMusic() {
        sound.openFromFile("battleThemeA.MP3");
        sound.play();
    }

    void handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                handleKeyPress(event.key.code);
            }
        }
    }

    void handleKeyPress(sf::Keyboard::Key key) {
        switch (key) {
        case sf::Keyboard::Up:
            if (spaceshipY > 2) spaceshipY--;
            break;
        case sf::Keyboard::Down:
            if (spaceshipY < SCREEN_HEIGHT - 11) spaceshipY++;
            break;
        case sf::Keyboard::Left:
            if (spaceshipX > 2) spaceshipX--;
            break;
        case sf::Keyboard::Right:
            if (spaceshipX < SCREEN_WIDTH - 9) spaceshipX++;
            break;
        case sf::Keyboard::Space:
            fireBullet();
            break;
        }
    }

    void fireBullet() {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) {
                bullets[i].x = fireFromLeft ? spaceshipX : spaceshipX + 6;
                bullets[i].y = spaceshipY - 1;
                bullets[i].active = true;
                bullets[i].hit = false;
                break;
            }
        }
        fireFromLeft = !fireFromLeft;
    }

    void update() {
        moveBullets();
        moveEnemies();
        checkCollisions();
        updateSpaceshipPosition();
        updateUI();
        displayWindow();
        adjustGameSpeed();
    }

    void moveBullets() {
        for (int i = 0; i < MAX_BULLETS; i++) {
            bullets[i].move();
        }
    }

    // Define enemySpeed at the beginning of the file or in an appropriate scope
    float enemySpeed = 5; // Replace 5 with the desired speed value

    void moveEnemies() {
        for (int i = 0; i < MAX_ENEMIES; i++) {
            enemies[i].move(enemySpeed);
            spawnNewEnemies(i);
        }
    }

    void spawnNewEnemies(int i) {
        if (rand() % 100 < 5 && !enemies[i].active) {
            enemies[i].x = rand() % (SCREEN_WIDTH - 7) + 1;
            enemies[i].y = 1;
            enemies[i].active = true;
            enemies[i].hit = false;
        }
    }

    void checkCollisions() {
        checkBulletEnemyCollisions();
        checkSpaceshipEnemyCollisions();
    }

    void checkBulletEnemyCollisions() {
        for (int i = 0; i < MAX_BULLETS; i++) {
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (bullets[i].active && enemies[j].active) {
                    float bulletX = bullets[i].x * 10;
                    float bulletY = bullets[i].y * 10;
                    float enemyX = enemies[j].x * 10;
                    float enemyY = enemies[j].y * 10;

                    if (bulletX + 20 >= enemyX && bulletX <= enemyX + 70 &&
                        bulletY + 20 >= enemyY && bulletY <= enemyY + 30) {
                        handleBulletEnemyCollision(i, j);
                    }
                }
            }
        }
    }

    void handleBulletEnemyCollision(int i, int j) {
        if (!bullets[i].hit && !enemies[j].hit) {
            bullets[i].hit = true;
            enemies[j].hit = true;
            bullets[i].active = false;
            score += (level * 10);
            increaseSpeedAndLevel();
            if (level > 5) {
                endGame();
            }
            enemies[j].active = false;
        }
    }

    void increaseSpeedAndLevel() {
        if (speed < 70) {
            speed += 5;
        }
        if (score % 50 == 0 && score != 0) {
            level++;
            if (level > 0 && level < 3) {
                enemySpeed += 0.7;
            }
            else if (level > 2 && level < 5) {
                enemySpeed += 0.7;
            }
            else if (level > 4) {
                enemySpeed += 0.7;
            }
        }
    }

    void endGame() {
        window.close();
        cout << "Congratulations! You have completed the game!" << endl;
        cout << "Your final score is: " << score << endl;
    }

    void checkSpaceshipEnemyCollisions() {
        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (enemies[j].active) {
                sf::FloatRect spaceshipBounds(spaceshipX * 10, spaceshipY * 10, 50, 50);
                sf::FloatRect enemyBounds(enemies[j].x * 10, enemies[j].y * 10, 70, 30);

                if (spaceshipBounds.intersects(enemyBounds)) {
                    handleSpaceshipEnemyCollision(j);
                }
            }
        }
    }

    void handleSpaceshipEnemyCollision(int j) {
        spaceshipShape.setFillColor(sf::Color::Red);
        lives--;
        resetSpaceshipPosition();
        if (lives == 0) {
            endGame();
        }
        enemies[j].active = false;
    }

    void resetSpaceshipPosition() {
        spaceshipX = SCREEN_WIDTH / 2;
        spaceshipY = SCREEN_HEIGHT - 11;
    }

    void updateSpaceshipPosition() {
        spaceshipShape.setPosition(spaceshipX * 10, spaceshipY * 10 - 5);
    }

    void updateUI() {
        livesText.setString("Lives: " + to_string(lives));
        scoreText.setString("Score: " + to_string(score));
        levelText.setString("Level: " + to_string(level));
    }

    void displayWindow() {
        window.clear();
        window.draw(backgroundRect);
        drawBullets();
        drawEnemies();
        window.draw(spaceshipShape);
        drawUI();
        window.display();
    }

    void drawBullets() {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                bulletShape.setPosition(bullets[i].x * 10, bullets[i].y * 10);
                window.draw(bulletShape);
            }
        }
    }

    void drawEnemies() {
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].active) {
                enemyShape.setPosition(enemies[i].x * 10, enemies[i].y * 10);
                window.draw(enemyShape);
            }
        }
    }

    void drawUI() {
        window.draw(livesText);
        window.draw(scoreText);
        window.draw(levelText);
    }

    void adjustGameSpeed() {
        Sleep(71 - speed);
    }

    void run() {
        while (window.isOpen()) {
            handleInput();
            update();
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
