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
#include <iostream>
#include "Database.hh"


extern "C" int classifyWord( const char *word, int len );


int classifyWord( const std::string &value )
{
	return classifyWord(value.c_str(), (int)value.length());
}


int main( int argc, char **argv )
{
	if (argc != 2) return 1;

	// loads words from input file
	Database words;
	words.load(argv[1]);
	if (words.empty())
	{
		std::cerr << "Can not load words from '" << argv[1] << "'" << std::endl;
		return 1;
	}
	std::cerr << "Loaded " << words.size() << " words" << std::endl << std::endl;

	std::vector<DatabaseEntry>::iterator first = words.begin();
	std::vector<DatabaseEntry>::iterator last = words.end();
	for (; first != last; ++first)
	{
		std::cout << first->word << "(" << first->response << ") == " << classifyWord(first->word) << std::endl;
	}
}