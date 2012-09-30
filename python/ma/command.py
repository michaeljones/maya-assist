
import subprocess
import socket
import pickle
import time
import sys

class MayaCommand(object):

    def __init__(self, name):
        self.name = name

    def __call__(self, *args, **kwargs):

        self.args = args
        self.kwargs = kwargs

        pickle_string = pickle.dumps(self)

        HOST = 'localhost'    # The remote host
        PORT = 50007          # The same port as used by the server
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        while 1:
            try:
                s.connect((HOST, PORT))
                break
            except Exception, e:
                print "Waiting for Maya subprocess to load"
                time.sleep(1)

        s.sendall(pickle_string)

        all_data = ""
        while 1:
            new_data = s.recv(4096)
            if new_data:
                all_data += new_data
            else:
                break

        s.close()

        data = pickle.loads(all_data)

        return data


class MayaSession(object):

    def __init__(self, mayapy_path):
        self.initialised = False
        self.maya_process = False
        self.mayapy_path = mayapy_path

    def close(self):

        HOST = 'localhost'    # The remote host
        PORT = 50007          # The same port as used by the server
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        while 1:
            try:
                s.connect((HOST, PORT))
                break
            except:
                print "Failed to connect"
                import time
                time.sleep(1)

        # Send nothing to indicate we're done
        s.sendall("")

        s.close()

        self.maya_process.wait()

    def initialise(self):

        if not self.initialised:

            try:
                self.maya_process = subprocess.Popen(
                        [self.mayapy_path, "-c", "from ma import session; session.run()" ],
                        )

                self.initialised = True

            except Exception, e:
                sys.stderr.write("Failed to initiaise Maya standalone in Python session")
                print e


    def __getattr__(self, name):

        self.initialise()

        return MayaCommand(name)

