#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "./chessUtil.h"
#include "./chessSystem.h"
#include "./tournament.h"
#include "./mtm_map/map.h"

struct chess_system_t {
    Map tournaments;
    Map players;
};

//converts from enum WinnerPlayer to enum  winner
static inline Winner toWinner(WinnerPlayer winner)
{
    switch(winner){
        case FIRST: return FIRST_PLAYER;
        case SECOND: return SECOND_PLAYER;
        case DRAWGAME: return DRAW;
    }
    return DRAW;                                                                                                                                                                                                      
}

ChessSystem chessCreate(){
    ChessSystem chess_system = malloc(sizeof(*chess_system));
    RETURN_VALUE_IF_TRUE(!chess_system,NULL);
    Map tournaments = mapCreate(copyTournament, copyTournamentID,
                                 freeTournament, freeTournametID, compareTournamentsIDs);
    if(!tournaments){
        free(chess_system);
        return NULL;
    }
    chess_system->tournaments = tournaments;
    Map players = mapCreate(copyPlayer, copyPlayerID, freePlayer, freeGameID, comparePlayersIDs);
    if(!players){
        free(tournaments);
        free(chess_system);
        return NULL;
    }
    chess_system->players = players;
    return chess_system;
}

void chessDestroy(ChessSystem chess){
    RETURN_VALUE_IF_TRUE(!chess,);
    mapDestroy(chess->tournaments);
    mapDestroy(chess->players);
    free(chess);
}

ChessResult chessAddTournament (ChessSystem chess, int tournament_id,
                                int max_games_per_player, const char* tournament_location){
    RETURN_VALUE_IF_TRUE(!chess || !tournament_location,CHESS_NULL_ARGUMENT);
    RETURN_VALUE_IF_TRUE(tournament_id < 1,CHESS_INVALID_ID);
    RETURN_VALUE_IF_TRUE(mapContains(chess->tournaments,
                        (MapDataElement)(&tournament_id)),CHESS_TOURNAMENT_ALREADY_EXISTS);
    RETURN_VALUE_IF_TRUE(!llegalTournamnetLocation(tournament_location),CHESS_INVALID_LOCATION);
    RETURN_VALUE_IF_TRUE(max_games_per_player < 1,CHESS_INVALID_MAX_GAMES);
    Tournament tournament = createTournament(tournament_id, tournament_location, max_games_per_player);
    MapResult result = mapPut(chess->tournaments, (MapKeyElement)(&tournament_id), (MapDataElement)tournament);
    freeTournament(tournament);
    assert(result != MAP_NULL_ARGUMENT && result != MAP_ITEM_ALREADY_EXISTS);
    RETURN_VALUE_IF_TRUE(result == MAP_OUT_OF_MEMORY,CHESS_OUT_OF_MEMORY);
    assert(result == MAP_SUCCESS);
    return CHESS_SUCCESS;
}

