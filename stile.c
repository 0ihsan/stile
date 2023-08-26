#include <Carbon/Carbon.h>
#include <ApplicationServices/ApplicationServices.h>
typedef int I;typedef void V;
	// ===/config\===
I dx=0,dy=44,gap=0,dw=1710,dh=1068;
#define mh 0.6685393258*dh
#define wr 2.0f
#define factor 0.39328
	// ===\config/===
/* names are based on the keyboard layout: "BRITISH - PC" (US for KEY_SECTION) */
enum{MOD_CMD=1048576,MOD_CTRL=262144,MOD_FN=8388608,MOD_OPT=524288,MOD_SHIFT=131072};
enum{KEY_A,KEY_S,KEY_D,KEY_F,KEY_H,KEY_G,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_SECTION,KEY_B,KEY_Q,KEY_W,KEY_E,KEY_R,KEY_Y,KEY_T,KEY_1,KEY_2,KEY_3,KEY_4,KEY_6,KEY_5,KEY_EQUALS,KEY_9,KEY_7,KEY_MINUS_OR_DASH,KEY_8,KEY_0,KEY_BRACKET_CLOSE,KEY_O,KEY_U,KEY_BRACKET_OPEN,KEY_I,KEY_P,KEY_RETURN,KEY_L,KEY_J,KEY_SINGLE_QUOTE,KEY_K,KEY_SC,KEY_SLASH_BACK,KEY_COMMA,KEY_SLASH_FORWARD,KEY_N,KEY_M,KEY_DOT,KEY_TAB,KEY_SPACE,KEY_GRAVE_ACCENT,KEY_BACKSPACE,KEY_ENTER,KEY_ESCAPE,KEY_RIGHT_CMD,KEY_LEFT_CMD,KEY_LEFT_SHIFT,KEY_CAPS_LOCK,KEY_LEFT_OPTION,KEY_LEFT_CTRL,KEY_RIGHT_SHIFT,KEY_RIGHT_OPTION,KEY_RIGHT_CTRL,KEY_FN,KEY_F17,KEY_NUMPAD_DOT,UNK66,KEY_STAR_ASTERISK,UNK68,KEY_PLUS,UNK70,KEY_CLEAR,UNK72,UNK73,UNK74,KEY_NUMPAD_SLASH_FORWARD,KEY_NUMPAD_ENTER,UNK77,KEY_NUMPAD_MINUS,KEY_F18,KEY_F19,KEY_NUMPAD_EQUALS,KEY_NUMPAD_0,KEY_NUMPAD_1,KEY_NUMPAD_2,KEY_NUMPAD_3,KEY_NUMPAD_4,KEY_NUMPAD_5,KEY_NUMPAD_6,KEY_NUMPAD_7,KEY_F20,KEY_NUMPAD_8,KEY_NUMPAD_9,UNK93,UNK94,UNK95,KEY_F5,KEY_F6,KEY_F7,KEY_F3,KEY_F8,KEY_F9,UNK102,KEY_F11,UNK104,KEY_F13,KEY_F16,KEY_F14_F1,UNK108,KEY_F10,UNK110,KEY_F12,UNK112,KEY_F15,KEY_HELP,KEY_HOME,KEY_PGUP,KEY_DELETE,KEY_F4,KEY_END,KEY_F2,KEY_PAGEDOWN,KEY_F1,KEY_LEFT,KEY_RIGHT,KEY_DOWN,KEY_UP};
float sw,cr0_x,cr0_y,cr1_x,cr1_y,fwg_x,fwg_y,fwg_w,fwg_h,llh_x,llh_y,llh_w,llh_h,tlq_x,tlq_y,tlq_w,tlq_h,blq_x,blq_y,blq_w,blq_h,rrh_x,rrh_y,rrh_w,rrh_h,trq_x,trq_y,trq_w,trq_h,brq_x,brq_y,brq_w,brq_h;

static V mvwin(I c,I x,I y,I w,I h) { // c:center ws:winsize wp:winpos
 pid_t p;ProcessSerialNumber sn;GetFrontProcess(&sn);GetProcessPID(&sn,&p); AXUIElementRef currwin,currapp=AXUIElementCreateApplication(p); CGSize ws;CGPoint wp;AXValueRef tmp,ts;
 AXUIElementCopyAttributeValue(currapp, kAXFocusedWindowAttribute, (CFTypeRef*)&currwin);
 if(c){AXUIElementCopyAttributeValue(currwin,kAXSizeAttribute,(CFTypeRef*)&ts); AXValueGetValue(ts,kAXValueCGSizeType,&ws); wp.x=(dx+dw-ws.width)/2; wp.y=(dy*2+dh-ws.height)/2;}
 else{wp.x=x; wp.y=y; ws.width=w; ws.height=h;}
 tmp=AXValueCreate(kAXValueCGPointType,&wp); AXUIElementSetAttributeValue(currwin, kAXPositionAttribute, tmp);
 tmp=AXValueCreate(kAXValueCGSizeType, &ws); AXUIElementSetAttributeValue(currwin, kAXSizeAttribute, tmp);
 CFRelease(tmp);
}

