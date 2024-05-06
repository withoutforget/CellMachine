#include <iostream>
#include <thread>

using namespace std::chrono_literals;

#include "cell_machine.hpp"

int main() {
    CellMachine cm(100, 20); // size of grid

    cm.set_default({{4, 4}, {5, 5}, {5, 6}, {4, 6}, {3, 6}, {3, 5}});

    /*
        if near alive cell exist between one and five alive cells then this cell is alive, if doesn't then cell is dead
        if near dead cell exist exactly 3 alive cells then cell is alive, if doesn't then cell is still dead
    */
    cm.setKeepAlive([](size_t near) { return near >= 1 && near <= 5; });
    cm.setRaiseDead([](size_t near) { return near == 3; });

    while (true) {
        std::system("cls || clear");
        std::cout << cm.get_grid().to_string('*', ' '); // get grid of cells where alive cells is '*' and dead is ' '
        cm.update(); // go next evolution
        std::this_thread::sleep_for(500ms);
    }
}
