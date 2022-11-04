#pragma once
#include <iostream>
#include <memory>
#include <string>
#include "plasma.h"
#include "plasma_client.h"

namespace LocalStore {

void Del(const std::string& id);

/**
 * @brief Write data to object. If object not exit, create it and write data to
 * it.
 * TODO: To solve call write in parallel problem.
 *
 * @param[in]
 * @param[in]
 * @return None
 */
void Write(const std::string& key,
           const char* buf,
           const size_t count,
           const long offset,
           const size_t chunk_size = 1048576);

int Read(const std::string& key,
         char* buf,
         const size_t count,
         const long offset);

/**
 * @brief Read all the data store in the object.
 *
 * @param[in] key  Object id.
 * @param[out] buf The data store in the object.
 * @return The length of the read data.
 */
int Read(const std::string& key, char* buf);

bool Truncate(const std::string& key, const long length);
}  // namespace LocalStore
