from QQOF.Logger import Logger
from QQOF.PythonPlugin import PythonPlugin

locLogger = Logger

def getLogger():
    global locLogger
    return locLogger
