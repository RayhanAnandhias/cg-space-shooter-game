#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "BmpLoader.h"
#include "tutorial4.h"
#include "texture.h"
#include "3dsloader.h"

using namespace std;

char title[] = "Rayhan Azka Anandhias Putra - 181524028";
float _angle = 0.0;
GLuint _textureSpace;
int screen_width=1000;
int screen_height=600;
obj_type object;	//kapal pemain
obj_type missile; //misil
obj_type enemy;  //kapal musuh
int missile_flag = 0;
int emissile_flag = 0;
int isPlayerHit = 0;
int isEnemyHit = 0;
int isEnemy2Hit = 0;
int pov = 0;
int score = 0;

double playerRotation_x=272, playerTrans_x = 0;
double playerRotation_y=0, playerTrans_y = -100;
double playerRotation_z=180, playerTrans_z = -500;

double missileTrans_z = playerTrans_z, missileTrans_z_inc = -2;
double missileRotation_x=0;
double missileRotation_y=90;
double missileRotation_z=180;
double missileTrans_x;
double missileTrans_y;

double enemyRotation_x=0, enemyTrans_x =  rand() % 800 - 400;
double enemyRotation_y=180, enemyTrans_y = 0;
double enemyRotation_z=0, enemyTrans_z = -800;
double enemyTrans_x_inc = -0.16;
double enemyTrans_y_inc = -0.16;

double enemy2Trans_x = 350;
double enemy2Trans_y = 0;
double enemy2Trans_x_inc = 0.16;
double enemy2Trans_y_inc = 0.16;

double emissileTrans_z = enemyTrans_z, emissileTrans_z_inc = 1;
double emissileRotation_x=0;
double emissileRotation_y=270;
double emissileRotation_z=180;
double emissileTrans_x;
double emissileTrans_y;

double emissile2Trans_z = enemyTrans_z, emissile2Trans_z_inc = 1;
double emissile2Trans_x;
double emissile2Trans_y; 


void Init();
void KeyboardHandler(unsigned char key, int x, int y);
void Reshape(GLsizei width, GLsizei height);
void Display();
GLuint loadTexture(const char* filename);
void keyboard_s (int key, int x, int y);
bool isCollisionToPlayer();
bool isCollisionToEnemy();
bool isCollisionToEnemy2();
bool isCollisionToPlayerFromEnemy2();
bool isCollisionPlayerEnemy1();
bool isCollisionPlayerEnemy2();
void renderBitmapString(float x, float y, void *font,const char *string);

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_width,screen_height);
	glutInitWindowPosition(50, 20);
	glutCreateWindow(title);
	
	glutDisplayFunc(Display);
	glutIdleFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyboardHandler);
    glutSpecialFunc (keyboard_s);
	Init();
	glutMainLoop();
	return 0;
}

