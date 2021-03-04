#pragma once

#include "SDL.h"
#include "gamemain.h"
#include "eventhandler.h"
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"

#include "object.h"

int gameinit(gamedata &_gamedata, int argc, char **argv);
int gamekill();