<?php

/*
================================================================================
Keyglove API Generator Script
2014-12-07 by Jeff Rowberg <jeff@rowberg.net>

Changelog:
    2014-12-07 - Initial release

================================================================================
Keyglove source code is placed under the MIT license
Copyright (c) 2014 Jeff Rowberg

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

================================================================================
*/

function wrap_code_line($line, $width, $language) {
    if (strlen($line) <= $width) return $line;
    $lines = array();
    $thisLine = '';
    $spaceList = array();
    $indentSpaces = strlen($line) - strlen(ltrim($line));
    $inString = 0;
    $inStringList = array();
    for ($i = 0; $i < strlen($line); $i++) {
        if ($line[$i] == ' ') {
            $spaceList[] = $i;
        } else if ($line[$i] == '"') {
            if (!$inString) $inString = 2;
            else if ($line[$i - 1] != '\\') $inString = 0;
        } else if ($line[$i] == "'") {
            if (!$inString) $inString = 1;
            else if ($line[$i - 1] != '\\') $inString = 0;
        }
        $inStringList[$i] = $inString;
        if ($i >= $width) {
            // time to split the line of code
            if ($language == 'python') {
                $useSpace = count($spaceList) - 1;
                while ($spaceList[$useSpace] > $width - ($inString ? 3 : 2) && $useSpace > 0) $useSpace--;
                if ($inStringList[$spaceList[$useSpace]] == 2) {
                    $lines[] = substr($line, 0, $spaceList[$useSpace]).'" \\';
                    $line = str_repeat(' ', $indentSpaces + 8).'"'.substr($line, $spaceList[$useSpace]);
                } else if ($inStringList[$spaceList[$useSpace]] == 1) {
                    $lines[] = substr($line, 0, $spaceList[$useSpace]).'\' \\';
                    $line = str_repeat(' ', $indentSpaces + 8)."'".substr($line, $spaceList[$useSpace]);
                } else {
                    $lines[] = substr($line, 0, $spaceList[$useSpace]).' \\';
                    $line = str_repeat(' ', $indentSpaces + 8).substr($line, $spaceList[$useSpace] + 1);
                }
                $spaceList = array();
                $i = -1;
            }
        }
    }
    if (strlen($line)) $lines[] = $line;
    return join("\n", $lines);
}

echo '<pre>';

echo "Reading '../kgapi.json'\n";
$content = file_get_contents("../kgapi.json");

echo "Decoding JSON content into data structure\n";
$kgapi = json_decode($content, true);
if (!$kgapi) die("Error parsing '../kgapi.json' into array. Please validate JSON content.");

$htmlRef = "";
$htmlRefNav = array(1, 1, 1, 1);

$arduinoChangelog = array();
$arduinoChangelog[] = "2014-04-08 - Initial release";

$arduinoCommandMacros = array();
$arduinoCommandDeclarations = array();
$arduinoCommandDefinitions = array();
$arduinoEventMacros = array();
$arduinoEventDeclarations = array();
$arduinoEventDefinitions = array();
$arduinoCases = array();

$pythonChangelog = array();
$pythonCommandDefinitions = array();
$pythonResponseDeclarations = array();
$pythonEventDeclarations = array();
$pythonResponseConditions = array();
$pythonEventConditions = array();
$pythonHandlerDefinitions = array();
$pythonHandlerAssignments = array();
$pythonFriendlyPacketCommandConditions = array();
$pythonFriendlyPacketResponseConditions = array();
$pythonFriendlyPacketEventConditions = array();

$pythonGUIPageDefinitions = array();
$pythonGUIPageAssignments = array();
$pythonGUIPageAdditions = array();

$pythonChangelog[] = "2014-09-01 - Added unknown last response/event case for stability";
$pythonChangelog[] = "2014-05-20 - Added friendly output for GUI tool and general logging";
$pythonChangelog[] = "2013-11-20 - Bundled clean serial + HID transport into KGLib code";
$pythonChangelog[] = "           - Added send_and_return() method for easier response handling";
$pythonChangelog[] = "2013-11-16 - Initial release";

