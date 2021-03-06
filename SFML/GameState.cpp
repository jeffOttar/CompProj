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
#include "GameState.h"
#include "CommandQueue.h"
#include "ResourceIdentifiers.h"

GameState::GameState(GEX::StateStack & stack, Context context) :
	State(stack,context),
	_world(*context.window, *context.sound,*context.playerBody),
	_player(*context.player)
{
	context.music->play(GEX::MusicID::MissionTheme);
}

void GameState::draw()
{
	_world.draw();
}

bool GameState::update(sf::Time dt)
{
	_world.update(dt, _world.getCommandQueue());
	_world.updateVillagers(dt);

	//check if player won or died
	if (!_world.hasAlivePlayer())
	{
		_player.setMissionStatus(GEX::MissionStatus::MissionFailure);
		requestStackPush(GEX::StateID::GameOver);
	}
	else if (_world.hasPlayerReachedEnd())
	{
		_player.setMissionStatus(GEX::MissionStatus::MissionSuccess);
		requestStackPush(GEX::StateID::GameOver);
	}
	else if (_world.villagerBuying())
	{
		requestStackPush(GEX::StateID::Selling);
	}
	else if (_world.OutOfMoney())
	{
		_player.setMissionStatus(GEX::MissionStatus::MissionFailure);
		requestStackPush(GEX::StateID::NoMoney);
	}




	//get the commands to handle real time input for the game
	GEX::CommandQueue& commands = _world.getCommandQueue();
	_player.handleRealTimeInput(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event & event)
{
	//get the commands to handle the event
	auto& commands = _world.getCommandQueue();//this is the same as above in update but using auto
	_player.handleEvent(event, commands);

	//if the escape key is pressed then stack push the pause state
	if (event.type==sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
	{
		requestStackPush(GEX::StateID::Pause);
	}


	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
	{
		requestStackClear();//go straight to menu
		requestStackPush(GEX::StateID::Menu);
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)//space events
	{
		if (_world.shelfEvent(event))
		{
			requestStackPush(GEX::StateID::Inventory);
		}
		else if (_world.dialogueEvent(event))//use for the other space button event of villager dialogue
		{
			requestStackPush(GEX::StateID::Dialogue);
		}
		else if (_world.atDoor())
		{
			requestStackPush(GEX::StateID::Map);
		}
	}


	return true;
}
