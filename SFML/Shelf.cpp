/**
* @file
* Shelf.cpp
* @authors
* Jeff Ottar
* John Romero
* Marco Corsini Baccaro 
* @Date
* 2018
* @version 1.0
*
* @section DESCRIPTION
* Frogger Game
*
* @section LICENSE
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
#include "Shelf.h"
#include "DataTables.h"
#include "Category.h"
#include "Utility.h"

namespace GEX {

	namespace
	{
		const std::map<Shelf::ShelfType, ShelfData> TABLE = initializeShelfData();
		const std::map<Item::Type, ItemData> ITEMTABLE = initializeItemData();
	}

	GEX::Shelf::Shelf(ShelfType type,  TextureManager & textures)
		: Entity(TABLE.at(type).hitpoints)
		, type_(type)
		, state_(ShelfState::empty)
		, sprite_(textures.get(TABLE.at(type).texture))
		, isMarkedForRemoval_(false)
		, occupied_(false)
		, backgroundSprite_()
		, _textures(&textures)
		, _item(Item::Type::Bread, textures)
	{
		//for (auto a : TABLE.at(type).animations)
		//{
		//	animations_[a.first] = a.second;
		//}

		// keeping a constant background
		// This is very important for the fly, because the is no fly over the Shelf image in the atlas
		backgroundSprite_.setTexture(textures.get(TABLE.at(type).texture));
		//backgroundSprite_.setTextureRect(textures.get(TABLE.at(type).textureRect));
		backgroundSprite_.scale(1.25f, 1.25f);
		centerOrigin(backgroundSprite_);

		sprite_.setTextureRect(sf::IntRect());
		sprite_.setPosition(sprite_.getPosition().x, sprite_.getPosition().y - 20.f);
		centerOrigin(sprite_);
	}

	void Shelf::updateStates()
	{
		// Reseting the Shelf
		if ((state_ == ShelfState::occupied )&& isOccupied() == false)
		{
			state_ = ShelfState::empty;
		}
		// Setting new states to the Shelf
		else if ((state_ == ShelfState::empty)  && isOccupied()==true)
		{
			state_ = ShelfState::occupied;
		}
	}

	void GEX::Shelf::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{

		target.draw(backgroundSprite_, states);
		
		target.draw(sprite_, states);
	}

	

	unsigned int GEX::Shelf::getCategory() const
	{
		return Category::Shelf;
	}

	sf::FloatRect GEX::Shelf::getBoundingBox() const
	{
		return getWorldTransform().transformRect(backgroundSprite_.getGlobalBounds());
		//return getWorldTransform().transformRect(sprite_.getGlobalBounds());
	}

	bool GEX::Shelf::isMarkedForRemoval() const
	{
		return false;
	}

	void Shelf::setState(ShelfState state)
	{
		state_ = state;
		animations_[state_].restart();
	}

	bool GEX::Shelf::isOccupied() const
	{
		return occupied_;
	}

	void GEX::Shelf::setOccupied(bool occupy)
	{
		occupied_ = occupy;
	}

	void GEX::Shelf::setItemOnShelf(Item item)
	{
		//get the item type and then the sprite from the table for that item type
		auto type = item.getType();
		_item.setItem(type);
		setOccupied(true);
		sprite_.setTexture(_textures->get(ITEMTABLE.at(_item.getType()).texture));
		sprite_.setTextureRect((ITEMTABLE.at(_item.getType()).textureRect));
		sprite_.setScale(2,2);
	}

	Item  GEX::Shelf::removeItemOnShelf()
	{
		sprite_.setScale(0.000000000001f, 0.000000000001f);

		setOccupied(false);
		return _item;
	}

	Item Shelf::getItem()
	{
		return _item;
	}

	
	void GEX::Shelf::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
		updateStates();

		/*auto rec = animations_.at(state_).update(dt);

		sprite_.setTextureRect(rec);*/
		centerOrigin(sprite_);

		if (state_ != ShelfState::occupied) {
			Entity::updateCurrent(dt, commands);
		}
	}
}

