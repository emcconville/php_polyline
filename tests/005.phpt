--TEST--
Encoded string validation
--SKIPIF--
<?php if(!extension_loaded('polyline')) print 'skip'; ?>
--FILE--
<?php
$c=0;
function YesNo($a) { global $c; printf("%04d : %s\n", ++$c, $a ? 'Yes' : 'No'); }
/* Yes */
YesNo(polyline_validate_encoded_string('aqa|F|ymrN{wE_|R}pGoeOgdHamNrdMhyDxtKkfI|rAbiRkl@jnRjcFlgP'));
YesNo(polyline_validate_encoded_string('kc~qFvja_ODkMiMa@]nMxHR'));
YesNo(polyline_validate_encoded_string('FOOBAR'));
YesNo(polyline_validate_encoded_string('qpxwFv_~mO'));

/* No */
// Missing terminating character
YesNo(polyline_validate_encoded_string('PglFcjRnj@lkRibAr|IfkKtxDyhMdrNmaHdgOeoGp}R|_Ew{Nrmy|F|aqa'));
// Invalid characters
YesNo(polyline_validate_encoded_string('aowFbfcnO??";i:1;s:10:"qpxwFv_~mO'));
// Unbalanced point
YesNo(polyline_validate_encoded_string('FOOZBAR'));
// Empty string
YesNo(polyline_validate_encoded_string(''));
?>
--EXPECT--
0001 : Yes
0002 : Yes
0003 : Yes
0004 : Yes
0005 : No
0006 : No
0007 : No
0008 : No
