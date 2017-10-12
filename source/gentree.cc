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

/*
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
*/

static bool textToInteger(
	const std::string &text,
	int &value )
{
	try
	{
		value = stoi(text);
		return true;
	} catch (...)
	{
		return false;
	}
}


int main( int argc, char **argv )
{
	if (argc != 3 && argc != 4)
	{
		std::cerr << "Usage: gentree <corpus> <output prefix>" << std::endl;
		std::cerr << "       gentree <corpus> <default class> <output prefix>" << std::endl;
		return 1;
	}

	// loads words from input file
	Database words;
	words.load(argv[1]);
	if (words.empty())
	{
		std::cerr << "Can not load words from '" << argv[1] << "'" << std::endl;
		return 1;
	}
	std::cerr << "Loaded " << words.size() << " words" << std::endl << std::endl;

	// creates the graph parsing each word
	Node root(nullptr);
	std::vector<DatabaseEntry>::iterator first = words.begin();
	std::vector<DatabaseEntry>::iterator last = words.end();
	for (; first != last; ++first)
	{
		root.append(first->word, first->response);
	}

	root.checkMixed();

#if 0
	// use this with larger word trees

	for (size_t i = 0; i < Node::MAX_SYMBOLS; ++i)
	{
		if (root[i] == nullptr) continue;

		std::cerr << "Exporting '" << Node::unmap(i) << "'" << std::endl;
		std::string fileName = "graph_";
		fileName += Node::unmap(i);
		fileName += ".dot";
		std::ofstream out(fileName.c_str());
		if (out.good())
		{
			root[i]->print(out);
			out.close();
		}
	}
#elif 0
	// use this with smaller word trees

	std::ofstream out1("words.dot", std::ios_base::ate);
	if (out1.good())
	{
		root.plot(out1);
		out1.close();
	}
#endif

	std::ofstream out;

	// check whether the user specified a default response
	int defaultValue = 0;
	std::string outputPreffix = argv[2];
	if (textToInteger(argv[2], defaultValue))
	{
		if (defaultValue < 0) defaultValue *= -1;
		outputPreffix = argv[3];
	}
	else
	{
		defaultValue = -1;
	}
	std::cerr << "Default response is " << defaultValue << std::endl;

	// build the decision tree
	Decision *decision = Decision::build(root, defaultValue);
	if (decision == nullptr)
	{
		std::cerr << "Unable to generate decision tree" << std::endl;
		return 1;
	}

	// plot the decision tree in a dotviz compatible format
	std::string fileName = outputPreffix + ".dot";
	out.open(fileName.c_str(), std::ios_base::ate);
	if (out.good())
	{
		decision->plot(out);
		out.close();
	}
	else
	{
		std::cerr << "Unable to create the file '" << fileName << "'" << std::endl;
		return 1;
	}

	// serialize the decision tree
	fileName = outputPreffix + ".tree";
	out.open(fileName.c_str(), std::ios_base::ate | std::ios_base::binary);
	if (out.good())
	{
		decision->serialize(out);
		out.close();
	}
	else
	{
		std::cerr << "Unable to create the file '" << fileName << "'" << std::endl;
		return 1;
	}

	return 0;
}

