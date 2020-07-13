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

#define main_to_adc_write	main_to_adc[1]
#define main_to_adc_read	main_to_adc[0]
#define adc_to_main_write	adc_to_main[1]
#define adc_to_main_read	adc_to_main[0]

#define main_to_gui_write	main_to_gui[1]
#define main_to_gui_read	main_to_gui[0]
#define gui_to_main_write	gui_to_main[1]
#define gui_to_main_read	gui_to_main[0]


int main(int argc, char *argv[]) {
	/* Create pipes to communicate with ADC and GUI processes */
	int main_to_adc[2];
	int main_to_gui[2];
	int adc_to_main[2];
	int gui_to_main[2];

	if(pipe(main_to_adc) == -1 ||
	   pipe(main_to_gui) == -1 ||
	   pipe(adc_to_main) == -1 ||
	   pipe(gui_to_main) == -1)
	{
		close(main_to_adc_write);
		close(main_to_adc_read);
		close(adc_to_main_write);
		close(adc_to_main_read);
		close(main_to_gui_write);
		close(main_to_gui_read);
		close(gui_to_main_write);
		close(gui_to_main_read);
		printf("Failed to create some of the pipes, closing program.\n");
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
		close(main_to_adc_write);		// Close main write pipe end from child
		close(adc_to_main_read);		// Close main read pipe end from child
		char s_adc_to_main_write[20];	// Array to store ADC write pipe file descriptor as a string
		char s_main_to_adc_read[20];	// Array to store ADC read pipe file escriptor as a string

		if(sprintf(s_adc_to_main_write, "%d", adc_to_main_write) < 0 ||
		   sprintf(s_main_to_adc_read, "%d", main_to_adc_read) < 0)
		{
			printf("Failed to convert one of the ADC pipe file descriptors to string, closing program.\n");
			exit(-1);
		}

		char *argv_adc[] = { "./adc", s_adc_to_main_write, s_main_to_adc_read, NULL };
		if(execv(argv_adc[0], argv_adc) == -1) {
			printf("Failed to open ADC program, closing program.\n");
			exit(-1);
		}
	}
	close(adc_to_main_write);	// Close ADC write pipe end from parent
	close(main_to_adc_read);	// Close ADC read pipe end from parent


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
		close(main_to_gui_write);		// Close main write pipe end from child
		close(gui_to_main_read);		// Close main read pipe end from child
		char s_gui_to_main_write[20];	// Array to store GUI write pipe file descriptor as a string
		char s_main_to_gui_read[20];	// Array to store GUI read pipe file descriptor as a string

		if(sprintf(s_gui_to_main_write, "%d", gui_to_main_write) < 0 ||
		   sprintf(s_main_to_gui_read, "%d", main_to_gui_read) < 0)
		{
			printf("Failed to convert one of the GUI pipe file descriptors to string, closing program.\n");
			exit(-1);
		}

		char *argv_gui[] = {"./gui", s_gui_to_main_write, s_main_to_gui_read, NULL };
		if(execv(argv_gui[0], argv_gui) == -1) {
			printf("Failed to open GUI program, closing program.\n");
			exit(-1);
		}
	}
	close(gui_to_main_write);	// Close GUI write pipe end from parent
	close(main_to_gui_read);	// Close GUI read pipe end from parent


	/* Move data from ADC process to GUI process */
	int byte_count;
	float force;

	while(1) {
		byte_count = read(adc_to_main_read, &force, sizeof(force));
		if(byte_count > 0) {
			write(main_to_gui_write, &force, byte_count);

			static char buffer[100];						// For debugging
			byte_count = sprintf(buffer, "%f\n", force);	//
			write(STDOUT_FILENO, buffer, byte_count);		//
		}
	}
}
