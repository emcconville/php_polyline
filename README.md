# Polyline (PECL) #
## Embeded Polyline Encoder Module ##

### Installation ###

    # Get source
    git clone http://git.emcconville.com/php_polyline.git
    # Change directory
    cd php_polyline
    # Included local configuration
    phpize
    # Auto-configure
    ./configure
    # Compile
    make
    # Verify
    make test
    # Copy shared object to extension directory (or use `sudo make install`)
    sudo cp modules/polyline.so `php-config --extension-dir`
    # Enable extension in php.ini
    MYPHPINI=`php-config --configure-options | sed -n 's/.*with-config-file-path=\([^ \]*\)\ .*/\1\/php.ini/p'`
    sudo echo "extension=polyline.so" >> $MYPHPINI

### Usage ###

Encode polyline

    $points=array(
      array(41.79999,-87.58695),
      array(41.79959,-87.58692),
      array(41.79959,-87.58741),
      array(41.79958,-87.58900),
      array(41.79956,-87.59037),
      array(41.79950,-87.59037),
      array(41.79949,-87.59206)
    );
    $encoded = polyline_encode($points);
    //=> "}`c~FlyquOnAE?`B@|HBpGJ?@pI"


Decode polyline

    $points = polyline_decode("}`c~FlyquOnAE?`B@|HBpGJ?@pI");
    //=> array(array(41.79999,-87.58695), /*...*/, array(41.79949,-87.59206))

### Hacking (ini settings) ###
#### Size of point "tuple" ####

The tuple of each point can be adjusted with the following INI setting

    ; integer (default 2)
    polyline.tuple = 2

Precision level can also be adjusted

    ; integer (default 5)
    polyline.precision = 5

**!!!Note!!!** : Most third parties expect a "point" to be defined as two doubles,
and encoded with a precision level of 1e5. Changing tuple & precision will not
guaranty that generated strings can be decoded.