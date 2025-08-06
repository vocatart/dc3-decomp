#pragma once

struct MemPointDelta {
    bool AnyGreaterThan(int) const;
    const char *ToString(int) const;

    static const char *HeaderString(const char *);
};
