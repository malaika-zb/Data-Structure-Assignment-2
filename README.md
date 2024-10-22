# Name: Malaika Zainab
# Class: DS-B
# Roll No: 23i-2605
# Assignment:2 
# DATA STRUCTURES 
# README: The Unseen Journey Quest

# Overview
It is a simple text-based maze game which uses the `ncurses` library. The player navigates through a maze represented by a grid, collects coins, finds a key, and unlocks a door to win the game. The maze is built using a multidimensional linked list and a bit of stack.

# Features
- Navigate a player character through a maze.
- Collect coins to increase score.
- Find a key to unlock a door.
- Encounter bombs that end the game.
- Provide hints for finding the key and door.

# Components
# 1. Node Class
Each node can link to its adjacent nodes (up, down, left, right).

```cpp
class Node {
public:
    char value;
    Node* up;
    Node* down;
    Node* left;
    Node* right;

    Node(char val): value(val), up(nullptr), down(nullptr), left(nullptr), right(nullptr) {}
};
```

# 2. Player Class
This class manages the player's position in the maze.

```cpp
class Player {
private:
    int x, y;

public:
    Player(int startx, int starty): x(startx), y(starty) {}

    void move(int newX, int newY) {
        x = newX;
        y = newY;
    }

    int getX() const {
        return x;
    }
    
    int getY() const {
        return y;
    }
};
```

# 3. Stack Class
It implements a simple undo functionality that allows the player to revert to the previous position.

```cpp
class Stack {
private:
    struct sNode {
        int x, y;
        sNode* down;
        
        sNode(int x, int y): x(x), y(y), down(nullptr) {}
    };

    sNode* top;
    int size;
    int limit;

public:
    Stack(int limit): top(nullptr), size(0), limit(limit) {}

    void push(int x, int y) {
        if (size < limit) {
            sNode* newNode = new sNode(x, y);
            newNode->down = top;
            top = newNode;
            size++;
        }
    }

    bool pop(int& x, int& y) {
        if (top) {
            sNode* temp = top;
            x = top->x;
            y = top->y;
            top = top->down;
            delete temp;
            size--;
            return true;
        }
        return false;
    }

    bool canUndo() const {
        return size > 0;
    }

    void undomove(Player& player) {
        int prevx, prevy;
        if (pop(prevx, prevy)) {
            player.move(prevx, prevy);
        }
    }

    ~Stack() {
        while (top) {
            int x, y;
            pop(x, y);
        }
    }
};
```

# 4. Maze Class
This class represents the maze itself. 
```cpp
class Maze {
private:
    Node*** grid;
    int rows, colms;
    int keyX, keyY;
    bool hasKey;
    int doorX = 10;
    int doorY = 10;

public:
    Maze(int r, int c): rows(r), colms(c), hasKey(false) {
        grid = new Node**[rows];
        // Code for creating the grid
    }

    void placekey() {
        keyX = 5;
        keyY = 5;
        grid[keyY][keyX]->value = '-';
    }

    void collectcoins(int playerX, int playerY, int& score, int& coinscollected) {
        if (grid[playerY][playerX]->value == 'C') {
            grid[playerY][playerX]->value = '.';
            score += 2;
            coinscollected++;
        }
    }

    void printMaze(Player& player) {
        // Code for printing the maze
    }
    
    // Additional methods omitted for brevity
};
```

# Setup Instructions
# Prerequisites
To run this program, ensure you have the following installed:

- A C++ compiler (like `g++`).
- The `ncurses` library.

# Compilation
1. Save the code to a file named `DS_B_i232605.cpp`.
2. Compile the program using the following command:
```
   g++ -o DS_B_i232605 DS_B_i232605.cpp -lncurses
```

# Execution
Run the compiled program with the following command:
```
./DS_B_i232605
```

# Gameplay Instructions
- Use arrow keys to move the player (`P`) around the maze.
- Collect coins represented by `C` to increase your score.
- Avoid bombs represented by `B`, as encountering one will end the game.
- Use the undo functionality to revert your last move if necessary (specific keybinding for undo is not provided in the code).

# Sample Output
Below is a sample output of the game.

# when you collect coin score increases 
# ouput 
```
##################################################
#................................................#
#................................................#
#.................C..............................#
#................................................#
#.......P........................................#
#...B............................................#
#....C...........................................#
#.....................B.....C....................#
#................................................#
#......... ....C.................................#
#................................................#
#...........C...B................................#
#................................................#
#................................................#
#......B............C............................#
#................................................#
#................................................#
#...........................B....................#
#................................................#
#........................C.......................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
##################################################
score: 4
coins collected: 2
distance to key: 3
you are getting closer to the key
```

# when you hit a bomb you lose game ends 
# output 
```
##################################################
#................................................#
#................................................#
#.................C..............................#
#................................................#
#................................................#
#...BP...........................................#
#....C...........................................#
#.....................B.....C....................#
#................................................#
#......... ....C.................................#
#................................................#
#...........C...B................................#
#................................................#
#................................................#
#......B............C............................#
#................................................#
#................................................#
#...........................B....................#
#................................................#
Game Over! you hit a bomb........................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
##################################################
score: 4
coins collected: 2
distance to key: 1
you are getting closer to the key
```

# when you find key and then enter door you win 
# output 
```
##################################################
#................................................#
#................................................#
#..C..............C..............................#
#................................................#
#.......C........................................#
#...B............................................#
#....C...........................................#
#.....................B.....C....................#
#................................................#
#........P ....C.................................#
#................................................#
#...........C...B................................#
#................................................#
#................................................#
#......B............C............................#
#................................................#
#................................................#
#...........................B....................#
#................................................#
you won..................C.......................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
#................................................#
##################################################
score: 0
coins collected: 0
distance to key: 9
you are getting further away from the key
```


# Notes on the Output
- `#` represents walls.
- `.` represents empty spaces.
- `C` represents coins.
- `B` represents bombs.


# Conclusion
This game showcases the use of classes, linked lists, and user interaction through the `ncurses` library. Players can enjoy navigating through the maze, collecting coins, and trying to reach the door while avoiding bombs.
