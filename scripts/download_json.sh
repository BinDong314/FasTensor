#!/bin/bash

# URL for the latest release of nlohmann/json
URL="https://github.com/nlohmann/json/releases/latest/download/json.hpp"

# Destination directory where the header should be placed
DEST_DIR="src/nlohmann"

# Create the directory if it doesn't exist
mkdir -p $DEST_DIR

# Download json.hpp
curl -L $URL -o $DEST_DIR/json.hpp

if [ $? -eq 0 ]; then
    echo "nlohmann/json.hpp downloaded successfully."
else
    echo "Failed to download nlohmann/json.hpp."
    exit 1
fi