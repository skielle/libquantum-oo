#include <gtk/gtk.h>
#include <vte/vte.h>
#include <string.h>

static void
playAlgorithm (GtkWidget *widget,
		gpointer data) {
	g_print("PLAYING!");
}

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
	GtkWidget *window;
	GtkWidget *bases, *keyLength, *errorBits, *errorTolerance, *playButton,
		*mode, *peerIp, *eveRate, *eveStrategy, *channelNoise, 
		*genBias;
	GtkWidget *windowBox, *optionGrid, *playBox;
	GtkWidget *baseLabel, *keyLengthLabel, *errorBitsLabel, 
		*errorToleranceLabel, *peerIpLabel, *eveRateLabel,
		*eveStrategyLabel, *channelNoiseLabel, *genBiasLabel;
	GtkWidget *terminal;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Quantum Network Simulator");
  gtk_window_set_default_size (GTK_WINDOW (window), 800, 800);

	windowBox = gtk_button_box_new (GTK_ORIENTATION_VERTICAL);
	gtk_container_add(GTK_CONTAINER(window), windowBox);

	playBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	gtk_container_add(GTK_CONTAINER(windowBox), playBox);

	optionGrid = gtk_grid_new ();
	gtk_container_add(GTK_CONTAINER(playBox), optionGrid);

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
	g_signal_connect (playButton, "clicked", G_CALLBACK(playAlgorithm), NULL);
	g_signal_connect_swapped (playButton, "clicked", G_CALLBACK(gtk_widget_destroy), window);
	gtk_grid_attach(GTK_GRID( optionGrid ), playButton, 0, 5, 4, 1);

	terminal = vte_terminal_new();
	char *startTerm[2] = {0,0};
	startTerm[0] = vte_get_user_shell();
	GPid pidTerm;

	vte_terminal_fork_command_full(VTE_TERMINAL(terminal),
		VTE_PTY_DEFAULT,
		NULL, //"/tmp",
		startTerm,
		NULL,
		(GSpawnFlags)(G_SPAWN_DO_NOT_REAP_CHILD | G_SPAWN_SEARCH_PATH),
		NULL,
		NULL,
		NULL,
		NULL );
	gtk_widget_set_size_request(terminal, 750, 400);

	char command[255] = "bin/demo-measurement\r\n";
	vte_terminal_feed_child(VTE_TERMINAL(terminal), command, strlen(command));

	gtk_grid_attach(GTK_GRID (optionGrid ), terminal, 0, 6, 4, 15);

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
