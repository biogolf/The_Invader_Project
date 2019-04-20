/* 
Computer Programming Project@IT KMITL
Developer:
  Chanayus Nuiad                    SID:61070034
  Passakorn Nuchitkachornwut        SID:61070164
  Supachai Chanto                   SID:61070224

Thank you to:
Learn basic openGL
  https://www.youtube.com/watch?v=3aJ8OR1C6pk
  https://www.youtube.com/watch?v=epzHAT_YGkI
  https://www.youtube.com/watch?v=NT-0Q2Psp2Y
Learn Example for game
  https://github.com/kaushikj/OpenGL/blob/Public098/spaceship.cpp
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

void display();
void reshape(int , int );
void timer(int);
void myCreate();
void myShip();
void drawEnemy(int ey);
void myBullet();
void keys(unsigned char key, int x, int y);
void genEnemy();
void displayRasterText(float x ,float y ,float z ,char *stringToDisplay);
void randomEnemy();
void start();
void play();
void win();
void lose();

int myx = 0, myy = -25, bullet_x, bullet_y=-100, shoot=0, myHP = 5, myScore = 0;
float enemy_x[103], enemy_y[103] = {0}, enemy_speed = 0.3, enemy_camouflage = 0; 
int enemy_amount=0, enemy_status[103] = {0};
int timepass = 0;
char HP[100], score[100];
int dis_start = 1, dis_play = 0, dis_win = 0, dis_lose = 0;

void init()
{
  glClearColor(0.0,0.0,0.0,1.0); //(R,G,B,alpha)
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv); //start to use glut
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); //DOUBLE is to swap front buffer and back buffer
  glutInitWindowPosition(100, 100); //Where to Show new window on screen
  glutInitWindowSize(900,500);
  glutTimerFunc(0,timer,0);
  glutCreateWindow("The Invader");
  randomEnemy();
  glutDisplayFunc(display); //make display call back
  glutReshapeFunc(reshape);
  init();
  glutMainLoop(); //Make window still running
  /* code */
  return 0;
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  if (dis_start == 1)
  {
    start();
  }
  else if(dis_play == 1)
  {
    play();
  }
  else if(dis_lose == 1)
  {
    lose();
  }
  else if(dis_win == 1)
  {
    win();
  }

  glFlush();
  glLoadIdentity();
  glutSwapBuffers();
  glutKeyboardFunc(keys);
}

void start()
{
  glPushMatrix();
    glTranslatef(0,0,0);
    glBegin(GL_POLYGON); 
      glColor3f(1.0,0.2,0.2);
      glVertex2f(-15,35);
      glVertex2f(15,35);
      glVertex2f(15,25);
      glVertex2f(-15,25);
    glEnd();
  glPopMatrix();
  glColor3f(0.2,1.0,0.0);
  displayRasterText(-7 ,29 ,1 ,"The Invader");
  glPushMatrix();
    glTranslatef(0,0,0);
    glBegin(GL_POLYGON); 
      glColor3f(0.2,0.2,0.2);
      glVertex2f(-35,20);
      glVertex2f(35,20);
      glVertex2f(35,-30);
      glVertex2f(-35,-30);
    glEnd();
  glPopMatrix();
  glColor3f(0.0,1.0,1.0);
  displayRasterText(-30 ,10 ,1 ,"Press 'w' to shoot");
  displayRasterText(-30 ,-5 ,1 ,"Press 'a' to move left");
  displayRasterText(-30 ,-20 ,1 ,"Press 'd' to move right");
  glColor3f(1.0,1.0,0.0);
  displayRasterText(-10 ,-40 ,1 ,"Press 'r' to start");

  //draw ship example
  glPushMatrix();//draw wing
    glTranslatef(20,-5,0);
    glBegin(GL_POLYGON);
      glColor3f(1.0,0.0,0.0);
      glVertex2f(2.0,1.0);
      glVertex2f(-2.0,1.0);
      glColor3f(1.0,1.0,0.0);
      glVertex2f(-4.0,-1.0);
      glVertex2f(4.0,-1.0);
    glEnd();
    glBegin(GL_POLYGON); //draw gun
      glColor3f(0.0,0.7,1.0);
      glVertex2f(1.5,1.0);
      glVertex2f(0.0,5.0);
      glVertex2f(-1.5,1.0);
    glEnd();
    glBegin(GL_POLYGON); //draw tail
    glColor3f(0.0,0.3,1.0);
      glVertex2f(-0.5,1.0);
      glVertex2f(-2.0,-4.0);
      glVertex2f(0.0,-2.5);
      glVertex2f(2.0,-4.0);
      glVertex2f(0.5,1.0);
    glEnd();
  glPopMatrix();
}

