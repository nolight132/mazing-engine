#include <graphics/camera.h>
#include <graphics/screen.h>
#include <ncurses.h>

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
    mvprintw(1, col1End, "| Map: %dx%d (%d chunks)", size, size, size * size);
    mvprintw(2, col2End, "| YXZ: (%.2f, %.2f, %.2f)", camera.position.y, camera.position.x, camera.position.z);
    mvprintw(2, col1End, "| Chunk: (%d, %d)", geometry.currentChunkZ, geometry.currentChunkX);
    mvprintw(3, col2End, "| %d FPS (%d max)", currentFps, screen.fps);
    mvprintw(3, col1End, "| frameTime: %.2f ms", frameTimeF);
    mvprintw(0, 0, " -- CONTROLS ---------------");
    mvprintw(1, 0, "| WASD - movement");
    mvprintw(2, 0, "| Arrow keys - camera");
    mvprintw(3, 0, "|");
    mvprintw(1, col2Start, "|");
    mvprintw(2, col2Start, "|");
    mvprintw(3, col2Start, "|");
}