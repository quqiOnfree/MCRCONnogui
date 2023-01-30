from QQOF.Logger import Logger
from QQOF.PythonPlugin import PythonPlugin
from QQOF.Comm import Package
import json,os

locLogger = None

def getMCRCONPort() -> int:
    with open("./config.json", 'r') as f:
        try:
            return json.load(f)["MCRCON"]['port']
        except:
            getLogger().error("无法获取MCRCON的服务器端口")
            os._exit(-1)

def getLogger() -> Logger:
    global locLogger
    return locLogger

def sendCommand(data: str):
    global locLogger
    locLogger.info(data)

