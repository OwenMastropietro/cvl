#pragma once

#include <filesystem>
#include <random>
#include <stdexcept>
#include <string>
#include <system_error>

namespace fs = std::filesystem;

class TempDir {

  public:
    fs::path path;

    explicit TempDir(const std::string &prefix = "cvl-test") {
        static std::mt19937_64 rng(std::random_device{}());

        const fs::path root = fs::temp_directory_path();

        for (int i = 0; i < 100; ++i) {
            path = root / (prefix + "-" + std::to_string(rng()));

            std::error_code ec;
            if (fs::create_directory(path, ec)) {
                return;
            }

            if (ec && ec != std::errc::file_exists) {
                throw fs::filesystem_error(
                    "Failed to create temporary directory", path, ec);
            }
        }

        throw std::runtime_error(
            "Failed to create a unique temporary directory");
    }

    ~TempDir() {
        std::error_code ec;
        fs::remove_all(path, ec);
    }

    TempDir(const TempDir &) = delete;
    TempDir &operator=(const TempDir &) = delete;

    fs::path file(const std::string &name) const { return path / name; }
};
