import socket
import QQOF

PluginInfo = {
    "Name": "PythonPlugin",
    "Start": 'PythonPlugin',
    "Version": "1.0"
}


class PythonPlugin:
    def __init__(self):
        self.serverPort = QQOF.getMCRCONPort()
        self.serverSocket = socket.socket
        self.serverSocket = socket.socket(("127.0.0.1", self.serverPort))
        self.MsgReceive = QQOF.MsgReceive(self.serverSocket)

    def onLoad(self):
        pass

    def onEnable(self):
        pass

    def onDisable(self):
        pass

    def onReceive(self, server: str, data: str):
        pass

    def sendMessage(self, server: str, data: str):
        basePackage = QQOF.Package()
        self.serverSocket.send(basePackage.pack(basePackage.pack(
            server.encode())+basePackage.pack(data.encode())))
