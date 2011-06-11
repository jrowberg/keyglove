<?php

/* touchset.php
 * 
 * Preliminary automatic Keyglove touchset code generation script
 * Jeff Rowberg <jeff@rowberg.net>
 * 4/5/2011
 */
//error_reporting(0);

require('touchset-mikenz.php');
//require('touchset-jrowberg.php');

// =============================================================================

// useful for "sendkeys"-style debugging
$keymap = array(
    "A"=>"a",
    "B"=>"b",
    "C"=>"c",
    "D"=>"d",
    "E"=>"e",
    "F"=>"f",
    "G"=>"g",
    "H"=>"h",
    "I"=>"i",
    "J"=>"j",
    "K"=>"k",
    "L"=>"l",
    "M"=>"m",
    "N"=>"n",
    "O"=>"o",
    "P"=>"p",
    "Q"=>"q",
    "R"=>"r",
    "S"=>"s",
    "T"=>"t",
    "U"=>"u",
    "V"=>"v",
    "W"=>"w",
    "X"=>"x",
    "Y"=>"y",
    "Z"=>"z",

    "0"=>"0",
    "1"=>"1",
    "2"=>"2",
    "3"=>"3",
    "4"=>"4",
    "5"=>"5",
    "6"=>"6",
    "7"=>"7",
    "8"=>"8",
    "9"=>"9",

    "BKSP"=>"{bksp}",
    "SPACE"=>" ",
    "ENTER"=>"{enter}",
    "TAB"=>"{tab}",

    "BACKTICK"=>"`",
    "MINUS"=>"-",
    "EQUALS"=>"=",
    "BACKSLASH"=>"\\",

    "CAPS"=>"{caps}",
    "LSHIFT"=>"+",
    "LCTRL"=>"^",
    "LALT"=>"%",
    "LGUI"=>"{gui}",
    "APPS"=>"{apps}",
    "ESC"=>"{esc}",

    "F1"=>"{f1}",
    "F2"=>"{f2}",
    "F3"=>"{f3}",
    "F4"=>"{f4}",
    "F5"=>"{f5}",
    "F6"=>"{f6}",
    "F7"=>"{f7}",
    "F8"=>"{f8}",
    "F9"=>"{f9}",
    "F10"=>"{f10}",
    "F11"=>"{f11}",
    "F12"=>"{f12}",
);

// descriptive sensor reference
$sensors = array(
    "A"=>"index finger tip",
    "B"=>"index finger middle",
    "C"=>"index finger bottom",
    "D"=>"middle finger tip",
    "E"=>"middle finger middle",
    "F"=>"middle finger bottom",
    "G"=>"ring finger tip",
    "H"=>"ring finger middle",
    "I"=>"ring finger bottom",
    "J"=>"little finger tip",
    "K"=>"little finger middle",
    "L"=>"little finger bottom",
    "M"=>"index side tip",
    "N"=>"index side middle",
    "O"=>"index side bottom",
    "P"=>"middle side tip",
    "Q"=>"middle side middle",
    "R"=>"middle side bottom",
    "S"=>"ring side tip",
    "T"=>"ring side middle",
    "U"=>"ring side bottom",
    "V"=>"little side tip",
    "W"=>"little side middle",
    "X"=>"little side bottom",
    "Y"=>"thumb tip",
    "Z"=>"thumb segment",
    "1"=>"palm top",
    "2"=>"palm middle",
    "3"=>"palm bottom",
    "4"=>"index fingernail",
    "5"=>"middle fingernail",
    "6"=>"ring fingernail",
    "7"=>"little fingernail",
    "8"=>"thumbnail",
);

// base 1-to-1 touches with VERY IMPORTANT precedence order
/*$touches = array(
    "Z7",
    "Y7",
    "J8",
    "JY",
    "KY",
    "LY",
    "VY",
    "WY",
    "XY",
    
    "Z6",
    "Y6",
    "G8",
    "GY",
    "HY",
    "IY",
    "SY",
    "TY",
    "UY",
    "G7",
    
    "Z5",
    "Y5",
    "D8",
    "DY",
    "EY",
    "FY",
    "PZ",
    "PY",
    "QZ",
    "QY",
    "RZ",
    "RY",
    "D4",
    "D6",
    "D7",
    "DM",
    
    "Z4",
    "Y4",
    "A8",
    "AY",
    "BY",
    "CY",
    "MZ",
    "MY",
    "NZ",
    "NY",
    "OZ",
    "OY",
    
    "A3",
    "A2",
    "A1",
    "D3",
    "D2",
    "D1",
    "G3",
    "G2",
    "G1",
    "J3",
    "J2",
    "J1",
    
    "Y1",
);*/
$touches = array(
    "Z7",
    "Y7",
    "J8",
    "JY",
    "KY",
    "LY",
    "HY",
    "VY",
    "WY",
    "XY",
    
    "Z6",
    "Y6",
    "G8",
    "GY",
    "IY",
    "SY",
    "TY",
    "UY",
    "G7",
    
    "Z5",
    "Y5",
    "D8",
    "DY",
    "EY",
    "FY",
    "PZ",
    "PY",
    "QZ",
    "QY",
    "RZ",
    "RY",
    "D4",
    "D6",
    "D7",
    "DM",
    
    "Z4",
    "Y4",
    "A8",
    "MY",
    "AY",
    "BY",
    "CY",
    "MZ",
    "NZ",
    "NY",
    "OZ",
    "OY",
    
    "A3",
    "A2",
    "A1",
    "D3",
    "D2",
    "D1",
    "G3",
    "G2",
    "G1",
    "J3",
    "J2",
    "J1",
    
    "Y1",
);


