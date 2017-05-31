#include "GameWorld/GameWorld.h"

int main(int argc, char* argv[])
{
	GameWorld game_world;
	game_world.SetTitle("SimFI: Steering and Constrained Behavior of Autonomous Agents");

	if(game_world.Init(800, 600) == false)
	{
		return 0;
	}
	game_world.Start();

	return 0;
}