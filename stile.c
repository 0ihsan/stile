#include <stdlib.h>
#include <Carbon/Carbon.h>
#include <ApplicationServices/ApplicationServices.h>

/* observed based on "BRITISH - PC" keyboard layout */
#define MOD_CMD 1048576
#define MOD_CTRL 262144
#define MOD_FN 8388608
#define MOD_OPT 524288
#define MOD_SHIFT 131072
#define KEY_A 0
#define KEY_B 11
#define KEY_C 8
#define KEY_D 2
#define KEY_E 14
#define KEY_F 3
#define KEY_G 5
#define KEY_H 4
#define KEY_I 34
#define KEY_J 38
#define KEY_K 40
#define KEY_L 37
#define KEY_M 46
#define KEY_N 45
#define KEY_O 31
#define KEY_P 35
#define KEY_Q 12
#define KEY_R 15
#define KEY_S 1
#define KEY_T 17
#define KEY_U 32
#define KEY_V 9
#define KEY_W 13
#define KEY_X 7
#define KEY_Y 16
#define KEY_Z 6
#define KEY_0 29
#define KEY_1 18
#define KEY_2 19
#define KEY_3 20
#define KEY_4 21
#define KEY_5 23
#define KEY_6 22
#define KEY_7 26
#define KEY_8 28
#define KEY_9 25
#define KEY_EQUALS 24
#define KEY_MINUS_OR_DASH 27
#define KEY_BRACKET_CLOSE 30
#define KEY_BRACKET_OPEN 33
#define KEY_RETURN 36
#define KEY_SINGLE_QUOTE 39
#define KEY_SEMI_COLON 41
#define KEY_SLASH_BACK 42
#define KEY_COMMA 43
#define KEY_SLASH_FORWARD 44
#define KEY_DOT 47
#define KEY_TAB 48
#define KEY_SPACE 49
#define KEY_GRAVE_ACCENT 50
#define KEY_DELETE 51
#define KEY_ENTER 52
#define KEY_ESCAPE 53
#define KEY_LEFT_CTRL 59
#define KEY_LEFT_OPTION 58
#define KEY_LEFT_CMD 55
#define KEY_RIGHT_CMD 54
#define KEY_RIGHT_OPTION 61
#define KEY_STAR_ASTERISK 67
#define KEY_PLUS 69
#define KEY_CLEAR 71
#define KEY_NUMPAD_SLASH_FORWARD 75
#define KEY_NUMPAD_ENTER 76  // numberpad on full keyboard
#define KEY_F5 96
#define KEY_F6 97
#define KEY_F7 98
#define KEY_F3 99
#define KEY_F8 100
#define KEY_F9 101
#define KEY_F11 103
#define KEY_F13 105
#define KEY_F14 107
#define KEY_F10 109
#define KEY_F12 111
#define KEY_F15 113
#define KEY_HELP 114
#define KEY_HOME 115
#define KEY_PGUP 116
// #define KEY_DELETE 117
#define KEY_F4 118
#define KEY_END 119
#define KEY_F2 120
#define KEY_PAGEDOWN 121
#define KEY_F1 122
#define KEY_LEFT 123
#define KEY_RIGHT 124
#define KEY_DOWN 125
#define KEY_UP 126

_,dx,dy,dw,dh,smallwidth,gap=84,mediumheight=714; float winratio=3.8/2.5;

static AXUIElementRef get_frontmost_app() {
	pid_t pid; ProcessSerialNumber psn; GetFrontProcess(&psn); GetProcessPID(&psn, &pid);
	return AXUIElementCreateApplication(pid);
}

void move_current_window(center, x, y, w, h) {
	CGSize winsiz; CGPoint winpos; AXValueRef tmp, tmpsize;
	AXUIElementRef currwin,currapp=get_frontmost_app();
	AXUIElementCopyAttributeValue(currapp, kAXFocusedWindowAttribute, (CFTypeRef*)&currwin);
	if (center) {
		AXUIElementCopyAttributeValue(currwin, kAXSizeAttribute, (CFTypeRef*)&tmpsize);
		AXValueGetValue(tmpsize, kAXValueCGSizeType, &winsiz);
		winpos.x = (dx+dw-winsiz.width)/2; winpos.y = (dy*2+dh-winsiz.height)/2;
	} else { winpos.x=x; winpos.y=y; winsiz.width=w; winsiz.height=h; }
	tmp = AXValueCreate(kAXValueCGPointType, &winpos);
	AXUIElementSetAttributeValue(currwin, kAXPositionAttribute, tmp);
	tmp = AXValueCreate(kAXValueCGSizeType, &winsiz);
	AXUIElementSetAttributeValue(currwin, kAXSizeAttribute, tmp);
	CFRelease(tmp);
}