echo "Parsing defined API classes\n";
foreach ($kgapi["classes"] as $class) {
    $classCommands = 0;
    $classResponses = 0;
    $classEvents = 0;
    $arduinoCommandMacros[$class["id"]] = array();
    $arduinoCommandDeclarations[$class["id"]] = array();
    $arduinoEventMacros[$class["id"]] = array();
    $arduinoEventDeclarations[$class["id"]] = array();
    $arduinoCases[$class["id"]] = array();
    
    $pythonGUIPageDefinitions[] = 'class KGPage'.ucfirst($class["name"]).'(wxsp.ScrolledPanel):';
    $pythonGUIPageDefinitions[] = '    def __init__(self, parent):';
    $pythonGUIPageDefinitions[] = '        wxsp.ScrolledPanel.__init__(self, parent)';
    $pythonGUIPageDefinitions[] = '        self.SetupScrolling()';
    $pythonGUIPageDefinitions[] = '';
    $pythonGUICommandButtonHandlers = array();
    
    $pythonGUIPageAssignments[] = 'page'.ucfirst($class["name"]).' = KGPage'.ucfirst($class["name"]).'(nb)';
    $pythonGUIPageAdditions[] = 'nb.AddPage(page'.ucfirst($class["name"]).', "'.ucfirst($class["name"]).'")';
    
    echo "+ ".$class["name"]."\n";
    if (!empty($class["commands"]) || !empty($class["events"]) || !empty($class["enumerations"])) {
        $pythonGUIPageDefinitions[] = '        pageHBox = wx.BoxSizer(wx.HORIZONTAL)';
        $pythonGUIPageDefinitions[] = '        commandVBox = wx.BoxSizer(wx.VERTICAL)';
        //$pythonGUIPageDefinitions[] = '        eventVBox = wx.BoxSizer(wx.VERTICAL)';
        $pythonGUIPageDefinitions[] = '        pageHBox.Add(commandVBox, 0, wx.ALL, 8)';
        //$pythonGUIPageDefinitions[] = '        pageHBox.Add(eventVBox, 0, wx.ALL, 8)';
        $pythonGUIPageDefinitions[] = '';
        $pythonGUIPageDefinitions[] = '        labelCommands = wx.StaticText(self, -1, "'.ucfirst($class["name"]).' commands", (8, 8))';
        $pythonGUIPageDefinitions[] = '        labelCommands.SetFont(labelCommands.GetFont().MakeBold())';
        $pythonGUIPageDefinitions[] = '        commandVBox.Add(labelCommands)';
        $pythonGUIPageDefinitions[] = '';

        $htmlRef .= '<h2><span class="headingtab">'.$htmlRefNav[0].'</span> '.ucfirst($class["name"]).' class (ID = '.$class["id"].')</h2>';
        $htmlRef .= $class["description"];
        if (!empty($class["commands"])) {
            $htmlRef .= '<h3><span class="headingtab">'.$htmlRefNav[0].'.'.$htmlRefNav[1].'</span> Commands</h3>';
            foreach ($class["commands"] as $command) {
                $classCommands++;
                $fixedLength = true;
                $payloadHTML = '';
                $payloadLength = 0;
                
                echo "  + kg_cmd_".$class["name"]."_".$command["name"]." (".$class["id"]."/".$command["id"].")\n";
                
                $arduinoCommandMacros[$class["id"]][] = "#define ".str_pad("KG_PACKET_ID_CMD_".strtoupper($class["name"].'_'.$command["name"]), 48, " ", STR_PAD_RIGHT).'    '.sprintf("0x%02X", $command["id"]);
                
                $arduinoCommandArgList = array();
                $arduinoCommandDefArgList = array();
                $arduinoCommandCommentArgList = array();
                $arduinoCommandDoxygenParams = array();
                $arduinoResponseVarList = array();
                $arduinoResponseArgList = array();
                $arduinoResponseDefArgList = array();
                $arduinoResponseCommentArgList = array();
                $arduinoResponseAssignList = array();
                $arduinoResponseInitializerList = array();
                
                $pythonPackStr = '<4B';
                $pythonUnpackStr = '';
                $pythonPackPost = '';
                $pythonLengthExtra = '';
                $pythonPackList = array('0xC0', '0x00', sprintf("0x%02X", $class["id"]), sprintf("0x%02X", $command["id"]));
                $pythonUnpackList = array();
                $pythonArgList = array('self');
                $pythonFriendlyArgList = array();
                $pythonDataExtra = array();
                $pythonHandlerPrintList = array();
                $pythonHandlerFormatList = array();
                
                $pythonGUICommandFields = array();
                $pythonGUICommandButtonHandlers[] = 'if event.Id == '.sprintf("1%02d%02d", $class["id"], $command["id"]).': # kg_cmd_'.$class["name"].'_'.$command["name"];
                $pythonGUICommandButtonArgs = array();

                $cArgList = array();

                foreach ($command["parameters"] as $parameter) {
                    $arduinoCommandCommentArgList[] = $parameter["type"].' '.$parameter["name"];
                    $byteMin = $payloadLength + 4;
                    $pythonGUICommandFields[] = 'hbox_'.$class["name"].'_'.$command["name"].'.Add(wx.StaticText(self, -1, "'.$parameter["name"].':"), 0, wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 4)';
                    $pythonGUICommandButtonHandlers[] = '    '.($payloadLength > 0 ? 'elif' : 'if').' not self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].'.IsValid():';
                    $pythonGUICommandButtonHandlers[] = '        wx.MessageBox("Please enter valid data in the \''.$parameter["name"].'\' field.", "Field value error", wx.OK | wx.ICON_WARNING)';
                    if (isset($parameter["units"])) {
                        @list($unit, $units) = explode(',', @$parameter["units"], 2);
                    } else {
                        $unit = $units = false;
                    }
                    $multiplier = isset($parameter["multiplier"]) ? $parameter["multiplier"] : 1;
                    switch ($parameter["type"]) {
                        case "macaddr_t":
                            $arduinoCommandArgList[] = 'rxPacket + '.($payloadLength + 4);
                            $arduinoCommandDefArgList[] = 'uint8_t *'.$parameter["name"];
                            $arduinoCommandDoxygenParams[] = ' * @param[in] '.$parameter["name"].' '.$parameter["description"];
                            $pythonPackStr .= '6s';
                            $pythonArgList[] = $parameter["name"];
                            $pythonPackList[] = 'b\'\'.join(chr(i) for i in '.$parameter["name"].')';
                            $pythonUnpackStr .= '6s';
                            $pythonUnpackList[] = $parameter["name"];
                            $pythonHandlerPrintList[] = $parameter["name"].': %s';
                            $pythonHandlerFormatList[] = "' '.join(['%02X' % b for b in ".$parameter["name"]."])";
                            $pythonFriendlyArgList[] = "'".$parameter["name"]."': ':'.join(['%02X' % b for b in ".$parameter["name"]."][::-1])";
                            $pythonDataExtra[] = $parameter["name"].' = [ord(b) for b in self.kgapi_rx_payload['.$payloadLength.':'.($payloadLength + 6).']]';
                            $pythonGUICommandFields[] = 'self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].' = wxm.TextCtrl(self, -1, "00:00:00:00:00:00", validRequired=False, emptyInvalid=True, mask="##:##:##:##:##:##", formatcodes="FS", includeChars="abcdefABCDEF")';
                            $pythonGUICommandFields[] = 'hbox_'.$class["name"].'_'.$command["name"].'.Add(self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].', 0, wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 4)';
                            $pythonGUICommandButtonArgs[] = '[int(b, 16) for b in self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].'.GetValue().split(":")][::-1]';
                            $payloadLength += 6;
                            break;
                        case "btcod_t":
                            $arduinoCommandArgList[] = 'rxPacket + '.($payloadLength + 4);
                            $arduinoCommandDefArgList[] = 'uint8_t *'.$parameter["name"];
                            $arduinoCommandDoxygenParams[] = ' * @param[in] '.$parameter["name"].' '.$parameter["description"];
                            $pythonPackStr .= '3s';
                            $pythonArgList[] = $parameter["name"];
                            $pythonPackList[] = 'b\'\'.join(chr(i) for i in '.$parameter["name"].')';
                            $pythonUnpackStr .= '3s';
                            $pythonUnpackList[] = $parameter["name"];
                            $pythonHandlerPrintList[] = $parameter["name"].': %s';
                            $pythonHandlerFormatList[] = "' '.join(['%02X' % b for b in ".$parameter["name"]."])";
                            $pythonFriendlyArgList[] = "'".$parameter["name"]."': ''.join(['%02X' % b for b in ".$parameter["name"]."][::-1])";
                            $pythonDataExtra[] = $parameter["name"].' = [ord(b) for b in self.kgapi_rx_payload['.$payloadLength.':'.($payloadLength + 3).']]';
                            $pythonGUICommandFields[] = 'self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].' = wxm.TextCtrl(self, -1, "000000", validRequired=False, emptyInvalid=True, mask="######", formatcodes="FSV", includeChars="abcdefABCDEF", validRegex="^[0-9a-fA-F]{,6}")';
                            $pythonGUICommandFields[] = 'hbox_'.$class["name"].'_'.$command["name"].'.Add(self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].', 0, wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 4)';
                            $pythonGUICommandButtonArgs[] = 'int(self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].'.GetValue(), 16)';
                            $payloadLength += 3;
                            break;
                        case "uint8_t[]":
                            $arduinoCommandArgList[] = 'rxPacket['.($payloadLength + 4).']';
                            $arduinoCommandArgList[] = 'rxPacket + '.($payloadLength + 5);
                            $arduinoCommandDefArgList[] = 'uint8_t '.$parameter["name"].'_len';
                            $arduinoCommandDefArgList[] = 'uint8_t *'.$parameter["name"].'_data';
                            $arduinoCommandDoxygenParams[] = ' * @param[in] '.$parameter["name"].'_len Length in bytes of '.$parameter["name"].'_data buffer';
                            $arduinoCommandDoxygenParams[] = ' * @param[in] '.$parameter["name"].'_data '.$parameter["description"];
                            $fixedLength = false;
                            $pythonPackStr .= 'B';
                            $pythonPackPost .= ' + str(len('.$parameter["name"].')) + \'s\'';
                            $pythonArgList[] = $parameter["name"];
                            $pythonLengthExtra .= ' + len('.$parameter["name"].')';
                            $pythonPackList[] = 'len('.$parameter["name"].')';
                            $pythonPackList[] = 'b\'\'.join(chr(i) for i in '.$parameter["name"].')';
                            $pythonUnpackStr .= 'B';
                            $pythonUnpackList[] = $parameter["name"]."_len";
                            $pythonHandlerPrintList[] = $parameter["name"].': %s';
                            $pythonHandlerFormatList[] = "' '.join(['%02X' % b for b in ".$parameter["name"]."])";
                            switch (@$parameter["format"]) {
                                case "string":
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ''.join(['%c' % b for b in ".$parameter["name"]."_data])";
                                    break;
                                default:
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ' '.join(['%02X' % b for b in ".$parameter["name"]."_data])";
                                    break;
                            }
                            $payloadLength++;
                            $pythonGUICommandFields[] = 'self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].' = wxm.TextCtrl(self, -1, "", validRequired=False, emptyInvalid=True, mask="*+", formatcodes="F")';
                            $pythonGUICommandFields[] = 'hbox_'.$class["name"].'_'.$command["name"].'.Add(self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].', 0, wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 4)';
                            $pythonGUICommandButtonArgs[] = 'len(self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].'.GetValue())';
                            $pythonGUICommandButtonArgs[] = 'self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].'.GetValue()';
                            $payloadLength++;
                            $pythonDataExtra[] = $parameter["name"].'_data = [ord(b) for b in self.kgapi_rx_payload['.$payloadLength.':]]';
                            break;
                        case "uint8_t":
                            $arduinoCommandArgList[] = 'rxPacket['.($payloadLength + 4).']';
                            $arduinoCommandDefArgList[] = 'uint8_t '.$parameter["name"];
                            $arduinoCommandDoxygenParams[] = ' * @param[in] '.$parameter["name"].' '.$parameter["description"];
                            $pythonPackStr .= 'B';
                            $pythonArgList[] = $parameter["name"];
                            $pythonPackList[] = $parameter["name"];
                            $pythonUnpackStr .= 'B';
                            $pythonUnpackList[] = $parameter["name"];
                            $pythonHandlerPrintList[] = $parameter["name"].': %02X';
                            $pythonHandlerFormatList[] = $parameter["name"];
                            switch (@$parameter["format"]) {
                                case "decimal":
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%d".($unit ? " %s": "")."' % (".$parameter["name"].($multiplier != 1 ? " * ".$multiplier : "").($unit ? ", '".$unit."'".($units ? " if (".$parameter["name"]." == 1) else '".$units."'" : "") : "")."))";
                                    break;
                                case "bool":
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%s' % ('TRUE' if ".$parameter["name"]." else 'FALSE'))";
                                    break;
                                default:
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%02X' % ".$parameter["name"].")";
                                    break;
                            }
                            $pythonGUICommandFields[] = 'self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].' = wxm.TextCtrl(self, -1, "0", validRequired=False, emptyInvalid=True, mask="##", formatcodes="FSV", includeChars="abcdefABCDEF", validRegex="^[0-9a-fA-F]{,2}")';
                            $pythonGUICommandFields[] = 'hbox_'.$class["name"].'_'.$command["name"].'.Add(self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].', 0, wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 4)';
                            $pythonGUICommandButtonArgs[] = 'int(self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].'.GetValue(), 16)';
                            $payloadLength++;
                            break;
                        case "int8_t":
                            $arduinoCommandArgList[] = 'rxPacket['.($payloadLength + 4).']';
                            $arduinoCommandDefArgList[] = 'int8_t '.$parameter["name"];
                            $arduinoCommandDoxygenParams[] = ' * @param[in] '.$parameter["name"].' '.$parameter["description"];
                            $pythonPackStr .= 'b';
                            $pythonArgList[] = $parameter["name"];
                            $pythonPackList[] = $parameter["name"];
                            $pythonUnpackStr .= 'b';
                            $pythonUnpackList[] = $parameter["name"];
                            $pythonHandlerPrintList[] = $parameter["name"].': %02X';
                            $pythonHandlerFormatList[] = $parameter["name"];
                            switch (@$parameter["format"]) {
                                case "decimal":
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%d".($unit ? " %s": "")."' % (".$parameter["name"].($multiplier != 1 ? " * ".$multiplier : "").($unit ? ", '".$unit."'".($units ? " if (".$parameter["name"]." == 1) else '".$units."'" : "") : "")."))";
                                    break;
                                default:
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%02X' % ".$parameter["name"].")";
                                    break;
                            }
                            $pythonGUICommandFields[] = 'self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].' = wxm.TextCtrl(self, -1, "0", validRequired=False, emptyInvalid=True, mask="##", formatcodes="FSV", includeChars="abcdefABCDEF", validRegex="^[0-9a-fA-F]{,2}")';
                            $pythonGUICommandFields[] = 'hbox_'.$class["name"].'_'.$command["name"].'.Add(self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].', 0, wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 4)';
                            $pythonGUICommandButtonArgs[] = 'int(self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].'.GetValue(), 16)';
                            $payloadLength++;
                            break;
                        case "uint16_t":
                            $arduinoCommandArgList[] = 'rxPacket['.($payloadLength + 4).'] | (rxPacket['.($payloadLength + 5).'] << 8)';
                            $arduinoCommandDefArgList[] = 'uint16_t '.$parameter["name"];
                            $arduinoCommandDoxygenParams[] = ' * @param[in] '.$parameter["name"].' '.$parameter["description"];
                            $pythonPackStr .= 'H';
                            $pythonArgList[] = $parameter["name"];
                            $pythonPackList[] = $parameter["name"];
                            $pythonUnpackStr .= 'H';
                            $pythonUnpackList[] = $parameter["name"];
                            $pythonHandlerPrintList[] = $parameter["name"].': %04X';
                            $pythonHandlerFormatList[] = $parameter["name"];
                            switch (@$parameter["format"]) {
                                case "decimal":
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%d".($unit ? " %s": "")."' % (".$parameter["name"].($multiplier != 1 ? " * ".$multiplier : "").($unit ? ", '".$unit."'".($units ? " if (".$parameter["name"]." == 1) else '".$units."'" : "") : "")."))";
                                    break;
                                default:
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%04X' % ".$parameter["name"].")";
                                    break;
                            }
                            $pythonGUICommandFields[] = 'self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].' = wxm.TextCtrl(self, -1, "0", validRequired=False, emptyInvalid=True, mask="####", formatcodes="FSV", includeChars="abcdefABCDEF", validRegex="^[0-9a-fA-F]{,4}")';
                            $pythonGUICommandFields[] = 'hbox_'.$class["name"].'_'.$command["name"].'.Add(self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].', 0, wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 4)';
                            $pythonGUICommandButtonArgs[] = 'int(self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].'.GetValue(), 16)';
                            $payloadLength += 2;
                            break;
                        case "uint32_t":
                            $arduinoCommandArgList[] = 'rxPacket['.($payloadLength + 4).'] | (rxPacket['.($payloadLength + 5).'] << 8) | (rxPacket['.($payloadLength + 5).'] << 16) | (rxPacket['.($payloadLength + 5).'] << 24)';
                            $arduinoCommandDefArgList[] = 'uint32_t '.$parameter["name"];
                            $arduinoCommandDoxygenParams[] = ' * @param[in] '.$parameter["name"].' '.$parameter["description"];
                            $pythonPackStr .= 'L';
                            $pythonArgList[] = $parameter["name"];
                            $pythonPackList[] = $parameter["name"];
                            $pythonUnpackStr .= 'L';
                            $pythonUnpackList[] = $parameter["name"];
                            $pythonHandlerPrintList[] = $parameter["name"].': %08X';
                            $pythonHandlerFormatList[] = $parameter["name"];
                            switch (@$parameter["format"]) {
                                case "decimal":
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%d".($unit ? " %s": "")."' % (".$parameter["name"].($multiplier != 1 ? " * ".$multiplier : "").($unit ? ", '".$unit."'".($units ? " if (".$parameter["name"]." == 1) else '".$units."'" : "") : "")."))";
                                    break;
                                default:
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%08X' % ".$parameter["name"].")";
                                    break;
                            }
                            $pythonGUICommandFields[] = 'self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].' = wxm.TextCtrl(self, -1, "0", validRequired=False, emptyInvalid=True, mask="########", formatcodes="FSV", includeChars="abcdefABCDEF", validRegex="^[0-9a-fA-F]{,8}")';
                            $pythonGUICommandFields[] = 'hbox_'.$class["name"].'_'.$command["name"].'.Add(self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].', 0, wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 4)';
                            $pythonGUICommandButtonArgs[] = 'int(self.txt_'.$class["name"].'_'.$command["name"].'_'.$parameter["name"].'.GetValue(), 16)';
                            $payloadLength += 4;
                            break;
                    }
                    $byteMax = $payloadLength + 4 - 1;
                    $payloadHTML .= '<tr class="payload"><td>'.$byteMin;
                    if ($byteMax > $byteMin) $payloadHTML .= '&nbsp;-&nbsp;'.$byteMax;
                    $payloadHTML .= '</td><td>'.$parameter["type"].'</td><td>'.$parameter["name"].'</td><td>'.$parameter["description"];
                    if (!empty($parameter["references"])) {
                        $payloadHTML .= '<ul>';
                        if (isset($parameter["references"]["commands"])) {
                            foreach ($parameter["references"]["commands"] as $refCommand) {
                                $payloadHTML .= '<li><em>Command:</em> <a href="#kg_cmd_'.$refCommand.'">'.$refCommand.'</a></li>';
                            }
                        }
                        if (isset($parameter["references"]["events"])) {
                            foreach ($parameter["references"]["events"] as $refEvent) {
                                $payloadHTML .= '<li><em>Event:</em> <a href="#kg_evt_'.$refEvent.'">'.$refEvent.'</a></li>';
                            }
                        }
                        if (isset($parameter["references"]["enumerations"])) {
                            foreach ($parameter["references"]["enumerations"] as $refEnumeration) {
                                $payloadHTML .= '<li><em>Enum:</em> <a href="#kg_enum_'.$refEnumeration.'">'.$refEnumeration.'</a></li>';
                            }
                        }
                        $payloadHTML .= '</ul>';
                    }
                    $payloadHTML .= '</td></tr>';
                }
                $htmlRef .= '<h4><span class="headingtab">'.$htmlRefNav[0].'.'.$htmlRefNav[1].'.'.$htmlRefNav[2].'</span> '.$class["name"].'_'.$command["name"].' <code style="color: #F00;">[ C0 '.sprintf("%02X", $payloadLength).($fixedLength ? '' : '+').' '.sprintf("%02X %02X", $class["id"], $command["id"]).' '.($payloadLength > 0 ? '... ' : '').']</code></h4>'; // ('.$command["id"].')';
                $htmlRef .= $command["description"];
                $htmlRef .= '<div class="breakauto">';
                $htmlRef .= '<table class="command">';
                $htmlRef .= '<thead>';
                //$htmlRef .= '<tr><td colspan="4"><img src="kgapi_command.png" title="KGAPI Command" width="200" height="70" />COMMAND STRUCTURE</td></tr>';
                $htmlRef .= '<tr><th colspan="4" class="tabletitle">OUTGOING COMMAND PACKET STRUCTURE</th></tr>';
                $htmlRef .= '<tr><th>Byte</th><th>Type</th><th>Name</th><th>Description</th></tr>';
                $htmlRef .= '</thead>';
                $htmlRef .= '<tbody>';
                $htmlRef .= '<tr class="header"><td>0</td><td>0xC0</td><td>type</td><td>Command packet</td></tr>';
                $htmlRef .= '<tr class="header"><td>1</td><td>0x'.sprintf("%02X", $payloadLength).(!$fixedLength ? '+' : '').'</td><td>length</td><td>';
                if ($payloadLength == 0) {
                    $htmlRef .= 'No payload';
                } else {
                    $htmlRef .= ($fixedLength ? "Fixed-length payload (".$payloadLength.")" : "Variable-length payload (".$payloadLength."+)");
                }
                $htmlRef .= '</td></tr>';
                $htmlRef .= '<tr class="header"><td>2</td><td>0x'.sprintf("%02X", $class["id"]).'</td><td>class</td><td>Command class: "'.$class["name"].'"</td></tr>';
                $htmlRef .= '<tr class="header"><td>3</td><td>0x'.sprintf("%02X", $command["id"]).'</td><td>id</td><td>Command ID: "'.$command["name"].'"</td></tr>';
                $htmlRef .= $payloadHTML;
                $htmlRef .= '</thead>';
                $htmlRef .= '</table>';
                $htmlRef .= '</div>';
                
                $arduinoCommandPayloadLength = $payloadLength;
                $arduinoCommandFixedLength = $fixedLength;

                // append relevant Python source code lines for command
                $pythonPackList[1] = sprintf("0x%02X", $payloadLength).$pythonLengthExtra;
                $pythonCommandDefinitions[] = 'def kg_cmd_'.$class["name"].'_'.$command["name"].'('.join(', ', $pythonArgList).'):';
                $pythonCommandDefinitions[] = '    return struct.pack(\''.$pythonPackStr.'\''.$pythonPackPost.', '.join(', ', $pythonPackList).')';
                $pythonCommandArgList = array_splice($pythonArgList, 1); // store command args (w/o "self") for later example snippet

                $cCommandArgList = $cArgList;

                // append relevant source code lines for command
                if ($classCommands == 1) {
                    $pythonFriendlyPacketCommandConditions[] = (empty($pythonFriendlyPacketCommandConditions) ? 'if' : 'elif').' packet_class == '.$class["id"].': # '.strtoupper($class["name"]);
                }
                $pythonFriendlyPacketCommandConditions[] = '    '.($classCommands == 1 ? 'if' : 'elif').' packet_command == '.$command["id"].': # kg_cmd_'.$class["name"].'_'.$command["name"];
                if (!empty($pythonUnpackList)) {
                    $pythonFriendlyPacketCommandConditions[] = '        '.join(', ', $pythonUnpackList).', = struct.unpack(\'<'.$pythonUnpackStr.'\', payload[:'.$payloadLength.'])';
                }
                foreach ($pythonDataExtra as $pde) {
                    $pythonFriendlyPacketCommandConditions[] = '        '.$pde;
                }
                $pythonFriendlyPacketCommandConditions[] = '        return { \'type\': \'command\', \'name\': \'kg_cmd_'.$class["name"].'_'.$command["name"].'\', \'length\': payload_length, \'class_id\': packet_class, \'command_id\': packet_command, \'payload\': { '.join(', ', $pythonFriendlyArgList).' }, \'payload_keys\': [ '.(!empty($pythonCommandArgList) ? "'".join("', '", $pythonCommandArgList)."'" : "").' ] }';

                // finish Python GUI command click handler
                if ($payloadLength == 0) {
                    $pythonGUICommandButtonHandlers[] = '    packet = keyglove.kg_cmd_'.$class["name"].'_'.$command["name"].'()';
                    $indent = '    ';
                } else {
                    $pythonGUICommandButtonHandlers[] = '    else:';
                    $pythonGUICommandButtonHandlers[] = '        packet = keyglove.kg_cmd_'.$class["name"].'_'.$command["name"].'('.join(', ', $pythonGUICommandButtonArgs).')';
                    $indent = '        ';
                }
                $pythonGUICommandButtonHandlers[] = $indent.'self.Parent.Parent.Parent.SendPacket(packet)';

                // deal with response packet
                $pythonUnpackStr = '';
                $pythonLengthExtra = '';
                $pythonDataExtra = array();
                $pythonUnpackList = array();
                $pythonArgList = array();
                $pythonFriendlyArgList = array();
                $pythonResponseArgList = array();
                $pythonHandlerPrintList = array();
                $pythonHandlerFormatList = array();

                $cArgList = array();
                $cHandlerPrintList = array();
                $cHandlerFormatList = array();
                
                // only process params if they are defined (but response packet is always generated just for ACK purposes)
                $fixedLength = true;
                $payloadHTML = '';
                $payloadLength = 0;
                if (!empty($command["returns"])) {
                    $classResponses++;
                    foreach ($command["returns"] as $return) {
                        $byteMin = $payloadLength + 4;
                        $arduinoResponseCommentArgList[] = $return["type"].' '.$return["name"];
                        if (isset($return["units"])) {
                            @list($unit, $units) = explode(',', $return["units"], 2);
                        } else {
                            $unit = $units = false;
                        }
                        $multiplier = isset($return["multiplier"]) ? $return["multiplier"] : 1;
                        $pythonResponseArgList[] = $return["name"];
                        switch ($return["type"]) {
                            case "macaddr_t":
                                $arduinoResponseVarList[] = 'uint8_t '.$return["name"].'[6];';
                                $arduinoResponseArgList[] = $return["name"];
                                $arduinoResponseDefArgList[] = 'uint8_t *'.$return["name"];
                                $arduinoResponseInitializerList[] = '0,0,0,0,0,0';
                                $arduinoResponseAssignList[] = 'memcpy(payload + '.$payloadLength.', '.$return["name"].', 6);';
                                $arduinoCommandDoxygenParams[] = ' * @param[out] '.$return["name"].' '.$return["description"];
                                $pythonUnpackStr .= '6s';
                                $pythonArgList[] = "'".$return["name"]."': ".$return["name"];
                                $pythonUnpackList[] = $return["name"];
                                $pythonHandlerPrintList[] = $return["name"].': %s';
                                $pythonHandlerFormatList[] = "' '.join(['%02X' % b for b in args['".$return["name"]."']])";
                                $pythonFriendlyArgList[] = "'".$return["name"]."': ':'.join(['%02X' % b for b in ".$return["name"]."][::-1])";
                                $pythonDataExtra[] = $return["name"].' = [ord(b) for b in self.kgapi_rx_payload['.$payloadLength.':'.($payloadLength + 6).']]';
                                $payloadLength += 6;
                                break;
                            case "btcod_t":
                                $arduinoResponseVarList[] = 'uint8_t '.$return["name"].'[3];';
                                $arduinoResponseArgList[] = $return["name"];
                                $arduinoResponseDefArgList[] = 'uint8_t *'.$return["name"];
                                $arduinoResponseInitializerList[] = '0,0,0';
                                $arduinoResponseAssignList[] = 'memcpy(payload + '.$payloadLength.', '.$return["name"].', 3);';
                                $arduinoCommandDoxygenParams[] = ' * @param[out] '.$return["name"].' '.$return["description"];
                                $pythonUnpackStr .= '3s';
                                $pythonArgList[] = "'".$return["name"]."': ".$return["name"];
                                $pythonUnpackList[] = $return["name"];
                                $pythonHandlerPrintList[] = $return["name"].': %s';
                                $pythonHandlerFormatList[] = "' '.join(['%02X' % b for b in args['".$return["name"]."']])";
                                $pythonFriendlyArgList[] = "'".$return["name"]."': ''.join(['%02X' % b for b in ".$return["name"]."][::-1])";
                                $pythonDataExtra[] = $return["name"].' = [ord(b) for b in self.kgapi_rx_payload['.$payloadLength.':'.($payloadLength + 3).']]';
                                $payloadLength += 3;
                                break;
                            case "uint8_t[]":
                                $arduinoResponseVarList[] = 'uint8_t '.$return["name"].'_len;';
                                $arduinoResponseVarList[] = 'uint8_t *'.$return["name"].'_data;';
                                $arduinoResponseArgList[] = '&'.$return["name"].'_len';
                                $arduinoResponseArgList[] = $return["name"].'_data';
                                $arduinoResponseDefArgList[] = 'uint8_t *'.$return["name"].'_len';
                                $arduinoResponseDefArgList[] = 'uint8_t *'.$return["name"].'_data';
                                $arduinoResponseAssignList[] = $return["name"].'_data = (uint8_t *)malloc('.$return["name"].'_len);';
                                $arduinoResponseInitialiazerList[] = '0,0,0,0,0,0';
                                $arduinoCommandDoxygenParams[] = ' * @param[out] '.$return["name"].'_len Length in bytes of '.$return;["name"].'_data buffer';
                                $arduinoCommandDoxygenParams[] = ' * @param[out] '.$return["name"].'_data '.$return;["description"];
                                $fixedLength = false;
                                $pythonUnpackStr .= 'B';
                                $pythonArgList[] = "'".$return["name"]."': ".$return["name"]."_data";
                                $pythonUnpackList[] = $return["name"]."_len";
                                $pythonHandlerPrintList[] = $return["name"].': %s';
                                $pythonHandlerFormatList[] = "' '.join(['%02X' % b for b in args['".$return["name"]."']])";
                                switch ($return["format"]) {
                                    case "string":
                                        $pythonFriendlyArgList[] = "'".$return["name"]."': ''.join(['%c' % b for b in ".$return["name"]."_data])";
                                        break;
                                    default:
                                        $pythonFriendlyArgList[] = "'".$return["name"]."': ' '.join(['%02X' % b for b in ".$return["name"]."_data])";
                                        break;
                                }
                                $payloadLength++;
                                $pythonDataExtra[] = $return["name"].'_data = [ord(b) for b in self.kgapi_rx_payload['.$payloadLength.':]]';
                                break;
                            case "uint8_t":
                                $arduinoResponseVarList[] = 'uint8_t '.$return["name"].';';
                                $arduinoResponseArgList[] = '&'.$return["name"];
                                $arduinoResponseDefArgList[] = 'uint8_t *'.$return["name"];
                                $arduinoResponseInitializerList[] = $return["name"];
                                $arduinoCommandDoxygenParams[] = ' * @param[out] '.$return["name"].' '.$return["description"];
                                $pythonUnpackStr .= 'B';
                                $pythonArgList[] = "'".$return["name"]."': ".$return["name"];
                                $pythonUnpackList[] = $return["name"];
                                $pythonHandlerPrintList[] = $return["name"].': %02X';
                                $pythonHandlerFormatList[] = "args['".$return["name"]."']";
                                switch (@$return["format"]) {
                                    case "decimal":
                                        $pythonFriendlyArgList[] = "'".$return["name"]."': ('%d".($unit ? " %s": "")."' % (".$return["name"].($multiplier != 1 ? " * ".$multiplier : "").($unit ? ", '".$unit."'".($units ? " if (".$return["name"]." == 1) else '".$units."'" : "") : "")."))";
                                        break;
                                    case "bool":
                                        $pythonFriendlyArgList[] = "'".$return["name"]."': ('%s' % ('TRUE' if ".$return["name"]." else 'FALSE'))";
                                        break;
                                    default:
                                        $pythonFriendlyArgList[] = "'".$return["name"]."': ('%02X' % ".$return["name"].")";
                                        break;
                                }
                                $payloadLength++;
                                break;
                            case "int8_t":
                                $arduinoResponseVarList[] = 'int8_t '.$return["name"].';';
                                $arduinoResponseArgList[] = '&'.$return["name"];
                                $arduinoResponseDefArgList[] = 'int8_t *'.$return["name"];
                                $arduinoResponseInitializerList[] = $return["name"];
                                $arduinoCommandDoxygenParams[] = ' * @param[out] '.$return["name"].' '.$return["description"];
                                $pythonUnpackStr .= 'b';
                                $pythonArgList[] = "'".$return["name"]."': ".$return["name"];
                                $pythonUnpackList[] = $return["name"];
                                $pythonHandlerPrintList[] = $return["name"].': %02X';
                                $pythonHandlerFormatList[] = "args['".$return["name"]."']";
                                switch ($return["format"]) {
                                    case "decimal":
                                        $pythonFriendlyArgList[] = "'".$return["name"]."': ('%d".($unit ? " %s": "")."' % (".$return["name"].($multiplier != 1 ? " * ".$multiplier : "").($unit ? ", '".$unit."'".($units ? " if (".$return["name"]." == 1) else '".$units."'" : "") : "")."))";
                                        break;
                                    default:
                                        $pythonFriendlyArgList[] = "'".$return["name"]."': ('%02X' % ".$return["name"].")";
                                        break;
                                }
                                $payloadLength++;
                                break;
                            case "uint16_t":
                                // the 'uint16_t result' first return value is the actual function return, not passed by reference
                                if ($payloadLength != 0 || $return["name"] != "result") {
                                    $arduinoResponseVarList[] = 'uint16_t '.$return["name"].';';
                                    $arduinoResponseArgList[] = '&'.$return["name"];
                                    $arduinoResponseDefArgList[] = 'uint16_t *'.$return["name"];
                                    $arduinoCommandDoxygenParams[] = ' * @param[out] '.$return["name"].' '.$return["description"];
                                }
                                $arduinoResponseInitializerList[] = $return["name"].' & 0xFF, ('.$return["name"].' >> 8) & 0xFF';
                                $pythonUnpackStr .= 'H';
                                $pythonArgList[] = "'".$return["name"]."': ".$return["name"];
                                $pythonUnpackList[] = $return["name"];
                                $pythonHandlerPrintList[] = $return["name"].': %04X';
                                $pythonHandlerFormatList[] = "args['".$return["name"]."']";
                                switch (@$return["format"]) {
                                    case "decimal":
                                        $pythonFriendlyArgList[] = "'".$return["name"]."': ('%d".($unit ? " %s": "")."' % (".$return["name"].($multiplier != 1 ? " * ".$multiplier : "").($unit ? ", '".$unit."'".($units ? " if (".$return["name"]." == 1) else '".$units."'" : "") : "")."))";
                                        break;
                                    default:
                                        $pythonFriendlyArgList[] = "'".$return["name"]."': ('%04X' % ".$return["name"].")";
                                        break;
                                }
                                $payloadLength += 2;
                                break;
                            case "uint32_t":
                                $arduinoResponseVarList[] = 'uint32_t '.$return["name"].';';
                                $arduinoResponseArgList[] = '&'.$return["name"];
                                $arduinoResponseDefArgList[] = 'uint32_t *'.$return["name"];
                                $arduinoResponseInitializerList[] = $return["name"].' & 0xFF, ('.$return["name"].' >> 8) & 0xFF, ('.$return["name"].' >> 16) & 0xFF, ('.$return["name"].' >> 24) & 0xFF';
                                $arduinoCommandDoxygenParams[] = ' * @param[out] '.$return["name"].' '.$return["description"];
                                $pythonUnpackStr .= 'L';
                                $pythonArgList[] = "'".$return["name"]."': ".$return["name"];
                                $pythonUnpackList[] = $return["name"];
                                $pythonHandlerPrintList[] = $return["name"].': %08X';
                                $pythonHandlerFormatList[] = "args['".$return["name"]."']";
                                switch ($return["format"]) {
                                    case "decimal":
                                        $pythonFriendlyArgList[] = "'".$return["name"]."': ('%d".($unit ? " %s": "")."' % (".$return["name"].($multiplier != 1 ? " * ".$multiplier : "").($unit ? ", '".$unit."'".($units ? " if (".$return["name"]." == 1) else '".$units."'" : "") : "")."))";
                                        break;
                                    default:
                                        $pythonFriendlyArgList[] = "'".$return["name"]."': ('%08X' % ".$return["name"].")";
                                        break;
                                }
                                $payloadLength += 4;
                                break;
                        }
                        $byteMax = $payloadLength + 4 - 1;
                        $payloadHTML .= '<tr class="payload"><td>'.$byteMin;
                        if ($byteMax > $byteMin) $payloadHTML .= '&nbsp;-&nbsp;'.$byteMax;
                        $payloadHTML .= '</td><td>'.$return["type"].'</td><td>'.$return["name"].'</td><td>'.$return["description"];
                        if (!empty($return["references"])) {
                            $payloadHTML .= '<ul>';
                            if (isset($return["references"]["commands"])) {
                                foreach ($return["references"]["commands"] as $refCommand) {
                                    $payloadHTML .= '<li><em>Command:</em> <a href="#kg_cmd_'.$refCommand.'">'.$refCommand.'</a></li>';
                                }
                            }
                            if (isset($return["references"]["events"])) {
                                foreach ($return["references"]["events"] as $refEvent) {
                                    $payloadHTML .= '<li><em>Event:</em> <a href="#kg_evt_'.$refEvent.'">'.$refEvent.'</a></li>';
                                }
                            }
                            if (isset($return["references"]["enumerations"])) {
                                foreach ($return["references"]["enumerations"] as $refEnumeration) {
                                    $payloadHTML .= '<li><em>Enum:</em> <a href="#kg_enum_'.$refEnumeration.'">'.$refEnumeration.'</a></li>';
                                }
                            }
                            $payloadHTML .= '</ul>';
                        }
                        $payloadHTML .= '</td></tr>';
                    }
                    $htmlRef .= '<div class="breakauto">';
                    $htmlRef .= '<table class="response">';
                    $htmlRef .= '<thead>';
                    //$htmlRef .= '<tr><td colspan="4"><img src="kgapi_response.png" alt="KGAPI Command" width="200" height="70" />RESPONSE STRUCTURE</td></tr>';
                    $htmlRef .= '<tr><th colspan="4" class="tabletitle">INCOMING RESPONSE PACKET STRUCTURE</th></tr>';
                    $htmlRef .= '<tr><th>Byte</th><th>Type</th><th>Name</th><th>Description</th></tr>';
                    $htmlRef .= '</thead>';
                    $htmlRef .= '<tbody>';
                    $htmlRef .= '<tr class="header"><td>0</td><td>0xC0</td><td>type</td><td>Response packet</td></tr>';
                    $htmlRef .= '<tr class="header"><td>1</td><td>0x'.sprintf("%02X", $payloadLength).(!$fixedLength ? '+' : '').'</td><td>length</td><td>';
                    if ($payloadLength == 0) {
                        $htmlRef .= 'No payload';
                    } else {
                        $htmlRef .= ($fixedLength ? "Fixed-length payload (".$payloadLength.")" : "Variable-length payload (".$payloadLength."+)");
                    }
                    $htmlRef .= '</td></tr>';
                    $htmlRef .= '<tr class="header"><td>2</td><td>0x'.sprintf("%02X", $class["id"]).'</td><td>class</td><td>Command class: "'.$class["name"].'"</td></tr>';
                    $htmlRef .= '<tr class="header"><td>3</td><td>0x'.sprintf("%02X", $command["id"]).'</td><td>id</td><td>Command ID: "'.$command["name"].'"</td></tr>';
                    $htmlRef .= $payloadHTML;
                    $htmlRef .= '</thead>';
                    $htmlRef .= '</table>';
                    $htmlRef .= '</div>';
                }
                
                // append firmware protocol code
                if (!empty($command["ifcond"])) $arduinoCases[$class["id"]][] = '#if '.$command["ifcond"];
                elseif (!empty($command["ifdef"])) $arduinoCases[$class["id"]][] = '#ifdef '.$command["ifdef"];
                $arduinoCases[$class["id"]][] = 'case KG_PACKET_ID_CMD_'.strtoupper($class["name"].'_'.$command["name"]).': // '.sprintf("0x%02X", $command["id"]);
                $arduinoCases[$class["id"]][] = '    // '.$class["name"].'_'.$command["name"].'('.join(', ', $arduinoCommandCommentArgList).')('.join(', ', $arduinoResponseCommentArgList).')';
                $arduinoCases[$class["id"]][] = '    // parameters = '.$arduinoCommandPayloadLength.' '.($arduinoCommandPayloadLength == 1 ? 'byte' : 'bytes');
                $arduinoCases[$class["id"]][] = '    if (rxPacket[1] '.($arduinoCommandFixedLength ? '!=' : '<').' '.$arduinoCommandPayloadLength.') {';
                $arduinoCases[$class["id"]][] = '        // incorrect parameter length';
                $arduinoCases[$class["id"]][] = '        protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;';
                $arduinoCases[$class["id"]][] = '    } else {';
                $arduinoCases[$class["id"]][] = '        // run command';
                foreach ($arduinoResponseVarList as $arv) $arduinoCases[$class["id"]][] = '        '.$arv;
                $arduinoCases[$class["id"]][] = '        uint16_t result = kg_cmd_'.$class["name"].'_'.$command["name"].'('.join(', ', array_merge($arduinoCommandArgList, $arduinoResponseArgList)).');';
                $arduinoCases[$class["id"]][] = '';
                if (@$command["autoresponse"] == "no") {
                } elseif (@$command["autoresponse"] == "test") {
                    $arduinoCases[$class["id"]][] = '        // build and send response if needed';
                    $arduinoCases[$class["id"]][] = '        if (result != 0xFFFF) {';
                    $arduinoCases[$class["id"]][] = '            // build response';
                    $arduinoCases[$class["id"]][] = '            uint8_t payload['.$payloadLength.'] = { '.join(', ', $arduinoResponseInitializerList).' };';
                    foreach ($arduinoResponseAssignList as $ars) $arduinoCases[$class["id"]][] = '            '.$ars;
                    $arduinoCases[$class["id"]][] = '';
                    $arduinoCases[$class["id"]][] = '            // send response';
                    $arduinoCases[$class["id"]][] = '            send_keyglove_packet(KG_PACKET_TYPE_COMMAND, '.$payloadLength.', rxPacket[2], rxPacket[3], '.($payloadLength ? 'payload' : '0').');';
                    $arduinoCases[$class["id"]][] = '        }';
                } else {
                    $arduinoCases[$class["id"]][] = '        // build response';
                    $arduinoCases[$class["id"]][] = '        uint8_t payload['.$payloadLength.'] = { '.join(', ', $arduinoResponseInitializerList).' };';
                    foreach ($arduinoResponseAssignList as $ars) $arduinoCases[$class["id"]][] = '        '.$ars;
                    $arduinoCases[$class["id"]][] = '';
                    $arduinoCases[$class["id"]][] = '        // send response';
                    $arduinoCases[$class["id"]][] = '        send_keyglove_packet(KG_PACKET_TYPE_COMMAND, '.$payloadLength.', rxPacket[2], rxPacket[3], '.($payloadLength ? 'payload' : '0').');';
                }
                $arduinoCases[$class["id"]][] = '    }';
                $arduinoCases[$class["id"]][] = '    break;';
                if (!empty($command["ifcond"])) $arduinoCases[$class["id"]][] = '#endif // '.$command["ifcond"];
                elseif (!empty($command["ifdef"])) $arduinoCases[$class["id"]][] = '#endif // '.$command["ifdef"];
                $arduinoCases[$class["id"]][] = '';

                if (!empty($command["ifcond"])) $arduinoCommandDeclarations[$class["id"]][] = '#if '.$command["ifcond"];
                elseif (!empty($command["ifdef"])) $arduinoCommandDeclarations[$class["id"]][] = '#ifdef '.$command["ifdef"];
                $arduinoCommandDeclarations[$class["id"]][] = '/* '.sprintf("0x%02X", $command["id"]).' */ uint16_t kg_cmd_'.$class["name"].'_'.$command["name"].'('.join(', ', array_merge($arduinoCommandDefArgList, $arduinoResponseDefArgList)).');';
                if (!empty($command["ifcond"])) $arduinoCommandDeclarations[$class["id"]][] = '#endif // '.$command["ifcond"];
                elseif (!empty($command["ifdef"])) $arduinoCommandDeclarations[$class["id"]][] = '#endif // '.$command["ifdef"];

                $arduinoCommandImplementations[$class["id"]][] = '/**';
                $arduinoCommandImplementations[$class["id"]][] = ' * @brief '.$command["doxbrief"];
                foreach ($arduinoCommandDoxygenParams as $param) $arduinoCommandImplementations[$class["id"]][] = $param;
                $arduinoCommandImplementations[$class["id"]][] = ' * @return Result code (0=success)';
                $arduinoCommandImplementations[$class["id"]][] = ' */';
                $arduinoCommandImplementations[$class["id"]][] = 'uint16_t kg_cmd_'.$class["name"].'_'.$command["name"].'('.join(', ', array_merge($arduinoCommandDefArgList, $arduinoResponseDefArgList)).') {';
                $arduinoCommandImplementations[$class["id"]][] = '    return 0; // success';
                $arduinoCommandImplementations[$class["id"]][] = '}';
                $arduinoCommandImplementations[$class["id"]][] = '';

                // append relevant source code lines for response
                if ($classResponses == 1) {
                    $pythonResponseConditions[] = (empty($pythonResponseConditions) ? 'if' : 'elif').' packet_class == '.$class["id"].': # '.strtoupper($class["name"]);
                    $pythonFriendlyPacketResponseConditions[] = (empty($pythonFriendlyPacketResponseConditions) ? 'if' : 'elif').' packet_class == '.$class["id"].': # '.strtoupper($class["name"]);
                }
                $pythonResponseDeclarations[] = 'kg_rsp_'.$class["name"].'_'.$command["name"].' = KeygloveEvent()';
                $pythonResponseConditions[] = '    '.($classResponses == 1 ? 'if' : 'elif').' packet_command == '.$command["id"].': # kg_rsp_'.$class["name"].'_'.$command["name"];
                $pythonFriendlyPacketResponseConditions[] = '    '.($classResponses == 1 ? 'if' : 'elif').' packet_command == '.$command["id"].': # kg_rsp_'.$class["name"].'_'.$command["name"];
                if (!empty($pythonUnpackList)) {
                    $pythonResponseConditions[] = '        '.join(', ', $pythonUnpackList).', = struct.unpack(\'<'.$pythonUnpackStr.'\', self.kgapi_rx_payload[:'.$payloadLength.'])';
                    $pythonFriendlyPacketResponseConditions[] = '        '.join(', ', $pythonUnpackList).', = struct.unpack(\'<'.$pythonUnpackStr.'\', self.kgapi_rx_payload[:'.$payloadLength.'])';
                }
                foreach ($pythonDataExtra as $pde) {
                    $pythonResponseConditions[] = '        '.$pde;
                    $pythonFriendlyPacketResponseConditions[] = '        '.$pde;
                }
                $pythonResponseConditions[] = '        self.last_response = { \'length\': payload_length, \'class_id\': packet_class, \'command_id\': packet_command, \'payload\': { '.join(', ', $pythonArgList).' }, \'raw\': self.kgapi_last_rx_packet }';
                $pythonResponseConditions[] = '        self.kg_rsp_'.$class["name"].'_'.$command["name"].'(self.last_response[\'payload\'])';
                $pythonFriendlyPacketResponseConditions[] = '        return { \'type\': \'response\', \'name\': \'kg_rsp_'.$class["name"].'_'.$command["name"].'\', \'length\': payload_length, \'class_id\': packet_class, \'command_id\': packet_command, \'payload\': { '.join(', ', $pythonFriendlyArgList).' }, \'payload_keys\': [ '.(!empty($pythonResponseArgList) ? "'".join("', '", $pythonResponseArgList)."'" : "").' ] }';
                $pythonHandlerDefinitions[] = 'def my_kg_rsp_'.$class["name"].'_'.$command["name"].'(sender, args):';
                $pythonHandlerDefinitions[] = '    print("kg_rsp_'.$class["name"].'_'.$command["name"].': { '.join(', ', $pythonHandlerPrintList).' }"'.(!empty($pythonHandlerFormatList) ? ' % ('.join(', ', $pythonHandlerFormatList).')' : '').')';
                $pythonHandlerDefinitions[] = '';
                $pythonHandlerAssignments[] = 'keyglove.kg_rsp_'.$class["name"].'_'.$command["name"].' += my_kg_rsp_'.$class["name"].'_'.$command["name"];

                // append relevant GUI elements for command/response
                $pythonGUIPageDefinitions[] = '        hbox_'.$class["name"].'_'.$command["name"].' = wx.BoxSizer(wx.HORIZONTAL)';
                $pythonGUIPageDefinitions[] = '        self.btn_'.$class["name"].'_'.$command["name"].' = wx.Button(self, id='.sprintf("1%02d%02d", $class["id"], $command["id"]).', label="'.$class["name"].'_'.$command["name"].'")';
                $pythonGUIPageDefinitions[] = '        self.Bind(wx.EVT_BUTTON, self.OnCommandClick, self.btn_'.$class["name"].'_'.$command["name"].')';
                $pythonGUIPageDefinitions[] = '        hbox_'.$class["name"].'_'.$command["name"].'.Add(self.btn_'.$class["name"].'_'.$command["name"].', 1, wx.TOP | wx.BOTTOM, 2)';
                foreach ($pythonGUICommandFields as $pgcf) $pythonGUIPageDefinitions[] = '        '.$pgcf;
                $pythonGUIPageDefinitions[] = '        commandVBox.Add(hbox_'.$class["name"].'_'.$command["name"].', 1, wx.LEFT | wx.RIGHT, 8)';
                $pythonGUIPageDefinitions[] = '';

                // add Python example snippets
                $htmlRef .= '<h5><span class="headingtab">'.$htmlRefNav[0].'.'.$htmlRefNav[1].'.'.$htmlRefNav[2].'.'.$htmlRefNav[3].'</span> Example Usage (Python)</h5>';
                $htmlRef .= '<pre><code class="python"># generate command packet only
