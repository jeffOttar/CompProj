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
#include "Map.h"
#include "Utility.h"
#include "FontManager.h"
#include "ResourceIdentifiers.h"

//#include "Player.h"


	Map::Map(GEX::StateStack & stack, Context context):
		State(stack, context),
		_options(),
		_optionsIndex(0)
	{
		_money = context.playerBody->getMoney();

		sf::Texture& texture = context.textures->get(GEX::TextureID::Map);

		_backgroundSprite.setTexture(texture);
		_backgroundSprite.scale(1.5, 1.5);

		/**
		set up sprite option list
		**/
		sf::Sprite shopOption;
		shopOption.setTexture(context.textures->get(GEX::TextureID::ShopIcon));
		centerOrigin(shopOption);
		shopOption.setPosition(context.window->getView().getSize().y / 1.2f, context.window->getView().getSize().x / 2.5f);
		_options.push_back(shopOption);//add the option to the list

		sf::Sprite warehouseOption;
		warehouseOption.setTexture(context.textures->get(GEX::TextureID::WarehouseIcon));
		centerOrigin(warehouseOption);
		warehouseOption.setPosition(context.window->getView().getSize().y / 5.f, context.window->getView().getSize().x / 2.f);
		_options.push_back(warehouseOption);//add the option to the list

		sf::Sprite marcoOption;
		marcoOption.setTexture(context.textures->get(GEX::TextureID::MarcoMoney));
		centerOrigin(marcoOption);
		marcoOption.setPosition(context.window->getView().getSize().y / 1.5f, context.window->getView().getSize().x / 12.f);
		_options.push_back(marcoOption);//add the option to the list
		

		//this updates the display to show current selection
		updateOptionSelect();
	}
	void Map::draw()
	{
		sf::RenderWindow& window = *getContext().window;
		window.setView(window.getDefaultView());

		//draw objects
		window.draw(_backgroundSprite);
		for (const sf::Sprite& icon : _options)
		{
			window.draw(icon);
		}
	}
	bool Map::update(sf::Time dt)
	{
		return false;
	}
	bool Map::handleEvent(const sf::Event & event)
	{
		//if the event is not a keypress
		if (event.type != sf::Event::KeyPressed)
		{
			return false;
		}

		if (event.key.code == sf::Keyboard::Return)//if key press is return
		{
			if (_optionsIndex == Warehouse)// if currently selected option is warehouse
			{
				requestStackPop();
				requestStackPush(GEX::StateID::Warehouse);
			}
			else if (_optionsIndex == Marco)
			{
				if (_money >= 100000000)//can only go to end game zone if have enough money
				/*if (_money >= 1)*/
				{
					requestStackPop();
					requestStackPush(GEX::StateID::MarcoLand);
				}
			}
			else if (_optionsIndex == Shop)
			{
				//remove the item from the stack so it is empty (pop will empty stack because the menu has nothing under it)
				requestStackPop();
			}
		}
		else if (event.key.code == sf::Keyboard::Up)//if key is up
		{
			//if they use the up key move the index in the correct direction for the selections
			if (_optionsIndex > 0)
			{
				_optionsIndex--;
			}
			else
			{
				_optionsIndex = _options.size() - 1;
			}

			updateOptionSelect();
		}
		else if (event.key.code == sf::Keyboard::Down)//if key is down
		{
			//if they use the up key move the index in the correct direction for the selections
			if (_optionsIndex < _options.size() - 1)// if it is less than the max size/ top of the vector
			{
				_optionsIndex++;
			}
			else//if it reaches the top then reset to the 0 indexed item
			{
				_optionsIndex = 0;
			}

			updateOptionSelect();
		}

		return false;
	}
	void Map::updateOptionSelect()
	{
		if (_options.empty())
		{
			return;
		}

		for (sf::Sprite& icon : _options)//set all icons to original
		{
			icon.setScale(1.f, 1.f);
		}

		//change the selected icon scale
		_options[_optionsIndex].scale(2.f,2.f);
	}
