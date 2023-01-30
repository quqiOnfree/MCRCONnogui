import socket,QQOF

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
    
    def onLoad(self):
        pass

    def onEnable(self):
        pass

    def onDisable(self):
        pass
    
    def onReceive(self, server: str, data: str):
        pass
