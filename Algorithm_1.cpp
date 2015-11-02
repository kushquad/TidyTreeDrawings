// Assignment 1 - Algorithm 1
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
	int next_x[N];

    for(int i=0;i<N; i++)
    	next_x[i] = 1;

	//Initialize walk from root
	node* current = &tree[0];
	current->status = 0;
	//Indicates when to stop traversal - when we rereach root
	bool flag = true;

	while (flag) 
	{
		if(current->status==0)
		{
			current->x = next_x[current->height];
			current->y = 2*current->height + 1;
			next_x[current->height] += 2;
			for(int i=0; i<current->children.size();i++)
				tree[current->children[i]].status = 0;
			current->status = 1;
		}

		else if(current->status>=1 && current->status<=current->children.size())
		{
			current->status++;
			current = &tree[current->children[current->status-2]];
		}

		else
		{
			if(current->parent!=-1)
					current = &tree[current->parent];
			//If parent==-1, means we have reached NIL
			else
				flag = false;
		}
	}
}

void updateRoutine(void)
{
	//Read, build and assign coordinates to tree nodes according to Algorithm 2
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

	//Display tree and window
	init2D();
	glutDisplayFunc(updateRoutine);
	glutIdleFunc(updateRoutine);
	glutMainLoop();
	
	return 0;
}
