# import socket
# import struct

# PORT, HOST_IP = 8080, '127.0.0.1'
# key = 4

# with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
#     s.bind((HOST_IP, PORT))
#     s.listen()
#     print("starting to listen")
#     conn, addr = s.accept()
#     with conn:
#         print('Connected by', addr)
#         while True:
#             # t = int(input("value (int): "))
#             # assert t >= 0
#             # d = struct.pack('I', t)
#             # conn.sendall(d)
#             msg = conn.recv(1000)
#             received_text = msg.decode('utf-8')
#             print("Received text:", received_text)

import socket

PORT, HOST_IP = 8080, '127.0.0.1'

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST_IP, PORT))
    s.listen()
    print("Server started, waiting for connections...")

    conn, addr = s.accept()
    print('Connected by', addr)

    while True:
        # Receive message from client
        msg = conn.recv(1000)
        if not msg:
            break

        try:
            received_text = msg.decode('utf-8')
            print("Client:", received_text)
        except UnicodeDecodeError:
            print("Received non-UTF-8 data")

        # Process the message (here, simply echoing back to the client)
        response_text = input("You: ")

        # Send response back to client
        conn.sendall(response_text.encode('utf-8'))

    print("Connection closed.")
