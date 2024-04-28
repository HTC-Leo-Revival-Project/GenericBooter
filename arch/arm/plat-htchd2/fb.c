//
// Thank you @zhuowei! <3
//

#include <string.h>

#define FONT_WIDTH 8

/* Don't change that */
#define MSM_SMI_BASE                0x00000000
#define MSM_SMI_SIZE                0x04000000

/* Begin SMI region */
/* First part of SMI is used for OEMSBL & AMSS */
#define MSM_PMEM_SMI_BASE        (MSM_SMI_BASE + 0x02500000)
#define MSM_PMEM_SMI_SIZE        0x01B00000

#define MSM_FB_BASE                0x02A00000
#define MSM_FB_SIZE                0x00600000

#define VIDEOBASE MSM_FB_BASE
#define WIDTH 480
#define HEIGHT 800

//Font taken from Minecraft.

#include "consolefont.c"

int Msm_console_row = 0;
int Msm_console_col = 0;

void Msm_framebuffer_drawRect(int beginX, int beginY, int width, int height, int color) {
	for (int y = beginY; y < beginY + height; y++) {
		for (int x = beginX; x < beginX + width; x++) {
			int loc = VIDEOBASE + (((y * WIDTH) + x) * 4);
			*((int*) loc) = color;
		}
	}
}

void Msm_framebuffer_setPixel(int x, int y, int color) {
	int loc = VIDEOBASE + (((y * WIDTH) + x) * 4);
	*((int*) loc) = color;
}

int Msm_framebuffer_drawCharacter(char mychar, int screenr, int screenc) {
	int index = ((int) mychar) * FONT_WIDTH;
	int rr, cc;
	for (rr = 0; rr < FONT_WIDTH; rr++) {
		int myrow = console_font[index + rr];
		for (cc = 0; cc < FONT_WIDTH; cc++) {
			int thisPixel = (myrow >> (FONT_WIDTH - 1 - cc)) & 0x1;
			if (thisPixel) {
				Msm_framebuffer_setPixel(screenc + cc, screenr + rr, 0xffffff);
			}
		}
	}
	return console_font_widths[(int) mychar] + 1;
}

void Msm_console_init(void) {
	memset((void*)0x02B00000, 0xcc, 480 * 800 * 2);
	Msm_console_col = 0;
	Msm_console_row = 0;
	Msm_framebuffer_drawRect(0, 0, WIDTH, HEIGHT, 0);
}

void Msm_console_putchar(char c) {
	if (c == '\n') {
		Msm_console_row += (FONT_WIDTH + 1);
		Msm_console_col = 0;
		return;
	}
	Msm_console_col += Msm_framebuffer_drawCharacter(c, Msm_console_row, Msm_console_col);
	if (Msm_console_col + FONT_WIDTH > WIDTH) {
		Msm_console_row += (FONT_WIDTH + 1);
		Msm_console_col = 0;
	}
}
