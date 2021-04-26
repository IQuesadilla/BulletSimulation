#pragma once
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>

#include <QtWidgets/QtWidgets>


#include "camera/camera.h"
#include "sdlwrapper/event.h"
#include "gamemain.h"

struct gamedata;

void keyboard_callback(SDL_Event,arg _arg);
void click_callback(SDL_Event,arg _arg);
void mouse_callback(SDL_Event,arg _arg);
void quit_callback(SDL_Event,arg _arg);

int event_callback(void *userdata, SDL_Event *event);


class eventhandler
{
public:
	eventhandler(gamedata *_gamedataptr);
	void keyboardEvent(QKeyEvent *event);

private:
	gamedata *_gamedata;
};

#endif