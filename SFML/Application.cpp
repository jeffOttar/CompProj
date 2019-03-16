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
#include "Application.h"
#include "GameState.h"
#include "PauseState.h"
#include "MenuState.h"
#include "TitleState.h"
#include "GexState.h"
#include "GameOverState.h"
#include "FontManager.h"
#include "Map.h"
#include "Warehouse.h"
#include "Opening.h"
#include "Ending.h"
#include "Inventory.h"
#include "RandomDialogue.h"
#include "CurrentShelf.h"
#include "CurrentVillager.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.0f / 60.f);//seconds per frame for 60fps

Application::Application() : //_window(sf::VideoMode(640, 480), "Killer Plane", sf::Style::Close), //smaller screen
_window(sf::VideoMode(1280, 960), "Marco's Merchandise", sf::Style::Close),//bigger screen
_player(), 
_font(),
_textures(),
_currentShelf(),
_currentVillager(),
_playerBody(GEX::Player::PlayerType::Player, _textures),
_stateStack(GEX::State::Context(_window, _textures, _player,_music,_sound, _playerBody)),
_statisticsText(),
_statisticsUpdateTime(),
_statisticsNumFrames(0)
{
	//disable the ability to repeatedly press keys --> so missile cant be held down to repeatedly fire, you fire one per press
	_window.setKeyRepeatEnabled(false);

	GEX::FontManager::getInstance().load(GEX::FontID::Main,"Media/Sansation.ttf");
	
	_textures.load(GEX::TextureID::TitleScreen, "Media/Textures/title.png");//for small screen
	//_textures.load(GEX::TextureID::TitleScreen, "Media/Textures/TitleScreenBig.png");//for big screen

	//ADD THIS BACK IN AFTER THE PLAYER HAS AN ACTUAL TEXTURE SET IN THE CONSTRUCTOR OF PLAYER
	//_textures.load(GEX::TextureID::Face, "Media/Textures/face.png");//load face texture

	_textures.load(GEX::TextureID::Villagers, "Media/Textures/characters.png");
	_textures.load(GEX::TextureID::Map, "Media/Textures/MapBackground.png");
	_textures.load(GEX::TextureID::ShopIcon, "Media/Textures/homeIcon.png");
	_textures.load(GEX::TextureID::MarcoMoney, "Media/Textures/endIcon.png");
	_textures.load(GEX::TextureID::WarehouseIcon, "Media/Textures/warehouseIcon.png");
	_textures.load(GEX::TextureID::Warehouse, "Media/Textures/warehouse.png");
	_textures.load(GEX::TextureID::Items, "Media/Textures/items.png");

	_statisticsText.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	_statisticsText.setPosition(5.f, 5.f);
	_statisticsText.setCharacterSize(15);
	_statisticsText.setString("Frames / Second = \nTime / Update =");

	registerStates();
	//SWITCH BACK TO LOADING TITLE FIRST AFTER FINISHED WITH THE MAP TESTING
	//_stateStack.pushState(GEX::StateID::Map);
	//_stateStack.pushState(GEX::StateID::Opening);
	_stateStack.pushState(GEX::StateID::Title);//when running titlescreen goes to top of stack so it shows first
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (_window.isOpen())
	{
		processInput();
		timeSinceLastUpdate = clock.restart();

		if (timeSinceLastUpdate > TimePerFrame)//this loop if for the time if it is above spf and catches the update up to it
		{
			updateStats(TimePerFrame);
			update(TimePerFrame);
		}
		else
		{
			updateStats(timeSinceLastUpdate);
			update(timeSinceLastUpdate);
		}
		timeSinceLastUpdate -= TimePerFrame;

		if (_stateStack.isEmpty()) //if state stack empty user has quit and the game needs to end
		{
			_window.close();
		}
		

		render();
	}

	//while (_window.isOpen())
	//{
	//	
	//	timeSinceLastUpdate = clock.restart();

	//	while (timeSinceLastUpdate > TimePerFrame)//this loop if for the time if it is above spf and catches the update up to it
	//	{
	//		processInput();
	//		update(TimePerFrame);

	//		if (_stateStack.isEmpty()) //if state stack empty user has quit and the game needs to end
	//		{
	//			_window.close();
	//		}

	//		timeSinceLastUpdate -= TimePerFrame;
	//	}

	//	updateStats(timeSinceLastUpdate);

	//	render();
	//}
}

void Application::processInput()
{
	sf::Event event;
	while (_window.pollEvent(event))
	{
		_stateStack.handleEvent(event);

		if (event.type == sf::Event::Closed) 
		{
			_window.close();
		}
	}
}

void Application::update(sf::Time timeElapsed)
{
	_stateStack.update(timeElapsed);
}

void Application::render()
{
	_window.clear();
	_stateStack.draw();//everything on statestack drawn

	_window.setView(_window.getDefaultView());//reset the view to the default
	_window.draw(_statisticsText);
	_window.display();
}

void Application::updateStats(sf::Time deltaTime)
{
	_statisticsUpdateTime += deltaTime;
	_statisticsNumFrames += 1;
	if (_statisticsUpdateTime > sf::seconds(1))
	{
		_statisticsText.setString("Frames / Second = " + std::to_string(_statisticsNumFrames) + "\n"
			+ "Time / Update = " + std::to_string(_statisticsUpdateTime.asMicroseconds() / _statisticsNumFrames) + "us");

		_statisticsUpdateTime -= sf::seconds(1);
		_statisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	_stateStack.registerState<TitleState>(GEX::StateID::Title);
	_stateStack.registerState<MenuState>(GEX::StateID::Menu);
	_stateStack.registerState<GameState>(GEX::StateID::Game);
	_stateStack.registerState<PauseState>(GEX::StateID::Pause);
	_stateStack.registerState<GexState>(GEX::StateID::Gex);
	_stateStack.registerState<GameOverState>(GEX::StateID::GameOver);

	_stateStack.registerState<Map>(GEX::StateID::Map);
	_stateStack.registerState<Warehouse>(GEX::StateID::Warehouse);
	_stateStack.registerState<Opening>(GEX::StateID::Opening);
	_stateStack.registerState<Ending>(GEX::StateID::Ending);
	_stateStack.registerState<Inventory>(GEX::StateID::Inventory);
	_stateStack.registerState<RandomDialogue>(GEX::StateID::Dialogue);
}
