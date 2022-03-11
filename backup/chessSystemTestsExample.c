#include <stdlib.h>
#include "../chessSystem.h"
#include "../test_utilities.h"

/*The number of tests*/
#define NUMBER_TESTS 9


bool testChessAddTournament() {
	ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(NULL, 1, 4, "London") == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, NULL) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddTournament(chess, 0, 4, "London") == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddTournament(chess, 1, 0, "London") == CHESS_INVALID_MAX_GAMES);
    ASSERT_TEST(chessAddTournament(chess, -10, 4, "London") == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddTournament(chess, 1, -10, "London") == CHESS_INVALID_MAX_GAMES);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "tel aviv") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "123123") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 1, 10, "Paris") == CHESS_TOURNAMENT_ALREADY_EXISTS);

	chessDestroy(chess);
	return true;
}

bool testChessRemoveTournament() {
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 1) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "Paris") == CHESS_SUCCESS);

    chessDestroy(chess);
    return true;
}

bool testChessAddGame(){
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 4, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 5, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 6, FIRST_PLAYER, 2000) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(NULL, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 4, FIRST_PLAYER, -10) == CHESS_INVALID_PLAY_TIME);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 4, FIRST_PLAYER, 0) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, -20, 1, 3, FIRST_PLAYER, 2000) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess, 1, 1, -20, FIRST_PLAYER, 2000) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess, 1, -10, 2, FIRST_PLAYER, 2000) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess, 1, 2, 2, FIRST_PLAYER, 2000) == CHESS_INVALID_ID);


    chessDestroy(chess);
    return true;
}

bool testChessPrintLevelsAndTournamentStatistics(){
    FILE* file_levels = fopen("./tests/player_levels_your_output.txt", "w");
    if(!file_levels){
        printf("test failed to open file, make sure you have the folder tests in the same folder with chessSystem"
               "excutable file and you have write permissions for the file /player_levels_your_output.txt");
    }
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 2, SECOND_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 4, 1, SECOND_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 2, 4, FIRST_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessSavePlayersLevels(chess, file_levels) == CHESS_SUCCESS);
    ASSERT_TEST(chessSaveTournamentStatistics(chess, "./tests/tournament_statistics_your_output.txt") == CHESS_SUCCESS);
    chessDestroy(chess);
    fclose(file_levels);
    return true;
}

bool testChessPrintLevelsAndTournamentStatistics2(){
    FILE* file_levels = fopen("./tests/player_levels_your_output2.txt", "w");
    if(!file_levels){
        printf("test failed to open file, make sure you have the folder tests in the same folder with chessSystem"
               "excutable file and you have write permissions for the file /player_levels_your_output2.txt");
    }
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 2, SECOND_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 4, 1, SECOND_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 2, 4, FIRST_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 4, "Paris") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_TOURNAMENT_ENDED);
    ASSERT_TEST(chessAddGame(chess, 2, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessSavePlayersLevels(chess, file_levels) == CHESS_SUCCESS);
    ASSERT_TEST(chessSaveTournamentStatistics(chess, "./tests/tournament_statistics_your_output2.txt") == CHESS_SUCCESS);
    chessDestroy(chess);
    fclose(file_levels);
    return true;
}

bool testChessBruteForce() {
	ChessSystem chess = chessCreate();
    for(int i=1; i<10; i++){
        ASSERT_TEST(chessAddTournament(chess, i, (i+1), "London") == CHESS_SUCCESS);
        for(int j=1; j<10; j++){
            ASSERT_TEST(chessAddGame(chess, i, j, (j+1), FIRST_PLAYER, 10) == CHESS_SUCCESS);
        }
    }
	chessDestroy(chess);
	return true;
}

bool testChessRemovePlayer()
{
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 10, 20, DRAW, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 20, 30, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 10, 30, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 30, 20, SECOND_PLAYER, 1000) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessRemovePlayer(chess, 40) == CHESS_PLAYER_NOT_EXIST);
    ASSERT_TEST(chessRemovePlayer(chess, 20) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 1) == CHESS_TOURNAMENT_ENDED);
    ASSERT_TEST(chessEndTournament(chess, 2) == CHESS_TOURNAMENT_NOT_EXIST);

    chessDestroy(chess);
    return true;
}

