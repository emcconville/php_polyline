--TEST--
Encode Hyde Park Records
--SKIPIF--
<?php if(!extension_loaded('polyline')) print 'skip'; ?>
--FILE--
<?php
$points=array(
  array(41.79999,-87.58695),
  array(41.79959,-87.58692),
  array(41.79959,-87.58741),
  array(41.79958,-87.58900),
  array(41.79956,-87.59037),
  array(41.79950,-87.59037),
  array(41.79949,-87.59206)
);
print polyline_encode($points);
?>
--EXPECT--
}`c~FlyquOnAE?`B@|HBpGJ?@pI

