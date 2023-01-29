import time, os

class Logger:
    def __init__(self) -> None:
        pass

    def init(self, path: str):
        try:
            os.mkdir(path)
        except:
            pass

    def info(self, data: str):
        print(data)