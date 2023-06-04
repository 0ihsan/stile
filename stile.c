#include <stdlib.h>
#include <Carbon/Carbon.h>
#include <ApplicationServices/ApplicationServices.h>

   // === config ===
#define dx ((int)0)
#define dy ((int)0)
#define dw ((int)1710)
#define dh ((int)1112)
#define gap ((int)0)
#define smallwidth ((float)(dw*0.4916*0.8))
#define mediumheight ((float)714)
#define winratio ((float)1.52) //3.8/2.5
   // === config ===

/* names are based on the keyboard layout: "BRITISH - PC" (US for KEY_SECTION) */
enum {MOD_CMD=1048576, MOD_CTRL=262144, MOD_FN=8388608, MOD_OPT=524288, MOD_SHIFT=131072};
enum {KEY_A, KEY_S, KEY_D, KEY_F, KEY_H, KEY_G, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_SECTION, KEY_B, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_Y, KEY_T, KEY_1, KEY_2, KEY_3, KEY_4, KEY_6, KEY_5, KEY_EQUALS, KEY_9, KEY_7, KEY_MINUS_OR_DASH, KEY_8, KEY_0, KEY_BRACKET_CLOSE, KEY_O, KEY_U, KEY_BRACKET_OPEN, KEY_I, KEY_P, KEY_RETURN, KEY_L, KEY_J, KEY_SINGLE_QUOTE, KEY_K, KEY_SC, KEY_SLASH_BACK, KEY_COMMA, KEY_SLASH_FORWARD, KEY_N, KEY_M, KEY_DOT, KEY_TAB, KEY_SPACE, KEY_GRAVE_ACCENT, KEY_BACKSPACE, KEY_ENTER, KEY_ESCAPE, KEY_RIGHT_CMD, KEY_LEFT_CMD, KEY_LEFT_SHIFT, KEY_CAPS_LOCK, KEY_LEFT_OPTION, KEY_LEFT_CTRL, KEY_RIGHT_SHIFT, KEY_RIGHT_OPTION, KEY_RIGHT_CTRL, KEY_FN, KEY_F17, KEY_NUMPAD_DOT, UNK66, KEY_STAR_ASTERISK, UNK68, KEY_PLUS, UNK70, KEY_CLEAR, UNK72, UNK73, UNK74, KEY_NUMPAD_SLASH_FORWARD, KEY_NUMPAD_ENTER, UNK77, KEY_NUMPAD_MINUS, KEY_F18, KEY_F19, KEY_NUMPAD_EQUALS, KEY_NUMPAD_0, KEY_NUMPAD_1, KEY_NUMPAD_2, KEY_NUMPAD_3, KEY_NUMPAD_4, KEY_NUMPAD_5, KEY_NUMPAD_6, KEY_NUMPAD_7, KEY_F20, KEY_NUMPAD_8, KEY_NUMPAD_9, UNK93, UNK94, UNK95, KEY_F5, KEY_F6, KEY_F7, KEY_F3, KEY_F8, KEY_F9, UNK102, KEY_F11, UNK104, KEY_F13, KEY_F16, KEY_F14_F1, UNK108, KEY_F10, UNK110, KEY_F12, UNK112, KEY_F15, KEY_HELP, KEY_HOME, KEY_PGUP, KEY_DELETE, KEY_F4, KEY_END, KEY_F2, KEY_PAGEDOWN, KEY_F1, KEY_LEFT, KEY_RIGHT, KEY_DOWN, KEY_UP};

void mvwin(c,x,y,w,h) { // c:center ws:winsize wp:winpos
	pid_t p;ProcessSerialNumber sn;GetFrontProcess(&sn);GetProcessPID(&sn,&p); AXUIElementRef currwin,currapp=AXUIElementCreateApplication(p); CGSize ws;CGPoint wp;AXValueRef tmp,ts;
	AXUIElementCopyAttributeValue(currapp, kAXFocusedWindowAttribute, (CFTypeRef*)&currwin);
	if(c){AXUIElementCopyAttributeValue(currwin,kAXSizeAttribute,(CFTypeRef*)&ts); AXValueGetValue(ts,kAXValueCGSizeType,&ws); wp.x=(dx+dw-ws.width)/2; wp.y=(dy*2+dh-ws.height)/2;}else{wp.x=x; wp.y=y; ws.width=w; ws.height=h;}
	tmp=AXValueCreate(kAXValueCGPointType,&wp); AXUIElementSetAttributeValue(currwin, kAXPositionAttribute, tmp);
	tmp=AXValueCreate(kAXValueCGSizeType, &ws); AXUIElementSetAttributeValue(currwin, kAXSizeAttribute, tmp);
	CFRelease(tmp);
}

