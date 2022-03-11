#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "chessEnums.h"
#include "game.h"

typedef struct tournament_t *Tournament;

int tournamentGetWinnerID (Tournament tournament);

int tournamentGetMaxGamesPerPlayer (Tournament tournament);

Map tournamentGetGames (Tournament tournament);

Map tournamentGetPlayers (Tournament tournament);

char* tournamentGetLocation (Tournament tournament);

void tournamentChangeWinnerID (Tournament tournament, int winner_id);

MapDataElement copyTournament (MapDataElement tournament);

MapKeyElement copyTournamentID (MapKeyElement tournament_id);

void freeTournament (MapDataElement tournament);

void freeTournametID (MapKeyElement tournament_id);

int compareTournamentsIDs (MapKeyElement tournament_id1, MapKeyElement tournament_id2);

Tournament createTournament(int id, const char* location, int max_games_per_player);

bool illegalTournamnetLocation(const char* tournament_location);

int calculateLongestPlayTime(Tournament tournament);

double calculateaveragePlayTime(Tournament tournament);

#endif //_TOURNAMENT_H