# Chat system in C
![Project logo](/assets/project_logo.png)

### Description
This project is a chat system, implemented in C and TCP for transmission.
The server is able to handle multiple clients at the same time.

clients can create and join channels, send messages to all the members of the channel. 

### Prequsites
- Linux
- gcc
- make

### How to use
First you need to clone the project to your computer:
```bash
git clone https://github.com/OfekiAlm/TCP-chat-system
```
Then you should open the project directory:
```bash
cd TCP-chat-system
```

For compiling purposes you need to have `/bin` directory in your root directory to store the programs.
```bash
mkdir bin
```

You can run the `/scripts/run.sh` script to compile and run the project.
```bash
chmod +x ./scripts/run.sh
./scripts/run.sh [client|server]
```

or you can compile the project by running the following command:
```bash
make
```
Then you can run the server by running the following command:
```bash
./server
```
And you can run the client by running the following command:
```bash
./client
```
you can run multiple clients at the same time

### Contact
You can contact me at: [ofekalm100@gmail.com](mailto:ofekalm100@gmail.com)