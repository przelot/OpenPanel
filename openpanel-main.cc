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
    char clearScreenCommand[6] = "clear";
#elif _WIN32 or _WIN64
    std::cout << "OS not supported"
    return 0;
#else
    std::cout << "Undefined operating system" << std::endl;
    return 0;
#endif

void clear_screen() {
    // Clear the terminal
    system(clearScreenCommand);
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
    if(stat(path, &info)!=0) {
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
    int command;
    // Hostname
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    // Local IP address
    char* localIpAddress;
    // Cpu
    long numberOfCpus = sysconf(_SC_NPROCESSORS_ONLN);
    // Memory
    long numberOfPages = sysconf(_SC_PHYS_PAGES);
    long pageSize = sysconf(_SC_PAGE_SIZE);
    int memorySize = numberOfPages*pageSize/pow(1024,2);
    std::cout << "Configuration" << std::endl;
    sleep(1);
    // Check/create "Assets" directory
    if(!if_directory_exists("Assets")) {
        std::cout << "[OK] Creating assets directory" << std::endl;
        std::filesystem::create_directory("Assets");
    } else if(if_directory_exists("Assets")) {
        std::cout << "[OK] Assets directory exists" << std::endl;
    } else {
        std::cout << "[ERROR] Unknown error occured" << std::endl;
    }
    // Check config file
    if (!if_file_exists("Assets/main-config.conf")) {
        std::cout << "[OK] Creating configuration file" << std::endl;
        std::ofstream writeConfig("Assets/main-config.conf");
        writeConfig.close();
    } else if(if_file_exists("Assets/main-config.conf")) {
        std::cout << "[OK] Configuration file exists" << std::endl;
    } else {
        std::cout << "[ERROR] Unknown error occured" << std::endl;
    }
    // Check network config file
    // Check server/client setup file
    if(!if_file_exists("Assets/server-config.conf")) {
        std::cout << "[OK] Creating socket conifguration file" << std::endl;
        std::ofstream writeConfig("Assets/server-config.conf");
        writeConfig.close();
    } else if(if_file_exists("Assets/server-config.conf")) {
        std::cout << "[OK] Server config file exists" << std::endl;
    } else {
        std::cout << "[ERROR] Unknown error occured" << std::endl;
    }
    blankline();
    sleep(1);
    std::cout << "0. Abort" << std::endl;
    std::cout << "1. Hardware configuration" << std::endl;
    std::cout << "2. Network/server configuration" << std::endl;
    std::cout << "> ";
    std::cin >> command;
    if(command == 0){
        clear_screen();
        std::cout << "[OK] Abort" << std::endl;
        return;
    } else if(command == 1) {
        // Hardware config
        std::ofstream writeConfig("Assets/main-config.conf", std::ofstream::in | std::ofstream::out);
        std::cout << "Hardware configuration:" << std::endl;
        sleep(1);
        // Hostname
        std::cout << "Host:" << std::endl;
        std::cout << "Hostname= " << hostname << std::endl;
        writeConfig << "HOSTNAME= " << hostname << std::endl;
        std::cout << "[OK] Host config saved" << std::endl;
        sleep(1);
        // Cpu
        std::cout << "CPU:" << std::endl;
        std::cout << "Detected CPU cores= " << numberOfCpus << std::endl;
        writeConfig << "NUMBER_OF_CPUS= " << numberOfCpus << std::endl;
        std::cout << "[OK] CPU config saved" << std::endl;
        sleep(1);
        //Memory
        std::cout << "Memory:" << std::endl;
        std::cout << "Detected memory size= " << memorySize << std::endl;
        writeConfig << "MEMORY_SIZE= " << memorySize << std::endl;
        std::cout << "[OK] Memory config saved" << std::endl;
        blankline();
        writeConfig.close();
        blankline();
        sleep(1);
    } else if(command == 2) {
        clear_screen();
        //Network/server config
        std::ofstream writeConfig("Assets/server-config.conf", std::ofstream::in | std::ofstream::out);
        std::cout << "Network and server/client configuration:" << std::endl;
        sleep(1);
        // Server/client mode
        std::cout << "Socket mode:" << std::endl;
        std::cout << "1. Server" << std::endl;
        std::cout << "2. Client" << std::endl;
        std::cout << "> ";
        std::cin >> command;
        if(command == 1) {
            std::cout << "Mode= Server" << std::endl;
            writeConfig << "MODE= SERVER" << std::endl;
            std::cout << "[OK] Server/client mode saved" << std::endl; 
        } else if(command == 2) {
            std::cout << "Mode= Client" << std::endl;
            writeConfig << "MODE= CLIENT" << std::endl;
            std::cout << "[OK] Server/client mode saved" << std::endl;
        } else {
            clear_screen();
            std::cout << "[ERROR] Unknown error occured" << std::endl;
        }
        sleep(1);
        // Detect local IP address

    }
}

void show_hardware_info() {
    // Get hardware info
    clear_screen();
    //Cpu
    long numberOfCpus = sysconf( _SC_NPROCESSORS_ONLN );
    // Memory
    long numberOfPages = sysconf(_SC_PHYS_PAGES);
    long pageSize = sysconf(_SC_PAGE_SIZE);
    int memorySize = numberOfPages*pageSize/pow(1024,2);
    std::cout << "Hardware info" << std::endl;
    blankline();
    // CPU
    std::cout << "CPU:" << std::endl;
    std::cout << "Number of cores: " << numberOfCpus << std::endl;
    blankline();
    // Memory
    std::cout << "Memory:" << std::endl;
    std::cout << "Memory size: " << memorySize << "MB" << std::endl;
    blankline();
    // Drives
    std::cout << "Drives:" << std::endl;
    blankline();
}

void selftest() {
    // Selftest
    // Host
    std::string hostnameFromConfig;
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    // CPU
    int numberOfCpusFromConfig;
    long numberOfCpus = sysconf(_SC_NPROCESSORS_ONLN);
    // Memory
    int memorySizeFromConfig;
    long numberOfPages = sysconf(_SC_PHYS_PAGES);
    long pageSize = sysconf(_SC_PAGE_SIZE);
    int memorySize = numberOfPages*pageSize/pow(1024, 2);
    // Network
    std::string modeFromConfig;
    clear_screen();
    std::cout << "Selftest" << std::endl;
    sleep(1);
    //Check if "assets" directory exists
    if(!if_directory_exists("Assets")) {
        std::cout << "[WARNING] No assets directory" << std::endl;
        return;
    } else if(if_directory_exists("Assets")) {
        std::cout << "[OK] Assets directory found" << std::endl;
    } else {
        std::cout << "[ERROR] Unknown error occured" << std::endl;
    }
    //Check if config exists
    if(!if_file_exists("Assets/main-config.conf")) {
        std::cout << "[WARNING] No configuration file" << std::endl;
    } else if(if_file_exists("Assets/main-config.conf")) {
        std::cout << "[OK] Configuration file found" << std::endl;
    } else {
        std::cout << "[ERROR] Unknown error occured" << std::endl;
    }
    if(!if_file_exists("Assets/server-config.conf")) {
        std::cout << "[WARNING] No socket configuration file" << std::endl;
    } else if(if_file_exists("Assets/server-config.conf")) {
        std::cout << "[OK] Socket configuration file found" << std::endl;
    } else {
        std::cout << "[ERROR] Unknown error occured" << std::endl;
    }
    sleep(1);
    std::ifstream readConfig("Assets/main-config.conf");
    if(readConfig.is_open()) {
        std::string line;
        while(getline(readConfig, line)) {
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
            if(line[0] == '#' || line.empty()) {
                continue;
            }
            auto delimiterPosition = line.find("=");
            auto name = line.substr(0, delimiterPosition);
            auto value = line.substr(delimiterPosition + 1);
            if (name == "HOSTNAME") {
                hostnameFromConfig = value;
            } else if(name == "NUMBER_OF_CPUS") {
                numberOfCpusFromConfig = std::stoi(value);
            } else if(name == "MEMORY_SIZE") {
                memorySizeFromConfig = std::stoi(value);
            }
        }
    } else {
        std::cout << "[ERROR] Couldn't open configuration file" << std::endl;
    }
    blankline();
    std::cout << "Configuration:" << std::endl;
    if(hostname == hostnameFromConfig) {
        std::cout << "[OK] Hostname= " << hostname << " From config= " << hostnameFromConfig << std::endl;
    } else {
        std::cout << "[WARNING] Hostname= " << hostname << " From config= " << hostnameFromConfig << std::endl;
        std::cout << "[WARNING] Configuration file may originate from another system" << std::endl;
    }
    if(numberOfCpus == numberOfCpusFromConfig) {
        std::cout << "[OK] Number of cpus= " << numberOfCpus << " From config= " << numberOfCpusFromConfig << std::endl;
    } else {
        std::cout << "[WARNING] Number of cpus= " << numberOfCpus << " From config= " << numberOfCpusFromConfig << std::endl;
        std::cout << "[WARNING] Saved CPU cores number doesn't match the actual value" << std::endl;
    }
    if(memorySize == memorySizeFromConfig) {
        std::cout << "[OK] Memory size= " << memorySizeFromConfig << " MB" << " From config= " << memorySizeFromConfig << " MB" << std::endl;
    } else {
        std::cout << "[WARNING] Memory size= " << memorySizeFromConfig << " MB" << " From config= " << memorySizeFromConfig << " MB" << std::endl;
        std::cout << "[WARNING] Saved memory size doesn't match the actual value" << std::endl;
    }
    readConfig.close();
    blankline();
    sleep(1);
    std::cout << "Network configuration:" << std::endl;
    std::ifstream readServerConfig("Assets/server-config.conf");
    if(readServerConfig.is_open()) {
        std::string line;
        while(getline(readServerConfig, line)) {
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
            if(line[0] == '#' || line.empty()) {
                continue;
            }
            auto delimiterPosition = line.find("=");
            auto name = line.substr(0, delimiterPosition);
            auto value = line.substr(delimiterPosition + 1);
            if (name == "MODE") {
                modeFromConfig = value;
            }
        }
    }
    if(modeFromConfig == "SERVER" || modeFromConfig == "CLIENT") {
        std::cout << "[OK] Socket mode: " << modeFromConfig << std::endl;
    } else {
        std::cout << "[WARNING] Socket mode: " << modeFromConfig << std::endl;
        std::cout << "[WARNING] Invalid value for socket mode" << std::endl;
    }
    
    sleep(1);
    blankline();
}

void socket_server() {
    std::string modeFromConfig;
    clear_screen();
    if(!if_file_exists("Assets/server-config.conf")) {
        std::cout << "[ERROR] Server/client configuration not found" << std::endl;
        std::cout << "[OK] Abort" << std::endl;
        return;
    }
    std::ifstream readConfig ("Assets/server-config.conf");
    if(readConfig.is_open()) {
        std::string line;
        while(getline(readConfig, line)) {
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
            if(line[0] == '#' || line.empty()) {
                continue;
            }
            auto delimiterPosition = line.find("=");
            auto name = line.substr(0, delimiterPosition);
            auto value = line.substr(delimiterPosition + 1);
            if (name == "MODE") {
                modeFromConfig = value;
            }
        }
    } else {
        std::cout << "[ERROR] Couldn't open configuration file" << std::endl;
    }
    std::cout << "[OK] Starting " << modeFromConfig << std::endl;
    if(modeFromConfig == "SERVER") {
        // creating socket
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        // specifying the address
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(8080);
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        // binding socket.
        bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
        // listening to the assigned socket
        listen(serverSocket, 5);
        // accepting connection request
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        // recieving data
        char buffer[1024] = { 0 };
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "Message from client: " << buffer << std::endl;
        // closing the socket.
        close(serverSocket);
        return;
    } else if(modeFromConfig == "CLIENT") {
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        // specifying address
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(8080);
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        // sending connection request
        connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
        // sending data
        const char* message = "[OK] Connection established";
        send(clientSocket, message, strlen(message), 0);
        // closing socket
        close(clientSocket);
        return;
    } else {
        std::cout << "[ERROR] Unknown socket mode" << std::endl;
    }
}

void list_main_menu_items() {
    clear_screen();
    std::cout << "0. This menu" << std::endl;
    std::cout << "1. System info" << std::endl;
    std::cout << "2. Main display" << std::endl;
    std::cout << "3. Socket server" << std::endl;
    std::cout << "8. Selftest" << std::endl;
    std::cout << "9. Config" << std::endl;
    std::cout << "99. Exit" << std::endl;
}

int main(int argc, char const *argv[]) {
    int command;
    // Main function
    clear_screen();
    // Selftest
    selftest();
    std::cout << "OpenPanel 2024-2025" << std::endl;
    // Wait for user input to continue
    do {
        std::cout << "Press enter to continue..." << std::endl;
    } while (std::cin.get() != '\n');
    // Main menu
    clear_screen();
    list_main_menu_items();
    while(command != 99) {
        std::cout << "> ";
        if(!(std::cin >> command)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clear_screen();
            std::cout << "Invalid argument" << std::endl;
            blankline();
            continue;
        }
        switch(command) {
            case 0: {
                list_main_menu_items();
                break;
            } case 1: {
                show_hardware_info();
                break;
            } case 2: {
                // Main display
                break;
            } case 3: {
                socket_server();
                break;
            } case 8: {
                selftest();
                break;
            } case 9: {
                config();
                break;
            } case 99: {
                std::cout << "[OK] Exit" << std::endl;
                return EXIT_SUCCESS;
                break;
            } default: {
                clear_screen();
                std::cout << "[WARNING] Invalid argument" << std::endl;
                blankline();
                break;
            }
        }
    }
}