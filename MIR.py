import time

from global_header import *
from SocketClass import *
from PacketFormat import *

attack_info = "FILELESS"
SUCCESS = 0
FAILURE = 1

class MIR_Process():
    def __init__(self, ip, port):
        print("MIR Process started.")
        self.__mir_sock = client_connect(__ip=ip, __port=port)
        mir_ip, port = self.__mir_sock.getsockname()
        print("Connecting with {}:{}".format(mir_ip, port))
        self.pkt_obj = PacketConf()
        data = {}

        data["type"] = "MIR"
        data["status"] = "NIL"
        data["os"] = "WIN7"
        data["message"] = "MALWARE_TEST"
        data["process"] = "REQUEST"
        self.pkt_obj.packet_send(self.__mir_sock, data)

        while(1):
            data = self.pkt_obj.packet_recv(self.__mir_sock)
            if data["message"] == "MALWARE_TEST" and data["process"] == "NOT_AVAILABLE":
                print("MTC is not available for testing. Notifying the server to close connection.")
                self.__mir_sock.close()
                break
            elif data["message"] == "MALWARE_TEST" and data["process"] == "INVALID_REQUEST":
                print("Server responded with invalid request. Run the setup with valid request. Notifying the server to close connection.")
                data["type"] = "MIR"
                data["status"] = "NIL"
                data["os"] = "WIN7"
                data["message"] = "MALWARE_TEST"
                data["process"] = "CLOSING_SOCK"
                self.__mir_sock.close()
                sys.exit(FAILURE)
            elif data["message"] == "MALWARE_TEST" and data["process"] == "AVAILABLE":
                print("MTC is available for testing. Notifying the server to perform the test.")
                data["type"] = "MIR"
                data["status"] = "NIL"
                data["os"] = "WIN7"
                data["process"] = "PERFORM"
                data["message"] = "MALWARE_TEST"
                self.pkt_obj.packet_send(self.__mir_sock, data)

                # data["process"] = input("Enter the attack name to perform the test: ")
                data["process"] = getAttackAction()
                self.pkt_obj.packet_send(self.__mir_sock, data)

                track = "PERFORM_TEST"
            elif data["message"] == "MALWARE_TEST" and data["process"] == "COMPLETED":
                print("Malware test completed. Notifying the server to close connection.")
                if track == "PERFORM_TEST":
                    data["type"] = "MIR"
                    data["status"] = "NIL"
                    data["os"] = "WIN7"
                    data["message"] = "MALWARE_TEST"
                    data["process"] = "DONE"
                    self.pkt_obj.packet_send(self.__mir_sock, data)
                    self.__mir_sock.close()
                    sys.exit(SUCCESS)
            elif data["message"] == "MALWARE_TEST" and data["process"] == "FAILURE":
                print("Malware test ended in failure. Notifying the server to close connection.")
                if track == "PERFORM_TEST":
                    data["type"] = "MIR"
                    data["status"] = "NIL"
                    data["os"] = "WIN7"
                    data["message"] = "MALWARE_TEST"
                    data["process"] = "DONE"
                    self.pkt_obj.packet_send(self.__mir_sock, data)
                    self.__mir_sock.close()
                    sys.exit(FAILURE)

