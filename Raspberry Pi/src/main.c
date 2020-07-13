#define _POSIX_SOURCE
#define _GNU_SOURCE

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define main_to_adc_write   main_to_adc[1]
#define main_to_adc_read    main_to_adc[0]
#define adc_to_main_write   adc_to_main[1]
#define adc_to_main_read    adc_to_main[0]

#define main_to_gui_write   main_to_gui[1]
#define main_to_gui_read    main_to_gui[0]
#define gui_to_main_write   gui_to_main[1]
#define gui_to_main_read    gui_to_main[0]

#define main_to_websocket_write     main_to_websocket[1]
#define main_to_websocket_read      main_to_websocket[0]
#define websocket_to_main_write     websocket_to_main[1]
#define websocket_to_main_read      websocket_to_main[0]


int main(int argc, char *argv[]) {
    /* Create pipes to communicate with child processes */
    int main_to_adc[2];
    int adc_to_main[2];
    int main_to_gui[2];
    int gui_to_main[2];
    int main_to_websocket[2];
    int websocket_to_main[2];

    if(pipe(main_to_adc) == -1       ||
       pipe(adc_to_main) == -1       ||
       pipe(main_to_gui) == -1       ||
       pipe(gui_to_main) == -1       ||
       pipe(main_to_websocket) == -1 ||
       pipe(websocket_to_main) == -1)
    {
        printf("Failed to create some of the pipes. "
            "Closing main program.\n");
        exit(-1);
    }


    /* Create process for ADC */
    pid_t adc_pid;
    adc_pid = fork();
    if(adc_pid < 0) {
        printf("Failed to create process for ADC. "
            "Closing main program.\n");
        exit(-1);
    }
    /* ADC child */
    else if(adc_pid == 0) {
        close(main_to_adc_write);       // Close main write pipe end from child
        close(adc_to_main_read);        // Close main read pipe end from child
        char s_adc_to_main_write[20];   // Array to store ADC write pipe file descriptor as a string
        char s_main_to_adc_read[20];    // Array to store ADC read pipe file escriptor as a string

        if(sprintf(s_adc_to_main_write, "%d", adc_to_main_write) < 0 ||
           sprintf(s_main_to_adc_read, "%d", main_to_adc_read) < 0)
        {
            printf("File descriptor to string conversion failed. "
                "Closing main program.\n");
            exit(-1);
        }

        char *argv_adc[] = {
            "./adc",
            s_adc_to_main_write,
            s_main_to_adc_read,
            NULL
        };
        if(execv(argv_adc[0], argv_adc) == -1) {
            printf("Failed to execute 'adc' program, exec failed. "
                "Closing main program.\n");
            exit(-1);
        }
    }
    close(adc_to_main_write);   // Close ADC write pipe end from parent
    close(main_to_adc_read);    // Close ADC read pipe end from parent


    /* Create process for GUI */
    pid_t gui_pid;
    gui_pid = fork();
    if(gui_pid < 0) {
        kill(adc_pid, SIGTERM);
        printf("Failed to create process for GUI. "
            "Closing main program.\n");
        exit(-1);
    }
    /* GUI child */
    else if(gui_pid == 0) {
        close(main_to_gui_write);       // Close main write pipe end from child
        close(gui_to_main_read);        // Close main read pipe end from child
        char s_gui_to_main_write[20];   // Array to store GUI write pipe file descriptor as a string
        char s_main_to_gui_read[20];    // Array to store GUI read pipe file descriptor as a string

        if(sprintf(s_gui_to_main_write, "%d", gui_to_main_write) < 0 ||
           sprintf(s_main_to_gui_read, "%d", main_to_gui_read) < 0)
        {
            printf("File descriptor to string conversion failed. "
                "Closing main program.\n");
            exit(-1);
        }

        char *argv_gui[] = {
            "./gui",
            s_gui_to_main_write,
            s_main_to_gui_read,
            NULL
        };
        if(execv(argv_gui[0], argv_gui) == -1) {
            printf("Failed to execute 'gui' program, exec failed. "
                "Closing main program.\n");
            exit(-1);
        }
    }
    close(gui_to_main_write);   // Close GUI write pipe end from parent
    close(main_to_gui_read);    // Close GUI read pipe end from parent


    /* Create process for WebSocket */
    pid_t websocket_pid;
    websocket_pid = fork();
    if(websocket_pid < 0) {
        kill(adc_pid, SIGTERM);
        kill(gui_pid, SIGTERM);
        printf("Failed to create process for WebSocket. "
            "Closing main program.\n");
        exit(-1);
    }
    /* WebSocket child */
    else if(websocket_pid == 0) {
        close(main_to_websocket_write); // Close main write pipe end from child
        close(websocket_to_main_read);  // Close main read pipe end from child
        char s_websocket_to_main_write[20]; // Array to store WebSocket write pipe file descriptor as a string
        char s_main_to_websocket_read[20];  // Array to store WebSocket read pipe file descriptor as a string

        if(sprintf(s_websocket_to_main_write, "%d", websocket_to_main_write) < 0 ||
           sprintf(s_main_to_websocket_read, "%d", main_to_websocket_read) < 0)
        {
            printf("File descriptor to string conversion failed. "
                "Closing main program.\n");
            exit(-1);
        }

        char *argv_websocket[] = {
            "./websocket",
            s_websocket_to_main_write,
            s_main_to_websocket_read,
            NULL
        };
        if(execv(argv_websocket[0], argv_websocket) == -1) {
            printf("Failed to execute 'websocket' program, exec failed. "
                "Closing main program.\n");
            exit(-1);
        }
    }
    close(websocket_to_main_write); // Close WebSocket write pipe end from parent
    close(main_to_websocket_read);  // Close WebSocket read pipe end from parent


    /* Move data from ADC process to GUI and WebSocket processes */
    while(1) {

        /* Read data from ADC */
        int byte_count;
        float force;
        byte_count = read(adc_to_main_read, &force, sizeof(force));
        if(byte_count > 0) {

            /* Send force to GUI */
            write(main_to_gui_write, &force, byte_count);

            /* Send force to WebSocket */
            char buffer[100];
            byte_count = sprintf(buffer, "%f\n", force);
            write(main_to_websocket_write, buffer, byte_count);

            /* Send force to STDOUT for debuggin */
            write(STDOUT_FILENO, buffer, byte_count);
        }
    }
}
