#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "./chessSystem.h"
#include "./tournament.h"
#include "./map.h"


//#ifdef NDEBUG
//#define assert(x) ((void)0)
//#else /* debugging enabled */
//void _assert (const char* condition, const char* filename, int line);
//#define assert(e) ((e) ? (void)0 : _assert(#e, __FILE__, __LINE__))
//#endif /* NDEBUG */

struct chess_system_t {
    Map tournaments;
};

ChessSystem chessCreate(){
    ChessSystem chess_system = malloc(sizeof(*chess_system));
    RETURN_VALUE_IF_TRUE(!chess_system,NULL);
    Map tournaments = mapCreate(copyTournament, copyTournamentID, freeTournament, freeTournametID, compareTournamentsIDs);
    RETURN_VALUE_IF_TRUE(!tournaments,NULL);
    chess_system->tournaments = tournaments;
    return chess_system;
}

void chessDestroy(ChessSystem chess){
    if (!chess){
        return;
    }
    mapDestroy(chess->tournaments);
    free(chess);
}

ChessResult chessAddTournament (ChessSystem chess, int tournament_id,
                                int max_games_per_player, const char* tournament_location){
    RETURN_VALUE_IF_TRUE(!chess || !tournament_location,CHESS_NULL_ARGUMENT);
    RETURN_VALUE_IF_TRUE(tournament_id < 1,CHESS_INVALID_ID);
    RETURN_VALUE_IF_TRUE(mapContains(chess->tournaments, (MapDataElement)(&tournament_id)),CHESS_TOURNAMENT_ALREADY_EXISTS);
    RETURN_VALUE_IF_TRUE(illegalTournamnetLocation(tournament_location),CHESS_INVALID_LOCATION);
    RETURN_VALUE_IF_TRUE(max_games_per_player < 1,CHESS_INVALID_MAX_GAMES);
    Tournament tournament = createTournament(tournament_id, tournament_location, max_games_per_player);
    int* id = &tournament_id;
    MapResult result = mapPut(chess->tournaments, (MapKeyElement)id, (MapDataElement)tournament);
    freeTournament(tournament);
    assert(result != MAP_NULL_ARGUMENT && result != MAP_ITEM_ALREADY_EXISTS);
    RETURN_VALUE_IF_TRUE(result == MAP_OUT_OF_MEMORY,CHESS_OUT_OF_MEMORY);
    assert(result == MAP_SUCCESS);
    return CHESS_SUCCESS;
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time){
    RETURN_VALUE_IF_TRUE(!chess,CHESS_NULL_ARGUMENT);
    RETURN_VALUE_IF_TRUE(tournament_id < 1 || first_player < 1 || second_player < 1 || first_player == second_player, CHESS_INVALID_ID);
    RETURN_VALUE_IF_TRUE(!mapContains(chess->tournaments, (MapKeyElement)(&tournament_id)), CHESS_TOURNAMENT_NOT_EXIST);
    Tournament tournament = mapGet(chess->tournaments, (MapKeyElement)(&tournament_id));
    RETURN_VALUE_IF_TRUE(tournamentGetWinnerID(tournament) > 0,CHESS_TOURNAMENT_ENDED);
    Map games = tournamentGetGames(tournament);
    RETURN_VALUE_IF_TRUE(gameAlreadyExist(games, first_player, second_player),CHESS_GAME_ALREADY_EXISTS);
    RETURN_VALUE_IF_TRUE(play_time < 0,CHESS_INVALID_PLAY_TIME);
    Map players = tournamentGetPlayers(tournament);
    if (mapContains(players, (MapKeyElement)(&first_player))){
        Player first = mapGet(players, (MapKeyElement)(&first_player));
        RETURN_VALUE_IF_TRUE(playerGetTotalGames(first) == tournamentGetMaxGamesPerPlayer(tournament),CHESS_EXCEEDED_GAMES);
    }
    else {
        Player first = createPlayer();
        int* first_player_temp = malloc(sizeof(*first_player_temp));
        *first_player_temp = first_player;
        mapPut(players, (MapKeyElement)(&first_player), (MapDataElement)first);
        free(first);
        free(first_player_temp);
    }
    if (mapContains(players, (MapDataElement)(&second_player))){
        Player second = mapGet(players, (MapDataElement)(&second_player));
        RETURN_VALUE_IF_TRUE(playerGetTotalGames(second) == tournamentGetMaxGamesPerPlayer(tournament),CHESS_EXCEEDED_GAMES);
    }
    else {
        Player second = createPlayer();
        int* second_player_temp = malloc(sizeof(*second_player_temp));
        *second_player_temp = second_player;
        mapPut(players, (MapKeyElement)second_player_temp, (MapDataElement)second);
        free(second);
        free(second_player_temp);
    }
    Game game = createGame(first_player, second_player, winner, play_time);
    int* game_id = malloc(sizeof(*game_id));
    *game_id = mapGetSize(games)+1;
    MapResult result = mapPut(games, (MapKeyElement)game_id, (MapDataElement)game);
    free(game_id);
    free(game);
    assert(result != MAP_NULL_ARGUMENT && result != MAP_ITEM_ALREADY_EXISTS);
    RETURN_VALUE_IF_TRUE(result == MAP_OUT_OF_MEMORY,CHESS_OUT_OF_MEMORY);
    assert(result == MAP_SUCCESS);
    Player first = mapGet(players, (MapKeyElement)(&first_player));
    Player second = mapGet(players, (MapDataElement)(&second_player));
    playerAddGames(first, 1);
    playerAddGames(second, 1);
    if (winner == FIRST_PLAYER){
        playerAddWins(first, 1);
        playerAddScore(first, 2);
        playerAddLosses(second, 1);
    }
    else {
        if (winner == SECOND_PLAYER){
            playerAddLosses(first, 1);
            playerAddWins(second, 1);
            playerAddScore(second, 2);
        }
        else {
            playerAddDraws(first, 1);
            playerAddDraws(second, 1);
            playerAddScore(first, 1);
            playerAddScore(second, 1);
        }
    }
    return CHESS_SUCCESS;
}

ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id){
    RETURN_VALUE_IF_TRUE(!chess,CHESS_NULL_ARGUMENT);
    RETURN_VALUE_IF_TRUE(tournament_id < 1,CHESS_INVALID_ID);
    MapResult result = mapRemove(chess->tournaments, (MapKeyElement)(&tournament_id));
    RETURN_VALUE_IF_TRUE(result==MAP_ITEM_DOES_NOT_EXIST,CHESS_TOURNAMENT_NOT_EXIST);
    return CHESS_SUCCESS;
}

