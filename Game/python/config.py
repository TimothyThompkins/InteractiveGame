import platform

class sys_config(object):

    def __init__(self):
        # self.start_window_width = realpart
        # self.start_window_height = imagpart
        #
        # def __set_stats(self):
        architecture = (platform.processor())

        if architecture == 'arm':
            # start_window_width = 512
            # start_window_height = 320
            #
            # game_x_res = 1024
            # game_y_res = 1024

            self.input_options = ['Tilt Sensor']

        else:
            self.start_window_width = 512
            self.start_window_height = 320

            self.game_x_res = 1024
            self.game_y_res = 1024

            self.input_options = ['Arrow Keys','Web Cam']
            #self.input_options = 'test'

    def get_start_window_dim(self):
        return (self.start_window_width, self.start_window_height)

    def get_game_res(self):
        return (self.game_x_res, self.game_y_res)

    def get_sys_input_options(self):
        return self.input_options
