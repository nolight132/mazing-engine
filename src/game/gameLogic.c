#include "types.h"
#include <game/gameLogic.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

int checkForWin(Vector2 playerPos, Vector2 goal)
{
    return (int)playerPos.x == (int)goal.x && (int)playerPos.y == (int)goal.y;
}

void victoryScreen(int score, double playTime)
{
    clear();

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    const char *victoryMsg = "Congratulations, You Won!";
    const char *restartMsg = "[R] Restart";
    const char *quitMsg = "[Q] Quit";

    // Centered display
    mvprintw(maxY / 4, (maxX - strlen(victoryMsg)) / 2, victoryMsg);
    mvprintw(maxY / 2 - 1, maxX / 4, "Score: %d", score);
    mvprintw(maxY / 2, maxX / 4, "Playtime: %.2f seconds", playTime);

    // Options at the bottom
    mvprintw(maxY - 3, maxX / 4, restartMsg);
    mvprintw(maxY - 3, 3 * maxX / 4 - strlen(quitMsg), quitMsg);

    // Interaction loop
    int ch;
    while (true)
    {
        ch = getch();
        switch (ch)
        {
            case 'R':
            case 'r':
                endwin();
                exit(0);
                return;
            case 'Q':
            case 'q':
                endwin();
                exit(0);
        }
    }
}
