#!/usr/bin/python
from gi.repository import Gtk, Vte
import cairo
import math

class Superposition_1(Gtk.Window):
	registerSize = 4

	def __init__(self):
		Gtk.Window.__init__(self)
		self.connect("delete-event", Gtk.main_quit)
		self.set_title("Superposition 1")
		self.resize(800, 600)
		self.daQubits = [self.registerSize]
		self.ePolarizations = [self.registerSize]
		polarizations = [0.0, 90.0, 45.0, 135.0]

		cVLauncher = Gtk.VBox()
		cHLauncher = Gtk.HBox()
		cHLauncher.set_size_request(700, 50)
		cHdaQubits = Gtk.HBox()
		cHdaQubits.set_size_request(800, 100)
		
		cVLauncher.pack_start(cHLauncher, True, False, 0)
		cVLauncher.pack_start(cHdaQubits, True, False, 0)

		for i in range(self.registerSize):
			self.daQubits.insert(i, Gtk.DrawingArea())
			self.daQubits[i].set_size_request(200, 150)
			self.daQubits[i].connect("draw", self.expose, i)
			cHdaQubits.pack_start(self.daQubits[i], True, False, 0)

		for i in range(self.registerSize):
			self.ePolarizations.insert(i,Gtk.Entry())
			self.ePolarizations[i].set_text(str(polarizations[i]))
			self.ePolarizations[i].set_width_chars(5)
			self.ePolarizations[i].connect("draw", self.updateDaQubits, i)
			cHLauncher.pack_start(
				self.ePolarizations[i], True, False, 0)


		daRegister = Gtk.DrawingArea()
		daRegister.set_size_request(800, 100)
		daRegister.connect("draw", self.exposeRegister)
		cVLauncher.pack_start(daRegister, True, False, 0)

		vt = Vte.Terminal()
		vt.set_size_request(800, 400)

		cVLauncher.pack_start(vt, True, True, 0)	

		self.add(cVLauncher)
		self.show_all()
	
	def updateDaQubits(self, widget, user_data, i):
		self.expose(self.daQubits[i], self.daQubits[i].get_window().cairo_create(), i)

	def expose(self, widget, cr, i):
		vcenter = 70
		hcenter = 80 
		radius = 25
		fontSize = 14

		cr.set_source_rgb(1, 1, 1)
		cr.paint()

		cr.set_source_rgb(0, 0, 0)
		cr.select_font_face("arial", cairo.FONT_SLANT_NORMAL, cairo.FONT_WEIGHT_NORMAL)
		cr.set_font_size(fontSize)

		p = self.ePolarizations[i].get_text()
		if len(p) == 0 :
			p = float(0)
		else:
			p = float(p)
		
		cr.arc(hcenter, vcenter, radius, 
			0, (p/180) * math.pi)
		cr.line_to(hcenter, vcenter)
		cr.stroke()
		cr.move_to(hcenter + radius + fontSize, vcenter)
		cr.text_path("|0>")
		cr.move_to(hcenter - radius - 2 * fontSize, vcenter)
		cr.text_path("|0>")
		cr.move_to(hcenter, vcenter + radius + fontSize)
		cr.text_path("|1>")
		cr.move_to(hcenter, vcenter - radius - fontSize)
		cr.text_path("|1>")
		cr.stroke()

	def exposeRegister(self, widger, cr):
		cr.set_source_rgb(.5, .5, 1)
		cr.paint()

	def getDrawAngle(angle):
		return (angle / 180 - .5) * math.pi

	def bRun_exec(self, bRun):
		print "SUPERPOSITION 1\r\n"
