import logging


class ColorfulFormatter(logging.Formatter):
    def format(self, record):
        ESC = "\033"
        symbols = {
            "INFO": f" {ESC}[1m{ESC}[36m[-]{ESC}[0m",
            "WARNING": f" {ESC}[1m{ESC}[33m[*]{ESC}[0m",
            "ERROR": f" {ESC}[1m{ESC}[31m[!]{ESC}[0m",
            "DEBUG": f" {ESC}[1m{ESC}[37m[-]{ESC}[0m",
            "CRITICAL": f" {ESC}[1m{ESC}[4m{ESC}[31m[!]{ESC}[0m",
        }

        level = record.levelname
        symbol = symbols.get(level, "[?]")
        timestamp = self.formatTime(record, self.datefmt)
        return f"{symbol} {ESC}[1m[{timestamp}]{ESC}[0m: {record.getMessage()}"
