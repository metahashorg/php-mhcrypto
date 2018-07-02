# php-mhcrypto

This repository contains Metahash wallet and signature PHP API sample code.
C++ code is used from repository https://github.com/metahashorg/cpplib_open_ssl_decor

## Get the source code
Clone the repository by:
```shell
git clone https://github.com/metahashorg/php-mhcrypto
```

## Build extension
Instructions to get PHP7 extension binary:
```shell
phpize
./configure --enable-mhcrypto
make
make install
```

## Test application
This PHP7 test script shows the use of extension.
Enable `dl()` function in your `php.ini` with `enable_dl` option.
```shell
php -f mhcrypto.php
```
