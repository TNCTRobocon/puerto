import zmq

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:40000")
while True:
    message = socket.recv()
    print(message)
    socket.send(b"test 1")
