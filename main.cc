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
    char clear_screen_command[6] = "clear";
#elif _WIN32 or _WIN64
    char clear_screen_command[4] = "cls";
#else
    std::cout << "Undefined operating system" << std::endl;
    char clear_screen_command[5] = "echo"
#endif

void clear_screen() {
    // Clear the terminal
    system(clear_screen_command);
}

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

bool if_file_exists(const std::string& filename) {
    // Check if a file exists
    return std::filesystem::exists(filename);
}

void config() {
    // Configure hardware
    clear_screen();
    // Cpu
    long num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    // Memory
    long num_pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    int memory_size = num_pages*page_size/pow(1024,2);
    std::string choice;
    // Create/open config file
    if (!if_file_exists("config.conf")) {
        std::cout << "Creating configuration file" << std::endl;
        std::ofstream config_file ("config.conf");
        config_file.close();
    }
    std::ofstream config_file;
    config_file.open("config.conf");
    // Cpu
    std::cout << "CPU:" << std::endl;
    std::cout << "Detected CPU cores: " << num_cpus << std::endl;
    std::cout << "Is this number correct? (y/n):" << std::endl;
    std::cin >> choice;
    if (choice == "y") {
        config_file << "NUM_CPUS= " << num_cpus << std::endl;
    } else if (choice == "n") {
        std::cout << "CPU configuration will not be saved at the moment" << std::endl;
    } else {
        std::cout << "Invalid argument" << std::endl;
    }
    std::cout << "Saving CPU config" << std::endl;
    blankline();
    //Memory
    std::cout << "Memory:" << std::endl;
    std::cout << "Detected memory size: " << memory_size << std::endl;
    std::cout << "This might be a little less than your actual memory size" << std::endl;
    std::cout << "Is this number correct? (y/n):" << std::endl;
    std::cin >> choice;
    if (choice == "y") {
        config_file << "MEMORY_SIZE= " << memory_size << std::endl;
    } else if (choice == "n") {
        std::cout << "Memory configuration will not be saved at the moment" << std::endl;
    }
    std::cout << "Saving memory config" << std::endl;
    config_file.close();
    blankline();
}

void hardware_info() {
    // Get hardware info
    clear_screen();
    long num_cpus = sysconf( _SC_NPROCESSORS_ONLN );
    long num_pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    int memory_size = num_pages*page_size/pow(1024,2);
    std::cout << "Hardware info" << std::endl;
    get_time();
    // CPU
    std::cout << "CPU:" << std::endl;
    std::cout << "Number of cores: " << num_cpus << std::endl;
    blankline();
    // Memory
    std::cout << "Memory:" << std::endl;
    //std::cout << "[DEBUG] " << num_pages << " num_pages " << page_size << " page_size" << std::endl;
    std::cout << "Memory size: " << memory_size << "MB" << std::endl;
    blankline();
    // Drives
    std::cout << "Drives:" << std::endl;
    blankline();
}

void selftest() {
    // Selftest
    std::cout << "Selftest" << std::endl;
    //Check if config exists
    if (!if_file_exists("config.conf")) {
        std::cout << "No configuration file" << std::endl;
    } else if (if_file_exists("config.conf")) {
        std::cout << "Configuration file detected" << std::endl;
    }
    blankline();
}

void main_display() {
    // Main status display
    clear_screen();
    std::cout << "Main display" << std::endl;
    get_time();
    // show status
    blankline();
}

int main(int argc, char const *argv[]) {
    int command;
    clear_screen();
    // Info
    std::cout << "Welcome to the Panel" << std::endl;
    blankline();
    //selftest, read config from a file and compare it to hardware_info()
    selftest();
    do {
        std::cout << "Press enter to continue..." << std::endl;
    } while (std::cin.get() != '\n');
    // Main menu
    clear_screen();
    while (command != 99) {
        std::cout << "0. Config" << std::endl;
        std::cout << "1. Hardware info" << std::endl;
        std::cout << "2. Main display" << std::endl;
        std::cout << "99. Exit" << std::endl;
        std::cout << "> ";
        if (!(std::cin >> command)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clear_screen();
            std::cout << "Invalid argument" << std::endl;
            blankline();
            continue;
        }
        switch (command) {
            case 0:
                config();
                break;
            case 1:
                hardware_info();
                break;
            case 2:
                main_display();
                break;
            case 99:
                std::cout << "Exit" << std::endl;
                return 0;
                break;
            default:
                clear_screen();
                std::cout << "Invalid argument" << std::endl;
                blankline();
                break;
        }
    }
}