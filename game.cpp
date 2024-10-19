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

    return 0;
}