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
#include "Database.hh"
#include <iostream>
#include <fstream>


void Database::load(
	const std::string &fileName )
{
	std::ifstream input(fileName.c_str());
	if (input.is_open())
	{
		std::string line;

		while (input.good())
		{
			std::getline(input, line);
			// skips empty lines
			if (line.empty()) continue;

			size_t pos = line.find_first_of(" ");
			if (pos == std::string::npos || pos + 1 >= line.length()) continue;

			std::string word = line.substr(0, pos);
			int response = atoi(line.substr(pos+1).c_str());

			// clean and validates the current word
			auto it = word.begin();
			for (; it != word.end(); ++it)
			{
				if (!IS_VALID_WORD( (uint8_t) *it))
				{
					//std::cerr << "Invalid character: " << (int)*it <<  std::endl;
					word = "";
					break;
				}
			}

			if (word.empty()) continue;

			DatabaseEntry entry;
			entry.word = word;
			entry.response = response;
			push_back(entry);
		}
		// ensures the word list is sorted
		//std::sort(words->begin(), words->end());
	}
}