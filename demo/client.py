import zmq
context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:12321")
socket.send(b"test 0")
message = socket.recv()
print(message)
