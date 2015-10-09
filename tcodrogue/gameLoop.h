#pragma once

#include "libtcod.hpp"
#include "tileData.h"
#include "render.h"
#include "creatureAction.h"
#include "playerInput.h"
#include "AIDecision.h"

//Has every actor and time-sensitive object do its thing each turn, then repeats
void gameLoop();
