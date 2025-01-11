#include <graphics/camera.h>
#include <graphics/screen.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

FILE *logFile;

int logWrite(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int result = vfprintf(logFile, format, args);
    va_end(args);
    return result;
}

int timedLogWrite(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    time_t timeraw = time(NULL);
    struct tm *t = localtime(&timeraw);

    char timeStr[300];
    snprintf(timeStr, sizeof(timeStr), "[%04d-%02d-%02d %02d:%02d:%02d]", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);

    int result = logWrite("%s ", timeStr);     // Log the timestamp
    result += vfprintf(logFile, format, args); // Write the log message

    va_end(args);
    return result;
}

int errorLogWrite(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int result = logWrite("ERROR: ");
    result += vfprintf(logFile, format, args);
    va_end(args);
    return result;
}

int initLog(FILE *file)
{
    logFile = malloc(sizeof(char) * 50);
    logFile = file;
    logWrite("----LOG----");
    return 0;
}

FILE *createLog(char *logFileName)
{
    time_t timeraw = time(NULL);
    struct tm *t = localtime(&timeraw);

    // Format the file name
    strftime(logFileName, 50, "./%Y-%m-%d_%H-%M-%S.log", t);
    printf("%s", logFileName);

    FILE *logFile = fopen("latest.log", "a");
    if (logFile == NULL)
    {
        perror("Failed to open log file");
        free(logFileName);
        return NULL;
    }
    return logFile;
}

void printDebugInfo(Screen screen, Camera camera, GeometryData geometry, int size, long long frameTime,
                    long long sleepTime)
{
    // Calculate and display FPS
    int currentFps = 1e9 / (frameTime + (sleepTime > 0 ? sleepTime : 0));
    float frameTimeF = (float)frameTime / 1e6;
    int col1End = screen.width - 58;
    int col2End = screen.width - 28;
    int col2Start = 28;
    mvprintw(0, col1End, " -- DEBUG -------------------- --------------------------");
    mvprintw(1, col2End, "| Res: %dx%d", screen.width, screen.height);
    mvprintw(1, col1End, "| Map: %dx%d (%d chunks)", size, size, size * size / geometry.defaultChunkSize);
    mvprintw(2, col2End, "| YXZ: (%.2f, %.2f, %.2f)", camera.position.y, camera.position.x, camera.position.z);
    mvprintw(2, col1End, "| Chunk: (%d, %d)", geometry.currentChunkZ, geometry.currentChunkX);
    mvprintw(3, col2End, "| %d FPS (%d max)", currentFps, screen.refreshRate);
    mvprintw(3, col1End, "| frameTime: %.2f ms", frameTimeF);
    mvprintw(0, 0, " -- INFO -------------------");
    mvprintw(1, 0, "| WASD - movement");
    mvprintw(2, 0, "| Arrow keys - camera");
    mvprintw(3, 0, "|");
    mvprintw(1, col2Start, "|");
    mvprintw(2, col2Start, "|");
    mvprintw(3, col2Start, "|");
}

void printTile(int type)
{
    switch (type)
    {
        case PATH:
            logWrite("  ");
            break;
        case WALL:
            logWrite("░░");
            break;
        case BORDER:
            logWrite("██");
            break;
        case GOAL:
            logWrite("GG");
            break;
        case START:
            logWrite("SS");
            break;
    }
}

void printMap(int **map, int size)
{
    for (int x = 0; x < size + 2; x++)
    {
        printTile(BORDER);
    }
    logWrite("\n");
    for (int y = 0; y < size; y++)
    {
        printTile(BORDER);
        for (int x = 0; x < size; x++)
        {
            printTile(map[y][x]);
        }
        printTile(BORDER);
        logWrite("\n");
    }
    for (int x = 0; x < size + 2; x++)
    {
        printTile(BORDER);
    }
    logWrite("\n");
}
