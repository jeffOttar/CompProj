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
#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <memory>
#include <algorithm>
#include <set>

#include "Category.h"


namespace GEX 
{
	//forward declaration
	struct Command;
	class CommandQueue;

	class SceneNode : public sf::Transformable, public sf::Drawable
	{
	public:
		using Ptr = std::unique_ptr<SceneNode>;//a unique ptr of scene nodes
		using Pair = std::pair<SceneNode*, SceneNode*>;

	public:
								SceneNode(Category::Type category = Category::Type::None);
		virtual					~SceneNode() = default;

								SceneNode(const SceneNode&) = delete;//cant be copied with CopyCtr or Copy Assignment Operator
		SceneNode&				operator=(SceneNode&) = delete;

		void					attachChild(Ptr child);
		Ptr						detachChild(const SceneNode& ptr);//detach a child and return the unique ptr of that child
		
		void					update(sf::Time dt, CommandQueue& commands);//when calling update pass a delta time, update calls the updates
		void					onCommand(const Command& command, sf::Time dt);
		virtual unsigned int	getCategory() const;

		sf::Vector2f			getWorldPosition() const;
		sf::Transform			getWorldTransform() const;

		virtual sf::FloatRect	getBoundingBox() const;
		void					drawBoundingBox(sf::RenderTarget& target, sf::RenderStates states) const;

		void					checkSceneCollision(SceneNode& rootNode, std::set<Pair>& collisionPair);
		void					checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPair);

		virtual bool			isDestroyed() const;
		virtual bool			isMarkedForRemoval() const;
		void					removeWrecks();

	protected:
		//update the tree
		virtual	void			updateCurrent(sf::Time dt, CommandQueue& commands);
		void					updateChildren(sf::Time dt, CommandQueue& commands);

		//draw the tree
		virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;//overwriting the draw from the sf::drawable
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		void					drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		SceneNode*				_parent;
		std::vector<Ptr>		_children;
		Category::Type			_category;
	};
	float distance(const SceneNode& lhs, const SceneNode& rhs);//this is a free function
	bool collision(const SceneNode& lhs, const SceneNode& rhs);
}