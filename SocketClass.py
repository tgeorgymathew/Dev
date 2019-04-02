import os
import sys
import socket
import time
import threading

def server_configuration( __ip, __port):
    __address = (__ip, __port)

    __sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    __sock.bind(__address)
    print("Listening")
    __sock.listen(5)
    return __sock

def accept_connection(server_sock):
    __client_sock, __client_address = server_sock.accept()
    __client_sock.settimeout(60)
    return __client_sock, __client_address

def client_connect(__ip, __port):
    print("Connecting to {}:{}".format(__ip, __port))
    addr = (__ip, __port)
    __sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        __sock.connect(addr)
        print("Successfully connected to {}:{}".format(__ip, __port))
    except ConnectionRefusedError:
        print("No connection could be made because the target machine '{}' actively refused it".format(addr))
        exit(1)
    return __sock
