def convertComment( s):
    
    last_symb = '0,'
    if s[4] == 'O': last_symb = '8,'
    c = s[:4]
    if c == '....': return "0x0" + last_symb 
    if c == '...O': return "0x1" + last_symb 
    if c == '..O.': return "0x2" + last_symb
    if c == '..OO': return "0x3" + last_symb
    if c == '.O..': return "0x4" + last_symb
    if c == '.O.O': return "0x5" + last_symb
    if c == '.OO.': return "0x6" + last_symb
    if c == '.OOO': return "0x7" + last_symb
    if c == 'O...': return "0x8" + last_symb
    if c == 'O..O': return "0x9" + last_symb
    if c == 'O.O.': return "0xA" + last_symb
    if c == 'O.OO': return "0xB" + last_symb
    if c == 'OO..': return "0xC" + last_symb
    if c == 'OO.O': return "0xD" + last_symb
    if c == 'OOO.': return "0xE" + last_symb 
    if c == 'OOOO': return "0xF" + last_symb
    return "Error!" 

def convertLine( s):
    last_symb = '0, '
    if s[4] == 'O': last_symb = '8, '
    c = s[:4]
    if c == '....': return "0x0" + last_symb 
    if c == '...O': return "0x1" + last_symb 
    if c == '..O.': return "0x2" + last_symb
    if c == '..OO': return "0x3" + last_symb
    if c == '.O..': return "0x4" + last_symb
    if c == '.O.O': return "0x5" + last_symb
    if c == '.OO.': return "0x6" + last_symb
    if c == '.OOO': return "0x7" + last_symb
    if c == 'O...': return "0x8" + last_symb
    if c == 'O..O': return "0x9" + last_symb
    if c == 'O.O.': return "0xA" + last_symb
    if c == 'O.OO': return "0xB" + last_symb
    if c == 'OO..': return "0xC" + last_symb
    if c == 'OO.O': return "0xD" + last_symb
    if c == 'OOO.': return "0xE" + last_symb 
    if c == 'OOOO': return "0xF" + last_symb
    return "Error!" 

f1 = open("Font_Data_CP1251.txt")
f2 = open("CP1251_font.h", "w")
f2.write("const uint8_t CP1251_font[] PROGMEM = {\n")

current_symbol = ""
current_line = ""
for i in range(10000):
    s = f1.readline()
    if len(s) <= 0: break
    s = s.strip('\n').strip('\r')
    if s.startswith("#"):
        if i>0:
            f2.write("  {:s}{:s}\n".format(current_line, current_symbol))
        current_symbol = s.replace("#", "//")
        current_line = ""
        continue
    current_line += convertLine( s)

current_line = current_line[:-2] + "  "
f2.write("  {:s}{:s}\n".format(current_line, current_symbol))
    
f1.close()
f2.write("};\n")
f2.close()
