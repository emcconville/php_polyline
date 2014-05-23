#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_smart_str.h"
#include "ext/standard/php_math.h"
#include "php_polyline.h"

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    PHP_INI_ENTRY("polyline.tuple","2",PHP_INI_ALL,NULL) // Change to enforce short
    PHP_INI_ENTRY("polyline.precision","5",PHP_INI_ALL,NULL)
PHP_INI_END()
/* }}} */

ZEND_BEGIN_ARG_INFO(arginfo_polyline_encode,0)
    ZEND_ARG_INFO(0,array)
ZEND_END_ARG_INFO();
ZEND_BEGIN_ARG_INFO(arginfo_polyline_decode,0)
    ZEND_ARG_INFO(0,string)
ZEND_END_ARG_INFO();
ZEND_BEGIN_ARG_INFO(arginfo_polyline_validate_encoded_string,0)
    ZEND_ARG_INFO(0,string)
ZEND_END_ARG_INFO();

/* {{{ polyline_functions[]
 */
zend_function_entry polyline_functions[] = {
    PHP_FE(polyline_encode,arginfo_polyline_encode)
    PHP_FE(polyline_decode,arginfo_polyline_decode)
    PHP_FE(polyline_validate_encoded_string,arginfo_polyline_validate_encoded_string)
    {NULL,NULL,NULL}
};
/* }}} */

/* {{{ polyline_module_entry
 */
zend_module_entry polyline_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_POLYLINE_EXTNAME,/*Name*/
    polyline_functions, /*Function*/
    PHP_MINIT(polyline), /*Init*/
    PHP_MSHUTDOWN(polyline), /*shutdown*/
    NULL, /*User init*/
    NULL, /*User shutdown*/
    PHP_MINFO(polyline), /*Info*/
    PHP_POLYLINE_VERSION, /*Version*/
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_POLYLINE
ZEND_GET_MODULE(polyline)
#endif

PHP_MINIT_FUNCTION(polyline)
{
    REGISTER_INI_ENTRIES();
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(polyline)
{
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}

PHP_MINFO_FUNCTION(polyline)
{
    php_info_print_table_start();
    php_info_print_table_header(2,"polyline support","enabled");
    php_info_print_table_row(2,"author","E. McConville <emcconville@emcconville.com>");
    php_info_print_table_row(2,"version",PHP_POLYLINE_VERSION);
    php_info_print_table_row(2,"polyline.tuple",INI_ORIG_STR("polyline.tuple"));
    php_info_print_table_row(2,"polyline.precision",INI_STR("polyline.precision"));
//    DISPLAY_INI_ENTRIES();
    php_info_print_table_end();
}

PHP_FUNCTION(polyline_encode)
{
    zval *zpoint, **data, **point;
    HashTable *zpoint_hash, *point_hash;
    HashPosition pointer;
    smart_str encoded = {0};
    int precision = _polyline_get_ini_precision(); 
    int tuple = _polyline_get_ini_tuple();
    int tuple_index = 0;
    int * previous = ecalloc(sizeof(int), tuple);
	int chunk;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &zpoint) == FAILURE) {
       return;
    }
    zpoint_hash = Z_ARRVAL_P(zpoint);
    for(
        zend_hash_internal_pointer_reset_ex(zpoint_hash,&pointer);
        zend_hash_get_current_data_ex(zpoint_hash,(void **)&data, &pointer) == SUCCESS;
        zend_hash_move_forward_ex(zpoint_hash,&pointer)
    ){
        if(Z_TYPE_PP(data) == IS_ARRAY) {
            point_hash = Z_ARRVAL_PP(data);
            // Iterate over nexted point
            
            if(zend_hash_index_exists(point_hash,tuple-1))
            {
                while(tuple_index < tuple)
                {
                
                   zend_hash_index_find(point_hash,tuple_index,(void **)&point);
                   chunk = (int)(Z_DVAL_PP(point) * pow(10,precision));
                   _polyline_encode_chunk(chunk - previous[tuple_index],&encoded);
                   previous[tuple_index] = chunk;
                   tuple_index++; 
               }
               tuple_index = 0;
            } else {
                php_error_docref(NULL TSRMLS_CC, E_WARNING, "Size of item does not match \"polyline.tuple\" configuration.");
				efree(previous);
				RETURN_FALSE;
            }
        } else {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "Element item not array.");
			efree(previous);
			RETURN_FALSE;
        }
    }
    efree(previous);
    smart_str_0(&encoded);
    ZVAL_STRINGL(return_value,encoded.c,encoded.len,1);
    smart_str_free(&encoded);
}

