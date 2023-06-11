#include <iostream>
#include <conio.h>

#include "cls.h"
#define N
#define ESC 27
#define MAX_SUB_MENU 3
#define MAX_COMMANDS 10

using namespace std;


project::Window *main_window_ptr;
project::SubWindow* iter;

char* allowCommands;
char userInput = 0;

void initialize_window();
bool showDialog();

int main()
{
	initialize_window();

	char* curentCommandSet = nullptr;
	char* selected_var = nullptr;



	do {
		//Zero oznacza, że użytkownik nie dokonał wyboru.
		userInput = 0;

		if (main_window_ptr->need_redraw) {
			//system("cls");
			project::draw_window(*main_window_ptr);
		}

		if (iter->w_type == project::DISPLAY || 
			iter->wstate == project::DEACTIVE) {
			iter = iter->next;
			continue;
		}

		switch (iter->w_type)
		{



		case project::CONTENT:
			if (iter->wstate == project::EDIT)
				continue;



		case project::CONTROL:

			switch (iter->wstate)
			{
			case project::ACTIVE:

			case project::INPUT:

				if(iter->commands != NULL ||
					userInput == 0)
					userInput = _getch();
				


				if(userInput == ESC) {
					if (iter->previous->w_type == project::CONTROL) {
						iter = project::remove_subwindow_at(*iter);
						main_window_ptr->need_redraw = true;
					}
					else {

						main_window_ptr->need_redraw = true;
						if(showDialog())
							return 0;
					}
						
				}

			default:
				break;

			}


		default:
			continue;
		}
		

		iter = iter->next;
	} while (true);
}
void initialize_window() {
	
	project::Window *wind;
	wind = new project::Window;
	wind->wTitle = "Magazyn Produkcyjny - Projekt";
	wind->need_redraw = true;

	main_window_ptr = wind;

	main_window_ptr->boundry = new project::Boundry(10, 0, 100, 25);

	project::SubWindow* header, *mainmenu, *test, *t2, *submenu;

	header = new project::SubWindow;
	mainmenu = new project::SubWindow;
	test = new project::SubWindow;
	t2 = new project::SubWindow;
	submenu = new project::SubWindow;

	header->content = "Magazyn Produkcyjny - Projekt\nHello World";
	//w pozycji (0, 0) ponieważ 
	//funkcja dodająca podokno stara utrzymać się wszystko w ramce głonego okna
	header->boundry = new project::Boundry(0, 0, 100, 4);
	header->boundry->background = true;
	header->w_type = project::DISPLAY;

	mainmenu->content = "It's my menu\n\t1. Opcja\n\t2. Opcja\n\t3. Opcja\n\t4. Opcja\n<- ESC \t\t\t\t\t\t\t\t";
	mainmenu->boundry = new project::Boundry(0, 4, 50, 20);
	mainmenu->w_type = project::CONTROL;
	mainmenu->wstate = project::INPUT;

	test->content = "Content";
	test->boundry = new project::Boundry(50, 4, 50, 20);
	test->w_type = project::CONTENT;

	t2->content = "Stopka";
	t2->boundry = new project::Boundry(0, 25, 100, 4);
	t2->w_type = project::DISPLAY;

	iter = project::add_subwindow_to_window(*main_window_ptr, *header);

	project::add_subwindow_to_window(*main_window_ptr, *mainmenu);
	project::add_subwindow_to_window(*main_window_ptr, *test);
	project::add_subwindow_to_window(*main_window_ptr, *t2);


	submenu->content = "It's my menu222\n\t1. asd\n\t2. asd\n<- ESC \t\t\t\t\t\t\t\t";
	submenu->boundry = mainmenu->boundry;
	submenu->w_type = project::CONTROL;
	submenu->wstate = project::INPUT;

	project::add_subwindow_after(*iter, *submenu);

	project::cls_start(*main_window_ptr);
	//project::draw_window(*main_window_ptr);
	//std::cout << *&main_window_ptr->content->boundry->position.X << std::endl;
}
bool showDialog() {

	project::Boundry tmp_bound = *main_window_ptr->boundry;
	project::SubWindow* n_win = new project::SubWindow;

	n_win->commands = "nNyY";
	n_win->title = "Tittle!";
	n_win->content = "Hello World!\n Yes\t\t\tNo";
	n_win->w_type = project::CONTROL;
	n_win->wstate = project::INPUT;
	n_win->boundry = new project::Boundry(25 % tmp_bound.width, tmp_bound.height % 14, 55, 4);

	iter = project::push_subwindow(*iter, *n_win);
	iter->redraw = true;
	project::draw_window(*main_window_ptr);
	iter = project::remove_subwindow_at(*iter);
	return true;
}