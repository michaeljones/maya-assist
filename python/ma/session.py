
from maya import cmds
from maya import standalone

import socket
import pickle
import sys
import time

def run():

    HOST = ''
    PORT = 50007

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    while 1:
        try:
            s.bind((HOST, PORT))
            break
        except Exception, e:
            print "Failed to bind, trying again"
            time.sleep(1)

    s.listen(1)

    standalone.initialize()

    while 1:

        conn, addr = s.accept()

        data = conn.recv(1024)
        if not data:
            break

        command_object = pickle.loads(data)

        command = getattr(cmds, command_object.name)

        try:
            results = command(*command_object.args, **command_object.kwargs)
        except Exception, e:
            print e
            continue

        result_string = pickle.dumps(results)

        conn.sendall(result_string)

        conn.close()

    s.shutdown(socket.SHUT_RDWR)
    s.close()

