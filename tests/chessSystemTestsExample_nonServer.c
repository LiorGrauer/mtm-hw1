
#include <stdlib.h>
#include "../chessSystem.h"
#include "../test_utilities.h"


int compareFile(FILE* fPtr1, FILE* fPtr2)
{
    char ch1, ch2;
    do
    {
        // Input character from both files
        ch1 = fgetc(fPtr1);
        ch2 = fgetc(fPtr2);
        // If characters are not same then return -1
        if (ch1 != ch2)
            return -1;
    } while (ch1 != EOF && ch2 != EOF);

    /* If both files have reached end */
    if (ch1 == EOF && ch2 == EOF)
        return 0;
    else
        return -1;
}

//segel tests
bool testChessAddTournament_segel() {
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 5, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 1, 10, "Paris") == CHESS_TOURNAMENT_ALREADY_EXISTS);

    chessDestroy(chess);
    return true;
}

bool testChessRemoveTournament_segel() {
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 1) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "Paris") == CHESS_SUCCESS);

    chessDestroy(chess);
    return true;
}

bool testChessAddGame_segel(){
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);

    chessDestroy(chess);
    return true;
}

bool testChessPrintLevelsAndTournamentStatistics_segel(){
    FILE* file_levels = fopen("your_output/player_levels_your_output.txt", "w");
    if(!file_levels){
        printf("test failed to open file, make sure you have the folder tests in the same folder with chessSystem"
               "excutable file and you have write permissions for the file /tests/player_levels_your_output.txt");
        return false;
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
    ASSERT_TEST(chessSaveTournamentStatistics(chess, "your_output/tournament_statistics_your_output.txt") == CHESS_SUCCESS);
    fclose(file_levels);

    FILE* file_levels_expected = fopen("expected_output/player_levels_expected_output.txt", "r");
    file_levels = fopen("your_output/player_levels_your_output.txt", "r");
    ASSERT_TEST(compareFile(file_levels, file_levels_expected) == 0);
    
    fclose(file_levels_expected);
    chessDestroy(chess);
    fclose(file_levels);
    return true;
}

//maaroof the ashkiller tests:
bool testChessDestroy_maaroof() {
    chessDestroy(NULL);

    ChessSystem sys1 = chessCreate();
    chessDestroy(sys1);

    ChessSystem sys2 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys2, 1, 1, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys2, 2, 1, "Location two") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys2, 3, 1, "Location three") == CHESS_SUCCESS);
    chessDestroy(sys2);

    ChessSystem sys3 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys3, 1, 1, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys3, 2, 1, "Location two") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys3, 3, 1, "Location three") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys3, 1, 01, 02, FIRST_PLAYER, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys3, 2, 01, 02, FIRST_PLAYER, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys3, 3, 01, 02, FIRST_PLAYER, 5) == CHESS_SUCCESS);
    chessDestroy(sys3);

    return true;
}

