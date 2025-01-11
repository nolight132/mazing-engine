# MazingEngine 🚀

MazingEngine is an ongoing project aimed at creating a terminal-based 3D maze visualization program using [AABBs](https://en.wikipedia.org/wiki/Bounding_volume) and raycasting.
It will generate random mazes and render them in a first-person perspective using ASCII characters.

## Features

-   [x] **Procedurally Generated Maze**
    -   [x] Implement random maze generation
-   [x] **Frame Rate Control**
    -   [x] FPS control for smooth rendering
    -   [x] Handy Unity/Godot-like deltaTime variable so that things don't go flying off the screen on high/low framerates
-   [x] **Raycasting for 3D-like Rendering**
    -   [x] Raycasting algorithm
    -   [x] Wall rendering
-   [x] **Camera Movement**
    -   [x] Controls for moving the camera in the maze
    -   [x] First-person perspective
-   [x] **Collision Detection**
    -   [x] You can't go through walls
    -   [x] There are totally no issues where sometimes a couple times a year with the right angle and a correct attitude you actually can for some reason

## Roadmap
- Proper scoring system
- 2D sprite support
- Basic texture support
- Interactive minimap in another tty (toggleable)
- Hardware-accelerated computation

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
