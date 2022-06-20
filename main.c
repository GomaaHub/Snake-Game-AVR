#include "Library/types.h"
#include "DIO_Interface.h"
#include "LCD_Interface.h"
#include "KPD_Interface.h"
#include "KPD_Config.h"
#include "Snake_Interface.h"
#include <avr/delay.h>
#include "stdlib.h"

#define H 4
#define W 16

static s32 Field[H][W];
u8 Gy,Head,Tail,Frogs,Game,score=0,a,b,key,dir;
s8 x,y,r;
u8 key_pressed;

u8 glyph0[8]={0x00,
		  0x00,
		  0x00,
		  0x00,
		  0x0E,
		  0x0E,
		  0x0E,
		  0x00};
u8 glyph1[8]={ 0x00,
		  0x00,
		  0x00,
		  0x00,
		  0x0A,
		  0x04,
		  0x0A,
		  0x00};
u8 glyph2[8]={  0x0E,
		  0x0E,
		  0x0E,
		  0x00,
		  0x00,
		  0x00,
		  0x00,
		  0x00};
u8 glyph3[8]={  0x0E,
		  0x0E,
		  0x0E,
		  0x00,
		  0x0E,
		  0x0E,
		  0x0E,
		  0x00};
u8 glyph4[8]={  0x0E,
		  0x0E,
		  0x0E,
		  0x00,
		  0x0A,
		  0x04,
		  0x0A,
		  0x00};
u8 glyph5[8]={0x0A,
		  0x04,
		  0x0A,
		  0x00,
		  0x00,
		  0x00,
		  0x00,
		  0x00};
u8 glyph6[8]={ 0x0A,
		  0x04,
		  0x0A,
		  0x00,
		  0x0E,
		  0x0E,
		  0x0E,
		  0x00};
u8 glyph7[8]={ 0x0A,
		  0x04,
		  0x0A,
		  0x00,
		  0x0A,
		  0x04,
		  0x0A,
		  0x00};



void main(void)
{
	DIO_SetPortDirection(DIO_PORTB,DIO_PORT_OUTPUT);	// LCD PORT

	DIO_SetPortDirection(KPD_PORT,0x0F);				// 4x4 KEYPAD PORT
	DIO_SetPortValue(KPD_PORT,0xF0);					// 4x4 KEYPAD PORT

	LCD_Init();
	LCD_DISPLAY_CONTROL(1,0,0);



	LCD_CreateNewChar(glyph0,0);
	LCD_GoToXY(0,0);
	LCD_CreateNewChar(glyph1,1);
	LCD_GoToXY(0,0);
	LCD_CreateNewChar(glyph2,2);
	LCD_GoToXY(0,0);
	LCD_CreateNewChar(glyph3,3);
	LCD_GoToXY(0,0);
	LCD_CreateNewChar(glyph4,4);
	LCD_GoToXY(0,0);
	LCD_CreateNewChar(glyph5,5);
	LCD_GoToXY(0,0);
	LCD_CreateNewChar(glyph6,6);
	LCD_GoToXY(0,0);
	LCD_CreateNewChar(glyph7,7);
	LCD_GoToXY(0,0);





	LCD_Print_String("Welcome to Snake");
	while(KPD_GET_KEY_PRESSED()==0xFF);

while(1)
{
	SnakeInit();

	while(Game==0)
	{
		PrintSnake();
		GenerateFrog();
		Movement();
		TailRemove();
	}


	LCD_CLEAR();
	LCD_GoToXY(0,0);
	LCD_Print_String("You Lost!");
	LCD_GoToXY(0,1);
	LCD_Print_String("Score:");
	LCD_Print_Integer(score);


	while(KPD_GET_KEY_PRESSED()==0xFF);
}
}


void SnakeInit(void)
{
	for(u8 i=0;i<H;i++)
	{
		for(u8 j=0;j<W;j++)
		{
			Field[i][j]=0;
		}
	}

	x=H/2;
	y=W/2;

	Gy=y;

	Head=5;
	Tail=1;
	Game=0;
	Frogs=0;
	dir='6';
	score=0;

	for(u8 i=0;i<Head;i++)
	{
		Gy++;
		Field[x][Gy-Head]=i+1;
	}

}

void PrintSnake(void)
{
	LCD_GoToXY(0,0);

	for(u8 i=0;i<H;i=i+2)
	{
			for(u8 j=0;j<W;j++)
			{
				if(Field[i][j] == 0 && Field[i+1][j] == 0)
				{
					LCD_Print_Char(' ');
				}
				else if(Field[i][j] == 0 && Field[i+1][j] > 0)
				{
					LCD_Print_Char(0);
				}
				else if((Field[i][j] == 0) && (Field[i+1][j] == -5))
				{
					LCD_Print_Char(1);
				}
				else if(Field[i][j] > 0 && Field[i+1][j] == 0)
				{
					LCD_Print_Char(2);
				}
				else if(Field[i][j] > 0 && Field[i+1][j] > 0)
				{
					LCD_Print_Char(3);
				}
				else if(Field[i][j] > 0 && Field[i+1][j] == -5)
				{
					LCD_Print_Char(4);
				}
				else if(Field[i][j] == -5 && Field[i+1][j] == 0)
				{
					LCD_Print_Char(5);
				}
				else if(Field[i][j] == -5 && Field[i+1][j] > 0)
				{
					LCD_Print_Char(6);
				}
				else if(Field[i][j] == -5 && Field[i+1][j] == -5)
				{
					LCD_Print_Char(7);
				}

			}


		}

}



void GenerateFrog(void)
{

	a = (rand() % 4);
	b = (rand() % 16);


	if(Frogs==0 && Field[a][b]==0)
	{
		Field[a][b]=-5;
		Frogs=1;
	}
}

void Movement(void)
{
	key=KPD_GET_KEY_PRESSED();

	if(key=='2' || key=='4' || key=='6' || key=='8')
	{
		dir=key;
	}

		switch(dir)
		{
		case '6':
		{
			y++;

			if(y==W)
			{
				y=0;
			}

			if(Field[x][y]!=0 && Field[x][y]!=-5)
			{
				GameOver();
			}

			if(Field[x][y]==-5)
			{
				Frogs=0;
				Tail--;
				score++;
			}



			break;
		}
		case '4':
		{
			y--;

			if(y==-1)
			{
				y=(W-1);
			}

			if(Field[x][y]!=0 && Field[x][y]!=-5)
			{
				GameOver();
			}



			if(Field[x][y]==-5)
			{
				Frogs=0;
				Tail--;
				score++;
			}

			break;
		}
		case '2':
		{
			x--;

			if(x<0)
			{
				x=(H-1);
			}

			if(Field[x][y]!=0 && Field[x][y]!=-5)
			{
				GameOver();
			}

			if(Field[x][y]==-5)
			{
				Frogs=0;
				Tail--;
				score++;
			}

			break;
		}
		case '8':
		{
			x++;

			if(x==H)
			{
				x=0;
			}

			if(Field[x][y]!=0 && Field[x][y]!=-5)
			{
				GameOver();
			}

			if(Field[x][y]==-5)
			{
				Frogs=0;
				Tail--;
				score++;
			}


			break;
		}
		}

			Head++;
			Field[x][y]=Head;
}

void TailRemove(void)
{
	for(u8 i=0;i<H;i++)
	{
		for(u8 j=0;j<W;j++)
		{
			if(Field[i][j]==Tail)
			{
				Field[i][j]=0;
			}
		}
	}

	Tail++;
}

void GameOver(void)
{
	Game=1;
}
