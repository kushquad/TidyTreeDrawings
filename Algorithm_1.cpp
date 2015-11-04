// Assignment 1 - Algorithm 1 (Naive)
// Tidy Tree Drawing
// Naive tree drawer of minimum width

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treedraw.h"

using namespace std;

void assignCoordinates()
{
	//Next X coordinate available for every row, worst case we have N rows
	//Initially x=1 is free for every row
	int next_x[N];
    for(int i=0;i<N; i++)
    	next_x[i] = 1;

	//Initialize walk from root
	node* current = &tree[0];
	current->status = 0;
	//Indicates when to stop traversal - when we rereach root
	bool flag = true;

	//Tree traversal
	//flag indicates if parent has been re-reached, false if re-reached
	while(flag) 
	{
		//First time visiting this node
		if(current->status==0)
		{
			//Allocate the x coordinate available for this height
			//Allocate y according to node's height
			//Update the next x coordinate
			current->x = next_x[current->height];
			current->y = 2*current->height + 1;
			next_x[current->height] += 2;

			//Set all children to 0 which means we are going to visit them
			for(int i=0; i<current->children.size();i++)
				tree[current->children[i]].status = 0;
			//status = 1 indicates all children visited
			current->status = 1;
		}

		//This condition checks if we have visited all children or not
		else if(current->status>=1 && current->status<=current->children.size())
		{
			//Visiting a new child
			current->status++;
			current = &tree[current->children[current->status-2]];
		}

		//All children of the node visited, go back to parent
		//However, if this is the root, it has no parent
		//Stop algorithm then
		else
		{
			if(current->parent!=-1)
					current = &tree[current->parent];
			//If parent==-1, means we have reached NIL, i.e rereached
			else
				flag = false;
		}
	}
}

void updateRoutine(void)
{
	//Read, build and assign coordinates to tree nodes according to Algorithm 2
	//Set spacing for graphical output and draw
	readTree();
	buildTree();
	assignCoordinates();
	setSpacingParameters();
    drawTree();
}

int main(int argc, char **argv) 
{
    //GLUT setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(900,600);
	glutCreateWindow("Algorithm 1");
	
	binarytreeflag = false;
	
	//Display tree and window
	init2D();
	glutDisplayFunc(updateRoutine);
	glutIdleFunc(updateRoutine);
	glutMainLoop();
	
	return 0;
}
