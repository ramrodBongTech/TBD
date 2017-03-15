// AI_Testing_Enviornment.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Game.h"
//#include "FLInput\FLInputManager.h"
#include <iostream>
#define SDL_main main

//* Set initial instance to be nullptr
//InputManager* InputManager::inputManagerInstance = nullptr;

int main()
{
	Game game(Vector2D(1280, 720), Vector2D(1280, 720), "AI Testing Enviornment");
	//auto exit = PressCommand([&]() { Game::quit = true; });
	//InputManager::GetInstance()->RegisterEventCallback(EventListener::ESCAPE, &exit, &game);

	game.init();

	//run the game loop
	LTimer capTimer;//to cap framerate
	float lastTime = 0;
	const int SCREEN_FPS = 100;
	const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

	int frameNum = 0;
	while (!Game::quit)
	{
		capTimer.start();

		//InputManager::GetInstance()->ProcessInput();

		unsigned int currentTime = LTimer::gameTime();//millis since game started
		float deltaTime = (currentTime - lastTime) / 1000.0;//time since last update

		game.loop(deltaTime);

		//save the curent time for next frame
		lastTime = currentTime;

		int frameTicks = capTimer.getTicks();//time since start of frame
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time before going to next frame
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}

	return 0;
}