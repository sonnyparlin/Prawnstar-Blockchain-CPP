#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <memory>
#include <iostream>

template<typename T, typename D>
std::unique_ptr<T, D> make_handle(T* handle, D deleter)
{
    return std::unique_ptr<T, D>{handle, deleter};
}

void print_openssl_error(std::string const& function)
{
    char buffer[1024];
    ERR_error_string_n(ERR_get_error(), buffer, sizeof(buffer));
    std::cerr << "openssl function " << function << " failed with " << buffer << "\n";
}

bool create_ec_private_key()
{
    // Create the context for the key generation
    auto kctx = make_handle(EVP_PKEY_CTX_new_id(EVP_PKEY_EC, nullptr), EVP_PKEY_CTX_free);
    if(!kctx)
    {
        print_openssl_error("EVP_PKEY_CTX_new");
        return false;
    }

    // Generate the key
    if(1 != EVP_PKEY_keygen_init(kctx.get()))
    {
        print_openssl_error("EVP_PKEY_keygen_init");
        return false;
    }

    //  We're going to use the NID_secp256k1 curve
    if(1 != EVP_PKEY_CTX_set_ec_paramgen_curve_nid(kctx.get(), NID_secp256k1)) 
    {
        print_openssl_error("EVP_PKEY_CTX_set_ec_paramgen_curve_nid");
        return false;
    }

    EVP_PKEY *pkey_temp = nullptr;
    if (1 != EVP_PKEY_keygen(kctx.get(), &pkey_temp))
    {
        print_openssl_error("EVP_PKEY_keygen");
        return false;
    }

    // write out to pem file
    auto pkey = make_handle(pkey_temp, EVP_PKEY_free);

    auto file = make_handle(BIO_new_file("ecprivatekey.pem", "w"), BIO_free);
    if(!file)
    {
        print_openssl_error("BIO_new_file");
        return false;
    }

    if(!PEM_write_bio_PrivateKey(file.get(), pkey.get(), nullptr, nullptr, 0, nullptr, nullptr))
    {
        print_openssl_error("PEM_write_bio_PrivateKey");
        return false;
    }

    return true;
}

int main() {
    create_ec_private_key();
    return 0;
}