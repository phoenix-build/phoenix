/*
 * (C) 2015-2016 Augustin Cavalier
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "Object.h"

using std::map;
using std::string;

namespace Script {

ObjectMap::ObjectMap()
	:
	_inherited()
{
}
ObjectMap::~ObjectMap()
{
}

Object ObjectMap::get(string key) const
{
	_inherited::const_iterator it = find(key);
	if (it != end())
		return CopyObject(it->second);
	return UndefinedObject();
}

Object ObjectMap::get_ptr(string key)
{
	_inherited::iterator it = find(key);
	if (it != end())
		return it->second;
	return nullptr;
}

void ObjectMap::set_ptr(string key, Object value)
{
	_inherited::iterator it = find(key);
	if (it != end())
		it->second = value;
	else
		insert({key, value});
}

}
