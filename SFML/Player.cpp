/**
* @file
* Player.cpp
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
#include "Player.h"
#include "DataTables.h"
#include "Utility.h"
#include "CommandQueue.h"
#include "SoundNode.h"
#include <functional>
#include "Category.h"


namespace GEX
{
	namespace
	{
		const std::map<Player::PlayerType, PlayerData> TABLE = initializePlayerData();
	}

	Player::Player(PlayerType type,  GEX::TextureManager & textures)
		: Entity(TABLE.at(type).hitpoints)
		, type_(type)
		, textures_(textures)
		, state_(State::Up)
		, riding_(false)
		, _money(1000)
	{
		textures.load(GEX::TextureID::Characters, "Media/Textures/characters.png");
		sprite_.setTexture(textures.get(TABLE.at(type).texture));
		sprite_.scale(5.f, 5.f);
		for (auto a : TABLE.at(type).animations)
		{
			animations_[a.first] = a.second;
		}

		//sprite_.setTextureRect(TABLE.at(type).textureRect);
		sprite_.setTextureRect(sf::IntRect());
		centerOrigin(sprite_);

	}
	unsigned int Player::getCategory() const
	{
		return Category::Player;
	}
	sf::FloatRect Player::getBoundingBox() const
	{
		auto box = getWorldTransform().transformRect(sprite_.getGlobalBounds());
		box.width -= 10; // tighten up bounding box for more realistic collisions
		box.left += 5;
		box.height -= 10;
		box.top += 5;
		return box;
	}

	void Player::accelerate(sf::Vector2f velocity)
	{
	}

	void Player::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
		//updateStates();

		auto rec = animations_.at(state_).update(dt);

		sprite_.setTextureRect(rec);
		
		centerOrigin(sprite_);

		Entity::updateCurrent(dt, commands);
	}

	void Player::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(sprite_, states);
	}

	void Player::updateStates()
	{

		if (state_ == State::MoveUp && animations_[state_].isFinished())
			setState(State::Up);
		else if (state_ == State::MoveDown && animations_[state_].isFinished())
			setState(State::Down);
		else if (state_ == State::MoveLeft && animations_[state_].isFinished())
			setState(State::Left);
		else if (state_ == State::MoveRight && animations_[state_].isFinished())
			setState(State::Right);

		if (isDestroyed() && state_ != State::Die) {
			setState(State::Die);
		}
	}
	void Player::playLocalSound(CommandQueue & commands, SoundEffectID effect)
	{
		Command playSoundCommand;
		playSoundCommand.category = Category::SoundEffect;
		playSoundCommand.action = derivedAction<SoundNode>(
			std::bind(&SoundNode::playSound, std::placeholders::_1, effect, getWorldPosition())
			);

		commands.push(playSoundCommand);

	}

	bool Player::isMarkedForRemoval() const
	{
		return isDestroyed() && finishedAnimation();
	}

	bool Player::finishedAnimation() const
	{
		return animations_[state_].isFinished();
	}

	bool Player::finishedAnimation(State state) const
	{
		if (state_ == state) {
			return animations_[state_].isFinished();
		}

		return false;
	}

	bool Player::finishedDeathAnimation() const
	{
		if (state_ == State::Die) {
			return animations_[state_].isFinished();
		}
		else {
			return true;
		}

	}

	void Player::destroy()
	{
		if (finishedDeathAnimation()) {
			state_ = State::Die;

			Entity::destroy();
		}
	}

	bool Player::isRiding() const
	{
		return riding_;
	}

	void Player::setRiding(bool riding)
	{
		riding_ = riding;
	}

	void Player::setState(State state)
	{
		state_ = state;
		animations_[state_].restart();
	}

}

