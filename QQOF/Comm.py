import socket
import struct


class Package:
    def __init__(self) -> None:
        self.rawData = b''

    def pack(self, data: str) -> bytes:
        buildmsg = data.encode()
        return struct.pack("Q", len(buildmsg)) + buildmsg

    def write(self, data: bytes):
        self.rawData += data

    def canRead(self) -> bool:
        if len(self.rawData) < 8:
            return False
        elif struct.unpack("Q", self.rawData[:8])[0] > (len(self.rawData) - 8):
            return False
        else:
            return True

    def Read(self) -> str:
        if self.canRead() == False:
            raise Exception("无法读取")

        returnDataSize = 8 + struct.unpack("Q", self.rawData[:8])[0]
        returnData = self.rawData[8: 8 + returnDataSize]
        self.rawData = self.rawData[8 + returnDataSize:]

        return returnData