static CGEventRef handler(CGEventTapProxy p, CGEventType t, CGEventRef e, V* r) {
 I k=CGEventGetIntegerValueField(e,kCGKeyboardEventKeycode), m=(I)CGEventGetFlags(e);
 m &= (kCGEventFlagMaskShift|kCGEventFlagMaskAlternate|kCGEventFlagMaskCommand|kCGEventFlagMaskControl|kCGEventFlagMaskSecondaryFn);
 if (m==MOD_FN)
  switch (k) { /* always use qwerty layout */
   // t:top b:bottom l:left r:right h:half q:quarter f:full w:with g:gap c:center r:resize
   case (KEY_U):  mvwin(0, tlq_x, tlq_y, tlq_w, tlq_h); return 0; // tlq
   case (KEY_J):  mvwin(0, llh_x, llh_y, llh_w, llh_h); return 0; // llh
   case (KEY_N):  mvwin(0, blq_x, blq_y, blq_w, blq_h); return 0; // blq
   case (KEY_P):  mvwin(0, trq_x, trq_y, trq_w, trq_h); return 0; // trq
   case (KEY_SC): mvwin(0, rrh_x, rrh_y, rrh_w, rrh_h); return 0; // rrh
   case (KEY_L):  mvwin(0, brq_x, brq_y, brq_w, brq_h); return 0; // brq
   case (KEY_F):  mvwin(0, fwg_x, fwg_y, fwg_w, fwg_h); return 0; // fwg
   case (KEY_S):  mvwin(0, cr1_x, cr1_y, mh*wr,    mh); return 0; // cr1
   case (KEY_A):  mvwin(0, cr0_x, cr0_y, sw,       sw); return 0; // cr0
   case (KEY_C):  mvwin(1, 0,0,0,0);                    return 0; // ccc
  }
 return e;
}

static I stoi(char*s){I r=0,i=0;while(s[i]!='\0')r=r*10+s[i++]-'0';return r;}
I main(I ac,char**av){if(!AXIsProcessTrusted()){AXUIElementCreateSystemWide();fprintf(stderr,"Settings > Privacy & Security > Accessibility > add /usr/local/bin/stile\n");return 1;}
 if(ac>1)gap=stoi(av[1]);
 printf("gap: %d\n",gap);
 sw=dw*factor,
 cr0_x=dx+(dw-sw)/2, cr0_y=dy+(dh-sw)/2,
 cr1_x=dx+(dw-(mh*wr))/2, cr1_y=dy+(dh-mh)/2,
 fwg_x=dx+gap, fwg_y=dy+gap, fwg_w=dw-gap*2, fwg_h=dh-gap*2,
 llh_x=dx+gap, llh_y=dy+gap, llh_w=(dw/2)-gap*1.5, llh_h=dh-gap*2,
 tlq_x=dx+gap, tlq_y=dy+gap, tlq_w=(dw/2)-gap*1.5, tlq_h=(dh/2)-(gap*1.5),
 blq_x=dx+gap, blq_y=dy+(dh/2)+(gap/2), blq_w=(dw/2)-(gap*1.5), blq_h=(dh/2)-(gap*1.5),
 rrh_x=dx+(dw/2)+(gap/2), rrh_y=dy+gap, rrh_w=(dw/2)-(gap*1.5), rrh_h=dh-(gap*2),
 trq_x=dx+(dw/2)+(gap/2), trq_y=dy+gap, trq_w=(dw/2)-(gap*1.5), trq_h=(dh/2)-(gap*1.5),
 brq_x=dx+(dw/2)+(gap/2), brq_y=dy+(dh/2)+(gap/2), brq_w=(dw/2)-(gap*1.5), brq_h=(dh/2)-(gap*1.5);
 CFRunLoopSourceRef rlsrc;CGEventMask em=CGEventMaskBit(kCGEventKeyDown)|CGEventMaskBit(kCGEventFlagsChanged); CFMachPortRef et=CGEventTapCreate(kCGSessionEventTap,kCGHeadInsertEventTap,0,em,handler,0);
 if(!et)return 1;
 rlsrc=CFMachPortCreateRunLoopSource(kCFAllocatorDefault,et,0); CFRunLoopAddSource(CFRunLoopGetCurrent(),rlsrc,kCFRunLoopCommonModes); CGEventTapEnable(et,true); CFRunLoopRun(); return 0;
}
