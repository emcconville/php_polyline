#ifndef PHP_POLYLINE_H
#define PHP_POLYLINE_H

#define PHP_POLYLINE_VERSION "1.0"
#define PHP_POLYLINE_EXTNAME "polyline"
#ifndef PHP_POLYLINE_REVISION
#define PHP_POLYLINE_REVISION "dev"
#endif

extern zend_module_entry polyline_module_entry;
PHP_MINIT_FUNCTION(polyline);
PHP_MSHUTDOWN_FUNCTION(polyline);
PHP_MINFO_FUNCTION(polyline);
PHP_FUNCTION(polyline_encode);
PHP_FUNCTION(polyline_decode);
PHP_FUNCTION(polyline_validate_encoded_string);

static inline void _polyline_encode_chunk( long delta, smart_str * buffer );
static inline long _polyline_decode_chunk( char * buffer, int * buffer_length );
static inline int  _polyline_validate_encoded_string( char * ptr, int len, int tuple );

static inline int  _polyline_get_ini_tuple(void);
static inline int  _polyline_get_ini_precision(void);

#endif

