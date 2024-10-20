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
class Maze
{
private:
Node***grid;  //making the grid
int rows; //the no of rows in grid 
int colms; //no of colms 
int keyX; //key at position X
int keyY; //key at position Y
bool hasKey; //if the player has founf key
int doorX;  //door at position x 
int doorY;  //door at position Y 

public:
//constructor 
Maze(int r, int c) : rows(r) , colms(c) , hasKey(false )
{
grid = new Node**[rows];
for(int i=0; i < rows; i++)   //creating pointer grid showing rows
{
grid[i]= new Node*[colms];
for(int j=0; j<colms; j++)   //creating pointer grid showing colms 
{

    // if true than # will be printed leaving other places with .
char cellvalue = (i== 0 || i== rows -1 || j==0 || j ==colms -1 )? '#': '.';
grid[i][j] = new Node(cellvalue);  //the desired value is printed 
}
}

for(int i= 0; i< rows; i++)
{
for (int j=0; j< rows ; j++)
{
//the grid is inter connected using the multi dim linked list 
if (i>0) 
{
    grid[i][j]->up = grid [i-1][j]; // for up 
}

if (i < rows-1)
{
grid[i][j]->down = grid [i-1][j]; //for down 
}

if (j>0)
{
grid[i][j]->left = grid [i][j-1]; //for left
}

if (i < colms-1)
{
grid[i][j]->right = grid [i][j+1];   //for right 
}
}
placekey();
placedoor();
}

void placekey()
{
keyX=5;
keyY=5;
grid [keyY][keyX]->value = '-';
}

void placedoor()
{
grid [doorY][doorX]->value = ' ';
}
}
};


int main()
{
int rows =30;
int colms =50;
Player player (1,1); //our player initialized with 1,1 position 
Maze maze (rows, colms);//maze initialized with rows and colms 
Stack stack(10);   //stack with 10 undo moves initialized 

int ch; //switch case variable 
int newX = player.getX();
int newY = player.getY();
int score = 0; // initial score is zero
int coinscollected= 0;  //no coins collected 

ch = getch();
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
stack.undomove(player);
break;

//exit the game
case 'q':
endwin();
return 0;
}
endwin();
    return 0;
}