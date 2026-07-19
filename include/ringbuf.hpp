#pragma once
#include <vector>
#include <stdexcept>
#include <cstddef>

class RingBuffer {
public:
    RingBuffer() = default;
    RingBuffer(size_t size);
    RingBuffer(const RingBuffer& obj);
    RingBuffer(RingBuffer&& obj);

    RingBuffer& operator=(const RingBuffer& obj);
    RingBuffer& operator=(RingBuffer&& obj);

    void copy(const RingBuffer& obj);
    void swap(RingBuffer& obj);

    size_t capacity() const;
    size_t usage() const;
    size_t available() const;

    bool full() const;
    bool empty() const;

    void resize(size_t size);

    size_t read(std::byte* dest, size_t size);
    std::vector<std::byte> read(size_t size);

    size_t write(const std::byte* src, size_t size);
    size_t write(const std::vector<std::byte>& src);

    void clear();

private:
    std::vector<std::byte> buffer;

    size_t readIndex;
    size_t writeIndex;

    size_t bufUsage;
};