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
#include "Decision.hh"


Decision::Decision() : symbol(-1), response(-1), index(-1), parent(nullptr)
{
	positive = negative = nullptr;
}

Decision::Decision(
	Decision *parent ) : symbol(-1), response(-1), index(-1), parent(parent)
{
	positive = negative = nullptr;
}

Decision::~Decision()
{
	delete positive;
	delete negative;
}


Decision *Decision::build(
	const Node &root,
	int defaultResponse,
	int depth )
{
	if ((root.flags & Node::FLAG_DECISION) == 0) return nullptr;
	if (defaultResponse < 0) return nullptr;

	Decision *output = nullptr;
	Decision *current = nullptr;

	current = output = new Decision();

	for (size_t i = 0; i < Node::MAX_SYMBOLS; ++i)
	{
		if (root[i] == nullptr) continue;

		if ((root[i]->flags & Node::FLAG_DECISION) == 0 &&
			root[i]->response == defaultResponse) continue;

		if (root[i]->flags & Node::FLAG_DECISION)
		{
			std::cerr << "decision <if " << Node::unmap((int)i) << ">" << std::endl;
			current->symbol = (int) i;
			current->index  = depth;
			current->positive = build( *root[i], defaultResponse, depth + 1 );
			current->negative = new Decision(current);
			current = current->negative;
		}
		else
		{
			std::cerr << "return " << root[i]->response << " <if " << Node::unmap((int)i) << ">" << std::endl;
			current->symbol = (int) i;
			current->index  = depth;
			current->positive = new Decision(current);
			current->positive->response = root[i]->response;
			current->negative = new Decision(current);
			current = current->negative;
		}
	}

	if (current != nullptr && current->symbol < 0 && current->response < 0)
	{
#if 1
		current->response = defaultResponse;
#else
		if (current->parent != nullptr && current->parent->negative == current)
		{
			current->parent->negative = nullptr;
			delete current;
		}
		else
			current->response = defaultResponse;
#endif
	}

	return output;
}


void Decision::plot(
	std::ostream &output )
{
	output << " graph ROOT {" << std::endl;
	//output << "ranksep=3; ratio=auto;" << std::endl;
	std::string name = "root";
	if (symbol >= 0)
		name = Node::unmap(symbol);
	output << "N" << (size_t)this << " [label=\"" << name << "?\"];" << std::endl;

	plot(output, nullptr, true);

	output << "}" << std::endl;
}


void Decision::plot(
	std::ostream &output,
	const Decision *parent,
	bool isPositive )
{
	if (positive != nullptr)
		positive->plot(output, this, true);
	if (negative != nullptr)
		negative->plot(output, this, false);

	// print the current child information
	output << "N" << (size_t)this << " [label=\"";
	if (symbol >= 0 && index >= 0)
		output << "[" << index << "] == " << (char)Node::unmap(symbol) << "?\"";
	else
	if (response >= 0)
		output << response << "\",style=filled";
	else
		output << "???\"";
	output << "]" << std::endl;

	// print the edge
	if (parent != nullptr)
	{
		output << "N" << (size_t)parent << " -- " << "N" << (size_t)this;
		if (isPositive)
			output << " [label=\"true\"];" << std::endl;
		else
			output << " [label=\"false\"];" << std::endl;
	}
}