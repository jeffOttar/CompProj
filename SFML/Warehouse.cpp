#include "Warehouse.h"
#include "Utility.h"
#include "FontManager.h"
#include "CommandQueue.h"
#include "Item.h"


	Warehouse::Warehouse(GEX::StateStack & stack, Context context):
		State(stack, context),
		_backgroundSprite()
	{
		//set the background face texture
		_backgroundSprite.setTexture(context.textures->get(GEX::TextureID::Face));
		centerOrigin(_backgroundSprite);
		_backgroundSprite.setColor(sf::Color(255, 255, 255, 100));//make image transparent

		//make loop creating a scrollable list of text for the items
		//set up the menu 
		//the play option settings
		//sf::Text playOption;
		//playOption.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
		//playOption.setString("Play");
		//centerOrigin(playOption);
		//playOption.setPosition(context.window->getView().getSize() / 2.f);
		//_options.push_back(playOption);//add the option to the list

		////exit option settings
		//sf::Text exitOption;
		//exitOption.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
		//exitOption.setString("Exit");
		//centerOrigin(exitOption);
		//exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f));
		//_options.push_back(exitOption);//add the option to the list
	}
	void Warehouse::draw()
	{
		sf::RenderWindow& window = *getContext().window;
		window.setView(window.getDefaultView());

		sf::RectangleShape backgroundShape;//the translucent background
		backgroundShape.setFillColor(sf::Color(255, 0, 0, 100));
		backgroundShape.setSize(window.getView().getSize());

		//draw objects
		window.draw(backgroundShape);
		window.draw(_backgroundSprite);
	}
	bool Warehouse::update(sf::Time dt)
	{
		return false;
	}
	bool Warehouse::handleEvent(const sf::Event & event)
	{
		return false;
	}


