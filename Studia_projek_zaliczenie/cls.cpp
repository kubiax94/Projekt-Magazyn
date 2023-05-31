#include "cls.h"
#include <iostream>
#include <iomanip>
#include <windows.h>

using namespace std;


struct Menu {

};


bool _error_handled = true;
const unsigned char _v_line = 186;
const unsigned char _h_line = 205;
const unsigned char _background = 219;



const unsigned char _box_corners[] = { 201, 187, 188, 200 };

unsigned int _console_cols = 0,
			 _console_rows = 0;




HANDLE _CCONSOLE_HANDLER;
_CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

void set_menu_color(int color) {

	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(console_color, &consoleInfo);
}


/// <summary>
/// Tworzy "pude³ko" na treœæ, 
/// 
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="width">szerokoœæ pude³ka max 240</param>
/// <param name="height">wysokoœæ pude³ka max 9001</param>
/// <param name="content">ci¹g znaków do wyœwietlenia</param>
void create_content_box(int x, int y, int width, int height, const char* content, bool background = false, const unsigned char* corners_char = _box_corners) {

	std::string text_content = content;

	for (char c = *content; c; c = *++content)
	{
		cout << c;
	}

	unsigned int content_start_poistion = 0,
				 content_length = text_content.length(),
				 max_width = x + width,
				 max_height = y + height;


	content_start_poistion = (width - content_length)/2 + x - 1;

	for (int iy = y; iy <= max_height; iy++) {

		for (int jx = x; jx <= max_width; jx++) {

			if (iy == y && jx != max_width ||
				iy == max_height && jx != max_width) {
				project::write_character_on_screen(jx, iy, _h_line);
				continue;
			}

			if (jx == content_start_poistion && iy == y+1) {

				cout << content;
				jx += content_length;
				continue;
			}

			if(background)
				project::write_character_on_screen(jx, iy, _background);
		}

		if (iy != y) {
			project::write_character_on_screen(x, iy, _v_line);
			project::write_character_on_screen(max_width, iy, _v_line);
		}

	}

	

	//DODAWANIE ROGÓW
	project::write_character_on_screen(x, y, corners_char[0]);
	project::write_character_on_screen(max_width, y, corners_char[1]);
	project::write_character_on_screen(max_width, max_height, corners_char[2]);
	project::write_character_on_screen(x, max_height, corners_char[3]);


}

void project::cls_start() {
	_CCONSOLE_HANDLER = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitleA("Magazyn Produkcyjny");

	create_content_box(10, 0, 100, 3, "Magazyn Produkcyjny - Projekt",true);

	const unsigned char _test[] = { 204, 185, 188, 200 };
	create_content_box(10, 3, 100, 26, "", false, _test);

	const unsigned char _test1[] = { 204, 203, 188, 204 };
	create_content_box(10, 3, 40, 23, "test\ntest\ntest\n", false, _test1);

	const unsigned char _test2[] = { 203, 185, 185, 202 };
	create_content_box(50, 3, 60, 23, "", false, _test2);
	getchar();

}

void project::show_menu() {

	

	//cout << setfill(_h_line) << setw(120) << '\n';
	//cout << "\t\t"<< _v_line << "\t\t\t\t" << "Magazyn Produkcyjny - Projekt" << "\t\t\t\t|" << '\n';
	//cout << "\t\t" << setfill('-') << setw(86) << '\n';
	set_menu_color(1);
}

void project::write_character_on_screen(int x, int y, char c) 
{
	COORD pos;
	pos.X = x;
	pos.Y = y;

	SetConsoleCursorPosition(_CCONSOLE_HANDLER, pos);
	cout << c;
}

