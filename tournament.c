#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "./tournament.h"

struct tournament_t {
    int id;
    char* location;
    Map games;
    Map players;
    int winner_id;
    int max_games_per_player;
    int total_players;
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

char* tournamentGetLocation (Tournament tournament){
    return tournament->location;
}

Map tournamentGetPlayers (Tournament tournament){
    return tournament->players;
}

int tournamentGetTotalPlayers (Tournament tournament){
    return tournament->total_players;
}

void tournamentChangeWinnerID (Tournament tournament, int winner_id){
    tournament->winner_id = winner_id;
}

void tournamentChangeTotalPlayers (Tournament tournament, int number_of_players){
    (tournament->total_players) += number_of_players;
}

MapDataElement copyTournament (MapDataElement tournament){
    RETURN_VALUE_IF_TRUE(!tournament,NULL);
    Tournament new_tournament = malloc(sizeof(*new_tournament));
    RETURN_VALUE_IF_TRUE(!new_tournament,NULL);
    new_tournament->id = ((Tournament)tournament)->id;
    char* location = malloc(sizeof(char)*(strlen(((Tournament)tournament)->location)+1));
    if(!location){
        free(new_tournament);
        return NULL;
    }
    strcpy(location, ((Tournament)tournament)->location);
    new_tournament->location = location;
    Map games = mapCopy(((Tournament)tournament)->games);
    new_tournament->games = games;
    Map players = mapCopy(((Tournament)tournament)->players);
    new_tournament->players = players;
    new_tournament->winner_id = ((Tournament)tournament)->winner_id;
    new_tournament->max_games_per_player = ((Tournament)tournament)->max_games_per_player;
    new_tournament->total_players = ((Tournament)tournament)->total_players;
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

int compareTournamentsIDs (MapKeyElement first_tournament_id, MapKeyElement second_tournament_id){
    return (*((int*)first_tournament_id))-(*((int*)second_tournament_id));
}

Tournament createTournament(int id, const char* location, int max_games_per_player){
    Tournament tournament = malloc(sizeof(*tournament));
    RETURN_VALUE_IF_TRUE(!tournament,NULL);
    tournament->id = id;
    tournament->location = malloc(sizeof(char)*(strlen(location)+1));
    if(!tournament->location){
        free(tournament);
        return NULL;
    }
    strcpy(tournament->location, location);
    tournament->max_games_per_player = max_games_per_player;
    Map games = mapCreate(copyGame, copyGameID, freeGame, freeGameID, compareGamesIDs);
    RETURN_VALUE_IF_TRUE(!games,NULL);
    tournament->games = games;
    Map players = mapCreate(copyPlayer, copyPlayerID, freePlayer, freeGameID, comparePlayersIDs);
    RETURN_VALUE_IF_TRUE(!players,NULL);
    tournament->players = players;
    tournament->winner_id = 0;
    tournament->total_players = 0;
    return tournament;
}

bool llegalTournamnetLocation(const char* tournament_location){
    RETURN_VALUE_IF_TRUE(!tournament_location,false);
    int len = strlen(tournament_location);
    RETURN_VALUE_IF_TRUE(tournament_location[0] < 'A' || tournament_location[0] > 'Z',false);
    for (int i=1; i<len; i++){
        RETURN_VALUE_IF_TRUE((tournament_location[i] < 'a' || tournament_location[i] > 'z')
                                && tournament_location[i] != ' ',false);
    }
    
    return true;
}

bool tournamentAddPlayers(Tournament tournament, int first_player,
                         int second_player, WinnerPlayer winner){
    if (!mapContains(tournament->players, (MapKeyElement)(&first_player))){
        RETURN_VALUE_IF_TRUE(!addPlayer(tournament->players, first_player),false);
        tournamentChangeTotalPlayers(tournament, 1);
    }
    if (!mapContains(tournament->players, (MapKeyElement)(&second_player))){
        RETURN_VALUE_IF_TRUE(!addPlayer(tournament->players, second_player),false);
        tournamentChangeTotalPlayers(tournament, 1);
    }
    Player first = mapGet(tournament->players, (MapKeyElement)(&first_player));
    Player second = mapGet(tournament->players, (MapDataElement)(&second_player));
    playerAddGames(first, 1);
    playerAddGames(second, 1);
    if (winner == FIRST){
        playerAddWins(first, 1);
        playerAddScore(first, 2);
        playerAddLosses(second, 1);
    }
    else {
        if (winner == SECOND){
            playerAddLosses(first, 1);
            playerAddWins(second, 1);
            playerAddScore(second, 2);
        }
        else { //draw
            playerAddDraws(first, 1);
            playerAddDraws(second, 1);
            playerAddScore(first, 1);
            playerAddScore(second, 1);
        }
    } 
    return true;
}

int calculateLongestPlayTime(Tournament tournament){
    int longest_play_time = 0;
    MAP_FOREACH (int*, game_id, tournament->games){
        Game game = mapGet(tournament->games, (MapKeyElement)game_id);
        free(game_id);
        if (gameGetPlayTime(game) > longest_play_time){
            longest_play_time = gameGetPlayTime(game);
        }
    }
    return longest_play_time;
}

void tournamentRemovePlayer(Tournament tournament, int player_id){
    mapRemove(tournament->players, (MapKeyElement)(&player_id));
}

void tournamentPlayerAddWins(Tournament tournament, int player_id, int number_of_games){
    Player player = mapGet(tournament->players, (MapKeyElement)(&player_id));
    assert(player!=NULL);
    playerAddWins(player, number_of_games);
}

void tournamentPlayerAddLosses(Tournament tournament, int player_id, int number_of_games){
    Player player = mapGet(tournament->players, (MapKeyElement)(&player_id));
    assert(player!=NULL);
    playerAddLosses(player, number_of_games);
}

void tournamentPlayerAddDraws(Tournament tournament, int player_id, int number_of_games){
    Player player = mapGet(tournament->players, (MapKeyElement)(&player_id));
    assert(player!=NULL);
    playerAddDraws(player, number_of_games);
}

void tournamentPlayerAddScore(Tournament tournament, int player_id, int number_of_games){
    Player player = mapGet(tournament->players, (MapKeyElement)(&player_id));
    assert(player!=NULL);
    playerAddScore(player, number_of_games);
}

double calculateaveragePlayTime(Tournament tournament){
    int total_games = 0;
    int total_play_time = 0;
    MAP_FOREACH (int*, game_id, tournament->games){
        Game game = mapGet(tournament->games, (MapKeyElement)game_id);
        free(game_id);
        total_games++;
        total_play_time += gameGetPlayTime(game);
    }
    return (double)total_play_time/total_games;
}


