#include <iostream>
#include "view/graphicview/window.h"
#include <thread>
#include <chrono>

using namespace std;


int x = 0;
int increment = 1;
Xwindow w;
int fps = 1000/50;

int main() {

  w.fillRectangle(0,0,500,500,Xwindow::White);

  for (int i = Xwindow::White ; i <= Xwindow::Brown; i++) {
    w.fillRectangle(50 * i + x, 200, 50, 250, i);
  }

  x+=increment;
  if (x < 100) {increment++;} else {increment--;}
  
  

  this_thread::sleep_for(chrono::milliseconds(fps));  
  main();
}

