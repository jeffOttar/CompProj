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
*/////
#include "Warehouse.h"
#include "Utility.h"
#include "FontManager.h"
#include "CommandQueue.h"
#include "Item.h"
#include <math.h>


	Warehouse::Warehouse(GEX::StateStack & stack, Context context):
		State(stack, context),
		_backgroundSprite(),
		_textures(context.textures),
		_itemCount(0),
		_player(context.playerBody)
	{
		_money = _player->getMoney();
		_backgroundSprite.setTexture(context.textures->get(GEX::TextureID::EasterEggWarehouse));


		/*Get a list of all items possible and make them availible in the warehouse*/
		GEX::Item tmp(GEX::Item::Type::Bread, *_textures);
		std::vector<GEX::Item::Type> itemTypes = tmp.getAllTypes();

		for (int i = 0; i < itemTypes.size(); i++)
		{
			std::unique_ptr<GEX::Item> item(new GEX::Item(itemTypes.at(i), *_textures));
			_items.push_back(*item);
		}
		for (int i = 0; i < itemTypes.size(); i++)
		{
			sf::Text option;
			option.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
			_optionString.insert(std::pair<int, std::string>(i, (tmp.getItemName(itemTypes[i]) + " " + std::to_string(_items.at(i).getPrice()))));
			_optionCount.insert(std::pair<int, std::string>(i," " + std::to_string(_itemCount)));
			option.setString(_optionString.at(i) + _optionCount.at(i));
			centerOrigin(option);
			option.setPosition(context.window->getView().getSize().x/2.f, (100.f + (50.f * i)));
			_options.push_back(option);//add the option to the list
		}
	}
	void Warehouse::draw()
	{
		sf::RenderWindow& window = *getContext().window;
		window.setView(window.getDefaultView());

		//draw objects
		window.draw(_backgroundSprite);

		//draw each of the text options
		for (const sf::Text& text : _options)
		{
			window.draw(text);
		}
	}
	bool Warehouse::update(sf::Time dt)
	{
		return false;
	}
	bool Warehouse::handleEvent(const sf::Event & event)
	{
		//if the event is not a keypress
		if (event.type != sf::Event::KeyPressed)
		{
			return false;
		}

		if (event.key.code == sf::Keyboard::Return)//if key press is return
		{

			//buy the items
			for (int i = 0; i < _itemCount; i++)//loop through the amount wanted
			{
				auto item = new GEX::Item(_items.at(_optionsIndex).getType(), *_textures);
				//item desired
				_player->addToInventory(item);
				
			}
			//change the money to new values
			double money = _player->getMoney();
			money = money - (_items.at(_optionsIndex).getPrice()*_itemCount);
			_player->setMoney(money);
			_money = _player->getMoney();
		}
		else if (event.key.code == sf::Keyboard::Up)//if key is up
		{
			_itemCount = 0;
			//if they use the up key move the index in the correct direction for the selections
			if (_optionsIndex > 0)
			{
				_optionsIndex--;
			}
			else
			{
				_optionsIndex = _options.size() - 1;
			}

			updateOptionText();
		}
		else if (event.key.code == sf::Keyboard::Down)//if key is down
		{
			_itemCount = 0;
			//if they use the up key move the index in the correct direction for the selections
			if (_optionsIndex < _options.size() - 1)// if it is less than the max size/ top of the vector
			{
				_optionsIndex++;
			}
			else//if it reaches the top then reset to the 0 indexed item
			{
				_optionsIndex = 0;
			}

			updateOptionText();
		}
		else if (event.key.code == sf::Keyboard::Right)
		{
			
			if (_money >= (_items.at(_optionsIndex).getPrice()*(_itemCount+1)))
			{
				_itemCount++;
			}
			else//if it reaches the top then reset to the 0 indexed item
			{
				_itemCount = 0;
			}

			updateOptionText();
		}
		else if (event.key.code == sf::Keyboard::Left)
		{

			if (_itemCount>=1)
			{
				_itemCount--;
			}
			else if(std::floor(_money / _items.at(_optionsIndex).getPrice())>=1)//if it reaches the top then reset to the 0 indexed item
			{
				int flooredAmount = std::floor(_money / _items.at(_optionsIndex).getPrice());
				_itemCount = flooredAmount;
			}
			else
			{
				_itemCount = 0;
			}

			updateOptionText();
		}
		else if (event.key.code == sf::Keyboard::Escape)
		{
			requestStackPop();
			requestStackPush(GEX::StateID::Map);
		}

		return false;
	}

	void Warehouse::updateOptionText()
	{
		if (_options.empty())
		{
			return;
		}

		for (sf::Text& text : _options)//set all text to white
		{
			text.setFillColor(sf::Color::White);
			text.setOutlineColor(sf::Color::Black);
		}

		//update option text
		_optionCount.at(_optionsIndex) = " " + std::to_string(_itemCount);
		_options[_optionsIndex].setString(_optionString.at(_optionsIndex) + _optionCount.at(_optionsIndex));

		//change the selected texts color
		_options[_optionsIndex].setFillColor(sf::Color::Magenta);
	}


