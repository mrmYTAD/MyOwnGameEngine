#define MENUBUTTON_H
#include "..\include.h"

class MenuButton {

public:

	MenuButton();

	MenuButton(
		int width, int height, int posx, int posy, int windowwidth, int windowheight,
		int text_aling, int text_justify, _In_opt_ int icon_align, _In_opt_ int icon_justify,
		int font_width, int font_height, LPCWSTR font_family,
		COLORREF border_color, COLORREF text_color, _In_opt_ COLORREF background_color,
		COLORREF hover_border, COLORREF hover_text, COLORREF hover_bg,
		_In_opt_ HDC hdc, LPCWSTR text
	);

	void drawBox(bool hovered);

	bool isMouseOverButton(short x, short y);

	int getPosX();
	int getPosY();
	int getWidth();
	int getHeight();

	void setHDC(HDC hdc);

private:

	int width, height, posx, posy, windowwidth, windowheight, text_align, text_justify, icon_align, icon_justify;
	COLORREF border_color, text_color, background_color,
		hover_border, hover_text, hover_bg;
	HFONT font;
	LPCWSTR text;
	HDC hdc;
	//	for the icon custom class where it will be drawn

};