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
* Copyright 2017
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
#include "Villager.h"
#include "Category.h"
#include "Utility.h"
#include "DataTables.h"
#include "Item.h"
#include <cstdlib>

namespace GEX {
	namespace
	{
		const std::map<Villager::Type, VillagerData> TABLE = initializeVillagerData();
	}
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, 100);
	auto randomNumber = std::bind(distribution, generator);//bound random num generator for reuse

	Villager::Villager(Villager::Type type, GEX::TextureManager & textures)
		: Entity(TABLE.at(type).hitpoints)
		, type_(type)
		, textures_(textures)
		, state_(State::Left)
		, _leaveTime(sf::seconds(300))
		, _buyTime(defaultBuyTime)
		, _moveTime(defaultMoveTime)
		, _buying(false)
		,_values(TABLE.at(type).value)
		, _dialogue(TABLE.at(type).dialogue)
	{ 
		sprite_.setTexture(textures.get(TABLE.at(type).texture));
		sprite_.scale(5.f, 5.f);
		for (auto a : TABLE.at(type).animations)
		{
			animations_[a.first] = a.second;
		}
		sprite_.setTextureRect(sf::IntRect());
		centerOrigin(sprite_);
	}	  

	unsigned int Villager::getCategory() const
	{
		switch (type_)//return the category based on the type
		{
		case Type::Courtney:
			return Category::Courtney;
			break;
		case Type::Greg:
			return Category::Greg;
			break;
		case Type::Jhon:
			return Category::Jhon;
			break;

		}
	}

	sf::FloatRect Villager::getBoundingBox() const
	{
		auto box = getWorldTransform().transformRect(sprite_.getGlobalBounds());
		box.width -= 85; // tighten up bounding box for more realistic collisions
		box.left += 45;
		box.height -= 75;
		box.top += 60;
		return box;
	}

	void Villager::accelerate(sf::Vector2f velocity)
	{
	}

	bool Villager::isMarkedForRemoval() const
	{
		return isDestroyed() && finishedAnimation();
	}

	bool Villager::isBuying()
	{
		return _buying;
	}

	void Villager::checkBuy()//random check to set variable
	{
		if (randomNumber() <= 85)//85% chance to buy 15% chance to not buy item
		{
			_buying = true;
		}
		else if (randomNumber() > 85)
		{
			_buying = false;
		}
	}

	sf::Vector2f Villager::randomMove()//random direction movement
	{
		if (randomNumber() <= 30)//random number is 1 - 100 so based on odds move in direction
		{
			return sf::Vector2f(-25,0);
		}
		else if (randomNumber() > 30 && randomNumber() < 50)
		{
			return sf::Vector2f(25, 0);
		}
		else if (randomNumber() >= 55 && randomNumber() < 75)
		{
			return sf::Vector2f(0, 25);
		}
		else if (randomNumber() >= 75 && randomNumber() < 95)
		{
			return sf::Vector2f(0, -25);
		}
		else
		{
			return sf::Vector2f();
		}

	}

	Villager::Type Villager::getType()
	{
		return type_;
	}

	int Villager::getValue(Item::Type item)
	{
		return _values.at(item);
	}

	std::string Villager::getRandomDialogue()
	{
		int size = _dialogue.size();
		int randomNum = (rand() % size);//return number of 0 -> size-1


		return _dialogue.at(randomNum);//get a random dialogue from the vector
	}

	void Villager::decrementBuyTime(sf::Time time)
	{
		_buyTime -= time;
	}

	sf::Time Villager::getBuyTime()
	{
		return _buyTime;
	}

	void Villager::decrementMoveTime(sf::Time time)
	{
		_moveTime -= time;
	}

	sf::Time Villager::getMoveTime()
	{
		return _moveTime;
	}

	void Villager::decrementLeaveTime(sf::Time time)
	{
		_leaveTime -= time;
	}

	sf::Time Villager::getLeaveTime()
	{
		return _leaveTime;
	}

	void Villager::resetBuyTime()
	{
		_buyTime = defaultBuyTime;
	}

	void Villager::resetMoveTime()
	{
		_moveTime = defaultMoveTime;
	}

	void Villager::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
		updateStates();

		auto rec = animations_.at(state_).update(dt);

		sprite_.setTextureRect(rec);

		centerOrigin(sprite_);

		Entity::updateCurrent(dt, commands);
	}

	void Villager::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(sprite_, states);
	}

	void Villager::updateStates()
	{
		if (state_ == State::MoveUp && animations_[state_].isFinished())
			setState(State::Up);
		else if (state_ == State::MoveDown && animations_[state_].isFinished())
			setState(State::Down);
		else if (state_ == State::MoveLeft && animations_[state_].isFinished())
			setState(State::Left);
		else if (state_ == State::MoveRight && animations_[state_].isFinished())
			setState(State::Right);
	}

	bool Villager::finishedAnimation() const
	{
		return animations_[state_].isFinished();
	}

	bool Villager::finishedAnimation(State state) const
	{
		if (state_ == state) {
			return animations_[state_].isFinished();
		}

		return false;
	}

	void Villager::setState(State state)
	{
		state_ = state;
		animations_[state_].restart();
	}

}