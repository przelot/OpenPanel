#include <iostream>
#include <thread>
#include <chrono>

#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

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

void socket_server() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    const char* message = "hello";
    send(client_socket, message, strlen(message), 0);

    close(client_socket);
}

int main(int argc, char const *argv[])
{
    // Main function
    std::cout << "Panel - Client" << std::endl;

    socket_server();
    return 0;
}