void play()
{
  myCreate();
    myShip();
    if (shoot==1)
    {
      myBullet();
      bullet_y += 5;
    }
    if (bullet_y >= 45)
    {
      shoot=0;
      bullet_y = -55;
    }
    for (int h = 0; h < enemy_amount; ++h)
    {
      if (bullet_x > enemy_x[h]-5 && bullet_x < enemy_x[h]+6 &&  //bullet hit enemy
        bullet_y < enemy_y[h]+61 && bullet_y > enemy_y[h]+54)
      {
        shoot=0;
        enemy_status[h] = 0;
        bullet_y = -100; //avoid invisible bullet
        enemy_y[h] = -150; //avoid score error
        myScore += 1;
      }
    }

    for (int k = 0; k < enemy_amount; ++k) //enemy crash my Ship
    {
      if (enemy_y[k] <= -85 && enemy_status[k] == 1)
      {
        enemy_status[k] = 0;
        enemy_y[k] = -150; //avoid score error
        myHP -= 1;
      }
    }
    genEnemy();

    itoa(myHP, HP, 10);
    glColor3f(1.0,0.8,0.0);
    displayRasterText(-50 ,45 ,1 ,"HP");
    displayRasterText(-42 ,45 ,1 ,HP);
    itoa(myScore, score, 10);
    displayRasterText(-50 ,40 ,1 ,"Score");
    displayRasterText(-42 ,40 ,1 ,score);

    if (myHP == 0)
    {
      dis_play = 0;
      dis_lose = 1;
    }

    if (enemy_amount == 103 && enemy_status[102] == 0)
    {
      dis_play = 0;
      dis_win = 1;
    }
}

void win()
{
  glPushMatrix();
    glTranslatef(0,0,0);
    glBegin(GL_POLYGON); 
      glColor3f(0.0,0.8,0.0);
      glVertex2f(-50,50);
      glVertex2f(50,50);
      glColor3f(0.5,0.8,0.5);
      glVertex2f(50,-50);
      glVertex2f(-50,-50);
    glEnd();
  glPopMatrix();
  glColor3f(0.0,0.0,0.0);
  displayRasterText(-6 ,5 ,1 ,"YOU WIN !");
  itoa(myScore, score, 10);
  displayRasterText(-4 ,-10 ,1 ,"Score : ");
  displayRasterText(4 ,-10 ,1 ,score);
  displayRasterText(-11 ,-40 ,1 ,"Press 'r' to re-start");
}

void lose()
{
  glPushMatrix();
    glTranslatef(0,0,0);
    glBegin(GL_POLYGON); 
      glColor3f(0.8,0.0,0.0);
      glVertex2f(-50,50);
      glVertex2f(50,50);
      glColor3f(0.3,0.0,0.0);
      glVertex2f(50,-50);
      glVertex2f(-50,-50);
    glEnd();
  glPopMatrix();
  glColor3f(0.0,0.0,0.0);
  displayRasterText(-6 ,5 ,1 ,"YOU LOSE !");
  itoa(myScore, score, 10);
  displayRasterText(-4 ,-10 ,1 ,"Score : ");
  displayRasterText(4 ,-10 ,1 ,score);
  displayRasterText(-11 ,-40 ,1 ,"Press 'r' to re-start");
}

void reshape(int w,int h)
{
  /* viewport */
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-50,50 , -50,50);
  glMatrixMode(GL_MODELVIEW);
}

void myCreate()
{
  glPushMatrix();//draw wing
    glTranslatef(myx,myy,0);
    glBegin(GL_POLYGON);
      glColor3f(1.0,0.0,0.0);
      glVertex2f(2.0,1.0);
      glVertex2f(-2.0,1.0);
      glColor3f(1.0,1.0,0.0);
      glVertex2f(-4.0,-1.0);
      glVertex2f(4.0,-1.0);
    glEnd();
    glBegin(GL_POLYGON); //draw gun
      glColor3f(0.0,0.7,1.0);
      glVertex2f(1.5,1.0);
      glVertex2f(0.0,5.0);
      glVertex2f(-1.5,1.0);
    glEnd();
    glBegin(GL_POLYGON); //draw tail
    glColor3f(0.0,0.3,1.0);
      glVertex2f(-0.5,1.0);
      glVertex2f(-2.0,-4.0);
      glVertex2f(0.0,-2.5);
      glVertex2f(2.0,-4.0);
      glVertex2f(0.5,1.0);
    glEnd();
  glPopMatrix();
}

