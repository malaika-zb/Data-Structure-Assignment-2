0//including the allowed libraries 
#include<iostream>
#include<ncurses.h>

using namespace std;
//node class for linking the grid with each other 

class Node
{
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

//player class for the movement of player P
class Player
{
private:
//the x and y cooridates
int x,y;

public:
//making the constructor
Player(int startx, int starty): x(startx) , y(starty)
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
int rows, colms; //the no of rows and colms in grid 
int keyX, keyY; //key at position X and Y
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
for (int j=0; j< colms ; j++)
{
//the grid is inter connected using the multi dim linked list 
if (i>0) 
{
    grid[i][j]->up = grid [i-1][j]; // for up 
}

if (i < rows-1)
{
grid[i][j]->down = grid [i+1][j]; //for down 
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


//placing key at this position 
void placekey()
{
keyX=5;
keyY=5;
grid [keyY][keyX]->value = '-';  // i have made the key - in the maze
}

//the door is placed at the appropriate place within boundaries 
void placedoor()
{
if(doorX > 0 && doorX < colms - 1 && doorY > 0 && doorY < rows - 1)
{
grid [doorY][doorX]->value = ' ';   // the door is not visible to the player 
}
}

//this will calculate the door sensing hint using the distance formula 
void providedoorhint(Player & player )
{
    //player is initialized a position 
    int playerX = player.getX();
    int playerY = player.getY();

//the use of distance formula 
    int distancetodoor = (playerX > doorX ? playerX-doorX: doorX-playerX) + (playerY>doorY? playerY-doorY: doorY-playerY);
    mvprintw(36,0, "distance to door: %d", distancetodoor); 
    if (distancetodoor <=3)  //setting up a threshold 
    {
        mvprintw(37,0,"you are getting closer to the door");  //close 
    }
    else 
    {
        mvprintw(37,0,"you are getting away from the door"); //far away 
    } 
}

void placeCoins()  // placing coins at these positions 
{
   int coinpositions[10][2] =  //coins will be used in scoring 
{ 
    {3,3} , {5,7} , {12,12} , {15,10} , {25,20} , {8,5} , {18,3} , {20,15} , {28,8} , {10,10}
};

for (int i =0 ; i<10; i++)
{
  int row = coinpositions[i][1];  //making a loop as we cannot use rand 
  int col = coinpositions[i][0];

if (row > 0 && row <rows-1 && col > 0 && col < colms-1 ) //placing coins randomly through loops 
{
    if (grid[row][col]->value == '.')  
     {
        grid[row][col]->value = 'C';  //coins are represented by C 
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
int row = bombposition[i][1];    //this will randomly place the bombs without using rand 
int col = bombposition[i][0];
if (row > 0 && row <rows-1 && col > 0 && col < colms-1 )
{
    grid[row][col]->value = 'B';   //bombs are represented by B 
}
}
}

bool bombencountered(int playerX, int playerY)   //if player encounters bomb game ends 
{
    return grid[playerY][playerX]->value == 'B';
}


//the player will collect coins to increase score 
void collectcoins(int playerX, int playerY, int& score, int& coinscollected )
{
    if(grid[playerY][playerX]->value == 'C')
    {
        grid[playerY][playerX]->value = '.';
        score+=2;   //score increased by 2 
        coinscollected++;
    }
}

//the key is collected and placed at a location for player to find 
void collectkey(int playerX, int playerY)
{
    if(grid[playerY][playerX]->value == '-')
    {
        hasKey = true;
        grid[playerY][playerX]->value = '.';
    }
}

//this is the condition that door will only be opened if the player finds key 
bool canenterdoor(int playerX, int playerY)
{
    return hasKey && (playerX  == doorX && playerY  == doorY);
}


//this will print the maze onto the terminal 
void printMaze(Player & player)
{
for( int i=0; i<rows; i++)
{
for(int j =0; j<colms; j++)
{
    if(i==player.getY() && j==player.getX())
    {   
        attron(A_BOLD | COLOR_PAIR(5));
        mvaddch(i,j, 'P');       //color for player 
        attroff(A_BOLD | COLOR_PAIR(5));
    }
    else 
    {
       switch(grid[i][j]->value)
       {
        case '#':  //for grid 
        attron(COLOR_PAIR(1));
        mvaddch(i,j, '#');
        attron(COLOR_PAIR(1));
        break;

        case '.':   //for empty spaces 
        attron(COLOR_PAIR(2));
        mvaddch(i,j, '.');
        attron(COLOR_PAIR(2));
        break;

        case 'C':              //for coins 
        attron(COLOR_PAIR(3));
        mvaddch(i,j, 'C');
        attron(COLOR_PAIR(3));
        break;

        case '-':             //for key 
        attron(COLOR_PAIR(2));
        mvaddch(i,j, '-');
        attron(COLOR_PAIR(2));
        break;


        case ' ':              //for door 
        attron(COLOR_PAIR(4));
        mvaddch(i,j, ' ');
        attron(COLOR_PAIR(4));
        break;


        case 'B':        //for bombs 
        attron(COLOR_PAIR(4));
        mvaddch(i,j, 'B');
        attron(COLOR_PAIR(4));
        break;

        default:      //default setting 
        mvaddch(i,j, grid[i][j]->value);        
        break;
       } 
    }
}    
}   
refresh(); 
}


//this is the hint to find the key 
void providekeyhint(Player& player)
{
    int playerX = player.getX();
    int playerY = player.getY();
    int distancetokey = (playerX > keyX? playerX-keyX: keyX-playerX) + (playerY>keyY? playerY-keyY: keyY-playerY);
    mvprintw(34,0, "distance to key: %d", distancetokey); //distance calculated using the distance formula 
    if(distancetokey<=3)
    {
        mvprintw(35,0,"you are getting closer to the key");
    }
    else
    {
        mvprintw(35,0,"you are getting further away from the key");
    }
}

//so that player does not go beyond the boundary
bool ismovevelid(int newX, int newY)
{
    if(newX<1 || newX >= colms-1 || newY<1 || newY>= rows-1)
    {
        return false;
    }
    return grid[newY][newX]->value!= '#';
}

//destructor
~Maze()
{ 
for(int i=0; i<rows; i++)
{
for (int j=0; j<colms; j++)
{
    delete grid[i][j];
}
delete[] grid[i];
}
delete[] grid;
}
};

void setupColors()
{    //adding colors to the game 
start_color();
init_pair(1, COLOR_WHITE, COLOR_BLACK);
init_pair(2, COLOR_GREEN, COLOR_BLACK);
init_pair(3, COLOR_YELLOW, COLOR_BLACK);
init_pair(4, COLOR_RED, COLOR_BLACK);
init_pair(5, COLOR_BLUE, COLOR_BLACK);
}


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
setupColors();

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
maze.printMaze(player);
mvprintw(32, 0, "score: %d", score); //the final score 
mvprintw(33, 0, "coins collected: %d", coinscollected );  //the no of coins collected 
maze.providekeyhint(player);
maze.providedoorhint(player);


//switch case for movement 
ch = getch();
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
if(stack.canUndo())
{
    stack.undomove(player);   //using z for undo moves 
}
break;

//exit the game
case 'q':
endwin(); // if you wanna exit press q 
return 0;
}

if(maze.ismovevelid(newX, newY))    //if the move is valid check 
{
    stack.push(player.getX(), player.getY());
    player.move(newX, newY);
    maze.collectcoins(newX, newY, score, coinscollected);
    maze.collectkey(newX, newY);


mvprintw(37, 0, "Checking position: (%d, %d)", newX, newY);
refresh();  //checking position i used because a segmentation fault came 

    if (maze.bombencountered(newX, newY))
    {
    mvprintw(20,0, "Game Over! you hit a bomb");   //if it hits bomb this will be displayed 
    refresh();
    getch();
    endwin();
    return 0;
    }
if(maze.canenterdoor(newX, newY))
{
    mvprintw(20, 0, "you won");   //if you find door this will be displayed 
    refresh();
    getch();
    break;
}

maze.collectcoins(player.getX() , player.getY(), score, coinscollected);   
}
}
endwin(); // reached end of game condition 
return 0;
}