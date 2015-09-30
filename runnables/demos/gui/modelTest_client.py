#!/usr/bin/python
from grpc.beta import implementations
import gui_pb2

def run():
	channel = implementations.insecure_channel('localhost', 60000)
	stub = gui_pb2.beta_create_ControllerView_stub(channel)
	print "here"
	polarize = stub.Polarize(gui_pb2.PolarizationMessage(RegisterAddress=1, Angle=90), 5)
	print "here"

if __name__ == '__main__':
	run()
