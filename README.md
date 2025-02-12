# cub3D

## Overview
🎯 **cub3D** is a graphical pseudo-3D game inspired by *Wolfenstein 3D*, developed in C using the **MLX graphical library** as part of the 42 School curriculum. The project demonstrates raycasting techniques to render a first-person perspective.

## 🕹️ Features
- **Parsing:** Reads and interprets a map file defining walls and player positions.
-  **Raycasting Algorithm:** Implements a raycasting engine to create a 3D effect using a 2D map.
- **Texture Mapping:** Applies textures to walls using pixel manipulation for a realistic appearance.
- **Bonus Features:** Includes a **minimap** for navigation and **sprites**, including a player's gunshot animation.

## 🛠️ Installation & Usage
### Prerequisites
- **MLX Library** (MinilibX)
- **gcc or clang** compiler
- **Make** utility

### Installation
```bash
# Clone the repository
git clone https://github.com/hienptx/cub3D.git
cd cub3D

# Compile the project
make

# Compile the project with bonus
make bonus

# Run the gamel like in Demo
./cub3D maps/2.cub
```

## 🎥 Screenshots & Video Demo
![Gameplay Screenshot](images/cub3D2.png)

[Watch the Demo](https://youtu.be/EVUZNPmnXw8)

## 🎮 Controls
- **W / A / S / D** - Move forward, left, backward, right
- **Left / Right Arrow** - Rotate camera
- **ESC** - Exit game
### Controls with bonus
- **SPACE** - Shooting

## 📑 Code Structure
- **main.c** - Entry point, initializes game
- **parsing/** - Handles map loading and validation
- **raycasting/** - Implements the raycasting algorithm
- **rendering/** - Manages drawing and textures

## 🔍 Algorithm
- **Raycasting Algorithm (DDA - Digital Differential Analysis)**
    Purpose: Converts a 2D grid-based map into a pseudo-3D perspective.
    
    Key Steps:
      1. Cast a ray for each vertical pixel column on the screen.
      2. Step through the grid to detect where the ray hits a wall.
      3. Calculate distance from the player to the wall to determine wall height.
      4. Adjust perspective using a fish-eye correction (to avoid distortion).
      5. Texture Mapping: If implemented, determines which texture to render on the wall.
    Used Functions:
      dda_algorithm() → Iterates through grid cells until a wall is hit.
      calculate_wall_height() → Uses distance to compute correct rendering.
  
## ⚙️ Configure input file
```bash
# Open .cub file
vim maps/1.cub
```
### Example:
```javascript
# paths to wall pictures for rendering
NO ./images/N_wide.png
SO ./images/S_wide.png
WE ./images/W_wide.png
EA ./images/E_wide.png

# rgb color values of floor (F) and ceiling (C)
F 100,150,105
C 60,50,110

# maze with 1 represent wall, 0 empty space
# place player with N, S, E, W
111111111
100100111
10010S001
100110001
110001101
110100101
111111111
```

## 🏗️ Challenges & Learnings
- Implementing an efficient **raycasting engine**.
- Handling **pixel rendering and texture mapping**.
- Parsing and validating **custom map formats**.

## Contributors
- 👤 [Hien Pham](https://github.com/hienptx)
- 👤 [Dongjub Lee](https://github.com/dongjle2)

## License
📜 This project is licensed under the MIT License - see the LICENSE file for details.

