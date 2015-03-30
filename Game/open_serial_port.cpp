#include <iostream>
#include <cstdio>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main( int    argc,
      char** argv )
{
    const std::string SERIAL_PORT_DEVICE = "/dev/tty.usbmodem1431" ;
    //
    // Try to open the serial port device.
    //
    int fd = open( SERIAL_PORT_DEVICE.c_str(),
                   O_RDWR ) ;
    if ( -1 == fd )
    {
        std::cerr << "Failed to open the serial port device "
                  << SERIAL_PORT_DEVICE
                  << std::endl ;
        return EXIT_FAILURE ;
    }
    else
    {
        std::cerr << "Serial port device opened successfully."
                  << std::endl ;
        close(fd) ;
    }
    return EXIT_SUCCESS ;
}
