//
//  Client/Display/Application.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 14/09/11.
//  Copyright (c) 2011 Samuel Williams. All rights reserved.
//

#include "Application.h"
#include "../../Events/Thread.h"

namespace Dream {
	namespace Client {
		namespace Display {
			
#pragma mark -
#pragma mark Startup Message

			void _showStartupMessagesIfNeeded ()
			{
				static bool _startupMessagesDisplayed = false;
			
				if (_startupMessagesDisplayed == false) {
					_startupMessagesDisplayed = true;

					std::cout << "Dream Framework. Copyright © 2006-2011 Samuel Williams. All Rights Reserved." << std::endl;
					std::cout << "For more information visit http://www.oriontransfer.co.nz/research/dream" << std::endl;
					std::cout << "Build Revision: " << buildRevision() << std::endl;
					std::cout << "Compiled at: " << buildDate() << std::endl;
					std::cout << "Compiled on: " << buildPlatform() << std::endl;
				}
			}
			
#pragma mark -
			
			IApplicationDelegate::~IApplicationDelegate ()
			{
			}
					
			void IApplicationDelegate::applicationDidFinishLaunching (IApplication * application)
			{
				_showStartupMessagesIfNeeded();
			}
			
			void IApplicationDelegate::applicationWillTerminate (IApplication * application)
			{
			}
			
			void IApplicationDelegate::applicationWillEnterBackground (IApplication * application)
			{
			
			}
			
			void IApplicationDelegate::applicationDidEnterForeground (IApplication * application)
			{
			
			}
						
			IApplication::~IApplication ()
			{
			}
			
			
#pragma mark -

			class ApplicationDelegate : public Object, implements IApplicationDelegate
			{
				public:
					ApplicationDelegate(PTR(IScene) scene, PTR(Dictionary) config);
					virtual ~ApplicationDelegate();
					
				protected:
					REF(IScene) m_scene;
					REF(Dictionary) m_config;
					REF(Context) m_context;
					REF(Events::Thread) m_thread;
					REF(SceneManager) m_sceneManager;
					
					virtual void applicationDidFinishLaunching (IApplication * application);
					
					virtual void applicationWillEnterBackground (IApplication * application);
					virtual void applicationDidEnterForeground (IApplication * application);
			};

			ApplicationDelegate::ApplicationDelegate(PTR(IScene) scene, PTR(Dictionary) config)
				: m_scene(scene), m_config(config)
			{
			
			}
			
			ApplicationDelegate::~ApplicationDelegate()
			{
				m_context->stop();
				m_thread->stop();
			}
			
			void ApplicationDelegate::applicationDidFinishLaunching (IApplication * application)
			{
				m_context = application->createContext(m_config);
				
				m_thread = new Events::Thread;
				REF(ILoader) loader = SceneManager::defaultResourceLoader();
				
				m_sceneManager = new SceneManager(m_context, m_thread->loop(), loader);
				m_sceneManager->pushScene(m_scene);
				
				m_thread->start();
				m_context->start();
			}
			
			void ApplicationDelegate::applicationWillEnterBackground (IApplication * application)
			{
				std::cerr << "Application entering background..." << std::endl;
				
				EventInput suspendEvent(EventInput::PAUSE);
				m_sceneManager->processInput(m_context, suspendEvent);
				
				//m_context->stop();
				//m_thread->stop();
			}
			
			void ApplicationDelegate::applicationDidEnterForeground (IApplication * application)
			{
				std::cerr << "Application entering foreground..." << std::endl;
				
				//m_thread->start();
				//m_context->start();
				
				EventInput resumeEvent(EventInput::RESUME);
				m_sceneManager->processInput(m_context, resumeEvent);
			}
			
			void IApplication::runScene(PTR(IScene) scene, PTR(Dictionary) config)
			{
				REF(ApplicationDelegate) applicationDelegate = new ApplicationDelegate(scene, config);
				
				IApplication::start(applicationDelegate);
			}
			
		}
	}
}
