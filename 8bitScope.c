#include <GL/freeglut.h>

#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef unsigned char uchar;


// PROTOTYPES
void grey(int, int);
void rgb(int, int);

void concentric();
//


// GLOBALS
#define W 64
#define H 64
uchar BUFFER[800*600];  // more memory!

void (*color_func)(int, int) = *rgb;
void (*drawing_func)(int, int) = *concentric;
//


// PRIMITIVES
void quad(double x, double y)
{
    glBegin(GL_POLYGON);
    glVertex2f(x+0, y+0);
    glVertex2f(x+1, y+0);
    glVertex2f(x+1, y+1);
    glVertex2f(x+0, y+1);
    glVertex2f(x+0, y+0);
    glEnd();
}
//


static void *loop(void *v)
{
    int i = 0;
    while(1)
    {
        scanf("%c", &(BUFFER[i]) );
        //printf("%c", BUFFER[buf_count] );
        i++;
        i %= W*H;
    }

    return NULL;
}


void rgb(int x, int y)
{
    double cr,cg,cb;
    cr = (double)BUFFER[x*(W/2)+y]/0xFF;
    cg = (double)BUFFER[x*(W/2)+y +W]/0xFF;
    cb = (double)BUFFER[x*(W/2)+y +2*W]/0xFF;
    glColor3f(cr, cg, cb);
}


void grey(int x, int y)
{
    double c = (double)BUFFER[x*(W/2)+y]/0xFF;
    glColor3f(c, c, c);
}


void concentric()
{
    int r, i;
    for(r=0; r<=W/2; r++)
    {
        for(i=0; i<r*2+1; i++)
        {
            (*color_func)(r, i);

            quad( H/2 + r, W/2 - r + i);
            quad( H/2 - r, W/2 - r + i);
            quad( H/2 - r + i, W/2 + r);
            quad( H/2 - r + i, W/2 - r);
        }
    }
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    concentric();

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


void special(int key, int x, int y)
{
    static int fullscreen = 0;
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