void Display()
{
	int l_index;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	char scr[50];
	sprintf(scr, "score : %d", score);
	renderBitmapString(-320,190,GLUT_BITMAP_9_BY_15, scr);
	glPopMatrix();

	if(pov == 1) {
		glPushMatrix();
		gluLookAt(playerTrans_x,playerTrans_y + 100,playerTrans_z+300,playerTrans_x,playerTrans_y + 60,playerTrans_z,0,1,0);
	} else {
		glPopMatrix();
	}

    // Sky
    glPushMatrix();
    	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureSpace);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-500);
        glRotatef(_angle, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS); //Belakang
            glTexCoord3f(1.0,1.0,0);  glVertex3f(-500,500,-500);
            glTexCoord3f(0.0,0.0,0);  glVertex3f(-500,-500,-500);
            glTexCoord3f(1.0,0.0,0);  glVertex3f(500,-500,-500);
            glTexCoord3f(0.0,1.0,0);  glVertex3f(500,500,-500);
        glEnd();
        glBegin(GL_QUADS); //Depan
            glTexCoord3f(1.0,1.0,0);  glVertex3f(-500,500,500);
            glTexCoord3f(0.0,0.0,0);  glVertex3f(-500,-500,500);
            glTexCoord3f(1.0,0.0,0);  glVertex3f(500,-500,500);
            glTexCoord3f(0.0,1.0,0);  glVertex3f(500,500,500);
        glEnd();
        glBegin(GL_QUADS); //Kanan
            glTexCoord3f(1.0,1.0,0);  glVertex3f(500,500,-500);
            glTexCoord3f(0.0,0.0,0);  glVertex3f(500,-500,-500);
            glTexCoord3f(1.0,0.0,0);  glVertex3f(500,-500,500);
            glTexCoord3f(0.0,1.0,0);  glVertex3f(500,500,500);
        glEnd();
        glBegin(GL_QUADS); //Kiri
            glTexCoord3f(1.0,1.0,0);  glVertex3f(-500,500,-500);
            glTexCoord3f(0.0,0.0,0);  glVertex3f(-500,-500,-500);
            glTexCoord3f(1.0,0.0,0);  glVertex3f(-500,-500,500);
            glTexCoord3f(0.0,1.0,0);  glVertex3f(-500,500,500);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
	
	// glPushMatrix();
	// 	glTranslatef(playerTrans_x, playerTrans_y, playerTrans_z - 170);
	// 	glScalef(100,100,100);
	// 	glBegin(GL_QUADS);
	// 		glVertex3f(1.05f, 0.3f, 1.0f);
	// 		glVertex3f(-1.05f, 0.3f, 1.0f);
	// 		glVertex3f(-1.05f, -0.6f, 1.0f);
	// 		glVertex3f(1.05f, -0.6f, 1.0f);
	// 	glEnd();
	// glPopMatrix();

	if(isPlayerHit == 0) {
		
		//my player
		glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			glTranslatef(playerTrans_x, playerTrans_y, playerTrans_z); // We move the object forward (the model matrix is multiplied by the translation matrix)
			glScalef(0.8,0.8,0.8);
			if (playerRotation_x > 359) playerRotation_x = 0;
			if (playerRotation_y > 359) playerRotation_y = 0;
			if (playerRotation_z > 359) playerRotation_z = 0;
			

			glRotatef(playerRotation_x,1.0,0.0,0.0); // Rotations of the object (the model matrix is multiplied by the rotation matrices)
			glRotatef(playerRotation_y,0.0,1.0,0.0);
			glRotatef(playerRotation_z,0.0,0.0,1.0);
			
			glBindTexture(GL_TEXTURE_2D, object.id_texture); // We set the active texture 

			glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
				for (l_index=0;l_index<object.polygons_qty;l_index++)
				{
					//----------------- FIRST VERTEX -----------------
					// Texture coordinates of the first vertex
					glTexCoord2f( object.mapcoord[ object.polygon[l_index].a ].u,
								object.mapcoord[ object.polygon[l_index].a ].v);
					// Coordinates of the first vertex
					glVertex3f( object.vertex[ object.polygon[l_index].a ].x,
								object.vertex[ object.polygon[l_index].a ].y,
								object.vertex[ object.polygon[l_index].a ].z); //Vertex definition

					//----------------- SECOND VERTEX -----------------
					// Texture coordinates of the second vertex
					glTexCoord2f( object.mapcoord[ object.polygon[l_index].b ].u,
								object.mapcoord[ object.polygon[l_index].b ].v);
					// Coordinates of the second vertex
					glVertex3f( object.vertex[ object.polygon[l_index].b ].x,
								object.vertex[ object.polygon[l_index].b ].y,
								object.vertex[ object.polygon[l_index].b ].z);
					
					//----------------- THIRD VERTEX -----------------
					// Texture coordinates of the third vertex
					glTexCoord2f( object.mapcoord[ object.polygon[l_index].c ].u,
								object.mapcoord[ object.polygon[l_index].c ].v);
					// Coordinates of the Third vertex
					glVertex3f( object.vertex[ object.polygon[l_index].c ].x,
								object.vertex[ object.polygon[l_index].c ].y,
								object.vertex[ object.polygon[l_index].c ].z);
				}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();

	}
	

	if(isEnemyHit == 0) {
		//enemy1
		glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			enemyTrans_x += enemyTrans_x_inc;
			enemyTrans_y += enemyTrans_y_inc;
			if(enemyTrans_x <= -400) {
				enemyTrans_x_inc = 0.16;
			} else if (enemyTrans_x >= 400) {
				enemyTrans_x_inc = -0.16;
			} else if(enemyTrans_y >= 200) {
				enemyTrans_y_inc = -0.16;
			} else if(enemyTrans_y <= -200) {
				enemyTrans_y_inc = 0.16;
			}
			
			glTranslatef(enemyTrans_x, enemyTrans_y, enemyTrans_z); // We move the object forward (the model matrix is multiplied by the translation matrix)
			glScalef(9,9,9);

			glRotatef(enemyRotation_x,1.0,0.0,0.0); // Rotations of the object (the model matrix is multiplied by the rotation matrices)
			glRotatef(enemyRotation_y,0.0,1.0,0.0);
			glRotatef(enemyRotation_z,0.0,0.0,1.0);
			
			glBindTexture(GL_TEXTURE_2D, enemy.id_texture); // We set the active texture 

			glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
				for (l_index=0;l_index<enemy.polygons_qty;l_index++)
				{
					//----------------- FIRST VERTEX -----------------
					// Texture coordinates of the first vertex
					glTexCoord2f( enemy.mapcoord[ enemy.polygon[l_index].a ].u,
								enemy.mapcoord[ enemy.polygon[l_index].a ].v);
					// Coordinates of the first vertex
					glVertex3f( enemy.vertex[ enemy.polygon[l_index].a ].x,
								enemy.vertex[ enemy.polygon[l_index].a ].y,
								enemy.vertex[ enemy.polygon[l_index].a ].z); //Vertex definition

					//----------------- SECOND VERTEX -----------------
					// Texture coordinates of the second vertex
					glTexCoord2f( enemy.mapcoord[ enemy.polygon[l_index].b ].u,
								enemy.mapcoord[ enemy.polygon[l_index].b ].v);
					// Coordinates of the second vertex
					glVertex3f( enemy.vertex[ enemy.polygon[l_index].b ].x,
								enemy.vertex[ enemy.polygon[l_index].b ].y,
								enemy.vertex[ enemy.polygon[l_index].b ].z);
					
					//----------------- THIRD VERTEX -----------------
					// Texture coordinates of the third vertex
					glTexCoord2f( enemy.mapcoord[ enemy.polygon[l_index].c ].u,
								enemy.mapcoord[ enemy.polygon[l_index].c ].v);
					// Coordinates of the Third vertex
					glVertex3f( enemy.vertex[ enemy.polygon[l_index].c ].x,
								enemy.vertex[ enemy.polygon[l_index].c ].y,
								enemy.vertex[ enemy.polygon[l_index].c ].z);
				}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	if (isEnemy2Hit == 0) {
		//enemy2
		glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			enemy2Trans_x += enemy2Trans_x_inc;
			if(enemy2Trans_x <= -400) {
				enemy2Trans_x_inc = 0.16;
			} else if (enemy2Trans_x >= 400) {
				enemy2Trans_x_inc = -0.16;
			}
			glTranslatef(enemy2Trans_x, enemyTrans_y + 150, enemyTrans_z); // We move the object forward (the model matrix is multiplied by the translation matrix)
			glScalef(9,9,9);

			glRotatef(enemyRotation_x,1.0,0.0,0.0); // Rotations of the object (the model matrix is multiplied by the rotation matrices)
			glRotatef(enemyRotation_y,0.0,1.0,0.0);
			glRotatef(enemyRotation_z,0.0,0.0,1.0);
			
			glBindTexture(GL_TEXTURE_2D, enemy.id_texture); // We set the active texture 

			glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
				for (l_index=0;l_index<enemy.polygons_qty;l_index++)
				{
					//----------------- FIRST VERTEX -----------------
					// Texture coordinates of the first vertex
					glTexCoord2f( enemy.mapcoord[ enemy.polygon[l_index].a ].u,
								enemy.mapcoord[ enemy.polygon[l_index].a ].v);
					// Coordinates of the first vertex
					glVertex3f( enemy.vertex[ enemy.polygon[l_index].a ].x,
								enemy.vertex[ enemy.polygon[l_index].a ].y,
								enemy.vertex[ enemy.polygon[l_index].a ].z); //Vertex definition

					//----------------- SECOND VERTEX -----------------
					// Texture coordinates of the second vertex
					glTexCoord2f( enemy.mapcoord[ enemy.polygon[l_index].b ].u,
								enemy.mapcoord[ enemy.polygon[l_index].b ].v);
					// Coordinates of the second vertex
					glVertex3f( enemy.vertex[ enemy.polygon[l_index].b ].x,
								enemy.vertex[ enemy.polygon[l_index].b ].y,
								enemy.vertex[ enemy.polygon[l_index].b ].z);
					
					//----------------- THIRD VERTEX -----------------
					// Texture coordinates of the third vertex
					glTexCoord2f( enemy.mapcoord[ enemy.polygon[l_index].c ].u,
								enemy.mapcoord[ enemy.polygon[l_index].c ].v);
					// Coordinates of the Third vertex
					glVertex3f( enemy.vertex[ enemy.polygon[l_index].c ].x,
								enemy.vertex[ enemy.polygon[l_index].c ].y,
								enemy.vertex[ enemy.polygon[l_index].c ].z);
				}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	// glPushMatrix();
	// 	glTranslatef(enemyTrans_x, enemyTrans_y, enemyTrans_z - 100);
	// 	glScalef(100,100,100);
	// 	glBegin(GL_QUADS);
	// 		glVertex3f(0.6f, 0.2f, 1.0f);
	// 		glVertex3f(-0.6f, 0.2f, 1.0f);
	// 		glVertex3f(-0.6f, -0.25f, 1.0f);
	// 		glVertex3f(0.6f, -0.25f, 1.0f);
	// 	glEnd();
	// glPopMatrix();

	//my missile
	if (missile_flag == 1  && missileTrans_z > -1000 && isPlayerHit == 0) {
		glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			missileTrans_x = playerTrans_x;
			missileTrans_y = playerTrans_y;
			missileTrans_z += missileTrans_z_inc;
			glTranslatef(missileTrans_x, missileTrans_y, missileTrans_z); // We move the object forward (the model matrix is multiplied by the translation matrix)
			glScalef(1.5,1.5,1.5);

			glRotatef(missileRotation_x,1.0,0.0,0.0); // Rotations of the object (the model matrix is multiplied by the rotation matrices)
			glRotatef(missileRotation_y,0.0,1.0,0.0);
			glRotatef(missileRotation_z,0.0,0.0,1.0);
			
			glBindTexture(GL_TEXTURE_2D, missile.id_texture); // We set the active texture 

			glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
				for (l_index=0;l_index<missile.polygons_qty;l_index++)
				{
					//----------------- FIRST VERTEX -----------------
					// Texture coordinates of the first vertex
					glTexCoord2f( missile.mapcoord[ missile.polygon[l_index].a ].u,
								missile.mapcoord[ missile.polygon[l_index].a ].v);
					// Coordinates of the first vertex
					glVertex3f( missile.vertex[ missile.polygon[l_index].a ].x,
								missile.vertex[ missile.polygon[l_index].a ].y,
								missile.vertex[ missile.polygon[l_index].a ].z); //Vertex definition

					//----------------- SECOND VERTEX -----------------
					// Texture coordinates of the second vertex
					glTexCoord2f( missile.mapcoord[ missile.polygon[l_index].b ].u,
								missile.mapcoord[ missile.polygon[l_index].b ].v);
					// Coordinates of the second vertex
					glVertex3f( missile.vertex[ missile.polygon[l_index].b ].x,
								missile.vertex[ missile.polygon[l_index].b ].y,
								missile.vertex[ missile.polygon[l_index].b ].z);
					
					//----------------- THIRD VERTEX -----------------
					// Texture coordinates of the third vertex
					glTexCoord2f( missile.mapcoord[ missile.polygon[l_index].c ].u,
								missile.mapcoord[ missile.polygon[l_index].c ].v);
					// Coordinates of the Third vertex
					glVertex3f( missile.vertex[ missile.polygon[l_index].c ].x,
								missile.vertex[ missile.polygon[l_index].c ].y,
								missile.vertex[ missile.polygon[l_index].c ].z);
				}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	} else {
		missile_flag = 0;
		missileTrans_z = playerTrans_z;
	}

	//enemy missile
	if(emissileTrans_z < 500 && isEnemyHit == 0)  {
		glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			emissileTrans_x = enemyTrans_x;
			emissileTrans_y = enemyTrans_y - 20;
			emissileTrans_z += emissileTrans_z_inc;
			glTranslatef(enemyTrans_x, enemyTrans_y - 20, emissileTrans_z); // We move the object forward (the model matrix is multiplied by the translation matrix)
			glScalef(1.5,1.5,1.5);

			glRotatef(emissileRotation_x,1.0,0.0,0.0); // Rotations of the object (the model matrix is multiplied by the rotation matrices)
			glRotatef(emissileRotation_y,0.0,1.0,0.0);
			glRotatef(emissileRotation_z,0.0,0.0,1.0);
			
			glBindTexture(GL_TEXTURE_2D, missile.id_texture); // We set the active texture 

			glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
				for (l_index=0;l_index<missile.polygons_qty;l_index++)
				{
					//----------------- FIRST VERTEX -----------------
					// Texture coordinates of the first vertex
					glTexCoord2f( missile.mapcoord[ missile.polygon[l_index].a ].u,
								missile.mapcoord[ missile.polygon[l_index].a ].v);
					// Coordinates of the first vertex
					glVertex3f( missile.vertex[ missile.polygon[l_index].a ].x,
								missile.vertex[ missile.polygon[l_index].a ].y,
								missile.vertex[ missile.polygon[l_index].a ].z); //Vertex definition

					//----------------- SECOND VERTEX -----------------
					// Texture coordinates of the second vertex
					glTexCoord2f( missile.mapcoord[ missile.polygon[l_index].b ].u,
								missile.mapcoord[ missile.polygon[l_index].b ].v);
					// Coordinates of the second vertex
					glVertex3f( missile.vertex[ missile.polygon[l_index].b ].x,
								missile.vertex[ missile.polygon[l_index].b ].y,
								missile.vertex[ missile.polygon[l_index].b ].z);
					
					//----------------- THIRD VERTEX -----------------
					// Texture coordinates of the third vertex
					glTexCoord2f( missile.mapcoord[ missile.polygon[l_index].c ].u,
								missile.mapcoord[ missile.polygon[l_index].c ].v);
					// Coordinates of the Third vertex
					glVertex3f( missile.vertex[ missile.polygon[l_index].c ].x,
								missile.vertex[ missile.polygon[l_index].c ].y,
								missile.vertex[ missile.polygon[l_index].c ].z);
				}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	} else {
		emissileTrans_z = enemyTrans_z;
	}

	//enemy missile 2
	if(emissile2Trans_z < 500 && isEnemy2Hit == 0)  {
		glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			emissile2Trans_x = enemy2Trans_x;
			emissile2Trans_y = enemyTrans_y - 20 + 150;
			emissile2Trans_z += emissile2Trans_z_inc;
			glTranslatef(enemy2Trans_x, enemyTrans_y - 20 + 150, emissile2Trans_z); // We move the object forward (the model matrix is multiplied by the translation matrix)
			glScalef(1.5,1.5,1.5);

			glRotatef(emissileRotation_x,1.0,0.0,0.0); // Rotations of the object (the model matrix is multiplied by the rotation matrices)
			glRotatef(emissileRotation_y,0.0,1.0,0.0);
			glRotatef(emissileRotation_z,0.0,0.0,1.0);
			
			glBindTexture(GL_TEXTURE_2D, missile.id_texture); // We set the active texture 

			glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
				for (l_index=0;l_index<missile.polygons_qty;l_index++)
				{
					//----------------- FIRST VERTEX -----------------
					// Texture coordinates of the first vertex
					glTexCoord2f( missile.mapcoord[ missile.polygon[l_index].a ].u,
								missile.mapcoord[ missile.polygon[l_index].a ].v);
					// Coordinates of the first vertex
					glVertex3f( missile.vertex[ missile.polygon[l_index].a ].x,
								missile.vertex[ missile.polygon[l_index].a ].y,
								missile.vertex[ missile.polygon[l_index].a ].z); //Vertex definition

					//----------------- SECOND VERTEX -----------------
					// Texture coordinates of the second vertex
					glTexCoord2f( missile.mapcoord[ missile.polygon[l_index].b ].u,
								missile.mapcoord[ missile.polygon[l_index].b ].v);
					// Coordinates of the second vertex
					glVertex3f( missile.vertex[ missile.polygon[l_index].b ].x,
								missile.vertex[ missile.polygon[l_index].b ].y,
								missile.vertex[ missile.polygon[l_index].b ].z);
					
					//----------------- THIRD VERTEX -----------------
					// Texture coordinates of the third vertex
					glTexCoord2f( missile.mapcoord[ missile.polygon[l_index].c ].u,
								missile.mapcoord[ missile.polygon[l_index].c ].v);
					// Coordinates of the Third vertex
					glVertex3f( missile.vertex[ missile.polygon[l_index].c ].x,
								missile.vertex[ missile.polygon[l_index].c ].y,
								missile.vertex[ missile.polygon[l_index].c ].z);
				}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	} else {
		emissile2Trans_z = enemyTrans_z;
	}

	if(isCollisionToPlayer() || isCollisionToPlayerFromEnemy2()) {
		isPlayerHit = 1;
	}

	if(isCollisionToEnemy()) {
		isEnemyHit = 1;
		score += 100;
	}

	if(isCollisionToEnemy2()) {
		isEnemy2Hit = 1;
		score += 100;
	}

	if(isCollisionPlayerEnemy1()) {
		isPlayerHit = 1;
		isEnemyHit = 1;
		playerTrans_z = -500;
	}

	if(isCollisionPlayerEnemy2()) {
		isPlayerHit = 1;
		isEnemy2Hit = 1;
		playerTrans_z = -500;
	}
	
	glutSwapBuffers();
}

void Init() {
	glClearColor(0.0, 0.0, 0.0, 0.0); // This clear the background color to black
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
   	
    // Viewport transformation
    glViewport(0,0,screen_width,screen_height);  

    // Projection transformation
    glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,10.0f,10000.0f); // We define the "viewing volume"
   
    glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
    
    glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping

    Load3DS(&object,"object/spaceship.3ds");
	Load3DS(&missile, "object/AVMT300.3ds");
	Load3DS(&enemy, "object/fighter2.3ds");

    object.id_texture=LoadBitmap("texture/kapal.bmp"); // The Function LoadBitmap() return the current texture ID
	missile.id_texture = LoadBitmap("texture/mistex.bmp");
	enemy.id_texture = LoadBitmap("texture/skull.bmp");
	
	_textureSpace = loadTexture("texture/space2.bmp");
}

