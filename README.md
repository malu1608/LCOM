# Jump King–style Platform Game in C (Minix)

A platform game written in **C** that runs directly on **Minix 3**, talking to the hardware through our own device drivers, with no game engine or graphics library.

The player climbs a level made of three stages of increasing difficulty, using the arrow keys to move and the space bar to jump. The fastest times go to a scoreboard.

## Features
- Main menu with play, rules and scoreboard, navigated with the **mouse**
- Player movement with the **keyboard**
- Level in three stages, each with its own scenery and a flag at the end
- Sprites and animations drawn pixel by pixel in **VBE graphics mode**
- **Timer**-driven game loop and animations
- Scoreboard sorted by time, with dates from the **real-time clock (RTC)**
- Pause menu

## Device drivers
| Device | What it does in the game |
|---|---|
| Timer (i8254) | Frame rate and animations |
| Keyboard (i8042) | Movement, jump and shortcuts |
| Mouse (i8042) | Menu navigation |
| Video card (VBE) | Double-buffered graphics and sprites |
| RTC | Dates for the scoreboard |

The `lab0`–`lab5` folders contain the course labs where each driver was first built and tested.

## My contribution
The **video card driver** (lab 5: VBE graphics modes, drawing rectangles, patterns and sprites) and keyboard handling, which became the base of the game's graphics layer, plus early work on the project's video module.

## Team
Group of 4 · André Ávila · **Maria Luísa Salvador** · Mariana Teixeira · Nuno Silva

## Run
Inside a Minix 3 virtual machine:
```bash
cd proj/src
make
lcom_run proj
```

---
Academic project developed at FEUP (Faculty of Engineering, University of Porto), Computer Laboratory (LCOM) 2021/22.
