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

#include "Inventory.h"
#include "Utility.h"
#include "FontManager.h"
#include "CurrentShelf.h"

Inventory::Inventory(GEX::StateStack & stack, Context context) :
	State(stack, context),
	_player(context.playerBody),
	_textures(context.textures),
	_shelf(GEX::CurrentShelf::getInstance().getCurrentShelf())
{
  	for (auto i : _player->getInventory())
	{
		auto item = new GEX::Item(i.first, *_textures);
		_items.push_back(*item);
	}
	for (int i = 0; i < _player->getInventory().size(); i++)
	{
		sf::Text option;
		option.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
		option.setString(_items[i].getItemName(_items[i].getType()));
		centerOrigin(option);
		option.setPosition(context.window->getView().getSize().x / 2.f, (100.f + (50.f * i)));
		_options.push_back(option);//add the option to the list
	}
}

void Inventory::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;//the translucent background
	backgroundShape.setFillColor(sf::Color(211, 211, 211, 100));
	backgroundShape.setSize(window.getView().getSize());

	//draw objects
	window.draw(backgroundShape);


	for (const sf::Text& text : _options)
	{
		window.draw(text);
	}
}

bool Inventory::update(sf::Time dt)
{
	return false;
}

bool Inventory::handleEvent(const sf::Event & event)
{
	//if the event is not a keypress
	if (event.type != sf::Event::KeyPressed)
	{
		return false;
	}

	if (event.key.code == sf::Keyboard::Return)//if key press is return
	{
		//set the item to shelf 
		_shelf->setItemOnShelf(_items[_optionsIndex]);
		_player->removeFromInventory(&_items[_optionsIndex]);
		requestStackPop();
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

		updateOptionText();
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

		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Escape)
	{
		requestStackPop();
	}
	return false;
}

void Inventory::updateOptionText()
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

	//change the selected texts color
	_options[_optionsIndex].setFillColor(sf::Color::Cyan);
}
