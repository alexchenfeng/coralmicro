Filesystem APIs
===============

APIs to read and write files on the Dev Board Micro flash memory,
based on littlefs.

For example, the following code demonstrates a variety of file operations
(from `examples/file_read_write/`):

```
namespace coralmicro {
namespace {
void PrintDirectory(lfs_dir_t* dir, const char* path, int num_tabs) {
  constexpr int kMaxDepth = 3;
  if (num_tabs > kMaxDepth) {
    return;
  }

  lfs_info info;
  while (lfs_dir_read(Lfs(), dir, &info) > 0) {
    if (info.name[0] == '.') {
      continue;
    }

    for (int i = 0; i < num_tabs; ++i) {
      printf("\t");
    }

    printf("%s", info.name);

    if (info.type == LFS_TYPE_DIR) {
      char subpath[LFS_NAME_MAX];
      printf("/\r\n");
      lfs_dir_t subdir;
      snprintf(subpath, LFS_NAME_MAX, "%s/%s", path, info.name);
      CHECK(lfs_dir_open(Lfs(), &subdir, subpath) >= 0);
      PrintDirectory(&subdir, subpath, num_tabs + 1);
      CHECK(lfs_dir_close(Lfs(), &subdir) >= 0);
    } else {
      printf("\t\t%ld\r\n", info.size);
    }
  }
}

void PrintFilesystemContents() {
  lfs_dir_t root;
  CHECK(lfs_dir_open(Lfs(), &root, "/") >= 0);
  printf("Printing filesystem:\r\n");
  PrintDirectory(&root, "", 0);
  printf("Finished printing filesystem.\r\n");
  CHECK(lfs_dir_close(Lfs(), &root) >= 0);
}

bool Mkdir(const char* path) {
  int ret = lfs_mkdir(Lfs(), path);
  if (ret == LFS_ERR_EXIST) {
    printf("Error dir exists");
    return false;
  }
  return (ret == LFS_ERR_OK);
}

void Main() {
  printf("Begin filesystem example\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);
  PrintFilesystemContents();

  printf("Creating some sample directories\r\n");
  CHECK(Mkdir("/dir"));
  CHECK(LfsDirExists("/dir"));
  constexpr char kFileStr[] = "HelloWorld";
  CHECK(LfsWriteFile("/dir/file", reinterpret_cast<const uint8_t*>(kFileStr),
                     std::strlen(kFileStr)));
  PrintFilesystemContents();
  std::string readstr;
  CHECK(LfsReadFile("/dir/file", &readstr));
  CHECK(readstr.length() == std::strlen(kFileStr));
  printf("File contents: %s\r\n", readstr.c_str());
  CHECK(lfs_remove(Lfs(), "/dir/file") >= 0);
  CHECK(lfs_remove(Lfs(), "/dir") >= 0);
  PrintFilesystemContents();
  printf("End filesystem example\r\n");
}

}  // namespace
}  // namespace coralmicro

extern "C" void app_main(void* param) {
  (void)param;
  coralmicro::Main();
  vTaskSuspend(nullptr);
}
```

[[filesystem.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/filesystem.h)

*namespace* `coralmicro`
:   Functions

    lfs\_t \*`Lfs`()[Â¶](#_CPPv4N10coralmicro3LfsEv "Permalink to this definition")
    :   Returns littlefs instance to use with functions from `lfs.h`.

        Returns
        :   Pointer to littlefs instance.

    bool `LfsMakeDirs`(*const* char \**path*)[Â¶](#_CPPv4N10coralmicro11LfsMakeDirsEPKc "Permalink to this definition")
    :   Creates directory, similar to `mkdir -p <path>`.

        Parameters
        :   **path** â Directory path.

        Returns
        :   True upon success, false otherwise.

    std::string `LfsDirname`(*const* char \**path*)[Â¶](#_CPPv4N10coralmicro10LfsDirnameEPKc "Permalink to this definition")
    :   Returns the string up to, but not including, the final â/â in the path.

        Parameters
        :   **path** â Path.

        Returns
        :   The string up to, but not including, the final â/â in the path.

    ssize\_t `LfsSize`(*const* char \**path*)[Â¶](#_CPPv4N10coralmicro7LfsSizeEPKc "Permalink to this definition")
    :   Returns file size.

        Parameters
        :   **path** â File path.

        Returns
        :   File size in bytes.

    bool `LfsDirExists`(*const* char \**path*)[Â¶](#_CPPv4N10coralmicro12LfsDirExistsEPKc "Permalink to this definition")
    :   Returns whether path exists and represents a directory.

        Parameters
        :   **path** â Path to check

        Returns
        :   True if path exists and represents a directory, false otherwise.

    bool `LfsFileExists`(*const* char \**path*)[Â¶](#_CPPv4N10coralmicro13LfsFileExistsEPKc "Permalink to this definition")
    :   Returns whether path exists and represents a file.

        Parameters
        :   **path** â Path to check

        Returns
        :   true if path exists and represents a file, false otherwise.

    bool `LfsReadFile`(*const* char \**path*, std::vector<uint8\_t> \**buf*)[Â¶](#_CPPv4N10coralmicro11LfsReadFileEPKcPNSt6vectorI7uint8_tEE "Permalink to this definition")
    :   Reads content of the file and stores it in `std::vector<uint8_t>`.

        Parameters
        :   * **path** â File path to read.
            * **buf** â Instance of `std::vector<uint8_t>` to read data in.

        Returns
        :   True upon success, false otherwise.

    bool `LfsReadFile`(*const* char \**path*, std::string \**str*)[Â¶](#_CPPv4N10coralmicro11LfsReadFileEPKcPNSt6stringE "Permalink to this definition")
    :   Reads content of the file and stores it in `std::string`.

        Parameters
        :   * **path** â File path to read.
            * **str** â Instance of `std::string` to read data in.

        Returns
        :   True upon success, false otherwise.

    size\_t `LfsReadFile`(*const* char \**path*, uint8\_t \**buf*, size\_t *size*)[Â¶](#_CPPv4N10coralmicro11LfsReadFileEPKcP7uint8_t6size_t "Permalink to this definition")
    :   Reads content of the file and stores it in memory buffer.

        Parameters
        :   * **path** â File path to read.
            * **buf** â Memory buffer to read data in.
            * **size** â Memory buffer size.

        Returns
        :   Number of read bytes not exceeding `size`.

    bool `LfsWriteFile`(*const* char \**path*, *const* uint8\_t \**buf*, size\_t *size*)[Â¶](#_CPPv4N10coralmicro12LfsWriteFileEPKcPK7uint8_t6size_t "Permalink to this definition")
    :   Writes content of the memory buffer to file.

        Parameters
        :   * **path** â File path to write.
            * **buf** â Memory buffer to write data from.
            * **size** â Memory buffer size.

        Returns
        :   True upon success, false otherwise.

    bool `LfsWriteFile`(*const* char \**path*, *const* std::string &*str*)[Â¶](#_CPPv4N10coralmicro12LfsWriteFileEPKcRKNSt6stringE "Permalink to this definition")
    :   Writes content of the string to file.

        Parameters
        :   * **path** â File path to write.
            * **str** â String to write data from.

        Returns
        :   True upon success, false otherwise.