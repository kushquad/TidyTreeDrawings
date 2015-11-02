// Assignment 1 - Algorithm 2 (Knuth)
// Tidy Tree Drawing
// Ensure parent is centered around children
// But ignores that tree should occupy minimum width

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include "treedraw.h"

using namespace std;

//Coordinate assignment according to Algorithm 2
void assignCoordinates()
{
	//Initialize walk from root
	node* current = &tree[0];
	int next_number = 1;
	current->status=FIRST_VISIT;

	//Indicates when to stop traversal - when we rereach root
	bool flag = true;

	while (flag) 
	{
		//INORDER PLACEMENT
		switch(current->status) 
		{
			case FIRST_VISIT: 
				current->status = LEFT_VISIT;
				if(current->children.size()>0)
				{
					current = &tree[current->children[0]];
					current->status = FIRST_VISIT; 
				}
				break;
			case LEFT_VISIT: 
				current->x = next_number;
				next_number = next_number + 1;
				current->y = 2*current->height + 1;
				current->status = RIGHT_VISIT;
				if(current->children.size()>1)
				{
					current = &tree[current->children[1]];
					current->status = FIRST_VISIT;
				}
				break;
			case RIGHT_VISIT:
				//Go up level and visit parent
				if(current->parent!=-1)
					current = &tree[current->parent];
				//If parent==-1, means we have reached NIL
				else
					flag = false;
				break;
		}
	}
}

int main(int argc, char **argv)
{
    //Read, build and assign coordinates to tree nodes according to Algorithm 2
	readTree();
	buildTree();
	printTree();
	assignCoordinates();
	printTree();

	//GLUT setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(900,600);
	glutCreateWindow("Algorithm 2");

	//Spacing Parameter set and reassigning tree node coordinates
	setSpacingParameters();

	//Display and initialize 2D setup
	glutDisplayFunc(drawTree);
	init2D();
	glutMainLoop();

	return 0;	
}








