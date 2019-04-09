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
#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
#include "TextureManager.h"
#include "Animation2.h"
#include <random>
#include "Item.h"
#include <vector>
#include <string>

namespace GEX {
	class Villager :
		public Entity
	{
	public:
		enum class Type { Courtney, Greg, Jhon, count };

		enum class State { Up, Down, Left, Right, MoveUp, MoveDown, MoveLeft, MoveRight, count };

		Villager(Villager::Type type, GEX::TextureManager& textures);
		~Villager() = default;

		unsigned int							getCategory() const override;
		sf::FloatRect							getBoundingBox() const override;
		void									accelerate(sf::Vector2f velocity) override;
		bool									isMarkedForRemoval() const override;
		bool									isBuying();
		void									checkBuy();
		void									setBuy(bool buying) { _buying = buying; }
		sf::Vector2f							randomMove();
		Type									getType();
		int										getValue(Item::Type item);
		std::string								getRandomDialogue();

		//timers
		void decrementBuyTime(sf::Time time);
		sf::Time getBuyTime();
		void decrementMoveTime(sf::Time time);
		sf::Time getMoveTime();
		void decrementLeaveTime(sf::Time time);
		sf::Time getLeaveTime();
		void resetBuyTime();
		void resetMoveTime();

		bool finishedAnimation() const;

		bool finishedAnimation(State state) const;

		void setState(State state);

	private:
		void									updateCurrent(sf::Time dt, CommandQueue& commands) override;
		void									drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
		void									updateStates();



	private:
		const sf::Time defaultBuyTime = sf::seconds(120);//120
		const sf::Time defaultMoveTime = sf::seconds(10);//10

		Type									type_;
		State									state_;
		mutable sf::Sprite						sprite_;
		mutable std::map<State, Animation2>		animations_;
		const TextureManager&					textures_;
		sf::Time								_leaveTime;
		sf::Time								_buyTime;
		sf::Time								_moveTime;
		bool									_buying;
		std::map<Item::Type, int>				_values;
		std::vector<std::string>				_dialogue;

	};

}