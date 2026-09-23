# 🟡 Pacman — C++ 

A more complete version of the **classic Pacman game**, developed in **C++** using **SFML** for graphics and real-time interaction.  
This version is structured in a fully object-oriented way, built as a university project for Object-Oriented Programming (POO).

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

## 👥 Contributors

* [Lucas Barros](https://github.com/LucasBarros93)
* [André Borba](https://github.com/AndreBorba)

---

## 📄 License

This project is licensed for educational use. Feel free to fork and experiment!
