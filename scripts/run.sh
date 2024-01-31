#!/bin/bash

# Function to compile the program
make_program() {
    cd ../
    make 
    clear
    cd bin
}

# Function to clean the program
clean_program() {
    cd ../
    make clean
    cd bin
}

# Check if the argument is "client" or "server"
if [ "$1" == "client" ]; then
    make_program
    ./client
    clean_program
elif [ "$1" == "server" ]; then
    make_program
    ./server
    clean_program
else
    clean_program
    echo "Something went wrong! Please try again."
    echo "Usage: ./run.sh [client|server]"
fi
