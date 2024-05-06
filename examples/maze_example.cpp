#include <iostream>
#include <thread>

using namespace std::chrono_literals;

#include "cell_machine.hpp"

int main() {
    CellMachine cm(100, 20);

    cm.set_default({{4, 4}, {5, 5}, {5, 6}, {4, 6}, {3, 6}, {3, 5}});

    cm.setKeepAlive([](size_t near) { return near >= 1 && near <= 5; });
    cm.setRaiseDead([](size_t near) { return near == 3; });

    while (true) {
        std::system("cls || clear");
        std::cout << cm.get_grid().to_string('*', ' ');
        cm.update();
        std::this_thread::sleep_for(500ms);
    }
}
