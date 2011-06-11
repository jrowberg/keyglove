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
            
            "AY" => array(array("action" => "keypress", "code" => "KEY_A"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "PY" => array(array("action" => "keypress", "code" => "KEY_B"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "TY" => array(array("action" => "keypress", "code" => "KEY_C"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "KY" => array(array("action" => "keypress", "code" => "KEY_D"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "BY" => array(array("action" => "keypress", "code" => "KEY_E"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "MY" => array(array("action" => "keypress", "code" => "KEY_F"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "UY" => array(array("action" => "keypress", "code" => "KEY_G"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "EY" => array(array("action" => "keypress", "code" => "KEY_H"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "CY" => array(array("action" => "keypress", "code" => "KEY_I"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "VY" => array(array("action" => "keypress", "code" => "KEY_J"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "RY" => array(array("action" => "keypress", "code" => "KEY_K"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "QY" => array(array("action" => "keypress", "code" => "KEY_L"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "SY" => array(array("action" => "keypress", "code" => "KEY_M"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "FY" => array(array("action" => "keypress", "code" => "KEY_N"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "NY" => array(array("action" => "keypress", "code" => "KEY_O"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "JY" => array(array("action" => "keypress", "code" => "KEY_P"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "Y4" => array(array("action" => "keypress", "code" => "KEY_Q"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "DY" => array(array("action" => "keypress", "code" => "KEY_R"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "IY" => array(array("action" => "keypress", "code" => "KEY_S"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "HY" => array(array("action" => "keypress", "code" => "KEY_T"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "OY" => array(array("action" => "keypress", "code" => "KEY_U"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "WY" => array(array("action" => "keypress", "code" => "KEY_V"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "GY" => array(array("action" => "keypress", "code" => "KEY_W"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "XY" => array(array("action" => "keypress", "code" => "KEY_X"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "LY" => array(array("action" => "keypress", "code" => "KEY_Y"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "Y5" => array(array("action" => "keypress", "code" => "KEY_Z"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            
            "ADY" => array(array("action" => "keypress", "code" => "KEY_BACKSPACE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "DGY" => array(array("action" => "keypress", "code" => "KEY_SPACE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "GJY" => array(array("action" => "keypress", "code" => "KEY_ENTER"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "AJY" => array(array("action" => "keypress", "code" => "KEY_TAB"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),

            "Y7" => array(array("action" => "keypress", "code" => "KEY_ESC"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "Y6" => array(array("action" => "keypress", "code" => "KEY_DELETE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            
            "A1" => array("action" => "setmode", "mode" => "numbers"),
            "A2" => array("action" => "setmode", "mode" => "symbols"),
            "A3" => array("action" => "setmode", "mode" => "functions"),
            "D1" => array("action" => "togglemodifier", "code" => "MODIFIERKEY_SHIFT"),
            "D2" => array("action" => "togglemodifier", "code" => "MODIFIERKEY_CTRL"),
            "D3" => array("action" => "togglemodifier", "code" => "MODIFIERKEY_ALT"),
            
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
            
            "AY" => array(array("action" => "keypress", "code" => "KEY_1"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "BY" => array(array("action" => "keypress", "code" => "KEY_2"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "CY" => array(array("action" => "keypress", "code" => "KEY_3"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "DY" => array(array("action" => "keypress", "code" => "KEY_4"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "EY" => array(array("action" => "keypress", "code" => "KEY_5"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "FY" => array(array("action" => "keypress", "code" => "KEY_6"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "GY" => array(array("action" => "keypress", "code" => "KEY_7"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "HY" => array(array("action" => "keypress", "code" => "KEY_8"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "IY" => array(array("action" => "keypress", "code" => "KEY_9"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            
            "JY" => array( // *
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_3"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT") 
                ),
            "KY" => array(array("action" => "keypress", "code" => "KEY_0"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "LY" => array( // #
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_8"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),

            "MY" => array(array("action" => "keypress", "code" => "KEYPAD_PLUS"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "NY" => array(array("action" => "keypress", "code" => "KEYPAD_MINUS"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            
            "PY" => array(array("action" => "keypress", "code" => "KEYPAD_ASTERIX"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "QY" => array(array("action" => "keypress", "code" => "KEYPAD_SLASH"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),

            "SY" => array(array("action" => "keypress", "code" => "KEYPAD_EQUAL"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "TY" => array( // ^
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_6"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            "UY" => array( // %
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_5"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            
            "VY" => array(array("action" => "keypress", "code" => "KEYPAD_PERIOD"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            
            
            "ADY" => array(array("action" => "keypress", "code" => "KEY_BACKSPACE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "DGY" => array(array("action" => "keypress", "code" => "KEY_SPACE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "GJY" => array(array("action" => "keypress", "code" => "KEYPAD_ENTER"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "AJY" => array(array("action" => "keypress", "code" => "KEY_TAB"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),

            "Y7" => array(array("action" => "keypress", "code" => "KEY_ESC"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "Y6" => array(array("action" => "keypress", "code" => "KEY_DELETE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            
            "A1" => array("action" => "setmode", "mode" => "default"),
            "A2" => array("action" => "setmode", "mode" => "symbols"),
            "A3" => array("action" => "setmode", "mode" => "functions"),
            "D1" => array("action" => "togglemodifier", "code" => "MODIFIERKEY_SHIFT"),
            "D2" => array("action" => "togglemodifier", "code" => "MODIFIERKEY_CTRL"),
            "D3" => array("action" => "togglemodifier", "code" => "MODIFIERKEY_ALT"),
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
            
            "AY" => array(array("action" => "keypress", "code" => "KEY_PERIOD"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "BY" => array(array("action" => "keypress", "code" => "KEY_COMMA"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "CY" => array( // @
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_2"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            "DY" => array( // (
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_9"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            "EY" => array( // )
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_0"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            "FY" => array( // ?
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_SLASH"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            "GY" => array( // <
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_COMMA"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),            
            "HY" => array( // >
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_PERIOD"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            "IY" => array(array("action" => "keypress", "code" => "KEY_SLASH"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "JY" => array(array("action" => "keypress", "code" => "KEY_SEMICOLON"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "KY" => array( // :
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_SEMICOLON"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            "LY" => array( // ~
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_TILDE"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            "MY" => array( // "
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_QUOTE"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            "NY" => array(array("action" => "keypress", "code" => "KEY_QUOTE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "OY" => array( // &
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_7"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            "PY" => array( // {
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_LEFT_BRACE"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            "QY" => array( // "
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_RIGHT_BRACE"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            "RY" => array( // !
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_1"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            "SY" => array(array("action" => "keypress", "code" => "KEY_LEFT_BRACE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "TY" => array(array("action" => "keypress", "code" => "KEY_RIGHT_BRACE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "UY" => array(array("action" => "keypress", "code" => "KEY_BACKSLASH"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "VY" => array(array("action" => "keypress", "code" => "KEY_TILDE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "WY" => array( // _
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_MINUS"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            "XY" => array( // |
                array("action" => "modifierdown", "code" => "MODIFIERKEY_SHIFT"),
                array("action" => "keypress", "code" => "KEY_BACKSLASH"),
                array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")
                ),
            
            
            "ADY" => array(array("action" => "keypress", "code" => "KEY_BACKSPACE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "DGY" => array(array("action" => "keypress", "code" => "KEY_SPACE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "GJY" => array(array("action" => "keypress", "code" => "KEY_ENTER"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "AJY" => array(array("action" => "keypress", "code" => "KEY_TAB"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),

            "Y7" => array(array("action" => "keypress", "code" => "KEY_ESC"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "Y6" => array(array("action" => "keypress", "code" => "KEY_DELETE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            
            "A1" => array("action" => "setmode", "mode" => "numbers"),
            "A2" => array("action" => "setmode", "mode" => "default"),
            "A3" => array("action" => "setmode", "mode" => "functions"),
            "D1" => array("action" => "togglemodifier", "code" => "MODIFIERKEY_SHIFT"),
            "D2" => array("action" => "togglemodifier", "code" => "MODIFIERKEY_CTRL"),
            "D3" => array("action" => "togglemodifier", "code" => "MODIFIERKEY_ALT"),
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
            
            "AY" => array(array("action" => "keypress", "code" => "KEY_F1"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "BY" => array(array("action" => "keypress", "code" => "KEY_F2"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "CY" => array(array("action" => "keypress", "code" => "KEY_F3"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "DY" => array(array("action" => "keypress", "code" => "KEY_F4"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "EY" => array(array("action" => "keypress", "code" => "KEY_F5"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "FY" => array(array("action" => "keypress", "code" => "KEY_F6"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "GY" => array(array("action" => "keypress", "code" => "KEY_F7"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "HY" => array(array("action" => "keypress", "code" => "KEY_F8"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "IY" => array(array("action" => "keypress", "code" => "KEY_F9"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "JY" => array(array("action" => "keypress", "code" => "KEY_F10"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "KY" => array(array("action" => "keypress", "code" => "KEY_F11"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "LY" => array(array("action" => "keypress", "code" => "KEY_F12"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),

            //"MY" => array(array("action" => "keypress", "code" => "KEY_F1"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")), //calc
            "NY" => array("action" => "modifierpress", "code" => "MODIFIERKEY_GUI"),
            "OY" => array(array("action" => "keypress", "code" => "KEY_APP"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "PY" => array(array("action" => "keypress", "code" => "KEY_MUTE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")), //mute
            //"QY" => array(array("action" => "keypress", "code" => "KEY_F5"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")), //vol up
            //"RY" => array(array("action" => "keypress", "code" => "KEY_F6"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")), //vol down
            //"SY" => array(array("action" => "keypress", "code" => "KEY_F7"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")), // play/pause
            //"TY" => array(array("action" => "keypress", "code" => "KEY_F8"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")), // prev track
            //"UY" => array(array("action" => "keypress", "code" => "KEY_F9"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")), // next track
            //"VY" => array(array("action" => "keypress", "code" => "KEY_F10"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")), // stop
            "WY" => array(array("action" => "keypress", "code" => "KEY_INSERT"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "XY" => array(array("action" => "keypress", "code" => "KEY_PRINTSCREEN"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),          
            
            "ADY" => array(array("action" => "keypress", "code" => "KEY_BACKSPACE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "DGY" => array(array("action" => "keypress", "code" => "KEY_SPACE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "GJY" => array(array("action" => "keypress", "code" => "KEY_ENTER"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "AJY" => array(array("action" => "keypress", "code" => "KEY_TAB"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),

            "Y7" => array(array("action" => "keypress", "code" => "KEY_ESC"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            "Y6" => array(array("action" => "keypress", "code" => "KEY_DELETE"), array("action" => "modifierup", "code" => "MODIFIERKEY_SHIFT"), array("action" => "modifierup", "code" => "MODIFIERKEY_CTRL"), array("action" => "modifierup", "code" => "MODIFIERKEY_ALT")),
            
            "A1" => array("action" => "setmode", "mode" => "numbers"),
            "A2" => array("action" => "setmode", "mode" => "symbols"),
            "A3" => array("action" => "setmode", "mode" => "default"),
            "D1" => array("action" => "togglemodifier", "code" => "MODIFIERKEY_SHIFT"),
            "D2" => array("action" => "togglemodifier", "code" => "MODIFIERKEY_CTRL"),
            "D3" => array("action" => "togglemodifier", "code" => "MODIFIERKEY_ALT"),
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

