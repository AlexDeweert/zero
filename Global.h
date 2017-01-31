#ifndef GLOBAL
#define GLOBAL
#define SCREENWIDTH 1024
#define SCREENHEIGHT 800
#define TILE_H 20
#define TILE_W 32
#define PLAYERHEIGHT 32
#define PLAYERWIDTH 32
#define SCALE 1.5
#define UI_OFFSET 160 //5 (tiles) times 32 (height of one tile)
enum KEYS{ UP, DOWN, LEFT, RIGHT, SPACE, N };
enum DIRECTION { NORTH, SOUTH, WEST, EAST};

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_image.h>
#include <iostream>
#include <swprintf.inl>
#include <sstream>
#include <list>

#endif 