void Reshape(GLsizei width, GLsizei height) {
	screen_width=width; // We obtain the new screen width values and store it
    screen_height=height; // Height value

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We clear both the color and the depth buffer so to draw the next frame
    glViewport(0,0,screen_width,screen_height); // Viewport transformation

    glMatrixMode(GL_PROJECTION); // Projection transformation
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,10.0f,10000.0f);

    glutPostRedisplay (); // This command redraw the scene (it calls the same routine of glutDisplayFunc)
}

void KeyboardHandler(unsigned char key, int x, int y)
{
	
	// d dan f
	switch (key) {
		case ' ' :
			missile_flag = 1;
			break;
		case ',':  
			_angle += 1;
			if (_angle > 360) _angle = 0.0;
			break;
		case '.':
			_angle -= 1;
			if (_angle > 360) _angle = 0.0;
			break;
		case 'w':
			playerRotation_x += 3.0;
			break;
		case 's':
			playerRotation_x -= 3.0;
			break;
		case 'a':
			playerRotation_z -= 3.0;
			break;
		case 'd':
			playerRotation_z += 3.0;
			break;
		case 'r':
			isEnemyHit = 0;
			isEnemy2Hit = 0;
			if(isPlayerHit == 1) {
				isPlayerHit = 0;
				score = 0;
			}
			break;
		case 'o':
			playerTrans_z -= 20;
			break;
		case 'l':
			playerTrans_z += 20;
			break;
		case 'p':
			if(pov == 0)
				pov = 1;
			else
				pov=0;
			break;
		case 27 :
			exit(0);
			break;
		default: 
			break;
	}
	glutPostRedisplay();
	//missile_flag = 0;
	//missileTrans_z = playerTrans_z;
}

