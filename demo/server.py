import zmq

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:12321")
while True:
    message = socket.recv()
    print(message)
    socket.send(b"test 1")
