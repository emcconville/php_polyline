--TEST--
Check if module was loaded
--SKIPIF--
<?php if(!extension_loaded('polyline')) print 'skip'; ?>
--FILE--
<?php
echo "ok";
?>
--EXPECT--
ok

