#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#include <unistd.h>

#include <string>
#include <sstream>
#include <fstream>

//Piano spacing constants
#define KEY_WIDTH 19.0/140.0
#define BLACK_KEY_WIDTH 13.0/140.0
#define KEY_SPACE 1.0/140.0

using namespace std;

//Number of notes in the buffer
int notecount = 0;

//Clear note buffer and reset notecount
void clearFile()
{
	ofstream file;
	file.open("notes.txt",fstream::out);
	file.close();
	notecount = 0;
}

//Register key and add to buffer
//The computation (n/24.0)/notecount is used to divide the piano
//with every successive note to determine which tree node is accessed
void addKey(int n)
{
	notecount+=1;
	ofstream file;
	file.open("notes.txt",fstream::app);
	file<<(int)(n/(24.0/notecount));
	file<<" ";
	file.close();
}

/* Invoke Python module for playing a sound
   of specified sound and frequency using command line arguments

   Frequency computation was taken from the wikipedia page
   on piano frequencies

   Composition of strings was required to send a system call
   to invoke the Python program
*/
float playTone(int note)
{
	int freq = 1+pow(2.0,(note-49.0)/12.0)*440.0;
	char freqs[5];
	sprintf(freqs,"%d",freq);
	string s1 = "python cgbeep.py ";
	string s2(freqs);
	string s3 = " 0.5";
	system((s1+s2+s3).c_str());
	addKey(note-64);
}

//What to do when the key is released
void releaseKey(unsigned char key, int x, int y)
{    
    glutPostRedisplay();
}

void drawBlackKey()
{
    //Draw Black Key Rectangle.
    glBegin(GL_QUADS);
	    glVertex3f(-1.0,1.0,0.0);
	    glVertex3f(-1.0,-0.333,0.0);
		glVertex3f(-(1 - BLACK_KEY_WIDTH),-0.333,0.0);
		glVertex3f(-(1 - BLACK_KEY_WIDTH),1.0,0.0);
    glEnd();
}

void drawAllBlackKeys()
{
    //Reset MODELVIEW_MATRIX
    glLoadIdentity();
    
    //Set color to BLACK.
    glColor3f(0.0f, 0.0f, 0.0f);
    
    //Locate key position and draw.
    glTranslatef(BLACK_KEY_WIDTH, 0.0f, 0.0f);
    for(int i = 1; i < 11; i++)
    {
        drawBlackKey();
        glTranslatef( KEY_WIDTH + KEY_SPACE, 0.0f, 0.0f);
        if (i%5 == 2 || i%5 == 0)
            glTranslatef( KEY_WIDTH + KEY_SPACE, 0.0f, 0.0f);   
	}	
}

void drawWhiteKey()
{
    //Draw White Key Rectangle.
    glBegin(GL_QUADS);
	    glVertex3f(-1.0,1.0,0.0);
	    glVertex3f(-1.0,-1.0,0.0);
		glVertex3f(-(1 - KEY_WIDTH),-1.0,0.0);
		glVertex3f(-(1 - KEY_WIDTH),1.0,0.0);
    glEnd();
	
}

void drawAllWhiteKeys()
{
    //Set color to WHITE.
    glColor3f(1.0f, 1.0f, 1.0f);
    
    //Locate key position and draw.
    for(int i=0;i<14;i++)
	{
	    drawWhiteKey();
	    glTranslatef(KEY_WIDTH + KEY_SPACE, 0.0f, 0.0f);	    
	}
}

void reddenWhiteKey(int n)
{
    //Reset MODELVIEW_MATRIX
    glLoadIdentity();
    
    //Set color to RED.
    glColor3f(1.0f, 0.0f, 0.0f);
    
    //Locate key position.
    for(int i=0;i<n-1;i++)
    	glTranslatef(KEY_WIDTH + KEY_SPACE, 0.0f, 0.0f);
    
    //Draw Keys after reddened
    drawWhiteKey();
    drawAllBlackKeys();
    
    // Load to Screen.
	glutSwapBuffers();
}
 
