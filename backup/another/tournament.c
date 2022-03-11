#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "./tournament.h"
#include "./map.h"

struct tournament_t {
    int id;
    char* location;
    Map games;
    int winner_id;
    int max_games_per_player;
    Map players;
};

int tournamentGetWinnerID (Tournament tournament){
    return tournament->winner_id;
}

int tournamentGetMaxGamesPerPlayer (Tournament tournament){
    return tournament->max_games_per_player;
}

Map tournamentGetGames (Tournament tournament){
    return tournament->games;
}

Map tournamentGetPlayers (Tournament tournament){
    return tournament->players;
}

char* tournamentGetLocation (Tournament tournament){
    return tournament->location;
}

void tournamentChangeWinnerID (Tournament tournament, int winner_id){
    tournament->winner_id = winner_id;
}

//*** required founctions for creating tournaments Map for chessSystem:
MapDataElement copyTournament (MapDataElement tournament){
    RETURN_VALUE_IF_TRUE(!tournament,NULL);
    Tournament new_tournament = malloc(sizeof(*new_tournament));
    RETURN_VALUE_IF_TRUE(!new_tournament,NULL);
    new_tournament->id = ((Tournament)tournament)->id;
    char* location = malloc(sizeof(char)*(strlen(((Tournament)tournament)->location)+1));
    RETURN_VALUE_IF_TRUE(!location,NULL);
    strcpy(location, ((Tournament)tournament)->location);
    new_tournament->location = location;
    Map games = mapCopy(((Tournament)tournament)->games);
    new_tournament->games = games;
    Map players = mapCopy(((Tournament)tournament)->players);
    new_tournament->players = players;
    new_tournament->winner_id = ((Tournament)tournament)->winner_id;
    new_tournament->max_games_per_player = ((Tournament)tournament)->max_games_per_player;
    return new_tournament;
}

MapKeyElement copyTournamentID (MapKeyElement tournament_id){
    RETURN_VALUE_IF_TRUE(!tournament_id,NULL);
    int* new_id = malloc(sizeof(int));
    RETURN_VALUE_IF_TRUE(!new_id,NULL);
    *new_id = *((int*)tournament_id); 
    return (MapKeyElement)new_id;
}

void freeTournament (MapDataElement tournament){
    mapDestroy(((Tournament)tournament)->games);
    mapDestroy(((Tournament)tournament)->players);
    free(((Tournament)tournament)->location);
    free(((Tournament)tournament));
}

void freeTournametID (MapKeyElement tournament_id){
    RETURN_VALUE_IF_TRUE(!tournament_id,);
    free((int*)tournament_id);
}

int compareTournamentsIDs (MapKeyElement tournament_id1, MapKeyElement tournament_id2){
    return (*((int*)tournament_id1))-(*((int*)tournament_id2));
}
//***

//creates new tournamet with id, location given and max_games_per_player. assumes that parameters are leggal.
Tournament createTournament(int id, const char* location, int max_games_per_player){
    Tournament tournament = malloc(sizeof(*tournament));
    RETURN_VALUE_IF_TRUE(!tournament,NULL);
    tournament->id = id;
    tournament->location = malloc(sizeof(char)*(strlen(location)+1));
    RETURN_VALUE_IF_TRUE(!tournament->location,NULL);
    strcpy(tournament->location, location);
    tournament->max_games_per_player = max_games_per_player;
    Map games = mapCreate(copyGame, copyGameID, freeGame, freeGameID, compareGamesIDs);
    RETURN_VALUE_IF_TRUE(!games,NULL);
    tournament->games = games;
    Map players = mapCreate(copyPlayer, copyPlayerID, freePlayer, freeGameID, comparePlayersIDs);
    RETURN_VALUE_IF_TRUE(!players,NULL);
    tournament->players = players;
    tournament->winner_id = 0;
    return tournament;
}

/*checks if the tournament location is illegal:
 * if the name is empty or doesn't start with a capital letter (A -Z)
 *                      followed by small letters (a -z) and spaces (' ').
 */
bool illegalTournamnetLocation(const char* tournament_location){
    RETURN_VALUE_IF_TRUE(!tournament_location,false);
    int len = strlen(tournament_location);
    for (int i=0; i<len; i++){
        RETURN_VALUE_IF_TRUE((tournament_location[i] < 'a' || tournament_location[i] > 'z') && tournament_location[i] != ' ',false);
    }
    
    return true;
}

int calculateLongestPlayTime(Tournament tournament){
    int longest = 0;
    MAP_FOREACH (int*, game_id, tournament->games){
        Game game = mapGet(tournament->games, (MapKeyElement)game_id);
        if (gameGetPlayTime(game) > longest){
            longest = gameGetPlayTime(game);
        }
    }
    return longest;
}

double calculateaveragePlayTime(Tournament tournament){
    int total_games = 0;
    int total_play_time = 0;
    MAP_FOREACH (int*, game_id, tournament->games){
        Game game = mapGet(tournament->games, (MapKeyElement)game_id);
        total_games++;
        total_play_time += gameGetPlayTime(game);
    }
    return (double)total_play_time/total_games;
}