bool testChessAddTournamentAndEndTournament_maaroof() {
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 1, NULL) == CHESS_NULL_ARGUMENT);
    ASSERT_TEST(chessAddTournament(NULL, 1, 1, "Location one") == CHESS_NULL_ARGUMENT);

    ASSERT_TEST(chessAddTournament(sys1, 1, 1, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 1, "Location two") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 3, 1, "Location three") == CHESS_SUCCESS);

    ASSERT_TEST(chessAddTournament(sys1, 1, 1, "Location one") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_TEST(chessAddTournament(sys1, 1, 1, "invalid location") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_TEST(chessAddTournament(sys1, 4, -1, "invalid location") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(sys1, 4, -1, "London") == CHESS_INVALID_MAX_GAMES);

    ASSERT_TEST(chessAddTournament(sys1, -2, -1, "invalid location") == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddTournament(sys1, 2, -1, "invalid location") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_TEST(chessAddTournament(sys1, 5, 1, "invalid location") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(sys1, 5, 1, " Invalid location") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(sys1, 10, 1, "L") == CHESS_SUCCESS);
    
    int player_1 = 1;
    int player_2 = 2;
    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 2, player_1, player_2, SECOND_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 3, player_1, player_2, SECOND_PLAYER, 10) == CHESS_SUCCESS);

    ASSERT_TEST(chessEndTournament(sys1, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(sys1, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(sys1, 3) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(sys1, 3) == CHESS_TOURNAMENT_ENDED);
    ASSERT_TEST(chessEndTournament(sys1, 4) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessEndTournament(sys1, -1) == CHESS_INVALID_ID);

   
    ASSERT_TEST(chessAddGame(sys1, 1, 4, 5, FIRST_PLAYER, 10) == CHESS_TOURNAMENT_ENDED);
    ASSERT_TEST(chessSaveTournamentStatistics(sys1, "your_output/test_add_tournament_end_tournament_your_output.txt") == CHESS_SUCCESS);
    FILE* f1 = fopen("your_output/test_add_tournament_end_tournament_your_output.txt", "r");
    FILE* f2 = fopen("expected_output/test_add_tournament_end_tournament_expected_output.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    ChessSystem sys2 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys2, 1, 1, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys2, 2, 1, "Location two") == CHESS_SUCCESS);
    ASSERT_TEST(chessSaveTournamentStatistics(sys2, "document.txt") == CHESS_NO_TOURNAMENTS_ENDED);

    chessDestroy(sys2);
    chessDestroy(sys1);
    return true;
}

bool testChessAddGame_maaroof()
{
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 1, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 1, "Location two") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 3, 1, "Location three") == CHESS_SUCCESS);
    
    int player_1 = 1;
    int player_2 = 2;
    int player_3 = 3;
    int player_4 = 4;

    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 10) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(sys1, 1, player_2, player_1, FIRST_PLAYER, 10) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(sys1, 2, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);

    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_3, FIRST_PLAYER, 10) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_4, FIRST_PLAYER, -1) == CHESS_INVALID_PLAY_TIME);
    //not sure about ZERO as play time
    //ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 0) == CHESS_INVALID_PLAY_TIME);
    ASSERT_TEST(chessAddGame(sys1, -1, player_1, player_2, FIRST_PLAYER, -1) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(sys1, 1, -8, player_2, FIRST_PLAYER, -1) == CHESS_INVALID_ID);

    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_3, SECOND_PLAYER, 10) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_2, FIRST_PLAYER, 10) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_1, FIRST_PLAYER, 10) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_4, SECOND_PLAYER, 10) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(sys1, 3, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 4, player_1, player_2, FIRST_PLAYER, 10) == CHESS_TOURNAMENT_NOT_EXIST);

    ASSERT_TEST(chessAddTournament(sys1, 4, 2, "Location four") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 4, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 4, player_3, player_2, SECOND_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 4, player_3, player_2, FIRST_PLAYER, 10) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(sys1, 4, player_4, player_2, FIRST_PLAYER, 10) == CHESS_EXCEEDED_GAMES);
    
    ASSERT_TEST(chessAddGame(sys1, 4, player_3, player_4, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(sys1,player_3) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 4, player_3, player_4, FIRST_PLAYER, 10) == CHESS_SUCCESS);

    chessDestroy(sys1);
    return true;
}

