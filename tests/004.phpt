--TEST--
Empty arguments (Issue #1)
--SKIPIF--
<?php if(!extension_loaded('polyline')) print 'skip'; ?>
--FILE--
<?php
$c=0;
function YesNo($a) { global $c; printf("%04d : %s\n", ++$c, $a ? 'Yes' : 'No'); }
YesNo(@polyline_encode(array(NULL)));
YesNo(@polyline_encode(array(array(NULL))));
YesNo(@polyline_encode(array(array(NULL,NULL))));
YesNo(@polyline_decode(""));
YesNo(@polyline_decode("??"));
YesNo(@polyline_decode("?"));
?>
--EXPECT--
0001 : No
0002 : No
0003 : Yes
0004 : No
0005 : Yes
0006 : No
