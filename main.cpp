#include <stdio.h>
#include <gl/glut.h>
#define KEY_ESC 27 /* GLUT doesn't supply this */

int fullscreen = 0;
int mouseDown = 0;

float xrot = 0;
float yrot = 305;

float xdiff = 100;
float ydiff = 100;

float tra_x = 0;
float tra_y = 0;
float tra_z = 0;

float grow_shrink = 80;
float resize_f = 1;

void drawBox()
{

    glTranslatef(tra_x, tra_y, tra_z);

#pragma region MAIN SHAPE
    // BACK VIEW
    glPushMatrix();
    glColor4f(.75, .75, .75, 1);
    glTranslatef(-3, 0, 0);
    glScalef(.05, 5, 3);
    glutSolidCube(1.0);
    glPopMatrix();

    // LEFT VIEW
    glPushMatrix();
    glColor4f(.75, .75, .75, 1);
    glTranslatef(-2, 0, -1.5);
    glScalef(2, 5, .05);
    glutSolidCube(1.0);
    glPopMatrix();

    // RIGHT VIEW
    glPushMatrix();
    glColor4f(.75, .75, .75, 1);
    glTranslatef(-2, 0, 1.5);
    glScalef(2, 5, .05);
    glutSolidCube(1.0);
    glPopMatrix();

    // TOP VIEW
    glPushMatrix();
    glColor4f(.75, .75, .75, 1);
    glTranslatef(-2, 2.5, 0);
    glScalef(2, .05, 3);
    glutSolidCube(1.0);
    glPopMatrix();

    // BUTTON VIEW
    glPushMatrix();
    glColor4f(.75, .75, .75, 1);
    glTranslatef(-2, -2.4, 0);
    glScalef(2, .2, 3);
    glutSolidCube(1.0);
    glPopMatrix();

    // MID HORIZON VIEW 2
    glPushMatrix();
    glColor4f(.6, .6, .6, 1);
    glTranslatef(-2, 1.8, 0);
    glScalef(1.5, .05, 3);
    glutSolidCube(1.0);
    glPopMatrix();

    // MID HORIZON VIEW 2
    glPushMatrix();
    glColor4f(.6, .6, .6, 1);
    glTranslatef(-2, 1.1, 0);
    glScalef(1.5, .05, 3);
    glutSolidCube(1.0);
    glPopMatrix();

    // MID HORIZON VIEW 3
    glPushMatrix();
    glColor4f(.7, .7, .7, 1);
    glTranslatef(-2, .5, 0);
    glScalef(2, .05, 3);
    glutSolidCube(1.0);
    glPopMatrix();

    // MID VERTICAL VIEW
    glPushMatrix();
    glColor4f(.6, .6, .6, 1);
    glTranslatef(-2.2, -1, 0);
    glScalef(1.6, 2.9, .05);
    glutSolidCube(1.0);
    glPopMatrix();
#pragma endregion

#pragma region DETAIL

#pragma region BOOK

    // ORDERED FROM LEFT TO RIGHT

#pragma region HORIZONTAL BOOK
    //  BOOK 1
    glPushMatrix();
    glColor4f(.7, .7, .7, 1);
    glTranslatef(-1.5, 1.85, .55);
    glScalef(.2, .1, 1);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 2
    glPushMatrix();
    glColor4f(.8, .8, .8, 1);
    glTranslatef(-1.5, 1.97, .55);
    glScalef(.2, .15, .8);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 3
    glPushMatrix();
    glColor4f(.7, .5, .9, 1);
    glTranslatef(-1.5, 1.85, -1);
    glScalef(.2, .1, 1);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 4
    glPushMatrix();
    glColor4f(.1, .7, .8, 1);
    glTranslatef(-1.5, 1.95, -1);
    glScalef(.2, .1, .8);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 5
    glPushMatrix();
    glColor4f(.1, .5, .8, 1);
    glTranslatef(-1.5, 2.05, -1);
    glScalef(.2, .1, .8);
    glutSolidCube(1.0);
    glPopMatrix();

#pragma endregion

#pragma region VERTICAL BOOK
    //  BOOK 1
    glPushMatrix();
    glColor4f(0, 0, 1, 1);
    glTranslatef(-2.3, 2.1, 1.3);
    glScalef(.8, .6, .2);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 2
    glPushMatrix();
    glColor4f(1, 0, 0, 1);
    glTranslatef(-2.3, 2, 1.15);
    glScalef(.8, .4, .15);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 3
    glPushMatrix();
    glColor4f(0, 0, .4, 1);
    glTranslatef(-2.3, 2, 1);
    glScalef(.8, .4, .15);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 4
    glPushMatrix();
    glColor4f(0, .4, 0, 1);
    glTranslatef(-2.3, 2.05, .8);
    glScalef(.8, .45, .1);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 5
    glPushMatrix();
    glColor4f(.4, .4, .4, 1);
    glTranslatef(-2.3, 2, .65);
    glScalef(.8, .4, .15);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 6
    glPushMatrix();
    glColor4f(.4, 0, .4, 1);
    glTranslatef(-2.3, 2, .5);
    glScalef(.8, .4, .1);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 7
    glPushMatrix();
    glColor4f(.2, .1, .4, 1);
    glTranslatef(-2.3, 2, .45);
    glScalef(.8, .4, .06);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 8
    glPushMatrix();
    glColor4f(.1, .3, .5, 1);
    glTranslatef(-2.3, 2, .4);
    glScalef(.8, .4, .06);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 9
    glPushMatrix();
    glColor4f(.2, .7, .5, 1);
    glTranslatef(-2.3, 2, .35);
    glScalef(.8, .4, .06);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 10
    glPushMatrix();
    glColor4f(.4, .4, .4, 1);
    glTranslatef(-2.3, 2, .2);
    glScalef(.8, .4, .15);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 11
    glPushMatrix();
    glColor4f(.8, .1, .9, 1);
    glTranslatef(-2.3, 2, .05);
    glScalef(.8, .4, .15);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 12
    glPushMatrix();
    glColor4f(.1, .1, .9, 1);
    glTranslatef(-2.3, 2, -.1);
    glScalef(.8, .4, .15);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 13
    glPushMatrix();
    glColor4f(.9, .1, .1, 1);
    glTranslatef(-2.3, 2.1, -.25);
    glScalef(.8, .6, .15);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 14
    glPushMatrix();
    glColor4f(.9, .9, .1, 1);
    glTranslatef(-2.3, 2.1, -.4);
    glScalef(.8, .6, .15);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 15
    glPushMatrix();
    glColor4f(.5, .9, .1, 1);
    glTranslatef(-2.3, 2, -.55);
    glScalef(.8, .4, .15);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 16
    glPushMatrix();
    glColor4f(.3, .9, .5, 1);
    glTranslatef(-2.3, 2, -.7);
    glScalef(.8, .4, .15);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 17
    glPushMatrix();
    glColor4f(.3, .7, .4, 1);
    glTranslatef(-2.3, 2, -.85);
    glScalef(.8, .4, .15);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 18
    glPushMatrix();
    glColor4f(.6, .2, .5, 1);
    glTranslatef(-2.3, 2.1, -1);
    glScalef(.8, .6, .15);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 19
    glPushMatrix();
    glColor4f(.2, .2, .5, 1);
    glTranslatef(-2.3, 2.1, -1.15);
    glScalef(.8, .6, .15);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 20
    glPushMatrix();
    glColor4f(.2, .2, .2, 1);
    glTranslatef(-2.3, 2, -1.3);
    glScalef(.8, .4, .15);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOOK 21
    glPushMatrix();
    glColor4f(.2, .6, .1, 1);
    glTranslatef(-2.3, 2, -1.43);
    glScalef(.8, .4, .1);
    glutSolidCube(1.0);
    glPopMatrix();
#pragma endregion

#pragma endregion

#pragma region BOARD GAME

    //  BOARD GAME 1
    glPushMatrix();
    glColor4f(1, 0, 0, 1);
    glTranslatef(-1.5, 1.2, 1.3);
    glScalef(.2, .2, .3);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOARD GAME 2
    glPushMatrix();
    glColor4f(1, 0, 0, 1);
    glTranslatef(-1.5, 1.4, 1.3);
    glScalef(.2, .2, .3);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOARD GAME 3
    glPushMatrix();
    glColor4f(1, 0, 0, 1);
    glTranslatef(-1.7, 1.6, 1.3);
    glScalef(.2, .2, .3);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOARD GAME 5
    glPushMatrix();
    glColor4f(.9, 0, 0, 1);
    glTranslatef(-1.5, 1.2, .95);
    glScalef(.2, .2, .3);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOARD GAME 6
    glPushMatrix();
    glColor4f(.9, 0, 0, 1);
    glTranslatef(-1.5, 1.4, .95);
    glScalef(.2, .2, .3);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOARD GAME 7
    glPushMatrix();
    glColor4f(1, 1, 0, 1);
    glTranslatef(-1.7, 1.3, .2);
    glScalef(1, .3, 1);
    glutSolidCube(1.0);
    glPopMatrix();

    //  BOARD GAME 7
    glPushMatrix();
    glColor4f(1, .5, 0, 1);
    glTranslatef(-1.7, 1.6, .2);
    glScalef(1, .3, 1);
    glutSolidCube(1.0);
    glPopMatrix();

#pragma endregion

#pragma region RAIL GLASS DOOR UP

    // UP GLASS DOOR TRAIL 1
    glPushMatrix();
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.2, 2.45, 0);
    glScalef(.02, .02, 3);
    glutSolidCube(1.0);
    glPopMatrix();

    // UP GLASS DOOR TRAIL 2
    glPushMatrix();
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.1, 2.45, 0);
    glScalef(.02, .02, 3);
    glutSolidCube(1.0);
    glPopMatrix();