// list of physically impossible touch combinations (e.g. AY+A2)
$impossible = array(
        /* // template (includes all combinations)
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A1", "A2", "A3", "A8",
        "D1", "D2", "D3", "D4", "D6", "D7", "D8",
        "G1", "G2", "G3", "G7", "G8",
        "J1", "J2", "J3", "J8",
        "DM",
        */
    "Y7" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6",
        "Z7",
        "A2", "A3",
        "D2", "D3", "D7",
        "G2", "G3", "G7",
        "J2", "J3", "J8",
    ),
    "J8" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y4", "Y5", "Y6", "Y7",
        "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G2", "G3", "G7", "G8",
        "J1", "J2", "J3", "J8",
    ),
    "JY" => array(
        "BY", "CY", "EY", "FY", "HY", "IY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G2", "G3", "G7", "G8",
        "J1", "J2", "J3", "J8",
    ),
    "KY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G2", "G3", "G7", "G8",
        "J1", "J2", "J3", "J8",
    ),
    "LY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G2", "G3", "G7", "G8",
        "J1", "J2", "J3", "J8",
    ),
    "VY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G2", "G3", "G7", "G8",
        "J2", "J3", "J8",
    ),
    "WY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G2", "G3", "G7", "G8",
        "J2", "J3", "J8",
    ),
    "XY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G2", "G3", "G7", "G8",
        "J2", "J3", "J8",
    ),
    "Y6" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G2", "G3", "G7", "G8",
    ),
    "G8" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y4", "Y5", "Y6", "Y7",
        "Z6", "Z7",
        "A1", "A2", "A3", "A8",
        "D1", "D2", "D3", "D4", "D6", "D8",
        "G1", "G2", "G3", "G7", "G8",
        "J8",
    ),
    "GY" => array(
        "BY", "CY", "EY", "FY", "HY", "IY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G1", "G2", "G3", "G7", "G8",
        "J1", "J2", "J3", "J8",
    ),
    "HY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G1", "G2", "G3", "G7", "G8",
        "J1", "J2", "J3", "J8",
    ),
    "IY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G1", "G2", "G3", "G7", "G8",
        "J1", "J2", "J3", "J8",
    ),
    "SY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G2", "G3", "G7", "G8",
        "J8",
    ),
    "TY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G2", "G3", "G7", "G8",
        "J8",
    ),
    "UY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G2", "G3", "G7", "G8",
        "J8",
    ),
    "G7" => array(
        "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z6", "Z7",
        "G1", "G2", "G3", "G7", "G8",
        "J1", "J2", "J3", "J8",
    ),
    "Y5" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "J1", "J2", "J3", "J8",
        "DM",
    ),
    "D8" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y4", "Y5", "Y6", "Y7",
        "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D1", "D2", "D3", "D4", "D6", "D8",
        "G8",
        "J8",
        "DM",
    ),
    "DY" => array(
        "BY", "CY", "EY", "FY", "HY", "IY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z5", "Z6", "Z7",
        "A2", "A3",
        "D1", "D2", "D3", "D4", "D6", "D8",
        "DM",
    ),
    "EY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z5", "Z6", "Z7",
        "A2", "A3",
        "D1", "D2", "D3", "D4", "D6", "D8",
        "DM",
    ),
    "FY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z5", "Z6", "Z7",
        "A2", "A3",
        "D1", "D2", "D3", "D4", "D6", "D8",
        "DM",
    ),
    "PZ" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D1", "D2", "D3", "D4", "D6", "D8",
        "G8",
        "J8",
        "DM",
    ),
    "PY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G8",
        "J8",
        "DM",
    ),
    "QZ" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D1", "D2", "D3", "D4", "D6", "D8",
        "G8",
        "J8",
        "DM",
    ),
    "QY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G8",
        "J8",
        "DM",
    ),
    "RZ" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D1", "D2", "D3", "D4", "D6", "D8",
        "G8",
        "J8",
        "DM",
    ),
    "RY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G8",
        "J8",
        "DM",
    ),
    "D4" => array(
        "DY", "EY", "FY", "PY", "QY", "RY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y4", "Y5",
        "Z4", "Z5",
        "A1", "A2", "A3", "A8",
        "D1", "D2", "D3", "D4", "D6", "D8",
    ),
    "D6" => array(
        "DY", "EY", "FY",
        "PZ", "QZ", "RZ",
        "Y5", "Y6",
        "Z5", "Z6", "Z7",
        "D1", "D2", "D3", "D4", "D6", "D8",
        "G1", "G2", "G3", "G7", "G8",
        "J1", "J2", "J3",
        "DM",
    ),
    "D7" => array(
        "DY", "EY", "FY", "GY", "IY", "SY", "TY", "UY",
        "Y6", "Y7",
        "Z5", "Z6", "Z7",
        "D1", "D2", "D3", "D4", "D6", "D7", "D8",
        "G1", "G2", "G3", "G7", "G8",
        "J1", "J2", "J3", "J8",
        "DM",
    ),
    "DM" => array(
        "DY", "EY", "FY", "MY", "QY", "RY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y4", "Y5",
        "Z4", "Z5",
        "A1", "A2", "A3", "A8",
        "D1", "D2", "D3", "D4", "D6", "D8",
    ),
    "Y4" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "DM",
    ),
    "A8" => array(
        "AY", "BY", "CY", "EY", "FY", "HY", "IY", "KY", "LY", "MY", "NY", "OY", "RY", "UY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4",
        "Z4", "Z5", "Z6", "Z7",
        "A1", "A2", "A3", "A8",
        "D8",
        "G8",
        "J8",
        "DM",
    ),
    "AY" => array(
        "BY", "CY", "EY", "FY", "HY", "IY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A1", "A2", "A3", "A8",
        "D8",
        "G8",
        "J8",
    ),
    "BY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A1", "A2", "A3", "A8",
        "D8",
        "G8",
        "J8",
    ),
    "CY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A1", "A2", "A3", "A8",
        "D8",
        "G8",
        "J8",
    ),
    "MZ" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4",
        "Z4", "Z5", "Z6", "Z7",
        "A1", "A2", "A3", "A8",
        "DM",
    ),
    "MY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A8",
        "D8",
        "G8",
        "J8",
        "DM",
    ),
    "NZ" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4",
        "Z4", "Z5", "Z6", "Z7",
        "A1", "A2", "A3", "A8",
        "DM",
    ),
    "NY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A8",
        "D8",
        "G8",
        "J8",
    ),
    "OZ" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4",
        "Z4", "Z5", "Z6", "Z7",
        "A1", "A2", "A3", "A8",
        "DM",
    ),
    "OY" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A8",
        "D8",
        "G8",
        "J8",
    ),
    "Z4" => array(
        "AY", "BY", "CY", "FY", "MY", "NY", "OY", "RY", "UY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y4",
        "A2", "A3", "A8",
        "D4",
        "DM",
    ),
    "Z5" => array(
        "CY", "DY", "EY", "FY", "IY", "PY", "QY", "RY", "UY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y5",
        "A2", "A3", "A8",
        "D1", "D2", "D3", "D4", "D6", "D8",
        "DM",
    ),
    "Z6" => array(
        "CY", "FY", "GY", "HY", "IY", "NY", "OY", "SY", "TY", "UY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y6",
        "A2", "A3",
        "D2", "D3", "D4", "D6",
        "G1", "G2", "G3", "G7", "G8",
    ),
    "Z7" => array(
        "BY", "CY", "EY", "FY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G2", "G3", "G7", "G8",
        "J1", "J2", "J3", "J8",
    ),
    "A1" => array(
        "AY", "BY", "CY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Z4", "Z5", "Z6", "Z7",
        "A1", "A2", "A3", "A8",
        "D4",
        "DM",
    ),
    "A2" => array(
        "AY", "BY", "CY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4",
        "Z4", "Z5", "Z6", "Z7",
        "A1", "A2", "A3", "A8",
        "D4",
        "DM",
    ),
    "A3" => array(
        "AY", "BY", "CY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4",
        "Z4", "Z5", "Z6", "Z7",
        "A1", "A2", "A3", "A8",
        "D4",
        "DM",
    ),
    "D1" => array(
        "DY", "EY", "FY", "GY", "HY", "IY", "SY", "TY", "UY",
        "PZ", "QZ", "RZ",
        "Z5", "Z6", "Z7",
        "D1", "D2", "D3", "D4", "D6", "D7", "D8",
        "G7", "G8",
        "J8",
        "DM",
    ),
    "D2" => array(
        "DY", "EY", "FY", "GY", "HY", "IY", "SY", "TY", "UY",
        "PZ", "QZ", "RZ",
        "Y1", "Y5",
        "Z5", "Z6", "Z7",
        "D1", "D2", "D3", "D4", "D6", "D7", "D8",
        "G7", "G8",
        "J8",
        "DM",
    ),
    "D3" => array(
        "DY", "EY", "FY", "GY", "HY", "IY", "SY", "TY", "UY",
        "PZ", "QZ", "RZ",
        "Y1", "Y5",
        "Z5", "Z6", "Z7",
        "D1", "D2", "D3", "D4", "D6", "D7", "D8",
        "G7", "G8",
        "J8",
        "DM",
    ),
    "G1" => array(
        "GY", "HY", "IY", "WY", "XY",
        "Z6", "Z7",
        "D6",
        "G1", "G2", "G3", "G7", "G8",
    ),
    "G2" => array(
        "GY", "HY", "IY", "WY", "XY",
        "Y6",
        "Z6", "Z7",
        "D6",
        "G1", "G2", "G3", "G7", "G8",
    ),
    "G3" => array(
        "GY", "HY", "IY", "WY", "XY",
        "Y6",
        "Z6", "Z7",
        "D6",
        "G1", "G2", "G3", "G7", "G8",
    ),
    "J1" => array(
        "JY", "KY", "LY",
        "Z7",
        "D7",
        "G7",
        "J1", "J2", "J3", "J8",
        "DM",
    ),
    "J2" => array(
        "JY", "KY", "LY",
        "Z7",
        "D7",
        "G7",
        "J1", "J2", "J3", "J8",
        "DM",
    ),
    "J3" => array(
        "JY", "KY", "LY",
        "Z7",
        "D7",
        "G7",
        "J1", "J2", "J3", "J8",
        "DM",
    ),
    "Y1" => array(
        "AY", "BY", "CY", "DY", "EY", "FY", "GY", "HY", "IY", "JY", "KY", "LY", "MY", "NY", "OY", "PY", "QY", "RY", "SY", "TY", "UY", "VY", "WY", "XY",
        "MZ", "NZ", "OZ", "PZ", "QZ", "RZ",
        "Y1", "Y4", "Y5", "Y6", "Y7",
        "Z4", "Z5", "Z6", "Z7",
        "A2", "A3", "A8",
        "D2", "D3", "D4", "D6", "D8",
        "G2", "G3", "G7", "G8",
        "J2", "J3",
    ),
);