//adds players to chessSystem players map
//recieves map of players, players ids, the winner and the play time
//changes the players statistics in the players map acorrding to the new game.
// return true if succeeded, false in case of memory allocation problem.
static bool chessAddPlayers(Map players, int first_player, int second_player, Winner winner, int play_time){
    if (!mapContains(players, (MapKeyElement)(&first_player))){
        RETURN_VALUE_IF_TRUE(!addPlayer(players, first_player),false);
    }
    if (!mapContains(players, (MapKeyElement)(&second_player))){
        RETURN_VALUE_IF_TRUE(!addPlayer(players, second_player),false);
    }
    Player first = mapGet(players, (MapKeyElement)(&first_player));
    Player second = mapGet(players, (MapDataElement)(&second_player));
    playerAddGames(first, 1);
    playerAddGames(second, 1);
    playerAddTotalPlayTime(first, play_time);
    playerAddTotalPlayTime(second, play_time);
    if (winner == FIRST_PLAYER){
        playerAddWins(first, 1);
        playerAddScore(first, CHESS_WIN_POINTS);
        playerAddLosses(second, 1);
    }
    else {
        if (winner == SECOND_PLAYER){
            playerAddLosses(first, 1);
            playerAddWins(second, 1);
            playerAddScore(second, CHESS_WIN_POINTS);
        }
        else { //draw
            playerAddDraws(first, 1);
            playerAddDraws(second, 1);
            playerAddScore(first, CHESS_DRAW_POINTS);
            playerAddScore(second, CHESS_DRAW_POINTS);
        }
    } 
    return true;
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time){
    RETURN_VALUE_IF_TRUE(!chess,CHESS_NULL_ARGUMENT);
    RETURN_VALUE_IF_TRUE(tournament_id < 1 || first_player < 1 || second_player < 1 ||
                         first_player == second_player, CHESS_INVALID_ID);
    RETURN_VALUE_IF_TRUE(!mapContains(chess->tournaments, (MapKeyElement)(&tournament_id)),
                         CHESS_TOURNAMENT_NOT_EXIST);
    Tournament tournament = mapGet(chess->tournaments, (MapKeyElement)(&tournament_id));
    RETURN_VALUE_IF_TRUE(tournamentGetWinnerID(tournament) > 0,CHESS_TOURNAMENT_ENDED);
    Map games = tournamentGetGames(tournament);
    RETURN_VALUE_IF_TRUE(mapContains(chess->players, (MapDataElement)(&first_player)) &&
                         mapContains(chess->players, (MapDataElement)(&second_player)) && 
                         gameAlreadyExist(games, first_player, second_player),CHESS_GAME_ALREADY_EXISTS);
    RETURN_VALUE_IF_TRUE(play_time < 0,CHESS_INVALID_PLAY_TIME);
    Map players = tournamentGetPlayers(tournament);
    if (mapContains(players, (MapKeyElement)(&first_player))){
        Player first = mapGet(players, (MapKeyElement)(&first_player));
        RETURN_VALUE_IF_TRUE(playerGetTotalGames(first) ==
                             tournamentGetMaxGamesPerPlayer(tournament),CHESS_EXCEEDED_GAMES);
    }
    if (mapContains(players, (MapDataElement)(&second_player))){
        Player second = mapGet(players, (MapDataElement)(&second_player));
        RETURN_VALUE_IF_TRUE(playerGetTotalGames(second) == 
                            tournamentGetMaxGamesPerPlayer(tournament),CHESS_EXCEEDED_GAMES);
    }
    Game game = createGame(first_player, second_player, winner, play_time);
    RETURN_VALUE_IF_TRUE(!game,CHESS_OUT_OF_MEMORY);
    int* game_id = malloc(sizeof(*game_id));
    if(!game_id){
        free(game);
        return CHESS_OUT_OF_MEMORY;
    }
    *game_id = mapGetSize(games)+1;
    MapResult result = mapPut(games, (MapKeyElement)game_id, (MapDataElement)game);
    free(game_id);
    free(game);
    assert(result != MAP_NULL_ARGUMENT && result != MAP_ITEM_ALREADY_EXISTS);
    RETURN_VALUE_IF_TRUE(result == MAP_OUT_OF_MEMORY,CHESS_OUT_OF_MEMORY);
    assert(result == MAP_SUCCESS);
    RETURN_VALUE_IF_TRUE(!chessAddPlayers(chess->players, first_player,
                         second_player, winner, play_time),CHESS_OUT_OF_MEMORY);
    RETURN_VALUE_IF_TRUE(!tournamentAddPlayers(tournament, first_player,
                         second_player, winner),CHESS_OUT_OF_MEMORY);
    return CHESS_SUCCESS;
}

//updates players map after removal of a player
//recieves map of players and map of games
//changes players statistics after the player removed
static void chessUpdateStatsBeforeRemove(Map players, Map games){
    MAP_FOREACH(int*, game_id, games){
        Game game = mapGet(games, game_id);
        free(game_id);
        int first_player_id = gameGetFirstPlayerID(game);
        int second_player_id = gameGetSecondPlayerID(game);
        Winner winner = toWinner(gameGetWinner(game));
        if (mapContains(players, &first_player_id)){
            Player first = mapGet(players, &first_player_id);
            playerAddGames(first, -1);
            playerAddTotalPlayTime(first,-gameGetPlayTime(game));
            if (winner == FIRST_PLAYER){
                playerAddWins(first, -1);
                playerAddScore(first, -CHESS_WIN_POINTS);
            }
            else{
                if (winner == SECOND_PLAYER){
                    playerAddLosses(first, -1);
                }
                else{ //draw
                    playerAddDraws(first, -1);
                    playerAddScore(first, -CHESS_DRAW_POINTS);
                }
            }
        }
        if (mapContains(players, &second_player_id)){
            Player second = mapGet(players, &second_player_id);
            playerAddGames(second, -1);
            playerAddTotalPlayTime(second,-gameGetPlayTime(game));
            if (winner == SECOND_PLAYER){
                playerAddWins(second, -1);
                playerAddScore(second, -CHESS_WIN_POINTS);
            }
            else{
                if (winner == FIRST_PLAYER){
                    playerAddLosses(second, -1);
                }
                else{ //draw
                    playerAddDraws(second, -1);
                    playerAddScore(second, -CHESS_DRAW_POINTS);
                }
            }
        } 
    }
}

ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id){
    RETURN_VALUE_IF_TRUE(!chess,CHESS_NULL_ARGUMENT);
    RETURN_VALUE_IF_TRUE(tournament_id < 1,CHESS_INVALID_ID);
    RETURN_VALUE_IF_TRUE(!mapContains(chess->tournaments, (MapKeyElement)(&tournament_id)),CHESS_TOURNAMENT_NOT_EXIST);
    Tournament tournament = mapGet(chess->tournaments, (MapKeyElement)(&tournament_id));  
    chessUpdateStatsBeforeRemove(chess->players, tournamentGetGames(tournament));
    mapRemove(chess->tournaments, (MapKeyElement)(&tournament_id));
    return CHESS_SUCCESS;
}

