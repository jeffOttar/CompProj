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

#include "PlayerControl.h"
#include "Command.h"
#include "CommandQueue.h"
#include "Category.h"
#include "Player.h"
#include <functional>

namespace GEX {

	struct PlayerMover
	{
		PlayerMover(float vx, float vy) : movement(vx, vy) {}

		void	operator() (Player& player, sf::Time dt) const//move then set the correct state
		{
			player.move(movement);


			//depending on the movement change the state if it is not running and set correct orientation
			if (movement.x < 0)
			{
				if (player.finishedAnimation())
					player.setState(Player::State::MoveLeft);
			}
			else if (movement.x > 0)
			{
				if (player.finishedAnimation())
					player.setState(Player::State::MoveRight);
			}
			else if (movement.y < 0)
			{
				if (player.finishedAnimation())
					player.setState(Player::State::MoveUp);

			}
			else if (movement.y > 0)
			{
				if (player.finishedAnimation())
					player.setState(Player::State::MoveDown);

			}
		}

		sf::Vector2f	movement;
	};


	PlayerControl::PlayerControl() :
		_currentMissionStatus(MissionStatus::MissionRunning)
	{
		//set up key bindings
		_keyBindings[sf::Keyboard::Left] = Action::MoveLeft;
		_keyBindings[sf::Keyboard::Right] = Action::MoveRight;
		_keyBindings[sf::Keyboard::Up] = Action::MoveUp;
		_keyBindings[sf::Keyboard::Down] = Action::MoveDown;

		//alternate key bindings
		//_keyBindings[sf::Keyboard::A] = Action::MoveLeft;
		//_keyBindings[sf::Keyboard::D] = Action::MoveRight;
		//_keyBindings[sf::Keyboard::W] = Action::MoveUp;
		//_keyBindings[sf::Keyboard::S] = Action::MoveDown;


		_keyBindings[sf::Keyboard::Return] = Action::Interact;

		//set up action bindings
		initializeActions();
	}

	///events are the one time events like firing a Missile
	void PlayerControl::handleEvent(const sf::Event & event, CommandQueue & commands)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			auto found = _keyBindings.find(event.key.code);
			if (found != _keyBindings.end())
			{
				commands.push(_actionBindings[found->second]);
			}
		}
	}
	void PlayerControl::handleRealTimeInput(CommandQueue & commands)
	{
		//traverse all the assigned keys, look up the action, generate the command
		for (auto pair : _keyBindings)
		{
			if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			{
				commands.push(_actionBindings[pair.second]);
			}
		}
	}
	void PlayerControl::setMissionStatus(MissionStatus status)
	{
		_currentMissionStatus = status;
	}
	MissionStatus PlayerControl::getMissionStatus() const
	{
		return _currentMissionStatus;
	}
	void PlayerControl::initializeActions()
	{
		const float playerSpeed = 1.f;

		_actionBindings[Action::MoveLeft].action = derivedAction<Player>(PlayerMover(-playerSpeed, 0.f));
		_actionBindings[Action::MoveRight].action = derivedAction<Player>(PlayerMover(playerSpeed, 0.f));
		_actionBindings[Action::MoveUp].action = derivedAction<Player>(PlayerMover(0.f, -playerSpeed));
		_actionBindings[Action::MoveDown].action = derivedAction<Player>(PlayerMover(0.f, playerSpeed));

		for (auto& pair : _actionBindings)//sets the action bindings to the player aircraft
		{
			pair.second.category = Category::Type::Player;
		}


	}
	bool PlayerControl::isRealtimeAction(Action action)
	{
		switch (action)
		{
		case Action::MoveLeft:
		case Action::MoveRight:
		case Action::MoveUp:
		case Action::MoveDown:
			return true;//if the action is any of the realtime actions return true
			break;
		default:
			return false;
		}
	}

}