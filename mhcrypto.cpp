/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Vitali Smolin                                                |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
  #include <openssl/evp.h>

  #include "php.h"
  #include "php_ini.h"
  #include "ext/standard/info.h"
  #include "php_mhcrypto.h"
}

#include <vector>
#include <string>

#include "crypto.h"



ZEND_DECLARE_MODULE_GLOBALS(mhcrypto)

/* True global resources - no need for thread safety here */
static int le_mhcrypto;


ZEND_BEGIN_ARG_INFO_EX(arginfo_mhcrypto_generate_wallet, 0, 0, 3)
  ZEND_ARG_INFO(1, private_key)
  ZEND_ARG_INFO(1, public_key)
  ZEND_ARG_INFO(1, mh_address)
  ZEND_ARG_INFO(0, password)
ZEND_END_ARG_INFO();

PHP_FUNCTION(mhcrypto_generate_wallet)
{
  zval *private_key = NULL;
  zval *public_key = NULL;
  zval *mh_address = NULL;
  zend_string *password = NULL;

  ZEND_PARSE_PARAMETERS_START(3, 4)
    Z_PARAM_ZVAL(private_key);
    Z_PARAM_ZVAL(public_key);
    Z_PARAM_ZVAL(mh_address);
    Z_PARAM_OPTIONAL
    Z_PARAM_STR(password);
  ZEND_PARSE_PARAMETERS_END();

  ZVAL_DEREF(private_key);
  ZVAL_DEREF(public_key);
  ZVAL_DEREF(mh_address);

  std::string str_private_key;
  std::string str_public_key;
  std::string str_mh_address;

  if(password)
    CRYPTO_generate_wallet(
      str_private_key,
      str_public_key,
      str_mh_address,
      std::string(ZSTR_VAL(password), ZSTR_LEN(password))
    );
  else
    CRYPTO_generate_wallet(
      str_private_key,
      str_public_key,
      str_mh_address
    );

  ZVAL_STRING(private_key, str_private_key.c_str());
  ZVAL_STRING(public_key, str_public_key.c_str());
  ZVAL_STRING(mh_address, str_mh_address.c_str());
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_mhcrypto_sign_text, 0, 0, 3)
  ZEND_ARG_INFO(1, sign)
  ZEND_ARG_INFO(0, private_key)
  ZEND_ARG_INFO(0, text)
  ZEND_ARG_INFO(0, password)
ZEND_END_ARG_INFO();

PHP_FUNCTION(mhcrypto_sign_text)
{
  zval *sign = NULL;
  zend_string *private_key = NULL;
  zend_string *text = NULL;
  zend_string *password = NULL;

  ZEND_PARSE_PARAMETERS_START(3, 4)
    Z_PARAM_ZVAL(sign);
    Z_PARAM_STR(private_key);
    Z_PARAM_STR(text);
    Z_PARAM_OPTIONAL
    Z_PARAM_STR(password);
  ZEND_PARSE_PARAMETERS_END();

  ZVAL_DEREF(sign);

  std::vector<unsigned char> vec_sign;

  if(password)
    CRYPTO_sign_text(
      vec_sign,
      std::string(ZSTR_VAL(private_key), ZSTR_LEN(private_key)),
      std::string(ZSTR_VAL(text), ZSTR_LEN(text)),
      std::string(ZSTR_VAL(password), ZSTR_LEN(password))
    );
  else
    CRYPTO_sign_text(
      vec_sign,
      std::string(ZSTR_VAL(private_key), ZSTR_LEN(private_key)),
      std::string(ZSTR_VAL(text), ZSTR_LEN(text))
    );

  ZVAL_STRINGL(sign, reinterpret_cast<char *>(vec_sign.data()), vec_sign.size());
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_mhcrypto_check_sign_text, 0, 0, 3)
  ZEND_ARG_INFO(0, sign)
  ZEND_ARG_INFO(0, public_key)
  ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO();

PHP_FUNCTION(mhcrypto_check_sign_text)
{
  zend_string *sign = NULL;
  zend_string *public_key = NULL;
  zend_string *text = NULL;

  ZEND_PARSE_PARAMETERS_START(3, 3)
    Z_PARAM_STR(sign);
    Z_PARAM_STR(public_key);
    Z_PARAM_STR(text);
  ZEND_PARSE_PARAMETERS_END();

  std::vector<unsigned char> vec_sign;
  vec_sign.resize(ZSTR_LEN(sign));
  memcpy(vec_sign.data(), ZSTR_VAL(sign), ZSTR_LEN(sign));

  bool rc = CRYPTO_check_sign_text(
    vec_sign,
    std::string(ZSTR_VAL(public_key), ZSTR_LEN(public_key)),
    std::string(ZSTR_VAL(text), ZSTR_LEN(text))
  );

  RETURN_BOOL(rc);
}


PHP_MINIT_FUNCTION(mhcrypto)
{
  //REGISTER_INI_ENTRIES();

  /* Load all digest and cipher algorithms */
  OpenSSL_add_all_algorithms();

  return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(mhcrypto)
{
  //UNREGISTER_INI_ENTRIES();

  /* Removes all digests and ciphers */
  EVP_cleanup();

  /* if you omit the next, a small leak may be left when you make
     use of the BIO (low level API) for e.g. base64 transformations */
  CRYPTO_cleanup_all_ex_data();

  return SUCCESS;
}

PHP_RINIT_FUNCTION(mhcrypto)
{
#if defined(COMPILE_DL_MHCRYPTO) && defined(ZTS)
  ZEND_TSRMLS_CACHE_UPDATE();
#endif
  return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(mhcrypto)
{
  return SUCCESS;
}

PHP_MINFO_FUNCTION(mhcrypto)
{
  php_info_print_table_start();
  php_info_print_table_header(2, "mhcrypto support", "enabled");
  php_info_print_table_end();

  //DISPLAY_INI_ENTRIES();
}

/*
 * Every user visible function must have an entry in mhcrypto_functions[].
 */
const zend_function_entry mhcrypto_functions[] = {
  PHP_FE(mhcrypto_generate_wallet,  arginfo_mhcrypto_generate_wallet)   /* Actual entry point for PHP. */
  PHP_FE(mhcrypto_sign_text,  arginfo_mhcrypto_sign_text)   /* Actual entry point for PHP. */
  PHP_FE(mhcrypto_check_sign_text,  arginfo_mhcrypto_check_sign_text)   /* Actual entry point for PHP. */
  PHP_FE_END  /* Must be the last line in mhcrypto_functions[] */
};


zend_module_entry mhcrypto_module_entry = {
  STANDARD_MODULE_HEADER,
  "mhcrypto",
  mhcrypto_functions,
  PHP_MINIT(mhcrypto),
  PHP_MSHUTDOWN(mhcrypto),
  PHP_RINIT(mhcrypto),
  PHP_RSHUTDOWN(mhcrypto),
  PHP_MINFO(mhcrypto),
  PHP_MHCRYPTO_VERSION,
  STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_MHCRYPTO
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(mhcrypto)
#endif
