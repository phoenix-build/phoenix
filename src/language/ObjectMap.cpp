/*
 * (C) 2015 Augustin Cavalier
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "ObjectMap.h"

using std::map;
using std::string;

namespace Language {

ObjectMap::ObjectMap()
	:
	map<string, Object>()
{
}

Object ObjectMap::get(string key)
{
	return (*this)[key];
}

void ObjectMap::set(string key, Object value)
{
	(*this)[key] = value;
}

};