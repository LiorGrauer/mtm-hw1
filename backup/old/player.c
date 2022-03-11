#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "map.h"
#include "player.h"

struct player_t {
    int score;
    int level;
    int total_games;
    int wins;
    int losses;
    int draws;
};

int playerGetTotalGames (Player player){
    return player->total_games;
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
    if (!player){
        return NULL;
    }
    Player new_player = malloc(sizeof(*new_player));
    if (!new_player){
        return NULL;
    }
    new_player->score = ((Player)player)->score;
    new_player->level = ((Player)player)->level;
    new_player->total_games = ((Player)player)->total_games;
    new_player->wins = ((Player)player)->wins;
    new_player->losses = ((Player)player)->losses;
    new_player->draws = ((Player)player)->draws;
    return (MapDataElement)new_player;
}

MapKeyElement copyPlayerID (MapKeyElement player_id){
    if (!player_id){
        return NULL;
    }
    int* new_id = malloc(sizeof(int));
    if (!new_id){
        return NULL;
    }
    *new_id = *((int*)player_id); 
    return (MapKeyElement)new_id;
}

void freePlayer (MapDataElement player){
    if (!player){
        return;
    }
    free((Player)player);
}

void freePlayerID (MapKeyElement player_id){
    if (!player_id){
        return;
    }
    free((int*)player_id);
}

int comparePlayersIDs (MapKeyElement player_id1, MapKeyElement player_id2){
    return (*((int*)player_id1))-(*((int*)player_id2));
}
//***

//creates new player 
Player createPlayer(){
    Player player = malloc(sizeof(*player));
    player->score = 0;
    player->level = 0;
    player->total_games = 0;
    player->wins = 0;
    player->losses = 0;
    player->draws = 0;
    return player;
}

//*** required founctions for creating Map for players levels:
MapDataElement copyPlayers (MapDataElement players){
    if (!players){
        return NULL;
    }
    Map new_players = mapCopy((Map)players);
    if (!new_players){
        return NULL;
    }
    free(new_players);
    return (MapDataElement)new_players;
}

MapKeyElement copyLevel (MapKeyElement level){
    if (!level){
        return NULL;
    }
    int* new_level = malloc(sizeof(double));
    if (!new_level){
        return NULL;
    }
    *new_level = *((double*)level); 
    return (MapKeyElement)new_level;
}

void freePlayers (MapDataElement players){
    mapDestroy((Map)players);
}

void freeLevel (MapKeyElement level){
    if (!level){
        return;
    }
    free((double*)level);
}

int compareLevels (MapKeyElement level1, MapKeyElement level2){
    return (*((double*)level2))-(*((double*)level1)); //reverse order for printing players level from high to low
}
//***

int calculateLevel (Player player, int total_games){
    return (6*(player->wins)-10*(player->losses)+2*(player->draws))/total_games;
}