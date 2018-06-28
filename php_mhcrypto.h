#ifndef PHP_MHCRYPTO_H
#define PHP_MHCRYPTO_H

extern zend_module_entry mhcrypto_module_entry;
#define phpext_mhcrypto_ptr &mhcrypto_module_entry

#define PHP_MHCRYPTO_VERSION "0.1.0"

#ifdef PHP_WIN32
#	define PHP_MHCRYPTO_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_MHCRYPTO_API __attribute__ ((visibility("default")))
#else
#	define PHP_MHCRYPTO_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(mhcrypto)
ZEND_END_MODULE_GLOBALS(mhcrypto)

/* Always refer to the globals in your function as MHCRYPTO_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define MHCRYPTO_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(mhcrypto, v) 

#if defined(ZTS) && defined(COMPILE_DL_DBSYNC)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_MHCRYPTO_H */
