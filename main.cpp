// These should include everything you might use
#include <FL/Fl.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <string>
#include <math.h>
#include <time.h>
#include <chrono>
#include <vector>
#include <iostream>
#include <random>
#include <array>
#include "player.hpp"
//#include <conio.h> //for the getch() function
using namespace std;

const int windowWidth = 1200;
const int windowHeight = 800;
const double refreshPerSecond = 60;
// regarder pour les variable globales
struct Point {
  int x, y;
};

/*--------------------------------------------------
 
Text class.

Use to display text on the screen. For example:
	
Text("Hello!", {250, 250}).draw();

will create a text temporary instance with the
string "Hello!" centered at 250, 250 and will
call draw on the temporary.

It should have all the features you need and you
should not need to edit it.

--------------------------------------------------*/

class Text {
  string s;
  Point center;
  int fontSize;
  Fl_Color color;

 public:
  // Constructor
  Text(string s, Point center, int fontSize = 10, Fl_Color color = FL_BLACK):
    s{s}, center{center}, fontSize{fontSize}, color{color} {}

  // Draw
  void draw();

  // Setters and getters
  string getString() {
    return s;
  }
  void setString(const string &newString) {
    s = newString;
  }
  int getFontSize() {
    return fontSize;
  }
  void setFontSize(int newFontSize) {
    fontSize = newFontSize;
  }
  Point getCenter() {
    return center;
  }
  void setCenter(Point newCenter) {
    center = newCenter;
  }
};

void Text::draw() {
  fl_color(color);
  fl_font(FL_HELVETICA, fontSize);
  int width, height;
  fl_measure(s.c_str(), width, height, false);
  fl_draw(s.c_str(), center.x-width/2, center.y-fl_descent()+height/2);
}

/*--------------------------------------------------

Rectangle class.

Use to display a fillend-in rectangle on the screen
with different colors for the fill and the border

It should have all the features you need and you
should not need to edit it.

--------------------------------------------------*/

class Rectangle {
  Point center;
  int w, h;
  Fl_Color fillColor, frameColor;

 public:
  Rectangle(Point center, int w, int h,
            Fl_Color frameColor = FL_BLACK,
            Fl_Color fillColor = FL_WHITE);
  void draw_ground();
  void draw_wall();
  void draw_box();
  void draw_player();
  void draw_box_ok();
  void draw_target();
  void draw_teleportation();
  void setFillColor(Fl_Color newFillColor);
  Fl_Color getFillColor() {
    return fillColor;
  }
  void setFrameColor(Fl_Color newFrameColor);
  Fl_Color getFrameColor() {
    return frameColor;
  }
  void setWidth(int neww) {
    w = neww;
  }
  void setHeight(int newh) {
    h = newh;
  }
  int getWidth() {
    return w;
  }
  int getHeight() {
    return h;
  }
  bool contains(Point p);
  Point getCenter() {
    return center;
  }
};

Rectangle::Rectangle(Point center, int w, int h,
                     Fl_Color frameColor,
                     Fl_Color fillColor):
  center{center}, w{w}, h{h}, fillColor{fillColor}, frameColor{frameColor} {}

void Rectangle::draw_teleportation(){
  Fl_PNG_Image png_g("ground.png");
  png_g.draw(center.x-w/2, center.y-h/2, w, h);
  Fl_PNG_Image png("teleportation.png");
  png.draw(center.x-w/2, center.y-h/2, w, h);		
}
void Rectangle::draw_ground() {
  Fl_PNG_Image png("ground.png");
  png.draw(center.x-w/2, center.y-h/2, w, h);
}

void Rectangle::draw_wall() {
  Fl_PNG_Image png("wall.png");
  png.draw(center.x-w/2, center.y-h/2, w, h);
}
void Rectangle::draw_box() {
  Fl_PNG_Image png_g("ground.png");
  png_g.draw(center.x-w/2, center.y-h/2, w, h);
  Fl_PNG_Image png("box.png");
  png.draw(center.x-w/2, center.y-h/2, w, h);
}

void Rectangle::draw_box_ok() {
  Fl_PNG_Image png_g("ground.png");
  png_g.draw(center.x-w/2, center.y-h/2, w, h);
  Fl_PNG_Image png("box_ok.png");
  png.draw(center.x-w/2, center.y-h/2, w, h);
}
void Rectangle::draw_target() {
  Fl_PNG_Image png_g("ground.png");
  png_g.draw(center.x-w/2, center.y-h/2, w, h);
  Fl_PNG_Image png("target.png");
  png.draw(center.x-w/4, center.y-h/4, w, h);
}

