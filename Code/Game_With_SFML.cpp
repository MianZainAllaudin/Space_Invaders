// Full Functional Game with Proper GUI using SFML 
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <windows.h>

using namespace std;

const int SCREEN_HEIGHT = 80;
const int SCREEN_WIDTH = 120;
const int MAX_BULLETS = 5;
const int MAX_ENEMIES = 5;
const int MAX_LIVES = 3;

// Structure to represent bullets
struct Bullet
{
    float x, y;
    bool active;
    bool hit;

    Bullet() : x(0), y(0), active(false), hit(false) {}

    void move()
    {
        if (active)
        {
            y -= 1.;
            if (y < 0)
            {
                active = false;
            }
        }
    }
};

// Structure to represent enemies
struct Enemy
{
    float x, y;
    bool active;
    bool hit;

    Enemy() : x(0), y(0), active(false), hit(false) {}

    void move(float& enemySpeed)
    {
        if (active)
        {
            y += enemySpeed;
            if (y > SCREEN_HEIGHT)
            {
                active = false;
            }
        }
    }
};

void showInstructions()
{
    sf::RenderWindow instructionsWindow(sf::VideoMode(1000, 600), "Instructions");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        cerr << "Error loading font\n";
        return;
    }

    // Setting background of instructions window
    sf::Texture instructionbackgroundTexture;
    if (!instructionbackgroundTexture.loadFromFile("Instruction.JPEG"))
    {
        cerr << "Error loading background texture\n";
        return;
    }

    // Adjusting the size of the background to the full screen of the instructions window
    sf::RectangleShape background(sf::Vector2f(instructionsWindow.getSize().x, instructionsWindow.getSize().y));
    background.setTexture(&instructionbackgroundTexture);

    // Draw the background first
    instructionsWindow.draw(background);

    sf::Text instructionsText(
        "Use arrow keys to move the spaceship.\n"
        "Press Space to shoot bullets.\n"
        "Avoid collisions with enemies.\n"
        "Survive as long as you can!\n\n"
        "Press Esc to return to the main menu.",
        font, 44);


    //giving colour to Text of Instruction
    instructionsText.setFillColor(sf::Color::Green);

    //Adjusting the position of text
    instructionsText.setPosition(50, 50);

    // Main loop for the instructions window
    while (instructionsWindow.isOpen())
    {
        // Event handling loop
        sf::Event event;
        while (instructionsWindow.pollEvent(event))
        {
            // Check if the window is closed
            if (event.type == sf::Event::Closed)
            {
                // Close the window if the close event is triggered
                instructionsWindow.close();
            }
            // Check if the Escape key is pressed
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                // Close the window if the Escape key is pressed
                instructionsWindow.close();
            }
        }

        // Additional code for updating and rendering can be placed here

        // Clear the window before redrawing
        instructionsWindow.clear();

        // Draw the text on top of the background
        instructionsWindow.draw(instructionsText);

        // Display the background and text
        instructionsWindow.display();
    }

}

