#include<iostream>
#include<ncurses.h>
using namespace std;

class Node{
public:
//all the nodes for the multidim grid
char value;
Node* up;
Node* down;
Node* left;
Node* right;

//constructor
Node(char val): value(val), up(nullptr), down(nullptr), left(nullptr), right(nullptr)
{}
};  



int main()
{

    return 0;
}