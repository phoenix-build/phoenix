/*
 * (C) 2015 Augustin Cavalier
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "Stack.h"

#include <iostream>

using std::cout;
using std::string;
using std::vector;

namespace Language {

std::map<std::string, Function> GlobalFunctions;

Stack::Stack()
{
	push();
}

void Stack::push()
{
	fStack.push_back(ObjectMap());
}

void Stack::pop()
{
	fStack.pop_back();
}

std::vector<ObjectMap>::size_type Stack::getPos(std::string variable)
{
	std::vector<ObjectMap>::size_type size = fStack.size() - 1;
	for (std::vector<ObjectMap>::size_type i = 0; i <= size; i++) {
		if (fStack[size - i].get_ptr(variable) != nullptr)
			return i;
	}
	return fStack.size() - 1;
}

Object Stack::get(const vector<string> variable)
{
	Object* ret = nullptr;
	if (variable[0][0] == '$') {
		// it's a superglobal
		string var = variable[0];
		var = var.substr(1);
		ret = fSuperglobalScope.get_ptr(var);
	} else
		ret = fStack[getPos(variable[0])].get_ptr(variable[0]);
	for (vector<string>::size_type i = 1; i < variable.size(); i++) {
		Language_COERCE_OR_THROW_PTR("referenced variable", ret, Map);
		ret = ret->map->get_ptr(variable[i]);
	}

	if (ret != nullptr)
		return *ret;
	return Object();
}

void Stack::set(vector<string> variable, Object value)
{
	if (variable[0][0] == '$') {
		// it's a superglobal
		throw Exception(Exception::AccessViolation, "superglobals are read-only");
	}
	vector<ObjectMap>::size_type loc = getPos(variable[0]);
	if (variable.size() == 1) {
		fStack[loc].set(variable[0], value);
		return;
	}

	Object* res = fStack[loc].get_ptr(variable[0]);
	Language_COERCE_OR_THROW_PTR("referenced variable", res, Map);
	for (vector<string>::size_type i = 1; i < variable.size() - 1; i++) {
		res = res->map->get_ptr(variable[i]);
		Language_COERCE_OR_THROW_PTR("referenced variable", res, Map);
	}
	res->map->set(variable[variable.size() - 1], value);
}

void Stack::addSuperglobal(string variableName, Object value)
{
	fSuperglobalScope.set(variableName, value);
}

void Stack::print()
{
	cout << "-- VM STACK DUMP --\n";
	int tabs = 1;
	for (const ObjectMap& m : fStack) {
		for (ObjectMap::const_iterator it = m.begin(); it != m.end(); it++) {
			for (int i = 0; i < tabs; i++)
				cout << "    ";
			cout << it->first << ": " << it->second->asString() << "\n";
		}
		tabs++;
	}
}

}
