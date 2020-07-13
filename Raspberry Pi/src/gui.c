#include <gtk/gtk.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define WINDOW_WIDTH	400
#define WINDOW_HEIGHT	200

void close_program();
gboolean timeout(gpointer label);

int write_pipe, read_pipe;


int main(int argc, char *argv[]) {
	/* Initialize process */
	write_pipe = strtol(argv[1], NULL, 10);
	read_pipe = strtol(argv[2], NULL, 10);

	int flags = fcntl(read_pipe, F_GETFL, 0);		// Set read_pipe to
	fcntl(read_pipe, F_SETFL, flags | O_NONBLOCK);	// non-blocking mode

	/* Initialize GTK */
	gtk_init(&argc, &argv);

	/* Create widgets */
	GtkWidget *window, *grid, *l_title, *e_force;	// Create pointers for widgets
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);	// Create window
	grid = gtk_grid_new();							// Create grid to hold all widgets in a window
	l_title = gtk_label_new("Force (N)");			// Create label to display title
	e_force = gtk_entry_new ();						// Create entry to display force

	/* Set properties */
	gtk_widget_set_hexpand(l_title, TRUE);				// Expand title horizontally to expand grid
	gtk_widget_set_margin_top(l_title, 20);				// Add top margin to title lable
	gtk_widget_set_halign(e_force, GTK_ALIGN_CENTER);	// Center force entry
	gtk_entry_set_width_chars(GTK_ENTRY(e_force), 10);	// Set force entry character width
	gtk_entry_set_alignment(GTK_ENTRY(e_force), 0.5);	// Center characters inside force entry
	gtk_widget_set_margin_top(e_force, 5);				// Add top margin to force entry
	gtk_widget_set_sensitive(e_force, FALSE);			// Make force entry read-only

	/* Add widgets to window */
	gtk_window_set_default_size(GTK_WINDOW(window),			// Set window default size
						WINDOW_WIDTH, WINDOW_HEIGHT);		//
	gtk_grid_attach(GTK_GRID(grid), l_title, 0, 0, 1, 1);	// Add title lable to grid
	gtk_grid_attach(GTK_GRID(grid), e_force, 0, 1, 1, 1);	// Add force entry to grid
	gtk_container_add(GTK_CONTAINER(window), grid);			// Add grid to window
	gtk_widget_show_all(window);							// Make window and all its widgets visible

	/* Event handling */
	g_signal_connect(window, "delete_event",			// Assign "close_program()" to
			G_CALLBACK(close_program), NULL);			// close window event
	g_timeout_add(50, (GSourceFunc)timeout, e_force);	// Trigger event every 50 ms

	/* Enter to gtk main loop to handle graphics and events */
	gtk_main();

	return -1;
}


void close_program() {
	gtk_main_quit();
}

gboolean timeout(gpointer label) {
	static float force;
	static int byte_count;
	static char buffer[20];

	byte_count = read(read_pipe, &force, sizeof(force));
	if(byte_count > 0) {
		byte_count = sprintf(buffer, "%.2f", force);
		gtk_entry_set_text(GTK_ENTRY(label), buffer);
	}

	return TRUE;
}
