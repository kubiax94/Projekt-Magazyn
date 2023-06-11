#pragma once
#include <windows.h>

namespace project {

	extern int menu_state;

	enum windowType {DISPLAY, CONTROL, CONTENT, DIALOG};
	enum windowState {NONE, INPUT, EDIT ,ACTIVE, DEACTIVE};

	
	void write_character_on_screen(int x, int y, char c);
	void set_cursor_position(int x, int y);

	struct Boundry {

		COORD position;
		int width;
		int height;

		const unsigned char* boundry_chars;
		const unsigned char* boundry_corners_chars;

		bool background = false;

		Boundry() : position({ 0,0 }), width(0), height(0), boundry_chars(nullptr), boundry_corners_chars(nullptr), background(false) {}
		Boundry(int x, int y, unsigned int width, unsigned int height);
	};

	struct SubWindow {

		int id = 0;
		windowType w_type;
		windowState wstate = ACTIVE;

		bool redraw = true;
		Boundry* boundry;

		const char* title;
		const char* content;
		const char *commands;

		SubWindow* next = nullptr;
		SubWindow* previous = nullptr;
	};


	struct Window {

		
		const char* wTitle;

		Boundry* boundry;
		SubWindow* content;		

		bool need_redraw;
		
		Window() : content(nullptr), need_redraw(false), boundry(new Boundry(0, 0, 118, 40)) {};

	};
	void cls_start(Window& w_inst);
	SubWindow* add_subwindow_to_window(Window& window, SubWindow& toadd);
	SubWindow* add_subwindow_after(SubWindow& iter, SubWindow& toadd);
	SubWindow* add_subwindow_before(SubWindow& iter, SubWindow& toadd);
	void create_window();
	void draw_window(Window &window);
	SubWindow* push_subwindow(SubWindow &cur_win, SubWindow& toadd);
	SubWindow* remove_subwindow_at(SubWindow &iter);

}