'.wrap_code_line('packet = kglib.kg_cmd_'.$class["name"].'_'.$command["name"].'('.join(', ', $pythonCommandArgList).')', 92, 'python').'</code></pre>';

                $htmlRef .= '<pre><code class="python"># generate and send command
'.wrap_code_line('kglib.send_command(rxtx_obj, kglib.kg_cmd_'.$class["name"].'_'.$command["name"].'('.join(', ', $pythonCommandArgList).'))', 92, 'python').'</code></pre>';

                $htmlRef .= '<pre><code class="python"># send command and wait for captured response
'.wrap_code_line('response = kglib.send_and_return(rxtx_obj, kglib.kg_cmd_'.$class["name"].'_'.$command["name"].'('.join(', ', $pythonCommandArgList).'), timeout)', 92, 'python').'
'.wrap_code_line('print("kg_rsp_'.$class["name"].'_'.$command["name"].': { '.join(', ', $pythonHandlerPrintList).' }"'.(!empty($pythonHandlerFormatList) ? ' % ('.str_replace("args['", "response['payload']['", join(', ', $pythonHandlerFormatList)).')' : '').')', 92, 'python').'</code></pre>';

$htmlRef .= '<pre><code class="python"># create separate callback for response
def my_kg_rsp_'.$class["name"].'_'.$command["name"].'(sender, args):
'.wrap_code_line('    print("kg_rsp_'.$class["name"].'_'.$command["name"].': { '.join(', ', $pythonHandlerPrintList).' }"'.(!empty($pythonHandlerFormatList) ? ' % ('.join(', ', $pythonHandlerFormatList).')' : '').')', 92, 'python').'

# assign separate callback function to appropriate KGLib response handler collection
kglib.kg_rsp_'.$class["name"].'_'.$command["name"].' += my_kg_rsp_'.$class["name"].'_'.$command["name"].'</code></pre>';
                $htmlRefNav[3]++;

                /*
                // add C example snippets
                $htmlRef .= '<h5><span class="headingtab">'.$htmlRefNav[0].'.'.$htmlRefNav[1].'.'.$htmlRefNav[2].'.'.$htmlRefNav[3].'</span> Example Usage (C)</h5>';
                $htmlRef .= '<pre><code class="c">// generate command packet only
'.wrap_code_line('uint8_t *packet = kglib_kg_cmd_'.$class["name"].'_'.$command["name"].'('.join(', ', $cCommandArgList).');', 92, 'c').'</code></pre>';

                $htmlRef .= '<pre><code class="c">// generate and send command
'.wrap_code_line('kglib_send_command(rxtx_obj, kglib_kg_cmd_'.$class["name"].'_'.$command["name"].'('.join(', ', $cCommandArgList).'));', 92, 'c').'</code></pre>';

                $htmlRef .= '<pre><code class="c">// send command and wait for captured response
'.wrap_code_line('response = kglib.send_and_return(rxtx_obj, kglib.kg_cmd_'.$class["name"].'_'.$command["name"].'('.join(', ', $cCommandArgList).'), timeout);', 92, 'c').'
'.wrap_code_line('printf("kg_rsp_'.$class["name"].'_'.$command["name"].': { '.join(', ', $cHandlerPrintList).' }"'.(!empty($cHandlerFormatList) ? ', '.str_replace("args['", "response['payload']['", join(', ', $cHandlerFormatList)) : '').');', 92, 'c').'</code></pre>';

$htmlRef .= '<pre><code class="c">// create separate callback for response
void my_kg_rsp_'.$class["name"].'_'.$command["name"].'(kg_rsp_'.$class["name"].'_'.$command["name"].'_t *payload) {
'.wrap_code_line('    printf("kg_rsp_'.$class["name"].'_'.$command["name"].': { '.join(', ', $cHandlerPrintList).' }"'.(!empty($cHandlerFormatList) ? ', '.join(', ', $cHandlerFormatList) : '').');', 92, 'c').'
}
    
// assign callback function to appropriate KGLib event handler function pointer
kglib_kg_rsp_'.$class["name"].'_'.$command["name"].' = my_kg_rsp_'.$class["name"].'_'.$command["name"].';</code></pre>';*/
                $htmlRefNav[3]++;

                $htmlRefNav[2]++;
                $htmlRefNav[3] = 1;
            }
            $htmlRefNav[1]++; 
            $htmlRefNav[2] = 1;
        } else {
            // no commands to show in GUI            
            $pythonGUIPageDefinitions[] = '        labelNoCommands = wx.StaticText(self, -1, "No commands implemented")';
            $pythonGUIPageDefinitions[] = '        labelNoCommands.SetFont(labelNoCommands.GetFont().MakeItalic())';
            $pythonGUIPageDefinitions[] = '        commandVBox.Add(labelNoCommands, 1, wx.ALL, 8)';
            $pythonGUIPageDefinitions[] = '';
        }

        // add Python GUI event label        
        //$pythonGUIPageDefinitions[] = '        labelEvents = wx.StaticText(self, -1, "'.ucfirst($class["name"]).' events")';
        //$pythonGUIPageDefinitions[] = '        labelEvents.SetFont(labelEvents.GetFont().MakeBold())';
        //$pythonGUIPageDefinitions[] = '        eventVBox.Add(labelEvents)';

        if (!empty($class["events"])) {
            $htmlRef .= '<h3><span class="headingtab">'.$htmlRefNav[0].'.'.$htmlRefNav[1].'</span> Events</h3>';
            foreach ($class["events"] as $event) {
                $classEvents++;
                $fixedLength = true;
                $payloadHTML = '';
                $payloadLength = 0;

                echo "  + kg_evt_".$class["name"]."_".$event["name"]." (".$class["id"]."/".$event["id"].")\n";

                $arduinoEventMacros[$class["id"]][] = "#define ".str_pad("KG_PACKET_ID_EVT_".strtoupper($class["name"].'_'.$event["name"]), 48, " ", STR_PAD_RIGHT).'    '.sprintf("0x%02X", $event["id"]);

                $arduinoEventDefArgList = array();
                $arduinoEventDoxygenParams = array();

                // deal with response packet
                $pythonUnpackStr = '';
                $pythonLengthExtra = '';
                $pythonDataExtra = array();
                $pythonUnpackList = array();
                $pythonArgList = array();
                $pythonHandlerPrintList = array();
                $pythonHandlerFormatList = array();
                $pythonFriendlyArgList = array();
                $pythonEventArgList = array();
                
                $cArgList = array();
                $cHandlerPrintList = array();
                $cHandlerFormatList = array();

                // run through each defined event parameter
                foreach ($event["parameters"] as $parameter) {
                    $byteMin = $payloadLength + 4;
                    $pythonEventArgList[] = $parameter["name"];
                    switch ($parameter["type"]) {
                        case "macaddr_t":
                            $arduinoEventDefArgList[] = 'uint8_t *'.$parameter["name"];
                            $arduinoEventDoxygenParams[] = ' * @param[in] '.$parameter["name"].' '.$parameter["description"];
                            $pythonUnpackStr .= '6s';
                            $pythonArgList[] = "'".$parameter["name"]."': ".$parameter["name"];
                            $pythonUnpackList[] = $parameter["name"];
                            $pythonHandlerPrintList[] = $parameter["name"].': %s';
                            $pythonHandlerFormatList[] = "' '.join(['%02X' % b for b in args['".$parameter["name"]."']])";
                            $pythonFriendlyArgList[] = "'".$parameter["name"]."': ':'.join(['%02X' % b for b in ".$parameter["name"]."][::-1])";
                            $pythonDataExtra[] = $parameter["name"].' = [ord(b) for b in self.kgapi_rx_payload['.$payloadLength.':'.($payloadLength + 6).']]';
                            $payloadLength += 6;
                            break;
                        case "btcod_t":
                            $arduinoEventDefArgList[] = 'uint8_t *'.$parameter["name"];
                            $arduinoEventDoxygenParams[] = ' * @param[in] '.$parameter["name"].' '.$parameter["description"];
                            $pythonUnpackStr .= '3s';
                            $pythonArgList[] = "'".$parameter["name"]."': ".$parameter["name"];
                            $pythonUnpackList[] = $parameter["name"];
                            $pythonHandlerPrintList[] = $parameter["name"].': %s';
                            $pythonHandlerFormatList[] = "' '.join(['%02X' % b for b in args['".$parameter["name"]."']])";
                            $pythonFriendlyArgList[] = "'".$parameter["name"]."': ''.join(['%02X' % b for b in ".$parameter["name"]."][::-1])";
                            $pythonDataExtra[] = $parameter["name"].' = [ord(b) for b in self.kgapi_rx_payload['.$payloadLength.':'.($payloadLength + 3).']]';
                            $payloadLength += 3;
                            break;
                        case "uint8_t[]":
                            $arduinoEventDefArgList[] = 'uint8_t '.$parameter["name"].'_len';
                            $arduinoEventDefArgList[] = 'uint8_t *'.$parameter["name"].'_data';
                            $arduinoEventDoxygenParams[] = ' * @param[in] '.$parameter["name"].'_len Length in bytes of '.$parameter["name"].'_data buffer';
                            $arduinoEventDoxygenParams[] = ' * @param[in] '.$parameter["name"].'_data '.$parameter["description"];
                            $fixedLength = false;
                            $pythonUnpackStr .= 'B';
                            $pythonArgList[] = "'".$parameter["name"]."': ".$parameter["name"]."_data";
                            $pythonUnpackList[] = $parameter["name"]."_len";
                            $pythonHandlerPrintList[] = $parameter["name"].': %s';
                            $pythonHandlerFormatList[] = "' '.join(['%02X' % b for b in args['".$parameter["name"]."']])";
                            switch (@$parameter["format"]) {
                                case "string":
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ''.join(['%c' % b for b in ".$parameter["name"]."_data])";
                                    break;
                                default:
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ' '.join(['%02X' % b for b in ".$parameter["name"]."_data])";
                                    break;
                            }
                            $payloadLength++;
                            $pythonDataExtra[] = $parameter["name"].'_data = [ord(b) for b in self.kgapi_rx_payload['.$payloadLength.':]]';
                            break;
                        case "uint8_t":
                            $arduinoEventDefArgList[] = 'uint8_t '.$parameter["name"];
                            $arduinoEventDoxygenParams[] = ' * @param[in] '.$parameter["name"].' '.$parameter["description"];
                            $pythonUnpackStr .= 'B';
                            $pythonArgList[] = "'".$parameter["name"]."': ".$parameter["name"];
                            $pythonUnpackList[] = $parameter["name"];
                            $pythonHandlerPrintList[] = $parameter["name"].': %02X';
                            $pythonHandlerFormatList[] = "args['".$parameter["name"]."']";
                            switch (@$parameter["format"]) {
                                case "decimal":
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%d".($unit ? " %s": "")."' % (".$parameter["name"].($multiplier != 1 ? " * ".$multiplier : "").($unit ? ", '".$unit."'".($units ? " if (".$parameter["name"]." == 1) else '".$units."'" : "") : "")."))";
                                    break;
                                case "bool":
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%s' % ('TRUE' if ".$parameter["name"]." else 'FALSE'))";
                                    break;
                                default:
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%02X' % ".$parameter["name"].")";
                                    break;
                            }
                            $payloadLength++;
                            break;
                        case "int8_t":
                            $arduinoEventDefArgList[] = 'int8_t '.$parameter["name"];
                            $arduinoEventDoxygenParams[] = ' * @param[in] '.$parameter["name"].' '.$parameter["description"];
                            $pythonUnpackStr .= 'b';
                            $pythonArgList[] = "'".$parameter["name"]."': ".$parameter["name"];
                            $pythonUnpackList[] = $parameter["name"];
                            $pythonHandlerPrintList[] = $parameter["name"].': %02X';
                            $pythonHandlerFormatList[] = "args['".$parameter["name"]."']";
                            switch (@$parameter["format"]) {
                                case "decimal":
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%d".($unit ? " %s": "")."' % (".$parameter["name"].($multiplier != 1 ? " * ".$multiplier : "").($unit ? ", '".$unit."'".($units ? " if (".$parameter["name"]." == 1) else '".$units."'" : "") : "")."))";
                                    break;
                                default:
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%02X' % ".$parameter["name"].")";
                                    break;
                            }
                            $payloadLength++;
                            break;
                        case "uint16_t":
                            $arduinoEventDefArgList[] = 'uint16_t '.$parameter["name"];
                            $arduinoEventDoxygenParams[] = ' * @param[in] '.$parameter["name"].' '.$parameter["description"];
                            $pythonUnpackStr .= 'H';
                            $pythonArgList[] = "'".$parameter["name"]."': ".$parameter["name"];
                            $pythonUnpackList[] = $parameter["name"];
                            $pythonHandlerPrintList[] = $parameter["name"].': %04X';
                            $pythonHandlerFormatList[] = "args['".$parameter["name"]."']";
                            switch (@$parameter["format"]) {
                                case "decimal":
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%d".($unit ? " %s": "")."' % (".$parameter["name"].($multiplier != 1 ? " * ".$multiplier : "").($unit ? ", '".$unit."'".($units ? " if (".$parameter["name"]." == 1) else '".$units."'" : "") : "")."))";
                                    break;
                                default:
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%04X' % ".$parameter["name"].")";
                                    break;
                            }
                            $payloadLength += 2;
                            break;
                        case "uint32_t":
                            $arduinoEventDefArgList[] = 'uint32_t '.$parameter["name"];
                            $arduinoEventDoxygenParams[] = ' * @param[in] '.$parameter["name"].' '.$parameter["description"];
                            $pythonUnpackStr .= 'L';
                            $pythonArgList[] = "'".$parameter["name"]."': ".$parameter["name"];
                            $pythonUnpackList[] = $parameter["name"];
                            $pythonHandlerPrintList[] = $parameter["name"].': %08X';
                            $pythonHandlerFormatList[] = "args['".$parameter["name"]."']";
                            switch (@$parameter["format"]) {
                                case "decimal":
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%d".($unit ? " %s": "")."' % (".$parameter["name"].($multiplier != 1 ? " * ".$multiplier : "").($unit ? ", '".$unit."'".($units ? " if (".$parameter["name"]." == 1) else '".$units."'" : "") : "")."))";
                                    break;
                                default:
                                    $pythonFriendlyArgList[] = "'".$parameter["name"]."': ('%08X' % ".$parameter["name"].")";
                                    break;
                            }
                            $payloadLength += 4;
                            break;
                    }
                    $byteMax = $payloadLength + 4 - 1;
                    $payloadHTML .= '<tr class="payload"><td>'.$byteMin;
                    if ($byteMax > $byteMin) $payloadHTML .= '&nbsp;-&nbsp;'.$byteMax;
                    $payloadHTML .= '</td><th>'.$parameter["type"].'</th><th>'.$parameter["name"].'</th><td>'.$parameter["description"];
                    if (!empty($parameter["references"])) {
                        $payloadHTML .= '<ul>';
                        if (isset($parameter["references"]["commands"])) {
                            foreach ($parameter["references"]["commands"] as $refCommand) {
                                $payloadHTML .= '<li><em>Command:</em> <a href="#kg_cmd_'.$refCommand.'">'.$refCommand.'</a></li>';
                            }
                        }
                        if (isset($parameter["references"]["events"])) {
                            foreach ($parameter["references"]["events"] as $refEvent) {
                                $payloadHTML .= '<li><em>Event:</em> <a href="#kg_evt_'.$refEvent.'">'.$refEvent.'</a></li>';
                            }
                        }
                        if (isset($parameter["references"]["enumerations"])) {
                            foreach ($parameter["references"]["enumerations"] as $refEnumeration) {
                                $payloadHTML .= '<li><em>Enum:</em> <a href="#kg_enum_'.$refEnumeration.'">'.$refEnumeration.'</a></li>';
                            }
                        }
                        $payloadHTML .= '</ul>';
                    }
                    $payloadHTML .= '</td></tr>';
                }
                $htmlRef .= '<h4><span class="headingtab">'.$htmlRefNav[0].'.'.$htmlRefNav[1].'.'.$htmlRefNav[2].'</span> '.$class["name"].'_'.$event["name"].' <code style="color: #F00;">[ 80 '.sprintf("%02X", $payloadLength).($fixedLength ? '' : '+').' '.sprintf("%02X %02X", $class["id"], $event["id"]).' '.($payloadLength > 0 ? '... ' : '').']</code></h4>'; // ('.$command["id"].')';
                $htmlRef .= $event["description"];
                $htmlRef .= '<div class="breakauto">';
                $htmlRef .= '<table class="event">';
                $htmlRef .= '<thead>';
                //$htmlRef .= '<tr><td colspan="4"><img src="kgapi_event.png" alt="KGAPI Command" width="200" height="70" />EVENT STRUCTURE</td></tr>';
                $htmlRef .= '<tr><th colspan="4" class="tabletitle">INCOMING EVENT PACKET STRUCTURE</th></tr>';
                $htmlRef .= '<tr><th>Byte</th><th>Type</th><th>Name</th><th>Description</th></tr>';
                $htmlRef .= '</thead>';
                $htmlRef .= '<tbody>';
                $htmlRef .= '<tr class="header"><td>0</td><td>0x80</td><td>type</td><td>Event packet</td></tr>';
                $htmlRef .= '<tr class="header"><td>1</td><td>0x'.sprintf("%02X", $payloadLength).(!$fixedLength ? '+' : '').'</td><td>length</td><td>';
                if ($payloadLength == 0) {
                    $htmlRef .= 'No payload';
                } else {
                    $htmlRef .= ($fixedLength ? "Fixed-length payload (".$payloadLength.")" : "Variable-length payload (".$payloadLength."+)");
                }
                $htmlRef .= '</td></tr>';
                $htmlRef .= '<tr class="header"><td>2</td><td>0x'.sprintf("%02X", $class["id"]).'</td><td>class</td><td>Event class: "'.$class["name"].'"</td></tr>';
                $htmlRef .= '<tr class="header"><td>3</td><td>0x'.sprintf("%02X", $event["id"]).'</td><td>id</td><td>Event ID: "'.$event["name"].'"</td></tr>';
                $htmlRef .= $payloadHTML;
                $htmlRef .= '</tbody>';
                $htmlRef .= '</table>';
                $htmlRef .= '</div>';
                
                // append firmware protocol code
                if (!empty($event["ifcond"])) $arduinoEventDeclarations[$class["id"]][] = '#if '.$event["ifcond"];
                elseif (!empty($event["ifdef"])) $arduinoEventDeclarations[$class["id"]][] = '#ifdef '.$event["ifdef"];
                $arduinoEventDeclarations[$class["id"]][] = '/* '.sprintf("0x%02X", $event["id"]).' */ uint8_t (*kg_evt_'.$class["name"].'_'.$event["name"].')('.join(', ', $arduinoEventDefArgList).');';
                if (!empty($event["ifcond"])) $arduinoEventDeclarations[$class["id"]][] = '#endif // '.$event["ifcond"];
                elseif (!empty($event["ifdef"])) $arduinoEventDeclarations[$class["id"]][] = '#endif // '.$event["ifdef"];

                $arduinoEventImplementations[$class["id"]][] = '/**';
                $arduinoEventImplementations[$class["id"]][] = ' * @brief '.$event["doxbrief"];
                foreach ($arduinoEventDoxygenParams as $param) $arduinoEventImplementations[$class["id"]][] = $param;
                $arduinoEventImplementations[$class["id"]][] = ' * @return KGAPI event packet fallthrough, zero allows and non-zero prevents';
                $arduinoEventImplementations[$class["id"]][] = ' */';
                $arduinoEventImplementations[$class["id"]][] = 'uint8_t my_kg_evt_'.$class["name"].'_'.$event["name"].'('.join(', ', $arduinoEventDefArgList).') {';
                $arduinoEventImplementations[$class["id"]][] = '    // TODO: special event handler code here';
                $arduinoEventImplementations[$class["id"]][] = '    // ...';
                $arduinoEventImplementations[$class["id"]][] = '';
                $arduinoEventImplementations[$class["id"]][] = '    return 0; // 0=send event API packet, otherwise skip sending';
                $arduinoEventImplementations[$class["id"]][] = '}';
                $arduinoEventImplementations[$class["id"]][] = '';

                // append relevant source code lines for event
                if ($classEvents == 1) {
                    $pythonEventConditions[] = (empty($pythonEventConditions) ? 'if' : 'elif').' packet_class == '.$class["id"].': # '.strtoupper($class["name"]);
                    $pythonFriendlyPacketEventConditions[] = (empty($pythonFriendlyPacketEventConditions) ? 'if' : 'elif').' packet_class == '.$class["id"].': # '.strtoupper($class["name"]);
                }
                $pythonEventDeclarations[] = 'kg_evt_'.$class["name"].'_'.$event["name"].' = KeygloveEvent()';
                $pythonEventConditions[] = '    '.($classEvents == 1 ? 'if' : 'elif').' packet_command == '.$event["id"].': # kg_evt_'.$class["name"].'_'.$event["name"];
                $pythonFriendlyPacketEventConditions[] = '    '.($classEvents == 1 ? 'if' : 'elif').' packet_command == '.$event["id"].': # kg_evt_'.$class["name"].'_'.$event["name"];
                if (!empty($pythonUnpackList)) {
                    $pythonEventConditions[] = '        '.join(', ', $pythonUnpackList).', = struct.unpack(\'<'.$pythonUnpackStr.'\', self.kgapi_rx_payload[:'.$payloadLength.'])';
                    $pythonFriendlyPacketEventConditions[] = '        '.join(', ', $pythonUnpackList).', = struct.unpack(\'<'.$pythonUnpackStr.'\', self.kgapi_rx_payload[:'.$payloadLength.'])';
                }
                foreach ($pythonDataExtra as $pde) {
                    $pythonEventConditions[] = '        '.$pde;
                    $pythonFriendlyPacketEventConditions[] = '        '.$pde;
                }
                $pythonEventConditions[] = '        self.last_event = { \'length\': payload_length, \'class_id\': packet_class, \'event_id\': packet_command, \'payload\': { '.join(', ', $pythonArgList).' }, \'raw\': self.kgapi_last_rx_packet }';
                $pythonEventConditions[] = '        self.kg_evt_'.$class["name"].'_'.$event["name"].'(self.last_event[\'payload\'])';
                $pythonFriendlyPacketEventConditions[] = '        return { \'type\': \'event\', \'name\': \'kg_evt_'.$class["name"].'_'.$event["name"].'\', \'length\': payload_length, \'class_id\': packet_class, \'event_id\': packet_command, \'payload\': { '.join(', ', $pythonFriendlyArgList).' }, \'payload_keys\': [ '.(!empty($pythonEventArgList) ? "'".join("', '", $pythonEventArgList)."'" : "").' ] }';
                
                $pythonHandlerDefinitions[] = 'def my_kg_evt_'.$class["name"].'_'.$event["name"].'(sender, args):';
                $pythonHandlerDefinitions[] = '    print("kg_evt_'.$class["name"].'_'.$event["name"].': { '.join(', ', $pythonHandlerPrintList).' }"'.(!empty($pythonHandlerFormatList) ? ' % ('.join(', ', $pythonHandlerFormatList).')' : '').')';
                $pythonHandlerDefinitions[] = '';
                $pythonHandlerAssignments[] = 'keyglove.kg_evt_'.$class["name"].'_'.$event["name"].' += my_kg_evt_'.$class["name"].'_'.$event["name"];

                // append relevant GUI elements for event
                //$pythonGUIPageDefinitions[] = '        self.chk_'.$class["name"].'_'.$event["name"].' = wx.CheckBox(self, id='.sprintf("2%02d%02d", $class["id"], $event["id"]).', label="'.$class["name"].'_'.$event["name"].'")';
                //$pythonGUIPageDefinitions[] = '        self.chk_'.$class["name"].'_'.$event["name"].'.SetValue(True)';
                //$pythonGUIPageDefinitions[] = '        self.Bind(wx.EVT_CHECKBOX, self.OnEventClick, self.chk_'.$class["name"].'_'.$event["name"].')';
                //$pythonGUIPageDefinitions[] = '        eventVBox.Add(self.chk_'.$class["name"].'_'.$event["name"].', 1, wx.ALL, 6)';
                //$pythonGUIPageDefinitions[] = '';

                // add Python example snippets
                $htmlRef .= '<h5><span class="headingtab">'.$htmlRefNav[0].'.'.$htmlRefNav[1].'.'.$htmlRefNav[2].'.'.$htmlRefNav[3].'</span> Example Usage (Python)</h5>';
$htmlRef .= '<pre><code class="python"># create callback for event
def my_kg_evt_'.$class["name"].'_'.$event["name"].'(sender, args):
'.wrap_code_line('    print("kg_evt_'.$class["name"].'_'.$event["name"].': { '.join(', ', $pythonHandlerPrintList).' }"'.(!empty($pythonHandlerFormatList) ? ' % ('.join(', ', $pythonHandlerFormatList).')' : '').')', 92, 'python').'
    
# assign callback function to appropriate KGLib event handler collection
kglib.kg_evt_'.$class["name"].'_'.$event["name"].' += my_kg_evt_'.$class["name"].'_'.$event["name"].'</code></pre>';
                $htmlRefNav[3]++;
                
                /*
                // add C example snippets
                $htmlRef .= '<h5><span class="headingtab">'.$htmlRefNav[0].'.'.$htmlRefNav[1].'.'.$htmlRefNav[2].'.'.$htmlRefNav[3].'</span> Example Usage (C)</h5>';
$htmlRef .= '<pre><code class="c">// create callback for event
void my_kg_evt_'.$class["name"].'_'.$event["name"].'(struct kg_evt_'.$class["name"].'_'.$event["name"].'_t *payload) {
'.wrap_code_line('    printf("kg_evt_'.$class["name"].'_'.$event["name"].': { '.join(', ', $cHandlerPrintList).' }"'.(!empty($cHandlerFormatList) ? ', '.join(', ', $cHandlerFormatList) : '').');', 92, 'c').'
}
    
// assign callback function to appropriate KGLib event handler function pointer
kglib_kg_evt_'.$class["name"].'_'.$event["name"].' = my_kg_evt_'.$class["name"].'_'.$event["name"].';</code></pre>';*/
                $htmlRefNav[3]++;
                
                $htmlRefNav[2]++;
                $htmlRefNav[3] = 1;
            }
            $htmlRefNav[1]++; 
            $htmlRefNav[2] = 1;
        } else {
            // no events to show in GUI            
            //$pythonGUIPageDefinitions[] = '        labelNoEvents = wx.StaticText(self, -1, "No events implemented")';
            //$pythonGUIPageDefinitions[] = '        labelNoEvents.SetFont(labelNoEvents.GetFont().MakeItalic())';
            //$pythonGUIPageDefinitions[] = '        eventVBox.Add(labelNoEvents, 1, wx.ALL, 8)';
            //$pythonGUIPageDefinitions[] = '';
        }
        
        // finish up Python GUI page
        $pythonGUIPageDefinitions[] = '        self.SetSizer(pageHBox)';
        $pythonGUIPageDefinitions[] = '';
        $pythonGUIPageDefinitions[] = '    def OnCommandClick(self, event):';
        //$pythonGUIPageDefinitions[] = '        print(event.Id)';
        foreach ($pythonGUICommandButtonHandlers as $pgcbh) $pythonGUIPageDefinitions[] = '        '.$pgcbh;
        if (empty($pythonGUICommandButtonHandlers)) $pythonGUIPageDefinitions[] = '        """ no commands defined """';
        $pythonGUIPageDefinitions[] = '';
        $pythonGUIPageDefinitions[] = '    def OnEventClick(self, event):';
        $pythonGUIPageDefinitions[] = '        print(event.Id, event.IsChecked())';
        $pythonGUIPageDefinitions[] = '';
        
        if (!empty($class["enumerations"])) {
            $htmlRef .= '<h3><span class="headingtab">'.$htmlRefNav[0].'.'.$htmlRefNav[1].'</span> Enumerations</h3>';
            foreach ($class["enumerations"] as $enumeration) {
                $htmlRef .= '<h4><span class="headingtab">'.$htmlRefNav[0].'.'.$htmlRefNav[1].'.'.$htmlRefNav[2].'</span> '.$class["name"].'_'.$enumeration["name"].'</h4>'; // ('.$command["id"].')';
                $htmlRef .= $enumeration["description"];
                $htmlRef .= '<table class="enumeration">';
                $htmlRef .= '<thead>';
                $htmlRef .= '<tr><th>Value</th><th>Name</th><th>Description</th></tr>';
                $htmlRef .= '</thead>';
                $htmlRef .= '<tbody>';
                foreach ($enumeration["values"] as $value) {
                    $htmlRef .= '<tr><td>'.$value["value"].'</td><td>'.$value["name"].'</td><td>'.$value["description"].'</td></tr>';
                }
                $htmlRef .= '</tbody>';
                $htmlRef .= '</table>';
            }
            $htmlRefNav[1]++; 
            $htmlRefNav[2] = 1;
        }
        $htmlRefNav[0]++;
        $htmlRefNav[1] = 1;
        
        if ($classCommands > 0) {
            $pythonCommandDefinitions[] = '';
        }
        if ($classResponses > 0) {
            $pythonResponseDeclarations[] = '';
        }
        if ($classEvents > 0) {
            $pythonEventDeclarations[] = '';
        }
    }
}

