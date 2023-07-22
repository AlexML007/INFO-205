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

using namespace std;

const int windowWidth = 1200;
const int windowHeight = 800 ;
const double refreshPerSecond = 60  ;

vector<vector<int>>level_map={
  {0,0,1,1,1,1,1,0},
  {1,1,1,0,5,0,1,0},
  {1,3,0,2,0,0,1,0},
  {1,1,1,0,2,3,1,0},
  {1,3,1,1,2,0,1,0},
  {1,0,1,0,3,0,1,1},
  {1,2,0,4,2,2,3,1},
  {1,0,0,0,3,0,0,1},
  {1,1,1,1,1,1,1,1},

};

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
  bool is_type()
  {
  	return type;
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
  else if (type==4)
  {
    r.draw_box_ok();
  }     
  else if (type==5)
  {
    r.draw_player();
  }          
  else {
    r.draw_target();
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
  vector< vector<Cell> > cells;
  vector<Cell *> neighbors;
  void initialize();
 public:
  Canvas(){
    initialize();
  }
  void draw();
  void mouseMove(Point mouseLoc);
  void mouseClick(Point mouseLoc);
  void keyPressed(int keyCode);
 
};

void Canvas::initialize() {
  cells.clear();
  for (unsigned short x = 0; x < 8; x++) {
    cells.push_back({});
    for (int y = 0; y < 9; y++){
      cells[x].push_back({{63*x+63, 63*y+63}, 63, 63,level_map[y][x]});
    
      }
  }
  for (unsigned x = 0; x < 8; x++) {
    for (unsigned y = 0; y < 9; y++){
    		vector<Cell *> neighbors;
    		for (auto &shift:vector<Point>({{-1, -1},{-1, 0},{-1, 1},{0, -1},{0, 1},{1, -1},{1, 0}  ,{1, 1}}))
    		{
    			if  ((x+shift.x>=0) and (x+shift.x)<cells.size() and (y+shift.y>=0)and (y+shift.y)<cells[x].size())
    			{	
    				neighbors.push_back(&cells[x+shift.x][y+shift.y]);
    			}
    		}
    	cells[x][y].set_neighbors(neighbors);
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
  switch (keyCode) {
    case 'q':
      exit(0);
    case ' ':
    	initialize();  
    default:
    {} // pass
  }
}

/*--------------------------------------------------

MainWindow class.

Do not edit!!!!

--------------------------------------------------*/

class MainWindow : public Fl_Window {
  Canvas canvas;

 public:
  MainWindow() : Fl_Window(500, 500, windowWidth, windowHeight, "Lab 3") {
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
