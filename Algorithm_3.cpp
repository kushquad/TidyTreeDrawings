// Assignment 1 - Algorithm 3 (Wetherell and Shannon)
// Tidy Tree Drawing
// Combines both Algorithm 1 and 2 to meet both aesthetics (most of the time)
// - minimum width tree drawing
// - parent centered around children

// Modified Algorithm 3 in assignCoordinatesModified() at end

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "treedraw.h"

using namespace std;

//Algorithm 3 for coordinate assignment
//Satisfies both aesthetics while minimizing paper width most of the time
void assignCoordinates()
{
	//Variables required by algorithm
	int modifier[max_height+1];
	int next_pos[max_height+1];

	for(int i=0;i<=max_height;i++)
		{modifier[i] = 0, next_pos[i] = 1;}

	//Initiate traversal from tree
	node* current = &tree[0];
	current->status = FIRST_VISIT;
	//Flag is to indicate when NIL has been reached i.e when parent=-1
	bool flag = true;

	while(flag)
	{
		//Iterative traversal through status markers
		switch(current->status)
		{
			//POST-ORDER TRAVERSAL
			case FIRST_VISIT:
								//Move to LEFT section from node
								current->status = LEFT_VISIT;
								if(current->children.size()>0)
								{
									current = &tree[current->children[0]];
									current->status = FIRST_VISIT; 
								}
								break;
			case LEFT_VISIT:
								//LEFT section visited, now visit RIGHT section
								current->status = RIGHT_VISIT;
								if(current->children.size()>1)
								{
									current = &tree[current->children[1]];
									current->status = FIRST_VISIT;
								}
								break;
			case RIGHT_VISIT:
								//After RIGHT, two possibilities arise
								//We are on a leaf or an internal node

								//We need to adjust the place of this node accordingly
								int h = current->height;
								bool is_leaf = (current->children.size()==0);
								int place;

								//Use algorithm 1's concept if a leaf
								if(is_leaf)
									place = next_pos[h];
								//If only one child, keep child below parent
								else if(current->children.size()<2)
									place = tree[current->children[0]].x;
								//If both children, take average of positions and assign
								else
									place = (tree[current->children[0]].x + tree[current->children[1]].x)/2;
								
								//Compute modifier for second traversal
								modifier[h] = max(modifier[h], next_pos[h] - place);
								if(is_leaf)
									current->x = place;
								else
									current->x = place + modifier[h];
								current->modifier = modifier[h];
								
								//Update next_pos for current height
								next_pos[h] = current->x + 2;
								
								//Go up level and visit parent
								if(current->parent!=-1)
									current = &tree[current->parent];
								//If parent==-1, means we have reached NIL
								else
									flag = false;
								break;
		}
	}

	//Set traversal variables to initial values for second traversal
	current = &tree[0];
	current->status = FIRST_VISIT;
	int modifier_sum = 0;
	flag = true;

	while(flag)
	{
		//POST-ORDER traversal
		switch(current->status)
		{
			case FIRST_VISIT:
								//Pass down modifier_sum down the tree
								//Set x,y coordinates for current node
								current->x = current->x + modifier_sum;
								modifier_sum = modifier_sum + current->modifier;
								current->y = 2*current->height + 1;
								//Move to left section of tree
								current->status = LEFT_VISIT;
								if(current->children.size()>0)
								{
									current = &tree[current->children[0]];
									current->status = FIRST_VISIT;
								}
								break;
			case LEFT_VISIT:
								//Same as previous traversal, move from left to right
								current->status = RIGHT_VISIT;
								if(current->children.size()>1)
								{
									current = &tree[current->children[1]];
									current->status = FIRST_VISIT;
								}
								break;
			case RIGHT_VISIT:
								//Traverse upwards
								//Do not pass up the same modifier sum up the tree
								modifier_sum = modifier_sum - current->modifier;
								if(current->parent!=-1)
									current = &tree[current->parent];
								else
									flag = false;
								break;
		}
	}
}

