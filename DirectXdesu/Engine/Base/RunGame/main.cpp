#include <stdio.h>
#include <d3dcompiler.h>
#include "KMyGame.h"

#include "Framework.h"

int main()
{
	Framework* game = new KMyGame();

	game->Run();

	delete game;

	return 0;
}