// get "right now" time
$now = new DateTime('now', new DateTimeZone('America/New_York'));
//$now = new DateTime('2014-11-28', new DateTimeZone('America/New_York'));

// build Arduino firmware protocol support files from template
echo "Building KGAPI Reference HTML document\n";
$templateHTMLRef = file_get_contents("template.kgapi_reference.html");
$lines = explode("\n", $templateHTMLRef);
$lines2 = array();
foreach ($lines as $line) {
    $count = preg_match_all('/(.*?)\{%([a-zA-Z0-9_]+)%\}/', $line, $matches);
    for ($i = 0; $i < $count; $i++) {
        $indent = 0;
        if (trim($matches[1][$i]) == "") $indent = strlen($matches[1][$i]);
        $replacement = false;
        switch ($matches[2][$i]) {
            case "date_ymd":
                $replacement = $now -> format("Y-m-d");
                break;
            case "date_year":
                $replacement = $now -> format("Y");
                break;
            case "apiref_html":
                $replacement = $htmlRef;
                break;
        }
        if ($replacement !== false) $line = str_replace('{%'.$matches[2][$i].'%}', $replacement, $line);
    }
    $lines2[] = $line;
}
$templateHTMLRef = join("\n", $lines2);
echo "--> Writing '../../docs/api/html/kgapi_reference.html'\n";
file_put_contents('../../docs/api/html/kgapi_reference.html', $templateHTMLRef);

