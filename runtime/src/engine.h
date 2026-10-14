#pragma once
#include "game.h"
#include "defines.h"

typedef struct engine {
} Engine;

void engine_init(Engine* engine);
void engine_shutdown(Engine* engine);
void engine_run(Engine* engine);
