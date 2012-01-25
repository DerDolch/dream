//
//  Client/Display/Cocoa/DWindowDelegate.mm
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 1/03/11.
//  Copyright (c) 2011 Samuel Williams. All rights reserved.
//

#import "DWindowDelegate.h"

@implementation DWindowDelegate

@synthesize displayContext = _window_context;

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
	using namespace Dream::Client::Display;

	if (_window_context) {
		EventInput ipt(EventInput::EXIT);
		
		_window_context->process(ipt);				
	}
	
	return NSTerminateCancel;
}

- (BOOL)windowShouldClose:(id)sender
{
	using namespace Dream::Client::Display;
	
	if (_window_context) {
		EventInput ipt(EventInput::EXIT);
		
		_window_context->process(ipt);
	}
	
	return NO;
}

- (void)windowWillEnterFullScreen:(NSNotification *)notification {
	NSLog(@"windowWillEnterFullScreen");
	_window_context->stop();
}

- (void)windowDidEnterFullScreen:(NSNotification *)notification {
	NSLog(@"windowDidEnterFullScreen");
	_window_context->start();
}

- (void)windowWillExitFullScreen:(NSNotification *)notification {
	NSLog(@"windowWillExitFullScreen");
	_window_context->stop();
}

- (void)windowDidExitFullScreen:(NSNotification *)notification {
	NSLog(@"windowDidExitFullScreen");
	_window_context->start();
}

@end
