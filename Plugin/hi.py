import QQOF

PluginInfo = {
    "Name": "Hi",
    "Start": 'Main',
    "Version": "1.0"
}


class Main(QQOF.PythonPlugin):
    def __init__(self):
        super().__init__()

    def onEnable(self):
        QQOF.getLogger().info("hi! Plugin!")