// recursively builds all possible combinations (including multitouches)
function build($stack=array()) {
    global $touches;
    global $impossible;
    global $combinations;
    foreach ($touches as $index=>$touch) {
        if (empty($stack)) {
            // first layer, so use all touches
            array_push($stack, $touch);
            build($stack);
            $combinations[] = $stack;
            array_pop($stack);
        } else if (!in_array($touch, $stack)) {
            // stack isn't empty and touch isn't in stack, so build all sub-combinations
            foreach ($stack as $test) if (in_array($touch, $impossible[$test]) || in_array($test, $impossible[$touch])) continue 2; // won't work
            array_push($stack, $touch);
            $com = $stack;
            sort($com);
            if (in_array($com, $combinations)) {
                array_pop($stack);
                continue; // already built this one
            }
            build($stack);
            $combinations[] = $com;
            array_pop($stack);
        }
    }
}

// orders any given touch alphabetically by group (input is an array of base touches, not a string representation of all)
function reformatTouch($stack) {
    $rebuild = array();
    $newStack = array();
    sort($stack);
    foreach ($stack as $part) {
        if (is_array($part)) {
            $part = current($part);
        }
        if ($part[0] == "Z") {
            $rebuild[$part[0]] = isset($rebuild[$part[0]]) ? $rebuild[$part[0]] . $part[1] : $part[1];
        } elseif (isset($part[1])) {
            $rebuild[$part[1]] = isset($rebuild[$part[1]]) ? $rebuild[$part[1]] . $part[0] : $part[0];
        }
    }
    foreach ($rebuild as $last=>$first) {
        if ($last == "Z") {
            $newStack[] = $last . $first;
        } else {
            $newStack[] = $first . $last;
        }
    }
    
    return $newStack;
}

