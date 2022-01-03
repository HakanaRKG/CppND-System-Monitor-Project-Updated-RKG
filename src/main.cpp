#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"

#include "format.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <set>

int main() {
  System system;
  NCursesDisplay::Display(system);
}