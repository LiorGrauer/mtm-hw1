#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "./map.h"
#include "./game.h"

struct game_t {
    int first_player_id;
    int second_player_id;
    Winner winner;
    int play_time;
};

int gameGetFirstPlayerID (Game game){
    return game->first_player_id;
}

int gameGetSecondPlayerID (Game game){
    return game->second_player_id;
}

Winner gameGetWinner (Game game){
    return game->winner;
}

int gameGetPlayTime (Game game){
    return game->play_time;
}

void gameChangeWinner (Game game, Winner winner){
    game->winner = winner;
}

//*** required founctions for creating games Map for tournament:
MapDataElement copyGame (MapDataElement game){
    RETURN_VALUE_IF_TRUE(!game,NULL);
    Game new_game = malloc(sizeof(*new_game));
    RETURN_VALUE_IF_TRUE(!new_game,NULL);
    new_game->first_player_id = ((Game)game)->first_player_id;
    new_game->second_player_id = ((Game)game)->second_player_id;
    new_game->winner = ((Game)game)->winner;
    new_game->play_time = ((Game)game)->play_time;
    return (MapDataElement)new_game;
}

MapKeyElement copyGameID (MapKeyElement game_id){
    RETURN_VALUE_IF_TRUE(!game_id,NULL);
    int* new_game = malloc(sizeof(int));
    RETURN_VALUE_IF_TRUE(!new_game,NULL);
    *new_game = *((int*)game_id); 
    return (MapKeyElement)new_game;
}

void freeGame (MapDataElement game){
    RETURN_VALUE_IF_TRUE(!game,);
    free((Game)game);
}

void freeGameID (MapKeyElement game_id){
    RETURN_VALUE_IF_TRUE(!game_id,);
    free((int*)game_id);
}

int compareGamesIDs (MapKeyElement game_id1, MapKeyElement game_id2){
    return (*((int*)game_id1))-(*((int*)game_id2));
}
//***

//creates new game with id's of both players, winner and length. assumes that parameters are leggal. 
Game createGame(int first_player_id, int second_player_id, Winner winner, int play_time){
    Game game = malloc(sizeof(*game));
    RETURN_VALUE_IF_TRUE(!game,NULL);
    game->first_player_id = first_player_id;
    game->second_player_id = second_player_id;
    game->winner = winner;
    game->play_time = play_time;
    return game;
}

//checks if there already is game in the tournament with same players 
bool gameAlreadyExist(Map games, int first_player, int second_player){
    MAP_FOREACH(int* , game_id, games){
        Game game = mapGet(games, (MapKeyElement)game_id);
        RETURN_VALUE_IF_TRUE((game->first_player_id == first_player && game->second_player_id == second_player) || 
        (game->first_player_id == second_player && game->second_player_id == first_player),true);
    }
    return false;
}