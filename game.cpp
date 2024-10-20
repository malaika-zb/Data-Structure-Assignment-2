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
//the x and y cooridates
int x,y;
public:
//making the constructor
Player(int startingx, int startingy): x(startingx) , y(startingy)
{}

//for moving the player
void move(int newX, int newY)
{
x = newX;
y = newY;
}

// getter and setter of this class 
int getX() const {
    return x;    //get x coordinate
}
int getY() const {  //get y coordinate 
    return y;
}
};


//for undo i am making class stack
class Stack{

};


//this will print the maze 
class Maze{

};


int main()
{
Player player (1,1); //our player initialized with 1,1 position 
Maze maze (rows, colms);//maze initialized with rows and colms 
Stack stack(10)   //stack with 10 undo moves initialized 

int ch; //switch case variable 
int newX = player.getX();
int newY = player.getY();

switch (ch)
{
case 'w':
newY--;   //for moving up
break;

case 's':
newY++;  //for down 
break;

case 'a':
newX--;  //for left 
break;

case 'd':
newX++; //for right 
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