// Function to create and run the game window
void createGameWindow()
{
    // Create SFML window
    sf::RenderWindow gameWindow(sf::VideoMode(SCREEN_WIDTH * 10, SCREEN_HEIGHT * 10), "Game");
    gameWindow.setFramerateLimit(60);

    // Load background texture
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Background.JPEG"))
    {
        cerr << "Error loading background texture\n";
        return;
    }

    // Create a rectangle for the background
    sf::RectangleShape backgroundRect(sf::Vector2f(gameWindow.getSize().x, gameWindow.getSize().y));
    backgroundRect.setTexture(&backgroundTexture);

    // Initialize player's spaceship position
    int spaceshipX = SCREEN_WIDTH / 2;
    int spaceshipY = SCREEN_HEIGHT - 11;

    // Arrays to store bullets and enemies
    Bullet bullets[MAX_BULLETS];
    Enemy enemies[MAX_ENEMIES];

    // Game variables
    int score = 0;
    int speed = 40;
    int lives = MAX_LIVES;
    int level = 1;



    // Load font for text rendering
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        cerr << "Error loading font\n";
        return;
    }

    // Create spaceship shape
    sf::RectangleShape spaceshipShape(sf::Vector2f(100, 110));
    spaceshipShape.setPosition(spaceshipX * 10 - 5, spaceshipY * 10 - 14);
    spaceshipShape.setFillColor(sf::Color::Cyan);

    // Load texture for the player's spaceship
    sf::Texture playerTexture;
    playerTexture.loadFromFile("Spaceship.JPEG");
    spaceshipShape.setTexture(&playerTexture);

    // Create bullet shape
    sf::RectangleShape bulletShape(sf::Vector2f(40, 80));
    bulletShape.setFillColor(sf::Color::Black);

    // Load texture for bullets
    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("bullet.JPEG");
    bulletShape.setTexture(&bulletTexture);

    // Create enemy shape
    sf::RectangleShape enemyShape(sf::Vector2f(60, 70));
    enemyShape.setFillColor(sf::Color::Magenta);

    // Load texture for enemies
    sf::Texture EnemyTexture;
    EnemyTexture.loadFromFile("Enemy.JPEG");
    enemyShape.setTexture(&EnemyTexture);

    // Initial enemy speed and direction
    float enemySpeed = 0.5;
    bool fireFromLeft = true;

    // Game loop
    while (gameWindow.isOpen())
    {
        sf::Event event;
        while (gameWindow.pollEvent(event))
        {
            // Handle window close event
            if (event.type == sf::Event::Closed)
            {
                gameWindow.close();
            }

            // Handle keyboard input
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    // Move spaceship based on arrow keys
                case sf::Keyboard::Up:
                    if (spaceshipY > 2)
                        spaceshipY--;
                    break;
                case sf::Keyboard::Down:
                    if (spaceshipY < SCREEN_HEIGHT - 11)
                        spaceshipY++;
                    break;
                case sf::Keyboard::Left:
                    if (spaceshipX > 2)
                        spaceshipX--;
                    spaceshipX--;
                    break;
                case sf::Keyboard::Right:
                    if (spaceshipX < SCREEN_WIDTH - 9)
                        spaceshipX++;
                    spaceshipX++;
                    break;
                    // Fire bullets on space key press
                case sf::Keyboard::Space:


                    //Iterate through each bullet in the array
                    for (int i = 0; i < MAX_BULLETS; i++)
                    {
                        // Check if the current bullet is not active (i.e., available for use)
                        if (!bullets[i].active)
                        {
                            // Set the position of the bullet based on the spaceship's position
                            bullets[i].x = fireFromLeft ? spaceshipX : spaceshipX + 6;
                            bullets[i].y = spaceshipY - 1;

                            // Activate the bullet
                            bullets[i].active = true;

                            // Mark the bullet as not hit
                            bullets[i].hit = false;

                            // Exit the loop after finding an inactive bullet to use
                            break;
                        }
                    }

                    // Toggle the direction of firing for the next shot
                    fireFromLeft = !fireFromLeft;

                    // Exit the loop, as only one bullet is fired at a time
                    break;
                }
            }
        }

        // Move bullets
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            bullets[i].move();
        }

        // Move enemies and spawn new enemies
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            enemies[i].move(enemySpeed);
            if (rand() % 100 < 5 && !enemies[i].active)
            {
                enemies[i].x = rand() % (SCREEN_WIDTH - 7) + 1;
                enemies[i].y = 1;
                enemies[i].active = true;
                enemies[i].hit = false;
            }
        }

        // Check for bullet-enemy collisions
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            for (int j = 0; j < MAX_ENEMIES; j++)
            {
                if (bullets[i].active && enemies[j].active)
                {
                    float bulletX = bullets[i].x * 10;
                    float bulletY = bullets[i].y * 10;
                    float enemyX = enemies[j].x * 10;
                    float enemyY = enemies[j].y * 10;

                    if (bulletX + 20 >= enemyX && bulletX <= enemyX + 70 &&
                        bulletY + 20 >= enemyY && bulletY <= enemyY + 30)
                    {
                        if (!bullets[i].hit && !enemies[j].hit)
                        {
                            bullets[i].hit = true;
                            enemies[j].hit = true;

                            bullets[i].active = false;
                            score += (level * 10);

                            // Increase speed based on score and level
                            if (speed < 70)
                            {
                                speed = speed + 5;
                            }

                            // Increase enemy speed based on level
                            if (score % 50 == 0 && score != 0)
                            {
                                level++;
                                if (level > 0 && level < 3)
                                {
                                    enemySpeed += 0.7;
                                }
                                else if (level > 2 && level < 5)
                                {
                                    enemySpeed += 0.7;
                                }
                                else if (level > 4)
                                {
                                    enemySpeed += 0.7;
                                }
                            }
                            if (level > 5)
                            {
                                gameWindow.close();
                                cout << "Congratulations! You have completed the game!" << endl;
                                cout << "Your final score is: " << score << endl;
                                return;
                            }
                            enemies[j].active = false;
                        }
                    }
                }
            }
        }

        // Check for spaceship-enemy collisions
        // 
        // Iterate through each enemy to check for collisions with the player's spaceship
        for (int j = 0; j < MAX_ENEMIES; j++)
        {
            // Check if the current enemy is active (i.e., visible on the screen)
            if (enemies[j].active)
            {
                // Define a bounding box for the player's spaceship
                sf::FloatRect spaceshipBounds(spaceshipX * 10, spaceshipY * 10, 50, 50);

                // Define a bounding box for the current enemy
                sf::FloatRect enemyBounds(enemies[j].x * 10, enemies[j].y * 10, 70, 30);

                // Check if the bounding boxes of the player's spaceship and the enemy intersect
                if (spaceshipBounds.intersects(enemyBounds))
                {
                    // Change the color of the player's spaceship to red upon collision
                    spaceshipShape.setFillColor(sf::Color::Red);

                    // Decrease the player's remaining lives
                    lives--;

                    // Reset spaceship position on collision
                    spaceshipX = SCREEN_WIDTH / 2;
                    spaceshipY = SCREEN_HEIGHT - 11;

                    // Game over if no lives left
                    if (lives == 0)
                    {
                        gameWindow.close();
                        cout << "Game Over! You were hit by an enemy." << endl;
                        cout << "Your final score is: " << score << endl;
                        return;
                    }
                    enemies[j].active = false;
                }
            }
        }

        // Update spaceship position
        spaceshipShape.setPosition(spaceshipX * 10, spaceshipY * 10 - 5);

        // Clear the window
        gameWindow.clear();
        // Draw the background
        gameWindow.draw(backgroundRect);

        // Draw bullets
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (bullets[i].active)
            {
                bulletShape.setPosition(bullets[i].x * 10, bullets[i].y * 10);
                gameWindow.draw(bulletShape);
            }
        }

        // Draw enemies
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            // Check if the current enemy is active
            if (enemies[i].active)
            {
                // Set the position of the enemy shape based on its coordinates
                enemyShape.setPosition(enemies[i].x * 10, enemies[i].y * 10);

                // Draw the enemy shape on the game window
                gameWindow.draw(enemyShape);
            }
        }

        // Draw spaceship
        gameWindow.draw(spaceshipShape);

        // Draw UI elements (lives, score, level)
        sf::Text livesText("Lives: " + to_string(lives), font, 50);
        livesText.setFillColor(sf::Color::White);
        livesText.setPosition(10, 10);
        gameWindow.draw(livesText);

        sf::Text scoreText("Score: " + to_string(score), font, 50);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(SCREEN_WIDTH * 10 - 250, 10);
        gameWindow.draw(scoreText);

        sf::Text levelText("Level: " + to_string(level), font, 60);
        levelText.setFillColor(sf::Color::Red);
        levelText.setPosition(SCREEN_WIDTH * 10 - 700, 10);
        gameWindow.draw(levelText);

        // Display the updated window
        gameWindow.display();

        // Introduce delay for controlling game speed
        Sleep(71 - speed);
    }

    return;
}

