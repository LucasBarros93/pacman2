# 🟡 Pacman 2 — C++ SFML Version

A more complete version of the **classic Pacman game**, developed in **C++** using **SFML** for graphics and real-time interaction.  
This version is structured in a fully object-oriented way, built as a university project for Object-Oriented Programming (POO).

---

## 🧱 Features

- 🎮 Real-time game loop with frame control
- 👻 4 unique ghosts (Blinky, Pinky, Inky, Clyde), each with distinct AI
- 🍒 Multiple fruit types with polymorphism
- 💾 Score saving with file I/O
- 💡 Modular object-oriented design
- 🎨 Smooth 2D rendering with SFML

---

## 🗂️ Project Structure

```

.
├── assets/                  # Images, fonts, sounds
├── bin/                     # Compiled binary (output)
├── inc/                     # Header files
│   └── entitys/
├── obj/                     # Object files (temporary)
├── src/                     # Source code (.cpp)
│   └── entitys/
├── main.cpp                 # Game entry point
├── makefile                 # Build instructions
├── trab.pdf                 # Project documentation/report
└── README.md                # This file

````

---

## ⚙️ Build Instructions

### 🔧 Requirements

- `g++` with C++17 support
- SFML 2.x (graphics, window, system)

#### On Debian/Ubuntu-based systems:

```bash
sudo apt install libsfml-dev
````

### 🧪 To Compile:

```bash
make
```

### ▶️ To Run:

```bash
make run
```

Or directly:

```bash
./bin/pacman
```

### 🧹 To Clean:

```bash
make clean
```

---

## 🧠 Design Overview

This project was designed with **modularity** and **object-oriented principles** in mind. Here's a summary of the main classes:

### 🎮 Game Entities

* **Map** (`Map`)
  Handles level layout, collision detection, and wall logic.

* **Pacman**
  The player character: eats, moves, becomes powered-up.

* **Ghost** *(Abstract)*
  Base class for all ghosts, handles movement, AI states, and behavior.

  * **Blinky, Pinky, Inky, Clyde**:
    Each subclass defines unique behavior/personality.

* **Fruit** *(Abstract)*
  Base class for all collectible items.

  * Standard dot `.`, Power pellet `o`, Bonus fruits (e.g., 🍒)

* **ScoreManager**
  Manages the score, handles operator overloading (`++`, `--`), and writes results to a file.

* **Menu**
  Displays start, pause, and game-over screens.

---

## 📝 Project Notes

* The game operates on a **frame-based loop**, simulating a game tick rate.
* Movement is continuous and responsive, closer to the original Pacman arcade feel.
* The graphics engine (**SFML**) ensures smooth animations and sound rendering.
* Designed to be scalable and easy to extend with new features or game mechanics.

---

## 📌 Future Improvements (Ideas)

* Add background music and sound effects
* Implement difficulty levels
* High score tracking and leaderboard
* More complex ghost AI behavior
* Joystick/controller support

---

## 👥 Contributors

* [Lucas Barros](https://github.com/lucasbarros01)
* [André Borba](https://github.com/AndreBorba)

---

## 📄 License

This project is licensed for educational use. Feel free to fork and experiment!
