#include <gtk/gtk.h>
#include <vte/vte.h>
#include <string.h>

static void
playAlgorithm (GtkWidget *widget,
		gpointer data) {

	char command[255] = "";
	GtkWidget *tty1 = g_object_get_data(data, "tty1");
	GtkWidget *mode = g_object_get_data(data, "in_mode");
	GtkWidget *bases = g_object_get_data(data, "in_bases");

	switch (gtk_combo_box_get_active(GTK_COMBO_BOX(mode)) ) {
		case 1:
			strcat(command, "bin/demo-superposition_1\r\n");
			break;
		case 3:
			sprintf(command, 
				"bin/bb84-polybase_Generation -b %s\r\n",
				gtk_entry_get_text(GTK_ENTRY(bases)));
		default:
			g_print("%i: %s\n", 
				gtk_combo_box_get_active(
					GTK_COMBO_BOX(mode)),
				gtk_combo_box_text_get_active_text(
					GTK_COMBO_BOX_TEXT(mode)));
			break;
	}

	vte_terminal_feed_child(VTE_TERMINAL(tty1), command, strlen(command));
	gtk_widget_set_sensitive(tty1, TRUE);
}

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
	GtkWidget *window;
	GtkWidget *bases, *keyLength, *errorBits, *errorTolerance, *playButton,
		*mode, *peerIp, *eveRate, *eveStrategy, *channelNoise, 
		*genBias;
	GtkWidget *windowBox, *optionGrid, *playGrid, *playBox;
	GtkWidget *baseLabel, *keyLengthLabel, *errorBitsLabel, 
		*errorToleranceLabel, *peerIpLabel, *eveRateLabel,
		*eveStrategyLabel, *channelNoiseLabel, *genBiasLabel;
	GtkWidget *tty1;
	GtkWidget *tty2;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Quantum Network Simulator");
  gtk_window_set_default_size (GTK_WINDOW (window), 1280, 720);

	windowBox = gtk_button_box_new (GTK_ORIENTATION_VERTICAL);
	gtk_container_add(GTK_CONTAINER(window), windowBox);

	playBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	gtk_container_add(GTK_CONTAINER(windowBox), playBox);

	optionGrid = gtk_grid_new ();
	gtk_container_add(GTK_CONTAINER(playBox), optionGrid);

	playGrid = gtk_grid_new ();
	gtk_grid_set_column_spacing(GTK_GRID(playGrid), 20); 
	gtk_container_add(GTK_CONTAINER(playBox), playGrid);

	mode = gtk_combo_box_text_new ();
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(mode), "Superposition Tutorial");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(mode), "Measurement Tutorial");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(mode), "Classic BB84 Tutorial");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(mode), "Polybase BB84 Tutorial");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(mode), "Bias BB84 Tutorial");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(mode), "Freestyle BB84");
	gtk_grid_attach(GTK_GRID( optionGrid ), mode, 0, 0, 2, 1);

	baseLabel = gtk_label_new("Number of bases");
	bases = gtk_entry_new ();
	gtk_grid_attach(GTK_GRID( optionGrid ), baseLabel , 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID( optionGrid ), bases , 1, 1, 1, 1);

	keyLengthLabel = gtk_label_new("Key Length");
	keyLength = gtk_entry_new ();
	gtk_grid_attach(GTK_GRID( optionGrid ), keyLengthLabel , 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID( optionGrid ), keyLength, 1, 2, 1, 1);

	errorBitsLabel = gtk_label_new("Number of error detection bits");
	errorBits = gtk_entry_new ();
	gtk_grid_attach(GTK_GRID( optionGrid ), errorBitsLabel , 0, 3, 1, 1);
		gtk_grid_attach(GTK_GRID( optionGrid ), errorBits , 1, 3, 1, 1);

	errorToleranceLabel = gtk_label_new("Number of error bits to tolerate");
	errorTolerance = gtk_entry_new ();
	gtk_grid_attach(GTK_GRID( optionGrid ), errorToleranceLabel , 0, 4, 1, 1);
	gtk_grid_attach(GTK_GRID( optionGrid ), errorTolerance , 1, 4, 1, 1);

	errorToleranceLabel = gtk_label_new("Peer IP");
	errorTolerance = gtk_entry_new ();
	gtk_grid_attach(GTK_GRID( optionGrid ), errorToleranceLabel , 2, 0, 1, 1);
	gtk_grid_attach(GTK_GRID( optionGrid ), errorTolerance , 3, 0, 1, 1);

	errorToleranceLabel = gtk_label_new("Eve Eavesdrop Rate");
	errorTolerance = gtk_entry_new ();
	gtk_grid_attach(GTK_GRID( optionGrid ), errorToleranceLabel , 2, 1, 1, 1);
	gtk_grid_attach(GTK_GRID( optionGrid ), errorTolerance , 3, 1, 1, 1);

	errorToleranceLabel = gtk_label_new("Eve Eavesdrop Strategy");
	errorTolerance = gtk_entry_new ();
	gtk_grid_attach(GTK_GRID( optionGrid ), errorToleranceLabel , 2, 2, 1, 1);
	gtk_grid_attach(GTK_GRID( optionGrid ), errorTolerance , 3, 2, 1, 1);

	errorToleranceLabel = gtk_label_new("Generator Bias");
	errorTolerance = gtk_entry_new ();
	gtk_grid_attach(GTK_GRID( optionGrid ), errorToleranceLabel , 2, 3, 1, 1);
	gtk_grid_attach(GTK_GRID( optionGrid ), errorTolerance , 3, 3, 1, 1);

	errorToleranceLabel = gtk_label_new("Channel Noise");
	errorTolerance = gtk_entry_new ();
	gtk_grid_attach(GTK_GRID( optionGrid ), errorToleranceLabel , 2, 4, 1, 1);
	gtk_grid_attach(GTK_GRID( optionGrid ), errorTolerance , 3, 4, 1, 1);

	playButton = gtk_button_new_with_label("PLAY QUANTUM ALGORITHM");
	g_signal_connect (playButton, "clicked", G_CALLBACK(playAlgorithm), playButton);
