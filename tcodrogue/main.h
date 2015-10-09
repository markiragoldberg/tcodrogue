#pragma once
#include "libtcod.hpp"
#include "render.h"
#include "mapgen.h"
#include "dataLoading.h"
#include "gameLoop.h"

// Hide the console window in release
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
