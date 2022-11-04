#include "local_store.h"

namespace LocalStore {
const std::string kSocketName = "/tmp/plasma";

void Del(const std::string& key) {
  int has_object;
  ObjectId id(key);
  auto conn = plasma_connect(kSocketName.c_str(), NULL, 0);
  plasma_contains(conn, id, &has_object);
  if (has_object)
    plasma_delete(conn, id);
  plasma_disconnect(conn);
}

void Write(const std::string& key,
           const char* buf,
           const size_t count,
           const long offset,
           const size_t chunk_size) {
  std::cout << "Write: key = " << key << " count = " << count << " offset = " << offset << " chunk_size = " << chunk_size << std::endl;
  int has_object;
  ObjectId id(key);
  auto conn = plasma_connect(kSocketName.c_str(), NULL, 0);
  plasma_contains(conn, id, &has_object);
  uint8_t* data;
  if (has_object) {
    int64_t size;
    plasma_get(conn, id, &size, &data, NULL, NULL);
    if ((size_t) size >= (count + offset)) {
      memcpy(data + offset, buf, count);
      plasma_release(conn, id);
    } else {
      uint8_t* data_copy = new uint8_t[offset];
      long copy_length = offset > size ? size : offset;
      memcpy(data_copy, data, copy_length);
      plasma_release(conn, id);
      plasma_delete(conn, id);
      uint8_t* new_data;
      plasma_create(conn, id, count + offset, NULL, 0, &new_data);
      memcpy(new_data + offset, buf, count);
      memcpy(new_data, data_copy, copy_length);
      plasma_seal(conn, id);
      plasma_release(conn, id);
    }
  } else {
    plasma_create(conn, id, chunk_size, NULL, 0, &data);
    memcpy(data + offset, buf, count);
    plasma_seal(conn, id);
    plasma_release(conn, id);
  }
  plasma_disconnect(conn);
}

int Read(const std::string& key,
         char* buf,
         const size_t count,
         const long offset) {
  std::cout << "Read: key = " << key << " count = " << count << " offset = " << offset << std::endl;
  int has_object;
  ObjectId id(key);
  auto conn = plasma_connect(kSocketName.c_str(), NULL, 0);
  plasma_contains(conn, id, &has_object);
  uint8_t* data;
  int read_size = 0;
  if (has_object) {
    int64_t size;
    plasma_get(conn, id, &size, &data, NULL, NULL);
    if ((size_t) size >= offset + count) {
      memcpy(buf, data + offset, count);
      read_size = count;
    }
    if (size > (int64_t) offset && size < (int64_t)(offset + count)) {
      memcpy(buf, data + offset, size - offset);
      read_size = size - offset;
    }
    plasma_release(conn, id);
  }
  plasma_disconnect(conn);
  return read_size;
}

int Read(const std::string& key, char* buf) {
  int has_object;
  ObjectId id(key);
  auto conn = plasma_connect(kSocketName.c_str(), NULL, 0);
  plasma_contains(conn, id, &has_object);
  uint8_t* data;
  int read_size = 0;
  if (has_object) {
    int64_t size;
    plasma_get(conn, id, &size, &data, NULL, NULL);
    memcpy(buf, data, size);
    read_size = size;
    plasma_release(conn, id);
  }
  plasma_disconnect(conn);
  return read_size;
}

bool Truncate(const std::string& key, const long length) {
  int has_object;
  ObjectId id(key);
  auto conn = plasma_connect(kSocketName.c_str(), NULL, 0);
  plasma_contains(conn, id, &has_object);
  uint8_t* data;
  if (has_object) {
    int64_t size;
    plasma_get(conn, id, &size, &data, NULL, NULL);
    if (size != length) {
      uint8_t* data_copy = new uint8_t[length];
      long copy_length = length > size ? size : length;
      memcpy(data_copy, data, copy_length);
      plasma_release(conn, id);
      plasma_delete(conn, id);
      uint8_t* new_data;
      plasma_create(conn, id, length, NULL, 0, &new_data);
      memcpy(new_data, data_copy, length);
      plasma_seal(conn, id);
      plasma_release(conn, id);
    } else {
      plasma_release(conn, id);
    }
  } else {
    plasma_disconnect(conn);
    return false;
  }
  plasma_disconnect(conn);
  return true;
}

}  // namespace LocalStore
