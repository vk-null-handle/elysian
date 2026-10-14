#pragma once
#include "core/window.h"
#include "game.h"
#include "defines.h"

typedef struct engine {
  Window window;
} Engine;

void engine_init(Engine* engine);
void engine_shutdown(Engine* engine);
void engine_run(Engine* engine);
