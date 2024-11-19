#include <iostream>
#include <unistd.h>
#include <cmath>
#include <cpuid.h>
#include <chrono>
#include <ctime>

// Define command to clear terminal
#ifdef __linux__
    char clearCom[6] = "clear";
#elif _WIN32 or _WIN64
    char clearCom[4] = "cls";
#else
    std::cout << "Undefined operating system" << std::endl;
    char clearCom[5] = "echo"
#endif

void blankline() {
    // Leave an empty line
    std::cout << std::endl;
}

void get_time() {
    // Get current time
    auto time = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(time);
    std::cout << "Current time: " << std::ctime(&time_now);
}

void config() {
    // Configure hardware
    system(clearCom);
    std::cout << "Config" << std::endl;
    // Write config to a file
}

void hardwareInfo() {
    // Get hardware info
    system(clearCom);
    long num_cpus = sysconf( _SC_NPROCESSORS_ONLN );
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    std::cout << "Hardware info" << std::endl;
    get_time();
    // CPU
    std::cout << "CPU:" << std::endl;
    
    std::cout << "Number of cores: " << num_cpus << std::endl;
    // Memory
    std::cout << "Memory:" << std::endl;
    std::cout << "[DEBUG] " << pages << " pages " << page_size << " page_size" << std::endl;
    std::cout << "Memory size: " << static_cast<int>(pages*page_size/pow(1024, 2)) << "MB" << std::endl;
    // Drives
    std::cout << "Drives:" << std::endl;
}

void mainDisp() {
    // Main status display
    system(clearCom);
    std::cout << "Main display" << std::endl;
    get_time();
    // show status
}

int main(int argc, char const *argv[]) {
    int command;
    system(clearCom);
    // Info
    std::cout << "Welcome to the Panel" << std::endl;
    blankline();
    //selftest, read config from a file and compare it to hardwareInfo()
    std::cout << "Selftest" << std::endl;
    blankline();
    do {
        std::cout << "Press enter to continue..." << std::endl;
    } while (std::cin.get() != '\n');
    // Main menu
    system(clearCom);
    while (command != 99) {
        std::cout << "0. Config" << std::endl;
        std::cout << "1. Hardware info" << std::endl;
        std::cout << "2. Main display" << std::endl;
        std::cout << "99. Exit" << std::endl;
        std::cout << "> ";
        if (!(std::cin >> command)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system(clearCom);
            std::cout << "Invalid argument" << std::endl;
            blankline();
            continue;
        }
        switch (command) {
            case 0:
                config();
                blankline();
                break;
            case 1:
                hardwareInfo();
                blankline();
                break;
            case 2:
                mainDisp();
                blankline();
                break;
            case 99:
                blankline();
                std::cout << "Exit" << std::endl;
                return 0;
                break;
            default:
                system(clearCom);
                std::cout << "Invalid argument" << std::endl;
                blankline();
                break;
        }
    }
}