#pragma endregion

#pragma region RAIL GLASS DOOR DOWN

    // UP GLASS DOOR TRAIL 1
    glPushMatrix();
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.2, .55, 0);
    glScalef(.02, .02, 3);
    glutSolidCube(1.0);
    glPopMatrix();

    // UP GLASS DOOR TRAIL 2
    glPushMatrix();
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.1, .55, 0);
    glScalef(.02, .02, 3);
    glutSolidCube(1.0);
    glPopMatrix();

#pragma endregion

#pragma region GLASS LEFT

    // HANDLE 1
    glPushMatrix();
    glColor4f(.5, .5, .5, 1);
    glTranslatef(-1.19, 1.5, 1.4);
    glScalef(.05, .15, .1);
    glutSolidCube(1.0);
    glPopMatrix();

    // HANDLE 2
    glPushMatrix();
    glColor4f(.8, .8, .8, 1);
    glTranslatef(-1.18, 1.5, 1.4);
    glScalef(.05, .1, .05);
    glutSolidCube(1.0);
    glPopMatrix();

    // LOCK
    glPushMatrix();
    glColor4f(.3, .3, .3, 1);
    glTranslatef(-1.19, 1.48, .1);
    glScalef(.05, .05, .05);
    glutSolidCube(1.0);
    glPopMatrix();

    // FRONT GLASS FRAME UP VIEW
    glPushMatrix();
    glColor4f(.9, .9, .9, 1);
    glTranslatef(-1.2, 2.4, .75);
    glScalef(.05, .2, 1.5);
    glutSolidCube(1.0);
    glPopMatrix();

    // FRONT GLASS FRAME DOWN VIEW
    glPushMatrix();
    glColor4f(.9, .9, .9, 1);
    glTranslatef(-1.2, .6, .75);
    glScalef(.05, .2, 1.5);
    glutSolidCube(1.0);
    glPopMatrix();

    // FRONT GLASS FRAME LEFT VIEW
    glPushMatrix();
    glColor4f(.9, .9, .9, 1);
    glTranslatef(-1.2, 1.5, 1.4);
    glScalef(.05, 1.7, .2);
    glutSolidCube(1.0);
    glPopMatrix();

    // FRONT GLASS FRAME RIGHT VIEW
    glPushMatrix();
    glColor4f(.9, .9, .9, 1);
    glTranslatef(-1.2, 1.5, .1);
    glScalef(.05, 1.7, .2);
    glutSolidCube(1.0);
    glPopMatrix();

    // FRONT GLASS LEFT VIEW
    glPushMatrix();
    glColor4f(.95, .95, .95, .5f); // abu-abu
    glTranslatef(-1.2, 1.5, .75);
    glScalef(.05, 2, 1.5);
    glutSolidCube(1.0);
    glPopMatrix();