//	g_signal_connect_swapped (playButton, "clicked", G_CALLBACK(gtk_widget_destroy), window);
	gtk_grid_attach(GTK_GRID( optionGrid ), playButton, 0, 5, 4, 1);

	char *startTerm[2] = {0,0};
	startTerm[0] = vte_get_user_shell();
	GPid pidTerm;

	tty1 = vte_terminal_new();
	vte_terminal_fork_command_full(VTE_TERMINAL(tty1),
		VTE_PTY_DEFAULT,
		NULL, //"/tmp",
		startTerm,
		NULL,
		(GSpawnFlags)(G_SPAWN_DO_NOT_REAP_CHILD | G_SPAWN_SEARCH_PATH),
		NULL,
		NULL,
		NULL,
		NULL );
	gtk_widget_set_size_request(tty1, 600, 400);
	gtk_grid_attach(GTK_GRID (playGrid ), tty1, 0, 6, 4, 15);
	gtk_widget_set_sensitive(tty1, FALSE);

	tty2 = vte_terminal_new();
	vte_terminal_fork_command_full(VTE_TERMINAL(tty2),
		VTE_PTY_DEFAULT,
		NULL, //"/tmp",
		startTerm,
		NULL,
		(GSpawnFlags)(G_SPAWN_DO_NOT_REAP_CHILD | G_SPAWN_SEARCH_PATH),
		NULL,
		NULL,
		NULL,
		NULL );
	gtk_widget_set_size_request(tty2, 600, 400);
	gtk_grid_attach(GTK_GRID (playGrid ), tty2, 4, 6, 4, 15);
	gtk_widget_set_sensitive(tty2, FALSE);

	g_object_set_data(G_OBJECT(playButton), "tty1", VTE_TERMINAL(tty1));
	g_object_set_data(G_OBJECT(playButton), "tty2", VTE_TERMINAL(tty2));
	g_object_set_data(G_OBJECT(playButton), "in_mode", GTK_COMBO_BOX_TEXT(mode));
	g_object_set_data(G_OBJECT(playButton), "in_bases", GTK_ENTRY(bases));
	g_object_set_data(G_OBJECT(playButton), "in_keyLength", GTK_ENTRY(keyLength));
	g_object_set_data(G_OBJECT(playButton), "in_errorBits", GTK_ENTRY(errorBits));
	g_object_set_data(G_OBJECT(playButton), "in_errorTolerance", GTK_ENTRY(errorTolerance));

  gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
