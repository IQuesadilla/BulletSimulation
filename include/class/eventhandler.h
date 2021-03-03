#pragma once
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>

#include "camera/camera.h"
#include "sdlwrapper/event.h"

struct eventdata
{
	Camera *camera;

	bool firstMouse = true;
	int lastX, lastY;
	bool quit = false;
};

void keyboard_callback(SDL_Event,arg _arg);
void click_callback(SDL_Event,arg _arg);
void mouse_callback(SDL_Event,arg _arg);
void quit_callback(SDL_Event,arg _arg);

int event_callback(void *userdata, SDL_Event *event);

#endif