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

## API
```
mhcrypto_generate_wallet(string $private_key, string $public_key, string $mh_address[, string $password])
```
This procedure generates new wallet. Wallet is represented by 3 strings: private and public key and wallet address.
String arguments are passed by reference. Means that procedure fills them with new generated values.
`$password` argument is optional and not implemented in that version.

```
mhcrypto_sign_text(string $sign, string $private_key, string $text[, string $password])
```
This procedure uses private key to build a signature for given text. All 3 arguments are string variables.
`$sign` signature variables is passed by reference. However `$sign` variable have a type of string the content of variable
is not a string rather it's binary data can be used in conjunction with `mhcrypto_check_sign_text` function.
`$password` argument is optional and not implemented in that version.

```
bool mhcrypto_check_sign_text(string $sign, string $public_key, string $text)
```
Takes 3 string arguments and returns boolean result of the test if given signature matches given text.
Public key is required for that operation.