//changes the winner in given game from given tournament to given player - after removal of the other player
static void makeWinner(Player player, int player_id, Winner new_winner,
                       WinnerPlayer new_winner_player, Game game, Tournament tournament){
    playerAddWins(player, 1);
    tournamentPlayerAddWins(tournament, player_id, 1);
    if (gameGetWinner(game) != DRAWGAME){
        playerAddLosses(player, -1);
        tournamentPlayerAddLosses(tournament, player_id, -1);
        playerAddScore(player, CHESS_WIN_POINTS);
        tournamentPlayerAddScore(tournament, player_id, CHESS_WIN_POINTS);
    }
    else { //draw
        playerAddDraws(player, -1);
        tournamentPlayerAddDraws(tournament, player_id, -1);
        playerAddScore(player, CHESS_DRAW_POINTS);
        tournamentPlayerAddScore(tournament, player_id, CHESS_DRAW_POINTS);
    }
    gameChangeWinner(game, new_winner_player);
}

//removes player from the system. for every game he played - other player gains the win
ChessResult chessRemovePlayer(ChessSystem chess, int player_id){
    RETURN_VALUE_IF_TRUE(!chess,CHESS_NULL_ARGUMENT);
    RETURN_VALUE_IF_TRUE(player_id < 1, CHESS_INVALID_ID);
    RETURN_VALUE_IF_TRUE(!mapContains(chess->players, &player_id), CHESS_PLAYER_NOT_EXIST);
    MAP_FOREACH(int*, tournament_id, chess->tournaments){
        Tournament tournament = mapGet(chess->tournaments, tournament_id);
        free(tournament_id);
        if(tournamentGetWinnerID(tournament) == 0){
            Map games = tournamentGetGames(tournament);
            MAP_FOREACH(int*, game_id, games){
                Game game = mapGet(games, game_id);
                free(game_id);
                if (player_id == gameGetFirstPlayerID(game)){
                    gameChangeFirstPlayerID(game, REMOVED_PLAYER);
                    if (toWinner(gameGetWinner(game)) != SECOND_PLAYER){
                        int second_player_id = gameGetSecondPlayerID(game);
                        if (second_player_id != REMOVED_PLAYER){
                            Player player = mapGet(chess->players, (MapKeyElement)(&second_player_id));
                            makeWinner(player, second_player_id, SECOND_PLAYER, SECOND, game, tournament);
                        }
                    }
                }
                if (player_id == gameGetSecondPlayerID(game)){
                    gameChangeSecondPlayerID(game, REMOVED_PLAYER);
                    if (toWinner(gameGetWinner(game)) != FIRST_PLAYER){
                        int first_player_id = gameGetFirstPlayerID(game);
                        if (first_player_id != REMOVED_PLAYER){
                            Player player = mapGet(chess->players, (MapKeyElement)(&first_player_id));
                            makeWinner(player, first_player_id, FIRST_PLAYER, FIRST, game, tournament);
                        }
                    }    
                }
            }
        }
    }
    mapRemove(chess->players, &player_id);
    MAP_FOREACH(int*, tournament_id, chess->tournaments){
        Tournament tournament = mapGet(chess->tournaments, tournament_id);
        free(tournament_id);
        tournamentRemovePlayer(tournament, player_id);
    }
    return CHESS_SUCCESS;
}

ChessResult chessEndTournament (ChessSystem chess, int tournament_id){
    RETURN_VALUE_IF_TRUE(!chess, CHESS_NULL_ARGUMENT);
    RETURN_VALUE_IF_TRUE(tournament_id < 1, CHESS_INVALID_ID);
    RETURN_VALUE_IF_TRUE(!mapContains(chess->tournaments, (MapKeyElement)(&tournament_id)),CHESS_TOURNAMENT_NOT_EXIST);
    Tournament tournament = mapGet(chess->tournaments, (MapKeyElement)(&tournament_id));
    RETURN_VALUE_IF_TRUE(tournamentGetWinnerID(tournament) > 0,CHESS_TOURNAMENT_ENDED);
    RETURN_VALUE_IF_TRUE(mapGetSize(tournamentGetGames(tournament)) == 0, CHESS_NO_GAMES);
    Map players = tournamentGetPlayers(tournament);
    int* winner_id = (int*)mapGetFirst(players);
    MAP_FOREACH (int*, player_id, players){
        Player player = mapGet(players, (MapKeyElement)player_id);
        Player winner = mapGet(players, (MapKeyElement)(winner_id));
        int player_score = playerGetScore(player);
        int winner_score = playerGetScore(winner);
        int player_losses = playerGetLosses(player);
        int winner_losses = playerGetLosses(winner);
        int player_wins = playerGetWins(player);
        int winner_wins = playerGetWins(winner);
        if  (player_score > winner_score || 
            (player_score == winner_score && player_losses < winner_losses) ||
            (player_score == winner_score && player_losses == winner_losses && player_wins > winner_wins) ||
            (player_score == winner_score && player_losses == winner_losses && player_wins == winner_wins &&
            *player_id < *winner_id)){
                *winner_id = *player_id;
        }
        free(player_id);
    }
    tournamentChangeWinnerID(tournament, *winner_id);
    free(winner_id);
    return CHESS_SUCCESS;
}

