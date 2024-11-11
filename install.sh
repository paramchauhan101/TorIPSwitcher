#!/bin/bash

# Check if running as root
if [[ $EUID -ne 0 ]]; then
   echo "[!] This script must be run as root (use sudo)"
   exit 1
fi

echo "[+] Updating package database and upgrading existing packages..."
pacman -Syu --noconfirm

# Install Tor
if ! command -v tor &> /dev/null; then
    echo "[+] Tor is not installed. Installing Tor..."
    pacman -S --noconfirm tor
    echo "[+] Tor installation completed."
else
    echo "[+] Tor is already installed."
fi

# Enable and start the Tor service
echo "[+] Enabling and starting Tor service..."
systemctl enable tor.service
systemctl start tor.service

# Install curl (provides libcurl for development)
if ! command -v curl &> /dev/null; then
    echo "[+] Installing curl (provides libcurl)..."
    pacman -S --noconfirm curl
else
    echo "[+] curl (libcurl) is already installed."
fi

# Install gcc for compiling C programs if not installed
if ! command -v gcc &> /dev/null; then
    echo "[+] Installing GCC compiler..."
    pacman -S --noconfirm gcc
else
    echo "[+] GCC is already installed."
fi

echo "[+] All necessary packages are installed."

# Compilation step (optional)
echo "[+] Compiling IP changer program..."
gcc -o toripswitcher toripswitcher.c -lcurl

if [[ $? -eq 0 ]]; then
    echo "[+] Compilation successful. You can now run the program with ./ip_changer"
else
    echo "[!] Compilation failed. Please check the code and try again."
fi

echo "[+] Installation complete. You are ready to use the IP changer program."
