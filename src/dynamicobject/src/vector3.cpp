/***************************************************************************
 *   Copyright (C) 2007 by Paf                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "../include/vector3.h"

#include <string>

#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace std;

namespace Gnoll
{
	namespace DynamicObject
	{
		Vector3::Vector3(const Ogre::Vector3& vec) : Ogre::Vector3(vec)
		{
		}

		Vector3::~Vector3()
		{
		}

		shared_ptr<xmlpp::Document> Vector3::serializeXML()
		{
			shared_ptr<xmlpp::Document> document( new xmlpp::Document("1.0"));
			xmlpp::Element* root = document->create_root_node( Vector3::DYNAMIC_OBJECT_NAME() );

			string finalString = lexical_cast<string> (x);
			root->set_attribute(Vector3::ATTRIBUTE_X(), finalString);

			finalString = lexical_cast<string> (y);
			root->set_attribute(Vector3::ATTRIBUTE_Y(), finalString);

			finalString = lexical_cast<string> (z);
			root->set_attribute(Vector3::ATTRIBUTE_Z(), finalString);

			return document;
		}

		void Vector3::deSerializeXML(xmlpp::Element* element)
		{
			if (element == NULL)
				return;

			xmlpp::Attribute* attrX = element->get_attribute(Vector3::ATTRIBUTE_X());
			xmlpp::Attribute* attrY = element->get_attribute(Vector3::ATTRIBUTE_Y());
			xmlpp::Attribute* attrZ = element->get_attribute(Vector3::ATTRIBUTE_Z());

			if (attrX == NULL || attrY == NULL || attrZ == NULL)
				return;

			x = lexical_cast<float>(attrX->get_value());
			y = lexical_cast<float>(attrY->get_value());
			z = lexical_cast<float>(attrZ->get_value());
		}
	}
}
