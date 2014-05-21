--TEST--
Decode Hyde Park Records
--SKIPIF--
<?php if(!extension_loaded('polyline')) print 'skip'; ?>
--FILE--
<?php
$string = '}`c~FlyquOnAE?`B@|HBpGJ?@pI';
foreach(polyline_decode($string) as $index => $point)
{
  printf('%d: %.5f, %.5f'.PHP_EOL,$index,$point[0],$point[1]);
}
?>
--EXPECT--
0: 41.79999, -87.58695
1: 41.79959, -87.58692
2: 41.79959, -87.58741
3: 41.79958, -87.58900
4: 41.79956, -87.59037
5: 41.79950, -87.59037
6: 41.79949, -87.59206