// build Arduino firmware protocol support files from template
echo "Building Arduino controller firmware protocol support files\n";
foreach ($kgapi["classes"] as $class) {
    $doxygenSeeContent = '';
    $declarationLines = array();
    $declarationLines[] = '/* =========================== */';
    $declarationLines[] = '/* KGAPI CONSTANT DECLARATIONS */';
    $declarationLines[] = '/* =========================== */';
    $declarationLines[] = '';
    foreach ($arduinoCommandMacros[$class["id"]] as $def) $declarationLines[] = $def;
    if (!empty($arduinoCommandMacros[$class["id"]]) && !empty($arduinoEventMacros[$class["id"]])) $declarationLines[] = '// -- command/event split --';
    foreach ($arduinoEventMacros[$class["id"]] as $def) $declarationLines[] = $def;
    if (!empty($arduinoCommandMacros[$class["id"]]) || !empty($arduinoEventMacros[$class["id"]])) $declarationLines[] = '';
    $declarationLines[] = '/* ================================ */';
    $declarationLines[] = '/* KGAPI COMMAND/EVENT DECLARATIONS */';
    $declarationLines[] = '/* ================================ */';
    $declarationLines[] = '';
    foreach ($arduinoCommandDeclarations[$class["id"]] as $def) $declarationLines[] = $def;
    if (!empty($arduinoCommandDeclarations[$class["id"]]) && !empty($arduinoEventDeclarations[$class["id"]])) $declarationLines[] = '// -- command/event split --';
    foreach ($arduinoEventDeclarations[$class["id"]] as $def) $declarationLines[] = $def;
    if (!empty($arduinoCommandDeclarations[$class["id"]]) || !empty($arduinoEventDeclarations[$class["id"]])) $declarationLines[] = '';
    if (!empty($arduinoCommandImplementations[$class["id"]])) {
        $declarationLines[] = '/* ============================= */';
        $declarationLines[] = '/* KGAPI COMMAND IMPLEMENTATIONS */';
        $declarationLines[] = '/* ============================= */';
        $declarationLines[] = '';
        foreach ($arduinoCommandImplementations[$class["id"]] as $def) $declarationLines[] = $def;
        $declarationLines[] = '';
    }
    if (!empty($arduinoEventImplementations[$class["id"]])) {
        $eventStubLines[] = '//////////////////////////////// '.strtoupper($class["name"]).' ////////////////////////////////';
        $eventStubLines[] = '';
        foreach ($arduinoEventImplementations[$class["id"]] as $def) $eventStubLines[] = $def;
        $eventStubLines[] = '';
    }
    $declarationLines[] = '';
    $declarationLines[] = '';
    
    // stop working early if we can
    if ($class["id"] == 0) continue; // skip the "protocol" class, which has no separate support files

    // build @see lines for Doxygen comments
    foreach ($class["commands"] as $command) {
        $doxygenSeeContent .= ' * @see KGAPI command: kg_cmd_'.$class["name"].'_'.$command["name"]."()\n";
    }

    // build API command support header files
    $templateArduino = file_get_contents("template.arduino.protocol.support.h");
    $lines = explode("\n", $templateArduino);
    $lines2 = array();
    foreach ($lines as $line) {
        $count = preg_match_all('/(.*?)\{%([a-zA-Z0-9_]+)%\}/', $line, $matches);
        for ($i = 0; $i < $count; $i++) {
            $indent = 0;
            if (trim($matches[1][$i]) == "") $indent = strlen($matches[1][$i]);
            $replacement = false;
            switch ($matches[2][$i]) {
                case "changelog":
                    $replacement = join("\n".str_repeat(' ', $indent), $arduinoChangelog);
                    break;
                case "date_ymd":
                    $replacement = $now -> format("Y-m-d");
                    break;
                case "date_year":
                    $replacement = $now -> format("Y");
                    break;                                                         
                case "header_constant":
                    $replacement = '_SUPPORT_PROTOCOL_'.strtoupper($class["name"]).'_H_';
                    break;                                                         
                case "packet_class":
                    $replacement = $class["name"];
                    break;                                                         
                case "packet_cases":
                    $replacement = join("\n".str_repeat(' ', $indent), $arduinoCases[$class["id"]]);
                    break;
                case "command_macros":
                    $replacement = join("\n".str_repeat(' ', $indent), $arduinoCommandMacros[$class["id"]]);
                    break;
                case "event_macros":
                    $replacement = join("\n".str_repeat(' ', $indent), $arduinoEventMacros[$class["id"]]);
                    break;
                case "command_function_declarations":
                    $replacement = join("\n".str_repeat(' ', $indent), $arduinoCommandDeclarations[$class["id"]]);
                    break;
                case "event_callback_declarations":
                    $replacement = join("\n".str_repeat(' ', $indent), $arduinoEventDeclarations[$class["id"]]);
                    break;
                case "extern_event_callback_declarations":
                    $replacement = str_replace('*/ uint8_t', '*/ extern uint8_t', join("\n".str_repeat(' ', $indent), $arduinoEventDeclarations[$class["id"]]));
                    break;
                case "doxygen_process":
                    $replacement = '/**
 * @brief Command processing routine for "'.$class["name"].'" packet class
 * @param[in] rxPacket Incoming KGAPI packet buffer
 * @return Protocol error, if any (0 for success)
 * @see protocol_parse()
'.$doxygenSeeContent.' */';
            }
            if ($replacement !== false) $line = str_replace('{%'.$matches[2][$i].'%}', $replacement, $line);
        }
        $lines2[] = $line;
    }
    $templateArduino = join("\n", $lines2);
    echo "--> Writing '../../controller/arduino/keyglove/support_protocol_".$class["name"].".h'\n";
    file_put_contents('../../controller/arduino/keyglove/support_protocol_'.$class["name"].'.h', $templateArduino);

    // build API command support implementation files
    if ($class["id"] == 0) continue; // skip the "protocol" class, which has no separate support files
    $templateArduino = file_get_contents("template.arduino.protocol.support.cpp");
    $lines = explode("\n", $templateArduino);
    $lines2 = array();
    foreach ($lines as $line) {
        $count = preg_match_all('/(.*?)\{%([a-zA-Z0-9_]+)%\}/', $line, $matches);
        for ($i = 0; $i < $count; $i++) {
            $indent = 0;
            if (trim($matches[1][$i]) == "") $indent = strlen($matches[1][$i]);
            $replacement = false;
            switch ($matches[2][$i]) {
                case "changelog":
                    $replacement = join("\n".str_repeat(' ', $indent), $arduinoChangelog);
                    break;
                case "date_ymd":
                    $replacement = $now -> format("Y-m-d");
                    break;
                case "date_year":
                    $replacement = $now -> format("Y");
                    break;                                                         
                case "header_constant":
                    $replacement = '_SUPPORT_PROTOCOL_'.strtoupper($class["name"]).'_H_';
                    break;                                                         
                case "packet_class":
                    $replacement = $class["name"];
                    break;                                                         
                case "packet_cases":
                    $replacement = join("\n".str_repeat(' ', $indent), $arduinoCases[$class["id"]]);
                    break;
                case "command_macros":
                    $replacement = join("\n".str_repeat(' ', $indent), $arduinoCommandMacros[$class["id"]]);
                    break;
                case "event_macros":
                    $replacement = join("\n".str_repeat(' ', $indent), $arduinoEventMacros[$class["id"]]);
                    break;
                case "command_function_declarations":
                    $replacement = join("\n".str_repeat(' ', $indent), $arduinoCommandDeclarations[$class["id"]]);
                    break;
                case "event_callback_declarations":
                    $replacement = join("\n".str_repeat(' ', $indent), $arduinoEventDeclarations[$class["id"]]);
                    break;
                case "extern_event_callback_declarations":
                    $replacement = str_replace('*/ uint8_t', '*/ extern uint8_t', join("\n".str_repeat(' ', $indent), $arduinoEventDeclarations[$class["id"]]));
                    break;
                case "doxygen_process":
                    $replacement = '/**
 * @brief Command processing routine for "'.$class["name"].'" packet class
 * @param[in] rxPacket Incoming KGAPI packet buffer
 * @return Protocol error, if any (0 for success)
 * @see protocol_parse()
'.$doxygenSeeContent.' */';
            }
            if ($replacement !== false) $line = str_replace('{%'.$matches[2][$i].'%}', $replacement, $line);
        }
        $lines2[] = $line;
    }
    $templateArduino = join("\n", $lines2);
    echo "--> Writing '../../controller/arduino/keyglove/support_protocol_".$class["name"].".cpp'\n";
    file_put_contents('../../controller/arduino/keyglove/support_protocol_'.$class["name"].'.cpp', $templateArduino);
}

