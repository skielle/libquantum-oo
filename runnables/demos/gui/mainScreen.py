#!/usr/bin/python
from gi.repository import Gtk

from superposition_1 import Superposition_1
from superposition_2 import Superposition_2
from measurement import Measurement

class Launcher(Gtk.Window):
	algorithms = [
		["1", "Superposition - choose encoding angle",
			"demo-superposition_1"],
		["2", "Superposition - choose encoding and measurement angle",
			"demo-superposition_2"],
		["11", "Measurement", "demo-measurement"]
	]

	def __init__(self):
		Gtk.Window.__init__(self, title="Quantum Launcher")
		self.connect("delete-event", Gtk.main_quit)
		self.resize(600, 100)

		cVLauncher = Gtk.VBox()
		cHLauncher = Gtk.HBox()
		cHLauncher.pack_start(cVLauncher, True, False, 5)

		lsAlgorithms = Gtk.ListStore(str, str)
		for e in self.algorithms:
			lsAlgorithms.append([e[0], e[1]])

		cbAlgorithm = Gtk.ComboBox.new_with_model_and_entry(lsAlgorithms)
		cbAlgorithm.set_entry_text_column(1)
		cbAlgorithm.set_id_column(0)

		#cbAlgorithm = Gtk.ComboBoxEntry(algorithms, column=1)
		#cbAlgorithm.set_model(algorithms)
		cVLauncher.pack_start(cbAlgorithm, True, False, 5) 
	
		bRun = Gtk.Button("Run")
		bRun.set_size_request(100, 40)
		bRun.connect("clicked", self.bRun_exec, cbAlgorithm)
		cVLauncher.pack_start(bRun, True, False, 5)

		self.add(cHLauncher)

	def bRun_exec(self, bRun, cbAlgorithm):
		activeAlgorithm = cbAlgorithm.get_active_id()

		if activeAlgorithm == "1":
			wAlgorithm = Superposition_1()
		elif activeAlgorithm == "2":
			wAlgorithm = Superposition_2()
		elif activeAlgorithm == "11":
			wAlgorithm = Measurement()
		else:
			wAlgorithm = Gtk.Window()
			wAlgorithm.set_title("Algorithm")

			wAlgorithm.add( Gtk.Label("this is a dummy window") )
	
		wAlgorithm.set_transient_for(self)
		#wAlgorithm.set_type_hint(Gtk.gdk.WINDOW_TYPE_HINT_DIALOG)

		wAlgorithm.connect("destroy", lambda *w: Gtk.main_quit() )

		wAlgorithm.show_all()

if __name__ == "__main__":
	wLauncher = Launcher()
	wLauncher.show_all()
	Gtk.main()
