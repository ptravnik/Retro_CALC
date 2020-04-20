#
# (Almost) Standard Cyrillic Phonetic Keyboard (ЯЖЕРТЫ)
# With a simple CP1251 encoding
#
# The key codes are composed of 16-bit integers, 8 numbers per button
# (by the number of modes). The bit pattern is as following:
# 0MMM0000 cccccccc
# 1MMMaaaa aaaaaaaa
# MMM - the next mode (from 0 to 7)
# cccccccc - a single-byte Unicode character (if >128, it is a control character, such as an arrow)
# aaaaa aaaaaaaaa - a 12-bit shift in the macros table;
# the latter is composed of null-terminated strings, up to 4k in length.
#
# This generator is for the pre-production version of the keyboard (v 1.0)
#
# Strokes are called in the form of ("Command", next_mode)
# Command can be composed of a single unicode character
# or a string using #CODE=xxx substitutes
#
class Button():
    def __init__(self, scan, strokes, encoding='utf-8'):
        self.scan = scan
        self.encoding = encoding
        self.keystrokes = [0, 0, 0, 0, 0, 0, 0, 0]
        self.strokes = strokes
        self.macros = [None,None,None,None,None,None,None,None]
        self.modeswitches = [0, 1, 2, 3, 4, 5, 6, 7]
        for i, stroke in enumerate(self.strokes):
            j, self.keystrokes[i], self.macros[i] = self.parseCode( stroke[0])
            self.modeswitches[i] = stroke[1] + j*8
            if self.macros[i] is not None: self.macros[i] += [0] 
        return

    def escapeCodes(self, s):
       return s.replace("\n", "\\n").replace("\r", "\\r").replace("\t", "\\t") 

    def parseCode( self, s, recursive=False):
        if len(s) < 1:
            return 0, 0, None
        if len(s) == 1:
            enc = s[0].encode( self.encoding);
            if len(enc) == 1:
                if recursive: return 1, None, [int(enc[0])]
                else: return 0, int(enc[0]), None
            tmp = []
            for i in range(len(enc)): tmp += [int(enc[i])]
            return 1, None, tmp
        if s.startswith("#CODE="):
            code = int(s[6:9])
            reminder = s[9:]
            if len(reminder) == 0:
                if recursive: return 1, None, [code]
                else: return 0, code, None
            _, _, further_encoding = self.parseCode( reminder, True) 
            return 1, None, [code] + further_encoding
        _, _, code = self.parseCode( s[0:1], True)
        _, _, further_encoding = self.parseCode( s[1:], True) 
        return 1, None, code + further_encoding

    def report(self, f=None):
        names = ["Unmodified      :",
                 "Shifted         :",
                 "F1              :",
                 "F2              :",
                 "Unmodified (Rus):",
                 "Shifted    (Rus):",
                 "F1         (Rus):",
                 "F2         (Rus):"]
        print("Button scan = {:03d}:".format(self.scan))
        if f is not None:
            f.write("Button scan = {:03d}:\n".format(self.scan))
        for i, stroke in enumerate(self.strokes):
            if self.modeswitches[i] < 8:
                print("   {:s} 0x{:04x} [{:s}]".format(
                    names[i],
                    self.keystrokes[i],
                    self.escapeCodes( stroke[0])))
                if f is not None:
                    f.write("   {:s} 0x{:04x} [{:s}]\n".format(
                    names[i],
                    self.keystrokes[i],
                    self.escapeCodes( stroke[0])))
            else:
                sout = "{"
                for c in self.macros[i]:
                    sout += "0x{:02x},".format(c)
                sout = sout[:-1] + "}"
                print("   {:s} 0x{:04x} {:s} [{:s}]".format(
                    names[i],
                    self.keystrokes[i],
                    sout,
                    self.escapeCodes( stroke[0])))
                if f is not None:
                    f.write("   {:s} 0x{:04x} {:s} [{:s}]".format(
                    names[i],
                    self.keystrokes[i],
                    sout,
                    self.escapeCodes( stroke[0])))
        return

