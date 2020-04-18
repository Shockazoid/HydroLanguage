//
//                 __  __            __           
//                / / / /__  __ ____/ /_____ ____ 
//               / /_/ // / / // __  // ___// __ \
//              / __  // /_/ // /_/ // /   / /_/ /
//             /_/ /_/ \__, / \__,_//_/    \____/ 
//                    /____/                      
//
//              The Hydro Programming Language
//
//        © 2020 Shockazoid, Inc. All Rights Reserved.
//

#ifndef __h3o_bytearray__
#define __h3o_bytearray__

#include <cstdlib>

namespace hydro
{

class bytearray
{
protected:
    uint8_t *m_buffer;
    uint32_t m_size, m_capacity, m_pos;

    void reserve(uint32_t newCapacity);
    void resize(uint32_t newSize);

public:
    bytearray();                       // default constructor
    bytearray(const bytearray &bytes); // copy constructor
    bytearray(bytearray &&bytes);      // move constructor
    ~bytearray();
    void put(uint8_t value);
    void put(uint8_t value, uint32_t &pos);
    void putInt(int32_t value);
    void putInt(int32_t value, uint32_t &pos);
    uint8_t get();
    uint8_t get(uint32_t &pos);
    int32_t getInt();
    int32_t getInt(uint32_t &pos);
    void seek(uint32_t pos);
    void reset();
    void clear();
    void freeUnusedMemory();
    uint32_t size() const { return m_size; }
    uint32_t bytesAvailable() const { return m_pos > m_size ? 0 : m_size - m_pos; }
    uint32_t bytesAvailable(uint32_t pos) const { return pos > m_size ? 0 : m_size - pos; }
    uint32_t capacity() const { return m_capacity; }
    uint32_t position() const { return m_pos; }
    bool operator==(const bytearray &rhs) const;
    bool operator!=(const bytearray &rhs) const;
    bytearray &operator=(const bytearray &rhs); // copy assignment
    bytearray &operator=(bytearray &&rhs);      // move assignment
};

} // namespace hydro

#endif /* __h3o_bytearray__ */
