//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_STORAGE_SECURE_STORAGE_HPP_
#define _AL_STORAGE_SECURE_STORAGE_HPP_

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "syslog/syslog.hpp"
#include "crypto/base-crypto.hpp"
#include "storage/base-storage.hpp"

namespace al::storage {
    class secure_storage: public al::storage::base_storage {
        public:
            secure_storage(std::shared_ptr<al::crypto::base_crypto> crypto) :
                _logger(al::syslog::syslog::create("secure_storage")),
                _crypto(crypto) {
            }

            virtual ~secure_storage() {
            }

            //
            // Gets access to a storage. If no data files - recreates them.
            // Layout is:
            //      storage/
            //          keys/
            //              audit_encryption
            //              audit_mac
            //              storage_encryption
            //              storage_mac
            //          audit_log
            //          secure_storage
            //
            virtual void initialize(const std::string& base_path = "./") {
                _full_storage_path = get_full_storage_path(base_path);
                
                _logger->debug("Initializing using base path: '" + base_path + "' and full path to the storage: '" + _full_storage_path.string() + "'");

                // Check or init fs structure

                ensure_dir_exists(_full_storage_path, true);
                
                ensure_dir_exists(_full_storage_path / std::filesystem::path("keys"), true);
                ensure_file_exists(_full_storage_path / std::filesystem::path("keys") / std::filesystem::path("audit_encryption") , true);
                ensure_file_exists(_full_storage_path / std::filesystem::path("keys") / std::filesystem::path("audit_mac") , true);
                ensure_file_exists(_full_storage_path / std::filesystem::path("keys") / std::filesystem::path("storage_encryption") , true);
                ensure_file_exists(_full_storage_path / std::filesystem::path("keys") / std::filesystem::path("storage_mac") , true);

                ensure_file_exists(_full_storage_path / std::filesystem::path("audit_log") , true);
                ensure_file_exists(_full_storage_path / std::filesystem::path("secure_storage") , true);

                // Read keys

                _audit_encryption = read_all_contents(_full_storage_path / std::filesystem::path("keys") / std::filesystem::path("audit_encryption"));
                _audit_mac= read_all_contents(_full_storage_path / std::filesystem::path("keys") / std::filesystem::path("audit_mac"));
                _storage_encryption = read_all_contents(_full_storage_path / std::filesystem::path("keys") / std::filesystem::path("storage_encryption"));
                _storage_mac = read_all_contents(_full_storage_path / std::filesystem::path("keys") / std::filesystem::path("storage_mac"));
            }

            //
            // Wipes audit_log entries and resets secure_storage to 0.
            //
            virtual void reset(void) const {
                reset_impl();
            }

            //
            // Checks the storage integrity.
            //
            virtual storage_state_t verify(void) const {
                try {
                    ensure_dir_exists(_full_storage_path, false);
                
                    ensure_dir_exists(_full_storage_path / std::filesystem::path("keys"), false);
                    ensure_file_exists(_full_storage_path / std::filesystem::path("keys") / std::filesystem::path("audit_encryption") , false);
                    ensure_file_exists(_full_storage_path / std::filesystem::path("keys") / std::filesystem::path("audit_mac") , false);
                    ensure_file_exists(_full_storage_path / std::filesystem::path("keys") / std::filesystem::path("storage_encryption") , false);
                    ensure_file_exists(_full_storage_path / std::filesystem::path("keys") / std::filesystem::path("storage_mac") , false);

                    ensure_file_exists(_full_storage_path / std::filesystem::path("audit_log") , false);
                    ensure_file_exists(_full_storage_path / std::filesystem::path("secure_storage") , false);

                    if (count_audit_log() == 0ULL) {
                        return storage_state_t::EMPTY;
                    }

                    return storage_state_t::GOOD;
                }
                catch(std::exception& e) {
                    return storage_state_t::BAD;
                }
            }

            //
            // Key getters
            //

            virtual const std::string get_audit_encryption(void) const {
                return _audit_encryption;
            }

            virtual const std::string get_audit_mac(void) const {
                return _audit_mac;
            }

            virtual const std::string get_storage_encryption(void) const {
                return _storage_encryption;
            }

            virtual const std::string get_storage_mac(void) const {
                return _storage_mac;
            }

            //
            // Blob accessors
            //

            virtual unsigned long long count_audit_log(void) const {
                return count_audit_log_impl();
            }

            virtual const std::vector<std::string> read_audit_log(void) const {
                return read_audit_log_impl();
            }

            virtual void append_audit_log(const std::string& line) const {
                append_audit_log_impl(line);
            }

            virtual secure_storage_record_t read_secure_storage_record(void) const {
                return read_secure_storage_record_impl();
            }

            virtual void write_secure_storage_record(const secure_storage_record_t record) const {
                return write_secure_storage_record_impl(record);
            }

