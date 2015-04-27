class Error(Exception):
    """Base class for other exceptions"""
    pass

class serialPortDefError(Error):
    """Raised when no serial port is defined"""
    pass

class baudRateDefError(Error):
    """Raised when no baud rate is defined"""
    pass
