#include <algorithm>
#include <arpa/inet.h>
#include <chrono>
#include <cmath>
#include <ctime>
#include <cpuid.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <thread>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

// Check operating system
#ifdef __linux__
    char clear_screen_command[6] = "clear";
#elif _WIN32 or _WIN64
    std::cout << "OS not supported"
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

bool if_directory_exists(const char *path) {
    struct stat info;
    if (stat(path, &info)!=0) {
        return false;
    } else {
        return true;
    }
}

bool if_file_exists(const std::string& filename) {
    // Check if a file exists
    return std::filesystem::exists(filename);
}

// ! Only functions above allowed in functions bellow !

void config() {
    // Configure hardware
    clear_screen();
    // Hostname
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    // Local IP address
    char* local_ip_address;
    // Cpu
    long number_of_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    // Memory
    long number_of_pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    int memory_size = number_of_pages*page_size/pow(1024,2);
    // Check/create "Assets" directory
    if (!if_directory_exists("Assets")) {
        std::cout << "[OK] Creating assets directory" << std::endl;
        std::filesystem::create_directory("Assets");
    }
    // Create/open config file
    if (!if_file_exists("Assets/main-config.conf")) {
        std::cout << "[OK] Creating configuration file" << std::endl;
        std::ofstream write_config ("Assets/main-config.conf");
        write_config.close();
    }
    std::ofstream write_config;
    write_config.open("Assets/main-config.conf");
    // Hostname
    std::cout << "Host:" << std::endl;
    std::cout << "Hostname: " << hostname << std::endl;
    write_config << "HOSTNAME= " << hostname << std::endl;
    std::cout << "[OK] Host config saved" << std::endl;
    blankline();
    // Local IP address
    // Cpu
    std::cout << "CPU:" << std::endl;
    std::cout << "Detected CPU cores: " << number_of_cpus << std::endl;
    write_config << "NUMBER_OF_CPUS= " << number_of_cpus << std::endl;
    std::cout << "[OK] CPU config saved" << std::endl;
    blankline();
    //Memory
    std::cout << "Memory:" << std::endl;
    std::cout << "Detected memory size: " << memory_size << std::endl;
    write_config << "MEMORY_SIZE= " << memory_size << std::endl;
    std::cout << "[OK] Memory config saved" << std::endl;
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
    // Host
    std::string hostname_from_config;
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    // CPU
    int number_of_cpus_from_config;
    long number_of_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    // Memory
    int memory_size_from_config;
    long number_of_pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    int memory_size = number_of_pages*page_size/pow(1024, 2);
    std::cout << "Selftest" << std::endl;
    //Check if "assets" directory exists
    if (!if_directory_exists("Assets")) {
        std::cout << "[ERROR] No assets directory" << std::endl;
        sleep(2);
        return;
    } else {
        std::cout << "[OK] Assets directory detected" << std::endl;
    }
    //Check if config exists
    if (!if_file_exists("Assets/main-config.conf")) {
        std::cout << "[ERROR] No configuration file" << std::endl;
        sleep(2);
        return;
    } else {
        std::cout << "[OK] Configuration file detected" << std::endl;
    }
    sleep(1);
    std::ifstream read_config ("Assets/main-config.conf");
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
                hostname_from_config = value;
            } else if (name == "NUMBER_OF_CPUS") {
                number_of_cpus_from_config = std::stoi(value);
            } else if (name == "MEMORY_SIZE") {
                memory_size_from_config = std::stoi(value);
            }
        }
    } else {
        std::cout << "[ERROR] Couldn't open configuration file" << std::endl;
    }
    blankline();
    std::cout << "Hostname= " << hostname << " From config= " << hostname_from_config << std::endl;
    if (hostname_from_config != hostname) {
        std::cout << "[WARNING] Configuration file may originate from another system" << std::endl;
    }
    blankline();
    std::cout << "Number of cpus= " << number_of_cpus << " From config= " << number_of_cpus_from_config << std::endl;
    if (number_of_cpus_from_config != number_of_cpus) {
        std::cout << "[WARNING] Saved CPU cores number doesn't match the actual value" << std::endl;
    }
    blankline();
    std::cout << "Memory size= " << memory_size_from_config << " MB" << " From config= " << memory_size_from_config << " MB" << std::endl;
    if (memory_size_from_config != memory_size) {
        std::cout << "[WARNING] Saved memory size doesn't match the actual value" << std::endl;
    }
    read_config.close();
    sleep(3);
    blankline();
}

void socket_server() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    // Server address
    server_address.sin_addr.s_addr = INADDR_ANY;
    bind(server_socket, (struct  sockaddr*)&server_address, sizeof(server_address));
    listen(server_socket, 5);
    int client_socket = accept(server_socket, nullptr, nullptr);
    char buffer[1024] = { 0 };
    recv(client_socket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client:" << buffer << std::endl;
    close(server_socket);
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
    // Selftest
    selftest();
    // Wait for user input to continue
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
            case 3:
                socket_server();
                break;
            case 98:
                selftest();
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