void Rectangle::draw_player() {
  Fl_PNG_Image png_g("ground.png");
  png_g.draw(center.x-w/2, center.y-h/2, w, h);
  Fl_PNG_Image png("player.png");
  png.draw(center.x-w/4, center.y-h/2, w, h);

}

void Rectangle::setFillColor(Fl_Color newFillColor) {
  fillColor = newFillColor;
}

void Rectangle::setFrameColor(Fl_Color newFrameColor) {
  frameColor = newFrameColor;
}

bool Rectangle::contains(Point p) {
  return p.x >= center.x-w/2 &&
         p.x < center.x+w/2 &&
         p.y >= center.y-h/2 &&
         p.y < center.y+h/2;
}


/*--------------------------------------------------

Cell class.

The Canvas class below will have cells as instance
vraiables and call the methods of Cell
--------------------------------------------------*/


class Cell {
  Rectangle r;
  int type;
  bool wall=false;
  bool target=false;
  bool player_in=false;
  bool teleportation=false;
  vector<Cell *> neighbors;
  Text neighbor_bomb_count;
  
 public:
  // Constructor
  Cell(Point center, int w, int h,int type);

  // Methods that draw and handle events
  void draw();
  void mouseMove(Point mouseLoc);
  void mouseClick(Point mouseLoc);
  void set_neighbors(vector<Cell *> new_neighbors);
  void set_teleportation(){
  	teleportation = true;
  }
  bool is_teleportation(){
  	return teleportation;
  }
  int get_type(){
  	return type;
  }
  void set_type(int new_type){
  	type = new_type;
  }
  vector<Cell *> get_neighbors(){
  	return neighbors;
  }
  void set_target(){
  	target = true;
  }
  bool is_target(){
  	return target;
  }
};

Cell::Cell(Point center, int w, int h,int cell_type):
    r(center, w, h, FL_BLACK, FL_WHITE),
     type{cell_type},
     neighbor_bomb_count("", center, h/2) {}
 

void Cell::draw() {
  if (type==0){
    r.draw_ground();
  }                
  else if (type==1){
    r.draw_wall();
  }               
  else if (type==2){
    r.draw_box();
  }
  else if (type==3){
    r.draw_target();
    }
  else if (type==4)
  {
    r.draw_box_ok();
  }     
  else if (type==5)
  {
    r.draw_player();
  }          
  else{
    r.draw_teleportation();
    }
  
}

void Cell::mouseMove(Point mouseLoc) {
  
}


void Cell::mouseClick(Point mouseLoc) {


}


void Cell::set_neighbors(const vector<Cell *> new_neighbors){
	neighbors=new_neighbors;
}


/*--------------------------------------------------

Canvas class.

One instance of the canvas class is made by the
MainWindow class.

The fltk system via MainWindow calls:

draw 60 times a second
mouseMove whenever the mouse is moved
mouseClick whenever the mouse is clicked
keyPressed whenever a key is pressed

Any drawing code should be called ONLY in draw
or methods called by draw. If you try to draw
elsewhere it will probably crash.
--------------------------------------------------*/



class Canvas {
  vector<vector<int>>level_map={
  {0,0,1,1,1,1,1,0},
  {1,1,1,0,5,0,1,0},
  {1,3,6,2,0,0,1,0},
  {1,1,1,0,2,3,1,0},
  {1,3,1,1,2,0,1,0},
  {1,0,1,0,3,0,1,1},
  {1,2,0,0,2,2,3,1},
  {1,0,0,6,3,0,0,0},
  {1,1,1,1,1,1,1,1}

};
  vector<Point> teleportation_boxes;
  vector< vector<Cell> > cells;
  void initialize();
  int box_on_target = 0;
  Player player; // sans doute à devoir changer de place et à modifier
 public:
  Canvas(){
    initialize();
  }
  bool check_move(int x, int y);
  void set_type_cell(int get_x, int get_y, int get_old_x, int get_old_y, bool box_move=false);
  void draw();
  void mouseMove(Point mouseLoc);
  void mouseClick(Point mouseLoc);
  void keyPressed(int keyCode); // doit être séparé du code de Sokoban
  void win_game();
  bool teleportation_ok(int delta_x, int delta_y);
};

