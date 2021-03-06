#include <GL/freeglut.h>

#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <time.h>


typedef unsigned char uchar;


//-- PROTOTYPES
void mono(int, int);
void rgb(int, int);

void concentric();
void vertical();
//--


//-- WHITE MAGIC
#define W 64
#define H 64

uchar _BUFFER[W*H];

// getter
#define BUFFER(i) (double) _BUFFER[i % (W*H)] / 0xFF

void (*_color_modes[])(int, int) = {
   *mono, *rgb
};

void (*_drawing_modes[])() = {
   *concentric, *vertical
};

enum {
    COLOR_index,
    GREYSCALE_index,
    DRAWING_index,
    SEPARE_index
};

double color_weights[] = {1, 1, 1};
int options[] = {0, 0, 0};


#define RW color_weights[0]
#define GW color_weights[1]
#define BW color_weights[2]

#define GREYSCALE options[GREYSCALE_index]

#define COLOR(x, y) (*   _color_modes[ options[COLOR_index]   ] )(x, y)
#define DRAW()      (* _drawing_modes[ options[DRAWING_index] ] )()

#define SEPARE options[SEPARE_index]
//--


void init_options()
{
    RW=1; GW=1; BW=1;
    if(!GREYSCALE)
    {
        // color_weights[rand()%3] = 0;
        if(rand()%2)
            color_weights[rand()%3] = rand() / RAND_MAX;
    }

    int i;
    for(i=0; i<4; i++)
        options[i] = rand() % 2;
}


//-- PRIMITIVES
void quad(double x, double y)
{
    #define DV(dx, dy) glVertex2f(x + dx,\
                                  y + dy);
    glBegin(GL_POLYGON);
    DV(0,0); DV(1,0);
    DV(1,1); DV(0,1);
    glEnd();
}
//--


static void *loop(void *v)
{
    int i = 0;
    while(1)
    {
        scanf("%c", &(_BUFFER[i]) );
        //printf("%c", _BUFFER[buf_count] );
        i++;
        i %= W*H;
    }

    return NULL;
}


void rgb(int x, int y)
{
    glColor3f(RW * (BUFFER(x*(W/2) + y)),
              GW * (BUFFER(x*(W/2) + y +W)),
              BW * (BUFFER(x*(W/2) + y +2*W)));
}


void mono(int x, int y)
{
    double c = BUFFER(x*(W/2) + y);
    glColor3f(RW*c, GW*c, BW*c);
}


void concentric()
{
    int r, _r, i;
    for(r=0; r<=W/2; r++)
    {
        for(i=0; i<r*2+1; i++)
        {
            COLOR(r, i);

            _r = r + SEPARE * BUFFER(r)*W/2;
            quad( H/2 + _r, W/2 - r + i);
            quad( H/2 - _r, W/2 - r + i);
            quad( H/2 - r + i, W/2 + _r);
            quad( H/2 - r + i, W/2 - _r);
        }
    }
}


void vertical()
{
    int x, _x , y;
    for(x=0; x<W; x++)
    {
        for(y=0; y<H; y++)
        {
            COLOR(x, y);

            _x = x + SEPARE * BUFFER(x)*W;
            quad(_x, y);
            quad(W-1- _x, y);
        }
    }
}



void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    DRAW();

    glutSwapBuffers();
    glutPostRedisplay();

    if(rand() % 180 == 0) {
      init_options();
    }
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

    srand(time(NULL));
    init_options();

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
