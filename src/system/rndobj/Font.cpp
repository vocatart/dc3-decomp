#include "rndobj/Font.h"
#include "rndobj/FontBase.h"
#include "obj/Object.h"
#include "utl/BinStream.h"
#include "math/Rot.h"
#include "utl/UTF8.h"

KerningTable::KerningTable() : mNumEntries(0), mEntries(0) { memset(mTable, 0, 0x80); }
KerningTable::~KerningTable() { delete mEntries; }

KerningTable::Entry *KerningTable::Find(unsigned short us1, unsigned short us2) {
    if (mNumEntries == 0)
        return nullptr;
    else {
        Entry *entry = mTable[TableIndex(us1, us2)];
        int key = Key(us1, us2);
        for (; entry != nullptr && key != entry->key; entry = entry->next)
            ;
        return entry;
    }
}

float KerningTable::Kerning(unsigned short us1, unsigned short us2) {
    Entry *kerningEntry = Find(us1, us2);
    if (kerningEntry)
        return kerningEntry->kerning;
    else
        return 0;
}

bool KerningTable::Valid(const RndFont::KernInfo &info, RndFontBase *font) {
    return !font || (font->CharDefined(info.unk0) && font->CharDefined(info.unk2));
}

void KerningTable::Save(BinStream &bs) {
    bs << mNumEntries;
    for (int i = 0; i < mNumEntries; i++) {
        bs << mEntries[i].key;
        bs << mEntries[i].kerning;
    }
}

void KerningTable::SetKerning(
    const std::vector<RndFont::KernInfo> &info, RndFontBase *font
) {
    int validcount = 0;
    for (int i = 0; i < info.size(); i++) {
        if (Valid(info[i], font)) {
            validcount++;
        }
    }
    if (validcount != mNumEntries) {
        mNumEntries = validcount;
        delete[] mEntries;
        mEntries = new Entry[mNumEntries];
    }
    memset(mTable, 0, 0x80);
    for (int i = 0; i < info.size(); i++) {
        const RndFont::KernInfo &curInfo = info[i];
        if (Valid(curInfo, font)) {
            Entry &curEntry = mEntries[i];
            curEntry.key = Key(curInfo.unk0, curInfo.unk2);
            curEntry.kerning = curInfo.kerning;
            int index = TableIndex(curInfo.unk0, curInfo.unk2);
            curEntry.next = mTable[index];
            mTable[index] = &curEntry;
        }
    }
}

RndMat *RndFont::Mat(int idx) const {
    if (idx >= 0 && idx < mMats.size()) {
        return (RndMat *)mMats[idx];
    } else
        return nullptr;
}

RndTex *RndFont::ValidTexture(int idx) const {
    if (Mat(idx)) {
        return Mat(idx)->GetDiffuseTex();
    } else
        return nullptr;
}

void RndFont::Save(BinStream &bs) {
    bs << packRevs(2, 0x11);
    SAVE_SUPERCLASS(RndFontBase)
    bs << mMats;
    bs << mCellSize << mDeprecatedSize;
    bs << mTextureOwner;
    bs << unka4;
    RndTex *validTex = ValidTexture(0);
    if (validTex) {
        bs << validTex->Width() << validTex->Height();
    } else {
        bs << 0 << 0;
    }
    bs << unk98;
    bs << mCharInfoMap.size();
    for (std::map<unsigned short, CharInfo>::iterator it = mCharInfoMap.begin();
         it != mCharInfoMap.end();
         ++it) {
        bs << it->first;
        CharInfo &info = it->second;
        bs << info.unk0;
        bs << info.charWidth;
        bs << info.unkc;
        bs << info.unk10;
        bs << info.unk14;
    }
}

void RndFont::Print() const {
    TheDebug << "   pages: " << mMats.size() << "\n";
    int numMats = mMats.size();
    for (int i = 0; i < numMats; i++) {
        TheDebug << "         " << mMats[i] << "\n";
    }
    TheDebug << "   cellSize: " << mCellSize << "\n";
    TheDebug << "   deprecated size: " << mDeprecatedSize << "\n";
    TheDebug << "   space: " << mBaseKerning << "\n";
    TheDebug << "   chars: ";
    for (int i = 0; i < mChars.size(); i++) {
        unsigned short us = mChars[i];
        TheDebug << WideCharToChar(&us);
    }
    TheDebug << "\n";
    TheDebug << "   kerning: TODO\n";
}

RndFont::RndFont()
    : mMats(this, (EraseMode)0, kObjListNoNull), mTextureOwner(this, this),
      mCellSize(1, 1), mDeprecatedSize(0), unka4(0) {}
