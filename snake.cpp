#include "raylib.h"
#include <cmath>
#include <iostream>
#include <string>
#include <deque>
#include "Randum.hpp"

constexpr int WIDTH = 1200;
constexpr int HEIGHT = 900;

enum class GameState {
	PLAYING,
	GAME_OVER
};


struct Vec2
{
	int x, y;
	Vec2 operator+(const Vec2& other) const { return { x + other.x, y + other.y }; }
	bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
};


class Snake
{
public:
	Snake()
		:direction{ -1,0 }, score{ 0 }, girth{ 10 }
	{}

public:
	std::deque<Vec2> body{ {60, 15}, {61, 15}, {62, 15}, {63, 15}, {64, 15},
					 {65, 15}, {66, 15}, {67, 15}, {68, 15}, {69, 15} };

	Vec2 direction{  };  // Starting direction: left
	int score{ 0 };
	int girth{ 10 };

};




Vec2 generateFood(int girth) {
	return {
	    arc::randomi(1, WIDTH / girth - 1),
	    arc::randomi(1, HEIGHT / girth - 1)
	};
}

int main()
{
	InitWindow(WIDTH, HEIGHT, "Snake");
	SetTargetFPS(30);

	GameState gameState = GameState::PLAYING;

	Snake snake;
	Vec2 food = generateFood(snake.girth);
	HideCursor();
	while (!WindowShouldClose())
	{
		switch (gameState)
		{
		case GameState::PLAYING:
		{
			// Input handling
			Vec2 newDirection = snake.direction;
			if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && snake.direction.x == 0)
				newDirection = { -1, 0 };
			if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && snake.direction.x == 0)
				newDirection = { 1, 0 };
			if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && snake.direction.y == 0)
				newDirection = { 0, -1 };
			if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && snake.direction.y == 0)
				newDirection = { 0, 1 };

			snake.direction = newDirection;

			// Move snake
			Vec2 newHead = snake.body.front() + snake.direction;
			snake.body.push_front(newHead);

			// Check for food collision
			if (newHead == food)
			{
				snake.score++;
				food = generateFood(snake.girth);
				// Snake grows, so we don't remove the tail
			}
			else
			{
				// Remove tail if no food was eaten
				snake.body.pop_back();
			}

			// Check for wall collision
			if (newHead.x < 0 || newHead.x >= WIDTH / snake.girth ||
				newHead.y < 0 || newHead.y >= HEIGHT / snake.girth)
			{
				gameState = GameState::GAME_OVER;
			}

			// Check for self-collision
			for (size_t i = 1; i < snake.body.size(); ++i)
			{
				if (newHead == snake.body[i])
				{
					gameState = GameState::GAME_OVER;
					break;
				}
			}
			break;
		}
		case GameState::GAME_OVER:
		{
			if (IsKeyPressed(KEY_ENTER))
			{
				// Restart the game
				snake = Snake();
				food = generateFood(snake.girth);
				gameState = GameState::PLAYING;
			}
			break;
		}
		}

		// Rendering
		BeginDrawing();
		ClearBackground(BLACK);

		// Draw food
		DrawRectangle(food.x * snake.girth, food.y * snake.girth, snake.girth, snake.girth, WHITE);

		// Draw snake
		for (size_t i = 0; i < snake.body.size(); ++i)
		{
			Color color = (i == 0) ? (gameState == GameState::GAME_OVER ? BLUE : ORANGE) :
				(gameState == GameState::GAME_OVER ? BLACK : RED);

			DrawRectangle(snake.body[i].x * snake.girth, snake.body[i].y * snake.girth,
				snake.girth, snake.girth, color);
		}

		// Draw score
		DrawText(("SCORE: " + std::to_string(snake.score)).c_str(), 950, 10, 20, WHITE);

		// Draw border
		DrawRectangleLines(0, 0, WIDTH, HEIGHT, gameState == GameState::GAME_OVER ? RED : GREEN);

		DrawText("ArcShahi", 1050, 870, 20, RED);

		// Draw game over message and restart prompt
		if (gameState == GameState::GAME_OVER)
		{
			DrawText("GAME OVER", WIDTH / 2 - 100, HEIGHT / 2 - 30, 40, RED);
			DrawText("\t\t\tPress ENTER to restart", WIDTH / 2 - 150, HEIGHT / 2 + 20, 20, WHITE);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}