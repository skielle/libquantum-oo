#!/usr/bin/python
from grpc.beta import implementations
from pprint import pprint
import gui_pb2

def run():
	channel = implementations.insecure_channel('localhost', 60000)
	stub = gui_pb2.beta_create_ControllerView_stub(channel)
	print "here"
	polarize = stub.Polarize(gui_pb2.PolarizationMessage(RegisterAddress=0, Angle=90), 120)
	polarize = stub.Polarize(gui_pb2.PolarizationMessage(RegisterAddress=1, Angle=90), 120)
	polarize = stub.Polarize(gui_pb2.PolarizationMessage(RegisterAddress=2, Angle=90), 120)
	polarize = stub.Polarize(gui_pb2.PolarizationMessage(RegisterAddress=3, Angle=90), 120)
	measure = stub.Measure(gui_pb2.RegisterAddressMessage(RegisterAddress=1), 120)
	print measure.RegisterAddress;
	print "here"

if __name__ == '__main__':
	run()
