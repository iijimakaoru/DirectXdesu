#include <stdio.h>
#include <d3dcompiler.h>

#include "KMyGame.h"

#include "Framework.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Framework* game = new KMyGame();

	game->Run();

	delete game;

	return 0;
}