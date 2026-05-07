# ELEC2645 Group Project

This repository contains the final combined version of our ELEC2645 group project, developed using the **STM32 Nucleo-L476RG** board and the provided coursework menu template.

Our final system is designed as a small **menu-based games console** running on a single STM32 board. Instead of developing completely separate standalone programs, both games were integrated into one shared menu framework so that they could be selected and launched from the same LCD interface. This allowed the final project to be more organised, easier to test, and more suitable for embedded hardware demonstration.

The final combined project includes:
- **Game 1 – Snake Game**, developed by me
- **Game 2 – Lonely Rex**, developed by my teammate
- **Game 3 – Placeholder**, kept to preserve the menu structure
- a shared menu system
- shared input, display, and sound modules
- a single hardware platform for the entire combined system

Both games run through the same menu-based system and use the same STM32 hardware setup.

---

## Hardware Used

The whole project was tested and demonstrated on real hardware. The main hardware used in the final system includes:

- STM32 Nucleo-L476RG
- 1.54" SPI LCD display
- Joystick module
- Push buttons
- Buzzer
- Breadboard and jumper wires

The LCD is used to display the main menu and the individual game screens.  
The joystick is used for menu navigation and gameplay control.  
The push buttons are used for game interaction, depending on the control logic of each game.  
The buzzer is used to provide audio feedback during important game events.

---

## Overall Project Structure

The software was organised in a modular way so that different game files and shared modules could be combined in one program.

- `game_1/` contains the source files for the Snake game
- `game_2/` contains the source files for the Lonely Rex game
- `game_3/` contains a placeholder module
- `shared/` contains the shared menu and input handling files
- `Joystick/` contains joystick driver files
- `Buzzer/` contains buzzer control files
- `ST7789V2_Driver_STM32L4/` contains LCD driver files for display handling
- `.ioc`, `CMakeLists.txt`, and related configuration files are used for project setup and build configuration

This structure allowed both group members to work on separate game files while still integrating them into one final system.

---

## Software Design Overview

At the top level, the project uses a **main menu** displayed on the LCD. The user can move through the menu using the joystick and select which game to launch. This means the final project behaves like a simple embedded games console rather than as two unrelated programs.

Below the menu layer, the project contains separate game modules for each individual game. Each game has its own game logic and screen handling, but both are connected to the same shared menu system. In the final version:
- **Game 1** is the Snake game
- **Game 2** is Lonely Rex
- **Game 3** remains as a placeholder

Under the game layer, the project uses several **shared modules**, including:
- `InputHandler`
- `Joystick`
- `LCD`
- `Buzzer`
- `PWM`

These shared modules allow both games to use the same hardware interface without duplicating code. This made the final software architecture simpler and easier to maintain.

At the lowest level, everything runs on the same **STM32 Nucleo-L476RG hardware platform**, using the same LCD, joystick, buzzer, and button setup.

---

# Game 1 – Snake Game

## Overview

Game 1 is a Snake game developed as my main contribution to the ELEC2645 project. I used the provided menu template as the starting point and then implemented my own Snake game so that it could run inside the shared STM32 menu framework.

The game runs on the STM32 board and is displayed on the LCD screen. The player controls the snake using the joystick, collects food to increase the score, and avoids collisions with the walls, obstacles, or the snake’s own body.

This game was not developed as a separate standalone program. Instead, it was integrated into the provided project structure so that it could be launched from the main menu and run on the same hardware system as the other games.

## Main Features

The Snake game includes the following main features:

- LCD menu display
- Snake gameplay running on real STM32 hardware
- Joystick control for movement
- Food spawning
- Score increase after collecting food
- Snake growth after eating food
- Fixed obstacles
- Start screen
- Running state
- Pause state
- Game over screen
- Restart and return-to-menu functions
- Buzzer feedback
- Successful hardware testing and flashing

## Controls

- **Joystick**: move through the menu and control snake direction
- **Joystick press / button**: used for menu selection depending on the hardware setup
- **Button**: start / pause / resume / restart
- **Return to menu**: handled within the game control structure

## Development Summary