bool testRemoveTournament_maaroof() 
{
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 3, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 3, "Location two") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 3, 3, "Location three") == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(sys1, 3) == CHESS_SUCCESS);

    int player_1 = 1;
    int player_2 = 2;
    int player_3 = 3;
    //int player_4 = 4;

    ASSERT_TEST(chessAddGame(sys1, 3, player_1, player_2, FIRST_PLAYER, 10) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddGame(sys1, 2, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 2, player_3, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(sys1, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 2, player_1, player_2, FIRST_PLAYER, 10) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddGame(sys1, 2, -1, player_2, FIRST_PLAYER, 10) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(sys1, -1, player_1, player_2, FIRST_PLAYER, -10) == CHESS_INVALID_ID);

    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);

    ASSERT_TEST(chessSaveTournamentStatistics(sys1,"your_output/test_tournament_remove_your_output.txt") == CHESS_NO_TOURNAMENTS_ENDED);

    ASSERT_TEST(chessEndTournament(sys1, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessSaveTournamentStatistics(sys1, "your_output/test_tournament_remove_your_output.txt") == CHESS_SUCCESS);
    FILE* f1 = fopen("your_output/test_tournament_remove_your_output.txt", "r");
    FILE* f2 = fopen("expected_output/test_tournament_remove_expected_output.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    chessDestroy(sys1);
    return true;
}

bool testChessRemovePlayer_maaroof() {
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 3, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 3, "Location two") == CHESS_SUCCESS);
    
    int player_1 = 1;
    int player_2 = 2;
    int player_3 = 3;
    //int player_4 = 4;

    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS); //tour 1: add 1,2
    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_2, FIRST_PLAYER, 4) == CHESS_SUCCESS); //tour 1: add 3,2
    ASSERT_TEST(chessAddGame(sys1, 2, player_1, player_2, FIRST_PLAYER, 10) == CHESS_SUCCESS);//tour 2: add 1,2
    
    FILE* file_levels = fopen("your_output/player_levels_all.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(sys1, file_levels) == CHESS_SUCCESS);
    fclose(file_levels);

    FILE* f1 = fopen("your_output/player_levels_all.txt", "r");
    FILE* f2 = fopen("expected_output/player_levels_all.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    ASSERT_TEST(chessRemovePlayer(sys1, player_1) == CHESS_SUCCESS);//remove 1
    ASSERT_TEST(chessEndTournament(sys1, 1) == CHESS_SUCCESS);//end 1
    ASSERT_TEST(chessEndTournament(sys1, 2) == CHESS_SUCCESS);//end 2
    
    FILE* file_levels_2 = fopen("your_output/remove_player_1.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(sys1, file_levels_2) == CHESS_SUCCESS);//print
    ASSERT_TEST(chessRemovePlayer(sys1, player_1) == CHESS_PLAYER_NOT_EXIST);
    fclose(file_levels_2);

    f1 = fopen("your_output/remove_player_1.txt", "r");
    f2 = fopen("expected_output/remove_player_1.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);


    chessDestroy(sys1);
    return true;
}

bool testChessRemovePlayer_2_maaroof() {
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 3, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 3, "Location two") == CHESS_SUCCESS);

    int player_1 = 1;
    int player_2 = 2;

    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS); //tour 1: add 1,2
    
    FILE* file1 = fopen("your_output/before_remove.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(sys1, file1) == CHESS_SUCCESS);
    fclose(file1);
    
    FILE* f1 = fopen("your_output/before_remove.txt", "r");
    FILE* f2 = fopen("expected_output/before_remove.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    ASSERT_TEST(chessRemovePlayer(sys1, player_1) == CHESS_SUCCESS);//remove 1

    file1 = fopen("your_output/after_remove.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(sys1, file1) == CHESS_SUCCESS);
    fclose(file1);

    f1 = fopen("your_output/after_remove.txt", "r");
    f2 = fopen("expected_output/after_remove.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS); //tour 1: add 1,2
    
    file1 = fopen("your_output/add_same_game_after_remove.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(sys1, file1) == CHESS_SUCCESS);
    fclose(file1);

    f1 = fopen("your_output/add_same_game_after_remove.txt", "r");
    f2 = fopen("expected_output/add_same_game_after_remove.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    chessDestroy(sys1);
    return true;
}

bool testAvgGameTime_maaroof() 
{
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 3, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 3, "Location two") == CHESS_SUCCESS);

    int player_1 = 1;
    int player_2 = 2;
    int player_3 = 3;
    int player_4 = 4;

    ChessResult* chess_result = malloc(sizeof(*chess_result));
    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS); //tour 1: add 1,2
    double avg1 = chessCalculateAveragePlayTime(sys1, player_1, chess_result);
    ASSERT_TEST(*chess_result == CHESS_SUCCESS);
    double avg2 = chessCalculateAveragePlayTime(sys1, player_2, chess_result);
    ASSERT_TEST(*chess_result == CHESS_SUCCESS);
    ASSERT_TEST(avg1 == avg2);
    ASSERT_TEST(avg1 == 6);
    
    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_3, FIRST_PLAYER, 14) == CHESS_SUCCESS);
    avg1 = chessCalculateAveragePlayTime(sys1, player_1, chess_result);
    ASSERT_TEST(*chess_result == CHESS_SUCCESS);
    ASSERT_TEST(avg1 == 10);

    avg1 = chessCalculateAveragePlayTime(sys1, player_3, chess_result);
    ASSERT_TEST(*chess_result == CHESS_SUCCESS);
    ASSERT_TEST(avg1 == 14);

    ASSERT_TEST(chessAddGame(sys1, 2, player_1, player_3, FIRST_PLAYER, 20) == CHESS_SUCCESS);
    avg1 = chessCalculateAveragePlayTime(sys1, player_1, chess_result);
    ASSERT_TEST(*chess_result == CHESS_SUCCESS);
    ASSERT_TEST(avg1 == 40.0/3.0);

    avg1 = chessCalculateAveragePlayTime(sys1, player_3, chess_result);
    ASSERT_TEST(*chess_result == CHESS_SUCCESS);
    ASSERT_TEST(avg1 == 17);

    chessCalculateAveragePlayTime(sys1, player_4, chess_result);
    ASSERT_TEST(*chess_result == CHESS_PLAYER_NOT_EXIST);

    chessCalculateAveragePlayTime(sys1, -1, chess_result);
    ASSERT_TEST(*chess_result == CHESS_INVALID_ID);

    chessCalculateAveragePlayTime(NULL, player_4, chess_result);
    ASSERT_TEST(*chess_result == CHESS_NULL_ARGUMENT);

    chessCalculateAveragePlayTime(NULL, -1, chess_result);
    ASSERT_TEST(*chess_result == CHESS_NULL_ARGUMENT);

    ASSERT_TEST(chessRemovePlayer(sys1, player_1) == CHESS_SUCCESS);
    chessCalculateAveragePlayTime(sys1, player_1, chess_result);
    ASSERT_TEST(*chess_result == CHESS_PLAYER_NOT_EXIST);

    avg1 = chessCalculateAveragePlayTime(sys1, player_3, chess_result);
    ASSERT_TEST(*chess_result == CHESS_SUCCESS);
    ASSERT_TEST(avg1 == 17);

    free(chess_result);
    chessDestroy(sys1);
    return true;
}

bool testSavePlayerLevelsAndTournamentStatistics_maaroof() 
{
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 3, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 3, "Location two") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 3, 3, "Location three") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 4, 3, "Location four") == CHESS_SUCCESS);

    int player_1 = 1;
    int player_2 = 2;
    //int player_3 = 3;
    //int player_4 = 4;
    //int player_5 = 5;

    char* fileName = "";
   // FILE* file;

    ASSERT_TEST(chessAddGame(sys1, 4, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(sys1, 4) == CHESS_SUCCESS);
    fileName = "your_output/tournament_statistics_1.txt";
    ASSERT_TEST(chessSaveTournamentStatistics(sys1, fileName) == CHESS_SUCCESS);
    
    FILE* f1 = fopen("your_output/tournament_statistics_1.txt", "r");
    FILE* f2 = fopen("expected_output/tournament_statistics_1.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    ASSERT_TEST(chessAddGame(sys1, 3, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(sys1, 1));
    ASSERT_TEST(chessEndTournament(sys1, 3) == CHESS_SUCCESS);
    fileName = "your_output/tournament_statistics_2.txt";
    ASSERT_TEST(chessSaveTournamentStatistics(sys1, fileName) == CHESS_SUCCESS);

    f1 = fopen("your_output/tournament_statistics_2.txt", "r");
    f2 = fopen("expected_output/tournament_statistics_2.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    ASSERT_TEST(chessAddGame(sys1, 2, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(sys1, 2) == CHESS_SUCCESS);
    fileName = "your_output/tournament_statistics_3.txt";
    ASSERT_TEST(chessSaveTournamentStatistics(sys1, fileName) == CHESS_SUCCESS);

    f1 = fopen("your_output/tournament_statistics_3.txt", "r");
    f2 = fopen("expected_output/tournament_statistics_3.txt", "r");
    ASSERT_TEST(compareFile(f1, f2) == 0);
    fclose(f1);
    fclose(f2);

    chessDestroy(sys1);
    return true;
}

bool testLiorIsTheKing(){
    ChessSystem sys1 = chessCreate();
    ASSERT_TEST(chessAddTournament(sys1, 1, 7, "Location one") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(sys1, 2, 7, "Location two") == CHESS_SUCCESS);

    int player_1 = 1;
    int player_2 = 2;
    int player_3 = 3;
    int player_4 = 4;
    //int player_5 = 5;

    //char* fileName = "";
   // FILE* file;

    ASSERT_TEST(chessAddGame(sys1, 1, player_1, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_4, FIRST_PLAYER, 6) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS);

    ASSERT_TEST(chessRemovePlayer(sys1, 3) == CHESS_SUCCESS);
    
    FILE* f1 = fopen("your_output/lior_1.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(sys1, f1) == CHESS_SUCCESS);
    fclose(f1);

    FILE* k1 = fopen("your_output/lior_1.txt", "r");
    FILE* k2 = fopen("expected_output/lior_1.txt", "r");
    ASSERT_TEST(compareFile(k1, k2) == 0);
    fclose(k1);
    fclose(k2);

    ASSERT_TEST(chessAddGame(sys1, 1, player_3, player_2, FIRST_PLAYER, 6) == CHESS_SUCCESS);
    FILE* f2 = fopen("your_output/lior_2.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(sys1, f2) == CHESS_SUCCESS);
    fclose(f2);

    FILE* k3 = fopen("your_output/lior_2.txt", "r");
    FILE* k4 = fopen("expected_output/lior_2.txt", "r");
    ASSERT_TEST(compareFile(k1, k2) == 0);
    fclose(k3);
    fclose(k4);

    ASSERT_TEST(chessAddGame(sys1, 1, player_4, player_2, DRAW, 6) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(sys1, 2, player_2, player_3, FIRST_PLAYER, 6) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(sys1, 2) == CHESS_SUCCESS);
    
    FILE* f3 = fopen("your_output/lior_3.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(sys1, f3) == CHESS_SUCCESS);
    fclose(f3);

    FILE* k5 = fopen("your_output/lior_3.txt", "r");
    FILE* k6 = fopen("expected_output/lior_3.txt", "r");
    ASSERT_TEST(compareFile(k1, k2) == 0);
    fclose(k5);
    fclose(k6);

    ASSERT_TEST(chessEndTournament(sys1, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(sys1, 3) == CHESS_SUCCESS);

    FILE* f4 = fopen("your_output/lior_4.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(sys1, f4) == CHESS_SUCCESS);
    fclose(f4);

    FILE* k7 = fopen("your_output/lior_4.txt", "r");
    FILE* k8 = fopen("expected_output/lior_4.txt", "r");
    ASSERT_TEST(compareFile(k1, k2) == 0);
    fclose(k7);
    fclose(k8);

    chessDestroy(sys1);
    return true;
}


/*The functions for the tests should be added here*/
bool (*tests[]) (void) = {
        testChessAddTournament_segel,
        testChessRemoveTournament_segel,
        testChessAddGame_segel,
        testChessPrintLevelsAndTournamentStatistics_segel,
        testChessDestroy_maaroof,
        testChessAddTournamentAndEndTournament_maaroof,
        testChessAddGame_maaroof,
        testRemoveTournament_maaroof,
        testChessRemovePlayer_maaroof,
        testChessRemovePlayer_2_maaroof,
        testAvgGameTime_maaroof,
        testSavePlayerLevelsAndTournamentStatistics_maaroof,
        testLiorIsTheKing
};

/*The names of the test functions should be added here*/
const char* testNames[] = {
        "testChessAddTournament_segel",
        "testChessRemoveTournament_segel",
        "testChessAddGame_segel",
        "testChessPrintLevelsAndTournamentStatistics_segel",
        "testChessDestroy_maaroof",
        "testChessAddTournamentAndEndTournament_maaroof",
        "testChessAddGame_maaroof",
        "testRemoveTournament_maaroof",
        "testChessRemovePlayer_maaroof",
        "testChessRemovePlayer_2_maaroof",
        "testAvgGameTime_maaroof",
        "testSavePlayerLevelsAndTournamentStatistics_maaroof",
        "testLiorIsTheKing"
};

#define NUMBER_TESTS 13

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
