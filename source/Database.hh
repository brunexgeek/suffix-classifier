/*
 *  Copyright 2017 Bruno Ribeiro
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef BRUNEXGEEK_DATABASE_HH
#define BRUNEXGEEK_DATABASE_HH


#include <vector>
#include <string>


#define IS_VALID_WORD(x) \
( ( (uint8_t)(x) >= 0x41 && (uint8_t)(x) <= 0x5A ) || \
  ( (uint8_t)(x) >= 0x61 && (uint8_t)(x) <= 0x7A ) || \
  ( (uint8_t)(x) >= 0xC0 && (uint8_t)(x) <= 0xC4 ) || \
  ( (uint8_t)(x) >= 0xC7 && (uint8_t)(x) <= 0xCF ) || \
  ( (uint8_t)(x) >= 0xD2 && (uint8_t)(x) <= 0xD6 ) || \
  ( (uint8_t)(x) >= 0xD9 && (uint8_t)(x) <= 0xDD ) || \
  ( (uint8_t)(x) >= 0xE0 && (uint8_t)(x) <= 0xE4 ) || \
  ( (uint8_t)(x) >= 0xE7 && (uint8_t)(x) <= 0xEF ) || \
  ( (uint8_t)(x) >= 0xF2 && (uint8_t)(x) <= 0xF6 ) || \
  ( (uint8_t)(x) >= 0xF9 && (uint8_t)(x) <= 0xFD ) || \
  ( (uint8_t)(x) == 0xFF ) )


struct DatabaseEntry
{
	std::string word;
	int response;
};


class Database : public std::vector<DatabaseEntry>
{
	public:
		void load(
			const std::string &fileName );
};

#endif