PluginInfo = {
    "Name": "PythonPlugin",
    "Start": 'PythonPlugin',
    "Version": "1.0"
}

class PythonPlugin:
    def __init__(self):
        pass
    
    def onLoad(self):
        pass

    def onEnable(self):
        pass

    def onDisable(self):
        pass
    
    def onReceive(self, server: str, data: str):
        pass

    def takePluginInfo(self):
        return PluginInfo
