#!/usr/bin/python
from gi.repository import Gtk

class Superposition_1(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self)
		self.connect("delete-event", Gtk.main_quit)
		self.set_title("Superposition 1")
		self.resize(600, 800)
		self.show_all()

		cVLauncher = Gtk.VBox()
		cHLauncher = Gtk.HBox()
		cHLauncher.pack_start(cVLauncher, True, False, 5)

		bRun = Gtk.Button("Execute")
		bRun.set_size_request(50, 40)
		bRun.connect("clicked", self.bRun_exec)
		cVLauncher.pack_start(bRun, True, False, 5)

		self.add(cHLauncher)

	def bRun_exec(self, bRun):
		print "SUPERPOSITION 1\r\n"
