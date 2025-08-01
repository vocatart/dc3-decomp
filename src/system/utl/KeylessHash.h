#pragma once

/**
 * @brief A keyless hash table.
 *
 * @tparam T1 const char* type (?)
 * @tparam T2 the type to store into the hash table.
 */
template <class T1, class T2>
class KeylessHash {
private:
    /** The collection of entries in the hash table. */
    T2 *mEntries; // 0x0
    /** The hash table's size. */
    int mSize; // 0x4
    /** Whether or not this table owns its entries. */
    bool mOwnEntries; // 0x8
    /** The number of currently allocated entries in the table. */
    int mNumEntries; // 0xc
    /** The type T2 representing an empty hash table entry. */
    T2 mEmpty; // 0x10
    /** The type T2 representing a removed hash table entry. */
    T2 mRemoved;

public:
    // NEW_OVERLOAD;
    // DELETE_OVERLOAD;

    KeylessHash(int, const T2 &, const T2 &, T2 *);
    ~KeylessHash();

    /** Get the entry in the hash table corresponding with the supplied key.
     * @param [in] key The key to search with.
     * @returns The corresponding entry, if it exists.
     */
    T2 *Find(const char *const &key);

    /** Insert this value into the hash table.
     * @param [in] val The val to insert.
     * @returns The entry in the hash table containing the value.
     */
    T2 *Insert(const T2 &val);

    /** Resize the hash table.
     * @param [in] size The new desired size of the table.
     * @param [in] entries Where the newly resized hash table entries will be written. If
     * this is NOT null, then this table does not own its entries.
     */
    void Resize(int size, T2 *entries);

    /** Get the first valid entry in the table from the supplied entry.
     * @param [in] entry The indexed entry in the table to begin searching from.
     * @returns The first valid entry from the supplied entry, if it exists.
     */
    T2 *FirstFrom(T2 *entry);

    // getters
    int Size() const { return mSize; }
    int UsedSize() const { return mNumEntries; }

    /** Advance the index to search the supplied hash table with.
     * The hash table implementation is a circular buffer,
     * so if the end is reached, loop back to index zero.
     */
    void Advance(int &idx) {
        idx++;
        if (idx == mSize)
            idx = 0;
    }

    // keep these in here so that they're inlined - needed for ObjDirItr

    /** Get the very first valid entry in the table. */
    T2 *Begin() { return FirstFrom(mEntries); }
    /** Get the next valid entry in the table from the supplied entry. */
    T2 *Next(T2 *entry) { return FirstFrom(&entry[1]); }
};
