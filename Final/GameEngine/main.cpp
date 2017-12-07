#include <Windows.h>

#include "ExampleGame.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int showConsole)
{	
	#if defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//This is just extra debugging for telling us if we have memory leaks (un-deleted pointers)
	#endif
		
	ExampleGame game(instance);

	if(!game.Init())
		return 0;
	
	return game.Run();
} 