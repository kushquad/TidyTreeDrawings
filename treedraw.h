#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>

int spacing;									//  Spacing of between nodes.
int r;      									//  Radius of circle.
float m, c; 									//  Line constants.

//Structure for tree node
struct node
{
	int data;
	int parent;
	std::vector<int> children;
	int height; 				// which level the node is on
	int modifier;

	int x; 						// x coordinate
	int y; 						// y coordinate
	int status; 				// 1 - First visit, 2 - Left Visit, 3 - Right Visit (0->len(child) for algo1)
	node() : data(0), x(-1), y(-1), status(0), height(0) {}
};	

//Global data structures and variables
//Main data structure is dynamic array (vector) tree
//Parent vector is passed from main to the tree builder routine
//N is the size of tree, taken at runtime
//max_height is globally used both for coordinate assignment and drawing functions
std::vector<node> tree;
std::vector<int> parent;
int N;
int max_height;
int max_width;

//Enumeration for status field in node structure
enum {NIL, FIRST_VISIT, LEFT_VISIT, RIGHT_VISIT};

//Required initializations for a 2D workspace
void init2D()
{ 
	//Black background
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//Set working matrix to projection for loading identity matrix to specify non-rotated
	//orthogonal plane
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Orthogonally defined clipped plane
	gluOrtho2D(0.0, 900.0, 0.0, 600.0);
	//Reset working matrix to MODELVIEW to work with the tree
	glMatrixMode(GL_MODELVIEW);
}

//Called by drawCircle function to plot a point in 8 symmetric sections
//drawCircle only executes midpoint algorithm on an octant
//This helps replicate all the octants to form a circle 
void plot8symmetry(int x, int y, int xc, int yc)
{
	//Set color to blue
	glColor3f(0,0,1);
	glBegin(GL_POINTS);
	    glVertex2i(x+xc,y+yc);
	    glVertex2i(-x+xc,y+yc);
	    glVertex2i(x+xc,-y+yc);
	    glVertex2i(-x+xc,-y+yc);
	    glVertex2i(y+xc,x+yc);
	    glVertex2i(-y+xc,x+yc);
	    glVertex2i(y+xc,-x+yc);
	    glVertex2i(-y+xc,-x+yc);
	glEnd();
}

void drawCircle(int xc, int yc, int r)
{
	//Start point is (0,r) and decision parameter is set.
	int x = 0, y = r; 
	int d = 1-r;
	
	//Plot the first point
	plot8symmetry(x, y, xc, yc);
	
	//  Loop to cover points from y axis to a y where circle's slope = 1 (to exploit 8-way symmetry)
	while(y>=x)	        
	{
		if(d < 0)				//  If d < 0, choose EAST point
			d += 2*x + 3;   	//  Update rule using first derivative of implicit equation of circle.
		else					//  SOUTHEAST point chosen
		{
			d += 2*(x-y) + 5;
			y--;
		}
		x++;
		//Plot the new generated pointS
		plot8symmetry(x, y, xc, yc);
	}
}

void drawline(int xleft, int yleft, int xright, int yright)
{
    //  Set color
	glColor3f(0,1,1);

	if (xleft == xright )
	{
	    int count = yright - yleft;
	    int add;
	    if ( yright > yleft )
	        add = 1;	        
	    else
	        add = -1;
	    
	    count = abs(count);
	    
	    while ( count-- )
	    {
	        glBegin(GL_POINTS);
			    glVertex2i(xleft, yleft);
		    glEnd();
		    yleft += add;
		}
    }
	else
	{
	    //  Straight Line constants.    
	    m = (float)(yright-yleft) / (float)(xright-xleft);
	    c = yleft - (m*xleft);
	    
	    
	    float yexact, xexact;	
	    int x, y;	
	    if( abs(m) <= 1 )
	    {
	        x = xleft;
		    while( x <= xright )
		    {
		        yexact = m*x + c;
		        y = round(yexact);
		        glBegin(GL_POINTS);
			        glVertex2i(x, y);
		        glEnd();
		        x++;
		    }
		
		
	    }
	    else
	    {
		    y = yleft;
		    while( y <= yright )
		    {
			    xexact = (y/m) - (c/m);
			    x = round(xexact);
			    glBegin(GL_POINTS);
				    glVertex2i(x, y);
			    glEnd();
			    y++;
		    }
		 }
    }
}