        private:
            const std::filesystem::path get_full_storage_path(const std::string& base_path) const {
                static const auto storage_path_component = std::filesystem::path(std::string("storage"));
                const auto normalized_base_path = std::filesystem::canonical(std::filesystem::path(base_path).lexically_normal());
                const auto ausolute_base_path = std::filesystem::absolute(normalized_base_path);
                const auto full_storage_path = ausolute_base_path / storage_path_component;
                return full_storage_path;
            }

            void ensure_dir_exists(const std::filesystem::path& path, bool create) const {
                if (std::filesystem::exists(path)) {
                    return;
                }

                if (!create) {
                    throw std::runtime_error("Directory not exists: " + path.string());
                }

                const auto created = std::filesystem::create_directories(path);

                if (!created) {
                    throw std::runtime_error("Directory not exists: " + path.string());
                }
            }

            void ensure_file_exists(const std::filesystem::path& path, bool create) const {
                if (std::filesystem::exists(path)) {
                    return;
                }

                if (!create) {
                    throw std::runtime_error("File not exists: " + path.string());
                }

                {
                    std::ofstream output(path.string());
                }

                if (std::filesystem::exists(path)) {
                    return;
                }

                throw std::runtime_error("File not exists: " + path.string());
            }

            void reset_impl(void) const {
                const auto audit_log_path = _full_storage_path / std::filesystem::path("audit_log");

                if (!std::filesystem::exists(audit_log_path)) {
                    throw std::runtime_error("File not exists: " + audit_log_path.string());
                }

                // Wipe audit log file
                std::filesystem::remove(audit_log_path);

                // Then re-create it again but empty
                ensure_file_exists(_full_storage_path / std::filesystem::path("audit_log") , true);

                // Reset storage record to zero
                const auto record = secure_storage_record_t { 0ULL };
                write_secure_storage_record(record);
            }

            const std::string read_all_contents(const std::filesystem::path& path) const {
                if (!std::filesystem::exists(path)) {
                    throw std::runtime_error("File not exists: " + path.string());
                }

                std::ifstream in_stream;
                in_stream.open(path.string());

                std::stringstream str_stream;
                str_stream << in_stream.rdbuf();
                const auto contents = str_stream.str();

                return contents;
            }

            unsigned long long count_audit_log_impl(void) const {
                const auto lines = read_audit_log_impl();
                return lines.size();
            }

            const std::vector<std::string> read_audit_log_impl(void) const {
                // TODO: Fix to not to read all data at once, mmap/asio/iterators etc.
                const auto audit_log_path = _full_storage_path / std::filesystem::path("audit_log");

                if (!std::filesystem::exists(audit_log_path)) {
                    throw std::runtime_error("File not exists: " + audit_log_path.string());
                }

                const auto contents = read_all_contents(audit_log_path);

                static const auto delimiter = std::string("\n");
                std::vector<std::string> lines;
                auto start = 0U;
                auto end = contents.find(delimiter);
                while (end != std::string::npos) {
                    const auto line = contents.substr(start, end - start);
                    lines.emplace_back(line);
                    start = end + delimiter.length();
                    end = contents.find(delimiter, start);
                }

                return lines;
            }

            void append_audit_log_impl(const std::string& line) const {
                const auto audit_log_path = _full_storage_path / std::filesystem::path("audit_log");

                if (!std::filesystem::exists(audit_log_path)) {
                    throw std::runtime_error("File not exists: " + audit_log_path.string());
                }
              
                {
                    std::ofstream out_stream;
                    out_stream.open(audit_log_path.string(), std::ofstream::out | std::ofstream::app);
                    out_stream << line << std::endl;
                }
            }

            secure_storage_record_t read_secure_storage_record_impl(void) const {
                const auto secure_storage_contents = read_all_contents(_full_storage_path / std::filesystem::path("secure_storage"));
                const auto secure_storage_contents_decrypted = _crypto->decrypt(secure_storage_contents, get_storage_encryption(), get_storage_mac());
                const auto records_count = std::stoull(secure_storage_contents_decrypted);
                const auto record = secure_storage_record_t { records_count };
                return record;
            }

            virtual void write_secure_storage_record_impl(const secure_storage_record_t record) const {
                const auto secure_storage_path = _full_storage_path / std::filesystem::path("secure_storage");

                if (!std::filesystem::exists(secure_storage_path)) {
                    throw std::runtime_error("File not exists: " + secure_storage_path.string());
                }

                std::stringstream str_stream;
                str_stream << record.records_count;
                const auto secure_storage_contents_encrypted = _crypto->encrypt(str_stream.str(), get_storage_encryption(), get_storage_mac());

                {
                    std::ofstream out_stream(secure_storage_path.string());
                    out_stream << secure_storage_contents_encrypted;
                }
            }

        private:
            const std::shared_ptr<al::syslog::syslog> _logger;
            const std::shared_ptr<al::crypto::base_crypto> _crypto;

            std::filesystem::path _full_storage_path;

            std::string _audit_encryption;
            std::string _audit_mac;
            std::string _storage_encryption;
            std::string _storage_mac;
    };
}

#endif /* _AL_STORAGE_SECURE_STORAGE_HPP_ */