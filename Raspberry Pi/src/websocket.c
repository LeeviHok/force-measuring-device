#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    char *args[] = {
        "pipenv",
        "run",
        "python",
        "../src/websocket.py",
        argv[1],    // Write pipe file descriptor
        argv[2],    // Read pipe file descriptor
        NULL
    };
    if(execvp(args[0], args) == -1) {
        printf("Failed to execute 'websocket.py' program, exec failed. "
            "Closing WebSocket child process.\n");
        exit(-1);
    }
}