void keyboard_s (int key, int x, int y)
{
        
    switch (key)
    {
        case GLUT_KEY_UP:

            //playerRotation_x += 6;
			playerTrans_y += 30.0;
        break;
        case GLUT_KEY_DOWN:
            //playerRotation_x -= 6;
			playerTrans_y -= 30.0;
        break;
        case GLUT_KEY_LEFT:
            //playerRotation_y -= 3.0;
			playerTrans_x -= 30.0;
        break;
        case GLUT_KEY_RIGHT:
            //playerRotation_y += 3.0;
			playerTrans_x += 30.0;
        break;
    }
	glutPostRedisplay();
}

bool isCollisionToPlayer() {
	return (emissileTrans_x >= -105 + playerTrans_x && emissileTrans_x <= 105 + playerTrans_x) &&
			(emissileTrans_y >= -60+playerTrans_y && emissileTrans_y <= 30+playerTrans_y) &&
			(emissileTrans_z >= playerTrans_z - 1 && emissileTrans_z <= playerTrans_z);
}

bool isCollisionToPlayerFromEnemy2() {
	return (emissile2Trans_x >= -105 + playerTrans_x && emissile2Trans_x <= 105 + playerTrans_x) &&
			(emissile2Trans_y >= -60+playerTrans_y && emissile2Trans_y <= 30+playerTrans_y) &&
			(emissile2Trans_z >= playerTrans_z - 1 && emissile2Trans_z <= playerTrans_z);
}

