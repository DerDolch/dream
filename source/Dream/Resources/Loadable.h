//
//  Resources/Loadable.h
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 4/05/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#ifndef _DREAM_RESOURCES_LOADABLE_H
#define _DREAM_RESOURCES_LOADABLE_H

#include "../Framework.h"
#include "../Core/Data.h"
#include "../Core/Strings.h"

#include <vector>
#include <exception>

namespace Dream {
	namespace Resources {
		using namespace Dream::Core;
		/*
		 The resource loader system ideally has two types of resources to load:
		 - Files which correspond directly to resources
		 - Files which index sets of resources

		 Files that directly correspond to resources shouldn't directly load classes
		 that equate to the resource. Generally, you should have a ResourceFactory
		 that contains the required information, and can build instances of the
		 required data. You can inject the resource loader into the factory so
		 that it is available to load further resources as needed.

		 In the case of files which index a set of resources, you should create a
		 MyDataTypeLibrary which has a function:
		    Ref<MyDataTypeFactory> find_resource (StringT name);
		 This factory can then be used to create corresponding resources.
		 */

		class ILoader;

		class LoadError : public std::exception {
		protected:
			StringT _what;
		public:
			LoadError (StringT what);
			virtual ~LoadError () throw ();

			virtual const char * what() const noexcept;
		};

		class ILoadable : implements IObject {
		public:
			virtual void register_loader_types (ILoader * loader) abstract;
			virtual Ref<Object> load_from_data (const Ptr<IData> data, const ILoader * loader) abstract;
		};

		class ILoadableFromStream : implements ILoadable {
		public:
			virtual Ref<Object> load_from_data (const Ptr<IData> data, const ILoader * loader);
			virtual Ref<Object> load_from_stream (std::istream& input, const ILoader * loader) abstract;
		};
	}
}

#endif