void drawLine(int xleft, int yleft, int xright, int yright)
{
    //  Set color
	glColor3f(0,1,1);
	
	//  If vertical line.
	if (xleft == xright )
	{
	    int count = yright - yleft;
	    int add;
	    if ( yright > yleft )
	        add = 1;	        
	    else
	        add = -1;
	    
	    count = abs(count);
	    
	    while(count--)
	    {
	        glBegin(GL_POINTS);
			    glVertex2i(xleft, yleft);
		    glEnd();
		    yleft += add;
		}
    }

	else
	{
	    //  Straight Line constants.
	    int dy = (yright-yleft);
	    int dx = (xright-xleft); 
	   
	    int add = 1;
	    
	    int d, incrE, incrNE;

	    int x = xleft, y = yleft;

	   	if(dy < 0)
	    {
	        std::swap(yleft, yright);
	    	dy = -dy;
	    	x = xright;
	    	add = -1;
	    }

	    d = 2*dy - dx;
	    incrE = 2 * dy;
	    incrNE = 2 * (dy - dx);

	    if(abs(dy/dx) <= 1)
	    {
	        x = xleft;
		    while( x <= xright )
		    {
		        if ( d <= 0 )
		        {
		            d += incrE;
		        }
		        else
		        {
		            d += incrNE;
		            y += add;
		        }
		        glBegin(GL_POINTS);
			        glVertex2i(x, y);
		        glEnd();
		        x++;
		    }
	    }

	    else
	    {
	    	d = 2*dx - dy;
	    	incrE = 2 * dx;
	    	incrNE = 2 * (dx - dy);

		    y = yleft;
		    while( y <= yright )
		    {
			    if ( d < 0 )
		        {
		            d += incrE;
		        }
		        else
		        {
		            d += incrNE;
		            x += add;
		        }
		        glBegin(GL_POINTS);
			        glVertex2i(x, y);
		        glEnd();
		        y++;
		    }
		 }
    }
}

//Line drawing from node to node
//Calculates the periphery points of both nodes to draw line between
void linen2n(int x1, int y1, int x2, int y2)
{
    float xleft, xright, yleft, yright;
    
    //Euclidean distance between two nodes.
    float dist = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
    
    //Locate Perimeter point of node to draw line from.
    xleft = x1 + r*(x2-x1)/dist;
    yleft = y1 + r*(y2-y1)/dist;
    
    xright = x2 + r*(x1-x2)/dist;
    yright = y2 + r*(y1-y2)/dist;
	
	//Draw line.
	drawLine(xleft, yleft, xright, yright);	
}

//Horizontal = vertical spacing
//r is radius of circle used for nodes
//Reassign coordinates based on spacing parameters
void setSpacingParameters()
{
	for(int i=0; i<N; i++)
    	max_width = std::max(max_width, tree[i].x);
	
	spacing = std::min(std::min(40, 300 / (max_height + 1)), 900/ (max_width+1));
	r = spacing / 4;

	for(int i=0;i<N;i++)
	{
		tree[i].x = spacing*tree[i].x;
		tree[i].y = 600 - spacing*tree[i].y;
	}
}

//Main tree drawing function
//Composed of circle for node and line from node to node
void drawTree(void) 
{
	glClear(GL_COLOR_BUFFER_BIT);
    
    int xl, xr, yl, yr;
    drawCircle(tree[0].x, tree[0].y, r);
    
    for(int i=1; i<N; i++)
    {
        xl = tree[i].x;
        yl = tree[i].y;
        
        xr = tree[tree[i].parent].x;        
        yr = tree[tree[i].parent].y;
        
        drawCircle(xl, yl, r);
       
        if(xl<xr)
          	linen2n(xl, yl, xr, yr);
        else
            linen2n(xr, yr, xl, yl);        
    }
    
	glutSwapBuffers();
}

//Prompt at runtime to specify tree by size and parent array
void readTree()
{
	std::ifstream file;
	while(!file)
		file.open("notes.txt",std::fstream::in);

	parent.erase(parent.begin(),parent.end());
	int temp = -1;
	while(!file.eof())
	{
		parent.push_back(temp);
		file>>temp;
	}
	file.close();
	N = parent.size();
}

//Build internal representation of tree from parental array
//by populating tree vector
void buildTree()
{
	tree.resize(N);
	for(int i=0;i<N;i++)
	{
		if(parent[i]==-1)
		{
			tree[i].height = 0;
			tree[i].parent = -1;
			continue;
		}

		//We get around using pointers by using the concept of parent and child indexes
		//to traverse the tree
		tree[i].parent = parent[i];
		tree[parent[i]].children.push_back(i);
		
		//Update height of new node w.r.t to parent
		//Also store max height so far
		tree[i].height = tree[parent[i]].height + 1;
		max_height = std::max(tree[i].height, max_height);
	}
}

//Command-line output for debugging
void printTree()
{
	for(int i=0;i<N;i++)
	{
		std::cout<<"Node "<<i<<" : ";
		std::vector<int> tempv = tree[i].children;
		if(tempv.size()==0)
			std::cout<<"Leaf node. ";
		else
			for(int j=0;j<tempv.size();j++)
				std::cout<<tempv[j]<<" ";
		std::cout<<"Height : "<<tree[i].height;
		std::cout<<" x = "<<tree[i].x;
		std::cout<<" y = "<<tree[i].y;
		std::cout<<"\n";
	}
}