bool playerExists (Map tournaments, int player_id){
    bool player_exists = false;
    MAP_FOREACH(int*, tournament_id, tournaments){ //iterates all the tournament in the chess system
        Tournament tournament = mapGet(tournaments, (MapKeyElement)tournament_id);
        if (mapContains(tournamentGetPlayers(tournament), (MapKeyElement)(&player_id))){
            player_exists = true;
        }
    }
    return player_exists;
}

//removes player from the system. for every game he played - other player gains the win
// if player exists - returns true and false otherwise
void removePlayer(Map tournaments, int player_id){
    MAP_FOREACH(int*, tournament_id, tournaments){ //iterates all the tournament in the chess system
        Tournament tournament = mapGet(tournaments, (MapKeyElement)tournament_id);
        Map players = tournamentGetPlayers(tournament);
        free(tournament_id);
        if (tournamentGetWinnerID(tournament) == 0) { //tournament active
            if (mapContains(players, (MapKeyElement)(&player_id))){
                Map games = tournamentGetGames(tournament);
                MAP_FOREACH(int*, game_id, games){
                    Game game = mapGet(games, (MapKeyElement)game_id);
                    free(game_id);
                    if (player_id == gameGetFirstPlayerID(game)){
                        if (gameGetWinner(game) != SECOND_PLAYER){
                            int second_player_id = gameGetSecondPlayerID(game);
                            Player player = mapGet(players, (MapKeyElement)(&second_player_id));
                            playerAddWins(player, 1);
                            if (gameGetWinner(game) == FIRST_PLAYER){
                                playerAddLosses(player, -1);
                                playerAddScore(player, 2);
                            }
                            else { //draw
                                playerAddDraws(player, -1);
                                playerAddScore(player, 1);
                            }
                            gameChangeWinner(game, SECOND_PLAYER);
                        }
                    }
                    if (player_id == gameGetSecondPlayerID(game)){
                        if (gameGetWinner(game) != FIRST_PLAYER){
                            int first_player_id = gameGetFirstPlayerID(game);
                            Player player = mapGet(players, (MapKeyElement)(&first_player_id));
                            playerAddWins(player, 1);
                            if (gameGetWinner(game) == SECOND_PLAYER){
                                playerAddLosses(player, -1);
                                playerAddScore(player, 2);
                            }
                            else { //draw
                                playerAddDraws(player, -1);
                                playerAddScore(player, 1); 
                            }
                            gameChangeWinner(game, FIRST_PLAYER);
                        }    
                    }
                }
                mapRemove(players, (MapKeyElement)(&player_id));    
            }
        }
    }
}

