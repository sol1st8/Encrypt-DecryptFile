#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#include <cryptopp/default.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

void EncryptFile(const fs::path& sourcefile, const fs::path& destfile, std::string_view password) {
    CryptoPP::FileSource source(sourcefile.c_str(), true, new CryptoPP::DefaultEncryptorWithMAC((CryptoPP::byte*)password.data(), password.size(), new CryptoPP::FileSink(destfile.c_str())));
}

void EncryptFile(const fs::path& filepath, std::string_view password) {
    auto temppath = fs::temp_directory_path() / filepath.filename();

    EncryptFile(filepath, temppath, password);

    fs::remove(filepath);
    fs::rename(temppath, filepath);
}

void DecryptFile(const fs::path& source_file, const fs::path& dest_file, std::string_view password) {
    CryptoPP::FileSource source(source_file.c_str(), true, new CryptoPP::DefaultDecryptorWithMAC((CryptoPP::byte*)password.data(), password.size(), new CryptoPP::FileSink(dest_file.c_str())));
}

void DecryptFile(const fs::path& filepath, std::string_view password) {
    auto temppath = fs::temp_directory_path() / filepath.filename();

    DecryptFile(filepath, temppath, password);

    fs::remove(filepath);
    fs::rename(temppath, filepath);
}

int main() {
    EncryptFile("../sample.txt", "../sample.txt.enc", "qwerty");
    DecryptFile("../sample.txt.enc", "../sample.txt.dec", "qwerty");

    EncryptFile("../sample.txt", "qwerty");
    DecryptFile("../sample.txt", "qwerty");
}