int main()
{
    // Create a window for the menu
    sf::RenderWindow menuWindow(sf::VideoMode(1200, 800), "Menu");

    // Load font for text rendering
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        cerr << "Error loading font\n";
        return 1;
    }

    // Playing music for the game
    sf::Music Sound;
    if (!Sound.openFromFile("battleThemeA.MP3"))
    {
        cerr << "Error loading music" << endl;
    }
    Sound.play();

    // Setting background of menu window
    sf::Texture MenubackgroundTexture;
    if (!MenubackgroundTexture.loadFromFile("MenuBackground.JPEG"))
    {
        cerr << "Error loading background texture\n";
        return 0;
    }
    // Adjusting the size of background to full screen of menu window
    sf::RectangleShape background(sf::Vector2f(menuWindow.getSize().x, menuWindow.getSize().y));
    background.setTexture(&MenubackgroundTexture);

    // Create text objects for menu options
    sf::Text TextPlay("Play", font, 60);  // creating play option in menu
    TextPlay.setPosition(550, 250);   // Adjusting the position
    TextPlay.setStyle(sf::Text::Bold); // Set text to bold

    sf::Text instructionsText("Instructions", font, 60);   // creating Instruction option in menu
    instructionsText.setPosition(500, 400);   // Adjusting the position
    instructionsText.setStyle(sf::Text::Bold); // Set text to bold

    sf::Text quitText("Quit", font, 60);   // creating Quit option in menu
    quitText.setPosition(550, 550);      // Adjusting the position
    quitText.setStyle(sf::Text::Bold); // Set text to bold

    // Text for the name of the game
    sf::Text gameNameText("Space Shooter", font, 80);
    gameNameText.setFillColor(sf::Color::White);
    gameNameText.setPosition(350, 50);
    gameNameText.setStyle(sf::Text::Bold);

    int selectedOption = 0; // Variable to track the selected option

    // Main menu loop
    while (menuWindow.isOpen())
    {
        // Handle events
        sf::Event event;
        while (menuWindow.pollEvent(event))
        {
            // Check for window close event
            if (event.type == sf::Event::Closed)
            {
                menuWindow.close();
            }
            // Check for key presses
            else if (event.type == sf::Event::KeyPressed)
            {
                // Handle arrow key presses for navigation
                if (event.key.code == sf::Keyboard::Up)
                {
                    // Handling and Moving selection up
                    selectedOption = (selectedOption - 1 + 3) % 3;
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    // Handling and Moving selection down
                    selectedOption = (selectedOption + 1) % 3;
                }
                // Handle Enter key press for selection
                else if (event.key.code == sf::Keyboard::Return)
                {
                    // Perform action based on selected option
                    if (selectedOption == 0)
                    {
                        menuWindow.close();
                        createGameWindow();
                    }
                    else if (selectedOption == 1)
                    {
                        showInstructions();
                    }
                    else if (selectedOption == 2)
                    {
                        menuWindow.close();
                    }
                }
            }
        }

        // Update the appearance of the menu items based on the selected option
        TextPlay.setFillColor(sf::Color::Red);   // setting colours to text in menu
        instructionsText.setFillColor(sf::Color::Red);  // setting colours to text in menu
        quitText.setFillColor(sf::Color::Red);   // setting colours to text in menu

        // this makes the current selected option yellow which makes it easy for the user to know at which option he is standing
        if (selectedOption == 0)
        {
            TextPlay.setFillColor(sf::Color::Yellow);
        }
        else if (selectedOption == 1)
        {
            instructionsText.setFillColor(sf::Color::Yellow);
        }
        else if (selectedOption == 2)
        {
            quitText.setFillColor(sf::Color::Yellow);
        }

        // Clear the window
        menuWindow.clear();

        // Drawing Background Window
        menuWindow.draw(background);

        // Draw menu items
        menuWindow.draw(TextPlay);
        menuWindow.draw(instructionsText);
        menuWindow.draw(quitText);

        // Draw the name of the game
        menuWindow.draw(gameNameText);

        // Display the updated window
        menuWindow.display();
    }

    return 0;
}