void reddenBlackKey(int n)
{
    //Reset MODELVIEW_MATRIX
    glLoadIdentity();
    
    //Set color to RED.
    glColor3f(1.0f, 0.0f, 0.0f);
    
    //Locate key position.
    glTranslatef(BLACK_KEY_WIDTH, 0.0f, 0.0f);
    for(int i=1;i<n;i++)
    {       
        glTranslatef(KEY_WIDTH + KEY_SPACE, 0.0f, 0.0f);
        if (i%5==2 || i%5==0)
            glTranslatef(KEY_WIDTH + KEY_SPACE, 0.0f, 0.0f);   
	}
	
	//Draw Key after reddened
	drawBlackKey();
	
	//Load to Screen.
	glutSwapBuffers();
}

//Exhaustive mapping of keyboard keys to tones to play
//The 24-key subset is offset from the 64th key in a grand piano          
void handleKeyPress( unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27: exit(0);
        case '\t':
        		reddenWhiteKey(1);
        		playTone(64);
	            break;
	    case '1':
	    		reddenBlackKey(1);
	    		playTone(65);
	            break;
	    case 'q':
	    		reddenWhiteKey(2);
	    		playTone(66);
	            break;
	    case '2':
	    		reddenBlackKey(2);
	    		playTone(67);
	            break;
	    case 'w':
	            reddenWhiteKey(3);
	            playTone(68);
	            break;
	    case 'e':
	    		reddenWhiteKey(4);
	    		playTone(69);
	            break;
	    case '4':
	    		reddenBlackKey(3);
	    		playTone(70);
	            break;
	    case 'r':
	    		reddenWhiteKey(5);
	    		playTone(71);
	            break;
	    case '5':
	    		reddenBlackKey(4);
	    		playTone(72);
	            break;
	    case 't':
	    		reddenWhiteKey(6);
	    		playTone(73);
	            break;
	    case '6':
	    		reddenBlackKey(5);
	    		playTone(74);
	            break;
	    case 'y':
	    		reddenWhiteKey(7);
	    		playTone(75);
	            break;
	    case 'u':
	    		reddenWhiteKey(8);
	    		playTone(76);
	            break;
	    case '8':
	    		reddenBlackKey(6);
	    		playTone(77);
	            break;
	    case 'i':
	    		reddenWhiteKey(9);
	    		playTone(78);
	            break;
	    case '9':
	    		reddenBlackKey(7);
	    		playTone(79);
	            break;
	    case 'o':
	    		reddenWhiteKey(10);
	    		playTone(80);
	            break;
	    case 'p':
	    		reddenWhiteKey(11);
	    		playTone(81);
	            break;
	    case '-':
	    		reddenBlackKey(8);
	    		playTone(82);
	            break;
	    case '[':
	    		reddenWhiteKey(12);
	    		playTone(83);
	            break;
	    case '=':
	    		reddenBlackKey(9);
	    		playTone(84);
	            break;
	    case ']':
				reddenWhiteKey(13);
				playTone(85);
	            break;
	    case 8:
	    		reddenBlackKey(10);
	    		playTone(86);
	            break;
	    case '\\':
	    		reddenWhiteKey(14);
	    		playTone(87);
	            break;
	    case 13:
	    		clearFile();
	    		break;
	}
}

        
void renderScene(void)
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);
    
    //Reset MODEL_MATRIX
    glLoadIdentity();    
    
	//Draw Keys.
	drawAllWhiteKeys();
	drawAllBlackKeys();

    //Load to Screen.
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
	//GLUT setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1200,500);
	glutCreateWindow("Piano");

	//Clear notes pressed buffer
	clearFile();

	glutDisplayFunc(renderScene);

	//Register keyboard callbacks, ensure only one press processed
	glutKeyboardFunc(handleKeyPress);
	glutKeyboardUpFunc(releaseKey);	
	glutIgnoreKeyRepeat(1);	
	
	glutMainLoop();
	
	return 1;
}