double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result){
    if (!chess){
        *chess_result = CHESS_NULL_ARGUMENT;
        return 0;
    }
    if (player_id < 1){
        *chess_result = CHESS_INVALID_ID;
        return 0;
    }
    if (!mapContains(chess->players, (MapKeyElement)(&player_id))){
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return 0;
    }
    *chess_result = CHESS_SUCCESS;
    Player player = mapGet(chess->players, (MapKeyElement)(&player_id));
    RETURN_VALUE_IF_TRUE(playerGetTotalGames(player) == 0,0);
    return ((double)(playerGetTotalPlayTime(player)))/playerGetTotalGames(player);
}

ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file){
    RETURN_VALUE_IF_TRUE(!chess || !file,CHESS_NULL_ARGUMENT);
    RETURN_VALUE_IF_TRUE(!chess,CHESS_SAVE_FAILURE);
    double current_level = 0;
    Map levels = mapCreate(copyPlayers, copyLevel, freePlayers, freeLevel, compareLevels);
    MAP_FOREACH(int*, player_id, chess->players){
        Player player = mapGet(chess->players, (MapKeyElement)player_id);
        int player_total_games = playerGetTotalGames(player);
        if (player_total_games > 0){
            current_level = calculateLevel(player,player_total_games);
            if (!mapContains(levels, (MapKeyElement)(&current_level))){
                Map players = mapCreate(copyPlayerID, copyPlayerID, freePlayerID, freeGameID, comparePlayersIDs);
                mapPut(players, (MapKeyElement)player_id, (MapDataElement)player_id);
                mapPut(levels, (MapKeyElement)(&current_level), (MapDataElement)players);
                mapDestroy(players);
            }
            else {
                Map players = mapGet(levels, (MapKeyElement)(&current_level));
                if (!mapContains(players, (MapKeyElement)player_id)){
                    mapPut(players, (MapKeyElement)player_id, (MapDataElement)player_id);
                }
            }
        }
        free(player_id);  
    }
    MAP_FOREACH (double*, level, levels){
        Map players = mapGet(levels, (MapKeyElement)level);
        MAP_FOREACH (int*, player_id, players){
            fprintf(file, "%d %.2f\n", *player_id, *level);
            free(player_id);
        }
        free(level);
    }
    mapDestroy(levels);
    return CHESS_SUCCESS;
}

//counts and returns the number of finished tournaments in the chessSystem
static int countFinishedTournaments (ChessSystem chess){
    int counter = 0;
    MAP_FOREACH (int*, tournament_id, chess->tournaments){
        Tournament tournament = mapGet(chess->tournaments, (MapKeyElement)tournament_id);
        free(tournament_id);
        if (tournamentGetWinnerID(tournament) > 0){
            counter++;
        }
    }
    return counter;
}

ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file){
    RETURN_VALUE_IF_TRUE(!chess,CHESS_NULL_ARGUMENT);
    RETURN_VALUE_IF_TRUE(!path_file,CHESS_NULL_ARGUMENT);
    RETURN_VALUE_IF_TRUE(countFinishedTournaments(chess) == 0,CHESS_NO_TOURNAMENTS_ENDED);
    FILE* file = fopen(path_file, "w");
    RETURN_VALUE_IF_TRUE(!file,CHESS_SAVE_FAILURE);
    MAP_FOREACH (int*, tournament_id, chess->tournaments){
        Tournament tournament = mapGet(chess->tournaments, (MapKeyElement)tournament_id);
        free(tournament_id);
        if (tournamentGetWinnerID(tournament) > 0){
            fprintf(file, "%d\n%d\n%.2f\n%s\n%d\n%d\n", tournamentGetWinnerID(tournament),
            calculateLongestPlayTime(tournament), calculateaveragePlayTime(tournament),
            tournamentGetLocation(tournament), mapGetSize(tournamentGetGames(tournament)), 
            tournamentGetTotalPlayers(tournament));
        }
    }
    fclose(file);
    return CHESS_SUCCESS;
}

