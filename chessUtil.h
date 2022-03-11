#ifndef _UTIL_H
#define _UTIL_H

#define REMOVED_PLAYER -1
#define CHESS_WIN_POINTS 2
#define CHESS_DRAW_POINTS 1
#define CHESS_WIN_SCORE_FOR_LEVEL 6
#define CHESS_DRAW_SCORE_FOR_LEVEL 2
#define CHESS_LOSE_SCORE_FOR_LEVEL -10

#define RETURN_VALUE_IF_TRUE(condition, value)                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        if (condition)                                                                                                 \
        {                                                                                                              \
            return value;                                                                                              \
        }                                                                                                              \
    } while (0)

typedef enum {
    FIRST,
    SECOND,
    DRAWGAME
} WinnerPlayer;

#endif //_UTIL_H
