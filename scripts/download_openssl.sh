#!/bin/bash

# Function to check the OS
check_os() {
  if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
  elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
  else
    echo "Unsupported OS: $OSTYPE"
    exit 1
  fi
}

# Function to install OpenSSL on Linux
install_openssl_linux() {
  echo "Installing OpenSSL on Linux..."

  # Update package list
  sudo apt-get update

  # Install dependencies
  sudo apt-get install -y build-essential checkinstall zlib1g-dev

  # Download OpenSSL source code
  wget https://www.openssl.org/source/openssl-1.1.1u.tar.gz

  # Extract the source code
  tar -xvzf openssl-1.1.1u.tar.gz
  cd openssl-1.1.1u

  # Configure and install
  ./config
  make
  sudo make install

  # Clean up
  cd ..
  rm -rf openssl-1.1.1u
  rm -f openssl-1.1.1u.tar.gz

  echo "OpenSSL installed successfully on Linux."
}

# Function to install OpenSSL on macOS
install_openssl_macos() {
  echo "Installing OpenSSL on macOS..."

  # Check if Homebrew is installed
  if ! command -v brew &> /dev/null; then
    echo "Homebrew is not installed. Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
  fi

  # Install OpenSSL via Homebrew
  brew install openssl

  echo "OpenSSL installed successfully on macOS."
}

# Main function to determine OS and install OpenSSL
main() {
  check_os

  if [ "$OS" == "linux" ]; then
    install_openssl_linux
  elif [ "$OS" == "macos" ]; then
    install_openssl_macos
  fi
}

# Execute the main function
main
