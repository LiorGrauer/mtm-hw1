#ifndef _GAME_H
#define _GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "./mtm_map/map.h"
#include "./chessUtil.h"
#include "player.h"

/** Type for representing a chess game */
typedef struct game_t *Game;

/**
 * gameGetFirstPlayerID: get the first player id in the game
 *
 * @param game - game. Must be type Game
 *
 * @return
 *     int - first player id
 */
int gameGetFirstPlayerID (Game game);

/**
 * gameGetSecondPlayerID: get the second player id in the game
 *
 * @param game - game. Must be type Game
 *
 * @return
 *     int - second player id
 */
int gameGetSecondPlayerID (Game game);

/**
 * gameGetWinner: get the winner of the game
 *
 * @param game - game. Must be type Game
 *
 * @return
 *     WinnerPlayer - winner of the game
 */
WinnerPlayer gameGetWinner (Game game);

/**
 * gameGetPlayTime: get the play time of the game
 *
 * @param game - game. Must be type Game
 *
 * @return
 *     int - play time
 */
int gameGetPlayTime (Game game);

/**
 * gameChangeWinner: change the winner of the game
 *
 * @param game - game. Must be type Game
 * @param winner - winner of the game. Must be type WinnerPlayer
 */
void gameChangeWinner (Game game, WinnerPlayer winner);

/**
 * gameChangeFirstPlayerID: change the id of the first player in the game
 *
 * @param game - game. Must be type Game
 * @param new_id - new player id
 */
void gameChangeFirstPlayerID(Game game, int new_id);

/**
 * gameChangeSecondPlayerID: change the id of the second player in the game
 *
 * @param game - game. Must be type Game
 * @param new_id - new player id
 */
void gameChangeSecondPlayerID(Game game, int new_id);

/**
 * copyTournament: copies game. function for creating map of games
 *
 * @param game - game. given as MapDataElement.
 *
 * @return
 *     MapDataElement - new game
 */
MapDataElement copyGame (MapDataElement game);

/**
 * copyGameID: copies game id. function for creating map of games
 *
 * @param game_id - game id to copy. given as MapKeyElement.
 *
 * @return
 *     MapDataElement - new game id
 */
MapKeyElement copyGameID (MapKeyElement game_id);

/**
 * freeGame: free allocated memory for the game. function for creating map of games
 *
 * @param game - game. given as MapDataElement.
 */
void freeGame (MapDataElement game);

/**
 * freeGameID: free allocated memory for the game id. function for creating map of games
 *
 * @param game_id - game id. given as MapKeyElement.
 */
void freeGameID (MapKeyElement game_id);

/**
 * compareGamesIDs: compares two games ids. function for creating map of games
 *
 * @param first_game_id - first game id. given as MapDataElement 
 * @param second_game_id - second game id. given as MapDataElement 
 *
 * @return
 *     + positive number if first game id is bigger than second game id
 *     - negative number if second game id is bigger than first game id
 *     0 zero if the game ids are equal
 */
int compareGamesIDs (MapKeyElement first_game_id, MapKeyElement second_game_id);

/**
 * createGame: creates new game with given parameters
 *
 * @param first_player_id - first player id
 * @param first_player_id - second player id
 * @param winner - winner of the game. Must be type WinnerPlayer
 * @param play_time - play time of the game
 *
 * @return
 *     Game - new game
 */
Game createGame(int first_player_id, int second_player_id, WinnerPlayer winner, int play_time);

/**
 * gameAlreadyExist: checks if there already game between those two players in the gives games map
 *
 * @param games - map of games
 * @param first_player - first player id
 * @param second_player - second player id
 *
 * @return
 *     true - there is already game between those players
 *     false - there is not game between those players
 */
bool gameAlreadyExist(Map games, int first_player, int second_player);

#endif //_GAME_H