#
# Main class for keyboard creation
#
class Keyboard():
    def __init__(self, encoding='utf-8'):
        self.scan = 1
        self.buttons = []
        self.enc = encoding
        self.macroTable = []
        return
    def addButton(self, strokes, encoding=None):
        if encoding is None:
            b = Button( self.scan, strokes, self.enc)
        else:
            b = Button( self.scan, strokes, encoding)
        self.scan += 1
        for i, macro in enumerate( b.macros):
            if macro is None:
                b.keystrokes[i] += b.modeswitches[i] * 4096
                continue
            la = len(self.macroTable)
            if la >= 4096:
                print("Macro sequence is longer than 4k!")
                break
            b.keystrokes[i] = b.modeswitches[i] * 4096 + la
            self.macroTable += macro
        b.report()
        self.buttons += [ b]
        return
    def addButtonNoSwitch(self, keys, encoding=None):
        strokes = []
        for i, k in enumerate(keys):
            if i<4: strokes += [(k,0)]
            else: strokes += [(k,4)]
        self.addButton( strokes, encoding)
        return
    def addButtonSimple(self, keys, encoding=None):        
        strokes = []
        for i in range(len( keys)):
            s = ""
            if keys[i] != ' ': s += keys[i]
            if i<4: strokes += [(s,0)]
            else: strokes += [(s,4)]
        self.addButton( strokes, encoding)
        return
    def generateHFile(self, name, f):
        f.write("const static uint16_t {:s}[] PROGMEM = {{\n".format(name))
        for button in self.buttons:
            s = " "
            for i in range(8):
                s += " 0x{:04x},".format(button.keystrokes[i])            
            if button.scan == 59:
                s = s[:-1] + " "
            s += " // Button {:02d}\n".format( button.scan)
            f.write( s)
        f.write("};\n\n")
        f.write("const static uint8_t {:s}[] PROGMEM = {{\n".format(name.replace("Codepage", "Macropage")))
        for i in range( 0, len( self.macroTable), 8):
            s = " "
            for j in range( i, i+8):
                if j==len( self.macroTable)-1:
                    s += " 0x{:02x}".format(self.macroTable[j])
                    break;
                s += " 0x{:02x},".format(self.macroTable[j])
            f.write("{:s}\n".format(s))
        f.write("};\n")
        return

SHIFT = 1
F1 =    2
F2 =    3
RUS =   4

kbd = Keyboard(encoding='cp1251')

kbd.addButton([("#CODE=027", 0), ("#CODE=027",0), ("\t",2), ("\t",3),
               ("#CODE=027", 4), ("#CODE=027",4), ("\t",6), ("\t",7)]) # 01 ESC
kbd.addButtonNoSwitch(["#CODE=031+","+","+","+",
                       "э","Э","+","+"])                              # 02
kbd.addButtonNoSwitch(["#CODE=031-","-","-","-",
                       "ю","Ю","-","-"])                              # 03
kbd.addButtonNoSwitch(["#CODE=031*","*","*","*",
                       "ъ","Ъ","*","*"])                              # 04
kbd.addButtonNoSwitch(["#CODE=031/","/","/","/",
                       "ь","Ь","/","/"])                              # 05
kbd.addButtonNoSwitch(["#CODE=031^","^","^","^",
                       "ё","Ё","^","^"])                              # 06
kbd.addButtonNoSwitch(["#CODE=031#CODE=177","~","\\","#CODE=177",
                       "~","\\","#CODE=177","#CODE=177"])             # 07
kbd.addButtonSimple("eeeeшШee")                                       # 08
kbd.addButtonNoSwitch([".",",",":",";",
                       "щ","Щ","#CODE=176",";"])                      # 09
kbd.addButton([("#CODE=031run#CODE=031", 0), ("#CODE=031halt#CODE=031",0),
               ("#CODE=031abort#CODE=031",0), ("#CODE=031step#CODE=031",3),
               ("#CODE=031run#CODE=031", 4), ("#CODE=031halt#CODE=031",4),
               ("#CODE=031abort#CODE=031",4), ("#CODE=031step#CODE=031",7)])            # 10 RUN