void Canvas::initialize() {
  box_on_target = 0;
  cells.clear();
  for (unsigned short x = 0; x < 9; x++) {
    cells.push_back({});
    for (int y = 0; y < 8; y++){
      cells[x].push_back({{63*y+63, 63*x+63}, 63, 63,level_map[x][y]});
    	if (cells[x][y].get_type() == 3){
    		cells[x][y].set_target();
    	}
    	else if(cells[x][y].get_type() == 6){
    		teleportation_boxes.push_back(Point(x, y));
    		cells[x][y].set_teleportation();
    	}
      }
  }
}




void Canvas::draw() {
  for (auto &v: cells)
    for (auto &c: v)
      c.draw();
  
}

void Canvas::mouseMove(Point mouseLoc) {
  for (auto &v: cells)
    for (auto &c: v)
      c.mouseMove(mouseLoc);
}

void Canvas::mouseClick(Point mouseLoc) {
  
}

void Canvas::keyPressed(int keyCode) {
	cout << "Please enter your keyboard key" << endl;
	// rajouter les conditions pour les murs, objets et traiter le cas où l'on sortirait du plateau 
	switch (keyCode) {
		
		case 'z':
			if (check_move(-1, 0)){
				if (teleportation_ok(-1, 0)){
					{};
				}
				else{
					player.set_x(1);
					set_type_cell(player.get_x(), player.get_y(), player.get_old_x(), player.get_old_y());
					win_game();
				}
			}
			break;
		case 'q':
			if (check_move(0, -1)){
				if (teleportation_ok(0, -1)){
					{};
				}
				else{
					player.set_y(2);
					set_type_cell(player.get_x(), player.get_y(), player.get_old_x(), player.get_old_y());
					win_game();
				}
			}
			break;
		case 's':
			if (check_move(1, 0)){
				if (teleportation_ok(1, 0)){		
					{};
				}
				else{
					player.set_x(3);
					set_type_cell(player.get_x(), player.get_y(), player.get_old_x(), player.get_old_y());
					win_game();
				}
			}
			break;
		case 'd':
			if (check_move(0, 1)){
				if (teleportation_ok(0, 1)){
					{};
				}
				else{
					player.set_y(4);
					set_type_cell(player.get_x(), player.get_y(), player.get_old_x(), player.get_old_y());
					win_game();
				}
			}
			break;
		case 'e': // trouver autre solution pour 'e'sc
			exit(0);
			
		case 'r':
			initialize();
			player.reinitialize();
		default:
			{cout <<"bad key!"<<endl;} // pass
			break;
	}
}
bool Canvas::check_move(int x, int y){
	if (cells[player.get_x() + x][player.get_y() + y].get_type() == 1){
		return false;
	}
	if(player.get_x() + x < 0 or player.get_x() + x >= cells.size() or player.get_y() + y < 0 or player.get_y() + y >= cells[player.get_x()].size()) {
		return false;
	}
	if((cells[player.get_x() + x][player.get_y() + y].get_type() == 4) or (cells[player.get_x() + x][player.get_y() + y].get_type() == 2)){
		if ((cells[player.get_x() + 2 * x][player.get_y() + 2 * y].get_type() == 0) or (cells[player.get_x() + 2 * x][player.get_y() + 2 * y].get_type() == 3) or (cells[player.get_x() + 2 * x][player.get_y() + 2 * y].get_type() == 6)){
			set_type_cell(player.get_x() + x, player.get_y() + y, player.get_x() + 2 * x, player.get_y() + 2 * y, true);
		}
		else{
			return false;
		}
	}	
	return true;
}

