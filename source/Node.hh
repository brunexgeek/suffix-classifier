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
#ifndef BRUNEXGEEK_NODE_HH
#define BRUNEXGEEK_NODE_HH


#include <stdint.h>
#include <string>
#include <iostream>


/**
 * This class represents a node of the graph. The final graph resembles
 * a Deterministic Finite Automata (DFA).
 */
class Node
{
	public:
		static const int MAX_SYMBOLS = 26 + 32;

		static const uint32_t FLAG_TERMINAL = 0x01;

		static const uint32_t FLAG_DECISION = 0x02;

		/**
		 * @brief Flags for binary properties.
		 */
		uint32_t flags;

		/**
		 * Response this node should provide (its class).
		 */
		int response;

		/**
		 * Symbol associated with this node.
		 */
		int symbol;

		/**
		 * Number of child nodes.
		 */
		int count;

		Node *parent;

		Node(
			Node *parent );

		~Node();

		/**
		 * @brief Build the graph
		 */
		void append(
			const std::string &value,
			int response );

		bool checkMixed();

		void plot(
			std::ostream &output );

		static int map(
			char value );

		static char unmap(
			int value );

		Node *operator[](
			int index );

		Node *operator[](
			size_t index );

		const Node *operator[](
			int index ) const;

		const Node *operator[](
			size_t index ) const;

	private:
		/**
		 * @brief Pointers to child nodes for each valid symbol.
		 */
		Node *next[MAX_SYMBOLS];

		/**
		 * @brief Build the graph
		 */
		void appendChar(
			const std::string &value,
			int resp,
			size_t position,
			bool &overwrite );

		void plot(
			std::ostream &output,
			const Node *parent );
};


#endif // BRUNEXGEEK_NODE_HH