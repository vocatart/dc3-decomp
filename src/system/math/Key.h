#pragma once
#include "utl/BinStream.h"
#include "utl/TextStream.h"
#include <vector>

/**
 * @brief A keyframe.
 *
 * @tparam T The value of this keyframe.
 */
template <class T>
class Key {
public:
    Key() : value(T()), frame(0.0f) {}
    Key(const T &v, float f) : value(v), frame(f) {}
    bool operator<(const Key &k) const { return frame < k.frame; }

    T value;
    float frame;
};

template <class T>
TextStream &operator<<(TextStream &ts, const Key<T> &key) {
    ts << "(frame:" << key.frame << " value:" << key.value << ")";
    return ts;
}

template <class T>
BinStream &operator<<(BinStream &bs, const Key<T> &key) {
    bs << key.value << key.frame;
    return bs;
}

template <class T>
BinStream &operator>>(BinStream &bs, Key<T> &key) {
    bs >> key.value >> key.frame;
    return bs;
}

// Keys is a vector<Key<T>>
/**
 * @brief A specialized vector for keyframes.
 *
 * @tparam T1 The value type stored inside each keyframe.
 * @tparam T2 The interpolated value type (see AtFrame).
 */
template <class T1, class T2>
class Keys : public std::vector<Key<T1> > {
public:
    /** Get the number of keyframes in this collection. */
    int NumKeys() const { return size(); }

    /** Add a value to the keys at a given frame.
     * @param [in] val The value to add.
     * @param [in] frame The frame at which this value will be.
     * @param [in] unique If true, overwrite the existing value at this frame. Otherwise,
     * create a new keyframe.
     * @returns The index in the vector corresponding to this new keyframe.
     */
    int Add(const T1 &val, float frame, bool unique);

    /** Get the value associated with the supplied frame.
     * @param [in] frame The keyframe to get a value from.
     * @param [out] val The retrieved value.
     * @returns The index in the vector where this keyframe resides.
     */
    int AtFrame(float frame, T2 &val) const;
};

/** Scale keyframes by a supplied multiplier.
 * @param [in] keys The collection of keys to multiply the frames of.
 * @param [in] scale The multiplier value.
 */
template <class T1, class T2>
void ScaleFrame(Keys<T1, T2> &keys, float scale) {
    for (Keys<T1, T2>::iterator it = keys.begin(); it != keys.end(); ++it) {
        (*it).frame *= scale;
    }
}

class Vector3;
namespace Hmx {
    class Quat;
}

// math functions defined in math/Key.cpp:
void SplineTangent(const Keys<Vector3, Vector3> &, int, Vector3 &);
void InterpTangent(const Vector3 &, const Vector3 &, const Vector3 &, const Vector3 &, float, Vector3 &);
void InterpVector(const Keys<Vector3, Vector3> &, const Key<Vector3> *, const Key<Vector3> *, float, bool, Vector3 &, Vector3 *);
void InterpVector(const Keys<Vector3, Vector3> &, bool, float, Vector3 &, Vector3 *);
void QuatSpline(const Keys<Hmx::Quat, Hmx::Quat> &, const Key<Hmx::Quat> *, const Key<Hmx::Quat> *, float, Hmx::Quat &);
