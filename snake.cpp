#include "raylib.h"
#include <cmath>
#include <iostream>
#include <string>
#include <deque>
#include "Randum.hpp"
#include <numeric>

constexpr int WIDTH = 1200;
constexpr int HEIGHT = 900;

enum class GameState {
	PLAYING,
	GAME_OVER,
	MAIN_MENU,
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
		:direction{ -1,0 }, score{ 0 }, girth{ 10 }, head{ 60,15 }
	{

		for (int i = 0; i < init_len; ++i) {
			body.push_back({ head.x + i, head.y });
		}
	}

public:

	Vec2 head;
	std::deque<Vec2> body;

	// Starting direction: left
	Vec2 direction{ };
	int score{ 0 };
	int girth{ 10 };
	int init_len = 10;

};




Vec2 generateFood(int girth) {
	return {
	    xe::randomi(1, WIDTH / girth - 1),
	    xe::randomi(1, HEIGHT / girth - 1)
	};
}

int main()
{
	InitWindow(WIDTH, HEIGHT, "Snake");
	SetTargetFPS(30);

	GameState gameState = GameState::MAIN_MENU;

	Snake snake;
	Vec2 food = generateFood(snake.girth);
	HideCursor();
	while (!WindowShouldClose())
	{
		// Input and game logic are separated by state
		switch (gameState)
		{
		case GameState::MAIN_MENU:
			if (IsKeyPressed(KEY_ENTER))
			{
				gameState = GameState::PLAYING;
			}
			break;

		case GameState::PLAYING:
		{
			// Input handling for movement
			Vec2 new_dir = snake.direction;
			if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && snake.direction.x == 0)
				new_dir = { -1, 0 };
			if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && snake.direction.x == 0)
				new_dir = { 1, 0 };
			if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && snake.direction.y == 0)
				new_dir = { 0, -1 };
			if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && snake.direction.y == 0)
				new_dir = { 0, 1 };

			snake.direction = new_dir;

			// Snake movement
			Vec2 new_head = snake.body.front() + snake.direction;
			snake.body.push_front(new_head);

			// Food collision
			if (new_head == food)
			{
				snake.score++;
				food = generateFood(snake.girth); // Generate new food
			}
			else
			{
				snake.body.pop_back(); // Remove tail if no food is eaten
			}

			// Wall collision
			if (new_head.x < 0 || new_head.x >= WIDTH / snake.girth ||
				new_head.y < 0 || new_head.y >= HEIGHT / snake.girth)
			{
				gameState = GameState::GAME_OVER;
			}

			// Self-collision
			for (size_t i = 1; i < snake.body.size(); ++i)
			{
				if (new_head == snake.body[i])
				{
					gameState = GameState::GAME_OVER;
					break;
				}
			}
			break;
		}

		case GameState::GAME_OVER:
			if (IsKeyPressed(KEY_ENTER))
			{
				// Restart game
				snake = Snake();
				food = generateFood(snake.girth);
				gameState = GameState::PLAYING;
			}
			break;
		}

		// Rendering
		BeginDrawing();
		ClearBackground(BLACK);

		switch (gameState)
		{
		case GameState::MAIN_MENU:
			DrawText("Snnaake", WIDTH / 2 - 100, HEIGHT / 2 - 50, 70, ORANGE);
			DrawText("Press RETURN to Play", WIDTH / 2, HEIGHT / 2 + 20, 20, WHITE);
			DrawText("Dev. ArcShahi", WIDTH - 200, HEIGHT - 50, 20, SKYBLUE);
			break;

		case GameState::PLAYING:
			// Draw food
			DrawRectangle(food.x * snake.girth, food.y * snake.girth, snake.girth, snake.girth, SKYBLUE);

			// Draw snake
			for (size_t i = 0; i < snake.body.size(); ++i)
			{
				Color color = (i == 0) ? ORANGE : RED; // Head is orange, body is red
				DrawRectangle(snake.body[i].x * snake.girth, snake.body[i].y * snake.girth,
					snake.girth, snake.girth, color);
			}

			// Draw score
			DrawText(("SCORE: " + std::to_string(snake.score)).c_str(), 10, 10, 20, WHITE);

			// Draw border
			DrawRectangleLines(0, 0, WIDTH, HEIGHT, GREEN);
			break;

		case GameState::GAME_OVER:
			DrawText("GAME OVER", WIDTH / 2 - 100, HEIGHT / 2 - 30, 40, RED);
			DrawText("Press ENTER to Restart", WIDTH / 2 - 120, HEIGHT / 2 + 20, 20, WHITE);
			break;
		}



		EndDrawing();
	}


	CloseWindow();
}
