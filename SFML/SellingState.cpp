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
#include "SellingState.h"
#include "CurrentShelf.h"
#include "CurrentVillager.h"
#include "Utility.h"
#include "FontManager.h"



SellingState::SellingState(GEX::StateStack & stack, Context context)
	: GEX::State(stack, context),
	_player(context.playerBody),
	_backgroundSprite(),
	_textures(context.textures),
	_total(0),
	_sold(false),
	_incrementer(1)
{
	_backgroundSprite.setTexture(context.textures->get(GEX::TextureID::Warehouse));

	_face.setTexture(context.textures->get(GEX::TextureID::Indifferent));
	_face.setPosition(context.window->getView().getSize() / 2.f - (sf::Vector2f(-250.f, 125.f)));

	//text for amount
	sf::Text option;
	option.setString("0");
	option.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	centerOrigin(option);
	option.setPosition(context.window->getView().getSize()/ 2.f - (sf::Vector2f(0.f, -250.f)));
	option.setCharacterSize(48);
	_amount.push_back(option);

	//text for item
	sf::Text item;
	auto item2 = GEX::CurrentShelf::getInstance().getCurrentShelf()->getItem().getType();
	auto s = GEX::CurrentShelf::getInstance().getCurrentShelf()->getItem().getItemName(item2);
	item.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	item.setString(s);
	centerOrigin(item);
	item.setPosition((context.window->getView().getSize() / 2.f) - sf::Vector2f(250.f,-250.f));
	item.setCharacterSize(48);
	_amount.push_back(item);
}

void SellingState::draw()
{
	//Draw the elements
	//getting window
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;//the translucent background
	backgroundShape.setFillColor(sf::Color(255, 255, 224, 150));
	backgroundShape.setSize(sf::Vector2f(window.getView().getSize().x, window.getView().getSize().y / 3));
	backgroundShape.setPosition(0, window.getView().getSize().y*0.7f);

	//draw objects
	window.draw(backgroundShape);

	window.draw(_face);

	//draw each of the text options
	for (const sf::Text& text : _amount)
	{
		window.draw(text);
	}
}

bool SellingState::update(sf::Time dt)
{
	return false;
}

bool SellingState::handleEvent(const sf::Event & event)
{
	//if the event is not a keypress
	if (event.type != sf::Event::KeyPressed)
	{
		return false;
	}

	if (event.key.code == sf::Keyboard::Return)//if key press is return
	{
		//update displayed text
		if (_sold)//if enter pressed after sold then user confirmed
		{
			//update all neccessary stuff
			_player->setMoney((_player->getMoney()+_total));//add money
			GEX::CurrentShelf::getInstance().getCurrentShelf()->removeItemOnShelf();//remove the item
			requestStackPop();
		}
		//if not sold update face and display some text
		auto v = GEX::CurrentVillager::getInstance().getCurrentVillager();
		auto item = (GEX::CurrentShelf::getInstance().getCurrentShelf()->getItem().getType());
		int willingToPay = v->getValue(item);
		
		if (_total <= willingToPay)//if amount less than amount willing to pay
		{
			_sold = true;
			_face.setTexture(_textures->get(GEX::TextureID::Happy));
		}
		else
		{
			_face.setTexture(_textures->get(GEX::TextureID::Unhappy));
		}

		//updateText();
	}
	else if (event.key.code == sf::Keyboard::Right)
	{

		if (_total <= 99999)
		{
			_total += _incrementer;
		}
		else//if it reaches the top then reset to the 0 indexed item
		{
			_total = 0;
		}

		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Left)
	{

		if (_total >= 1 && (!((_total / _incrementer) < 1)))
		{
			_total -= _incrementer;
		}
		else//if it reaches the bottom then reset to the 0 indexed item
		{
			_total = 0;
		}

		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Escape)
	{
		requestStackPop();
	}
	else if (event.key.code == sf::Keyboard::Num1)//change the number you increment/decrement by with numberkeys below
	{
		_incrementer = 1;
	}
	else if (event.key.code == sf::Keyboard::Num2)
	{
		_incrementer = 5;
	}
	else if (event.key.code == sf::Keyboard::Num3)
	{
		_incrementer = 10;
	}
	else if (event.key.code == sf::Keyboard::Num4)
	{
		_incrementer = 50;
	}
	else if (event.key.code == sf::Keyboard::Num5)
	{
		_incrementer = 100;
	}
	else if (event.key.code == sf::Keyboard::Num6)
	{
		_incrementer = 500;
	}
	else if (event.key.code == sf::Keyboard::Num7)
	{
		_incrementer = 1000;
	}

	return false;
}

void SellingState::updateOptionText()
{

	//update option text
	_amount[0].setString(std::to_string(_total));
	_face.setTexture(_textures->get(GEX::TextureID::Indifferent));

}