// build Arduino application stub file from template
echo "Building Arduino controller firmware application stub callback file\n";
$eventStubLines = array();
//$eventStubLines[] = '// KGAPI event stub functions '.date("r");
//$eventStubLines[] = '';
foreach ($kgapi["classes"] as $class) {
    if (!empty($arduinoEventImplementations[$class["id"]])) {
        $eventStubLines[] = '//////////////////////////////// '.strtoupper($class["name"]).' ////////////////////////////////';
        $eventStubLines[] = '';
        foreach ($arduinoEventImplementations[$class["id"]] as $def) $eventStubLines[] = $def;
        $eventStubLines[] = '';
    }
}
$templateArduino = file_get_contents("template.arduino.application_stubs.cpp");
$lines = explode("\n", $templateArduino);
$lines2 = array();
foreach ($lines as $line) {
    $count = preg_match_all('/(.*?)\{%([a-zA-Z0-9_]+)%\}/', $line, $matches);
    for ($i = 0; $i < $count; $i++) {
        $indent = 0;
        if (trim($matches[1][$i]) == "") $indent = strlen($matches[1][$i]);
        $replacement = false;
        switch ($matches[2][$i]) {
            case "changelog":
                $replacement = join("\n".str_repeat(' ', $indent), $arduinoChangelog);
                break;
            case "date_ymd":
                $replacement = $now -> format("Y-m-d");
                break;
            case "date_year":
                $replacement = $now -> format("Y");
                break;
            case "event_stubs":
                $replacement = join("\n", $eventStubLines);
                break;
        }
        if ($replacement !== false) $line = str_replace('{%'.$matches[2][$i].'%}', $replacement, $line);
    }
    $lines2[] = $line;
}
$templateArduino = join("\n", $lines2);
echo "--> Writing '../../controller/arduino/keyglove/application_stubs.cpp'\n";
file_put_contents('../../controller/arduino/keyglove/application_stubs.cpp', $templateArduino);

