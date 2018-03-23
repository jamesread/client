#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Renderer.hpp"
#include "GameState.hpp"
#include "startup.hpp"
#include "YamlNode.hpp"
#include "NetworkManager.hpp"

void mainLoop() {
	SDL_Event e;

	while (true) {
		NetworkManager::get().recvAll();

		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				case SDL_MOUSEMOTION:
				case SDL_MOUSEWHEEL:
					break;

				case SDL_WINDOWEVENT:
					SDL_GetWindowSize(Renderer::get().getWindow(), &Renderer::get().window_w, &Renderer::get().window_h);
					cout << "window changed" << endl;
					break;
				case SDL_QUIT:
					return;
				case SDL_CONTROLLERAXISMOTION:
					//handleControllerAxis(e.value);
					break;
				case SDL_KEYDOWN:
					handleKeypress(e.key);
					break;
				case SDL_KEYUP: break; // ignore
				case SDL_AUDIODEVICEADDED:
					std::cout << "Audio device found" << std::endl;
					break;
				default: 
					std::cout << "Unknown event: 0x" << hex << e.type << std::endl;
			}
		}

		Renderer::get().renderFrame();

		SDL_Delay(100);
	}
}

void startGame() {
	GameState::get().ui->state = PLAY;
}

void quitGame() {
	std::cout << "quitGame()" << endl;

	SDL_Event e;
	e.type = SDL_QUIT;

	SDL_PushEvent(&e);
}

void initLibraries() {
	srand(time(NULL));

	SDL_Init(SDL_INIT_EVERYTHING);

	initControllerJoysticks();

	initFreetype();
}

int main(int argc, char* argv[]) {
	initLibraries();

	auto world = new YamlNode();
	world->attr("shape", "flat");

	auto people = world->child("people");
	people->attr("type", "peopleList");

	auto james = people->child("James");
	james->attr("age", "30");
	james->attr("color", "Blue");
	
	auto jamessStatus = james->child("status");
	jamessStatus->attr("health", 100);
	jamessStatus->attr("energy", 3);

//	auto jamessItems = james->item();
	//jamessItems->add("type", "sword");
//	jamessItems->add("type", "axe");

	auto evie = people->child("Evie");
	evie->attr("age", "28");
	evie->attr("color", "Pink");

	std::cout << world->toString() << endl;

	SDL_Window *win = SDL_CreateWindow("Greyvar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 512, SDL_WINDOW_SHOWN);
	SDL_SetWindowResizable(win, SDL_TRUE);

	Renderer::set(win, SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
//	Renderer::get().resCache->loadStartup();

	GameState::get().loadWorld("default");

	mainLoop();

	SDL_Quit();

	return 0;
}