bool testSaveTournamentStatistics()
{
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 10, "Tokyo") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 10, 20, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 20, 30, FIRST_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 20, 40, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 50, 20, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 50, 10, FIRST_PLAYER, 5000) == CHESS_SUCCESS);
    ChessResult error = CHESS_SUCCESS;
    ASSERT_TEST(chessCalculateAveragePlayTime(chess, -20, &error) == 0);
    ASSERT_TEST(error == CHESS_INVALID_ID);
    ASSERT_TEST(chessCalculateAveragePlayTime(chess, 100, &error) == 0);
    ASSERT_TEST(error == CHESS_PLAYER_NOT_EXIST);
    ASSERT_TEST(chessCalculateAveragePlayTime(chess, 20, &error) == 2250);
    ASSERT_TEST(error == CHESS_SUCCESS);
    ASSERT_TEST(chessCalculateAveragePlayTime(chess, 10, &error) == 4000);
    ASSERT_TEST(error == CHESS_SUCCESS);
    ASSERT_TEST(chessCalculateAveragePlayTime(chess, 50, &error) == 3500);
    ASSERT_TEST(error == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(chess, 20) == CHESS_SUCCESS);
    ASSERT_TEST(chessCalculateAveragePlayTime(chess, 20, &error) == 0);
    ASSERT_TEST(error == CHESS_PLAYER_NOT_EXIST);
    chessDestroy(chess);
    return true;
}

bool testJunkValues()
{
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(NULL, 1, 4, "London") == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, NULL) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddTournament(chess, -1, 4, "London") == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddTournament(chess, 1, -3, "London") == CHESS_INVALID_MAX_GAMES);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "london") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "lOnd6n") == CHESS_INVALID_LOCATION);

    ASSERT_TEST(chessAddTournament(chess, 1, 2, "Tel aviv") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 1, 2, "Tel aviv") == CHESS_TOURNAMENT_ALREADY_EXISTS);

    ASSERT_TEST(chessAddGame(NULL, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddGame(chess, -1, 1, 2, FIRST_PLAYER, 2000) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess, 1, -3, 2, FIRST_PLAYER, 2000) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess, 1, 1, -2, FIRST_PLAYER, 2000) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, -2000) == CHESS_INVALID_PLAY_TIME);
    ASSERT_TEST(chessAddGame(chess, 2, 1, 2, FIRST_PLAYER, 2000) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 4, FIRST_PLAYER, 2000) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessEndTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 3, 4, FIRST_PLAYER, 2000) == CHESS_TOURNAMENT_ENDED);

    ASSERT_TEST(chessRemoveTournament(NULL, 1) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessRemoveTournament(chess, -5) == CHESS_INVALID_ID);
    ASSERT_TEST(chessRemoveTournament(chess, 3) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessRemoveTournament(chess, 1) == CHESS_SUCCESS);

    ASSERT_TEST(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 3000) == CHESS_TOURNAMENT_NOT_EXIST);

    ASSERT_TEST(chessAddTournament(chess, 1, 1, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(NULL, 1) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessRemovePlayer(chess, -1) == CHESS_INVALID_ID);
    ASSERT_TEST(chessRemovePlayer(chess, 1) == CHESS_PLAYER_NOT_EXIST);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(chess, 1) == CHESS_SUCCESS);

    ASSERT_TEST(chessRemovePlayer(chess, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(chess, 1) == CHESS_PLAYER_NOT_EXIST);
    ASSERT_TEST(chessEndTournament(chess, 2) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 3000) == CHESS_TOURNAMENT_ENDED);
    chessDestroy(chess);
    return true;
}

/*The functions for the tests should be added here*/
bool (*tests[]) (void) = {
                      testChessAddTournament,
                      testChessRemoveTournament,
                      testChessAddGame,
                      testChessPrintLevelsAndTournamentStatistics,
                      testChessPrintLevelsAndTournamentStatistics2,
                      testChessBruteForce,
                      testChessRemovePlayer,
                      testSaveTournamentStatistics,
                      testJunkValues
};

/*The names of the test functions should be added here*/
const char* testNames[] = {
                           "testChessAddTournament",
                           "testChessRemoveTournament",
                           "testChessAddGame",
                           "testChessPrintLevelsAndTournamentStatistics",
                           "testChessPrintLevelsAndTournamentStatistics2",
                           "testChessBruteForce",
                           "testChessRemovePlayer",
                           "testSaveTournamentStatistics",
                           "testJunkValues"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
              RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
      fprintf(stdout, "Usage: chessSystem <test index>\n");
      return 0;
  }

  int test_idx = strtol(argv[1], NULL, 10);
  if (test_idx < 1 || test_idx > NUMBER_TESTS) {
      fprintf(stderr, "Invalid test index %d\n", test_idx);
      return 0;
  }

  RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
  return 0;
}
