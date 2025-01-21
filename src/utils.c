#include <errno.h>
#include <graphics/camera.h>
#include <graphics/screen.h>
#include <graphics/vector.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

FILE *logFile;

int createDir(const char *dir)
{
    if (mkdir(dir, 0755) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkdir failed");
            return -1;
        }
    }
    return 0;
}

int createDirsForFile(const char *filePath)
{
    char path[256];
    strncpy(path, filePath, sizeof(path));

    for (char *p = path + 1; *p; p++)
    {
        if (*p == '/')
        {
            *p = '\0';
            if (createDir(path) == -1)
            {
                return -1;
            }
            *p = '/';
        }
    }
    return 0;
}

int logWrite(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int result = vfprintf(logFile, format, args);
    fflush(logFile);
    va_end(args);
    return result;
}

int consoleLog(const char *format, ...)
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

int errorLog(const char *format, ...)
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
    logFile = file;
    logWrite("----LOG----\n");
    return 0;
}

FILE *createLog(char *logFileName, char *latestLogFilePath)
{
    char logDir[512];
#ifdef _WIN32
    char appDataDir[MAX_PATH];
    if (GetEnvironmentVariable("APPDATA", appDataDir, MAX_PATH) == 0)
    {
        perror("Failed to get environment variable.\n");
        exit(1);
    }
    sprintf(logDir, "%s\\MazingEngine", appDataDir);
#elif __unix__ || __APPLE__
    const char *homeDir = getenv("HOME");
    if (homeDir != NULL)
    {
#ifdef __APPLE__
        sprintf(logDir, "%s/Library/Application Support/MazingEngine/logs", homeDir);
#else
        sprintf(logDir, "%s/.config/MazingEngine", homeDir);
#endif
    }
    else
    {
        perror("Failed to get HOME environment variable.\n");
        exit(1);
    }
#endif
    time_t timeraw = time(NULL);
    struct tm *t = localtime(&timeraw);

    // Format the file name
    char logTime[64];
    strftime(logTime, sizeof(logTime), "%Y-%m-%d_%H-%M-%S.log", t);
    sprintf(logFileName, "%s/%s", logDir, logTime);

    sprintf(latestLogFilePath, "%s/latest.log", logDir);
    createDirsForFile(latestLogFilePath);

    FILE *latestLogFile = fopen(latestLogFilePath, "a");
    if (latestLogFile == NULL)
    {
        fprintf(stderr, "Failed to open log file %s\n", latestLogFilePath);
        return NULL;
    }
    return latestLogFile;
}

void drawDebugInfo(WINDOW *win, Screen screen, Camera camera, GeometryData geometry, int size, long long frameTime,
                   long long sleepTime, double totalPlayTime)
{
    int winHeight, winWidth;
    getmaxyx(win, winHeight, winWidth);

    int currentFps = 1e9 / (frameTime + (sleepTime > 0 ? sleepTime : 0));
    float frameTimeF = (float)frameTime / 1e6;
    int col1End = winWidth - 58;
    int col2End = winWidth - 28;
    int col2Start = 28;
    mvwprintw(win, 0, col1End, " -- DEBUG -------------------- --------------------------");
    mvwprintw(win, 1, col2End, "| Res: %dx%d", winWidth, winHeight);
    mvwprintw(win, 1, col1End, "| Map: %dx%d (%d chunks)", size, size, size * size / geometry.defaultChunkSize);
    mvwprintw(win, 2, col2End, "| YXZ: (%.2f, %.2f, %.2f)", camera.position.y, camera.position.x, camera.position.z);
    mvwprintw(win, 2, col1End, "| Chunk: (%d, %d)", geometry.currentChunkZ, geometry.currentChunkX);
    mvwprintw(win, 3, col2End, "| %d FPS (%d max)", currentFps, screen.refreshRate);
    mvwprintw(win, 3, col1End, "| frameTime: %.2f ms", frameTimeF);
    mvwprintw(win, 0, 0, " -- INFO -------------------");
    mvwprintw(win, 1, 0, "| WASD - movement");
    mvwprintw(win, 2, 0, "| Arrow keys - camera");
    mvwprintw(win, 2, winWidth / 2 - 16, "Time spent: %.2f", totalPlayTime);
    mvwprintw(win, 3, 0, "|");
    mvwprintw(win, 1, col2Start, "|");
    mvwprintw(win, 2, col2Start, "|");
    mvwprintw(win, 3, col2Start, "|");
    wrefresh(win);
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
        case PLAYER:
            logWrite("PP");
            break;
    }
}

void printMap(int **map, int size, Vector2 playerPos)
{
    int **tempMap = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
    {
        tempMap[i] = malloc(size * sizeof(int));
        memcpy(tempMap[i], map[i], size * sizeof(int));
    }
    tempMap[(int)playerPos.y][(int)playerPos.x] = PLAYER;
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
            printTile(tempMap[y][x]);
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
