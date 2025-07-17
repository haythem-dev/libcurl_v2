#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
#include <openssl/ssl.h>
#pragma GCC diagnostic pop
#endif // __GNUC__

extern "C" {

#undef SSLv23_client_method
const SSL_METHOD *SSLv23_client_method(void)
{
	return TLS_client_method();
}

#undef SSL_library_init
int SSL_library_init(void)
{
    return OPENSSL_init_ssl(0, NULL);
}

#undef SSL_load_error_strings
void SSL_load_error_strings()
{
    OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
}

} // extern "C"
