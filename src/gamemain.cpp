#include "gamemain.h"

gamedata::gamedata(int argc, char **argv)
{
	gameinit(*this,argc,argv);
}

gamedata::~gamedata()
{
	gamekill();
}

int main(int argc, char **argv)
{
	gamedata _gamedata (argc,argv);
	gameloop(_gamedata);
	return 1;
}