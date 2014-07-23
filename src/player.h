#ifndef THEA_PLAYER_H
#define THEA_PLAYER_H

#define HEAD_Y          LINES/2
#define HEAD_X          COLS/2+1
#define BODY_Y          LINES/2+1
#define BODY_X          COLS/2
#define LEFT_FOOT_Y     LINES/2+2
#define LEFT_FOOT_X     COLS/2+1
#define RIGHT_FOOT_Y    LEFT_FOOT_Y
#define RIGHT_FOOT_X    COLS/2+2
#define LEFT_ARM_Y      BODY_Y
#define LEFT_ARM_X      COLS/2-1
#define RIGHT_ARM_Y     BODY_Y
#define RIGHT_ARM_X     COLS/2+1

static bool walk_status = 0;

#define PRINT_PLAYER()                              \
    do {                                            \
        mvprintw(HEAD_Y, HEAD_X, "0");              \
        mvprintw(BODY_Y, BODY_X, "\\|/");           \
        mvprintw(LEFT_FOOT_Y, LEFT_FOOT_X, "/");    \
        if (walk_status)                            \
        mvprintw(RIGHT_FOOT_Y, RIGHT_FOOT_X, "|");  \
        else                                        \
        mvprintw(RIGHT_FOOT_Y, RIGHT_FOOT_X, "\\"); \
        walk_status = !walk_status;                 \
    } while(0)

#endif // THEA_PLAYER_H
