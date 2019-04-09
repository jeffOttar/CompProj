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
#include "RandomDialogue.h"
#include "Utility.h"
#include "FontManager.h"
#include <string>
#include <sstream>
#include "CurrentVillager.h"

RandomDialogue::RandomDialogue(GEX::StateStack & stack, Context context)
	: GEX::State(stack, context)
	, _displayedText1()
	, _displayedText2()
	, _displayedText3()
	, _end(false)
{
	//put the texts into a vector
	_displayedTexts.push_back(&_displayedText1);
	_displayedTexts.push_back(&_displayedText2);
	_displayedTexts.push_back(&_displayedText3);

	//Get the font from the font manager
	sf::Font& font = GEX::FontManager::getInstance().get(GEX::FontID::Main);

	std::string s = GEX::CurrentVillager::getInstance().getCurrentVillager()->getRandomDialogue();

	std::string word;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, word,'/'))//   / is used to denote end of line in the map
	{
		_text.push_back(word);
	}

	//set up styling and position of texts
	const float LEFT_MARGIN = 50.f;

	sf::Vector2f viewsize = context.window->getView().getSize();

	_displayedText1.setFont(font);
	_displayedText1.setCharacterSize(50);
	_displayedText1.setFillColor(sf::Color::Black);
	centerOrigin(_displayedText1);
	_displayedText1.setPosition(LEFT_MARGIN, 0.7f * viewsize.y);

	_displayedText2.setFont(font);
	_displayedText2.setCharacterSize(50);
	_displayedText2.setFillColor(sf::Color::Black);
	centerOrigin(_displayedText2);
	_displayedText2.setPosition(LEFT_MARGIN, 0.8f * viewsize.y);

	_displayedText3.setFont(font);
	_displayedText3.setCharacterSize(50);
	_displayedText3.setFillColor(sf::Color::Black);
	centerOrigin(_displayedText3);
	_displayedText3.setPosition(LEFT_MARGIN, 0.9f * viewsize.y);

	_index = 0;
	updateText();
}

void RandomDialogue::draw()
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

	//Send elements to the window draw method
	window.draw(_displayedText1);
	window.draw(_displayedText2);
	window.draw(_displayedText3);
}

bool RandomDialogue::update(sf::Time dt)
{
	return false;
}

bool RandomDialogue::handleEvent(const sf::Event & event)
{

	//if the event is not a keypress
	if (event.type != sf::Event::KeyPressed)
	{
		return false;
	}

	if (event.key.code == sf::Keyboard::Return)//if key press is return
	{
		//update displayed text
		updateText();
		if (_end)
		{
			requestStackPop();
		}
		
	}
	return false;
}

void RandomDialogue::updateText()
{
	_end = false;
	for (int i = 0; i < _displayedTexts.size(); i++)//blank out all texts
	{
		_displayedTexts[i]->setString("");
	}
	int count = 0;
	int index = _index;
	for (int i = index; i < (index + 3); i++) {//make new texts
		if (i < _text.size())//do something
		{
			_displayedTexts[count]->setString(_text[_index]);//set as appropriate item
			count++;
			_index++;
		}
		else
		{
			_end = true;
		}
	}

}