bool isCollisionToEnemy() {
	return (missileTrans_x >= -60 + enemyTrans_x && missileTrans_x <= 60 + enemyTrans_x) &&
			(missileTrans_y >= -25+enemyTrans_y && missileTrans_y <= 20+enemyTrans_y) &&
			(missileTrans_z >= enemyTrans_z - 1 && missileTrans_z <= enemyTrans_z);
}

bool isCollisionToEnemy2() {
	return (missileTrans_x >= -60 + enemy2Trans_x && missileTrans_x <= 60 + enemy2Trans_x) &&
			(missileTrans_y >= -25+enemyTrans_y+150 && missileTrans_y <= 20+enemyTrans_y+150) &&
			(missileTrans_z >= enemyTrans_z - 1 && missileTrans_z <= enemyTrans_z);
}

bool isCollisionPlayerEnemy1() {
	return (-105 + playerTrans_x <=  60 + enemyTrans_x && 105 + playerTrans_x >=  -60 + enemyTrans_x)&&
			(-60+playerTrans_y <= 20+enemyTrans_y && 30+playerTrans_y >= -25+enemyTrans_y)&&
			(playerTrans_z <= enemyTrans_z);
}

bool isCollisionPlayerEnemy2() {
	return (-105 + playerTrans_x <=  60 + enemy2Trans_x && 105 + playerTrans_x >=  -60 + enemy2Trans_x)&&
			(-60+playerTrans_y <= 20+enemyTrans_y+150 && 30+playerTrans_y >= -25+enemyTrans_y+150)&&
			(playerTrans_z <= enemyTrans_z);
}

void renderBitmapString(float x, float y, void *font,const char *string){
    const char *c;
	//glColor3f(1.0f,1.0f,1.0f);
    glRasterPos3f(x, y, -500);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

GLuint loadTexture(const char* filename) {
	BmpLoader bl(filename);	
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 bl.iWidth, bl.iHeight,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 bl.textureData);
	return textureId;
}
