import zmq
context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:40000")
socket.send(b"world")
message = socket.recv()
print(message)
