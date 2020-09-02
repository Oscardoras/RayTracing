#ifndef WINDOW_H_
#define WINDOW_H_


#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>


int Window_main() {
	if(0 != SDL_Init(SDL_INIT_VIDEO)) return EXIT_FAILURE;

	SDL_Window *window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	if(window == NULL) return EXIT_FAILURE;

	SDL_Delay(3000);

	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}


#endif
