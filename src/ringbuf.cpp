#include <vector>
#include <cstddef>
#include <ringbuf.hpp>

RingBuffer::RingBuffer(size_t size) : readIndex(0), writeIndex(0), bufUsage(0) {
    resize(size);
}

RingBuffer::RingBuffer(const RingBuffer& obj) {
    copy(obj);
}

RingBuffer::RingBuffer(RingBuffer&& obj) {
    swap(obj);
}

RingBuffer& RingBuffer::operator=(const RingBuffer& obj) {
    copy(obj);

    return *this;
}

RingBuffer& RingBuffer::operator=(RingBuffer&& obj) {
    swap(obj);

    return *this;
}

void RingBuffer::copy(const RingBuffer& obj) {
    buffer = obj.buffer;
    readIndex = obj.readIndex;
    writeIndex = obj.writeIndex;
    bufUsage = obj.bufUsage;
}

void RingBuffer::swap(RingBuffer& obj) {
    std::swap(buffer, obj.buffer);
    std::swap(readIndex, obj.readIndex);
    std::swap(writeIndex, obj.writeIndex);
    std::swap(bufUsage, obj.bufUsage);
}

size_t RingBuffer::capacity() const {
    return buffer.size();
}

size_t RingBuffer::usage() const {
    return bufUsage;
}

size_t RingBuffer::available() const {
    return capacity() - usage();
}

bool RingBuffer::full() const {
    return usage() == capacity();
}

bool RingBuffer::empty() const {
    return !usage();
}

void RingBuffer::resize(size_t size) {
    if (!size) throw std::runtime_error("Buffer size can't be 0!");

    clear();

    buffer.resize(size);
}

size_t RingBuffer::read(std::byte* dest, size_t size) {
    if (empty()) return 0;

    size = std::min(size, usage());

    size_t part1Size = std::min(size, capacity() - readIndex);
    size_t part2Size = size - part1Size;

    std::copy(buffer.begin() + readIndex, buffer.begin() + readIndex + part1Size, dest);
    std::copy(buffer.begin(), buffer.begin() + part2Size, dest + part1Size);

    bufUsage -= size;
    readIndex = (readIndex + size) % capacity();
    
    return size;
}

std::vector<std::byte> RingBuffer::read(size_t size) {
    std::vector<std::byte> vec(size);
    vec.resize(read(vec.data(), size));

    return vec;
}

size_t RingBuffer::write(const std::byte* src, size_t size) {
    if (full()) return 0;

    size = std::min(size, available());

    size_t part1Size = std::min(size, capacity() - writeIndex);
    size_t part2Size = size - part1Size;

    std::copy(src, src + part1Size, buffer.begin() + writeIndex);
    std::copy(src + part1Size, src + size, buffer.begin());

    bufUsage += size;
    writeIndex = (writeIndex + size) % capacity();

    return size;
}

size_t RingBuffer::write(const std::vector<std::byte>& src) {
    return write(src.data(), src.size());
}

void RingBuffer::clear() {
    buffer.clear();

    readIndex = 0;
    writeIndex = 0;
    bufUsage = 0;
}