#pragma endregion

#pragma region GLASS RIGHT

    // HANDLE 1
    glPushMatrix();
    glColor4f(.5, .5, .5, 1);
    glTranslatef(-1.09, 1.5, -.65);
    glScalef(.05, .15, .1);
    glutSolidCube(1.0);
    glPopMatrix();

    // HANDLE 2
    glPushMatrix();
    glColor4f(.8, .8, .8, 1);
    glTranslatef(-1.08, 1.5, -.65);
    glScalef(.05, .1, .05);
    glutSolidCube(1.0);
    glPopMatrix();

    // LOCK 1
    glPushMatrix();
    glColor4f(.8, .8, .8, 1);
    glTranslatef(-1.09, 1.5, .65);
    glScalef(.05, .15, .05);
    glutSolidCube(1.0);
    glPopMatrix();

    // LOCK 2
    glPushMatrix();
    glColor4f(.3, .3, .3, 1);
    glTranslatef(-1.08, 1.48, .65);
    glScalef(.05, .05, .05);
    glutSolidCube(1.0);
    glPopMatrix();

    // FRONT GLASS FRAME UP VIEW
    glPushMatrix();
    glColor4f(.9, .9, .9, 1);
    glTranslatef(-1.1, 2.4, 0);
    glScalef(.05, .2, 1.5);
    glutSolidCube(1.0);
    glPopMatrix();

    // FRONT GLASS FRAME DOWN VIEW
    glPushMatrix();
    glColor4f(.9, .9, .9, 1);
    glTranslatef(-1.1, .6, .0);
    glScalef(.05, .2, 1.5);
    glutSolidCube(1.0);
    glPopMatrix();

    // FRONT GLASS FRAME LEFT VIEW
    glPushMatrix();
    glColor4f(.9, .9, .9, 1);
    glTranslatef(-1.1, 1.5, .65);
    glScalef(.05, 1.7, .2);
    glutSolidCube(1.0);
    glPopMatrix();

    // FRONT GLASS FRAME RIGHT VIEW
    glPushMatrix();
    glColor4f(.9, .9, .9, 1);
    glTranslatef(-1.1, 1.5, -.65);
    glScalef(.05, 1.7, .2);
    glutSolidCube(1.0);
    glPopMatrix();

    // FRONT GLASS RIGHT VIEW
    glPushMatrix();
    glColor4f(.95, .95, .95, .5f);
    glTranslatef(-1.1, 1.5, 0);
    glScalef(.05, 2, 1.5);
    glutSolidCube(1.0);
    glPopMatrix();