// returns a string representation of a touch array
function displayTouch($stack) {
    global $sensors;
    $disp = array();
    foreach ($stack as $part) {
        $cur = array();
        for ($i = 0; $i < strlen($part); $i++) $cur[] = $sensors[$part[$i]];
        $disp[] = join(", ", $cur);
    }
    echo join('_', $stack).":\n- ".join("\n- ", $disp)."\n\n";
}

// sorts touches according to precedence
function sortTouch($a, $b) {
    global $touches;
    $aScore = array_search($a, $touches);
    $bScore = array_search($b, $touches);
    if ($aScore < $bScore) return -1;
    if ($aScore > $bScore) return 1;
    return 0; // equal? hmm...
}

// sorts a whole set of touches according to precedence
function sortTouches($a, $b) {
    global $touches;
    $aScore = array();
    $bScore = array();
    foreach ($a as $t) $aScore[] = array_search($t, $touches);
    foreach ($b as $t) $bScore[] = array_search($t, $touches);
    sort($aScore);
    sort($bScore);
    for ($i = 0; $i < count($aScore) && $i < count($bScore); $i++) {
        if ($aScore[$i] < $bScore[$i]) return -1;
        if ($aScore[$i] > $bScore[$i]) return 1;
    }
    if (count($aScore) > count($bScore)) return -1;
    if (count($aScore) < count($bScore)) return 1;
    return 0; // equal? hmm...
}

