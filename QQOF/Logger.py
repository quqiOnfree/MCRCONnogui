import time
import os


class Logger:
    def __init__(self) -> None:
        self.logger = None

    def __del__(self):
        if self.logger != None:
            self.logger.close()

    def init(self, path: str):
        try:
            os.mkdir(path)
            print("创建 ./logs/ 成功")
        except:
            pass

        self.logger = open("./logs/" + time.strftime("%Y-%m-%d-%H-%M-%S",
                           time.localtime(time.time())) + '.log', "w", encoding="utf-8")

    def IOOut(self, data: str, dataType: str):
        prefix = "[{}][{}]".format(time.strftime(
            "%Y-%m-%d %H:%M:%S", time.localtime(time.time())), dataType)
        message = prefix + data
        print(message)
        self.logger.write(message + '\n')
        self.logger.flush()

    def info(self, data: str):
        self.IOOut(data, 'INFO')

    def warning(self, data: str):
        self.IOOut(data, 'WARNING')
    
    def error(self, data:str):
        self.IOOut(data, 'ERROR')
