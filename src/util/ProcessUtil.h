/*
 * (C) 2015 Augustin Cavalier
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <string>

class ProcessUtil
{
public:
	struct ExecResult {
		std::string output;
		int exitcode;
	};
	static ExecResult exec(const std::string& command);
};
