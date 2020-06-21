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

#define adc_w_pipe	a_adc_w_pipe[1]
#define adc_w_close	a_adc_w_pipe[0]
#define adc_r_pipe	a_adc_r_pipe[0]
#define adc_r_close	a_adc_r_pipe[1]

#define gui_w_pipe	a_gui_w_pipe[1]
#define gui_w_close	a_gui_w_pipe[0]
#define gui_r_pipe	a_gui_r_pipe[0]
#define gui_r_close	a_gui_r_pipe[1]


int main(int argc, char *argv[]) {
	/* Create pipes to communicate with ADC and GUI processes */
	int a_adc_w_pipe[2], a_adc_r_pipe[2], a_gui_w_pipe[2], a_gui_r_pipe[2];

	if(pipe(a_adc_w_pipe) == -1) {
		printf("Failed to create ADC write pipe, closing program.\n");
		exit(-1);
	}
	if(pipe(a_adc_r_pipe) == -1) {
		close(adc_w_pipe);
		close(adc_w_close);
		printf("Failed to create ADC read pipe, closing program.\n");
	}
	if(pipe(a_gui_w_pipe) == -1) {
		close(adc_w_pipe);
		close(adc_w_close);
		close(adc_r_pipe);
		close(adc_r_close);
		printf("Failed to create GUI write pipe, closing program.\n");
		exit(-1);
	}
	if(pipe(a_gui_r_pipe) == -1) {
		close(adc_w_pipe);
		close(adc_w_close);
		close(adc_r_pipe);
		close(adc_r_close);
		close(gui_w_pipe);
		close(gui_w_close);
		printf("Failed to create GUI read pipe, closing program.\n");
		exit(-1);
	}


	/* Create process for ADC */
	pid_t adc_pid;
	adc_pid = fork();
	if(adc_pid < 0) {
		printf("Failed to create ADC process, closing program.\n");
		exit(-1);
	}
	/* ADC child */
	else if(adc_pid == 0) {
		char s_adc_w_pipe[20];	// Array to store ADC write pipe file descriptor as a string
		char s_adc_w_close[20];	// Array to store pipe file descriptor, to be closed on child end
		char s_adc_r_pipe[20];	// Array to store ADC read pipe file descriptor as a string
		char s_adc_r_close[20];	// Array to store pipe file descriptor, to be closed on child end

		if(sprintf(s_adc_w_pipe, "%d", adc_w_pipe) < 0 ||
		   sprintf(s_adc_r_pipe, "%d", adc_r_pipe) < 0 ||
		   sprintf(s_adc_w_close, "%d", adc_w_close) < 0 ||
		   sprintf(s_adc_r_close, "%d", adc_r_close) < 0)
		{
			printf("Failed to convert one of the ADC pipe file descriptors to string, closing program.\n");
			exit(-1);
		}

		char *argv_adc[] = { "./adc", s_adc_w_pipe, s_adc_w_close, s_adc_r_pipe, s_adc_r_close, NULL };
		if(execv(argv_adc[0], argv_adc) == -1) {
			printf("Failed to open ADC program, closing program.\n");
			exit(-1);
		}
	}
	close(adc_w_close);	// Close ADC write pipe read end from parent
	close(adc_r_close);	// Close ADC read pipe write end from parent


	/* Create process for GUI */
	pid_t gui_pid;
	gui_pid = fork();
	if(gui_pid < 0) {
		kill(adc_pid, SIGTERM);
		printf("Failed to create GUI process, closing program.\n");
		exit(-1);
	}
	/* GUI child */
	else if(gui_pid == 0) {
		char s_gui_w_pipe[20];	// Array to store gui write pipe file descriptor as a string
		char s_gui_w_close[20];	// Array to store gui write pipe file descriptor as a string
		char s_gui_r_pipe[20];	// Array to store gui read pipe file descriptor as a string
		char s_gui_r_close[20];	// Array to store gui read pipe file descriptor as a string

		if(sprintf(s_gui_w_pipe, "%d", gui_w_pipe) < 0 ||
		   sprintf(s_gui_r_pipe, "%d", gui_r_pipe) < 0 ||
		   sprintf(s_gui_w_close, "%d", gui_w_close) < 0 ||
		   sprintf(s_gui_r_close, "%d", gui_r_close) < 0)
		{
			printf("Failed to convert one of the GUI pipe file descriptors to string, closing program.\n");
			exit(-1);
		}

		char *argv_gui[] = {"./gui", s_gui_w_pipe, s_gui_w_close, s_gui_r_pipe, s_gui_r_close, NULL };
		if(execv(argv_gui[0], argv_gui) == -1) {
			printf("Failed to open GUI program, closing program.\n");
			exit(-1);
		}
	}
	close(gui_w_close);	// Close GUI write pipe read end from parent
	close(gui_r_close);	// Close GUI read pipe write end from parent


	/* Move data from ADC process to GUI process */
	int byte_count;
	float force;

	while(1) {
		byte_count = read(adc_r_pipe, &force, sizeof(force));
		if(byte_count > 0) {
			write(gui_w_pipe, &force, byte_count);

			static char buffer[100];						// For debugging
			byte_count = sprintf(buffer, "%f\n", force);	//
			write(STDOUT_FILENO, buffer, byte_count);		//
		}
	}
}
