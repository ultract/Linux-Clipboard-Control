//
/* Hook Clipboard of Gtk by ultract */
/* Remove all clipboard datas When "Copy" event occurs */
//
// Notice : 
// 	gtk_clipboard_wait_for_xxx, gtk_clipboard_wait_is_xxx will be return,
// 	if other clipboard applications are running.
//


#include <gtk/gtk.h>
#include <string.h>

void print_msg(gchar *summary, gchar *msg){
	GError *error;
	gchar *command_line;
/*
	gchar *command_line = "gdbus call --session \
				           --dest org.freedesktop.Notifications \
						   --object-path /org/freedesktop/Notifications \
						   --method org.freedesktop.Notifications.Notify \
						   my_app_name \
						   42 \
						   gtk-dialog-info \
						   \"The Summary\" \
						   \"Here's the body of the notification\" \
						   [] \
						   {} \
						   5000";
*/
	 command_line = g_strdup_printf("gdbus call --session \
	 								--dest org.freedesktop.Notifications \
									--object-path /org/freedesktop/Notifications \
									--method org.freedesktop.Notifications.Notify \
									my_app_name 42 gtk-dialog-info \
									\"%s\" \"%s\" \
									[] {} 5000", summary, msg);

	//g_debug("%s", command_line);
	error = NULL;
	if(!g_spawn_command_line_async(command_line, &error)){
		g_error_free(error);
	}
	else{
		g_debug("g_spawn_command_line_async called.");
		//exit(1);
	}
}

// Refer to 
// https://developer.gnome.org/gtk3/stable/gtk3-Clipboards.html#GtkClipboard-owner-change
void clipboard_data_handle(GtkClipboard *clipboard, GdkEvent *event, gpointer user_data){
	gchar *clipboard_text;
	GdkPixbuf *pixbuf;
	GtkTextBuffer *buffer;
	GdkAtom *format;
	gchar **uris;
	gsize *length;

	//(void)event;
	//(void)user_data;
	g_debug("clipboard_data_handle called");

	buffer = gtk_text_buffer_new(NULL);

	// Text
	if(gtk_clipboard_wait_is_text_available(clipboard)){
		g_debug("gtk_clipboard_wait_is_text_available called");
		clipboard_text = gtk_clipboard_wait_for_text(clipboard);
		if(clipboard_text == NULL)
			return;
		
		if(strlen((char *)clipboard_text)==0)
			return;

		g_debug("clipboard_text: %s", clipboard_text);
		gtk_clipboard_set_text(clipboard, "", -1);	// Clear Clipboard Text
		gtk_clipboard_clear(clipboard);
		g_free(clipboard_text);

		// Check null clipboard_text
		if(strlen((char *)clipboard_text)==0)
			return;

		print_msg("Notice", "Clipboard is Not Allowed!!");
		return;
	}
	// Image
	else if(gtk_clipboard_wait_is_image_available(clipboard)){
		g_debug("gtk_clipboard_wait_is_image_available called");
		pixbuf = gtk_clipboard_wait_for_image(clipboard);
		if(pixbuf == NULL)
			return;

		gtk_clipboard_clear(clipboard);
		g_object_unref(pixbuf);
		print_msg("Notice", "Clipboard is Not Allowed!!");
		return;
	}
	// Rich Test
	else if(gtk_clipboard_wait_is_rich_text_available(clipboard, buffer)){
		g_debug("gtk_clipboard_wait_is_rich_text_available called");
		gtk_clipboard_wait_for_rich_text(clipboard, buffer, format, length);
		if(buffer == NULL)
			return;

		g_free(buffer);
		g_object_unref(format);
		g_free(length);
		print_msg("Notice", "Clipboard is Not Allowed!!");
		return;
		
	}
	// Uris
	else if(gtk_clipboard_wait_is_uris_available(clipboard)){
		g_debug("gtk_clipboard_wait_is_uris_available called");
		uris = gtk_clipboard_wait_for_uris(clipboard);
		if(uris == NULL)
			return;

		g_strfreev(uris);
		print_msg("Notice", "Clipboard is Not Allowed!!");
		return;
	}

}

int main(int argc, char *argv[]){

//	GtkClipboard *primary;
	GtkClipboard *clipboard;

	/* Initiate GTK+ */
	gtk_init(&argc, &argv);

/*
	primary = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
	if (NULL == primary) {
		g_warning("gtk_clipboard_get failed.");
		return FALSE;
	}
	// Get Selected Text by Drag	
	//primary_text = gtk_clipboard_wait_for_text(primary);
*/
	clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	if (NULL == clipboard) {
		g_warning("gtk_clipboard_get failed.");
		return FALSE;
	}

	// Initialize clipboard data
	gtk_clipboard_set_text(clipboard, "", -1);	// Clear Clipboard Text
	gtk_clipboard_clear(clipboard);

	// Connect to the "owner-change" signal which means that ownership
	// of the X clipboard has changed
	g_signal_connect(clipboard, "owner-change", G_CALLBACK(clipboard_data_handle), NULL);

	// Run the GTK main loop so that we get the signal
	// until the user kills the program(^C)
	gtk_main();

	return 0;
}
