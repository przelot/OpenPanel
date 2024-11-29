#include <iostream>
#include <unistd.h>
#include <cmath>
#include <cpuid.h>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <thread>
#include <limits.h>

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

bool if_file_exists(const std::string& filename) {
    // Check if a file exists
    return std::filesystem::exists(filename);
}

// ! Only functions above allowed in functions bellow !

void config() {
    // Configure hardware
    clear_screen();
    // Host
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    // Cpu
    long number_of_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    // Memory
    long number_of_pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    int memory_size = number_of_pages*page_size/pow(1024,2);
    std::string choice;
    // Create/open config file
    if (!if_file_exists("config.conf")) {
        std::cout << "Creating configuration file" << std::endl;
        std::ofstream write_config ("config.conf");
        write_config.close();
    }
    std::ofstream write_config;
    write_config.open("config.conf");
    // Host
    std::cout << "Host:" << std::endl;
    std::cout << "Hostname: " << hostname << std::endl;
    write_config << "HOSTNAME= " << hostname << std::endl;
    blankline();
    // Cpu
    std::cout << "CPU:" << std::endl;
    std::cout << "Detected CPU cores: " << number_of_cpus << std::endl;
    write_config << "NUMBER_OF_CPUS= " << number_of_cpus << std::endl;
    std::cout << "CPU config saved" << std::endl;
    blankline();
    //Memory
    std::cout << "Memory:" << std::endl;
    std::cout << "Detected memory size: " << memory_size << std::endl;
    write_config << "MEMORY_SIZE= " << memory_size << std::endl;
    std::cout << "Memory config saved" << std::endl;
    write_config.close();
    blankline();
}

void hardware_info() {
    // Get hardware info
    clear_screen();
    //Cpu
    long number_of_cpus = sysconf( _SC_NPROCESSORS_ONLN );
    // Memory
    long number_of_pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    int memory_size = number_of_pages*page_size/pow(1024,2);
    std::cout << "Hardware info" << std::endl;
    blankline();
    // CPU
    std::cout << "CPU:" << std::endl;
    std::cout << "Number of cores: " << number_of_cpus << std::endl;
    blankline();
    // Memory
    std::cout << "Memory:" << std::endl;
    std::cout << "Memory size: " << memory_size << "MB" << std::endl;
    blankline();
    // Drives
    std::cout << "Drives:" << std::endl;
    blankline();
}

void selftest() {
    // Selftest
    std::string hostname;
    int number_of_cpus;
    int memory_size;
    std::cout << "Selftest" << std::endl;
    //Check if config exists
    if (!if_file_exists("config.conf")) {
        std::cout << "No configuration file" << std::endl;
        blankline();
        sleep(3);
        return;
    } else {
        std::cout << "Configuration file detected" << std::endl;
    }
    std::ifstream read_config ("config.conf");
    if (read_config.is_open()) {
        std::string line;
        while (getline(read_config, line)) {
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
            if (line[0] == '#' || line.empty()) {
                continue;
            }
            auto delimiter_position = line.find("=");
            auto name = line.substr(0, delimiter_position);
            auto value = line.substr(delimiter_position + 1);
            if (name == "HOSTNAME") {
                hostname = value;
            } else if (name == "NUMBER_OF_CPUS") {
                number_of_cpus = std::stoi(value);
            } else if (name == "MEMORY_SIZE") {
                memory_size = std::stoi(value);
            }
        }
    } else {
        std::cout << "Couldn't open configuration file" << std::endl;
    }
    std::cout << "Hostname= " << hostname << std::endl;
    std::cout << "Number of cpus= " << number_of_cpus << std::endl;
    std::cout << "Memory size= " << memory_size << " MB" << std::endl;
    sleep(3);
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
    // Main function
    clear_screen();
    std::cout << "Panel" << std::endl;
    blankline();
    //selftest, read config from a file and compare it to hardware_info()
    selftest();
    do {
        std::cout << "Press enter to continue..." << std::endl;
    } while (std::cin.get() != '\n');
    // Main menu
    clear_screen();
    while (command != 99) {
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
