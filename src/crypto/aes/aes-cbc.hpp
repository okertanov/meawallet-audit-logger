//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_CRYPTO_AES_CBC_HPP_
#define _AL_CRYPTO_AES_CBC_HPP_

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <tuple>
#include <exception>
#include <stdexcept>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/des.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

namespace al::crypto {
    class aes_cbc {
        public:
            aes_cbc() {
            }

            virtual ~aes_cbc() {
            }

            virtual const std::string encrypt(const std::string& data, const std::string key, const std::string mac) const {
                std::ignore = mac;

                ERR_load_ERR_strings();
                ERR_load_crypto_strings();
                OpenSSL_add_all_algorithms();

                std::unique_ptr<EVP_CIPHER_CTX, void(*)(EVP_CIPHER_CTX *)> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);
                
                const auto rc_init = EVP_CIPHER_CTX_init(ctx.get());
                if (rc_init <= 0) {
                    ERR_print_errors_fp(stderr);
                    throw std::runtime_error("Wrong crypto input");
                }

                const auto key_c = reinterpret_cast<const unsigned char*>(key.c_str());
                const auto iv_c = reinterpret_cast<const unsigned char*>(_iv.c_str());
                const auto rc_e_init = EVP_EncryptInit(ctx.get(), EVP_aes_128_cbc(), key_c, iv_c);

                if (rc_e_init <= 0) {
                    ERR_print_errors_fp(stderr);
                    throw std::runtime_error("Wrong crypto input");
                }

                const auto blocklen = EVP_CIPHER_CTX_block_size(ctx.get());

                const auto buflen = data.size();
                const auto outbuf_len = buflen + blocklen;
                auto outbuf = (unsigned char*)calloc(outbuf_len, sizeof(unsigned char));

                const auto data_c = reinterpret_cast<const unsigned char*>(data.c_str());
                auto outlen = outbuf_len;
                const auto rc_update = EVP_EncryptUpdate(ctx.get(), outbuf, (int*)&outlen, data_c, buflen);

                if (rc_update <= 0) {
                    ERR_print_errors_fp(stderr);
                    throw std::runtime_error("Wrong crypto input");
                }

                int finallen = outbuf_len - outlen;
                const auto rc_final = EVP_EncryptFinal_ex(ctx.get(), outbuf + outlen, &finallen);

                if (rc_final <= 0) {
                    ERR_print_errors_fp(stderr);
                    throw std::runtime_error("Wrong crypto input");
                }

                std::stringstream str_stream;
                for (unsigned long i = 0; i < outlen + finallen; ++i) {
                    const auto ch = outbuf[i];
                    str_stream << std::setfill('0') << std::setw(2) << std::hex << (0xff & (unsigned int)ch);
                }

                return str_stream.str();
            }

            virtual const std::string decrypt(const std::string& data, const std::string key, const std::string mac) const {
                std::ignore = mac;

                // No input produce no output for now.
                if (data.length() == 0) {
                    return data;
                }

                ERR_load_ERR_strings();
                ERR_load_crypto_strings();
                OpenSSL_add_all_algorithms();

                std::unique_ptr<EVP_CIPHER_CTX, void(*)(EVP_CIPHER_CTX *)> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);
                
                const auto rc_init = EVP_CIPHER_CTX_init(ctx.get());
                if (rc_init <= 0) {
                    ERR_print_errors_fp(stderr);
                    throw std::runtime_error("Wrong crypto input");
                }

                const auto key_c = reinterpret_cast<const unsigned char*>(key.c_str());
                const auto iv_c = reinterpret_cast<const unsigned char*>(_iv.c_str());
                const auto rc_d_init = EVP_DecryptInit(ctx.get(), EVP_aes_128_cbc(), key_c, iv_c);

                if (rc_d_init <= 0) {
                    ERR_print_errors_fp(stderr);
                    throw std::runtime_error("Wrong crypto input");
                }

                const auto blocklen = EVP_CIPHER_CTX_block_size(ctx.get());

                // Convert hex data to raw bytes (see encrypt)
                std::vector<unsigned char> raw_bytes;

                for (unsigned int i = 0; i < data.length(); i += 2) {
                    const auto one_byte = data.substr(i, 2);
                    char byte = (char)strtol(one_byte.c_str(), NULL, 16);
                    raw_bytes.push_back(byte);
                }
                
                unsigned char* raw_bytes_c = reinterpret_cast<unsigned char*> (raw_bytes.data());
                //

                const auto buflen = raw_bytes.size();
                const auto outbuf_len = buflen + blocklen;

                auto outbuf = (unsigned char*)calloc(outbuf_len, sizeof(unsigned char));

                auto outlen = outbuf_len;
                const auto rc_update = EVP_DecryptUpdate(ctx.get(), outbuf, (int*)&outlen, raw_bytes_c, buflen);

                if (rc_update <= 0) {
                    ERR_print_errors_fp(stderr);
                    throw std::runtime_error("Wrong crypto input");
                }

                int finallen = 0;
                const auto rc_final = EVP_DecryptFinal_ex(ctx.get(), outbuf + outlen, &finallen);

                if (rc_final <= 0) {
                    ERR_print_errors_fp(stderr);
                    throw std::runtime_error("Wrong crypto input");
                }

                const auto decrypted = std::string(reinterpret_cast<const char*>(outbuf), outlen + finallen);

                return decrypted;
            }

        private:
            const std::string _iv = "01234567890123456";
    };
}

#endif /* _AL_CRYPTO_AES_CBC_HPP_ */