// build Python library from template
echo "Building Python KGLib implementation\n";
$templatePython = file_get_contents("template.python.kglib.py");
$lines = explode("\n", $templatePython);
$lines2 = array();
foreach ($lines as $line) {
    $count = preg_match_all('/(.*?)\{%([a-zA-Z0-9_]+)%\}/', $line, $matches);
    for ($i = 0; $i < $count; $i++) {
        $indent = 0;
        if (trim($matches[1][$i]) == "") $indent = strlen($matches[1][$i]);
        $replacement = false;
        switch ($matches[2][$i]) {
            case "changelog":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonChangelog);
                break;
            case "date_ymd":
                $replacement = $now -> format("Y-m-d");
                break;
            case "date_year":
                $replacement = $now -> format("Y");
                break;
            case "command_definitions":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonCommandDefinitions);
                break;
            case "response_declarations":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonResponseDeclarations);
                break;
            case "event_declarations":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonEventDeclarations);
                break;
            case "response_conditions":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonResponseConditions);
                break;
            case "event_conditions":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonEventConditions);
                break;
            case "friendly_packet_command_conditions":
                // str_replace() here for better code reuse earlier on
                $replacement = str_replace('self.kgapi_rx_payload', 'payload', join("\n".str_repeat(' ', $indent), $pythonFriendlyPacketCommandConditions));
                break;
            case "friendly_packet_response_conditions":
                // str_replace() here for better code reuse earlier on
                $replacement = str_replace('self.kgapi_rx_payload', 'payload', join("\n".str_repeat(' ', $indent), $pythonFriendlyPacketResponseConditions));
                break;
            case "friendly_packet_event_conditions":
                // str_replace() here for better code reuse earlier on
                $replacement = str_replace('self.kgapi_rx_payload', 'payload', join("\n".str_repeat(' ', $indent), $pythonFriendlyPacketEventConditions));
                break;
        }
        if ($replacement !== false) $line = str_replace('{%'.$matches[2][$i].'%}', $replacement, $line);
    }
    $lines2[] = $line;
}
$templatePython = join("\n", $lines2);
echo "--> Writing '../../host/python/kglib.py'\n";
file_put_contents('../../host/python/kglib.py', $templatePython);

