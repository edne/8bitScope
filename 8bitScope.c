#include <GL/freeglut.h>

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include <string.h>
#include <math.h>
#include <time.h>

#define W 64
#define H 64
#define BUF_SIZE W*H

typedef unsigned char uchar;

double f;
uchar buffer[BUF_SIZE];

static void *loop(void *v)
{
    int buf_count=0;

    while(1)
    {
        scanf("%c", &(buffer[buf_count]) );
        //printf("%c", buffer[buf_count] );
        buf_count++;
        buf_count%=BUF_SIZE;
    }

    return NULL;
}

void quad(double x, double y)
{
    glBegin(GL_POLYGON);
    glVertex2f(x+0,y+0);
    glVertex2f(x+1,y+0);
    glVertex2f(x+1,y+1);
    glVertex2f(x+0,y+1);
    glVertex2f(x+0,y+0);
    glEnd();
}

void display()
{
    double c;

    glClear(GL_COLOR_BUFFER_BIT);
    
    int r,i;
    for(r=0; r<=W/2; r++)
    {
        for(i=0; i<r*2+1; i++)
        {
            c = (double)buffer[r*(W/2)+i]/0xFF;
            glColor3f(c, c, c);
            
            quad( H/2 + r, W/2 - r + i);
            quad( H/2 - r, W/2 - r + i);
            quad( H/2 - r + i, W/2 + r);
            quad( H/2 - r + i, W/2 - r);
        }
    }

    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glTranslatef(-1, 1, 0);
    glScalef(2.0/W, -2.0/H, 1);
}

int fullscreen = 0;
void special(int key, int x, int y)
{
    if(key==GLUT_KEY_F11)
    {
        if(fullscreen)
            glutReshapeWindow(W,H);
        else
            glutFullScreen();
        
        fullscreen = !fullscreen;
    }
}

int main(int argc, char **argv)
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, loop, 0);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(W, H);

    glutCreateWindow("8bitScope");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);

    glutMainLoop();

    return 0;
}

