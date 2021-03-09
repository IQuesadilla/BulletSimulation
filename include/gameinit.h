#pragma once

#include "SDL2/SDL.h"
#include "gamemain.h"
#include "class/eventhandler.h"
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"

#include "class/object.h"

struct gamedata;

int gameinit(gamedata &_gamedata, int argc, char **argv);
int gamekill();