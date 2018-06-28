dnl $Id$
dnl config.m4 for extension mhcrypto


PHP_ARG_ENABLE(mhcrypto, whether to enable mhcrypto support,
Make sure that the comment is aligned:
[  --enable-mhcrypto           Enable mhcrypto support])

if test "$PHP_MHCRYPTO" != "no"; then
  AC_DEFINE(HAVE_MHCRYPTOLIB,1,[ ])
  PHP_REQUIRE_CXX()

  PHP_ADD_LIBRARY(crypto, 1, MHCRYPTO_SHARED_LIBADD)
  dnl PHP_ADD_LIBRARY(stdc++, 1, MHCRYPTO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(mhcrypto, mhcrypto.cpp crypto.cpp, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
  PHP_SUBST(MHCRYPTO_SHARED_LIBADD)
fi
