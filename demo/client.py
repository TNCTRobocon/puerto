import zmq
import json
context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:40000")
motors = {'#1': {'duty': 99}}
commands = {'motors': motors}
socket.send_json(commands)
message = socket.recv()
print(message)
