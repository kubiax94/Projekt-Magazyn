#include "cls.h"
#include <iostream>
#define EMPTY '\0'

using namespace std;

bool _error_handled = true;
const unsigned char _v_line = 186;
const unsigned char _h_line = 205;
const unsigned char _background = 219;
const unsigned char _empty_space = ' ';



const unsigned char _box_corners[] = { 201, 187, 188, 200 };
const unsigned char _window_border_walls[] = {186, 205};

unsigned int _console_cols = 0,
			 _console_rows = 0;

unsigned int _deflaut_color = 1;

HANDLE _CCONSOLE_HANDLER;
_CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

void set_menu_color(int color) {

	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(console_color, &consoleInfo);
}

unsigned int calculate_content_length(const char* content, int *largest_text = nullptr) {

	unsigned int count = 0,
				 current_row_length = 0,
				 max_row_length = 0;

	while(*content) {
		switch (*content)
		{
		case('\t'):
			current_row_length += 4;
			count += 4;
			break;

		case('\n'):
			current_row_length = 0;
			

		default:
			count++;
			current_row_length++;
			break;
		}

		if (current_row_length > max_row_length) {
			max_row_length = current_row_length;
		}
		content++;
	}

	if (largest_text != nullptr )
		*largest_text = max_row_length;

	//Reset wskaŸnika do pocz¹tkowego stanu.
	content -= count;

	return count;
}


void showContent(project::SubWindow &window) {

	switch (window.w_type)
	{
	case project::DISPLAY:
		break;

	case project::CONTROL:
		break;

	default:
		break;
	}

}


void clear_boundry_content(project::Boundry bound) {
	
	for (int iy = bound.position.Y + 1; iy < bound.position.Y+bound.height-1; iy++) {
		for (int ix = bound.position.X + 1; ix < bound.position.X+bound.width-1; ix++) {
			project::write_character_on_screen(ix, iy, ' ');
		}
	}

}


/// <summary>
/// Tworzy "pude³ko" na treœæ, 
/// 
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="width">szerokoœæ pude³ka max 120</param>
/// <param name="height">wysokoœæ pude³ka max 30</param>
/// <param name="content">ci¹g znaków do wyœwietlenia</param>
void create_content_box_from_boundry(project::Boundry bound, const char *content, bool background = false, const unsigned char* corners_char = _box_corners) {

	int c_start_poistion = 0,
		content_rows = 0,
		content_length = calculate_content_length(content, &content_rows),
		content_height = 0,
		max_width = bound.position.X + bound.width,
		max_height = bound.position.Y + bound.height;

	unsigned int row_length = 0;

	if (content_rows == content_length) {
		c_start_poistion = (bound.width - content_length) / 2 + bound.position.X;
	}
	else
	{
		c_start_poistion = (bound.width - content_rows) / 2 + bound.position.X;
	}



	for (int iy = bound.position.Y; iy <= max_height; iy++) {
		for (int ix = bound.position.X; ix <= max_width; ix++) {

			if (iy == bound.position.Y && ix != max_width ||
				iy == max_height && ix != max_width) {
				project::write_character_on_screen(ix, iy, _h_line);
				continue;
			}


			if (*content &&
				ix == c_start_poistion + row_length &&
				iy == bound.position.Y + 1 + content_height)
			{
				if (*content != '\n') {
					if (*content != '\t')
						project::write_character_on_screen(c_start_poistion + row_length++, iy + content_height, *content);
					else {
						row_length += 4;
						project::write_character_on_screen(c_start_poistion + row_length, iy + content_height, *content);
					}
					content++;
					continue;
				}
				else
				{
					row_length = 0;
					++content_height;
					++content;
					continue;
				}
			}

			if (background)
				project::write_character_on_screen(ix, iy, _background);
		}

		if (iy != bound.position.Y) {
			project::write_character_on_screen(bound.position.X, iy, bound.boundry_chars[0]);
			project::write_character_on_screen(max_width, iy, bound.boundry_chars[0]);
		}
	}

	//DODAWANIE ROGÓW
	project::write_character_on_screen(bound.position.X, bound.position.Y, bound.boundry_corners_chars[0]);
	project::write_character_on_screen(max_width, bound.position.Y, bound.boundry_corners_chars[1]);
	project::write_character_on_screen(max_width, max_height, bound.boundry_corners_chars[2]);
	project::write_character_on_screen(bound.position.X, max_height, bound.boundry_corners_chars[3]);

	content -= content_length;
}

