#!/bin/bash

# Check if we're running on Debian-based Linux (Ubuntu, etc.)
if [ -f /etc/debian_version ]; then
    echo "Debian/Ubuntu-based system detected."
    echo "Installing libcurl development package..."
    sudo apt-get update
    sudo apt-get install -y libcurl4-openssl-dev

# Check for macOS with Homebrew
elif command -v brew &>/dev/null; then
    echo "Homebrew detected on macOS."
    echo "Installing libcurl via Homebrew..."
    brew install curl

else
    echo "Unknown system. Please install libcurl manually."
    exit 1
fi