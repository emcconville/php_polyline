#ifndef PHP_POLYLINE_H
#define PHP_POLYLINE_H

#define PHP_POLYLINE_VERSION "1.0"
#define PHP_POLYLINE_EXTNAME "polyline"

extern zend_module_entry polyline_module_entry;
PHP_MINIT_FUNCTION(polyline);
PHP_MSHUTDOWN_FUNCTION(polyline);
PHP_MINFO_FUNCTION(polyline);
PHP_FUNCTION(polyline_encode);
PHP_FUNCTION(polyline_decode);

void _polyline_encode_chunk( long delta, smart_str * buffer );
long _polyline_decode_chunk( char * buffer, int * buffer_length );

#endif