/*
// build Python serial demo from template
$templatePython = file_get_contents("template.python.kglib_test_demo_serial.py");
$lines = explode("\n", $templatePython);
$lines2 = array();
foreach ($lines as $line) {
    $count = preg_match_all('/(.*?)\{%([a-zA-Z0-9_]+)%\}/', $line, $matches);
    for ($i = 0; $i < $count; $i++) {
        $indent = 0;
        if (trim($matches[1][$i]) == "") $indent = strlen($matches[1][$i]);
        $replacement = false;
        switch ($matches[2][$i]) {
            case "changelog":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonChangelog);
                break;
            case "date_ymd":
                $replacement = $now -> format("Y-m-d");
                break;
            case "date_year":
                $replacement = $now -> format("Y");
                break;
            case "handler_definitions":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonHandlerDefinitions);
                break;
            case "handler_assignments":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonHandlerAssignments);
                break;
        }
        if ($replacement !== false) $line = str_replace('{%'.$matches[2][$i].'%}', $replacement, $line);
    }
    $lines2[] = $line;
}
$templatePython = join("\n", $lines2);
//file_put_contents('../../host/python/kglib_test_demo_serial.py', $templatePython);

// build Python serial demo from template
$templatePython = file_get_contents("template.python.kglib_test_demo_rawhid_teensy_windows.py");
$lines = explode("\n", $templatePython);
$lines2 = array();
foreach ($lines as $line) {
    $count = preg_match_all('/(.*?)\{%([a-zA-Z0-9_]+)%\}/', $line, $matches);
    for ($i = 0; $i < $count; $i++) {
        $indent = 0;
        if (trim($matches[1][$i]) == "") $indent = strlen($matches[1][$i]);
        $replacement = false;
        switch ($matches[2][$i]) {
            case "changelog":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonChangelog);
                break;
            case "date_ymd":
                $replacement = $now -> format("Y-m-d");
                break;
            case "date_year":
                $replacement = $now -> format("Y");
                break;
            case "handler_definitions":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonHandlerDefinitions);
                break;
            case "handler_assignments":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonHandlerAssignments);
                break;
        }
        if ($replacement !== false) $line = str_replace('{%'.$matches[2][$i].'%}', $replacement, $line);
    }
    $lines2[] = $line;
}
$templatePython = join("\n", $lines2);
//file_put_contents('../../host/python/kglib_test_demo_rawhid_teensy_windows.py', $templatePython);

// build Python GUI demo from template
$templatePython = file_get_contents("template.python.kglib_test_gui.py");
$lines = explode("\n", $templatePython);
$lines2 = array();
foreach ($lines as $line) {
    $count = preg_match_all('/(.*?)\{%([a-zA-Z0-9_]+)%\}/', $line, $matches);
    for ($i = 0; $i < $count; $i++) {
        $indent = 0;
        if (trim($matches[1][$i]) == "") $indent = strlen($matches[1][$i]);
        $replacement = false;
        switch ($matches[2][$i]) {
            case "changelog":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonChangelog);
                break;
            case "date_ymd":
                $replacement = $now -> format("Y-m-d");
                break;
            case "date_year":
                $replacement = $now -> format("Y");
                break;
            case "handler_definitions":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonHandlerDefinitions);
                break;
            case "handler_assignments":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonHandlerAssignments);
                break;
            case "gui_page_definitions":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonGUIPageDefinitions);
                break;
            case "gui_page_assignments":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonGUIPageAssignments);
                break;
            case "gui_page_additions":
                $replacement = join("\n".str_repeat(' ', $indent), $pythonGUIPageAdditions);
                break;
        }
        if ($replacement !== false) $line = str_replace('{%'.$matches[2][$i].'%}', $replacement, $line);
    }
    $lines2[] = $line;
}
$templatePython = join("\n", $lines2);
//file_put_contents('../../host/python/kglib_test_gui.py', $templatePython);
*/

echo "Done!\n";
echo '</pre>';

?>
