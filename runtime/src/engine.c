#include "engine.h"
#include "game.h"

void engine_init(Engine* engine) {
	win_init(1920, 1080, "Engine", &engine->window);
	game_init();
}

void engine_run(Engine* engine) {
	while (!win_should_close(&engine->window)) {
		win_poll_events();
		game_tick();
	}
}

void engine_shutdown(Engine* engine) {
	win_shutdown(&engine->window);
	game_shutdown();
}
