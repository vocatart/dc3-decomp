#include "utl/FilePath.h"
#include "os/File.h"

FilePath FilePath::sRoot;
FilePath FilePath::sNull("");

__declspec(noinline) FilePath::FilePath(const char *str) { Set(sRoot.c_str(), str); }
__declspec(noinline) FilePath::FilePath(const char *str1, const char *str2) {
    Set(str1, str2);
}

void FilePath::Set(const char *str1, const char *str2) {
    char buf[256];
    const char *path;
    if (str2 && *str2) {
        path = FileMakePathBuf(str1, str2, buf);
    } else
        path = "";

    this->String::operator=(path); // well ok then
    // *this = path;
}