ChessResult chessRemovePlayer(ChessSystem chess, int player_id){
    RETURN_VALUE_IF_TRUE((!chess),CHESS_NULL_ARGUMENT);
    RETURN_VALUE_IF_TRUE((player_id < 1),CHESS_INVALID_ID);
    RETURN_VALUE_IF_TRUE((!chess->tournaments),CHESS_PLAYER_NOT_EXIST);
    RETURN_VALUE_IF_TRUE((!playerExists(chess->tournaments, player_id)),CHESS_PLAYER_NOT_EXIST);
    removePlayer(chess->tournaments, player_id);
    return CHESS_SUCCESS;
}

ChessResult chessEndTournament (ChessSystem chess, int tournament_id){
    RETURN_VALUE_IF_TRUE(!chess, CHESS_NULL_ARGUMENT);
    RETURN_VALUE_IF_TRUE(tournament_id < 1, CHESS_INVALID_ID);
    RETURN_VALUE_IF_TRUE(!mapContains(chess->tournaments, (MapKeyElement)(&tournament_id)),CHESS_TOURNAMENT_NOT_EXIST);
    Tournament tournament = mapGet(chess->tournaments, (MapKeyElement)(&tournament_id));
    RETURN_VALUE_IF_TRUE(tournamentGetWinnerID(tournament) > 0,CHESS_TOURNAMENT_ENDED);
    Map players = tournamentGetPlayers(tournament);

    int winner_id = (*(int*)mapGetFirst(players));
    MAP_FOREACH (int*, player_id, players){
        Player player = mapGet(players, (MapKeyElement)player_id);
        Player winner = mapGet(players, (MapKeyElement)(&winner_id));
        int player_score = playerGetScore(player);
        int winner_score = playerGetScore(winner);
        int player_losses = playerGetLosses(player);
        int winner_losses = playerGetLosses(winner);
        int player_wins = playerGetWins(player);
        int winner_wins = playerGetWins(winner);
        if (player_score > winner_score || 
        (player_score == winner_score && player_losses < winner_losses) ||
        (player_score == winner_score && player_losses == winner_losses && player_wins > winner_wins) ||
        (player_score == winner_score && player_losses == winner_losses && player_wins == winner_wins && *player_id < winner_id)){
            winner_id = *player_id;
        }
    }
    tournamentChangeWinnerID(tournament, winner_id);
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
    if (!playerExists(chess->tournaments, player_id)){
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return 0;
    }
    *chess_result = CHESS_SUCCESS;
    int total_game_play = 0;
    int total_games_played = 0;
    MAP_FOREACH (int*, tournament_id, chess->tournaments){ //iterates all tournaments and games for calculating total play time for given player
        Tournament tournament = mapGet(chess->tournaments, (MapKeyElement)tournament_id);
        if (mapContains(tournamentGetPlayers(tournament), (MapKeyElement)(&player_id))){
            MAP_FOREACH (int*, game_id, tournamentGetGames(tournament)){
                Game game = mapGet(tournamentGetGames(tournament), (MapKeyElement)game_id);
                if (gameGetFirstPlayerID(game) == player_id || gameGetSecondPlayerID(game) == player_id){
                    total_game_play += gameGetPlayTime(game);
                }
            }
            Player player = mapGet(tournamentGetPlayers(tournament), (MapKeyElement)(&player_id));
            total_games_played += playerGetTotalGames(player);
        }
    }
    return ((double)(total_game_play))/total_games_played;
}

