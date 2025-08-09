#pragma once

class Trie {
public:
    int store(const char *);
    void remove(unsigned int);
    char *get(int, char *, int);
    void check_index(unsigned int);
};
