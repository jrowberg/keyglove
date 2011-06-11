<?php

$touchSet = array(
    "default"=>array(
        "begin"=>array(
            array("action"=>"beep", "tone"=>"2000", "mode"=>"KBEEP_SHORTBEEP", "duration"=>1),
            array("action"=>"redled", "mode"=>"KLED_OFF"),
            array("action"=>"greenled", "mode"=>"KLED_SOLID", "duration"=>0),
            array("action"=>"blueled", "mode"=>"KLED_OFF"),
            ),
        "touch"=>array(
            "J1"=>array("action"=>"mousedown", "button"=>"KMOUSE_LEFT"),
            "J2"=>array("action"=>"mousedown", "button"=>"KMOUSE_MIDDLE"),
            "J3"=>array("action"=>"mousedown", "button"=>"KMOUSE_RIGHT"),
            "A8"=>array("action"=>"mouseon", "mode"=>"KMOUSE_MOVE"),
            "D8"=>array("action"=>"mouseon", "mode"=>"KMOUSE_SCROLL"),
            ),
        "release"=>array(
            "A8"=>array("action"=>"mouseoff", "mode"=>"KMOUSE_MOVE"),
            "D8"=>array("action"=>"mouseoff", "mode"=>"KMOUSE_SCROLL"),
            "J8"=>array(
                array("action"=>"keypress", "code"=>"KEY_T"),
                array("action"=>"keypress", "code"=>"KEY_E"),
                array("action"=>"keypress", "code"=>"KEY_S"),
                array("action"=>"keypress", "code"=>"KEY_T"),
                array("action"=>"keypress", "code"=>"KEY_SPACE"),
                ),

            "J1"=>array("action"=>"mouseup", "button"=>"KMOUSE_LEFT"),
            "J2"=>array("action"=>"mouseup", "button"=>"KMOUSE_MIDDLE"),
            "J3"=>array("action"=>"mouseup", "button"=>"KMOUSE_RIGHT"),

            "GY"=>array("action"=>"keypress", "code"=>"KEY_A"),
            "WY"=>array("action"=>"keypress", "code"=>"KEY_B"),
            "MY"=>array("action"=>"keypress", "code"=>"KEY_C"),
            "FY"=>array("action"=>"keypress", "code"=>"KEY_D"),
            "AY"=>array("action"=>"keypress", "code"=>"KEY_E"),
            "SY"=>array("action"=>"keypress", "code"=>"KEY_F"),
            "QY"=>array("action"=>"keypress", "code"=>"KEY_G"),
            "CY"=>array("action"=>"keypress", "code"=>"KEY_H"),
            "BY"=>array("action"=>"keypress", "code"=>"KEY_I"),
            "Y4"=>array("action"=>"keypress", "code"=>"KEY_J"),
            "RY"=>array("action"=>"keypress", "code"=>"KEY_K"),
            "IY"=>array("action"=>"keypress", "code"=>"KEY_L"),
            "PY"=>array("action"=>"keypress", "code"=>"KEY_M"),
            "EY"=>array("action"=>"keypress", "code"=>"KEY_N"),
            "JY"=>array("action"=>"keypress", "code"=>"KEY_O"),
            "TY"=>array("action"=>"keypress", "code"=>"KEY_P"),
            "XY"=>array("action"=>"keypress", "code"=>"KEY_Q"),
            "KY"=>array("action"=>"keypress", "code"=>"KEY_R"),
            "HY"=>array("action"=>"keypress", "code"=>"KEY_S"),
            "DY"=>array("action"=>"keypress", "code"=>"KEY_T"),
            "LY"=>array("action"=>"keypress", "code"=>"KEY_U"),
            "OY"=>array("action"=>"keypress", "code"=>"KEY_V"),
            "NY"=>array("action"=>"keypress", "code"=>"KEY_W"),
            "UY"=>array("action"=>"keypress", "code"=>"KEY_X"),
            "VY"=>array("action"=>"keypress", "code"=>"KEY_Y"),
            "Z4"=>array("action"=>"keypress", "code"=>"KEY_Z"),
            
            "ADY"=>array("action"=>"keypress", "code"=>"KEY_SPACE"),
            "DGY"=>array("action"=>"keypress", "code"=>"KEY_BACKSPACE"),
            "GJY"=>array("action"=>"keypress", "code"=>"KEY_ENTER"),
            "AJY"=>array("action"=>"keypress", "code"=>"KEY_TAB"),
            
            "A2"=>array("action"=>"keypress", "code"=>"KEY_PERIOD"),
            "AD2"=>array("action"=>"keypress", "code"=>"KEY_COMMA"),
            "ADG2"=>array("action"=>"keypress", "code"=>"KEY_SEMICOLON"),
            
            "AY_J1"=>array("action"=>"setmode", "mode"=>"numbers"),
            "BY_J1"=>array("action"=>"setmode", "mode"=>"symbols"),
            "CY_J1"=>array("action"=>"setmode", "mode"=>"functions"),
            "J1_MY"=>array("action"=>"togglemode", "mode"=>"shift"),
            "J1_NY"=>array("action"=>"togglemode", "mode"=>"ctrl"),
            "J1_OY"=>array("action"=>"togglemode", "mode"=>"alt"),
            
            "Y7"=>array("action"=>"keypress", "code"=>"KEY_ESC"),
            "Y6"=>array("action"=>"modifierpress", "code"=>"MODIFIERKEY_GUI"),
            "Y5"=>array("action"=>"keypress", "code"=>"KEY_APP"),
            
            ),
        ),
    "numbers"=>array(
        "begin"=>array(
            array("action"=>"beep", "tone"=>"2000", "mode"=>"KBEEP_SHORTBEEP", "duration"=>2),
            array("action"=>"redled", "mode"=>"KLED_OFF"),
            array("action"=>"greenled", "mode"=>"KLED_OFF"),
            array("action"=>"blueled", "mode"=>"KLED_SOLID", "duration"=>0),
            ),
        "release"=>array(
            "AY"=>array("action"=>"keypress", "code"=>"KEY_1"),
            "BY"=>array("action"=>"keypress", "code"=>"KEY_2"),
            "CY"=>array("action"=>"keypress", "code"=>"KEY_3"),
            "DY"=>array("action"=>"keypress", "code"=>"KEY_4"),
            "EY"=>array("action"=>"keypress", "code"=>"KEY_5"),
            "FY"=>array("action"=>"keypress", "code"=>"KEY_6"),
            "GY"=>array("action"=>"keypress", "code"=>"KEY_7"),
            "HY"=>array("action"=>"keypress", "code"=>"KEY_8"),
            "IY"=>array("action"=>"keypress", "code"=>"KEY_9"),
            "JY"=>array(
                array("action"=>"modifierdown", "code"=>"MODIFIERKEY_SHIFT"),
                array("action"=>"keypress", "code"=>"KEY_3"),
                array("action"=>"modifierup", "code"=>"MODIFIERKEY_SHIFT"),
                ),
            "KY"=>array("action"=>"keypress", "code"=>"KEY_0"),
            "LY"=>array(
                array("action"=>"modifierdown", "code"=>"MODIFIERKEY_SHIFT"),
                array("action"=>"keypress", "code"=>"KEY_8"),
                array("action"=>"modifierup", "code"=>"MODIFIERKEY_SHIFT"),
                ),
            "AY_J1"=>array("action"=>"setmode", "mode"=>"default"),
            "BY_J1"=>array("action"=>"setmode", "mode"=>"symbols"),
            "CY_J1"=>array("action"=>"setmode", "mode"=>"functions"),
            "J1_MY"=>array("action"=>"togglemode", "mode"=>"shift"),
            "J1_NY"=>array("action"=>"togglemode", "mode"=>"ctrl"),
            "J1_OY"=>array("action"=>"togglemode", "mode"=>"alt"),
            ),
        ),
    "symbols"=>array(
        "begin"=>array(
            array("action"=>"beep", "tone"=>"2000", "mode"=>"KBEEP_SHORTBEEP", "duration"=>3),
            array("action"=>"redled", "mode"=>"KLED_OFF"),
            array("action"=>"greenled", "mode"=>"KLED_SOLID", "duration"=>0),
            array("action"=>"blueled", "mode"=>"KLED_SOLID", "duration"=>0),
            ),
        "release"=>array(
            "AY"=>array("action"=>"keypress", "code"=>"KEY_MINUS"),
            "BY"=>array("action"=>"keypress", "code"=>"KEY_EQUAL"),
            //"CY"=>array("action"=>"keypress", "code"=>"KEY_"),
            "DY"=>array("action"=>"keypress", "code"=>"KEY_LEFT_BRACE"),
            "EY"=>array("action"=>"keypress", "code"=>"KEY_RIGHT_BRACE"),
            "FY"=>array("action"=>"keypress", "code"=>"KEY_BACKSLASH"),
            "GY"=>array("action"=>"keypress", "code"=>"KEY_SEMICOLON"),
            "HY"=>array("action"=>"keypress", "code"=>"KEY_QUOTE"),
            //"IY"=>array("action"=>"keypress", "code"=>"KEY_"),
            "JY"=>array("action"=>"keypress", "code"=>"KEY_COMMA"),
            "KY"=>array("action"=>"keypress", "code"=>"KEY_PERIOD"),
            "LY"=>array("action"=>"keypress", "code"=>"KEY_SLASH"),
            "AY_J1"=>array("action"=>"setmode", "mode"=>"numbers"),
            "BY_J1"=>array("action"=>"setmode", "mode"=>"default"),
            "CY_J1"=>array("action"=>"setmode", "mode"=>"functions"),
            "J1_MY"=>array("action"=>"togglemode", "mode"=>"shift"),
            "J1_NY"=>array("action"=>"togglemode", "mode"=>"ctrl"),
            "J1_OY"=>array("action"=>"togglemode", "mode"=>"alt"),
            ),
        ),
    "functions"=>array(
        "begin"=>array(
            array("action"=>"beep", "tone"=>"2000", "mode"=>"KBEEP_SHORTBEEP", "duration"=>4),
            array("action"=>"redled", "mode"=>"KLED_SOLID", "duration"=>0),
            array("action"=>"greenled", "mode"=>"KLED_OFF"),
            array("action"=>"blueled", "mode"=>"KLED_OFF"),
            ),
        "release"=>array(
            "AY"=>array("action"=>"keypress", "code"=>"KEY_F1"),
            "BY"=>array("action"=>"keypress", "code"=>"KEY_F2"),
            "CY"=>array("action"=>"keypress", "code"=>"KEY_F3"),
            "DY"=>array("action"=>"keypress", "code"=>"KEY_F4"),
            "EY"=>array("action"=>"keypress", "code"=>"KEY_F5"),
            "FY"=>array("action"=>"keypress", "code"=>"KEY_F6"),
            "GY"=>array("action"=>"keypress", "code"=>"KEY_F7"),
            "HY"=>array("action"=>"keypress", "code"=>"KEY_F8"),
            "IY"=>array("action"=>"keypress", "code"=>"KEY_F9"),
            "JY"=>array("action"=>"keypress", "code"=>"KEY_F10"),
            "KY"=>array("action"=>"keypress", "code"=>"KEY_F11"),
            "LY"=>array("action"=>"keypress", "code"=>"KEY_F12"),
            "AY_J1"=>array("action"=>"setmode", "mode"=>"numbers"),
            "BY_J1"=>array("action"=>"setmode", "mode"=>"symbols"),
            "CY_J1"=>array("action"=>"setmode", "mode"=>"default"),
            "J1_MY"=>array("action"=>"togglemode", "mode"=>"shift"),
            "J1_NY"=>array("action"=>"togglemode", "mode"=>"ctrl"),
            "J1_OY"=>array("action"=>"togglemode", "mode"=>"alt"),
            ),
        ),
    "caps"=>array(
        "options"=>array(
            "fallthrough"=>array("default", "numbers", "symbols"),
            ),
        "begin"=>array(
            array("action"=>"beep", "tone"=>"1000", "mode"=>"KBEEP_SHORTPULSE", "duration"=>1),
            array("action"=>"greenled", "mode"=>"KLED_LONGBLINK", "duration"=>0),
            array("action"=>"keypress", "code"=>"KEY_CAPS_LOCK"),
            ),
        "end"=>array(
            array("action"=>"keypress", "code"=>"KEY_CAPS_LOCK"),
            ),
        ),
    "shift"=>array(
        "options"=>array(
            "fallthrough"=>array("default", "numbers"),
            ),
        "begin"=>array(
            array("action"=>"beep", "tone"=>"1000", "mode"=>"KBEEP_SHORTPULSE", "duration"=>1),
            array("action"=>"greenled", "mode"=>"KLED_SHORTBLINK", "duration"=>0),
            array("action"=>"modifierdown", "code"=>"MODIFIERKEY_SHIFT"),
            ),
        "end"=>array(
            array("action"=>"modifierup", "code"=>"MODIFIERKEY_SHIFT"),
            ),
        ),
    "ctrl"=>array(
        "options"=>array(
            "fallthrough"=>array("default", "numbers"),
            ),
        "begin"=>array(
            array("action"=>"beep", "tone"=>"1200", "mode"=>"KBEEP_SHORTPULSE", "duration"=>1),
            array("action"=>"redled", "mode"=>"KLED_SHORTBLINK", "duration"=>0),
            array("action"=>"modifierdown", "code"=>"MODIFIERKEY_CTRL"),
            ),
        "end"=>array(
            array("action"=>"modifierup", "code"=>"MODIFIERKEY_CTRL"),
            ),
        ),
    "alt"=>array(
        "options"=>array(
            "fallthrough"=>array("default", "numbers"),
            ),
        "begin"=>array(
            array("action"=>"beep", "tone"=>"1400", "mode"=>"KBEEP_SHORTPULSE", "duration"=>1),
            array("action"=>"blueled", "mode"=>"KLED_SHORTBLINK", "duration"=>0),
            array("action"=>"modifierdown", "code"=>"MODIFIERKEY_ALT"),
            ),
        "end"=>array(
            array("action"=>"modifierup", "code"=>"MODIFIERKEY_ALT"),
            ),
        ),
    "gui"=>array(
        "options"=>array(
            "fallthrough"=>array("default", "numbers"),
            ),
        "begin"=>array(
            array("action"=>"beep", "tone"=>"1600", "mode"=>"KBEEP_SHORTPULSE", "duration"=>1),
            array("action"=>"greenled", "mode"=>"KLED_SHORTBLINK", "duration"=>0),
            array("action"=>"modifierdown", "code"=>"MODIFIERKEY_GUI"),
            ),
        "end"=>array(
            array("action"=>"modifierup", "code"=>"MODIFIERKEY_GUI"),
            ),
        ),
);