The development of the Snake game started with understanding the provided STM32 menu template and how the project was structured. Before writing the game itself, I needed to understand how the menu worked, how input was handled, and how the LCD output was managed.

After that, I replaced the original game file with my own Snake game implementation. The first main part of the coding work was implementing the basic game logic. This included snake movement, food spawning, score increase, and collision detection. I then extended the game by adding fixed obstacles so that the gameplay would be less simple and more challenging.

Another important part of the development was adding multiple **game states**. These included:
- start
- running
- paused
- game over

This made the program easier to organise and improved the user experience because the player could clearly move between different stages of the game.

I also added buzzer feedback for important game events. This helped make the game feel more interactive on the real hardware platform.

## Hardware Testing

A major part of my contribution was testing the Snake game on the actual STM32 hardware. This involved:
- building the project successfully in STM32CubeIDE
- flashing the code to the Nucleo board
- checking that the LCD displayed correctly
- testing joystick navigation and gameplay control
- testing button input
- checking buzzer output

This was important because it confirmed that the game was not only complete in code, but also working correctly on the real board.

---

# Game 2 – Lonely Rex

## Overview

Game 2 is **Lonely Rex**, developed by my teammate. This game was also built using the STM32 Nucleo L476RG board and the provided coursework framework as the base.

Lonely Rex is inspired by the **Google Chrome Dino game**. It is an **endless runner game** where the player controls a dinosaur that must avoid incoming cactus obstacles. The game becomes faster over time, making it more difficult the longer the player survives. The player can jump and duck using joystick controls. The game also includes animations, sound effects, score tracking, and a high score system.

Like Game 1, Lonely Rex was integrated into the same menu-based STM32 framework so that it could be launched from the shared main menu.

## Main Features

Lonely Rex includes the following main features:

- Endless runner gameplay inspired by the Chrome Dino game
- LCD graphics and menu system
- Animated dinosaur sprites
- Jump and duck controls
- Collision detection
- Random cactus spawning
- Increasing game difficulty
- Score tracking
- High score tracking
- Buzzer sound effects
- Start screen
- Game over screen

## Controls

- **Joystick Up** for jumping
- **Joystick Down** for ducking
- **Joystick button (Button 3)** to return to the menu
- **Joystick Up input on menu screens** to start or restart the game

## Development

The development of Lonely Rex began with setting up the STM32 hardware and testing the LCD display, joystick input, and buzzer output individually. Once the hardware was working correctly, the menu system was modified so that the game could launch properly from the main menu.

The next stage was building the core gameplay systems. The dinosaur movement and jumping system were developed first. A gravity and velocity system was used to make the jump feel smoother and more natural instead of moving the sprite by fixed amounts. Extra tuning was also added to make the falling speed faster than the upward jump, giving the jump more impact and making the gameplay feel faster paced.

After this, the cactus obstacle system and collision detection were implemented. Random obstacle group generation was added so the gameplay would feel less repetitive. Sprite animations for running and ducking were then introduced to make the character movement look more alive during gameplay.

The final stage focused on improving the overall game feel. This included:
- adding score and high score tracking
- increasing game speed over time
- adding buzzer sound effects for jumping and milestones
- improving responsiveness through repeated hardware testing on the STM32 board

---

# Game 3 – Placeholder

Game 3 is currently kept as a placeholder in order to preserve the menu structure of the overall project. This ensures that the full menu system remains complete and organised, even though only two main games are used in the final submission.

---

## Integration of Group Work

One of the main goals of the final project was to combine both members’ work into a single program that could compile and run on one STM32 Nucleo board.

This was achieved by:
- keeping each game in its own folder and module
- using a shared menu system
- using shared hardware support modules
- combining both games into one final project build
- testing the combined version on a single board with the same LCD, joystick, button, and buzzer setup

This meant the final result was not just a collection of separate files, but a working integrated embedded system.

---

## Final Note

This group project was developed using the ELEC2645 coursework template as a starting point. The game logic, implementation, testing, and integration for each game were completed by the individual group members as their own work, and then combined into one final menu-based STM32 project.
