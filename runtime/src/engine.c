#include "engine.h"
#include "game.h"

void engine_init(Engine* engine) {
	game_init();
}

void engine_run(Engine* engine) {
	game_tick();
}

void engine_shutdown(Engine* engine) {
	game_shutdown();
}