void myShip()
{
  glPushMatrix();//draw BigShip
    glTranslatef(0,0,0);
    glBegin(GL_POLYGON); //draw body
      glColor3f(0.7,0.7,0.7);
      glVertex2f(-50,-30);
      glVertex2f(50,-30);
      glVertex2f(50,-50);
      glVertex2f(-50,-50);
    glEnd();
    glBegin(GL_POLYGON); //draw line
      glColor3f(0.2,0.2,0.2);
      glVertex2f(-50,-40);
      glVertex2f(-30,-40);
      glVertex2f(-30,-42);
      glVertex2f(-50,-42);
    glEnd();
    glBegin(GL_POLYGON); //draw line
      glColor3f(0.2,0.2,0.2);
      glVertex2f(-20,-40);
      glVertex2f(20,-40);
      glVertex2f(20,-42);
      glVertex2f(-20,-42);
    glEnd();
    glBegin(GL_POLYGON); //draw line
      glColor3f(0.2,0.2,0.2);
      glVertex2f(30,-40);
      glVertex2f(50,-40);
      glVertex2f(50,-42);
      glVertex2f(30,-42);
    glEnd();
    glBegin(GL_POLYGON); //draw edge window
      glColor3f(0.1,0.1,0.3);
      glVertex2f(-29,-33);
      glVertex2f(-21,-33);
      glVertex2f(-21,-49);
      glVertex2f(-29,-49);
    glEnd();
    glBegin(GL_POLYGON); //draw edge window
      glColor3f(0.1,0.1,0.3);
      glVertex2f(21,-33);
      glVertex2f(29,-33);
      glVertex2f(29,-49);
      glVertex2f(21,-49);
    glEnd();
    glBegin(GL_POLYGON); //draw glass window
      glColor3f(0.0,0.9,1.0);
      glVertex2f(-28,-35);
      glVertex2f(-22,-35);
      glVertex2f(-22,-47);
      glVertex2f(-28,-47);
    glEnd();
    glBegin(GL_POLYGON); //draw glass window
      glColor3f(0.0,0.9,1.0);
      glVertex2f(22,-35);
      glVertex2f(28,-35);
      glVertex2f(28,-47);
      glVertex2f(22,-47);
    glEnd();
  glPopMatrix();
}

void drawEnemy(int ey){
  glPushMatrix();//draw wing
  glLoadIdentity();
    glTranslatef(enemy_x[ey],10+enemy_y[ey],0);
    glBegin(GL_POLYGON);
      glColor3f(0.0,1.0 - enemy_camouflage,0.0);
      glVertex2f(0,50);
      glVertex2f(-5,45);
      glVertex2f(-3,44);
      glVertex2f(-1,47);
      glVertex2f(0,44);
      glVertex2f(1,47);
      glVertex2f(3,44);
      glVertex2f(5,45);
      glVertex2f(0,50);
    glEnd();
  glPopMatrix();
}

void myBullet(){
  glPushMatrix();//draw bullet
    glTranslatef(bullet_x,bullet_y,0);
    glBegin(GL_POLYGON);
      glColor3f(1.0,0.0,0.0);
      glVertex2f(1.0,6.0);
      glVertex2f(-1.0,6.0);
      glVertex2f(-1.0,5.0);
      glVertex2f(1.0,5.0);
    glEnd();
  glPopMatrix();
}

void timer(int what)
{
  glutPostRedisplay();
  glutTimerFunc(1000/60,timer,0);
  
}

//from https://github.com/kaushikj/OpenGL/blob/Public098/spaceship.cpp
//line 787
void keys(unsigned char key, int x, int y)
{
  if(key == 'a' && dis_play == 1) {myx-=5;}
  if(key == 'd' && dis_play == 1) {myx+=5;}
  if(key == 'w' && shoot==0 && dis_play == 1) {shoot=1;bullet_x=myx;bullet_y=-25;}
  if(key == 'r' && dis_start == 1) {dis_start=0;dis_play=1;}
  if(key == 'r' && (dis_win == 1 || dis_lose == 1))
  {
    myx = 0; myy = -25; shoot=0; myHP = 5; myScore = 0;
    bullet_y = -100; //avoid invisible bullet
    randomEnemy();
    for (int m = 0; m < 103; ++m)
    {
      enemy_y[m] = 0;
      enemy_status[m] = 0;
    }
    enemy_speed = 0.3; 
    enemy_amount=0;
    enemy_camouflage = 0;
    timepass = 0;
    dis_start = 1; dis_play = 0; dis_win = 0; dis_lose = 0;
  }
  display();
}

void genEnemy()
{
  timepass += 1;
  if (timepass%85 == 0 && enemy_amount < 103)
  {
    enemy_status[enemy_amount] = 1;
    enemy_amount += 1;
  }
  if (enemy_amount%30 == 0 && enemy_amount != 0)
  {
    enemy_speed += 0.1;
    enemy_amount += 1;
    enemy_camouflage += 0.29;
  }
  for (int i = 0; i < enemy_amount; ++i)
  {
    if (enemy_status[i] == 1)
    {
      enemy_y[i] -= enemy_speed;
      drawEnemy(i);
    }
  }
}

//from https://github.com/kaushikj/OpenGL/blob/Public098/spaceship.cpp
//line82
void displayRasterText(float x ,float y ,float z ,char *stringToDisplay) {
  int length;
  glRasterPos3f(x, y, z);
    length = strlen(stringToDisplay);

  for(int i=0 ;i<length ;i++){
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 ,stringToDisplay[i]); 
  }
}

void randomEnemy()
{
  for (int j = 0; j < 101; ++j)
  {
    enemy_x[j] = (rand()%80)-40;
  }
}