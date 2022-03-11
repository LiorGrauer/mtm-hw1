#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "./player.h"

struct player_t {
    int score;
    int level;
    int total_games;
    int wins;
    int losses;
    int draws;
    int total_play_time;
};

int playerGetTotalGames (Player player){
    return player->total_games;
}

int playerGetTotalPlayTime (Player player){
    return player->total_play_time;
}

int playerGetScore (Player player){
    return player->score;
}

int playerGetLosses (Player player){
    return player->losses;
}

int playerGetWins (Player player){
    return player->wins;
}

void playerAddTotalPlayTime (Player player, int play_time){
    (player->total_play_time) += play_time;
}

void playerAddGames (Player player, int number_of_games){
    (player->total_games) += number_of_games;
}

void playerAddWins (Player player, int number_of_wins){
    (player->wins) += number_of_wins;
}

void playerAddLosses (Player player, int number_of_losses){
    (player->losses) += number_of_losses;
}

void playerAddDraws (Player player, int number_of_draws){
    (player->draws) += number_of_draws;
}

void playerAddScore (Player player, int number_of_score){
    (player->score) += number_of_score;
}

//*** required founctions for creating players Map for tournament:
MapDataElement copyPlayer (MapDataElement player){
    RETURN_VALUE_IF_TRUE(!player,NULL);
    Player new_player = malloc(sizeof(*new_player));
    RETURN_VALUE_IF_TRUE(!new_player,NULL);
    new_player->score = ((Player)player)->score;
    new_player->level = ((Player)player)->level;
    new_player->total_games = ((Player)player)->total_games;
    new_player->wins = ((Player)player)->wins;
    new_player->losses = ((Player)player)->losses;
    new_player->draws = ((Player)player)->draws;
    new_player->total_play_time = ((Player)player)->total_play_time;
    return (MapDataElement)new_player;
}

MapKeyElement copyPlayerID (MapKeyElement player_id){
    RETURN_VALUE_IF_TRUE(!player_id,NULL);
    int* new_id = malloc(sizeof(int));
    RETURN_VALUE_IF_TRUE(!new_id,NULL);
    *new_id = *((int*)player_id); 
    return (MapKeyElement)new_id;
}

void freePlayer (MapDataElement player){
    RETURN_VALUE_IF_TRUE(!player,);
    free((Player)player);
}

void freePlayerID (MapKeyElement player_id){
    RETURN_VALUE_IF_TRUE(!player_id,);
    free((int*)player_id);
}

int comparePlayersIDs (MapKeyElement first_player_id, MapKeyElement second_player_id){
    return (*((int*)player_id1))-(*((int*)player_id2));
}
//***

//creates new player 
Player createPlayer(){
    Player player = malloc(sizeof(*player));
    RETURN_VALUE_IF_TRUE(!player,NULL);
    player->score = 0;
    player->level = 0;
    player->total_games = 0;
    player->wins = 0;
    player->losses = 0;
    player->draws = 0;
    player->total_play_time = 0;
    return player;
}

void addPlayer(Map players, int player_id){
    Player player = createPlayer();
    int* player_id_temp = malloc(sizeof(*player_id_temp));
    *player_id_temp = player_id;
    mapPut(players, (MapKeyElement)player_id_temp, (MapDataElement)player);
    free(player);
    free(player_id_temp);
}


//*** required founctions for creating Map for players levels:
MapDataElement copyPlayers (MapDataElement players){
    RETURN_VALUE_IF_TRUE(!players,NULL);
    Map new_players = mapCopy((Map)players);
    RETURN_VALUE_IF_TRUE(!new_players,NULL);
    return (MapDataElement)new_players;
}

MapKeyElement copyLevel (MapKeyElement level){
    RETURN_VALUE_IF_TRUE(!level,NULL);
    double* new_level = malloc(sizeof(double));
    RETURN_VALUE_IF_TRUE(!new_level,NULL);
    *new_level = *((double*)level); 
    return (MapKeyElement)new_level;
}

void freePlayers (MapDataElement players){
    mapDestroy((Map)players);
}

void freeLevel (MapKeyElement level){
    RETURN_VALUE_IF_TRUE(!level,);
    free((double*)level);
}

int compareLevels (MapKeyElement first_level, MapKeyElement second_level){ //reverse order for printing players level from high to low
    double diff = (*(double*)second_level)-((*(double*)first_level));
    RETURN_VALUE_IF_TRUE(diff < 1e-6 && diff>-(1e-6),0);
    RETURN_VALUE_IF_TRUE(diff > 0,1);
    return -1; 
}

double calculateLevel (Player player, int total_games){
    return ((6*(player->wins)-10*(player->losses)+2*(player->draws))/(double)total_games);
}