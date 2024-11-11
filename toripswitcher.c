/*
 * Created by Param Chauhan
 * Program to automatically change IP address using Tor on Arch Linux
 * This version changes IP every 30 seconds in an infinite loop.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curl/curl.h>
#include <signal.h>

void install_tor() {
    printf("[+] Tor not detected. Installing Tor...\n");
    system("sudo pacman -Syu --noconfirm tor");
    printf("[!] Tor installed successfully.\n");
}

void start_tor_service() {
    system("sudo systemctl start tor.service");
}

void reload_tor_service() {
    system("sudo systemctl reload tor.service");
}

size_t write_callback(void *ptr, size_t size, size_t nmemb, char *data) {
    strncat(data, (char *)ptr, size * nmemb);
    return size * nmemb;
}

void display_banner() {
    printf(
        "╭━━━━╮╱╱╱╱╭━━┳━━━╮╭━━━╮╱╱╱╱╱╱╱╱╱╭╮╱╱╱╱╱╭╮\n"
        "┃╭╮╭╮┃╱╱╱╱╰┫┣┫╭━╮┃┃╭━╮┃╱╱╱╱╱╱╱╱╭╯╰╮╱╱╱╱┃┃\n"
        "╰╯┃┃┣┻━┳━╮╱┃┃┃╰━╯┃┃╰━━╮╭╮╭╮╭╮╭╮╰╮╭╯╭━━╮┃╰━╮╭━━╮╭━╮\n"
        "╱╱┃┃┃╭╮┃╭╯╱┃┃┃╭━━╯╰━━╮┃┃╰╯╰╯┃┣┫╱┃┃╱┃╭━╯┃╭╮┃┃┃━┫┃╭╯\n"
        "╱╱┃┃┃╰╯┃┃╱╭┫┣┫┃╱╱╱┃╰━╯┃╰╮╭╮╭╯┃┃╱┃╰╮┃╰━╮┃┃┃┃┃┃━┫┃┃\n"
        "╱╱╰╯╰━━┻╯╱╰━━┻╯╱╱╱╰━━━╯╱╰╯╰╯╱╰╯╱╰━╯╰━━╯╰╯╰╯╰━━╯╰╯\n\n"
    );
}

void get_ip(char *ip) {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://checkip.amazonaws.com");
        curl_easy_setopt(curl, CURLOPT_PROXY, "socks5h://127.0.0.1:9050");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, ip);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Failed to retrieve IP: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
}

void change_ip() {
    reload_tor_service();
    char ip[16] = {0};
    get_ip(ip);
    printf("[+] New IP address: %s\n", ip);
}

void signal_handler(int signal) {
    printf("\nIP changer stopped.\n");
    exit(0);
}

int main() {
    signal(SIGINT, signal_handler); // Handle Ctrl+C

    display_banner(); // Display the ASCII art banner

    // Check if Tor is installed
    if (system("which tor > /dev/null 2>&1") != 0) {
        install_tor();
    }

    // Start the Tor service
    printf("Starting Tor service...\n");
    start_tor_service();
    sleep(3); // Give Tor a moment to initialize

    int interval = 30;  // Set default interval to 30 seconds

    printf("Starting infinite IP change every %d seconds. Press Ctrl+C to stop.\n", interval);

    // Infinite loop to change IP every 30 seconds
    while (1) {
        sleep(interval);
        change_ip();
    }

    return 0;
}
