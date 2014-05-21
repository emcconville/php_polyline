dnl config.mf for polyline

PHP_ARG_ENABLE(polyline, Enable Polyline encoding,
[  --enable-polyline       Enable Polyline])
if test "$PHP_POLYLINE" != "no"; then
  PHP_NEW_EXTENSION(polyline, php_polyline.c, $ext_shared)
fi