bool Canvas::teleportation_ok(int delta_x, int delta_y){
	vector <Point> stack;
	for(auto teleportation_point = teleportation_boxes.begin(); teleportation_point != teleportation_boxes.end(); ++teleportation_point){
		if (teleportation_point->x == player.get_x() + delta_x and teleportation_point->y == player.get_y() + delta_y){
			if (teleportation_point == teleportation_boxes.begin()){
				if (cells[teleportation_boxes[1].x][teleportation_boxes[1].y].get_type() == 6){
					cells[teleportation_boxes[1].x][teleportation_boxes[1].y].set_type(5);
					cells[player.get_x()][player.get_y()].set_type(0);
					player.reinitialise_process_set(teleportation_boxes[1].x, teleportation_boxes[1].y);
					return true;
				}
				else{
					return false;
				}
			}
			else{
				if (cells[teleportation_boxes[0].x][teleportation_boxes[0].y].get_type() == 6){
					cells[teleportation_boxes[0].x][teleportation_boxes[0].y].set_type(5);
					cells[player.get_x()][player.get_y()].set_type(0);
					cout << "ici2" << endl;
					player.reinitialise_process_set(teleportation_boxes[0].x, teleportation_boxes[0].y);
					return true;
				}
				else{
					return false;
				}
			}		
		}
	}
	return false;
}

void Canvas::set_type_cell(int get_x, int get_y, int get_old_x, int get_old_y, bool box_move){
	if (box_move){
	cout << get_x << get_y << endl;
		if (cells[get_old_x][get_old_y].get_type() == 3){
			cells[get_old_x][get_old_y].set_type(4);
			box_on_target++;
			if (cells[get_x][get_y].is_teleportation()){
				cells[get_x][get_y].set_type(6);
			}
			else{
				cells[get_x][get_y].set_type(0);	
			}
		}
		else{	
			cout << get_x << get_y << endl;
			if (cells[get_x][get_y].get_type() == 4){
				box_on_target--;
			}
			if(cells[get_x][get_y].is_teleportation()){
				cells[get_x][get_y].set_type(6);
				cout << "ici1" << endl;
			}
			/*
			else{
				cells[get_x][get_y].set_type(0);
			}
			*/
			
			cells[get_old_x][get_old_y].set_type(2);
		}
		//cells[get_x][get_y].set_type(0);
	}
	else{
		if (cells[get_old_x][get_old_y].is_target()){
			cells[get_old_x][get_old_y].set_type(3);
		}
		else if(cells[get_old_x][get_old_y].is_teleportation()){
			cells[get_old_x][get_old_y].set_type(6);
		}
		else{
			cells[get_old_x][get_old_y].set_type(0);
		}
		cells[get_x][get_y].set_type(5);
	}
}

void Canvas::win_game(){
	if (box_on_target == 6){
		exit(0);
	}
}

/*
void Canvas::start_game(){
	while(1){
		bool end_game = false;
		while(!end_game){	
		}
		
		while(1){
			{} //pass
		}
		cout << "Do you want to replay? [y/n]";
		char choice;
		cin >> choice;
		switch (choice){
			case 'y':
				{cout << "good choice" << endl;}
			case 'n':
				{cout << "goodbye!" <<endl;}
				break;
		}

	}

}
*/	



/*--------------------------------------------------

MainWindow class.

Do not edit!!!!

--------------------------------------------------*/

class MainWindow : public Fl_Window {
  Canvas canvas;

 public:
  MainWindow() : Fl_Window(500, 500, windowWidth, windowHeight, "Sokoban") {
    Fl::add_timeout(1.0/refreshPerSecond, Timer_CB, this);
    resizable(this);
  }
  void draw() override {
    Fl_Window::draw();
    canvas.draw();
  }
  int handle(int event) override {
    switch (event) {
      case FL_MOVE: 
        canvas.mouseMove(Point{Fl::event_x(), Fl::event_y()});
        return 1;
      case FL_PUSH:
        canvas.mouseClick(Point{Fl::event_x(), Fl::event_y()});
        return 1;
      case FL_KEYDOWN:
        canvas.keyPressed(Fl::event_key());
        return 1;
      default:
        return 0;
    }
    return 0;
  }
  static void Timer_CB(void *userdata) {
    MainWindow *o = static_cast<MainWindow*>(userdata);
    o->redraw();
    Fl::repeat_timeout(1.0/refreshPerSecond, Timer_CB, userdata);
  }
};


/*--------------------------------------------------

main

Do not edit!!!!

--------------------------------------------------*/


int main(int argc, char *argv[]) {
  srand(static_cast<unsigned>(time(nullptr)));
  MainWindow window;
  window.show(argc, argv);
  return Fl::run();
}



/*
Bibliograhie:
	openclassrooms.com/forum/sujet/acquerir-la-touche-fleche-en-c-97853
*/