##keyb.buttons += [Button([ "~Run~","~Halt~","~Abort~","~Step~","~Run~","~Halt~","~Abort~","~Step~"], [0,0,0,3,4,4,4,7])] # 10

kbd.addButtonSimple("1!  1!  ") # 11
kbd.addButtonSimple("2@  2@  ") # 12
kbd.addButtonSimple("3#  3#  ") # 13
kbd.addButtonSimple("4$  4$  ") # 14
kbd.addButtonSimple("5%  5%  ") # 15
kbd.addButtonNoSwitch(["6","|","[]#CODE=017","{}#CODE=017",
                       "6","|","[]#CODE=017","{}#CODE=017"])            # 16
kbd.addButtonNoSwitch(["7","&","<>#CODE=017","</>#CODE=017#CODE=017",
                       "7","&","<>#CODE=017","</>#CODE=017#CODE=017"])  # 17
kbd.addButtonNoSwitch(["8",'""#CODE=017',"''#CODE=017","'",
                       "8",'""#CODE=017',"''#CODE=017","'"])            # 18
kbd.addButtonNoSwitch(["9",'()#CODE=017',"[]#CODE=017","{}#CODE=017",
                       "9",'()#CODE=017',"[]#CODE=017","{}#CODE=017"])  # 19
kbd.addButtonNoSwitch(["0",'=',"==","!=",
                       "0",'+',"==","!="])                              # 20

kbd.addButtonNoSwitch(["q","Q","#CODE=010quad#CODE=013","#CODE=010goff2#CODE=013",
                       "я","Я","#CODE=010quad#CODE=013","#CODE=010goff2#CODE=013"]) # 21 (quad/goff2)
kbd.addButtonSimple("wW  жЖ  ") # 22
kbd.addButtonSimple("eE  еЕ  ") # 23
kbd.addButtonNoSwitch(["r","R","#CODE=010radius#CODE=013","#CODE=010cath#CODE=013",
                       "р","Р","#CODE=010radius#CODE=013","#CODE=010cath#CODE=013"]) # 24 (radius/cath)
kbd.addButtonSimple("tT  тТ  ") # 25
kbd.addButtonSimple("yY  ыЫ  ") # 26
kbd.addButtonSimple("uU  уУ  ") # 27
kbd.addButtonSimple("iI  иИ  ") # 28
kbd.addButtonSimple("oO  оО  ") # 29
kbd.addButtonSimple("pP  пП  ") # 30

kbd.addButtonSimple("aA  аА  ") # 31
kbd.addButtonSimple("sS  сс  ") # 32
kbd.addButtonSimple("dD  дД  ") # 33
kbd.addButtonSimple("fF  фФ  ") # 34
kbd.addButtonSimple("gG  гГ  ") # 35
kbd.addButtonSimple("hH  чЧ  ") # 36
kbd.addButtonSimple("jJ  йй  ") # 37
kbd.addButtonSimple("kK  кК  ") # 38
kbd.addButtonSimple("lL  лЛ  ") # 39
kbd.addButtonNoSwitch(["#CODE=008","#CODE=127","#CODE=008","#CODE=127",
                       "#CODE=008","#CODE=127","#CODE=008","#CODE=127"]) # 40

#kbd.addButtonSimple("zZ  зЗ  ") # 41
kbd.addButtonNoSwitch(["z","Z","#CODE=010sin#CODE=013","#CODE=010asin#CODE=013",
                       "з","З","#CODE=010sin#CODE=013","#CODE=010asin#CODE=013"]) # 41 (sine)
#kbd.addButtonSimple("xX  хХ  ") # 42
kbd.addButtonNoSwitch(["x","X","#CODE=010cos#CODE=013","#CODE=010acos#CODE=013",
                       "х","Х","#CODE=010cos#CODE=013","#CODE=010acos#CODE=013"]) # 42 (cosine)
#kbd.addButtonSimple("cC  цЦ  ") # 43
kbd.addButtonNoSwitch(["c","C","#CODE=010tan#CODE=013","#CODE=010atan#CODE=013",
                       "ц","Ц","#CODE=010tan#CODE=013","#CODE=010atan#CODE=013"]) # 42 (tangent)
