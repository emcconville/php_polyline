--TEST--
Alter point precision
--SKIPIF--
<?php if(!extension_loaded('polyline')) print 'skip'; ?>
--INI--
polyline.tuple=3
polyline.precision=6
--FILE--
<?php
$string = 'AQ_seKA@_seKA@_seKA@_seKA@_seKA@_seKA@_seKA@_seKA@_seK';
foreach(polyline_decode($string) as $index => $point)
{
  printf('%d: %.6f, %.6f, %.6f'.PHP_EOL,$index,$point[0],$point[1],$point[2]);
}
?>
--EXPECT--
0: 0.000001, 0.000009, 0.200000
1: 0.000002, 0.000008, 0.400000
2: 0.000003, 0.000007, 0.600000
3: 0.000004, 0.000006, 0.800000
4: 0.000005, 0.000005, 1.000000
5: 0.000006, 0.000004, 1.200000
6: 0.000007, 0.000003, 1.400000
7: 0.000008, 0.000002, 1.600000
8: 0.000009, 0.000001, 1.800000
