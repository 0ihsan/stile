#include "config.h"
#include "key.h"
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

int _,dw,dh; // desktop with, desktop height (will be set in init())


/* active desktop, excluding the dock. */
void
get_display_bounds(int* x, int* y, int* w, int* h)
{
	// get the cursor position
	CGEventRef event = CGEventCreate(NULL);
	CGPoint cursorLocation = CGEventGetLocation(event);
	CFRelease(event);

	// get the display which contains the cursor, that's the one we want to tile
	// on
	int numDisplays;
	CGDirectDisplayID displays[16];
	CGGetDisplaysWithPoint(cursorLocation, 16, displays, &numDisplays);

	HIRect bounds;
	HIWindowGetAvailablePositioningBounds(displays[0], kHICoordSpace72DPIGlobal,
		&bounds);

	*x = bounds.origin.x;
	*y = bounds.origin.y;
	*w = bounds.size.width;
	*h = bounds.size.height;
}


int
init()
{
	// is accessibility API autharized?
	if (!(AXAPIEnabled() || AXIsProcessTrusted())) {
		fprintf(stderr, "AXIsProcessTrusted returned false; does darwintiler have "
		                "accessibility API permissions?\n");
		return 1;
	}
	get_display_bounds(&_, &_, &dw, &dh);
	return 0;
}


static AXUIElementRef
get_frontmost_app()
{
	pid_t pid; ProcessSerialNumber psn;
	GetFrontProcess(&psn); GetProcessPID(&psn, &pid);
	return AXUIElementCreateApplication(pid);
}


void
move_current_window(int center, int x, int y, int w, int h)
{
	AXValueRef temp;
	AXUIElementRef frontMostApp;
	AXUIElementRef frontMostWindow;
	CGSize windowSize;
	CGPoint windowPosition;
	windowPosition.x  = x;
	windowPosition.y  = y;
	windowSize.width  = w;
	windowSize.height = h;

	frontMostApp = get_frontmost_app();
	AXUIElementCopyAttributeValue(
		frontMostApp, kAXFocusedWindowAttribute, (CFTypeRef *)&frontMostWindow);

	if (center) {

	}

	temp = AXValueCreate(kAXValueCGPointType, &windowPosition);
	AXUIElementSetAttributeValue(frontMostWindow, kAXPositionAttribute, temp);
	CFRelease(temp);

	temp = AXValueCreate(kAXValueCGSizeType, &windowSize);
	AXUIElementSetAttributeValue(frontMostWindow, kAXSizeAttribute, temp);
	CFRelease(temp);
}


/* key event handler `return 0` means don't pass the shortcut to the rest */
static CGEventRef event_handler(CGEventTapProxy p, CGEventType t, CGEventRef event, void* r) {
	int repeating = CGEventGetIntegerValueField(event,kCGKeyboardEventAutorepeat);
	int keycode   = CGEventGetIntegerValueField(event,kCGKeyboardEventKeycode);
	int modifiers = (int)CGEventGetFlags(event);

	modifiers &= (kCGEventFlagMaskShift   | kCGEventFlagMaskAlternate |
		           kCGEventFlagMaskCommand | kCGEventFlagMaskControl   |
		           kCGEventFlagMaskSecondaryFn);


	/* // ANALYSING KEY CODES (EVENT VIEWER)
	printf("is repeating: %d\n", repeating);
	printf("keycode: %d\n", keycode);
	printf("modifiers: (%d)\n\n", modifiers); */
	if (modifiers == MOD_CMD+MOD_CTRL+MOD_OPT+MOD_SHIFT)
		switch (keycode) {
			case (KEY_S):
				move_current_window(0, (dw-(winsize*winratio))/2,
				            (dh-winsize)/2,
	                     winsize*winratio,
	                     winsize);
				return 0; // don't do anything else with this shortcut.

			case (KEY_F):
				printf("", get_frontmost_app());
				move_current_window(0, gap, gap, dw-(gap*2), dh-(gap*2));
				return 0;

			case (KEY_C):
				printf("", get_frontmost_app());
				move_current_window(1, 0,0,0,0); // 1:center
				return 0;
		}

	return event;
}

/* create and run eventloop (a keytap from applicationservices) */
int cr_eventloop() {
	CFRunLoopSourceRef rlsrc; // run loop source
	CGEventMask em = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged);
	CFMachPortRef et = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0, em, event_handler, 0);
	if (!et) return 1; /* 1:failure, 0:success */
	rlsrc = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, et, 0);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), rlsrc, kCFRunLoopCommonModes);
	CGEventTapEnable(et, true);
	CFRunLoopRun();
	return 0; // sucess
}

int
main()
{
	init();
	cr_eventloop();
}
