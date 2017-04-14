/*
			The^day^of^DooM

			Borland C++ 3.1
				  &&
			  Open Watcom

	Create date:   XX.XX.2005
	Last Update:   28.03.2007
*/

#ifndef __MOUSE_H__
	#define __MOUSE_H__

	#define UsesMouse

#ifndef __OPEN_WATCOM__
	#define BORLANDC
#endif

typedef struct {
   int x,y,button;
} MOUSE;

MOUSE mouse;

#ifdef UsesMouse
	#ifdef __OPEN_WATCOM__
		#define Mouse {													\
			union REGS regs;												\
			regs.w.ax = 0x03;												\
			int86(0x33, &regs, &regs);									\
			mouse.x = regs.w.cx;											\
			mouse.y = regs.w.dx;											\
			mouse.button = regs.w.bx;									\
		};

		#define InitMouse {											  	\
			union REGS regs;												\
			regs.w.ax = 0;                            			\
			int86(0x33, &regs, &regs);									\
			if(regs.w.ax != 0xffff) {									\
				PutStrXY("Mouse NOT installed...", 10, 1, 0);	\
				PutStrXY("Press any key to exit...", 10, 1, 1); \
				while(kbhit()) getch();									\
				getch();                              				\
				exit(1);                              				\
			}                                         			\
			mouse.x=mouse.y=mouse.button = -1;        			\
		};

		#define ShowMouse {                                   \
			union REGS regs;                         				\
			regs.w.ax = 1;                            			\
			int86(0x33, &regs, &regs);                			\
		};

		#define HideMouse {                                   \
			union REGS regs;												\
			regs.w.ax = 2;													\
			int86(0x33, &regs, &regs);									\
		};
	#else
		#ifdef BORLANDC
			#define Mouse {                                 	\
				int x,y,button;                              	\
				_asm mov AX,0x03;                            	\
				_asm int 0x33;                               	\
				_asm mov button,BX;                          	\
				_asm mov x,CX;                               	\
				_asm mov y,DX;                               	\
				mouse.x=x;  mouse.y=y;  mouse.button=button; 	\
			};

			#define InitMouse {                             	\
				mouse.x=mouse.y=mouse.button=-1;             	\
				_asm mov AX,0x00;                            	\
				_asm int 0x33;                               	\
			};

			#define ShowMouse {                             	\
				_asm mov AX,0x01;                            	\
				_asm int 0x33;                               	\
			};

			#define HideMouse {                             	\
				_asm mov AX,0x02;                            	\
				_asm int 0x33;                               	\
			};
		#endif
	#endif
#else
	#define Mouse;
	#define ShowMouse;
	#define HideMouse;
	#define InitMouse       mouse.x=mouse.y=mouse.button=-1;
#endif

#endif // __MOUSE_H__