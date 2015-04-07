import platform

class system_info(object):

    architecture = (platform.processor())
    if architecture == 'arm':
        print ('Still need to develop pi program')
