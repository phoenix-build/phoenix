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
	_inherited()
{
}
ObjectMap::~ObjectMap()
{
	for (_inherited::const_iterator it = begin(); it != end(); it++) {
		delete it->second;
	}
}

Object ObjectMap::get(string key)
{
	_inherited::iterator it = find(key);
	if (it != end())
		return *it->second;
	return Object(Type::Undefined);
}

Object* ObjectMap::get_ptr(string key)
{
	_inherited::iterator it = find(key);
	if (it != end())
		return it->second;
	return nullptr;
}

void ObjectMap::set(string key, Object value)
{
	_inherited::iterator it = find(key);
	if (it != end())
		it->second = new Object(value);
	else
		insert({key, new Object(value)});
}

}
