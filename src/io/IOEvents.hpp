#ifndef ARI_IO_EVENTS_HPP
#define ARI_IO_EVENTS_HPP

#include <cstdint>

    enum {
        ARI_MAX_TOUCHPOINTS = 8,
        ARI_MAX_MOUSEBUTTONS = 3,
        ARI_MAX_KEYCODES = 512,
    };

    typedef enum ari_event_type {
        ARI_EVENTTYPE_INVALID,
        ARI_EVENTTYPE_KEY_DOWN,
        ARI_EVENTTYPE_KEY_UP,
        ARI_EVENTTYPE_CHAR,
        ARI_EVENTTYPE_MOUSE_DOWN,
        ARI_EVENTTYPE_MOUSE_UP,
        ARI_EVENTTYPE_MOUSE_SCROLL,
        ARI_EVENTTYPE_MOUSE_MOVE,
        ARI_EVENTTYPE_MOUSE_ENTER,
        ARI_EVENTTYPE_MOUSE_LEAVE,
        ARI_EVENTTYPE_TOUCHES_BEGAN,
        ARI_EVENTTYPE_TOUCHES_MOVED,
        ARI_EVENTTYPE_TOUCHES_ENDED,
        ARI_EVENTTYPE_TOUCHES_CANCELLED,
        ARI_EVENTTYPE_RESIZED,
        ARI_EVENTTYPE_ICONIFIED,
        ARI_EVENTTYPE_RESTORED,
        ARI_EVENTTYPE_SUSPENDED,
        ARI_EVENTTYPE_RESUMED,
        ARI_EVENTTYPE_UPDATE_CURSOR,
        ARI_EVENTTYPE_QUIT_REQUESTED,
        ARI_EVENTTYPE_CLIPBOARD_PASTED,
        ARI_EVENTTYPE_FILES_DROPPED,
        _ARI_EVENTTYPE_NUM,
        _ARI_EVENTTYPE_FORCE_U32 = 0x7FFFFFF
    } ari_event_type;

    /* key codes are the same names and values as GLFW */
    typedef enum ari_keycode {
        ARI_KEYCODE_INVALID          = 0,
        ARI_KEYCODE_SPACE            = 32,
        ARI_KEYCODE_APOSTROPHE       = 39,  /* ' */
        ARI_KEYCODE_COMMA            = 44,  /* , */
        ARI_KEYCODE_MINUS            = 45,  /* - */
        ARI_KEYCODE_PERIOD           = 46,  /* . */
        ARI_KEYCODE_SLASH            = 47,  /* / */
        ARI_KEYCODE_0                = 48,
        ARI_KEYCODE_1                = 49,
        ARI_KEYCODE_2                = 50,
        ARI_KEYCODE_3                = 51,
        ARI_KEYCODE_4                = 52,
        ARI_KEYCODE_5                = 53,
        ARI_KEYCODE_6                = 54,
        ARI_KEYCODE_7                = 55,
        ARI_KEYCODE_8                = 56,
        ARI_KEYCODE_9                = 57,
        ARI_KEYCODE_SEMICOLON        = 59,  /* ; */
        ARI_KEYCODE_EQUAL            = 61,  /* = */
        ARI_KEYCODE_A                = 65,
        ARI_KEYCODE_B                = 66,
        ARI_KEYCODE_C                = 67,
        ARI_KEYCODE_D                = 68,
        ARI_KEYCODE_E                = 69,
        ARI_KEYCODE_F                = 70,
        ARI_KEYCODE_G                = 71,
        ARI_KEYCODE_H                = 72,
        ARI_KEYCODE_I                = 73,
        ARI_KEYCODE_J                = 74,
        ARI_KEYCODE_K                = 75,
        ARI_KEYCODE_L                = 76,
        ARI_KEYCODE_M                = 77,
        ARI_KEYCODE_N                = 78,
        ARI_KEYCODE_O                = 79,
        ARI_KEYCODE_P                = 80,
        ARI_KEYCODE_Q                = 81,
        ARI_KEYCODE_R                = 82,
        ARI_KEYCODE_S                = 83,
        ARI_KEYCODE_T                = 84,
        ARI_KEYCODE_U                = 85,
        ARI_KEYCODE_V                = 86,
        ARI_KEYCODE_W                = 87,
        ARI_KEYCODE_X                = 88,
        ARI_KEYCODE_Y                = 89,
        ARI_KEYCODE_Z                = 90,
        ARI_KEYCODE_LEFT_BRACKET     = 91,  /* [ */
        ARI_KEYCODE_BACKSLASH        = 92,  /* \ */
        ARI_KEYCODE_RIGHT_BRACKET    = 93,  /* ] */
        ARI_KEYCODE_GRAVE_ACCENT     = 96,  /* ` */
        ARI_KEYCODE_WORLD_1          = 161, /* non-US #1 */
        ARI_KEYCODE_WORLD_2          = 162, /* non-US #2 */
        ARI_KEYCODE_ESCAPE           = 256,
        ARI_KEYCODE_ENTER            = 257,
        ARI_KEYCODE_TAB              = 258,
        ARI_KEYCODE_BACKSPACE        = 259,
        ARI_KEYCODE_INSERT           = 260,
        ARI_KEYCODE_DELETE           = 261,
        ARI_KEYCODE_RIGHT            = 262,
        ARI_KEYCODE_LEFT             = 263,
        ARI_KEYCODE_DOWN             = 264,
        ARI_KEYCODE_UP               = 265,
        ARI_KEYCODE_PAGE_UP          = 266,
        ARI_KEYCODE_PAGE_DOWN        = 267,
        ARI_KEYCODE_HOME             = 268,
        ARI_KEYCODE_END              = 269,
        ARI_KEYCODE_CAPS_LOCK        = 280,
        ARI_KEYCODE_SCROLL_LOCK      = 281,
        ARI_KEYCODE_NUM_LOCK         = 282,
        ARI_KEYCODE_PRINT_SCREEN     = 283,
        ARI_KEYCODE_PAUSE            = 284,
        ARI_KEYCODE_F1               = 290,
        ARI_KEYCODE_F2               = 291,
        ARI_KEYCODE_F3               = 292,
        ARI_KEYCODE_F4               = 293,
        ARI_KEYCODE_F5               = 294,
        ARI_KEYCODE_F6               = 295,
        ARI_KEYCODE_F7               = 296,
        ARI_KEYCODE_F8               = 297,
        ARI_KEYCODE_F9               = 298,
        ARI_KEYCODE_F10              = 299,
        ARI_KEYCODE_F11              = 300,
        ARI_KEYCODE_F12              = 301,
        ARI_KEYCODE_F13              = 302,
        ARI_KEYCODE_F14              = 303,
        ARI_KEYCODE_F15              = 304,
        ARI_KEYCODE_F16              = 305,
        ARI_KEYCODE_F17              = 306,
        ARI_KEYCODE_F18              = 307,
        ARI_KEYCODE_F19              = 308,
        ARI_KEYCODE_F20              = 309,
        ARI_KEYCODE_F21              = 310,
        ARI_KEYCODE_F22              = 311,
        ARI_KEYCODE_F23              = 312,
        ARI_KEYCODE_F24              = 313,
        ARI_KEYCODE_F25              = 314,
        ARI_KEYCODE_KP_0             = 320,
        ARI_KEYCODE_KP_1             = 321,
        ARI_KEYCODE_KP_2             = 322,
        ARI_KEYCODE_KP_3             = 323,
        ARI_KEYCODE_KP_4             = 324,
        ARI_KEYCODE_KP_5             = 325,
        ARI_KEYCODE_KP_6             = 326,
        ARI_KEYCODE_KP_7             = 327,
        ARI_KEYCODE_KP_8             = 328,
        ARI_KEYCODE_KP_9             = 329,
        ARI_KEYCODE_KP_DECIMAL       = 330,
        ARI_KEYCODE_KP_DIVIDE        = 331,
        ARI_KEYCODE_KP_MULTIPLY      = 332,
        ARI_KEYCODE_KP_SUBTRACT      = 333,
        ARI_KEYCODE_KP_ADD           = 334,
        ARI_KEYCODE_KP_ENTER         = 335,
        ARI_KEYCODE_KP_EQUAL         = 336,
        ARI_KEYCODE_LEFT_SHIFT       = 340,
        ARI_KEYCODE_LEFT_CONTROL     = 341,
        ARI_KEYCODE_LEFT_ALT         = 342,
        ARI_KEYCODE_LEFT_SUPER       = 343,
        ARI_KEYCODE_RIGHT_SHIFT      = 344,
        ARI_KEYCODE_RIGHT_CONTROL    = 345,
        ARI_KEYCODE_RIGHT_ALT        = 346,
        ARI_KEYCODE_RIGHT_SUPER      = 347,
        ARI_KEYCODE_MENU             = 348,
    } ari_keycode;

    typedef struct ari_touchpoint {
        uintptr_t identifier = 0;
        float pos_x = 0.0f;
        float pos_y = 0.0f;
        bool changed = false;
    } ari_touchpoint;

    typedef enum ari_mousebutton {
        ARI_MOUSEBUTTON_INVALID = 0x100,
        ARI_MOUSEBUTTON_LEFT = 0,
        ARI_MOUSEBUTTON_RIGHT = 1,
        ARI_MOUSEBUTTON_MIDDLE = 2,
    } ari_mousebutton;

    enum {
        ARI_MODIFIER_SHIFT = (1<<0),
        ARI_MODIFIER_CTRL = (1<<1),
        ARI_MODIFIER_ALT = (1<<2),
        ARI_MODIFIER_SUPER = (1<<3)
    };

    typedef struct ari_event {
        uint64_t frame_count = 0;
        ari_event_type type = ARI_EVENTTYPE_INVALID;
        ari_keycode key_code = ARI_KEYCODE_INVALID;
        uint32_t char_code = 0;
        bool key_repeat = false;
        uint32_t modifiers = 0;
        ari_mousebutton mouse_button = ARI_MOUSEBUTTON_INVALID;
        float mouse_x = 0.0f;
        float mouse_y = 0.0f;
        float mouse_dx = 0.0f;
        float mouse_dy = 0.0f;
        float scroll_x = 0.0f;
        float scroll_y = 0.0f;
        int num_touches = 0;
        ari_touchpoint touches[ARI_MAX_TOUCHPOINTS];
        int window_width = 0;
        int window_height = 0;
        int framebuffer_width = 0;
        int framebuffer_height = 0;
    } ari_event;

#endif // ARI_IO_EVENTS_HPP