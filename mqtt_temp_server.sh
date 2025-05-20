#!/bin/bash

# Script to launch a temporary MQTT server for development
# Based on config.h values but with local host binding

# Exit on error
set -e

# Get the local IP address
LOCAL_IP=$(ifconfig | grep -Eo 'inet (addr:)?([0-9]*\.){3}[0-9]*' | grep -Eo '([0-9]*\.){3}[0-9]*' | grep -v '127.0.0.1' | head -n 1)

# Extract MQTT port from config.h
BASE_MQTT_PORT=$(grep "MQTT_BROKER_PORT" src/config.h | grep -v "//" | awk '{print $3}')

# Fallback to default MQTT port if not found
if [ -z "$BASE_MQTT_PORT" ]; then
    BASE_MQTT_PORT=1883
    echo "Could not extract MQTT_BROKER_PORT from config.h, using default: $BASE_MQTT_PORT"
else
    echo "Using MQTT port from config.h: $BASE_MQTT_PORT"
fi

# Function to check if a port is in use
is_port_in_use() {
    lsof -i:"$1" >/dev/null 2>&1
    return $?
}

# Find an available port starting from the base port
MQTT_PORT=$BASE_MQTT_PORT
while is_port_in_use $MQTT_PORT; do
    echo "Port $MQTT_PORT is already in use, trying next port..."
    MQTT_PORT=$((MQTT_PORT + 1))
done

echo "Starting MQTT broker on local IP: $LOCAL_IP:$MQTT_PORT"

# Check if Mosquitto is installed
if ! command -v mosquitto &> /dev/null; then
    echo "Mosquitto is not installed. Installing..."
    if command -v brew &> /dev/null; then
        brew install mosquitto
    else
        echo "Homebrew not found. Please install Mosquitto manually."
        exit 1
    fi
fi

# Create a random filename
TMP_CONFIG="/tmp/mosquitto_config_$RANDOM.conf"

# Ensure file doesn't exist
if [ -f "$TMP_CONFIG" ]; then
    rm -f "$TMP_CONFIG"
fi

cat > "$TMP_CONFIG" << EOF
listener $MQTT_PORT
allow_anonymous true
EOF

echo "Using temporary config at $TMP_CONFIG"
echo "MQTT broker will be accessible at $LOCAL_IP:$MQTT_PORT"
echo "Press Ctrl+C to stop the MQTT broker"

# Run mosquitto with the temporary config
mosquitto -c "$TMP_CONFIG" -v

# Clean up config file upon exit
trap "rm -f $TMP_CONFIG; echo 'MQTT broker stopped. Temporary config removed.'" EXIT 