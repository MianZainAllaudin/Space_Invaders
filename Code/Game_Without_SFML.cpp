#include<iostream>
#include<windows.h>
#include<conio.h>
using namespace std;

const int SCREEN_HEIGHT = 30;
const int SCREEN_WIDTH = 70;
const int MAX_BULLETS = 5;  // Maximum number of bullets
const int MAX_ENEMIES = 5;  // Maximum number of enemies

void gotoxy(short col, short rows)
{
    COORD coordinate = { col, rows };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}

struct Bullet {
    int x, y;
    bool active;

    Bullet() : x(0), y(0), active(false) {} // this is constructor//

    void move() {
        if (active) {
            gotoxy(x, y);
            cout << ' ';
            y--;
            if (y < 1) {
                active = false;  // Mark the bullet as inactive
            }
        }
    }
};

struct Enemy {
    int x, y;
    bool active;

    Enemy() : x(0), y(0), active(false) {}

    void move() {
        if (active) {
            // Clear the entire area occupied by the enemy
            gotoxy(x - 1, y);
            cout << "   ";

            y++;
            if (y > SCREEN_HEIGHT - 2) {
                active = false;  // Mark the enemy as inactive when it goes off-screen
            }
        }
    }
};
void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}
void Displaymenu()
{
    gotoxy(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    cout << "Press 1 to play game :";
    gotoxy(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 1);
    cout << "Press 0 to exit :";
    _kbhit();
    char tell = _getch();
    switch (tell)
    {
    case 49:
        break;
    case 48:
        exit(1);
    deafult:
        cout << "\n INVALID INPUT!!!";
        exit(1);

    }
    system("cls");
}

int main()
{
    Displaymenu();
    ShowConsoleCursor(false);
    // Print the rectangle
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        gotoxy(i, 0);
        cout << "*";
        gotoxy(i, SCREEN_HEIGHT - 1);
        cout << "*";
    }

    for (int i = 1; i < SCREEN_HEIGHT - 1; i++)
    {
        gotoxy(0, i);
        cout << "*";
        gotoxy(SCREEN_WIDTH - 1, i);
        cout << "*";
    }

    int spaceshipX = SCREEN_WIDTH / 2;  // Initial X position of the spaceship
    int spaceshipY = SCREEN_HEIGHT - 5; // Initial Y position of the spaceship

    char spaceship = '^'; // Symbol for the spaceship

    Bullet bullets[MAX_BULLETS];
    Enemy enemies[MAX_ENEMIES];
    int score{ 0 };
    int speed{ 0 };
    int level{ 1 };

    while (level <= 5)
    {
        // Print the spaceship
        gotoxy(spaceshipX, spaceshipY);
        cout << spaceship;
        // priting score //
        gotoxy(SCREEN_WIDTH + 5, SCREEN_HEIGHT / 2);
        cout << "SCORE :" << score;
        gotoxy(SCREEN_WIDTH + 5, SCREEN_HEIGHT / 2 + 1);
        cout << "LEVEL :" << level;
        // Move and draw bullets
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            bullets[i].move();
        }

        // Move and draw enemies
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            enemies[i].move();
        }

        // Generate random enemies
        if (rand() % 100 < 5) {
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (!enemies[i].active) {
                    enemies[i].x = rand() % (SCREEN_WIDTH - 3) + 1; // Random X position within the screen borders
                    enemies[i].y = 1;  // Start enemies from the top
                    enemies[i].active = true;
                    break;
                }
            }
        }
        // Check for collisions between enemies and the spaceship
        for (int j = 0; j < MAX_ENEMIES; j++)
        {
            if (enemies[j].active &&
                spaceshipX >= enemies[j].x - 1 && spaceshipX < enemies[j].x + 3 &&
                spaceshipY == enemies[j].y)
            {

                system("cls");
                gotoxy(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
                cout << "Game Over! You were hit by an enemy." << endl;
                cout << "Your score is :" << score;
                exit(0);
            }
        }


        // Check for arrow key input
        if (_kbhit())
        {
            char key = _getch();

            // Erase the spaceship's current position
            gotoxy(spaceshipX, spaceshipY);
            cout << ' ';

            // Move the spaceship based on arrow keys
            switch (key)
            {
            case 72: // Up arrow
                spaceshipY--;
                if (spaceshipY == 2)
                    spaceshipY++;
                break;
            case 80: // Down arrow
                spaceshipY++;
                if (spaceshipY > SCREEN_HEIGHT - 2)
                    spaceshipY--;
                break;
            case 75: // Left arrow
                spaceshipX--;
                if (spaceshipX == 2)
                    spaceshipX++;
                break;
            case 77: // Right arrow
                spaceshipX++;
                if (spaceshipX == SCREEN_WIDTH - 2)
                    spaceshipX--;
                break;
            case 32: // Spacebar for shooting
                // Find an inactive bullet and activate it
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bullets[i].active) {
                        bullets[i].x = spaceshipX;
                        bullets[i].y = spaceshipY - 1;
                        bullets[i].active = true;
                        break;
                    }
                }
                break;
            }
        }

        // Check for collisions between bullets and enemies
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            for (int j = 0; j < MAX_ENEMIES; j++)
            {
                if (bullets[i].active && enemies[j].active &&
                    bullets[i].x >= enemies[j].x && bullets[i].x < enemies[j].x + 3 &&  // Adjust 3 based on the size of the enemy box
                    bullets[i].y == enemies[j].y)
                {
                    bullets[i].active = false;  // Deactivate the bullet
                    enemies[j].active = false;  // "Kill" the enemy
                    score += (level * 10);
                    if (speed <= 45)
                        speed += 2;
                    if (score % 50 == 0)
                    {
                        level++;
                    }
                }
            }
        }


        // Draw bullets
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (bullets[i].active) {
                gotoxy(bullets[i].x, bullets[i].y);
                cout << '|';
            }
        }

        // Draw enemies
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (enemies[i].active) {
                gotoxy(enemies[i].x, enemies[i].y);
                cout << "[]";
            }
        }

        // Pause to control the speed of the spaceship, bullets, and enemies
        Sleep(50 - speed + 1);
    }
    system("pause");
    return 0;
}

