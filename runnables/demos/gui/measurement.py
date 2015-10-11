#!/usr/bin/python
from gi.repository import Gtk, Vte
import cairo
import math
import time

from gi.repository import GLib
import os
from grpc.beta import implementations
from pprint import pprint
import gui_pb2

class Measurement(Gtk.Window):
	registerSize = 4
	initDone = 0

	def __init__(self):
		Gtk.Window.__init__(self)

		self.channel = implementations.insecure_channel('localhost', 60000)
		self.stub = gui_pb2.beta_create_ControllerView_stub(self.channel)

		self.connect("delete-event", Gtk.main_quit)
		self.set_title("Measurement")
		self.resize(800, 400)
		self.daQubits = [self.registerSize]
		self.hQubits = [self.registerSize]
		self.eZeros = [self.registerSize]
		self.eOnes = [self.registerSize]
		self.cVquBitInfo = [self.registerSize]
		self.ePolarizations = [self.registerSize]
		self.eMeasurementAngles = [self.registerSize]
		self.bFindAngle = [self.registerSize]
		self.bMeasure = [self.registerSize]

		self.polarizations = [0.0, 0.0, 0.0, 0.0]
		self.measurementAngles = [0.0, 0.0, 0.0, 0.0]

		cGMain = Gtk.Grid(column_spacing=10)

		cGQubits = Gtk.Grid()
		cGUserInput = Gtk.Grid()

		cVRegister = Gtk.VBox()
		cVRegister.set_size_request(150, 400)

		lGQubits = Gtk.Label()
		lGQubits.set_text("Qubit State")
		lGRegister = Gtk.Label()
		lGRegister.set_text("Register State")
		lGUserInput = Gtk.Label()
		lGUserInput.set_text("Outcome Probability")

		cGMain.attach(lGUserInput, 0, 0, 1, 1)
		cGMain.attach(lGQubits, 1, 0, 2, 1)
		cGMain.attach(lGRegister, 3, 0, 1, 1)

		cGMain.attach(cGQubits, 0, 1, 3, 8)
		cGMain.attach(cVRegister, 3, 1, 1, 8)

		for i in range(self.registerSize):
			cGProbabilities = Gtk.Grid()
			lProbabilities = Gtk.Label()
			lProbabilities.set_text("Qubit " + str(i))
			lProbabilities.set_size_request(80, 20)

			lZero = Gtk.Label()
			lZero.set_text("|0>")
			lZero.set_size_request(40, 20)
			self.eZeros.insert(i, Gtk.Entry())
			self.eZeros[i].set_text("1.0")
			self.eZeros[i].set_width_chars(5)
			lOne = Gtk.Label()
			lOne.set_text("|1>")
			lOne.set_size_request(40, 20)
			self.eOnes.insert(i, Gtk.Entry())
			self.eOnes[i].set_text("0.0")
			self.eOnes[i].set_width_chars(5)

			userInputOffset = 5 * i

			cGQubits.attach(lProbabilities, 0, userInputOffset + 0, 2, 1)
			cGQubits.attach(lZero, 0, userInputOffset + 1, 1, 1)
			cGQubits.attach(self.eZeros[i], 1, userInputOffset + 1, 1, 1)
			cGQubits.attach(lOne, 0, userInputOffset + 2, 1, 1)
			cGQubits.attach(self.eOnes[i], 1, userInputOffset + 2, 1, 1)

			self.daQubits.insert(i, Gtk.DrawingArea())
			self.daQubits[i].set_size_request(150, 100)
			self.daQubits[i].connect("draw", self.expose, i)
			cGQubits.attach(self.daQubits[i], 2, userInputOffset, 1, 3)
		
			self.bFindAngle.insert(i, Gtk.Button())
			self.bFindAngle[i].set_label("Find Angle")
			self.bFindAngle[i].connect("clicked", self.bFindAngle_exec, i)
			cGQubits.attach(self.bFindAngle[i], 3, userInputOffset, 1, 1)
			
			self.bMeasure.insert(i, Gtk.Button())
			self.bMeasure[i].set_label("Measure")
			self.bMeasure[i].connect("clicked", self.bMeasure_exec, i)
			cGQubits.attach(self.bMeasure[i], 4, userInputOffset, 1, 1)

			lPolarization = Gtk.Label()
			lPolarization.set_justify(Gtk.Justification.LEFT)
			lPolarization.set_text("Polarization:")
			lPolarization.set_size_request(200, 20)
			self.ePolarizations.insert(i, Gtk.Entry())
			self.ePolarizations[i].set_text(str(self.polarizations[i]))
			self.ePolarizations[i].set_width_chars(5)
			self.ePolarizations[i].set_editable(False)
			self.ePolarizations[i].set_has_frame(False)
			self.ePolarizations[i].set_can_focus(False)
			cGQubits.attach(lPolarization, 3, userInputOffset + 1, 1, 1)
			cGQubits.attach(self.ePolarizations[i], 4, userInputOffset + 1, 1, 1)

			lMeasurementAngle = Gtk.Label()
			lMeasurementAngle.set_justify(Gtk.Justification.LEFT)
			lMeasurementAngle.set_text("Detector Angle:")
			lMeasurementAngle.set_size_request(200, 20)
			self.eMeasurementAngles.insert(i, Gtk.Entry())
			self.eMeasurementAngles[i].set_text(str(self.measurementAngles[i]))
			self.eMeasurementAngles[i].set_width_chars(5)
			cGQubits.attach(lMeasurementAngle, 3, userInputOffset+2, 1, 1)
			cGQubits.attach(self.eMeasurementAngles[i], 4, userInputOffset+2, 1, 1)

		self.daRegister = Gtk.DrawingArea()
		self.daRegister.set_size_request(150, 400)
		self.daRegister.connect("draw", self.exposeRegister)
		cVRegister.pack_start(self.daRegister, True, False, 0)

		self.vt = Vte.Terminal()
		self.vt.set_size_request(400, 400)
		self.vt.fork_command_full(
			Vte.PtyFlags.DEFAULT,
			os.environ['PWD'],
			["/bin/bash"],
			[],
			GLib.SpawnFlags.DO_NOT_REAP_CHILD,
			None,
			None,
		)

		command = "./superpositionController\r\n"
		self.vt.feed_child(command, len(command))
		self.vt.hide()
		cGMain.attach(self.vt, 4, 1, 1, 8)

		self.add(cGMain)
		self.show_all()
		self.vt.hide()
		time.sleep(1)
		self.initDone = 1

	def bFindAngle_exec(self, widget, ra):
		rawAngle = self.stub.FindAngle(gui_pb2.FindAngleMessage(pZero=float(self.eZeros[ra].get_text()), pOne=float(self.eOnes[ra].get_text())), 120)
		deltaAngle = rawAngle.angle - float(self.eMeasurementAngles[ra].get_text())
		self.ePolarizations[ra].set_text(str(deltaAngle))
		polarize = self.stub.Polarize(gui_pb2.PolarizationMessage(RegisterAddress=ra, Angle=deltaAngle), 120)
		self.polarizations[ra] = deltaAngle
		self.updateDaQubits(self.daQubits[ra], 0, ra)
		self.exposeRegister(self, self.daRegister.get_window().cairo_create())
		
        def bPolarize_exec(self, widget, ra):
		deltaAngle = float(self.ePolarizations[ra].get_text()) - self.polarizations[ra]
		polarize = self.stub.Polarize(gui_pb2.PolarizationMessage(RegisterAddress=ra, Angle=deltaAngle), 120)
		self.polarizations[ra] = deltaAngle + self.polarizations[ra]
		self.updateDaQubits(self.daQubits[ra], 0, ra)
		self.exposeRegister(self, self.daRegister.get_window().cairo_create())
			
	def bChangeAngle_exec(self, widget, ra):
		deltaAngle = -1.0 * (float(self.eMeasurementAngles[ra].get_text()) - self.measurementAngles[ra])
		polarize = self.stub.Polarize(gui_pb2.PolarizationMessage(RegisterAddress=ra, Angle=deltaAngle), 120)
		self.measurementAngles[ra] = self.measurementAngles[ra] - deltaAngle
		self.updateDaQubits(self.daQubits[ra], 0, ra)
		self.exposeRegister(self, self.daRegister.get_window().cairo_create())
		
	
	def bMeasure_exec(self, widget, ra):
		result = self.stub.Measure(gui_pb2.RegisterAddressMessage(RegisterAddress=ra), 120)
		if result.RegisterAddress == 0 :
			self.polarizations[ra] = 0
			self.ePolarizations[ra].set_text("0")
			self.eZeros[ra].set_text("1.0")
			self.eOnes[ra].set_text("0.0")
		else :
			self.polarizations[ra] = 180
			self.ePolarizations[ra].set_text("180")
			self.eZeros[ra].set_text("0.0")
			self.eOnes[ra].set_text("1.0")

		self.measurementAngles[ra] = 0
		self.eMeasurementAngles[ra].set_text("0.0")
		self.updateDaQubits(self, self.polarizations[ra], ra)
		self.exposeRegister(self, self.daRegister.get_window().cairo_create())

	def updateDaQubits(self, widget, user_data, i):
		self.expose(self.daQubits[i], self.daQubits[i].get_window().cairo_create(), i)

	def expose(self, widget, cr, i):
		vcenter = 50
		hcenter = 75 
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
#		cr.text_path("|0>")
#		cr.move_to(hcenter, vcenter + radius + fontSize)
		cr.text_path("|1>")