void ccc() { mvwin(1, 0,0,0,0);}
void cr0() { mvwin(0, dx+(dw-smallwidth)/2             , dy+(dh-smallwidth)/2  , smallwidth           , smallwidth);}
void cr1() { mvwin(0, dx+(dw-(mediumheight*winratio))/2, dy+(dh-mediumheight)/2, mediumheight*winratio, mediumheight);}
void fwg() { mvwin(0, dx+gap                           , dy+gap                , dw-gap*2             , dh-gap*2);}
void llh() { mvwin(0, dx+gap                           , dy+gap                , (dw/2)-gap*1.5       , dh-gap*2);}
void tlq() { mvwin(0, dx+gap                           , dy+gap                , (dw/2)-gap*1.5       , (dh/2)-(gap*1.5));}
void blq() { mvwin(0, dx+gap                           , dy+(dh/2)+(gap/2)     , (dw/2)-(gap*1.5)     , (dh/2)-(gap*1.5));}
void rrh() { mvwin(0, dx+(dw/2)+(gap/2)                , dy+gap                , (dw/2)-(gap*1.5)     , dh-(gap*2));}
void trq() { mvwin(0, dx+(dw/2)+(gap/2)                , dy+gap                , (dw/2)-(gap*1.5)     , (dh/2)-(gap*1.5));}
void brq() { mvwin(0, dx+(dw/2)+(gap/2)                , dy+(dh/2)+(gap/2)     , (dw/2)-(gap*1.5)     , (dh/2)-(gap*1.5));}

static CGEventRef event_handler(CGEventTapProxy p, CGEventType t, CGEventRef event, void* r) {
	int k=CGEventGetIntegerValueField(event,kCGKeyboardEventKeycode), m=(int)CGEventGetFlags(event);
	m &= (kCGEventFlagMaskShift|kCGEventFlagMaskAlternate|kCGEventFlagMaskCommand|kCGEventFlagMaskControl|kCGEventFlagMaskSecondaryFn);

	if (m == MOD_FN)
		switch (k) { /* always use qwerty layout */
			// t:top l:left r:right h:half q:quarter
			// f:full w:with g:gap c:center r:resize
			case (KEY_U):  tlq(); return 0;
			case (KEY_J):  llh(); return 0;
			case (KEY_N):  blq(); return 0;

			case (KEY_P):  trq(); return 0;
			case (KEY_SC): rrh(); return 0;
			case (KEY_L):  brq(); return 0;

			case (KEY_F):  fwg(); return 0;
			case (KEY_S):  cr1(); return 0;
			case (KEY_A):  cr0(); return 0;
			case (KEY_C):  ccc(); return 0;
		}
	return event;
}

main(){if(!AXIsProcessTrusted()){AXUIElementCreateSystemWide();fprintf(stderr,"Settings > Privacy & Security > Accessibility > add /usr/local/bin/stile\n");return 1;}
	CFRunLoopSourceRef rlsrc;CGEventMask em=CGEventMaskBit(kCGEventKeyDown)|CGEventMaskBit(kCGEventFlagsChanged); CFMachPortRef et=CGEventTapCreate(kCGSessionEventTap,kCGHeadInsertEventTap,0,em,event_handler,0);
	if(!et)return 1;
	rlsrc=CFMachPortCreateRunLoopSource(kCFAllocatorDefault,et,0);
	CFRunLoopAddSource(CFRunLoopGetCurrent(),rlsrc,kCFRunLoopCommonModes); CGEventTapEnable(et,true); CFRunLoopRun(); return 0;
}
