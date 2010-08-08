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


/*-----------------------------IPageRenderer-------------------------------*\
|   This is a page renderer interface                                       |
|                                                                           |
|   Changelog :                                                             |
|               12/04/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/


#ifndef __IPAGERENDERER_H__
#define __IPAGERENDERER_H__


#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>
#include <glibmm/ustring.h>


#include "../../../dynamicobject/include/abstractattribute.h"

#include <string.h>
#include "cpage.h"

using namespace std;
using namespace boost;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Scene
	{

		class IPageRenderer : public AbstractAttribute
		{
			
				
			public:
			
			
				/**
				 * This is a constructor
				 */
				IPageRenderer() {}
				
				
				/**
				 * This is a destructor
				 */
				virtual ~IPageRenderer() {}
		
				
				/**
				 * Page Renderer initialization
				 */
				virtual void init( CPage*  _parentPage ) {};
				
				/**
				 * Update method
				 */		
				virtual void update() {};
		
		
				/**
				 * Page Renderer exits
				 */
				 virtual void exit() {};
				
				/**
				 * This method serialize the object. <br/>
				 * It has to be implemented by all classes that inherits from this class.
				 *
				 * @return This return the object as a XML tree 
				 */
				virtual shared_ptr<xmlpp::Document> serializeXML()
			  	{
					shared_ptr<xmlpp::Document> document( new xmlpp::Document("1.0"));  

					return document;
				};		


				/**
				 * This method deserialize the object. <br/>
				 * This method initializes this object thanks to a XML tree given as a parameter. <br/>
				 * It has to be implemented by all classes that inherits from this class.
				 *
				 * @param _element This is the XML tree containing the state of this object
				 */
				virtual void deSerializeXML( xmlpp::Element* _element ) {};		

		};
	}
}

#endif // __IPAGERENDERER_H__
