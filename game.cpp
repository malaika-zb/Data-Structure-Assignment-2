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
private:
struct sNode
{
int x , y;
sNode* down;

//constructor
sNode(int x, int y): x(x) , y(y), down(nullptr)
{}
};
sNode* top;  //representing the top of stack
int size;  //size of stack
int limit;   //limit after which stack cannot do undo 

public:
//constructor 
Stack (int limit) : top (nullptr) , size(0), limit(limit) 
{}

void push (int x, int y)   //push onto the stack
{
if (size < limit)
 {
   sNode* newNode = new sNode(x, y);
   newNode->down = top;
   top = newNode;
   size++; 
 }
}

bool pop (int&x , int&y)   //this is used to pop off the stack
{
    if (top)
    {
        sNode*temp = top;
        x = top->x;       //the stack will pop causing the player to move back to the previous position 
        y = top-> y;
        top = top->down;
        delete temp;
        size--;
        return true;
    }
    return false;
}

bool canUndo() const   //can only undo if size is greater than 0
{
    return size > 0;
}

void undomove(Player & player)      //undo of movement condition 
{
int prevx, prevy;
if(pop(prevx, prevy))
{
player.move(prevx , prevy);
}
}


//destructor avoiding memeory leaks 
~Stack()
    {
        while(top)
        {
        int x,y;
        pop(x,y);
        }
    }


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
int doorX= 10;  //door at position x 
int doorY =10;  //door at position Y 

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
}
placekey();   // placing key
placedoor();           //placing bombs 
}

void placekey()
{
keyX=5;
keyY=5;
grid [keyY][keyX]->value = '-';  // i have made the key - in the maze
}

void placedoor()
{
grid [doorY][doorX]->value = ' ';   // the door is not visible to the player 
}

void placeCoins()  // placing coins at these positions 
{
   int coinpositions[10][2] =  //coins will be used in scoring 
{ 
    {3,3} , {5,7} , {12,12} , {15,10} , {25,20} , {8,5} , {18,3} , {20,15} , {28,8} , {10,10}
};

for (int i =0 ; i<10; i++)
{
  int rows = coinpositions[i][1];
  int colms = coinpositions[i][0];

if (rows > 0 && rows <rows-1 && colms > 0 && colms < colms -1 ) //placing coins randomly through loops 
{
    if (grid[rows][colms]->value == '.')  
     {
        grid[rows][colms]->value == 'C';  //coins are represented by C 
     }
}
}
}

void placebombs ()    //placing bombs at the following positions 
{
int bombposition[5][2] = 
{
{4,6} , {7,15} , {16,12} , {22,8} , {28,18}
};

for (int i= 0; i<5; i++)
{
int rows = bombposition[i][1];    //this will randomly place the bombs without using rand 
int colms= bombposition[i][0];
if (rows > 0 && rows <rows-1 && colms > 0 && colms < colms -1 )
{
    grid[rows][colms]->value = 'B';   //bombs are represented by B 
}
}
}

void printMaze(Player & player)
{
for( int i=0; i<rows; i++)
{
for(int j =0; j<colms; j++)
{
    if(i==player.getY() && j==player.getX())
    {
        mvaddch(i,j, 'P');
    }
    else 
    {
       switch(grid[i][j]->value)
       {
        case '#':
        mvaddch(i,j, '#');
        break;

        case '.':
        mvaddch(i,j, '.');
        break;

        case 'C':
        mvaddch(i,j, 'C');
        break;

        case '-':
        mvaddch(i,j, '-');
        break;


        case ' ':
        mvaddch(i,j, ' ');
        break;


        case 'B':
        mvaddch(i,j, 'B');
        break;

        default:
        mvaddch(i,j, grid[i][j]->value);        
        break;
       } 
    }
}    
}   
refresh(); 
}






};


int main()
{
int rows =30;
int colms =50;

//setting up the basic ncurses library 
initscr();
noecho();
cbreak();
keypad(stdscr , TRUE);
curs_set(0);

Player player (1,1); //our player initialized with 1,1 position 
Maze maze (rows, colms);//maze initialized with rows and colms 
Stack stack(10);   //stack with 10 undo moves initialized 

maze.placeCoins();
maze.placebombs();

int ch; //switch case variable 
int newX = player.getX();
int newY = player.getY();
int score = 0; // initial score is zero
int coinscollected= 0;  //no coins collected 

while (true)
{
clear();

mvprintw(32, 0, "score: %d", score);
mvprintw(33, 0, "coins collected: %d", coinscollected );



//switch case for movement 
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
stack.undomove(player); //this will give you 10 undo moves so you can go to previous position 
break;

//exit the game
case 'q':
endwin(); // if you wanna exit press q 
return 0;
}

}
endwin(); // reached end of game condition 
return 0;
}