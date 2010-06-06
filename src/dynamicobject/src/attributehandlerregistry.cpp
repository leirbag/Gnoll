/***************************************************************************
 *   Copyright (C) 2006 by Puzzle Team                                     *
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

#include "../include/attributehandlerregistry.h"

#include "../include/genericattributehandler.h"
#include "../include/inheritsattributehandler.h"
#include "../include/integer.h"
#include "../include/float.h"
#include "../include/string.h"
#include "../include/double.h"
#include "../include/vector3.h"
#include "../include/list.h"
#include "../include/set.h"
#include "../include/inherits.h"
#include "../../scene/include/cmeshpagerenderer.h"
#include "../../scene/include/cstaticgobject.h"
#include "../../scene/include/gobject.h"
#include "../../scene/include/cogremeshcomponent.h"
#include "../../scene/include/cogreanimatedmeshcomponent.h"
#include "../../scene/include/cpositioncomponent.h"
#include "../../scene/include/cscalingcomponent.h"
#include "../../scene/include/crotationcomponent.h"

using namespace Gnoll::Scene;

namespace Gnoll
{
	namespace DynamicObject
	{
		typedef shared_ptr<AbstractAttributeHandler> AbstractAttributeHandler_ptr;

		AttributeHandlerRegistry::AttributeHandlerRegistry()
		{
			// Default handlers are added here
			this->registerHandler(Vector3::DYNAMIC_OBJECT_NAME(),  
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<Vector3>()));
			this->registerHandler(Integer::DYNAMIC_OBJECT_NAME(),  
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<Integer>()));
			this->registerHandler(Float::DYNAMIC_OBJECT_NAME(),    
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<Float>()));
			this->registerHandler(String::DYNAMIC_OBJECT_NAME(),   
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<String>()));
			this->registerHandler(Double::DYNAMIC_OBJECT_NAME(),   
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<Double>()));
			this->registerHandler(List::DYNAMIC_OBJECT_NAME(),     
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<List>()));
			this->registerHandler(Set::DYNAMIC_OBJECT_NAME(),      
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<Set>()));
			this->registerHandler(Inherits::DYNAMIC_OBJECT_NAME(), 
					AbstractAttributeHandler_ptr(new InheritsAttributeHandler()));
			this->registerHandler(Gnoll::Scene::CMeshPageRenderer::DYNAMIC_OBJECT_NAME(), 
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<CMeshPageRenderer>()));
			this->registerHandler(Gnoll::Scene::CStaticGObject::DYNAMIC_OBJECT_NAME(),     
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<CStaticGObject>()));
			this->registerHandler(Gnoll::Scene::GObject::DYNAMIC_OBJECT_NAME() ,           
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<GObject>()));
			this->registerHandler(Gnoll::Scene::COgreMeshComponent::DYNAMIC_OBJECT_NAME(), 
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<COgreMeshComponent>()));
			this->registerHandler(Gnoll::Scene::COgreAnimatedMeshComponent::DYNAMIC_OBJECT_NAME(), 
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<COgreAnimatedMeshComponent>()));
			this->registerHandler(Gnoll::Scene::CPositionComponent::DYNAMIC_OBJECT_NAME(),
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<CPositionComponent>()));
			this->registerHandler(Gnoll::Scene::CScalingComponent::DYNAMIC_OBJECT_NAME(), 
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<CScalingComponent>()));
			this->registerHandler(Gnoll::Scene::CRotationComponent::DYNAMIC_OBJECT_NAME(),
					AbstractAttributeHandler_ptr(new GenericAttributeHandler<CRotationComponent>()));
		}

		AttributeHandlerRegistry::~AttributeHandlerRegistry()
		{
		}

		void AttributeHandlerRegistry::registerHandler(string attributeType, AbstractAttributeHandler_ptr handler)
		{
			m_handlersMap[attributeType] = handler;
		}

		void AttributeHandlerRegistry::unregisterHandler(string attributeType)
		{
			m_handlersMap.erase(attributeType);
		}

		AbstractAttributeHandler_ptr AttributeHandlerRegistry::getHandler(string attributeType)
		{
			return m_handlersMap[attributeType];
		}
	}
}
