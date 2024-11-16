# Tor IP Switcher

This program automatically changes your IP address using the Tor network every 30 seconds. It uses the Tor service on Arch Linux to switch IPs and provides a simple way to keep your IP address anonymous.

## Features

### 🌟 Key Features
- **Auto IP Change**: Automatically switches your IP address every 30 seconds using Tor.
- **Tor Installation**: If Tor is not installed, the program installs it for you.
- **Service Management**: Starts, stops, and reloads the Tor service when needed.
- **Real-time IP Display**: Displays the current IP after every change.
- **Infinite Loop**: The program runs indefinitely, changing the IP every 30 seconds until manually stopped.

## Requirements

- **Arch Linux** (other Linux distributions may require modifications).
- **Tor** (automatically installed if not already present).
- **libcurl** library for HTTP requests.
  
## Installation

### 1. Clone the repository

```bash
git clone https://github.com/paramchauhan101/TorIPSwitcher.git
cd TorIPSwitcher