void print_window_to_screen_info(project::Window *window) {

	project::SubWindow* current = window->content;
	std::string test = "";
	

	 do {

		 cout << "Id: " << current->id << ' ';
		 cout << "Content:  " << current->content << ' ';
		 cout << '{' << current->boundry->position.X << ',';
		 cout << current->boundry->position.Y << "} ";
		 cout << "W: " << current->boundry->width;
		 cout << "H: " << current->boundry->height << endl;

		 current = current->next;

	 } while (current->previous != NULL);

}
project::SubWindow* project::add_subwindow_after(SubWindow &iter, SubWindow& toadd) 
{
	SubWindow *tmp_iter = &iter;

	do {

		if (tmp_iter->w_type == toadd.w_type &&
			tmp_iter->next->w_type != toadd.w_type) {

			toadd.previous = tmp_iter;
			toadd.next = tmp_iter->next;

			tmp_iter->next = &toadd;
			tmp_iter->next->previous = tmp_iter;

			tmp_iter->wstate = DEACTIVE;

			return &toadd;
		}

		tmp_iter = tmp_iter->next;
	} while (tmp_iter != &iter);

	return push_subwindow(iter, toadd);
}

void project::draw_window(Window &window) {

	SubWindow *cur = window.content;
	Boundry correct_boundry;

	do {

		if (cur->wstate == DEACTIVE || !cur->redraw) {
			cur = cur->next;
			continue;
		}

		correct_boundry.position.X =  window.boundry->position.X + cur->boundry->position.X;
		correct_boundry.position.Y =  window.boundry->position.Y + cur->boundry->position.Y;
		correct_boundry.boundry_chars = cur->boundry->boundry_chars;
		correct_boundry.boundry_corners_chars = cur->boundry->boundry_corners_chars;
		correct_boundry.width = cur->boundry->width;
		correct_boundry.height = cur->boundry->height;

		clear_boundry_content(correct_boundry);
		create_content_box_from_boundry(correct_boundry, cur->content, cur->boundry->background);
		cur->redraw = false;

		cur = cur->next;
	} while (cur->previous != NULL);

	window.need_redraw = false;
}



project::SubWindow* project::add_subwindow_to_window(Window &window, SubWindow &toadd) {

	if (window.content == NULL) {
		window.content = &toadd;
		toadd.next = window.content;
		return &toadd;
	}
	else {
		project::push_subwindow(*window.content, toadd);
	}

	return &toadd;
}

project::SubWindow* project::push_subwindow(SubWindow &cur_win, SubWindow &toadd) {

	if (cur_win.next->previous == NULL) {

		toadd.id = cur_win.id + 1;

		toadd.next = cur_win.next;
		toadd.previous = &cur_win;
		
		cur_win.next = &toadd;

		return &toadd;
	}

	if (cur_win.previous != cur_win.next) {
		push_subwindow(*cur_win.next, toadd);
	}

	return &toadd;
}

project::SubWindow* project::remove_subwindow_at(SubWindow &iter) {
	SubWindow* tmp = &iter;

	if (tmp->previous != NULL) {

		switch (tmp->previous->w_type) {

		case CONTROL:
			tmp->previous->wstate = INPUT;
			tmp->wstate = DEACTIVE;

			tmp->previous->redraw = true;
			break;

		default:
			break;

		}

		if (tmp->next->previous == NULL) {
			tmp->previous->next = tmp->next;

			tmp = tmp->previous;
			delete& iter;

			return tmp->next;
		}

		tmp->previous->next = tmp->next;
		tmp->next->previous = tmp->previous;

		tmp = tmp->previous;

		delete &iter;
	}

	return tmp;
}

void project::cls_start(Window& w_inst) {

	_CCONSOLE_HANDLER = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitleA(w_inst.wTitle);

	CONSOLE_CURSOR_INFO     console_cursor_info;
	console_cursor_info.bVisible = FALSE;
	console_cursor_info.dwSize = 1;

	SetConsoleCursorInfo(_CCONSOLE_HANDLER, &console_cursor_info);

}

void project::write_character_on_screen(int x, int y, char c) 
{
	_CCONSOLE_HANDLER = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD pos;
	pos.X = x;
	pos.Y = y;

	SetConsoleCursorPosition(_CCONSOLE_HANDLER, pos);
	cout << c;
}



project::Boundry::Boundry(int x, int y, unsigned int width, unsigned int height) 
{

	this->position.X = x;
	this->position.Y = y;

	this->width = width;
	this->height = height;

	this->boundry_chars = *&_window_border_walls;
	this->boundry_corners_chars = *&_box_corners;
}

