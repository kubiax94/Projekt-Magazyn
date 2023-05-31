#pragma once

namespace project {

	extern int menu_state;

	void cls_start();
	void show_menu();
	void write_character_on_screen(int x, int y, char c);
	void set_cursor_position(int x, int y);
}