#include<iostream>
#include<ncurses.h>
using namespace std;

class Node{
public:
//this will be the multidimensional linked list
char value;
Node* up;
Node* down;
Node* left;
Node* right;

//constructor
Node(char val): value(val), up(nullptr), down(nullptr), left(nullptr), right(nullptr)
{}
};  

class Player{
private:
int x,y;
public:
//making the constructor
Player(int startingx, int startingy): x(startingx) , y(startingy)
{}

void move(int newX, int newY)
{
x = newX;
y = newY;
}

// getter and setter of this class 
int getX() const {
    return x;
}
int getY() const {
    return y;
}
};

class Stack{

};


class Maze{





};


int main()
{
Player player (1,1);

int ch;
int newX = player.getX();
int newY = player.getY();

switch (ch)
{
case 'w':
newY--;
break;

case 's':
newY++;
break;

case 'a':
newX--;
break;

case 'd':
newX++;
break;

//for undo move 
case 'z':
break;

//exit the game
case 'q':
return 0;
}


    return 0;
}