// returns the numeric value of any given touch (takes array input, not string representation)
function touchValues($stack) {
    global $touches;
    $v1 = (float)0;
    $v2 = (float)0;
    foreach ($stack as $touch) {
        $pos = array_search($touch, $touches);
        if ($pos < 32) {
            $v1 |= 1 << $pos;
        } else {
            $v2 |= 1 << $pos - 32;
        }
    }
    return array($v1, $v2);
}

// generates the code used by the Arduino sketch
function behaviorCode($level, $touch, $def) {
    global $keymap;
    if (!$def || empty($def)) return str_repeat('    ', $level + 1)."// EMPTY\n";
    $code = '';
    if (is_array($def)) {
        if (isset($def["action"])) $def = array($def);
        foreach ($def as $behavior) {
            $code .= str_repeat('    ', $level + 1);
            switch ($behavior["action"]) {
                case "setmode":
                    $friendly = "KMODE_".strtoupper(preg_replace('/[^a-zA-Z0-9]/', '', $behavior["mode"]));
                    $code .= 'setmode('.$friendly.');'."\n";
                    break;
                case "togglemode":
                    $friendly = "KMODE_".strtoupper(preg_replace('/[^a-zA-Z0-9]/', '', $behavior["mode"]));
                    $code .= 'togglemode('.$friendly.');'."\n";
                    break;
                case "pushmode":
                    $friendly = "KMODE_".strtoupper(preg_replace('/[^a-zA-Z0-9]/', '', $behavior["mode"]));
                    $code .= 'pushmode('.$friendly.');'."\n";
                    break;
                case "popmode":
                    $code .= 'popmode();'."\n";
                    break;
                case "keydown":
                    $code .= 'keydown('.$behavior["code"].');'."\n";
                    break;
                case "keyup":
                    $code .= 'keyup('.$behavior["code"].');'."\n";
                    break;
                case "keypress":
                    $code .= 'keypress('.$behavior["code"].');'."\n";
                    break;
                case "modifierdown":
                    $code .= 'modifierdown('.$behavior["code"].');'."\n";
                    break;
                case "modifierup":
                    $code .= 'modifierup('.$behavior["code"].');'."\n";
                    break;
                case "modifierpress":
                    $code .= 'modifierpress('.$behavior["code"].');'."\n";
                    break;
                case "mousedown":
                    $code .= 'mousedown('.$behavior["button"].');'."\n";
                    break;
                case "mouseup":
                    $code .= 'mouseup('.$behavior["button"].');'."\n";
                    break;
                case "mouseclick":
                    $code .= 'mouseclick('.$behavior["button"].');'."\n";
                    break;
                case "mouseon":
                    $code .= 'mouseon('.$behavior["mode"].');'."\n";
                    break;
                case "mouseoff":
                    $code .= 'mouseoff('.$behavior["mode"].');'."\n";
                    break;
                case "redled":
                    $code .= 'redled('.$behavior["mode"].', '.(isset($behavior["duration"]) ? $behavior["duration"] : 0).');'."\n";
                    break;
                case "greenled":
                    $code .= 'greenled('.$behavior["mode"].', '.(isset($behavior["duration"]) ? $behavior["duration"] : 0).');'."\n";
                    break;
                case "blueled":
                    $code .= 'blueled('.$behavior["mode"].', '.(isset($behavior["duration"]) ? $behavior["duration"] : 0).');'."\n";
                    break;
                case "vibrate":
                    $code .= 'vibrate('.$behavior["mode"].', '.(isset($behavior["duration"]) ? $behavior["duration"] : 0).');'."\n";
                    break;
                case "beep":
                    $code .= 'beep('.$behavior["tone"].', '.$behavior["mode"].', '.$behavior["duration"].');'."\n";
                    break;
                default:
                    $code .= '// UNKNOWN: '.str_replace("\n", " ", var_export($behavior, true))."\n";
                    break;
            }
        }
        
        $code .= str_repeat('    ', $level + 1) . "return;\n";
        return $code;
    } else {
        // IGNORED NOW
        return '{
keypress('.$def.');
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release '.$touch.'");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys '.$keymap[substr($def, 4)].'");
#endif
}';
    }
}

// stores all possible combinations generated by build()
$combinations = array();

/* Load complete array of configured touch combinations from cache (or force regeneration) */
$forceRegen = false;
if ($forceRegen || !file_exists(dirname(__FILE__) . '/combinations.cache')) {
    /* Cache file missing, create the array of combinations */
    build();
    file_put_contents(dirname(__FILE__) . '/combinations.cache', serialize($combinations));
} else {
    $combinations = unserialize(file_get_contents(dirname(__FILE__) . '/combinations.cache'));
}

