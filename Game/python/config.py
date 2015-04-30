import platform
import sys

class sys_config(object):

    def __init__(self):

        # These are what we need for each instance of game
        self.game_init = None
        self.new_game = None
        self.input_method = None
        self.serial_port = None
        self.baud_rate = None

        if sys.platform.startswith('win'):
            self.sys_platform = 'windows'

        # For Linux Platforms
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            self.sys_platform = 'linux'

        # For OSX Platforms
        elif sys.platform.startswith('darwin'):
            self.sys_platform = 'OSX'

        else:
            self.sys_platform = None
            raise EnvironmentError('Unsupported platform')

        architecture = (platform.processor())

        # For Rasberry Pi (probably)
        if architecture == 'arm':

            # This window height is the pi resolution
            self.start_window_width = 480
            self.start_window_height = 320

            self.game_window_width = 480
            self.game_window_height = 320

            self.input_options = ['Tilt Sensor']

        # For PC
        else:
            self.start_window_width = 512
            self.start_window_height = 320

            self.game_window_width = 1024
            self.game_window_height = 640

            self.input_options = ['Arrow Keys','Web Cam']

    # Get Data
    def get_sys_platform(self):
        return self.sys_platform

    def get_start_window_dim(self):
        return (self.start_window_width, self.start_window_height)

    def get_game_window_dim(self):
        return (self.game_window_width, self.game_window_height)

    def get_sys_input_options(self):
        return self.input_options

    def get_game_init(self):
        return self.game_init

    def get_new_game(self):
        return self.new_game

    def get_input_method(self):
        return self.input_method

    def get_serial_port(self):
        return self.serial_port

    def get_baud_rate(self):
        return self.baud_rate

    # Set Data
    def set_game_init(self, game_init):
        self.game_init = game_init

    def set_new_game(self, new_game):
        self.new_game = new_game

    def set_input_method(self, input_method):
        self.input_method = input_method

    def set_serial_port(self, serial_port):
        self.serial_port = serial_port

    def set_baud_rate(self, baud_rate):
        self.baud_rate = baud_rate


    def print_data(self):
        print (self.new_game, self.input_method, self.serial_port, self.baud_rate)
