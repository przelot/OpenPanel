#include <iostream>
#include <unistd.h>
#include <cmath>
#include <cpuid.h>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>

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
    std::cout << "\n";
}

void get_time() {
    // Get current time
    auto time = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(time);
    std::cout << "Current time: " << std::ctime(&time_now);
}

bool ifFileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}

void config() {
    // Configure hardware
    system(clearCom);
    // Cpu
    long num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    // Memory
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    int memorySize = pages*page_size/pow(1024,2);
    std::string filename = "config.config";
    std::string ifCorrect;
    // Create config
    if (!ifFileExists(filename)) {
        std::cout << "Creating configuration file" << std::endl;
        std::ofstream outfile ("config.config");
        outfile.close();
    }
    // Cpu
    std::cout << "Detected CPU cores: " << num_cpus << std::endl;
    std::cout << "Is this number correct? (y/n)" << std::endl;
    std::cin >> ifCorrect;
    if (ifCorrect == "y") {
        std::cout << "Saving CPU config";
    } else if (ifCorrect == "n") {
        std::cout << "CPU configuration will not be saved at the moment" << std::endl;
        // Save to settings file
    } else {
        std::cout << "Invalid argument" << std::endl;
    }
    blankline();
    //Memory
    std::cout << "Detected memory size: " << memorySize << std::endl;
    std::cout << "This could be a little less than your actual memory size" << std::endl;
    std::cout << "Is this number correct? (y/n)" << std::endl;
    std::cin >> ifCorrect;
    if (ifCorrect == "y") {
        std::cout << "Saving memory config" << std::endl;
        // Save to settings file
    } else if (ifCorrect=="n") {
        std::cout << "Memory configuration will not be saved at the moment" << std::endl;
    }
    blankline();
}

void hardwareInfo() {
    // Get hardware info
    system(clearCom);
    long num_cpus = sysconf( _SC_NPROCESSORS_ONLN );
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    int memorySize = pages*page_size/pow(1024,2);
    std::cout << "Hardware info" << std::endl;
    get_time();
    // CPU
    std::cout << "CPU:" << std::endl;
    std::cout << "Number of cores: " << num_cpus << std::endl;
    // Memory
    std::cout << "Memory:" << std::endl;
    //std::cout << "[DEBUG] " << pages << " pages " << page_size << " page_size" << std::endl;
    std::cout << "Memory size: " << memorySize << "MB" << std::endl;
    // Drives
    std::cout << "Drives:" << std::endl;
    blankline();
}

void selftest() {
    // Selftest
    std::string filename = "config.conf";
    std::cout << "Selftest" << std::endl;
    //Check if config exists
    if (!ifFileExists(filename)) {
        std::cout << "No configuration file" << std::endl;
    }
    blankline();
}

void mainDisp() {
    // Main status display
    system(clearCom);
    std::cout << "Main display" << std::endl;
    get_time();
    // show status
    blankline();
}

int main(int argc, char const *argv[]) {
    int command;
    system(clearCom);
    // Info
    std::cout << "Welcome to the Panel" << std::endl;
    blankline();
    //selftest, read config from a file and compare it to hardwareInfo()
    selftest();
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
                break;
            case 1:
                hardwareInfo();
                break;
            case 2:
                mainDisp();
                break;
            case 99:
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