#pragma endregion

#pragma region RAIL DOWN DOOR UP
    // UP GLASS DOOR TRAIL 1
    glPushMatrix();
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.2, .45, 0);
    glScalef(.02, .02, 3);
    glutSolidCube(1.0);
    glPopMatrix();

    // UP GLASS DOOR TRAIL 2
    glPushMatrix();
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.1, .45, 0);
    glScalef(.02, .02, 3);
    glutSolidCube(1.0);
    glPopMatrix();
#pragma endregion

#pragma region RAIL DOWN DOOR DOWN
    // UP DOWN DOOR TRAIL 1
    glPushMatrix();
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.2, -2.3, 0);
    glScalef(.02, .02, 3);
    glutSolidCube(1.0);
    glPopMatrix();

    // UP DOWN DOOR TRAIL 2
    glPushMatrix();
    glColor4f(.60, .60, .60, 1);
    glTranslatef(-1.1, -2.3, 0);
    glScalef(.02, .02, 3);
    glutSolidCube(1.0);
    glPopMatrix();

#pragma endregion

#pragma region DOWN DOOR LEFT

    // HANDLE 1
    glPushMatrix();
    glColor4f(.5, .5, .5, 1);
    glTranslatef(-1.19, -1, 1.4);
    glScalef(.05, .15, .1);
    glutSolidCube(1.0);
    glPopMatrix();

    // HANDLE 2
    glPushMatrix();
    glColor4f(.8, .8, .8, 1);
    glTranslatef(-1.18, -1, 1.4);
    glScalef(.05, .1, .05);
    glutSolidCube(1.0);
    glPopMatrix();

    // FRONT GLASS LEFT VIEW
    glPushMatrix();
    glColor4f(.9, .9, .9, 1);
    glTranslatef(-1.2, -1, .75);
    glScalef(.05, 2.9, 1.5);
    glutSolidCube(1.0);
    glPopMatrix();