kbd.addButtonSimple("vV  вВ  ") # 44
kbd.addButtonSimple("bB  бБ  ") # 45
kbd.addButtonSimple("nN  нН  ") # 46
kbd.addButtonSimple("mM  мМ  ") # 47
kbd.addButton([(", ", 0), ("! ",1), ("<",0), (":\n\t",0),
               (", ", 4), ("! ",5), ("<",4), (":\n\t",4)]) # 48 COMMA
kbd.addButton([(". ", 1), ("? ",1), (">",0), (";\n", 0),
               (". ", 5), ("? ",5), (">",4), (".\n", 5)])   # 49 PERIOD
kbd.addButtonNoSwitch([" ","_","\t","",
                       " ","_","\t",""]) # 50 SPACE

kbd.addButton([("", 4), ("",5), ("",6), ("",7),
               ("", 0), ("",1), ("",2), ("",3)])   # 51 Rus/Lat
kbd.addButton([("", 2), ("",2), ("",0), ("",2),
               ("", 6), ("",6), ("",4), ("",6)])   # 52 F1
kbd.addButton([("", 3), ("",3), ("",3), ("",0),
               ("", 7), ("",7), ("",7), ("",0)])   # 53 F2
kbd.addButton([("#CODE=017", 0), ("S-left",1), ("#CODE=021",0), ("S-home",1),
               ("#CODE=017", 4), ("S-left",5), ("#CODE=021",4), ("S-home",5)])   # 54 arrow left
kbd.addButton([("#CODE=020", 0), ("S-down",1), ("#CODE=024",0), ("S-pgdn",3),
               ("#CODE=020", 4), ("S-down",5), ("#CODE=024",4), ("S-pgdn",7)])   # 55 arrow down
kbd.addButton([("#CODE=019", 0), ("S-up",1), ("#CODE=023",0), ("S-pgup",3),
               ("#CODE=019", 4), ("S-up",5), ("#CODE=023",4), ("S-pgup",7)])     # 56 arrow up
kbd.addButton([("#CODE=018", 0), ("S-right",1), ("#CODE=022",0), ("#CODE=010inj#CODE=013",0),
               ("#CODE=018", 4), ("S-right",5), ("#CODE=022",4), ("#CODE=010inj#CODE=013",4)])   # 57 arrow right
kbd.addButtonNoSwitch(["#CODE=013","#CODE=013","SC-end","SC-home",
                       "#CODE=013","#CODE=013","SC-end","SC-home"])              # 58 ENTER
kbd.addButton([("", 1), ("",0), ("",2), ("",3),
               ("", 5), ("",4), ("",6), ("",7)])   # 59 SHIFT


f = open("Cyrillic_Phonetic_CP1251_Mockup.h", "w")
f.write("/*\n")
f.write("  Cyrillic_Phonetic_CP1251_Mockup.h - codes and macros for Hardware keyboard.\n")
f.write("  This is applicable to the pre-production mockup version;\n")
f.write("  For the production layout, use Cyrillic_Phonetic_CP1251.h\n")
f.write("\n")
f.write("  Copyright (c) 2019 Pavel Travnik.  All rights reserved.\n")
f.write("\n")
f.write("  This library is free software; you can redistribute it and/or\n")
f.write("  modify it under the terms of the GNU Lesser General Public\n")
f.write("  License as published by the Free Software Foundation; either\n")
f.write("  version 2.1 of the License, or (at your option) any later version.\n")
f.write("\n")
f.write("  This library is distributed in the hope that it will be useful,\n")
f.write("  but WITHOUT ANY WARRANTY; without even the implied warranty of\n")
f.write("  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n")
f.write("  Lesser General Public License for more details.\n")
f.write("\n")
f.write("  You should have received a copy of the GNU Lesser General Public\n")
f.write("  License along with this library; if not, write to the Free Software\n")
f.write("  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA\n")
f.write("*/\n")

kbd.generateHFile("HWKBD_Codepage_Cyrillic_Phonetic_CP1251", f)
f.close()