//Algorithm 3 for coordinate assignment (modified as per paper)
//Satisfies both aesthetics while minimizing paper width
void assignCoordinatesModified()
{
	//Variables required by algorithm
	int modifier[max_height+1];
	int next_pos[max_height+1];

	for(int i=0;i<=max_height;i++)
		{modifier[i] = 0, next_pos[i] = 1;}

	//Initiate traversal from tree
	node* current = &tree[0];
	current->status = FIRST_VISIT;
	//Flag is to indicate when NIL has been reached i.e when parent=-1
	bool flag = true;

	while(flag)
	{
		//Iterative traversal through status markers
		switch(current->status)
		{
			//POST-ORDER TRAVERSAL
			case FIRST_VISIT:
								//Move to LEFT section from node
								current->status = LEFT_VISIT;
								if(current->children.size()>0)
								{
									current = &tree[current->children[0]];
									current->status = FIRST_VISIT; 
								}
								break;
			case LEFT_VISIT:
								//LEFT section visited, now visit RIGHT section
								current->status = RIGHT_VISIT;
								if(current->children.size()>1)
								{
									current = &tree[current->children[1]];
									current->status = FIRST_VISIT;
								}
								break;
			case RIGHT_VISIT:
								//After RIGHT, two possibilities arise
								//We are on a leaf or an internal node

								//We need to adjust the place of this node accordingly
								int h = current->height;
								bool is_leaf = (current->children.size()==0);
								int place;

								//Use algorithm 1's concept if a leaf
								if(is_leaf)
									place = next_pos[h];
								//If only one child, keep child below parent
								else if(current->children.size()<2)
									place = tree[current->children[0]].x;
								//If both children, take average of positions and assign
								else
									place = (tree[current->children[0]].x + tree[current->children[1]].x)/2;
								
								//Compute modifier for second traversal
								modifier[h] = max(modifier[h], next_pos[h] - place);
								if(is_leaf)
									current->x = place;
								else
									current->x = place + modifier[h];
								current->modifier = modifier[h];
								
								//Update next_pos for current height
								next_pos[h] = current->x + 2;
								
								//Go up level and visit parent
								if(current->parent!=-1)
									current = &tree[current->parent];
								//If parent==-1, means we have reached NIL
								else
									flag = false;
								break;
		}
	}

	//Set traversal variables to initial values for second traversal
	//MODIFICATIONS BEGIN HERE
	for(int i=0;i<=max_height;i++)
		next_pos[i] = 1;
	current = &tree[0];
	current->status = FIRST_VISIT;
	int modifier_sum = 0;
	flag = true;

	while(flag)
	{
		switch(current->status)
		{
			case FIRST_VISIT:
								//Pass down modifier_sum down the tree
								modifier_sum = modifier_sum + current->modifier;
								current->status = LEFT_VISIT;
								//Move to left section of tree
								if(current->children.size()>0)
								{
									current = &tree[current->children[0]];
									current->status = FIRST_VISIT;
								}
								break;
			case LEFT_VISIT:
								//Main modifications here
								//Discussed on Page 519 of paper
								current->x = min(next_pos[current->height], 
									current->x+modifier_sum-current->modifier);
								if(current->children.size()>0)
									current->x = max(current->x, tree[current->children[0]].x+1);
								if(current->parent!=-1)
									if(tree[current->parent].status==RIGHT_VISIT)
										current->x = max(current->x, tree[current->parent].x+1);
								//This helps eliminate worst case drawings
								next_pos[current->height] = current->x + 2;
								current->y = 2*current->height + 1;

								current->status = RIGHT_VISIT;
								if(current->children.size()>1)
								{
									current = &tree[current->children[1]];
									current->status = FIRST_VISIT;
								}
								break;
			case RIGHT_VISIT:
								//Traverse upwards
								//Do not pass up the same modifier sum up the tree again
								modifier_sum = modifier_sum - current->modifier;
								if(current->parent!=-1)
									current = &tree[current->parent];
								else
									flag = false;
								break;
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

int main(int argc, char* argv[])
{
	//GLUT setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(900,600);
	glutCreateWindow("Algorithm 3");

	binarytreeflag = true;
	
	//Display tree and window
	init2D();
	glutDisplayFunc(updateRoutine);
	glutIdleFunc(updateRoutine);
	glutMainLoop();

	return 0;
}

