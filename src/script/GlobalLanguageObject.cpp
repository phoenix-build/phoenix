/*
 * (C) 2015-2016 Augustin Cavalier
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "GlobalLanguageObject.h"

#include <vector>

#include "Phoenix.h"

#include "Function.h"
#include "Object.h"
#include "Stack.h"

#include "util/PrintUtil.h"
#include "util/StringUtil.h"

using std::vector;

namespace Script {

GlobalLanguageObject::GlobalLanguageObject()
	:
	Object(Type::Map)
{
	map = new ObjectMap;

	// Instantiate this object
	map->set("checkVersion", FunctionObject([](Stack*, Object*, ObjectMap& params) -> Object {
		NativeFunction_COERCE_OR_THROW("minimum", minimum, Type::String);
		vector<std::string> components = StringUtil::split(minimum.string, ".");
		if (components.size() == 0)
			throw Exception(Exception::TypeError,
				std::string("'minimum' must have at least 1 component"));
		vector<int> parts;
		for (const std::string& str : components)
			parts.push_back(std::stoi(str, nullptr, 10));
		while (parts.size() != 3)
			parts.push_back(-1);
		if (parts[0] > PHOENIX_VERSION_MAJOR ||
			parts[1] > PHOENIX_VERSION_MINOR ||
			parts[2] > PHOENIX_VERSION_PATCH)
			throw Exception(Exception::UserError,
				std::string("minimum version of Phoenix required is ").append(minimum.string)
				 .append(" and this is " PHOENIX_VERSION));
		return Object();
	}));

	// Also instantiate the GlobalFunctions object
	GlobalFunctions.insert({"print", Function([](Stack*, Object*, ObjectMap& params) -> Object {
		Object zero = params.get("0");
		std::string message;
		if (zero.type() == Type::String)
			message += zero.string;
		else
			message += zero.asStringPretty();
		PrintUtil::message(message);
		return Object();
	})});
	GlobalFunctions.insert({"fatal", Function([](Stack*, Object*, ObjectMap& params) -> Object {
		NativeFunction_COERCE_OR_THROW("0", zero, Type::String);
		throw Exception(Exception::UserError, std::string(zero.string));
	})});
	GlobalFunctions.insert({"Map", Function([](Stack*, Object*, ObjectMap& params) -> Object {
		return MapObject(new ObjectMap(params));
	})});
}

}