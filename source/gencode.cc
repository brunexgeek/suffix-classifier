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
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include "Database.hh"
#include "Node.hh"
#include "Decision.hh"


using namespace std;


static void indent(
	std::ostream &out,
	int depth )
{
	++depth;
	while (depth > 0)
	{
		out << "   ";
		--depth;
	}
}


static void generateDecisionCode(
	std::ostream &out,
	Decision &root,
	int depth,
	int &maxIndex )
{
	if (maxIndex < root.index) maxIndex = root.index;

	if (root.symbol >= 0)
	{
		indent(out, depth);
		out << "if ( features[" << std::dec << root.index << "] == " << std::hex << "0x" << root.symbol;
		out << " /* " << Node::unmap(root.symbol) << " */ )" << std::endl;
		indent(out, depth);
		out << "{" << std::endl;

		generateDecisionCode(out, *root.positive, depth + 1, maxIndex);

		indent(out, depth);
		out << "}" << std::endl;

		if (root.negative != nullptr)
		{
			indent(out, depth);
			out << "else" << std::endl;
			indent(out, depth);
			out << "{" << std::endl;

			generateDecisionCode(out, *root.negative, depth + 1, maxIndex);

			indent(out, depth);
			out << "}" << std::endl;
		}
	}
	else
	{
		indent(out, depth);
		out << "return " << root.response << ";" << std::endl;
	}
}


static std::string replaceString(
	const std::string &source,
	const std::string& from,
	const std::string& to)
{
	std::string output = source;

	size_t pos = 0;
	while ((pos = output.find(from, pos)) != std::string::npos)
	{
		output.replace(pos, from.length(), to);
		pos += from.length();
	}
	return output;
}


static void generateCode(
	std::ostream &out,
	Decision &root )
{
	out << "static int classify( const int *features )" << std::endl;
	out << "{" << std::endl;

	int size = 0;
	generateDecisionCode(out, root, 0, size);
	++size;

	out << "}" << std::endl << std::endl;

	// function to map text to features
	out << R"BLA(
static int mapCharacter( char c )
{
	int value = (unsigned char) c;
	if ( value >= 0x41 && value <= 0x5A )
		return value - 0x41;
	if ( value >= 0x61 && value <= 0x7A )
		return value - 0x61;
	if ( value >= 0xC0 && value <= 0xDF )
		return value - 0xC0 + 26;
	if ( value >= 0xE0 && value <= 0xFF )
		return value - 0xE0 + 26;
	return -1;
}

)BLA";


std::string publicFunc = R"BLA(
int classifyWord( const char *word, int len )
{
	int features[@SIZE@];
	int i = 0;
	const char *ptr;
	if (word == 0) return -1;
	ptr = word + (len - 1);
	for (; i < @SIZE@; ++i, --ptr)
	{
		if (i < len)
			features[i] = mapCharacter(*ptr);
		else
			features[i] = -1;
	}
	return classify(features);
}
)BLA";

	char temp[8];
	snprintf(temp, sizeof(temp) - 1, "%d", size);
	publicFunc = replaceString(publicFunc, "@SIZE@", temp);
	out << publicFunc;
}


int main( int argc, char **argv )
{
	if (argc != 3)
	{
		std::cerr << "Usage: gencode <tree file> <output file>" << std::endl;
		return 1;
	}

	Decision *decision = nullptr;

	// load the decision tree
	std::ifstream input(argv[1], std::ios_base::binary);
	if (input.good())
	{
		decision = Decision::deserialize(input);
		input.close();
	}

	if (decision == nullptr)
	{
		std::cerr << "Unable to load decision tree from '" << std::string(argv[1]) << "'" << std::endl;
		return 1;
	}

	// serialize the decision tree
	std::ofstream out(argv[2], std::ios_base::ate);
	if (out.good())
	{
		generateCode(out, *decision);
		out.close();
	}
	else
	{
		std::cerr << "Unable to save the C source code to '" << std::string(argv[2]) << "'" << std::endl;
		return 1;
	}

	return 0;
}

