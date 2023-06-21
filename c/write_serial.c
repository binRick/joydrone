
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <cserial/c_serial.h>

int main( int argc, char** argv ){
    c_serial_port_t* m_port;
    c_serial_control_lines_t m_lines;
    int status, bytes_read, data_length, x;
    uint8_t data[ 255 ];

    if( argc != 2 ){
        fprintf( stderr, "ERROR: First argument must be serial port\n" );
		const char** port_list = c_serial_get_serial_ports_list();
		x = 0;
		printf("Available ports:\n");
		while( port_list[ x ] != NULL ){
			printf( "%s\n", port_list[ x ] );
			x++;
		}
		c_serial_free_serial_ports_list( port_list );
        return 1;
    }
 
    c_serial_set_global_log_function( c_serial_stderr_log_function );

    if( c_serial_new( &m_port, NULL ) < 0 ){
        fprintf( stderr, "ERROR: Unable to create new serial port\n" );
        return 1;
    }

    if( c_serial_set_port_name( m_port, argv[ 1 ] ) < 0 ){
        fprintf( stderr, "ERROR: can't set port name\n" );
    }

    c_serial_set_baud_rate( m_port, CSERIAL_BAUD_115200 );
    c_serial_set_data_bits( m_port, CSERIAL_BITS_8 );
    c_serial_set_stop_bits( m_port, CSERIAL_STOP_BITS_1 );
    c_serial_set_parity( m_port, CSERIAL_PARITY_NONE );
    c_serial_set_flow_control( m_port, CSERIAL_FLOW_NONE );

    printf( "Baud rate is %d\n", c_serial_get_baud_rate( m_port ) );
    c_serial_set_serial_line_change_flags( m_port, CSERIAL_LINE_FLAG_ALL );

    status = c_serial_open( m_port );
    if( status < 0 ){
        fprintf( stderr, "ERROR: Can't open serial port\n" );
        return 1;
    }

    do{
	char *s = "{\"pitch\": 15, \"yaw\": 996, \"roll\": 0, \"throttle\": 2000,\"button0\": 0}";
        status = c_serial_write_data( m_port, s, &data_length);
        if( status < 0 ){
	    printf("error writing to port\n");
            break;
        }
	printf("wrote %d bytes to port\n",strlen(s));
    }while( 1 );
}
