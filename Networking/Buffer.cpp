#include "Buffer.h"
#include "Containers/Array.h"
#include "HAL/PlatformMemory.h"
#include "Misc/ScopeLock.h" // FScopeLock
#include <cstring>

Buffer::Buffer(size_t size)
{
    _buffer.SetNum(size); // std::vector(size) 대신
}

void Buffer::Write(const uint8* data, size_t len) {
    FScopeLock lock(&_mutex);
    size_t space = _buffer.Num() - _writePos;
    if (len > space) {
        _buffer.SetNum(_writePos + len); // resize
    }
    FMemory::Memcpy(_buffer.GetData() + _writePos, data, len);
    _writePos += len;
}

size_t Buffer::Read(char* dest, size_t len) {
    FScopeLock lock(&_mutex);
    size_t stored = _writePos - _readPos;
    size_t toRead = (len < stored) ? len : stored;
    FMemory::Memcpy(dest, _buffer.GetData() + _readPos, toRead);
    _readPos += toRead;
    if (_readPos == _writePos) {
        _readPos = _writePos = 0;
    }
    return toRead;
}

uint8* Buffer::GetBuffer()
{
    FScopeLock lock(&_mutex);
    return _buffer.GetData();
}

uint8* Buffer::GetWriteBuffer()
{
    FScopeLock lock(&_mutex);
    return _buffer.GetData() + _writePos;
}

const uint8* Buffer::GetReadBuffer(){
    FScopeLock lock(&_mutex);
    return _buffer.GetData() + _readPos;
}

size_t Buffer::GetBufferSize(){
    FScopeLock lock(&_mutex);
    return _buffer.Num();
}

size_t Buffer::GetStoredSize(){
    FScopeLock lock(&_mutex);
    return _writePos - _readPos;
}

int Buffer::GetRemainingSize(){
    FScopeLock lock(&_mutex);
    return _buffer.Num() - _writePos;
}

void Buffer::OnWrite(size_t bytes) {
    FScopeLock lock(&_mutex);
    _writePos += bytes;
    _readPos = 0;
}

void Buffer::OnRead(size_t bytes) {
    FScopeLock lock(&_mutex);
    _readPos += bytes;
    if (_readPos >= _writePos) {
        _readPos = 0;
        _writePos = 0;
    }
}

void Buffer::Append(TUniquePtr<Buffer> other)
{
    FScopeLock lock(&_mutex);

    size_t appendSize = other->GetStoredSize();
    const uint8* appendData = other->GetReadBuffer();

    if (_writePos + appendSize > _buffer.Num()) {
        _buffer.SetNum(_writePos + appendSize);
    }

    FMemory::Memcpy(_buffer.GetData() + _writePos, appendData, appendSize);
    _writePos += appendSize;
}

void Buffer::PreserveFrom(size_t offset)
{
    FScopeLock lock(&_mutex);

    size_t remaining = _writePos - offset;
    FMemory::Memmove(_buffer.GetData(), _buffer.GetData() + offset, remaining);
    _readPos = 0;
    _writePos = remaining;
}

void Buffer::Reset() {
    FScopeLock lock(&_mutex);
    _readPos = _writePos = 0;
}

void Buffer::Resize(size_t size)
{
    FScopeLock lock(&_mutex);
    _buffer.SetNum(size);
    Reset();
}