void center() {move_current_window(1, 0,0,0,0);}
void center_resize0() { move_current_window(0, dx+(dw-smallwidth)/2, dy+(dh-smallwidth)/2, smallwidth, smallwidth); }
void center_resize1() /* (ratio at config.h) */ {move_current_window(0, dx+(dw-(mediumheight*winratio))/2, dy+(dh-mediumheight)/2, mediumheight*winratio, mediumheight);}
void full_screen_w_gaps() {move_current_window(0, dx+gap, dy+gap, dw-gap*2, dh-gap*2);}
void left_half() {move_current_window(0, dx+gap, dy+gap, (dw/2)-gap*1.5, dh-gap*2);}
void top_left_quarter() {move_current_window(0, dx+gap, dy+gap, (dw/2)-gap*1.5, (dh/2)-(gap*1.5));}
void bottom_left_quarter() {move_current_window(0, dx+gap, dy+(dh/2)+(gap/2), (dw/2)-(gap*1.5), (dh/2)-(gap*1.5));}
void right_half() {move_current_window(0, dx+(dw/2)+(gap/2), dy+gap, (dw/2)-(gap*1.5), dh-(gap*2));}
void top_right_quarter() {move_current_window(0, dx+(dw/2)+(gap/2), dy+gap, (dw/2)-(gap*1.5), (dh/2)-(gap*1.5));}
void bottom_right_quarter() {move_current_window(0, dx+(dw/2)+(gap/2), dy+(dh/2)+(gap/2), (dw/2)-(gap*1.5), (dh/2)-(gap*1.5));}

/* active desktop, excluding the dock. */
void get_display_bounds(int* x, int* y, int* w, int* h) {
	CGEventRef e = CGEventCreate(NULL);
	CGPoint curloc = CGEventGetLocation(e); CFRelease(e);
	// get display which contains the cursor, that's to be tiled
	int numDisplays; CGDirectDisplayID displays[16];
	CGGetDisplaysWithPoint(curloc, 16, displays, &numDisplays);
	HIRect bounds; HIWindowGetAvailablePositioningBounds(displays[0], kHICoordSpace72DPIGlobal, &bounds);
	*x=bounds.origin.x, *y=bounds.origin.y, *w=bounds.size.width, *h=bounds.size.height;
}

init() {
	if (!(AXAPIEnabled()||AXIsProcessTrusted())) {
		AXUIElementCreateSystemWide();
		fprintf(stderr, "AXIsProcessTrusted returned false; try giving accessibility permissions on\n"
		                "System Settings -> Privacy & Security -> Accessibility -> + -> /pathto/stile\n");
		return 1;}
	get_display_bounds(&dx, &dy, &dw, &dh); smallwidth=(dw*0.4916*0.8);
	return 0;
}

/* `return 0` means don't pass the shortcut to the rest */
static CGEventRef event_handler(CGEventTapProxy p, CGEventType t, CGEventRef event, void* r) {
	int rr = CGEventGetIntegerValueField(event,kCGKeyboardEventAutorepeat),
	     k = CGEventGetIntegerValueField(event,kCGKeyboardEventKeycode),
	     m = (int)CGEventGetFlags(event);
	m &= (kCGEventFlagMaskShift|kCGEventFlagMaskAlternate|kCGEventFlagMaskCommand|kCGEventFlagMaskControl|kCGEventFlagMaskSecondaryFn);
	/*
	// UNCOMMENT TO ANALYSE KEY CODES (EVENT VIEWER)
	printf("is repeating: %d\n"
	       "     keycode: %d\n"
	       "   modifiers: %d\n"
	       "is fwd slash: %d\n\n", rr, k, m, k==KEY_SLASH_FORWARD);
	*/
	if (m == MOD_FN) // (m == MOD_CMD+MOD_CTRL+MOD_OPT+MOD_SHIFT)
		switch (k) { /* always grabs as it is qwerty layout */
			case (KEY_W):      system("open -a Safari"); return 0;
			case (KEY_RETURN): system("open -a Terminal"); return 0;

			case (KEY_U):             top_left_quarter(); return 0;
			case (KEY_J):             left_half(); return 0;
			case (KEY_N):             bottom_left_quarter(); return 0;

			case (KEY_P):             top_right_quarter(); return 0;
			case (KEY_SEMI_COLON):    right_half(); return 0;
			case (KEY_L):             bottom_right_quarter(); return 0;

			case (KEY_F):             full_screen_w_gaps(); return 0;
			case (KEY_S):             center_resize1(); return 0;
			case (KEY_A):             center_resize0(); return 0;
			case (KEY_C):             center(); return 0;
		}
	return event;
}

cr_eventloop() { /* create and run eventloop (a keytap from applicationservices) */
	CFRunLoopSourceRef rlsrc;
	CGEventMask em = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged);
	CFMachPortRef et = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0, em, event_handler, 0);
	if(!et)return 1; /* 1:fail, 0:success */
	rlsrc = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, et, 0);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), rlsrc, kCFRunLoopCommonModes);
	CGEventTapEnable(et, true); CFRunLoopRun();
	return 0;
}

main(){init();cr_eventloop();}
