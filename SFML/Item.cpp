/**
* @file
* @author Jeff Ottar-
* @version 1.0
*
*
* @section DESCRIPTION
* <  >
*
*
* @section LICENSE
*
*
* Copyright 2018
* Permission to use, copy, modify, and/or distribute this software for
* any purpose with or without fee is hereby granted, provided that the
* above copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
* @section Academic Integrity
* I certify that this work is solely my own and complies with
* NBCC Academic Integrity Policy (policy 1111)
*/

#include "Item.h"
#include "DataTables.h"


namespace GEX {

	namespace
	{
		const std::map<Item::Type, ItemData> TABLE = initializeItemData();
	}


	Item::Item(Item::Type type,  GEX::TextureManager & textures):
		type_(type)
		, state_(State::Normal)
		, _textures(&textures)
	{
		sprite_.setTexture(_textures->get(TABLE.at(type).texture));
		sprite_.setTextureRect(TABLE.at(type).textureRect);
	}
	std::string Item::getItemName(Item::Type type)
	{
		switch (type)
		{
		case Item::Type::BlackCoat:
			return "Black Coat";
		case Item::Type::Bread:
			return "Bread";
		case Item::Type::OldSword:
			return "Old Sword";
		case Item::Type::Hat:
			return "Hat";
		case Item::Type::JeweledBangle:
			return "Jeweled Bangle";
		default: 
			return "";
		}
	}

	std::vector<Item::Type> Item::getAllTypes()
	{
		std::vector<Item::Type> items;
		for (int i = 0; i < (int)Item::Type::Count; i++)
		{
			items.push_back(static_cast<Item::Type>(i));
		}
		return items;
	}

	double Item::getPrice()
	{
		return TABLE.at(type_).price;
	}

	
}