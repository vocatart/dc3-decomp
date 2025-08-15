#pragma once
#include <vector>

struct JukeboxItem {
    JukeboxItem(int x, int y) : unk0(x), unk4(y) {}
    ~JukeboxItem() {}
    bool operator==(int x) const { return unk0 == x; }
    int unk0;
    int unk4;
};

class Jukebox {
private:
    void AddItem(int, int);

    std::vector<JukeboxItem> mJukeboxItems; // 0x0
    int unkc; // 0xc

public:
    Jukebox(int);
    void Play(int);
    int Pick(const std::vector<int> &);
};
