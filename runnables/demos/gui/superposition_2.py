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

class Superposition_2(Gtk.Window):
	registerSize = 4
	initDone = 0

	def __init__(self):
		Gtk.Window.__init__(self)

		self.channel = implementations.insecure_channel('localhost', 60000)
		self.stub = gui_pb2.beta_create_ControllerView_stub(self.channel)

		self.connect("delete-event", Gtk.main_quit)
		self.set_title("Superposition 2")
		self.resize(800, 600)
		self.daQubits = [self.registerSize]
		self.ePolarizations = [self.registerSize]
		self.eMeasurementAngles = [self.registerSize]
		self.bPolarize = [self.registerSize]
		self.bMeasure = [self.registerSize]
		self.bChangeAngle = [self.registerSize]

		self.polarizations = [0.0, 0.0, 0.0, 0.0]
		self.measurementAngles = [0.0, 0.0, 0.0, 0.0]

		cVLauncher = Gtk.VBox()
		cHLauncher = Gtk.HBox()
		cHLauncher.set_size_request(700, 30)
		cHma = Gtk.HBox()
		cHma.set_size_request(700, 30)
		cHdaQubits = Gtk.HBox()
		cHdaQubits.set_size_request(800, 100)
		
		cVLauncher.pack_start(cHLauncher, True, False, 0)
		cVLauncher.pack_start(cHma, True, False, 0)
		cVLauncher.pack_start(cHdaQubits, True, False, 0)

		for i in range(self.registerSize):
			self.daQubits.insert(i, Gtk.DrawingArea())
			self.daQubits[i].set_size_request(200, 150)
			self.daQubits[i].connect("draw", self.expose, i)
			cHdaQubits.pack_start(self.daQubits[i], True, False, 0)

		for i in range(self.registerSize):
			self.ePolarizations.insert(i,Gtk.Entry())
			self.ePolarizations[i].set_text(str(self.polarizations[i]))
			self.ePolarizations[i].set_width_chars(5)
			self.bPolarize.insert(i,Gtk.Button())
			self.bPolarize[i].set_label("Polarize")
			self.bPolarize[i].connect("clicked", self.bPolarize_exec, i)
			self.bMeasure.insert(i,Gtk.Button())
			self.bMeasure[i].set_label("Measure")
			self.bMeasure[i].connect("clicked", self.bMeasure_exec, i)
			cHLauncher.pack_start(
				self.ePolarizations[i], True, False, 0)
			cHLauncher.pack_start(
				self.bPolarize[i], True, False, 0)
			cHLauncher.pack_start(
				self.bMeasure[i], True, False, 0)

		for i in range(self.registerSize):
			self.eMeasurementAngles.insert(i,Gtk.Entry())
			self.eMeasurementAngles[i].set_text(str(self.measurementAngles[i]))
			self.eMeasurementAngles[i].set_width_chars(5)
			self.bChangeAngle.insert(i,Gtk.Button())
			self.bChangeAngle[i].set_label("Set Detector Angle")
			self.bChangeAngle[i].connect("clicked", self.bChangeAngle_exec, i)

			cHma.pack_start(
				self.eMeasurementAngles[i], True, False, 0)
			cHma.pack_start(
				self.bChangeAngle[i], True, False, 0)

		self.daRegister = Gtk.DrawingArea()
		self.daRegister.set_size_request(800, 100)
		self.daRegister.connect("draw", self.exposeRegister)
		cVLauncher.pack_start(self.daRegister, True, False, 0)

		self.vt = Vte.Terminal()
		self.vt.set_size_request(800, 400)
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
		cVLauncher.pack_start(self.vt, True, True, 0)	

		self.add(cVLauncher)
		self.show_all()
		time.sleep(1)
		self.initDone = 1

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
		else :
			self.polarizations[ra] = 180
			self.ePolarizations[ra].set_text("180")
#			self.polarizations[ra] = 90
#			self.ePolarizations[ra].set_text("90")

		self.measurementAngles[ra] = 0
		self.eMeasurementAngles[ra].set_text("0.0")
		self.updateDaQubits(self, self.polarizations[ra], ra)
		self.exposeRegister(self, self.daRegister.get_window().cairo_create())

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
		offset = 50
		baseWidth = 800
		drawWidth = baseWidth - offset - padding
		zeroHeight = 80

		cr.set_source_rgb(0, .6588, .4196)
		cr.paint()
		
		cr.set_source_rgb(.3255, .3255, .3255)
		cr.rectangle(50, 10, 740, 70)
		cr.stroke()

		if self.initDone == 1:
			status = self.stub.GetRegisterStatus(gui_pb2.VoidMessage(), 120)
			for node in status.nodes:
				xStart = offset+node.NodeID*drawWidth/registerWidth
				yProb = zeroHeight - node.NodeProbability * (zeroHeight - padding)
			
				cr.set_source_rgb(.3255, .3255, .3255)
				cr.move_to(xStart, zeroHeight + padding)
				cr.text_path("|" + str(node.NodeID) + ">")
				cr.stroke()
				
				cr.set_source_rgb(1, .75, 0)
				cr.move_to(xStart, zeroHeight)
				cr.rectangle(xStart, yProb, drawWidth/registerWidth, zeroHeight - yProb)
				cr.fill()

	def getDrawAngle(angle):
		return (angle / 180 - .5) * math.pi

	def bRun_exec(self, bRun):
		print "SUPERPOSITION 1\r\n"
