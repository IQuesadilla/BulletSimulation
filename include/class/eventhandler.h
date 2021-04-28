#pragma once
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QtWidgets/QtWidgets>


#include "camera/camera.h"
#include "sdlwrapper/event.h"
#include "gamemain.h"

struct gamedata;

class eventhandler
{
public:
	eventhandler(gamedata *_gamedataptr);
	void keyboardEvent(QKeyEvent *event, bool down);
	void mouseEvent(QMouseEvent *event);
	void quitEvent();

private:
	gamedata *_gamedata;
};

#endif