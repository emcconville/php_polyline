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

#endif

