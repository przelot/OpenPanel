#include <iostream>
#include <thread>
#include <chrono>

// Define command to clear terminal
#ifdef __linux__
    char clear_screen_command[6] = "clear";
#elif _WIN32 or _WIN64
    std::cout << "This is version for linux, please download windows version."
    return 0;
#else
    std::cout << "Undefined operating system" << std::endl;
    return 0;
#endif

void clear_screen() {
    // Clear the terminal
    system(clear_screen_command);
}

void blankline() {
    // Leave an empty line
    std::cout << '\n';
}

void sleep(int seconds) {
    // Do nothing for X seconds
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void get_time() {
    // Get current time
    auto time = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(time);
    std::cout << "Current time: " << std::ctime(&time_now);
}

int main(int argc, char const *argv[])
{
    // Main function
    std::cout << "Panel - Client" << std::endl;
    return 0;
}
