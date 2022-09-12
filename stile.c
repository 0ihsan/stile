#include "config.h"
#include "key.h"

#include <stdlib.h>

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

int _,dx,dy,dw,dh; // desktop with, desktop height (will be set in init())
int smallwidth;


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
	AXUIElementRef current_app = get_frontmost_app();
	AXUIElementRef current_win;
	AXValueRef tempforsize;
	CGSize winsiz;
	CGPoint winpos;

	AXUIElementCopyAttributeValue(current_app,
	                              kAXFocusedWindowAttribute,
	                              (CFTypeRef*)&current_win);

	if (center) {
		AXUIElementCopyAttributeValue(current_win,
		                              kAXSizeAttribute,
		                              (CFTypeRef*)&tempforsize);
		AXValueGetValue(tempforsize, kAXValueCGSizeType, &winsiz);
		winpos.x = (dx+dw-winsiz.width)/2;
		winpos.y = (dy*2+dh-winsiz.height)/2;
	} else {
		winpos.x = x;
		winpos.y = y;
		winsiz.width  = w;
		winsiz.height = h;
	}

	temp = AXValueCreate(kAXValueCGPointType, &winpos);
	AXUIElementSetAttributeValue(current_win, kAXPositionAttribute, temp);

	temp = AXValueCreate(kAXValueCGSizeType, &winsiz);
	AXUIElementSetAttributeValue(current_win, kAXSizeAttribute, temp);
	CFRelease(temp);

}


/* key event handler `return 0` means don't pass the shortcut to the rest */
static CGEventRef event_handler(CGEventTapProxy p, CGEventType t,
                                CGEventRef event, void* r) {
	int repeating = CGEventGetIntegerValueField(event,kCGKeyboardEventAutorepeat);
	int keycode   = CGEventGetIntegerValueField(event,kCGKeyboardEventKeycode);
	int modifiers = (int)CGEventGetFlags(event);

	modifiers &= (kCGEventFlagMaskShift   | kCGEventFlagMaskAlternate |
		           kCGEventFlagMaskCommand | kCGEventFlagMaskControl   |
		           kCGEventFlagMaskSecondaryFn);


	/* // UNCOMMENT TO ANALYSE KEY CODES (EVENT VIEWER)
	printf("is repeating: %d\n", repeating);
	printf("     keycode: %d\n", keycode);
	printf("   modifiers: %d\n\n", modifiers); */


	if (modifiers == MOD_CMD+MOD_CTRL+MOD_OPT+MOD_SHIFT)
		switch (keycode) {

			case (KEY_W):
				system("open -a Safari");
				return 0;

			case (KEY_RETURN):
				system("open -a Terminal");
				return 0;

			// center fixed small
			case (KEY_A):
				move_current_window(0,
				                    dx+(dw-smallwidth)/2, dy+(dh-smallwidth)/2,
				                    smallwidth,        smallwidth);
				return 0;

			// center medium (ratio at config.h)
			case (KEY_S):
				move_current_window(0,
				                    dx+(dw-(mediumheight*winratio))/2,
				                    dy+(dh-mediumheight)/2,
				                    mediumheight*winratio,
				                    mediumheight);
				return 0;

			// full screen w/gaps
			case (KEY_F):
				move_current_window(0,
				                    dx+gap,      dy+gap,
				                    dw-gap*2, dh-gap*2);
				return 0;

			// center (same size)
			case (KEY_C):
				move_current_window(1, 0,0,0,0); // 1:center
				return 0;

			// left half
			case (KEY_H):
				move_current_window(0,
				                    dx+gap,            dy+gap,
				                    (dw/2)-gap*1.5, dh-gap*2);
				return 0;

			// top left quarter
			case (KEY_Y):
				move_current_window(0, dx+gap,            dy+gap,
				                       (dw/2)-gap*1.5, (dh/2)-(gap*1.5));
				return 0;

			// bottom left quarter
			case (KEY_B):
				move_current_window(0,
				                    dx+gap,              dy+(dh/2)+(gap/2),
				                    (dw/2)-(gap*1.5),   (dh/2)-(gap*1.5));
				return 0;

			// right half
			case (KEY_L):
				move_current_window(0, dx+(dw/2)+(gap/2),   dy+gap,
				                       (dw/2)-(gap*1.5), dh-(gap*2));
				return 0;

			// top right quarter
			case (KEY_I):
				move_current_window(0, dx+(dw/2)+(gap/2),   dy+gap,
				                       (dw/2)-(gap*1.5), (dh/2)-(gap*1.5));
				return 0;

			// bottom right quarter
			case (KEY_M):
				move_current_window(0, dx+(dw/2)+(gap/2),   dy+(dh/2)+(gap/2),
				                       (dw/2)-(gap*1.5),   (dh/2)-(gap*1.5));
				return 0;

		}

	return event;
}

/* active desktop, excluding the dock. */
void
get_display_bounds(int* x, int* y, int* w, int* h)
{
	// get the cursor position
	CGEventRef event = CGEventCreate(NULL);
	CGPoint cursorLocation = CGEventGetLocation(event);
	CFRelease(event);

	// get display which contains the cursor, that's the one we want to tile on
	int numDisplays; CGDirectDisplayID displays[16];
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
	if (!(AXAPIEnabled() || AXIsProcessTrusted())) {
		AXUIElementCreateSystemWide();
		fprintf(stderr, "AXIsProcessTrusted returned false; try giving"
		                " accessibility permissions on\nPreferences ->"
		                " Security&Privacy -> Unlock -> Accessibility "
		                "-> + -> /pathto/stile\n");
		return 1;
	}
	get_display_bounds(&dx, &dy, &dw, &dh);

	/* width in KEY_A mode. perfectly selected for signcolumn plus 80 column
	 * ruler
	 *
	 * font@terminus               [17]: dw*0.4916
	 * font@firacode nerd font mono[14]: dw*0.52
	 *
	 */
	smallwidth = (dw*0.4916);

	return 0;
}


/* create and run eventloop (a keytap from applicationservices) */
int
cr_eventloop()
{
	CFRunLoopSourceRef rlsrc;
	CGEventMask em = CGEventMaskBit(
		kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged);

	CFMachPortRef et = CGEventTapCreate(
		kCGSessionEventTap, kCGHeadInsertEventTap, 0, em, event_handler, 0);
	if (!et) return 1; /* 1:failure, 0:success */

	rlsrc = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, et, 0);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), rlsrc, kCFRunLoopCommonModes);
	CGEventTapEnable(et, true);
	CFRunLoopRun();

	fprintf(stderr, "exiting...\n");
	return 0; // sucess
}

int
main()
{
	init();
	cr_eventloop();
}
