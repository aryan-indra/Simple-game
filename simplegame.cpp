#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

using namespace std;

const int width = 30;
const int height = 10;

int ballX, ballY, paddle1Y, paddle2Y;
int ballDirX, ballDirY;
bool gameOver;
char input;

// Function to detect keyboard input without blocking
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    
    return 0;
}

void Setup() {
    gameOver = false;
    ballX = width / 2;
    ballY = height / 2;
    paddle1Y = height / 2;
    paddle2Y = height / 2;
    ballDirX = -1;
    ballDirY = -1;
}

void Draw() {
    system("clear");
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#";
            if (i == ballY && j == ballX)
                cout << "O";
            else if (i >= paddle1Y && i <= paddle1Y + 3 && j == 1)
                cout << "|";
            else if (i >= paddle2Y && i <= paddle2Y + 3 && j == width - 2)
                cout << "|";
            else
                cout << " ";
            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
}

void Input() {
    if (kbhit()) {
        input = getchar();
        if (input == 'w' && paddle1Y > 0)
            paddle1Y--;
        if (input == 's' && paddle1Y + 4 < height)
            paddle1Y++;
    }
}

void Logic() {
    if (ballX == 1) {
        if (ballY >= paddle1Y && ballY <= paddle1Y + 3)
            ballDirX = 1;
        else
            gameOver = true;
    }
    if (ballX == width - 2) {
        if (ballY >= paddle2Y && ballY <= paddle2Y + 3)
            ballDirX = -1;
        else
            gameOver = true;
    }

    if (ballY == 0 || ballY == height - 1)
        ballDirY = -ballDirY;

    ballX += ballDirX;
    ballY += ballDirY;
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        usleep(100000); // delay for smooth animation
    }
    return 0;
}
