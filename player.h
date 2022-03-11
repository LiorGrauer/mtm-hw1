#ifndef _PLAYER_H
#define _PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "./mtm_map/map.h"
#include "./chessUtil.h"

/** Type for representing a chess player */
typedef struct player_t *Player;

/**
 * playerGetTotalGames: get the total games of the player
 *
 * @param player - player. Must be type Player
 *
 * @return
 *     int - total games of the player
 */
int playerGetTotalGames (Player player);

/**
 * playerGetTotalPlayTime: get the total play time of the player
 *
 * @param player - player. Must be type Player
 *
 * @return
 *     int - total play time of the player
 */
int playerGetTotalPlayTime (Player player);

/**
 * playerGetScore: get the score of the player
 *
 * @param player - player. Must be type Player
 *
 * @return
 *     int - score of the player
 */
int playerGetScore (Player player);

/**
 * playerGetLosses: get the number of losses of the player
 *
 * @param player - player. Must be type Player
 *
 * @return
 *     int - number of losses of the player
 */
int playerGetLosses (Player player);

/**
 * playerGetWins: get the number of wins of the player
 *
 * @param player - player. Must be type Player
 *
 * @return
 *     int - number of wins of the player
 */
int playerGetWins (Player player);

/**
 * playerAddTotalPlayTime: change the total play time of the player
 *
 * @param player - player. Must be type Player
 * @param play_time - play time to add
 */
void playerAddTotalPlayTime (Player player, int play_time);

/**
 * playerAddGames: change the number of games of the player
 *
 * @param player - player. Must be type Player
 * @param number_of_games - number of games to add
 */
void playerAddGames (Player player, int number_of_games);

/**
 * playerAddWins: change the number of wins of the player
 *
 * @param player - player. Must be type Player
 * @param number_of_wins - number of wins to add
 */
void playerAddWins (Player player, int number_of_wins);

/**
 * playerAddLosses: change the number of losses of the player
 *
 * @param player - player. Must be type Player
 * @param number_of_losses - number of losses to add
 */
void playerAddLosses (Player player, int number_of_losses);

/**
 * playerAddDraws: change the number of draws of the player
 *
 * @param player - player. Must be type Player
 * @param number_of_draws - number of draws to add
 */
void playerAddDraws (Player player, int number_of_draws);

/**
 * playerAddScore: change the score of the player
 *
 * @param player - player. Must be type Player
 * @param number_of_score - number of score to add
 */
void playerAddScore (Player player, int number_of_score);

/**
 * copyPlayer: copies player. function for creating map of players
 *
 * @param player - player. given as MapDataElement.
 *
 * @return
 *     MapDataElement - new player
 */
MapDataElement copyPlayer (MapDataElement player);

/**
 * copyPlayerID: copies player id. function for creating map of players
 *
 * @param player_id - player id to copy. given as MapKeyElement.
 *
 * @return
 *     MapDataElement - new game id
 */
MapKeyElement copyPlayerID (MapKeyElement player_id);

/**
 * freePlayer: free allocated memory for the player. function for creating map of players
 *
 * @param player - player. given as MapDataElement.
 */
void freePlayer (MapDataElement player);

/**
 * freePlayerID: free allocated memory for the game id. function for creating map of players 
 *
 * @param player_id - player id. given as MapKeyElement.
 */
void freePlayerID (MapKeyElement player_id);

/**
 * compareGamesIDs: compares two games ids. function for creating map of games
 *
 * @param first_player_id - first player id. given as MapDataElement 
 * @param second_player_id - second player id. given as MapDataElement 
 *
 * @return
 *     + positive number if first player id is bigger than second player id
 *     - negative number if second player id is bigger than first player id
 *     0 zero if the player ids are equal
 */
int comparePlayersIDs (MapKeyElement first_player_id, MapKeyElement second_player_id);

/**
 * createPlayer: creates new player with 0 at all fields
 *
 * @return
 *     Player - new player
 */
Player createPlayer();

/**
 * addPlayer: add new player to the players map
 *
 * @param players - map of players
 * @param player_id - player id to add to the map
 * 
 * @return
 *     true - if succeeded
 *     false - if memory allocation problem
 */
bool addPlayer(Map players, int player_id);

/**
 * copyPlayers: copies map of players. function for creating map of players levels
 *
 * @param players - map of players. given as MapDataElement.
 *
 * @return
 *     MapDataElement - new players
 */
MapDataElement copyPlayers (MapDataElement players);

/**
 * copyLevel: copies level. function for creating map of players levels
 *
 * @param level - level to copy. given as MapKeyElement.
 *
 * @return
 *     MapDataElement - new level
 */
MapKeyElement copyLevel (MapKeyElement level);

/**
 * freePlayers: free allocated memory for the players map. function for creating map of players levels
 *
 * @param players - map of players. given as MapDataElement.
 */
void freePlayers (MapDataElement players);

/**
 * freeLevel: free allocated memory for the level. function for creating map of players levels
 *
 * @param level - level. given as MapKeyElement.
 */
void freeLevel (MapKeyElement level);

/**
 * compareLevels: compares two levels. function for creating map of games.
 * note that this function compares in reverse order for printing players level from high to low
 *
 * @param first_level - first level. given as MapDataElement 
 * @param second_level - second level. given as MapDataElement 
 *
 * @return
 *     + positive number if second level is bigger than first level
 *     - negative number if first level is bigger than second level
 *     0 zero if the levels are equal
 */
int compareLevels (MapKeyElement first_level, MapKeyElement second_level);

/**
 * calculateLevel: calculate the level of the player according to the given.
 * 
 *
 * @param player - player. Must be type Player
 * @param total_games - total games of the player
 *
 * @return
 *     double - the player level
 */
double calculateLevel (Player player, int total_games);
#endif //_PLAYER_H