#		cr.move_to(hcenter, vcenter - radius - fontSize)
#		cr.text_path("|1>")
		cr.stroke()

		cr.set_source_rgba(0, 0, 0, 0.1)
		cr.arc(hcenter, vcenter, radius+4, (self.measurementAngles[i]+10)/180.0 * math.pi, (self.measurementAngles[i]+170)/180.0 * math.pi)
		cr.fill()
		cr.arc(hcenter, vcenter, radius+4, (self.measurementAngles[i]+190.0)/180.0 * math.pi, (self.measurementAngles[i]+350.0)/180.0 * math.pi)
		cr.fill()

	def exposeRegister(self, widget, cr):
		registerWidth = pow(2, self.registerSize)
		padding = 10
		baseHeight = 400
		drawHeight = baseHeight - 2 * padding
		zeroWidth = 40 
		maxWidth = 100
		barHeight = drawHeight/registerWidth

		cr.set_source_rgb(0, .6588, .4196)
		cr.paint()
		
		cr.set_source_rgb(.3255, .3255, .3255)
		cr.rectangle(zeroWidth, padding, maxWidth, drawHeight)
		cr.stroke()

		if self.initDone == 1:
			status = self.stub.GetRegisterStatus(gui_pb2.VoidMessage(), 120)
			for node in status.nodes:
				xStart = padding
				yStart = padding + node.NodeID * barHeight
				xProb = node.NodeProbability * maxWidth 
			
				cr.set_source_rgb(.3255, .3255, .3255)
				cr.move_to(padding, yStart + barHeight / 2)
				cr.text_path("|" + str(node.NodeID) + ">")
				cr.stroke()
				
				cr.set_source_rgb(1, .75, 0)
				cr.move_to(zeroWidth, yStart)
				cr.rectangle(zeroWidth, yStart, xProb, barHeight)
				cr.fill()

	def getDrawAngle(angle):
		return (angle / 180 - .5) * math.pi

	def bRun_exec(self, bRun):
		print "SUPERPOSITION 1\r\n"
