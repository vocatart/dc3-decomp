#pragma once
#include "rndobj/Mesh.h"
#include "math/Vec.h"
#include "utl/BinStream.h"

struct CompressedVertex_Xbox {
    int unk0;
    int unk4;
    int unk8;
    int unkc; // 0xc - packed color
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
};

void PackVector(
    unsigned int &,
    const Vector4 &,
    unsigned char,
    unsigned char,
    unsigned char,
    unsigned char,
    bool
);
void FillCompressedVertex(CompressedVertex_Xbox &, const RndMesh::Vert &, bool);
void SaveCompressedVertex(const CompressedVertex_Xbox &, BinStream &);