// gather list of all touches used in touchset
$setCombinations["touch"] = array();
$setCombinations["release"] = array();
foreach ($touchSet as $mode=>$set) {
    if (isset($set["touch"]) && is_array($set["touch"])) {
        foreach ($set["touch"] as $com=>$action) {
            if (!in_array($com, $setCombinations["touch"])) $setCombinations["touch"][] = $com;
        }
    }
    if (isset($set["touch"]) && is_array($set["touch"])) {
        foreach ($set["release"] as $com=>$action) {
            if (!in_array($com, $setCombinations["release"])) $setCombinations["release"][] = $com;
        }
    }
}

// reformat and priority-sort list of all touches used
$useCombinations = array();
$useCombinations["touch"] = array();
$useCombinations["release"] = array();
$useCombinations["all"] = array();
foreach ($combinations as $com) {
    if (in_array(join('_', reformatTouch($com)), $setCombinations["touch"])) {
        $useCombinations["touch"][] = $com;
        if (!in_array($com, $useCombinations["all"])) $useCombinations["all"][] = $com;
        foreach ($com as $part) if (!in_array(array($part), $useCombinations["all"])) $useCombinations["all"][] = array($part);
    }
    if (in_array(join('_', reformatTouch($com)), $setCombinations["release"])) {
        $useCombinations["release"][] = $com;
        if (!in_array($com, $useCombinations["all"])) $useCombinations["all"][] = $com;
        foreach ($com as $part) if (!in_array(array($part), $useCombinations["all"])) $useCombinations["all"][] = array($part);
    }
}

// sort all created touch arrays by precedence
usort($useCombinations["all"], 'sortTouches');
usort($useCombinations["touch"], 'sortTouches');
usort($useCombinations["release"], 'sortTouches');

echo '// Keyglove controller source code - Custom touchset definition
// 4/5/2011 by Jeff Rowberg <jeff@rowberg.net>
// (Generated '.date("r").' @ ' . php_uname('n') . ')

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

';

echo '// TOTAL: '.count($useCombinations["all"]).' unique combinations used, '.count($touchSet).' modes
// ===============================================================

';

/*echo '#define KG_TOTAL_SENSORS '.count($sensors).'
#define KG_BASE_COMBINATIONS '.count($touches)."\n\n";*/

//$temp = array();
//foreach ($sensorPins as $sensor=>$pin) $temp[] = $pin;
//echo 'int pins[KG_TOTAL_SENSORS] = { '.join(', ', $temp).' };'."\n";

//$temp = array();
//foreach ($touches as $touch) $temp[] = '    { '.$sensorPins[$touch[0]].', '.$sensorPins[$touch[1]].' /* '.$touch.' */ }';
//echo 'int combinations[KG_BASE_COMBINATIONS][2] = {'."\n".join(",\n", $temp)."\n};\n\n";*/

echo '// numeric mode definitions'."\n\n";
$modeConstants = array();
$modeNum = 0;
foreach ($touchSet as $mode=>$modeSet) {
    $friendly = "KMODE_".strtoupper(preg_replace('/[^a-zA-Z0-9]/', '', $mode));
    $modeConstants[$mode] = $friendly;
    echo '#define '.$friendly.' '.($modeNum++)."\n";
}
echo "\n";

echo "void activate_mode(int mode) {\n";
echo "    switch (mode) {\n";
foreach ($touchSet as $mode=>$modeSet) {
    if (isset($modeSet["begin"])) {
        $friendly = "KMODE_".strtoupper(preg_replace('/[^a-zA-Z0-9]/', '', $mode));
        echo "        case ".$friendly.":\n";
        echo behaviorCode(2, "", $modeSet["begin"]);
        echo "            break;\n";
    }
}
echo "    }\n";
echo "}\n\n";


echo "void deactivate_mode(int mode) {\n";
echo "    switch (mode) {\n";
foreach ($touchSet as $mode=>$modeSet) {
    if (isset($modeSet["end"])) {
        $friendly = "KMODE_".strtoupper(preg_replace('/[^a-zA-Z0-9]/', '', $mode));
        echo "        case ".$friendly.":\n";
        echo behaviorCode(2, "", $modeSet["end"]);
        echo "            break;\n";
    }
}
echo "    }\n";
echo "}\n\n";

echo '// numeric sensor definitions'."\n\n";

foreach ($useCombinations["all"] as $com) {
    $val = touchValues($com);
    echo "#define KS1_" . join('_', reformatTouch($com)) . " 0x" . dechex($val[0]) . "\n";
    echo "#define KS2_" . join('_', reformatTouch($com)) . " 0x" . dechex($val[1]) . "\n\n";
}

