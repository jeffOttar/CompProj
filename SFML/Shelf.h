/**
* @file
* Shelf.h
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

//
//

#pragma once
#include "Entity.h"
#include "TextureManager.h"
#include "Animation.h"
#include "Item.h"

namespace GEX {

	class Animation2;
	class Shelf: public Entity
	{

	public:
		enum class ShelfType{Shelf};

		enum class ShelfState {
			empty,
			occupied,
		};

	public:
		explicit		Shelf(ShelfType type,  TextureManager& textures);

		
		unsigned int	getCategory() const override;

		sf::FloatRect	getBoundingBox() const override;

		bool			isMarkedForRemoval()const override;

		void			setState(ShelfState state);

		bool			isOccupied() const;
		void			setOccupied(bool occupy);
		void			setItemOnShelf(Item item);
		Item			removeItemOnShelf();
		Item			getItem();



	private:
		void			updateStates();
		void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	protected:

		void			updateCurrent(sf::Time dt, CommandQueue& commands) override;

	private:
		ShelfType								type_;
		ShelfState								state_;
		std::map<ShelfState, Animation2>		animations_;
		mutable sf::Sprite						sprite_;
		sf::Sprite								backgroundSprite_;
	
		bool									isMarkedForRemoval_;
		bool									occupied_;
		Item									_item;
		TextureManager*							_textures;
	};

}


