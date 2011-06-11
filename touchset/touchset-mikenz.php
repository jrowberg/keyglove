<?php

$touchSet = array(
    "default" => array(
        "begin" => array(
            array("action" => "beep", "tone" => "2000", "mode" => "KBEEP_SHORTBEEP", "duration"=>1),
            array("action" => "redled", "mode" => "KLED_OFF"),
            array("action" => "greenled", "mode" => "KLED_SOLID", "duration"=>0),
            array("action" => "blueled", "mode" => "KLED_OFF"),
            ),
        "touch" => array(
            "A8" => array("action" => "mouseon", "mode" => "KMOUSE_MOVE"),
            "D8" => array("action" => "mouseon", "mode" => "KMOUSE_SCROLL"),

            "J1" => array("action" => "mousedown", "button" => "KMOUSE_LEFT"),
            "J2" => array("action" => "mousedown", "button" => "KMOUSE_MIDDLE"),
            "J3" => array("action" => "mousedown", "button" => "KMOUSE_RIGHT"),
            ),
        "release" => array(
            "A8" => array("action" => "mouseoff", "mode" => "KMOUSE_MOVE"),
            "D8" => array("action" => "mouseoff", "mode" => "KMOUSE_SCROLL"),
            
            "J1" => array("action" => "mouseup", "button" => "KMOUSE_LEFT"),
            "J2" => array("action" => "mouseup", "button" => "KMOUSE_MIDDLE"),
            "J3" => array("action" => "mouseup", "button" => "KMOUSE_RIGHT"),
            
            "AY" => array("action" => "keypress", "code" => "KEY_A"),
            "PY" => array("action" => "keypress", "code" => "KEY_B"),
            "TY" => array("action" => "keypress", "code" => "KEY_C"),
            "KY" => array("action" => "keypress", "code" => "KEY_D"),
            "BY" => array("action" => "keypress", "code" => "KEY_E"),
            "MY" => array("action" => "keypress", "code" => "KEY_F"),
            "UY" => array("action" => "keypress", "code" => "KEY_G"),
            "EY" => array("action" => "keypress", "code" => "KEY_H"),
            "CY" => array("action" => "keypress", "code" => "KEY_I"),
            "VY" => array("action" => "keypress", "code" => "KEY_J"),
            "RY" => array("action" => "keypress", "code" => "KEY_K"),
            "QY" => array("action" => "keypress", "code" => "KEY_L"),
            "SY" => array("action" => "keypress", "code" => "KEY_M"),
            "FY" => array("action" => "keypress", "code" => "KEY_N"),
            "NY" => array("action" => "keypress", "code" => "KEY_O"),
            "JY" => array("action" => "keypress", "code" => "KEY_P"),
            "Y4" => array("action" => "keypress", "code" => "KEY_Q"),
            "DY" => array("action" => "keypress", "code" => "KEY_R"),
            "IY" => array("action" => "keypress", "code" => "KEY_S"),
            "HY" => array("action" => "keypress", "code" => "KEY_T"),
            "OY" => array("action" => "keypress", "code" => "KEY_U"),
            "WY" => array("action" => "keypress", "code" => "KEY_V"),
            "GY" => array("action" => "keypress", "code" => "KEY_W"),
            "XY" => array("action" => "keypress", "code" => "KEY_X"),
            "LY" => array("action" => "keypress", "code" => "KEY_Y"),
            "Y5" => array("action" => "keypress", "code" => "KEY_Z"),
            
            "ADY" => array("action" => "keypress", "code" => "KEY_BACKSPACE"),
            "DGY" => array("action" => "keypress", "code" => "KEY_SPACE"),
            "GJY" => array("action" => "keypress", "code" => "KEY_ENTER"),
            "AJY" => array("action" => "keypress", "code" => "KEY_TAB"),

            "Y7" => array("action" => "keypress", "code" => "KEY_ESC"),
            "Y6" => array("action" => "keypress", "code" => "KEY_DELETE"),
            
            "A1" => array("action" => "setmode", "mode" => "numbers"),
            "A2" => array("action" => "setmode", "mode" => "symbols"),
            "A3" => array("action" => "setmode", "mode" => "functions"),
            "D1" => array("action" => "togglemode", "mode" => "shift"),
            "D2" => array("action" => "togglemode", "mode" => "ctrl"),
            "D3" => array("action" => "togglemode", "mode" => "alt"),
            
            ),
        ),
    "numbers" => array(
        "begin" => array(
            array("action" => "beep", "tone" => "2000", "mode" => "KBEEP_SHORTBEEP", "duration"=>2),
            array("action" => "redled", "mode" => "KLED_OFF"),
            array("action" => "greenled", "mode" => "KLED_OFF"),
            array("action" => "blueled", "mode" => "KLED_SOLID", "duration"=>0),
            ),
        "touch" => array(
            "A8" => array("action" => "mouseon", "mode" => "KMOUSE_MOVE"),
            "D8" => array("action" => "mouseon", "mode" => "KMOUSE_SCROLL"),

            "J1" => array("action" => "mousedown", "button" => "KMOUSE_LEFT"),
            "J2" => array("action" => "mousedown", "button" => "KMOUSE_MIDDLE"),
            "J3" => array("action" => "mousedown", "button" => "KMOUSE_RIGHT"),
            ),
        "release" => array(
            "A8" => array("action" => "mouseoff", "mode" => "KMOUSE_MOVE"),
            "D8" => array("action" => "mouseoff", "mode" => "KMOUSE_SCROLL"),
            
            "J1" => array("action" => "mouseup", "button" => "KMOUSE_LEFT"),
            "J2" => array("action" => "mouseup", "button" => "KMOUSE_MIDDLE"),
            "J3" => array("action" => "mouseup", "button" => "KMOUSE_RIGHT"),
            
            "AY" => array("action" => "keypress", "code" => "KEY_1"),
            "BY" => array("action" => "keypress", "code" => "KEY_2"),
            "CY" => array("action" => "keypress", "code" => "KEY_3"),
            "DY" => array("action" => "keypress", "code" => "KEY_4"),
            "EY" => array("action" => "keypress", "code" => "KEY_5"),
            "FY" => array("action" => "keypress", "code" => "KEY_6"),
            "GY" => array("action" => "keypress", "code" => "KEY_7"),
            "HY" => array("action" => "keypress", "code" => "KEY_8"),
            "IY" => array("action" => "keypress", "code" => "KEY_9"),
            
            "JY" => array( // *
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_3"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "KY" => array("action" => "keypress", "code" => "KEY_0"),
            "LY" => array( // #
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_8"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),

            "MY" => array("action" => "keypress", "code" => "KEYPAD_PLUS"),
            "NY" => array("action" => "keypress", "code" => "KEYPAD_MINUS"),
            
            "PY" => array("action" => "keypress", "code" => "KEYPAD_ASTERIX"),
            "QY" => array("action" => "keypress", "code" => "KEYPAD_SLASH"),

            "SY" => array("action" => "keypress", "code" => "KEYPAD_EQUAL"),
            "TY" => array( // ^
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_6"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "UY" => array( // %
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_5"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            
            "VY" => array("action" => "keypress", "code" => "KEYPAD_PERIOD"),
            
            
            "ADY" => array("action" => "keypress", "code" => "KEY_BACKSPACE"),
            "DGY" => array("action" => "keypress", "code" => "KEY_SPACE"),
            "GJY" => array("action" => "keypress", "code" => "KEYPAD_ENTER"),
            "AJY" => array("action" => "keypress", "code" => "KEY_TAB"),

            "Y7" => array("action" => "keypress", "code" => "KEY_ESC"),
            "Y6" => array("action" => "keypress", "code" => "KEY_DELETE"),
            
            "A1" => array("action" => "setmode", "mode" => "default"),
            "A2" => array("action" => "setmode", "mode" => "symbols"),
            "A3" => array("action" => "setmode", "mode" => "functions"),
            "D1" => array("action" => "togglemode", "mode" => "shift"),
            "D2" => array("action" => "togglemode", "mode" => "ctrl"),
            "D3" => array("action" => "togglemode", "mode" => "alt"),
            ),
        ),
    "symbols" => array(
        "begin" => array(
            array("action" => "beep", "tone" => "2000", "mode" => "KBEEP_SHORTBEEP", "duration"=>3),
            array("action" => "redled", "mode" => "KLED_OFF"),
            array("action" => "greenled", "mode" => "KLED_SOLID", "duration"=>0),
            array("action" => "blueled", "mode" => "KLED_SOLID", "duration"=>0),
            ),
        "touch" => array(
            "A8" => array("action" => "mouseon", "mode" => "KMOUSE_MOVE"),
            "D8" => array("action" => "mouseon", "mode" => "KMOUSE_SCROLL"),

            "J1" => array("action" => "mousedown", "button" => "KMOUSE_LEFT"),
            "J2" => array("action" => "mousedown", "button" => "KMOUSE_MIDDLE"),
            "J3" => array("action" => "mousedown", "button" => "KMOUSE_RIGHT"),
            ),
        "release" => array(
            "A8" => array("action" => "mouseoff", "mode" => "KMOUSE_MOVE"),
            "D8" => array("action" => "mouseoff", "mode" => "KMOUSE_SCROLL"),
            
            "J1" => array("action" => "mouseup", "button" => "KMOUSE_LEFT"),
            "J2" => array("action" => "mouseup", "button" => "KMOUSE_MIDDLE"),
            "J3" => array("action" => "mouseup", "button" => "KMOUSE_RIGHT"),
            
            "AY" => array("action" => "keypress", "code" => "KEY_PERIOD"),
            "BY" => array("action" => "keypress", "code" => "KEY_COMMA"),
            "CY" => array( // @
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_2"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "DY" => array( // (
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_9"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "EY" => array( // )
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_0"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "FY" => array( // ?
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_SLASH"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "GY" => array( // <
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_COMMA"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),            
            "HY" => array( // >
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_PERIOD"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "IY" => array("action" => "keypress", "code" => "KEY_SLASH"),
            "JY" => array("action" => "keypress", "code" => "KEY_SEMICOLON"),
            "KY" => array( // :
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_SEMICOLON"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "LY" => array( // ~
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_TILDE"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "MY" => array( // "
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_QUOTE"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "NY" => array("action" => "keypress", "code" => "KEY_QUOTE"),
            "OY" => array( // &
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_7"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "PY" => array( // {
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_LEFT_BRACE"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "QY" => array( // "
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_RIGHT_BRACE"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "RY" => array( // !
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_1"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "SY" => array("action" => "keypress", "code" => "KEY_LEFT_BRACE"),
            "TY" => array("action" => "keypress", "code" => "KEY_RIGHT_BRACE"),
            "UY" => array("action" => "keypress", "code" => "KEY_BACKSLASH"),
            "VY" => array("action" => "keypress", "code" => "KEY_TILDE"),
            "WY" => array( // _
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_MINUS"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            "XY" => array( // |
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_BACKSLASH"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
                ),
            
            
            "ADY" => array("action" => "keypress", "code" => "KEY_BACKSPACE"),
            "DGY" => array("action" => "keypress", "code" => "KEY_SPACE"),
            "GJY" => array("action" => "keypress", "code" => "KEY_ENTER"),
            "AJY" => array("action" => "keypress", "code" => "KEY_TAB"),

            "Y7" => array("action" => "keypress", "code" => "KEY_ESC"),
            "Y6" => array("action" => "keypress", "code" => "KEY_DELETE"),
            
            "A1" => array("action" => "setmode", "mode" => "numbers"),
            "A2" => array("action" => "setmode", "mode" => "default"),
            "A3" => array("action" => "setmode", "mode" => "functions"),
            "D1" => array("action" => "togglemode", "mode" => "shift"),
            "D2" => array("action" => "togglemode", "mode" => "ctrl"),
            "D3" => array("action" => "togglemode", "mode" => "alt"),
            ),
        ),
    "functions" => array(
        "begin" => array(
            array("action" => "beep", "tone" => "2000", "mode" => "KBEEP_SHORTBEEP", "duration"=>4),
            array("action" => "redled", "mode" => "KLED_SOLID", "duration"=>0),
            array("action" => "greenled", "mode" => "KLED_OFF"),
            array("action" => "blueled", "mode" => "KLED_OFF"),
            ),
        "touch" => array(
            "A8" => array("action" => "mouseon", "mode" => "KMOUSE_MOVE"),
            "D8" => array("action" => "mouseon", "mode" => "KMOUSE_SCROLL"),

            "J1" => array("action" => "mousedown", "button" => "KMOUSE_LEFT"),
            "J2" => array("action" => "mousedown", "button" => "KMOUSE_MIDDLE"),
            "J3" => array("action" => "mousedown", "button" => "KMOUSE_RIGHT"),
            ),
        "release" => array(
            "A8" => array("action" => "mouseoff", "mode" => "KMOUSE_MOVE"),
            "D8" => array("action" => "mouseoff", "mode" => "KMOUSE_SCROLL"),
            
            "J1" => array("action" => "mouseup", "button" => "KMOUSE_LEFT"),
            "J2" => array("action" => "mouseup", "button" => "KMOUSE_MIDDLE"),
            "J3" => array("action" => "mouseup", "button" => "KMOUSE_RIGHT"),
            
            "AY" => array("action" => "keypress", "code" => "KEY_F1"),
            "BY" => array("action" => "keypress", "code" => "KEY_F2"),
            "CY" => array("action" => "keypress", "code" => "KEY_F3"),
            "DY" => array("action" => "keypress", "code" => "KEY_F4"),
            "EY" => array("action" => "keypress", "code" => "KEY_F5"),
            "FY" => array("action" => "keypress", "code" => "KEY_F6"),
            "GY" => array("action" => "keypress", "code" => "KEY_F7"),
            "HY" => array("action" => "keypress", "code" => "KEY_F8"),
            "IY" => array("action" => "keypress", "code" => "KEY_F9"),
            "JY" => array("action" => "keypress", "code" => "KEY_F10"),
            "KY" => array("action" => "keypress", "code" => "KEY_F11"),
            "LY" => array("action" => "keypress", "code" => "KEY_F12"),

            //"MY" => array("action" => "keypress", "code" => "KEY_F1"), //calc
            "NY" => array("action" => "modifierpress", "code" => "MODIFIERKEY_GUI"),
            "OY" => array("action" => "keypress", "code" => "KEY_APP"),
            "PY" => array("action" => "keypress", "code" => "KEY_MUTE"), //mute
            //"QY" => array("action" => "keypress", "code" => "KEY_F5"), //vol up
            //"RY" => array("action" => "keypress", "code" => "KEY_F6"), //vol down
            //"SY" => array("action" => "keypress", "code" => "KEY_F7"), // play/pause
            //"TY" => array("action" => "keypress", "code" => "KEY_F8"), // prev track
            //"UY" => array("action" => "keypress", "code" => "KEY_F9"), // next track
            //"VY" => array("action" => "keypress", "code" => "KEY_F10"), // stop
            "WY" => array("action" => "keypress", "code" => "KEY_INSERT"),
            "XY" => array("action" => "keypress", "code" => "KEY_PRINTSCREEN"),          
            
            "ADY" => array("action" => "keypress", "code" => "KEY_BACKSPACE"),
            "DGY" => array("action" => "keypress", "code" => "KEY_SPACE"),
            "GJY" => array("action" => "keypress", "code" => "KEY_ENTER"),
            "AJY" => array("action" => "keypress", "code" => "KEY_TAB"),

            "Y7" => array("action" => "keypress", "code" => "KEY_ESC"),
            "Y6" => array("action" => "keypress", "code" => "KEY_DELETE"),
            
            "A1" => array("action" => "setmode", "mode" => "numbers"),
            "A2" => array("action" => "setmode", "mode" => "symbols"),
            "A3" => array("action" => "setmode", "mode" => "default"),
            "D1" => array("action" => "togglemode", "mode" => "shift"),
            "D2" => array("action" => "togglemode", "mode" => "ctrl"),
            "D3" => array("action" => "togglemode", "mode" => "alt"),
            ),
        ),
    "shift" => array(
        "options" => array(
            "fallthrough" => array("default"),
            ),
        "begin" => array(
            array("action" => "beep", "tone" => "1000", "mode" => "KBEEP_SHORTPULSE", "duration"=>1),
            array("action" => "greenled", "mode" => "KLED_SHORTBLINK", "duration"=>0),
            array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
            ),
        "end" => array(
            array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"),
            ),
        ),
    "ctrl" => array(
        "options" => array(
            "fallthrough" => array("default", "numbers"),
            ),
        "begin" => array(
            array("action" => "beep", "tone" => "1200", "mode" => "KBEEP_SHORTPULSE", "duration"=>1),
            array("action" => "redled", "mode" => "KLED_SHORTBLINK", "duration"=>0),
            array("action" => "modifierdown", "code" => "MODIFIERKEY_CTRL"),
            ),
        "end" => array(
            array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"),
            ),
        ),
    "alt" => array(
        "options" => array(
            "fallthrough" => array("default", "numbers"),
            ),
        "begin" => array(
            array("action" => "beep", "tone" => "1400", "mode" => "KBEEP_SHORTPULSE", "duration"=>1),
            array("action" => "blueled", "mode" => "KLED_SHORTBLINK", "duration"=>0),
            array("action" => "modifierdown", "code" => "MODIFIERKEY_ALT"),
            ),
        "end" => array(
            array("action" => "modifierup", "code" => "MODIFIERKEY_ALT"),
            ),
        ),
    "gui" => array(
        "options" => array(
            "fallthrough" => array("default"),
            ),
        "begin" => array(
            array("action" => "beep", "tone" => "1600", "mode" => "KBEEP_SHORTPULSE", "duration"=>1),
            array("action" => "greenled", "mode" => "KLED_SHORTBLINK", "duration"=>0),
            array("action" => "modifierdown", "code" => "MODIFIERKEY_GUI"),
            ),
        "end" => array(
            array("action" => "modifierup", "code" => "MODIFIERKEY_GUI"),
            ),
        ),
);

