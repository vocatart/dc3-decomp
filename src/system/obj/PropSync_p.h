#pragma once

enum PropOp {
    kPropGet = 1,
    kPropSet = 2,
    kPropInsert = 4,
    kPropRemove = 8,
    kPropSize = 16,
    kPropHandle = 32,
    kPropUnknown0x40 = 64,
};