int countGames (ChessSystem chess){
    int total_games = 0;
    MAP_FOREACH (int*, tournament_id, chess->tournaments){
        Tournament tournament = mapGet(chess->tournaments, (MapKeyElement)tournament_id);
        total_games += mapGetSize(tournamentGetGames(tournament));
    }
    return total_games;
}

ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file){
    RETURN_VALUE_IF_TRUE(!chess,CHESS_NULL_ARGUMENT);
    double curr_level=0;
    Map levels = mapCreate(copyPlayers, copyLevel, freePlayers, freeLevel, compareLevels);
    MAP_FOREACH (int*, tournament_id, chess->tournaments){ //iterates all tournaments and games for adding players to the map
        Tournament tournament = mapGet(chess->tournaments, (MapKeyElement)tournament_id);
        MAP_FOREACH (int*, player_id, tournamentGetPlayers(tournament)){
            Player player = mapGet(tournamentGetPlayers(tournament), (MapKeyElement)player_id);
            curr_level = calculateLevel(player);
            if (!mapContains(levels, (MapKeyElement)(&curr_level))){
                Map players = mapCreate(copyPlayerID, copyPlayerID, freePlayerID, freeGameID, comparePlayersIDs);
                double* curr_level_temp = &curr_level;
                mapPut(players, (MapKeyElement)player_id, (MapDataElement)player_id);
                mapPut(levels, (MapKeyElement)curr_level_temp, (MapDataElement)players);
                mapDestroy(players);
            }
            else{
                Map players = mapGet(levels, (MapKeyElement)(&curr_level));
                if (!mapContains(players, (MapKeyElement)player_id)){
                    mapPut(players, (MapKeyElement)player_id, (MapDataElement)player_id);
                }
            }           
        }
    }
    MAP_FOREACH (double*, level, levels){
        Map players = mapGet(levels, (MapKeyElement)level);
        MAP_FOREACH (int*, player_id, players){
            fprintf(file, "%d %.2f\n", *player_id, *level);
        }
    }
    mapDestroy(levels);
    return CHESS_SUCCESS;
}

int countFinishedTournaments (ChessSystem chess){
    int counter = 0;
    MAP_FOREACH (int*, tournament_id, chess->tournaments){
        Tournament tournament = mapGet(chess->tournaments, (MapKeyElement)tournament_id);
        if (tournamentGetWinnerID(tournament) > 0){
            counter++;
        }
    }
    return counter;
}

ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file){
    RETURN_VALUE_IF_TRUE(!chess,CHESS_NULL_ARGUMENT);
    RETURN_VALUE_IF_TRUE(countFinishedTournaments(chess) == 0,CHESS_NO_TOURNAMENTS_ENDED);
    FILE* file = fopen(path_file, "w");
    RETURN_VALUE_IF_TRUE(!file,CHESS_SAVE_FAILURE);
    MAP_FOREACH (int*, tournament_id, chess->tournaments){
        Tournament tournament = mapGet(chess->tournaments, (MapKeyElement)tournament_id);
        if (tournamentGetWinnerID(tournament) > 0){
            fprintf(file, "%d\n%d\n%.2f\n%s\n%d\n%d\n", tournamentGetWinnerID(tournament), calculateLongestPlayTime(tournament), calculateaveragePlayTime(tournament), tournamentGetLocation(tournament), mapGetSize(tournamentGetGames(tournament)), mapGetSize(tournamentGetPlayers(tournament)));
        }
    }
    fclose(file);
    return CHESS_SUCCESS;
}

