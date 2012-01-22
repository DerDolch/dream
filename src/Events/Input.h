/*
 *  Client/Display/Input.h
 *  This file is part of the "Dream" project, and is licensed under the GNU GPLv3.
 *
 *  Created by Samuel Williams on 16/10/06.
 *  Copyright 2006 Samuel G. D. Williams. All rights reserved.
 *
 */

#ifndef _DREAM_CLIENT_DISPLAY_INPUT_H
#define _DREAM_CLIENT_DISPLAY_INPUT_H

#include "Thread.h"
#include "../Core/Timer.h"
#include "../Numerics/Vector.h"
#include "../Geometry/AlignedBox.h"

#include <map>

namespace Dream
{
	namespace Events
	{
	
		using namespace Dream::Core;
		using namespace Dream::Numerics;
		using namespace Dream::Geometry;
		
		typedef unsigned DeviceT;
		typedef unsigned ButtonT;
		typedef unsigned StateT;
		
		enum Device {
			NullDevice = 0,
			DefaultKeyboard = 1,
			DefaultMouse = 2,
			DefaultTouchPad = 3,
			AnyDevice = 0xFFFF
		};
		
		enum State {
			Released = 0,
			Pressed = 1,
			Dragged = 2,
		};
		
		// If a mouse button is released after being pressed or dragged, it will
		// create a motion key with State::Released and MouseButton::Mouse*Button
		// If a mouse is just dragged around with no button, it will generate similar
		// keys but with Mouse
		
		enum MouseButton {
			NullButton = 0,
			MouseLeftButton = 1,
			MouseMiddleButton = 2,
			MouseRightButton = 3,
			
			/// Trackpad / Scroll Wheel input
			MouseScroll = 0xF1,
			
			/// Window Events
			MouseEntered = 0xF2,
			MouseExited = 0xF3
		};
		
		/* Simple encapsulation of a device and its button */
		class Key {
		protected:
			DeviceT m_device;
			ButtonT m_button;
			
		public:
			const DeviceT& device () const { return m_device; }
			const ButtonT& button () const { return m_button; }
			
			bool operator< (const Key& other) const {
				return m_device < other.m_device && m_button < other.m_button;
			}
			
			bool operator<= (const Key& other) const {
				return m_device <= other.m_device && m_button <= other.m_button;
			}
			
			bool operator> (const Key& other) const {
				return m_device > other.m_device && m_button > other.m_button;
			}
			
			bool operator>= (const Key& other) const {
				return m_device >= other.m_device && m_button >= other.m_button;
			}
			
			bool operator== (const Key& other) const {
				return m_device == other.m_device && m_button == other.m_button;
			}
			
			bool operator!= (const Key& other) const {
				return ! ((*this) == other);
			}
			
			Key (const DeviceT &device, const ButtonT &button) : m_device(device), m_button(button) {
				/* Simple Constructor */
			}
		};
		
		class ResizeInput;
		class ButtonInput;
		class MotionInput;
		class EventInput;
		class IInputHandler;
		
		class Input
		{
		protected:
			TimeT m_time;
			
		public:
			Input ();
			Input (const Input & other);
			
			virtual ~Input();
			
			TimeT time () const { return m_time; }
			virtual bool act (IInputHandler * handler) const abstract;
		};
		
		class IInputHandler
		{
		protected:
			friend class ResizeInput;
			virtual bool resize(const ResizeInput &) { return false; }
			
			friend class ButtonInput;
			virtual bool button(const ButtonInput &) { return false; }
			
			friend class MotionInput;
			virtual bool motion(const MotionInput &) { return false; }
			
			friend class EventInput;
			virtual bool event(const EventInput &) { return false; }
			
		public:
			virtual ~IInputHandler ();
			virtual bool process (const Input & input);
		};
		
		class EventInput : public Input {
		public:
			enum EventName {
				EXIT = 1,
				PAUSE = 2,
				RESUME = 3,
			};
			
			EventInput(EventName event);
			EventInput(const EventInput & other);
			
			virtual ~EventInput ();
			
			EventName event() const;
		
		protected:
			EventName m_event;
			
			virtual bool act (IInputHandler * handler) const;
		};
		
		class ButtonInput : public Input {
		private:
			Key m_key;
			StateT m_state;
			
		public:
			ButtonInput(const Key &e, const StateT &s);
			ButtonInput(const ButtonInput & other);
			
			virtual ~ButtonInput ();
			
			virtual bool act (IInputHandler * handler) const;
			
			const Key & key () const { return m_key; }
			const StateT & state () const { return m_state; }
			
			bool buttonPressed (const char b) const {
				return m_state == Pressed && m_key.button() == b;
			}
			
			bool buttonReleased (const char b) const {
				return m_state == Released && m_key.button() == b;
			}
		};
		
		class MotionInput : public Input {
		private:
			Key m_key;
			StateT m_state;
			Vec3 m_position, m_motion;
			
			// The bounds of the view/screen which handled the motion input.
			AlignedBox<2> m_bounds;
			
		public:
			MotionInput(const Key &key, const StateT &state, const Vec3 &position, const Vec3 &motion, const AlignedBox<2> & bounds);
			MotionInput(const MotionInput & other);
			
			virtual ~MotionInput ();
			
			virtual bool act (IInputHandler * handler) const;
			
			const Vec3 & currentPosition () const { return m_position; }
			const Vec3 previousPosition () const { return m_position - m_motion; }
			const Vec3 motion () const { return m_motion; }
			
			const AlignedBox<2> & bounds() const { return m_bounds; }
			
			// Updated bounds must be within the coordinate system provided by the current input event.
			MotionInput inputByRefiningBounds(const AlignedBox<2> & updatedBounds);
			
			const Key & key () const { return m_key; }
			const StateT & state () const { return m_state; }
			
			bool buttonPressed(MouseButton b) const {
				return m_state == Pressed && m_key.button() == b;
			}
			
			bool buttonPressedOrDragged(MouseButton b) const {
				return (m_state == Pressed || m_state == Dragged) && m_key.button() == b;
			}
			
			bool buttonReleased(MouseButton b) const {
				return m_state == Released && m_key.button() == b;
			}
		};
		
		class ResizeInput : public Input {
		private:
			Vec2u m_newSize;
			
		public:
			ResizeInput(const Vec2u & newSize);
			ResizeInput(const ResizeInput & other);
			
			virtual ~ResizeInput ();
			
			virtual bool act (IInputHandler * handler) const;
			
			Vec2u newSize () const { return m_newSize; }
		};
		
		/// The input queue can send user input across threads.
		class InputQueue : implements IInputHandler {
			protected:
				Events::Queue<Input*> m_queue;
				
				virtual bool resize(const ResizeInput &);
				virtual bool button(const ButtonInput &);
				virtual bool motion(const MotionInput &);
				virtual bool event(const EventInput &);
				
			public:
				virtual ~InputQueue ();
									
				void dequeue (IInputHandler * handler);
		};
		
		template <typename ActionT>
		class InputMapper {
		protected:
			typedef std::map<Key, ActionT> ActionsMap;
			ActionsMap m_actions;
			
		public:
			void bind (const Key &e, ActionT a) {
				m_actions[e] = a;
			}
			
			void bind (DeviceT d, ButtonT b, ActionT a) {
				Key e(d,b);
				m_actions[e] = a;
			}
			
			bool lookup (const Key &e, const ActionT &a) const {
				typename ActionsMap::const_iterator i = m_actions.find(e);
				if (i != m_actions.end()) {
					a = *i;
					return true;
				} else {
					return false;
				}
			}
		};
	}
}

#endif