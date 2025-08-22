#include <nds.h>
#include <stdio.h>
#include <math.h> 
#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#define NUM_STARS 2048
#define MAX_Z -40.0
const short song_order[] = { MOD_ZALZA };
const short song_volumes[] = { 2048 };

typedef struct 
{
	float x,y,z;
	float speed;
 
}Star;


 
Star stars[NUM_STARS];
void InitStars(void)
{
	int i;
 
	for(i = 0; i < NUM_STARS; i++)
	{
		stars[i].x = (rand() % 200 - 64);
		stars[i].y = (rand() % 200 - 33);
		stars[i].z = MAX_Z;
		stars[i].speed = rand() % 5 + 0.4;
	}
}
 

void DrawStar(Star* star)
{
	glLoadIdentity();								// Reset The Current Modelview Matrix


	glTranslatef(star->x,star->y,star->z);					
	glColor3f(1.0f,1.0f,1.0f);						// Set The Color To white
	glBegin(GL_QUADS);								// Draw A Quad
		glVertex3f(-0.1f, 0.1f, 0.0f);				// Top Left
		glVertex3f( 0.1f, 0.1f, 0.0f);				// Top Right
		glVertex3f( 0.1f,-0.1f, 0.0f);				// Bottom Right
		glVertex3f(-0.1f,-0.1f, 0.0f);				// Bottom Left
	glEnd();	
}


void MoveStar(Star* star)
{
	star->z += star->speed;
	if(star->z >= 0)
	{
		star->z = MAX_Z;
		star->x = (rand() % 200 - 47);
		star->y = (rand() % 200 - 45);
		star->speed = rand () % 4 + 0.4;
		
	}
}


int main() {
	
	consoleDemoInit();
	BG_PALETTE_SUB[0] = RGB15(0,0,0);
	iprintf("\n\n\nShyStudios - 2014\ntekilla groove - zalza - 1999");
	swiWaitForVBlank();
	// initialize the geometry engine
	glInit();	

	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);
	//mmSelectMode(2);
	//map some vram to background for printing
	vramSetBankC(VRAM_C_MAIN_BG_0x06000000);
 
	consoleInit(0,1, BgType_Text4bpp, BgSize_T_256x256, 31,0, true, true);

	//put bg 0 at a lower priority than the text background
	bgSetPriority(0, 1);
  

 
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
 
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);
 
	// Set our viewport to be the same size as the screen
	glViewport(0,0,255,191);
 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 100);
 
	//ds specific, several attributes can be set here	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
 
	// Set the current matrix to be the model matrix
	glMatrixMode(GL_MODELVIEW);
	
	// sound stuff 
	mmInitDefaultMem( (mm_addr)soundbank_bin );
	mmLoad( MOD_ZALZA );
 	mmSetModuleVolume( song_volumes[0] );
	mmStart( song_order[0], MM_PLAY_LOOP );
	
	InitStars();
	int i;

	while (1) {
 
		for(i = 0; i < NUM_STARS; i++)
		{
			DrawStar(&stars[i]); 
			MoveStar(&stars[i]);

		}	
		// flush to screen	
		glFlush(0);
 
		// wait for the screen to refresh
		swiWaitForVBlank();



	}
 
	return 0;
}
