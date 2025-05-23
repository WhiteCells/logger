#include "logger.h"

void test() {
    Logger::info("world{}112", "test");
}

int main() {
    Logger::init();
    test();
    return 0;
}