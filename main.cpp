#include <raylib.h>
#include <iostream>

// declaring colors to use for graphics
Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

// variable to keep track of the player's score
int player_score = 0;

// variable to keep track of the computer's score
int cpu_score = 0;

// creating a Ball class
class Ball {
 public:
    // x and y coordinates of the ball
    float x, y;

    // the speed of the ball in x and y axis 
    int speed_x, speed_y;

    // the radius of the ball
    int radius;

    // function to draw the ball on the screen 
    void Draw() {
        DrawCircle(x, y, radius, Yellow);
    }

    // an update function to move the ball
    void Update() {
        x += speed_x;
        y += speed_y;

        // checking if the ball has touched the window edge to prevent it from moving outside the window
        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }

        // Computer won so increasing the computer's score
        if (x + radius >= GetScreenWidth()) {
            cpu_score++;
            ResetBall();
        }

        // player won so increasing the player's score
        if (x - radius <= 0) {
            player_score++;
            ResetBall();
        }
    }

    // reseting the ball
    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

// creating a paddle class
class Paddle {
 protected:
    
    // stopping the paddle from moving out of the game window
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

 public:

    // x and y coordinates of the paddle
    float x;
    float y;

    // the width and height of the paddle
    float width;
    float height;

    // the speed of the paddle
    int speed;

    // draw the paddle on the screen
    void Draw() {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    // update method to update the paddle's position
    void Update() {
        // check if the paddle needs to be moved up
        if (IsKeyDown(KEY_UP)) {
            y = y - speed;
        }

        // check if the paddle needs to be moved down
        if (IsKeyDown(KEY_DOWN)) {
            y = y + speed;
        }

        LimitMovement();
    }
};

// creating a cpu paddle class that iinherits from the paddle class
class CpuPaddle : public Paddle {
 public:
    // using an artifical intelligence algorithm to move the cpu paddle
    void Update(int ball_y){
        if (y + height / 2 > ball_y) {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y) {
            y = y + speed;
        }
        LimitMovement();
    }
};

// creating an instance of the ball class
Ball ball;

// creating a paddle object
Paddle player;

// creating a cpu paddle object
CpuPaddle cpu;

int main() {
    // the screen's width and height 
    const int screen_width = 1280;
    const int screen_height = 800;

    // initializing and creating the window
    InitWindow(screen_width, screen_height, "My Pong Game!");

    // how fast the game should run
    SetTargetFPS(60);

    // initializing the ball object
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    // initializing the paddle object
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    // initializing the cpu paddle object
    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    // game loop
    while (WindowShouldClose() == false) {
        // creating a blank canvas to draw the game objects onto
        BeginDrawing();

        // Updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        // Checking for collisions
        if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player.x, player.y, player.width, player.height})) {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {cpu.x, cpu.y, cpu.width, cpu.height})) {
            ball.speed_x *= -1;
        }

        // Drawing the game elements
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
        // drawing the ball
        ball.Draw();
        cpu.Draw();
        player.Draw();

        // displaying the computer's score on the screen
        DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);

        // displaying the player's score on the screen
        DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

        // to end the canvas drawing
        EndDrawing();
    }

    // closing the window
    CloseWindow();
    return 0;
}