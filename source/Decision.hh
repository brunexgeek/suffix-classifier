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
#ifndef BRUNEXGEEK_DECISION_HH
#define BRUNEXGEEK_DECISION_HH


#include <stdint.h>
#include <string>
#include <iostream>
#include "Node.hh"


class Decision
{
	public:
		int32_t symbol;

		int32_t response;

		int32_t index;

		Decision *positive;

		Decision *negative;

		Decision *parent;

		Decision();

		Decision(
			Decision *parent );

		~Decision();

		static Decision *build(
			const Node &root,
			int defaultResponse,
			int depth = 0 );

		void plot(
			std::ostream &output);

		static Decision *deserialize(
			std::istream &input );

		void serialize(
			std::ostream &output );

	private:
		void plot(
			std::ostream &output,
			const Decision *parent,
			bool isPositive );
};


#endif // BRUNEXGEEK_DECISION_HH