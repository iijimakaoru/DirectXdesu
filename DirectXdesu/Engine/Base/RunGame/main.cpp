#include <stdio.h>
#include <d3dcompiler.h>
#include "KMyGame.h"

#include "Framework.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	Framework* game = new KMyGame();

	game->Run();

	delete game;

	return 0;
}