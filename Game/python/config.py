import platform

class sys_config(object):

    def __init__(self):

        architecture = (platform.processor())

        if architecture == 'arm':

            # This window height is the pi resolution
            self.start_window_width = 480
            self.start_window_height = 320

            # game_x_res = 1024
            # game_y_res = 640

            self.input_options = ['Tilt Sensor']

        else:
            self.start_window_width = 512
            self.start_window_height = 320

            # self.game_x_res = 1024
            # self.game_y_res = 640

            self.input_options = ['Arrow Keys','Web Cam']

    def get_start_window_dim(self):
        return (self.start_window_width, self.start_window_height)

    def get_game_res(self):
        return (self.game_x_res, self.game_y_res)

    def get_sys_input_options(self):
        return self.input_options

    #def set_game_options(self, new_game, input_method, serial_port, baud_rate):
    def set_game_options(self, new_game, input_method,baud_rate):
        new_game = new_game
        input_method = input_method
        #serial_port = serial_port
        baud_rate = baud_rate

        print (new_game, input_method, baud_rate)
