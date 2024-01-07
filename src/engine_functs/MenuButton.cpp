#include "..\include.h"

MenuButton::MenuButton() {

	int width = 0;

}

MenuButton::MenuButton(
	int width, int height, int posx, int posy, int windowwidth, int windowheight,
	int text_aling, int text_justify, _In_opt_ int icon_align, _In_opt_ int icon_justify,
	int font_width, int font_height, LPCWSTR font_family,
	COLORREF border_color, COLORREF text_color, COLORREF background_color,
	COLORREF hover_border, COLORREF hover_text, COLORREF hover_bg,
	_In_opt_ HDC hdc, LPCWSTR text
) {

	MenuButton::width = width;
	MenuButton::height = height;
	MenuButton::posx = posx;
	MenuButton::posy = posy;
	MenuButton::windowwidth = windowwidth;
	MenuButton::windowheight = windowheight;
	MenuButton::text_align = text_aling;
	MenuButton::text_justify = text_justify;
	if (icon_align != NULL) MenuButton::icon_align = icon_align;
	if (icon_justify != NULL) MenuButton::icon_justify = icon_justify;
	MenuButton::border_color = border_color;
	MenuButton::text_color = text_color;
	MenuButton::background_color = background_color;
	if (hdc != NULL) MenuButton::hdc = hdc;
	MenuButton::text = text;
	MenuButton::hover_border = hover_border;
	MenuButton::hover_text = hover_text;
	MenuButton::hover_bg = hover_bg;
	
	MenuButton::font = CreateFontW(font_width, font_height, 0, 0,
		FW_EXTRALIGHT, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, font_family);

}

void MenuButton::drawBox(bool hovered) {

	if (hdc != NULL){

		COLORREF border_col, text_col, bg_col;

		if (hovered) {

			border_col = MenuButton::hover_border;
			text_col = MenuButton::hover_text;
			bg_col = MenuButton::hover_bg;

		}
		else {

			border_col = MenuButton::border_color;
			text_col = MenuButton::text_color;
			bg_col = MenuButton::background_color;

		}

		SelectObject(MenuButton::hdc, MenuButton::font);

		if (MenuButton::posx == NULL) posx = MenuButton::windowwidth / 2;
		if (MenuButton::posy == NULL) posy = MenuButton::windowheight / 2;

		SetTextColor(hdc, text_col);
		SetBkMode(hdc, TRANSPARENT);
		RECT rect = { posx - (width / 2), posy,  posx + (width / 2), posy + height };

		if (MenuButton::background_color != NULL) {

			HBRUSH brush = CreateSolidBrush(bg_col);
			FillRect(hdc, &rect, brush);
			DeleteObject(brush);

		}

		DrawText(hdc, MenuButton::text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		for (int i = 0; i < (width / 2); i++) SetPixel(hdc, posx - i, posy, border_col);
		for (int i = 0; i < (width / 2); i++) SetPixel(hdc, posx + i, posy, border_col);
		for (int i = 0; i < height + 1; i++) SetPixel(hdc, posx - (MenuButton::width / 2), posy + i, border_col);
		for (int i = 0; i < height + 1; i++) SetPixel(hdc, posx + (MenuButton::width / 2), posy + i, border_col);
		for (int i = 0; i < (width / 2); i++) SetPixel(hdc, posx - i, posy + height, border_col);
		for (int i = 0; i < (width / 2); i++) SetPixel(hdc, posx + i, posy + height, border_col);
	}

}

int MenuButton::getPosX() {

	if (MenuButton::posx != NULL) return MenuButton::posx;
	else return (MenuButton::windowwidth / 2);

}

int MenuButton::getPosY() {

	if (MenuButton::posy != NULL) return MenuButton::posy;
	else return (MenuButton::windowheight / 2);
	return MenuButton::posy;

}

int MenuButton::getWidth() {

	return MenuButton::width;

}

int MenuButton::getHeight() {

	return MenuButton::height;

}

void MenuButton::setHDC(HDC hdc) {

	MenuButton::hdc = hdc;

}