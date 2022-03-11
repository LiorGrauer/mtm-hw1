#ifndef _PLAYER_H
#define _PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "./chessEnums.h"
#include "./map.h" /////////needs to be removed!!!!!!
/////////needs to be removed!!!!!!
/////////needs to be removed!!!!!!

typedef struct player_t *Player;

int playerGetTotalGames (Player player);

int playerGetScore (Player player);

int playerGetLosses (Player player);

int playerGetWins (Player player);

void playerAddGames (Player player, int number_of_games);

void playerAddWins (Player player, int number_of_wins);

void playerAddLosses (Player player, int number_of_losses);

void playerAddDraws (Player player, int number_of_draws);

void playerAddScore (Player player, int number_of_score);

MapDataElement copyPlayer (MapDataElement player);

MapKeyElement copyPlayerID (MapKeyElement player_id);

void freePlayer (MapDataElement player);

void freePlayerID (MapKeyElement player_id);

int comparePlayersIDs (MapKeyElement player_id1, MapKeyElement player_id2);

Player createPlayer();

MapDataElement copyPlayers (MapDataElement players);

MapKeyElement copyLevel (MapKeyElement level);

void freePlayers (MapDataElement players);

void freeLevel (MapKeyElement level);

int compareLevels (MapKeyElement level1, MapKeyElement level2);

int calculateLevel (Player player, int total_games);

#endif //_PLAYER_H