# ELEC2645 Project – Snake Game

This project was completed for the ELEC2645 coursework. I used the provided menu template as the starting point and then developed my own Snake game based on it.

The project was tested on real hardware using the STM32 Nucleo-L476RG board, together with an LCD display, joystick, button, and buzzer.

## Project Overview

The main part of this project is a Snake game running on the STM32 board. The player controls the snake using the joystick, collects food to increase the score, and avoids obstacles and collisions.

The project also includes a menu system, allowing the player to enter the game from the main menu and interact with the game through hardware controls.

## Main Features

- LCD menu display
- Snake game implemented on STM32 hardware
- Joystick control
- Score system
- Snake growth after collecting food
- Fixed obstacles
- Game over state
- Buzzer feedback
- Hardware testing and successful flashing

## Hardware Used

- STM32 Nucleo-L476RG
- 1.54" SPI LCD
- Joystick module
- Button
- Buzzer
- Breadboard and jumper wires

## Controls

- **Joystick**: move through the menu and control snake direction
- **Joystick press / button**: used for selection depending on the hardware setup
- **Button**: start / pause / resume / restart

## Project Structure

- `game_1/` – Snake game files
- `game_2/` – placeholder / additional game area
- `shared/` – shared menu and input handling files
- `Joystick/` – joystick driver files
- `Buzzer/` – buzzer driver files
- LCD driver files
- `.ioc` / `CMakeLists.txt` – project configuration files

## Development Summary

I first worked on understanding the menu template and project structure. After that, I replaced the original game file with my own Snake game implementation.

I then tested the code on hardware. The main progress included successful project build, code flashing, LCD menu display, and joystick input testing on the STM32 board. This confirmed that the project was working on real hardware rather than only in software.

## Note

This project was developed based on the ELEC2645 template provided for the coursework. The Snake game implementation, integration, and testing were completed as my own project work.