PHP_FUNCTION(polyline_decode)
{
    char *encoded;
    int len,index = 0;
    zval *zpoint = NULL;
    int precision = _polyline_get_ini_precision(); 
    int tuple = _polyline_get_ini_tuple(); 
    int tuple_index = 0;
    int * previous = ecalloc(sizeof(int), tuple);
	int number;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &encoded, &len) == FAILURE) {
        return;
    }
    if ( len > 0 ) {
        if(_polyline_validate_encoded_string(encoded,len,tuple) > 0) {
            php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Invalid encoded string.");
            RETURN_FALSE;
        }
    }
//    ALLOC_INIT_ZVAL(zpoints);
    array_init(return_value);
    while( index < len )
    {
        number = previous[tuple_index] + _polyline_decode_chunk( encoded, &index );
        previous[tuple_index] = number;
        if(!zpoint) {
            MAKE_STD_ZVAL(zpoint);
            array_init_size(zpoint,tuple);
        }
        add_next_index_double(zpoint,((double)number * 1 / pow(10,precision) ));
        tuple_index++;
        // Complte tuple, allocated array & rest
        if(tuple_index == tuple)
        {
            add_next_index_zval(return_value,zpoint);
            zpoint = NULL;
            tuple_index = 0;
        }
    }
    if(zpoint) {
	  php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Ending tuple incomplete. Finial point omitted.");
      efree(zpoint);
    }
    efree(previous);
}

PHP_FUNCTION(polyline_validate_encoded_string)
{
    char * str;
    int len;
    int tuple = _polyline_get_ini_tuple();
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &len) == FAILURE) {
        return;
    }
    
    if ( len > 0 && _polyline_validate_encoded_string(str,len,tuple) == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}

static inline void _polyline_encode_chunk( long delta, smart_str * buffer )
{
    long number = (delta < 0) ? ~(delta << 1) : (delta << 1);
    while ( number >= 0x20 )
    {
        smart_str_appendc(buffer,(char)(0x20 | (number & 0x1f)) + 0x3f);
        number >>= 0x05;
    }
    smart_str_appendc(buffer,(char)number + 0x3f);
}

static inline long _polyline_decode_chunk( char * buffer, int * buffer_length )
{
    long chunk, shift, result;
    char c;
    shift = result = 0;
    do
    {
        c = buffer[(*buffer_length)++];
        chunk = (long)c - 0x3f;
        result |= ( chunk & 0x1f ) << shift;
        shift  += 0x05;
    } while ( chunk >= 0x20 );
    return ( result & 1 ) ? ~( result >> 1 ) : ( result >> 1 );
}

static inline int _polyline_validate_encoded_string( char * ptr, int len, int tuple )
{
    int byte = ptr[--len] -  0x3f;
    int valid = byte < 0x20 ? 1 : 0;
    if( valid == 0 ) { return 1; }
    while (len > 0)
    {
        byte = ptr[--len] -  0x3f;
        if( byte < 0 ) { return 1; }
        if( byte < 0x20 )
            ++valid;
    };
    return valid % tuple;
}

int _polyline_get_ini_tuple()
{
    int tuple = INI_INT("polyline.tuple");
    if( tuple < 1 ) {
       php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid polyline.tuple setting.");
       tuple = 2;
    }
    return tuple;
}

int _polyline_get_ini_precision()
{
    int precision = INI_INT("polyline.precision");
    if( precision < 0 || precision > 6) {
       php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid polyline.precision setting.");
       precision = 5;
    }
    return precision;
}