#pragma endregion

#pragma region DOWN DOOR RIGHT

    // HANDLE 1
    glPushMatrix();
    glColor4f(.5, .5, .5, 1);
    glTranslatef(-1.09, -1, -1.4);
    glScalef(.05, .15, .1);
    glutSolidCube(1.0);
    glPopMatrix();

    // HANDLE 2
    glPushMatrix();
    glColor4f(.8, .8, .8, 1);
    glTranslatef(-1.08, -1, -1.4);
    glScalef(.05, .1, .05);
    glutSolidCube(1.0);
    glPopMatrix();

    // LOCK 1
    glPushMatrix();
    glColor4f(.8, .8, .8, 1);
    glTranslatef(-1.09, -1, -.1);
    glScalef(.05, .15, .05);
    glutSolidCube(1.0);
    glPopMatrix();

    // LOCK 2
    glPushMatrix();
    glColor4f(.3, .3, .3, 1);
    glTranslatef(-1.08, -1, -.1);
    glScalef(.05, .05, .05);
    glutSolidCube(1.0);
    glPopMatrix();

    // FRONT GLASS RIGHT VIEW
    glPushMatrix();
    glColor4f(.9, .9, .9, 1);
    glTranslatef(-1.1, -1, -.75);
    glScalef(.05, 2.9, 1.5);
    glutSolidCube(1.0);
    glPopMatrix();
#pragma endregion

#pragma endregion
}

int init(void)
{
    // glClearColor(0.93f, 0.93f, 0.93f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0f);

    return 1;
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gluLookAt(
        4, 1, 4,
        0, 0, 0,
        0, 1, 0);

    glRotatef(xrot, 1, 0, 0);
    glRotatef(yrot, 0, 1, 0);

    drawBox();

    glFlush();
    glutSwapBuffers();
}

void resize(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(grow_shrink, resize_f * w / h, resize_f, 100 * resize_f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void idle(void)
{
    if (!mouseDown)
    {
        xrot += 0.3f;
        yrot += 0.4f;
    }

    glutPostRedisplay();
}

void mySpecialFunction()
{
    printf("U -----------> rotate clockwise\n");
    printf("Y -----------> rotate counter clockwise\n");
    printf("W or w ------> Up\n");
    printf("S or s -----> Down\n");
    printf("D or d ------> Right\n");
    printf("A or a ------> Left\n");
    printf("Z or z ------> Shrink\n");
    printf("X or x ------> Grow\n");
    printf("Escape Key ---> exit the program\n\n");
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(1);
        break;

    case 'w':
    case 'W':
        tra_x += 0.1f;
        break;
    case 's':
    case 'S':
        tra_x -= 0.1f;
        break;
    case 'a':
    case 'A':
        tra_z += 0.1f;
        break;
    case 'd':
    case 'D':
        tra_z -= 0.1f;
        break;
    case 'u':
    case 'U':
        xrot += 1.0f;
        yrot += 1.0f;
        xdiff += 1.0f;
        ydiff += 1.0f;
        break;

    case 'y':
    case 'Y':
        xrot -= 1.0f;
        yrot -= 1.0f;
        xdiff += 1.0f;
        ydiff += 1.0f;
        break;

    case 'Z':
    case 'z':
        grow_shrink--;
        resize(800, 800);

        break;
    case 'X':
    case 'x':
        grow_shrink++;
        resize(800, 800);

        break;
    }

    glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y)
{
    if (key == GLUT_KEY_F1)
    {
        fullscreen = !fullscreen;

        if (fullscreen)
            glutFullScreen();
        else
        {
            glutReshapeWindow(800, 800);
            glutPositionWindow(80, 80);
        }
    }
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mouseDown = 1;

        xdiff = x - yrot;
        ydiff = -y + xrot;
    }
    else
        mouseDown = 0;
}

void mouseMotion(int x, int y)
{
    if (mouseDown)
    {
        yrot = x - xdiff;
        xrot = y + ydiff;

        glutPostRedisplay();
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(800, 800);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutCreateWindow("Lemari Game Board");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutReshapeFunc(resize);
    mySpecialFunction();
    // glutIdleFunc(idle);

    if (!init())
        return 1;

    glutMainLoop();

    return 0;
}