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
#include "Node.hh"
#include <cstring>


Node::Node(
	Node *parent) : flags(0), response(-1), symbol(-1),
		count(0), parent(parent)
{
	memset(next, 0, sizeof(next));
}

Node::~Node()
{
	for (size_t i = 0; i < MAX_SYMBOLS; ++i)
		delete next[i];
}

/**
* @brief Build the graph
*/
void Node::append(
	const std::string &value,
	int response )
{
	if (&value == nullptr || value.empty()) return;

	for (size_t i = 0; i < value.length(); ++i)
		if (map( (uint8_t) value[i] ) < 0) return;

	bool change = true;
	appendChar(value, response, value.length() -1, change);
}


bool Node::checkMixed()
{
	int resp = -1;
	bool result = false;

	count = 0;

	// every node must be visited
	for (size_t i = 0; i < MAX_SYMBOLS; ++i)
	{
		if (next[i] == nullptr) continue;

		++count;
		result |= next[i]->checkMixed();
		if (resp < 0) resp = next[i]->response;

		// if the child node has a different response or is
		// a decision point itself, the current node became a
		// decision point too
		result |= (resp != next[i]->response);
	}

	// if the current node is a terminal with a response different than its
	// child nodes, this is a decision point
	result |= (flags & FLAG_TERMINAL && resp >= 0 && response != resp);

	// top root node is always a decision point
	result |= (parent == nullptr);

	if (result) flags |= FLAG_DECISION;
	return result;
}


void Node::plot(
	std::ostream &output)
{
	output << " graph ROOT {" << std::endl;
	//output << "ranksep=3; ratio=auto;" << std::endl;
	std::string name = "root";
	if (symbol >= 0)
		name = unmap(symbol);
	output << "N" << (size_t)this << " [label=\"" << name << "\"";
	if (flags & FLAG_DECISION)
		output << ", color=blue";
	output << "];" << std::endl;

	for (size_t i = 0; i < MAX_SYMBOLS; ++i)
	{
		if (next[i] == nullptr) continue;

		output << " subgraph PATH_" << i << " {" << std::endl;
		next[i]->plot(output, this);
		output << "}" << std::endl;
	}

	output << "}" << std::endl;
}


void Node::plot(
	std::ostream &output,
	const Node *parent )
{
	for (size_t i = 0; i < MAX_SYMBOLS; ++i)
	{
		if (next[i] == nullptr) continue;

		next[i]->plot(output, this);
	}

	// print the current child information
	output << "N" << (size_t)this << " [label=\"" << (char)unmap(symbol);
	//if (flags & FLAG_TERMINAL)
	if (response >= 0)
		output << " (" << response << ")";
	if (flags & FLAG_TERMINAL)
		output << "\",style=filled";
	else
		output << "\"";
	if (flags & FLAG_DECISION)
		output << ", color=blue";
	output << "];" << std::endl;

	// print the edge
	if (parent != nullptr)
	{
		output << "N" << (size_t)this << " -- " << "N" << (size_t)parent;
		if (flags & FLAG_DECISION)
			output << " [color=blue]";
		output << ";" << std::endl;
	}
}

int Node::map(
	char input )
{
	int value = (uint8_t) input;
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

char Node::unmap(
	int value )
{
	if ( value >= 0  && value <= 25 )
		return (char) (0x41 + value);
	if ( value >= 26 && value <= 58 )
		return (char)((uint8_t)(0xC0 + value - 26));

	return 0;
}

Node *Node::operator[](
	int index )
{
	if (index < 0 || index >= MAX_SYMBOLS) return nullptr;
	return next[index];
}


Node *Node::operator[](
	size_t index )
{
	return (*this)[ (int)index ];
}


const Node *Node::operator[](
	int index ) const
{
	if (index < 0 || index >= MAX_SYMBOLS) return nullptr;
	return next[index];
}


const Node *Node::operator[](
	size_t index ) const
{
	return (*this)[ (int)index ];
}


void Node::appendChar(
	const std::string &value,
	int resp,
	size_t position,
	bool &overwrite )
{
	int index = map( (uint8_t) value[position]);

	if (next[index] == nullptr)
	{
		next[index] = new Node(this);
		next[index]->symbol = index;
		next[index]->response = resp;
		overwrite = false;
	}

	if (position > 0)
	{
		next[index]->appendChar(value, resp, position - 1, overwrite);
	}
	else
	{
		next[index]->flags |= FLAG_TERMINAL;
		next[index]->response = resp;
	}

	if (overwrite) response = resp;
}