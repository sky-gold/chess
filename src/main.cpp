#include <iostream>
#include <thread>

#include "init.h"

#include "app.h"

#include "move_gen_test.h"


int main() {
    init::init();
    std::cout << "Init takes " << clock() << "ms" << std::endl;
    std::cout << "Number of threads supported by the system: " << std::thread::hardware_concurrency() << std::endl;
    move_gen_test::test();
//    move_gen_test::speed_test();
    App app;
    app.run();
    return 0;
}