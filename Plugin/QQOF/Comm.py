import socket
import struct
import time,QQOF
from threading import Thread


class Package:
    def __init__(self) -> None:
        self.rawData = b''

    def pack(self, data: bytes) -> bytes:
        return struct.pack("Q", len(data)) + data

    def write(self, data: bytes):
        self.rawData += data

    def canRead(self) -> bool:
        if len(self.rawData) < 8:
            return False
        elif struct.unpack("Q", self.rawData[:8])[0] > (len(self.rawData) - 8):
            return False
        else:
            return True

    def read(self) -> bytes:
        if self.canRead() == False:
            raise Exception("无法读取")

        returnDataSize = 8 + struct.unpack("Q", self.rawData[:8])[0]
        returnData = self.rawData[8: 8 + returnDataSize]
        self.rawData = self.rawData[8 + returnDataSize:]

        return returnData


class MsgReceive:
    def __init__(self, Socket: socket.socket, Plugin: QQOF.PythonPlugin) -> None:
        self.thread = Thread(target=self.run)
        self.package = Package()
        self.socket = Socket
        self.isRunning = True
        self.plugin = Plugin

    def __del__(self):
        self.stop()

    def run(self):
        while self.isRunning:
            self.package.write(self.socket.recv(8192))
            while self.package.canRead():
                basePackage = QQOF.Package()
                basePackage.write(self.package.read())
                self.plugin.onReceive(basePackage.read(), basePackage.read())

    def stop(self):
        self.isRunning = False
        self.thread.join()
