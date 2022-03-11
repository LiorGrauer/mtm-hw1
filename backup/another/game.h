#ifndef _GAME_H
#define _GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "chessEnums.h"
#include "player.h"

typedef struct game_t *Game;

int gameGetFirstPlayerID (Game game);

int gameGetSecondPlayerID (Game game);

Winner gameGetWinner (Game game);

int gameGetPlayTime (Game game);

void gameChangeWinner (Game game, Winner winner);

MapDataElement copyGame (MapDataElement game);

MapKeyElement copyGameID (MapKeyElement game_id);

void freeGame (MapDataElement game);

void freeGameID (MapKeyElement game_id);

int compareGamesIDs (MapKeyElement game_id1, MapKeyElement game_id2);

Game createGame(int first_player_id, int second_player_id, Winner winner, int play_time);

bool gameAlreadyExist(Map games, int first_player, int second_player);

#endif //_GAME_H