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


/*--------------------------CMeshPageRenderer------------------------------*\
|   This is a page renderer for pages made of a mesh                        |
|                                                                           |
|   Changelog :                                                             |
|               12/04/2007 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/



#include "../include/cmeshpagerenderer.h"
#include "../../graphic/include/cgraphicmodule.h"


using namespace std;
using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::Graphic;


namespace Gnoll
{
	namespace Scene
	{

		
		CMeshPageRenderer::CMeshPageRenderer()
		{
		}

				
		CMeshPageRenderer::~CMeshPageRenderer()
		{
		}
				

		void CMeshPageRenderer::init( CPage*  _parentPage )
		{
			m_parentPage = _parentPage;
			SceneNode *parentNode = _parentPage->getPageRootNode();

			SceneNode * pageNode = parentNode->createChildSceneNode( _parentPage->getInstance() + "_renderer" );

			std::string entName = _parentPage->getInstance() + "_entity";
			Ogre::SceneManager* sm = CGraphicModule::getInstancePtr()->getSceneManager();
			
			Ogre::Entity *ent = sm->createEntity( entName, m_meshName );
			pageNode->attachObject( ent );
			
		}
		
				
		void CMeshPageRenderer::update()
		{
		}
		
				
		void CMeshPageRenderer::exit()
		{
			std::string entName = m_parentPage->getInstance() + "_entity";
			Ogre::SceneManager* sm = CGraphicModule::getInstancePtr()->getSceneManager();
			
			SceneNode* pageNode = sm->getSceneNode( m_parentPage->getInstance() + "_renderer" );
			pageNode->detachObject( entName );

			sm->destroyEntity(entName);
			sm->destroySceneNode( m_parentPage->getInstance() );
				
		
		}
		 
		 
		shared_ptr<xmlpp::Document> CMeshPageRenderer::serializeXML()
		{
			
			// First a new document is created
			shared_ptr<xmlpp::Document> document( new xmlpp::Document("1.0"));  

			xmlpp::Element* root = document->create_root_node("CMeshPageRenderer");
			root->set_attribute("meshName", m_meshName);
	

			return document;
		}
		

		void CMeshPageRenderer::deSerializeXML( xmlpp::Element* _element )
		{
		
			if (_element == NULL)
			{
				return;
			}

			xmlpp::Attribute* attr = _element->get_attribute("meshName");

			if (attr == NULL)
			{
				return;
			}

			m_meshName = attr->get_value();
		}

	}
}


