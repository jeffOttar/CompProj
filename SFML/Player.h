/**
* @file
* Player.h
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
#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
#include "TextureManager.h"
#include "Command.h"
#include "Animation2.h"

namespace GEX
{
	class Player : public Entity
	{
	public:

		enum class PlayerType { Player };

		enum class State { Up, Down, Left, Right, MoveUp, MoveDown, MoveLeft, MoveRight, Die, count };

		Player(PlayerType type,  GEX::TextureManager& textures);
		~Player() = default;

		unsigned int							getCategory() const override;
		sf::FloatRect							getBoundingBox() const override;
		void									accelerate(sf::Vector2f velocity) override;
		void									setState(State state);
		bool									isMarkedForRemoval() const override;
		bool									finishedAnimation() const;
		bool									finishedAnimation(State state) const;
		bool									finishedDeathAnimation() const;
		void									destroy() override;
		bool									isRiding() const;
		void									setRiding(bool riding);
		void									playLocalSound(CommandQueue & commands, SoundEffectID effect);

		double getMoney() { return _money; }
		void setMoney(double money) { _money = money; }

	private:
		void									updateCurrent(sf::Time dt, CommandQueue& commands) override;
		void									drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
		void									updateStates();

	private:
		PlayerType								type_;
		State									state_;
		mutable sf::Sprite						sprite_;
		mutable std::map<State, Animation2>		animations_;
		const TextureManager&					textures_;
		bool									riding_;
		double _money;



	};
}
