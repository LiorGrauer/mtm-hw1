#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "./mtm_map/map.h"
#include "./chessUtil.h"
#include "game.h"

/** Type for representing a chess tournament that organizes chess games */
typedef struct tournament_t *Tournament;

/**
 * tournamentGetWinnerID: get the winner id in the tournament
 *
 * @param tournament - tournament. Must be type Tournament
 *
 * @return
 *     int - winner id
 */
int tournamentGetWinnerID (Tournament tournament);

/**
 * tournamentGetMaxGamesPerPlayer: get the allowed maximum number of games per player in the tournament
 *
 * @param tournament - tournament. Must be type Tournament
 *
 * @return
 *     int - max games per player
 */
int tournamentGetMaxGamesPerPlayer (Tournament tournament);

/**
 * tournamentGetGames: get the map of games in the tournament
 *
 * @param tournament - tournament. Must be type Tournament
 *
 * @return
 *     Map - map of the games in the tournament
 */
Map tournamentGetGames (Tournament tournament);

/**
 * tournamentGetLocation: get the location of the tournament
 *
 * @param tournament - tournament. Must be type Tournament
 *
 * @return
 *     char* - location of the tournament
 */
char* tournamentGetLocation (Tournament tournament);

/**
 * tournamentGetPlayers: get the map of players in the tournament
 *
 * @param tournament - tournament. Must be type Tournament
 *
 * @return
 *     Map - map of the players in the tournament
 */
Map tournamentGetPlayers (Tournament tournament);

/**
 * tournamentGetPlayers: get thethe number of players played in the tournament
 *
 * @param tournament - tournament. Must be type Tournament
 *
 * @return
 *     int - the number of players played in the tournament
 */
int tournamentGetTotalPlayers (Tournament tournament);

/**
 * tournamentChangeWinnerID: changes the winner id of the tournament
 *
 * @param tournament - tournament. Must be type Tournament
 */
void tournamentChangeWinnerID (Tournament tournament, int winner_id);

/**
 * tournamentChangeTotalPlayers: changes the number of players played in the tournament
 *
 * @param tournament - tournament. Must be type Tournament
 */
void tournamentChangeTotalPlayers (Tournament tournament, int number_of_players);

/**
 * copyTournament: copies tournament. function for creating map of tournaments
 *
 * @param tournament - tournament. given as MapDataElement 
 *
 * @return
 *     MapDataElement - new tournament
 */
MapDataElement copyTournament (MapDataElement tournament);

/**
 * copyTournamentID: copies tournament id. function for creating map of tournaments
 *
 * @param tournament_id - tournament id. given as MapKeyElement 
 *
 * @return
 *     MapDataElement - new tournament id
 */
MapKeyElement copyTournamentID (MapKeyElement tournament_id);

/**
 * freeTournament: free allocated memory for the tournament. function for creating map of tournaments
 *
 * @param tournament - tournament. given as MapDataElement 
 */
void freeTournament (MapDataElement tournament);

/**
 * freeTournametID: free allocated memory for the tournament id. function for creating map of tournaments
 *
 * @param tournament_id - tournament id. given as MapDataElement 
 */
void freeTournametID (MapKeyElement tournament_id);

/**
 * compareTournamentsIDs: compares two tournament ids. function for creating map of tournaments
 *
 * @param first_tournament_id - first tournament id. given as MapDataElement 
 * @param second_tournament_id - second tournament id. given as MapDataElement 
 *
 * @return
 *     + positive number if first tournament id is bigger than second tournament id
 *     - negative number if second tournament id is bigger than first tournament id
 *     0 zero if the tournaments ids are equal
 */
int compareTournamentsIDs (MapKeyElement first_tournament_id, MapKeyElement second_tournament_id);

/**
 * createTournament: creates new tournament with given parameters
 *
 * @param id - tournament id
 * @param location - location of the tournament
 * @param max_games_per_player - allowed maximum number of games per player in the tournament
 *
 * @return
 *     Tournamet - new tournament
 *     null if given location is illegal
 */
Tournament createTournament(int id, const char* location, int max_games_per_player);

/**
 * llegalTournamnetLocation: checks if given location is leggal:
 * starts with capital letter followes by small letters and spaces only
 *
 * @param tournament_location - string of location. given as const
 *
 * @return
 *     true - the location is llegal
 *     false - illegal tournament location
 */
bool llegalTournamnetLocation(const char* tournament_location);

/**
 * tournamentAddPlayers: adds the given players to the players map of the tournament.
 * updates the players statistics acoording to the winner in the game
 *
 * @param tournament - tournament. Must be type Tournament
 * @param first_player - first player id
 * @param second_player - second player id
 * @param winner - winner of the game. Must be type WinnerPlayer type
 * 
 * @return
 *     true - if succeeded
 *     false - if memory allocation problem
 */
bool tournamentAddPlayers(Tournament tournament, int first_player,
                         int second_player, WinnerPlayer winner);

/**
 * tournamentRemovePlayer: removes goven player from players map of the tournament
 *
 * @param tournament - tournament. Must be type Tournament
 * @param player_id - player id
 */
void tournamentRemovePlayer(Tournament tournament, int player_id);

/**
 * tournamentPlayerAddWins: adds wins to the total number of wins of the given player
 *
 * @param tournament - tournament. Must be type Tournament
 * @param player_id - player id
 * @param number_of_games - number of new wins
 */
void tournamentPlayerAddWins(Tournament tournament, int player_id, int number_of_games);

/**
 * tournamentPlayerAddLosses: adds losses to the total number of losses of the given player
 *
 * @param tournament - tournament. Must be type Tournament
 * @param player_id - player id
 * @param number_of_games - number of new losses
 */
void tournamentPlayerAddLosses(Tournament tournament, int player_id, int number_of_games);

/**
 * tournamentPlayerAddDraws: adds draws to the total number of draws of the given player
 *
 * @param tournament - tournament. Must be type Tournament
 * @param player_id - player id
 * @param number_of_games - number of new draws
 */
void tournamentPlayerAddDraws(Tournament tournament, int player_id, int number_of_games);

/**
 * tournamentPlayerAddScore: adds score to the total score of the given player
 *
 * @param tournament - tournament. Must be type Tournament
 * @param player_id - player id
 * @param number_of_games - number of new score
 */
void tournamentPlayerAddScore(Tournament tournament, int player_id, int number_of_games);

/**
 * calculateLongestPlayTime: returns the play time of the longest game in the tournament
 *
 * @param tournament - tournament. Must be type Tournament
 *
 * @return
 *     int - the play time of the longest game in the tournament
 */
int calculateLongestPlayTime(Tournament tournament);

/**
 * calculateaveragePlayTime: returns the average play time in the tournament
 *
 * @param tournament - tournament. Must be type Tournament
 *
 * @return
 *     double - the average play time in the tournament
 */
double calculateaveragePlayTime(Tournament tournament);

#endif //_TOURNAMENT_H