// TOUCH/RELEASE TESTS
$tests = array('touch', 'release');
foreach ($tests as $test) {
    echo "void check_sensors_$test(long unsigned int sensors1, long unsigned int sensors2, int pos) {\n";

    $layers = array();
    foreach ($useCombinations[$test] as $com) {
        if (count($com) > 1) {
            usort($com, 'sortTouch');
            $layers[$com[0]][$com[1]][] = $com;
        } else {
            $layers[$com[0]][0][] = $com;
        }            
    }
    
    $done1 = 0;
    foreach ($touches as $pos1 => $touch1) {
        if (!isset($layers[$touch1])) {
            continue;
        }
        
        $var1 = $pos1 < 32 ? 1 : 2;
        if (count($layers[$touch1]) > 1) {
            echo ($done1++ > 0 ? " else " : "    ") . "if ((sensors1 & KS1_$touch1) == KS1_$touch1 && (sensors2 & KS2_$touch1) == KS2_$touch1) {\n";
            $done2 = 0;
            
            foreach ($touches as $pos2 => $touch2) {
                $var2 = $pos2 < 32 ? 1 : 2;
                if (isset($layers[$touch1][$touch2]) && count($layers[$touch1][$touch2]) > 1) {
                    echo ($done2 > 0 ? " else " : "        ")."if ((sensors1 & KS1_$touch2) == KS1_$touch2 && (sensors2 & KS2_$touch2) == KS2_$touch2) {\n";
                    foreach ($layers[$touch1][$touch2] as $pos3=>$com) {
                        $touch = join('_', reformatTouch($com));
                        
                        /* Get all the modes with behaviors for this touch */
                        $modesWithEvents = array();
                        foreach ($touchSet as $mode => $modeSet) {
                            if (isset($modeSet[$test]) && isset($modeSet[$test][$touch])) {
                                $modesWithEvents[$mode] = $modeSet[$test][$touch];
                            }
                        }
                        
                        /* How many modes with behaviors? */
                        if (count($modesWithEvents) == 1) {
                            /* Just one */
                            $mode = current(array_keys($modesWithEvents));
                            $behavior = current($modesWithEvents);
                            echo ($pos3 > 0 ? " else " : "            ") . "if ((sensors1 & KS1_$touch) == KS1_$touch && (sensors2 & KS2_$touch) == KS2_$touch && modeCheck($modeConstants[$mode], pos)) {\n";
                            echo behaviorCode(3, $touch, $behavior);
                            echo "            }";
                        } else {
                            /* Output the behaviors */
                            echo ($pos3 > 0 ? " else " : "            ") . "if ((sensors1 & KS1_$touch) == KS1_$touch && (sensors2 & KS2_$touch) == KS2_$touch) {\n";
                            $first3 = true;
                            foreach ($modesWithEvents as $mode => $behavior) {
                                echo ($first3 ? '                ' : ' else ') . 'if (modeCheck(' . $modeConstants[$mode] . ', pos)) {'."\n";
                                echo behaviorCode(4, $touch, $behavior);
                                echo '                }';
                                $first3 = false;
                            }
                            echo "\n            }";
                        }
                    }
                    echo "\n        }";
                    $done2++;
                } elseif (isset($layers[$touch1][$touch2]) && count($layers[$touch1][$touch2]) == 1) {
                    $com = current($layers[$touch1][$touch2]);
                    $touch = join('_', reformatTouch($com));
                        
                    /* Get all behaviors for this touch */
                    $touch_behaviors = array();
                    foreach ($touchSet as $mode => $modeSet) {
                        if (isset($modeSet[$test]) && isset($modeSet[$test][$touch])) {
                            $touch_behaviors[serialize($modeSet[$test][$touch])][] = $modeConstants[$mode];
                        }
                    }
                            
                    /* How many different behaviors? */
                    if (count($touch_behaviors) == 1) {
                        /* Just one */
                        $modes = current($touch_behaviors);
                        $behavior = unserialize(current(array_keys($touch_behaviors)));
                        $condition = "modeCheck(" . implode(", pos) || modeCheck(", $modes) . ", pos)";
                        $condition = (count($modes) > 1) ? '(' . $condition . ')' : $condition;
                        echo ($done2 > 0 ? " else " : "        ") . "if ((sensors1 & KS1_$touch) == KS1_$touch && (sensors2 & KS2_$touch) == KS2_$touch && $condition) {\n";
                        echo behaviorCode(2, $touch, $behavior);
                        echo "        }";
                    } else {
                        /* Output multiple behaviours */
                        echo ($done2 > 0 ? " else " : "        ") . "if ((sensors1 & KS1_$touch) == KS1_$touch && (sensors2 & KS2_$touch) == KS2_$touch) {\n";
                        $first3 = true;
                        foreach ($touch_behaviors as $behavior => $modes) {
                            $behavior = unserialize($behavior);
                            echo ($first3 ? "            " : " else ") . "if (modeCheck(" . implode(", pos) || modeCheck(", $modes) . ", pos)) {\n";
                            echo behaviorCode(3, $touch, $behavior);
                            echo "             }";
                            $first3 = false;
                        }
                        echo "\n        }";
                    }                       
                    $done2++;
                }
            }
            if (!empty($layers[$touch1][0])) {
                foreach ($layers[$touch1][0] as $pos2=>$com) {
                    $touch = join('_', reformatTouch($com));
                    
                    /* Get all the modes with behaviors for this touch */
                    $modesWithEvents = array();
                    foreach ($touchSet as $mode => $modeSet) {
                        if (isset($modeSet[$test]) && isset($modeSet[$test][$touch])) {
                            $modesWithEvents[$mode] = $modeSet[$test][$touch];
                        }
                    }
                    
                    /* How many modes with behaviors? */
                    if (count($modesWithEvents) == 1) {
                        /* Just one */
                        $mode = current(array_keys($modesWithEvents));
                        $behavior = current($modesWithEvents);
                        echo " else if (modeCheck($modeConstants[$mode], pos)) {\n";
                        echo behaviorCode(2, $touch, $behavior);
                        echo "        }";
                    } else {
                        /* Output multiple modes */
                        echo " else {\n";
                        $first3 = true;
                        foreach ($modesWithEvents as $mode => $behavior) {
                            echo ($first3 ? "             " : " else ") . "if (modeCheck($modeConstants[$mode], pos)) {\n";
                            echo behaviorCode(3, $touch, $behavior);
                            echo "             }";
                            $first3 = false;
                        }
                        echo "\n        }";
                    }
                    $done1++;
                }
            }
            echo "\n    }";
        } elseif (isset($layers[$touch1]) && count($layers[$touch1]) == 1) {
            $com = current($layers[$touch1]);
            $touch = join('_', reformatTouch($com));
            
            /* Get all behaviors for this touch */
            $touch_behaviors = array();
            foreach ($touchSet as $mode => $modeSet) {
                if (isset($modeSet[$test]) && isset($modeSet[$test][$touch])) {
                    $touch_behaviors[serialize($modeSet[$test][$touch])][] = $modeConstants[$mode];
                }
            }
                    
            /* How many different behaviors? */
            if (count($touch_behaviors) == 1) {
                /* Just one */
                $modes = current($touch_behaviors);
                $behavior = unserialize(current(array_keys($touch_behaviors)));
                $condition = "modeCheck(" . implode(", pos) || modeCheck(", $modes) . ", pos)";
                $condition = (count($modes) > 1) ? '(' . $condition . ')' : $condition;
                echo ($done1 > 0 ? " else " : "    ") . "if ((sensors1 & KS1_$touch) == KS1_$touch && (sensors2 & KS2_$touch) == KS2_$touch && $condition) {\n";
                echo behaviorCode(1, $touch, $behavior);
                echo "    }";
            } else {
                /* Output multiple behaviours */
                echo ($done1 > 0 ? " else " : "    ") . "if ((sensors1 & KS1_$touch) == KS1_$touch && (sensors2 & KS2_$touch) == KS2_$touch) {\n";
                $first3 = true;
                foreach ($touch_behaviors as $behavior => $modes) {
                    $behavior = unserialize($behavior);
                    echo ($first3 ? "        " : " else ") . "if (modeCheck(" . implode(", pos) || modeCheck(", $modes) . ", pos)) {\n";
                    echo behaviorCode(2, $touch, $behavior);
                    echo "         }";
                    $first3 = false;
                }
                echo "\n    }";
            }   
            
            $done1++;
        }
    }
    
    /* Fall throughs */
    $first0 = true;
    foreach ($touchSet as $mode => $touchdef) {
        if (!isset($touchdef['options']['fallthrough'])) {
            continue;
        }
        if (count($touchdef['options']['fallthrough']) == 1) {
            $ft_mode = current($touchdef['options']['fallthrough']);
            echo ($first0 ? "\n\n    /* Fall throughs */\n    " : " else ") . "if (modeCheck($modeConstants[$mode], pos) && modeCheck($modeConstants[$ft_mode], pos + 1)) {\n";
            echo "        check_sensors_$test(sensors1, sensors2, pos + 1);\n";
            echo "        return;\n";
            echo "    }";
        } else {
            echo ($first0 ? "\n\n    /* Fall throughs */\n    " : " else ") . "if (modeCheck($modeConstants[$mode], pos) && ("; 
            foreach ($touchdef['options']['fallthrough'] as $ftid => $ft_mode) {
                echo  ($ftid ? ' || ' : '') . "modeCheck($modeConstants[$ft_mode], pos + 1)";
            }
            echo ")) {\n";
            echo "        check_sensors_$test(sensors1, sensors2, pos + 1);\n";
            echo "        return;\n";
            echo "    }";
        }
        $first0 = false;
    }
    echo "\n}\n\n";
}
//echo '</pre>';

