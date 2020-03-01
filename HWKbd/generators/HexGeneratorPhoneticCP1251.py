#
# (Almost) Standard Cyrillic Phonetic Keyboard (ЯЖЕРТЫ)
# With a simple CP1251 encoding
#

class Button():
    def __init__(self, strokes=[], modeswitches=[]):
        self.keystrokes = ["", "", "", "", "", "", "", ""]
        self.modeswitches = [0, 1, 2, 3, 4, 5, 6, 7]
        for i, s in enumerate(strokes):
            self.keystrokes[i] = s
            self.modeswitches[i] = modeswitches[i]
            if s.startswith("#CODE="): continue
            if len(s.encode('utf-8'))>1: self.modeswitches[i] += 8
    def makeMode( self, mode, strokeSeqLength):
        addrFormat = lambda c: ["0x{:04x}".format( c)]
        strokeFormat = lambda c: ["0x{:02x}".format( c)]
        k =self.keystrokes[mode]
        isCode = False
        if k.startswith("#ACTION="):
            enc = [1, int(k[8:])]
        elif k.startswith("#CODE="):
            isCode = True;
            enc = [int(k[6:])]
        else:
            enc = k.encode( 'utf-8')
        m = self.modeswitches[mode]
        c = m * 4096 # 16**3
        if m < 8:
            if isCode: c += enc[0] 
            elif len(k) == 1: c += ord( k[0])
            return addrFormat( c), []
        c += strokeSeqLength
        tmp = []
        for ch in enc:
            tmp += strokeFormat( ch)
        tmp += strokeFormat( 0)
        return addrFormat( c), tmp
    def makeArrays( self, strokeSeqLength):
        tmpAddr = []
        tmpSeq = []
        for mode in range(len(self.keystrokes)):
            a, s = self.makeMode( mode, strokeSeqLength+len(tmpSeq))
            tmpAddr += a
            tmpSeq += s
            #print("Addresses: ", tmpAddr)
            #print("Sequence: ",  strokeSeqLength+len(tmpSeq), tmpSeq)
        return tmpAddr, tmpSeq


class Keyboard():
    buttons = []
    def makeArrays(self):
        addressSequence = []
        macroSequence = []
        for k, v in enumerate( self.buttons):
            addr, macro = v.makeArrays( len(macroSequence)) 
            addressSequence += addr
            macroSequence += macro
            if len(macroSequence) >= 4096:
                print("Macro sequence is longer than 4k!")
                break
        return addressSequence, macroSequence

keyb = Keyboard()

keyb.buttons += [Button([ "#CODE=27", "\t", "\t", "#CODE=27", "\t", "\t", "#CODE=27", "#CODE=27"], [0,0,2,0,4,4,6,4])] # 01 Esc Tab
keyb.buttons += [Button([ "~+~", "+", "+", "+","#CODE=253","#CODE=221", "+", "+"], [0,0,0,0,4,4,4,4])] # 02 э
keyb.buttons += [Button([ "~-~", "-", "-", "-","#CODE=254","#CODE=222", "-", "-"], [0,0,0,0,4,4,4,4])] # 03 ю 
keyb.buttons += [Button([ "~*~", "*", "*", "*","#CODE=250","#CODE=218", "*", "*"], [0,0,0,0,4,4,4,4])] # 04 ъ
keyb.buttons += [Button([ "~/~", "/", "/", "/","#CODE=252","#CODE=220", "/", "/"], [0,0,0,0,4,4,4,4])] # 05 ь
keyb.buttons += [Button([ "~^~", "^", "^", "^","#CODE=184","#CODE=168", "^", "^"], [0,0,0,0,4,4,4,4])] # 06 ё
keyb.buttons += [Button([ "~~~","~","\\","#CODE=177","~","\\","#CODE=177","#CODE=177"], [0,0,0,0,4,4,4,4])] # 07
keyb.buttons += [Button([ "e","e","e","e","#CODE=248","#CODE=216","e","e"], [0,0,0,0,4,4,4,4])] # 08 ш
keyb.buttons += [Button([ ".", ",", ":", ";","#CODE=249","#CODE=217", "#CODE=176", ";"], [0,0,0,0,4,4,4,4])] # 09 щ
keyb.buttons += [Button([ "~Run~","~Halt~","~Abort~","~Step~","~Run~","~Halt~","~Abort~","~Step~"], [0,0,0,3,4,4,4,7])] # 10

keyb.buttons += [Button(["1","!","","","1","!","",""], [0,0,0,0,4,4,4,4])] # 11
keyb.buttons += [Button(["2","@","","","2","@","",""], [0,0,0,0,4,4,4,4])] # 12
keyb.buttons += [Button(["3","#","","","3","#","",""], [0,0,0,0,4,4,4,4])] # 13
keyb.buttons += [Button(["4","$","","","4","$","",""], [0,0,0,0,4,4,4,4])] # 14
keyb.buttons += [Button(["5",'%',"","","5","%","",""], [0,0,0,0,4,4,4,4])] # 15
keyb.buttons += [Button(["6","|","[]","{}","6","|","[]","{}"], [0,0,0,0,4,4,4,4])] # 16
keyb.buttons += [Button(["7","&", "<>","</>","7","&", "<>","</>"], [0,0,0,0,4,4,4,4])] # 17
keyb.buttons += [Button(["8",'""',"''","'","8",'""',"''","'"], [0,0,0,0,4,4,4,4])] # 18
keyb.buttons += [Button(["9","()","[]","{}","9","()","[]","{}"], [0,0,0,0,4,4,4,4])] # 19
keyb.buttons += [Button(["0","=","==","!=","0","=","==","!="], [0,0,0,0,4,4,4,4])] # 20

keyb.buttons += [Button(["q","Q","","","#CODE=255","#CODE=223","",""], [0,0,0,0,4,4,4,4])] # 21 q я
keyb.buttons += [Button(["w","W","","","#CODE=230","#CODE=198","",""], [0,0,0,0,4,4,4,4])] # 22 w ж
keyb.buttons += [Button(["e","E","","","#CODE=229","#CODE=197","",""], [0,0,0,0,4,4,4,4])] # 23 e е
keyb.buttons += [Button(["r","R","","","#CODE=240","#CODE=208","",""], [0,0,0,0,4,4,4,4])] # 24 r р
keyb.buttons += [Button(["t","T","","","#CODE=242","#CODE=210","",""], [0,0,0,0,4,4,4,4])] # 25 t т
keyb.buttons += [Button(["y","Y","","","#CODE=251","#CODE=219","",""], [0,0,0,0,4,4,4,4])] # 26 y ы
keyb.buttons += [Button(["u","U","","","#CODE=243","#CODE=211","",""], [0,0,0,0,4,4,4,4])] # 27 u у
keyb.buttons += [Button(["i","I","","","#CODE=232","#CODE=200","",""], [0,0,0,0,4,4,4,4])] # 28 i и
keyb.buttons += [Button(["o","O","","","#CODE=238","#CODE=206","",""], [0,0,0,0,4,4,4,4])] # 29 o о
keyb.buttons += [Button(["p","P","","","#CODE=239","#CODE=207","",""], [0,0,0,0,4,4,4,4])] # 30 p п

keyb.buttons += [Button(["a","A","","","#CODE=224","#CODE=192","",""], [0,0,0,0,4,4,4,4])] # 31 a а
keyb.buttons += [Button(["s","S","","","#CODE=241","#CODE=209","",""], [0,0,0,0,4,4,4,4])] # 32 s с
keyb.buttons += [Button(["d","D","","","#CODE=228","#CODE=196","",""], [0,0,0,0,4,4,4,4])] # 33 d д
keyb.buttons += [Button(["f","F","","","#CODE=244","#CODE=212","",""], [0,0,0,0,4,4,4,4])] # 34 f ф
keyb.buttons += [Button(["g","G","","","#CODE=227","#CODE=195","",""], [0,0,0,0,4,4,4,4])] # 35 g г
keyb.buttons += [Button(["h","H","","","#CODE=247","#CODE=215","",""], [0,0,0,0,4,4,4,4])] # 36 h ч
keyb.buttons += [Button(["j","J","","","#CODE=233","#CODE=201","",""], [0,0,0,0,4,4,4,4])] # 37 j й
keyb.buttons += [Button(["k","K","","","#CODE=234","#CODE=202","",""], [0,0,0,0,4,4,4,4])] # 38 k к
keyb.buttons += [Button(["l","L","","","#CODE=235","#CODE=203","",""], [0,0,0,0,4,4,4,4])] # 39 l л
keyb.buttons += [Button(["#CODE=8","#CODE=127","#CODE=8","#CODE=127","#CODE=8","#CODE=127","#CODE=8","#CODE=127"], [0,0,0,0,4,4,4,4])] # 40

keyb.buttons += [Button(["z","Z","","","#CODE=231","#CODE=199","",""], [0,0,0,0,4,4,4,4])] # 41 z з
keyb.buttons += [Button(["x","X","","","#CODE=245","#CODE=213","",""], [0,0,0,0,4,4,4,4])] # 42 x х
keyb.buttons += [Button(["c","C","","","#CODE=246","#CODE=214","",""], [0,0,0,0,4,4,4,4])] # 43 c ц
keyb.buttons += [Button(["v","V","","","#CODE=226","#CODE=194","",""], [0,0,0,0,4,4,4,4])] # 44 v в 
keyb.buttons += [Button(["b","B","","","#CODE=225","#CODE=193","",""], [0,0,0,0,4,4,4,4])] # 45 b б
keyb.buttons += [Button(["n","N","","","#CODE=237","#CODE=205","",""], [0,0,0,0,4,4,4,4])] # 46 n н
keyb.buttons += [Button(["m","M","","","#CODE=236","#CODE=204","",""], [0,0,0,0,4,4,4,4])] # 47 m м
keyb.buttons += [Button([", ","! ","<",":\n\t",", ","! ","<",":\n\t"], [0,1,0,0,4,5,4,4])] # 48
keyb.buttons += [Button([". ","? ",">",";\n",". ","? ",">",".\n"], [1,1,0,0,5,5,4,5])] # 49
keyb.buttons += [Button([" ","_","\t",""," ","_","\t",""], [0,0,0,0,4,4,4,4])] # 50 SPACE

keyb.buttons += [Button(["","","","","","","",""], [4,5,6,7,0,1,2,3])] # 51 Rus
keyb.buttons += [Button(["","","","","","","",""], [2,2,0,2,6,6,4,6])] # 52 F1
keyb.buttons += [Button(["","","","","","","",""], [3,3,3,0,7,7,7,0])] # 53 F2
keyb.buttons += [Button(["#CODE=17","S-left","#CODE=21","S-home","#CODE=17","S-left","#CODE=21","S-home"], [0,1,0,1,4,5,4,5])] # 54 left
keyb.buttons += [Button(["#CODE=20","S-down","#CODE=24","S-pgdown","#CODE=20","S-down","#CODE=24","S-pgdown"], [0,1,0,3,4,5,4,7])] # 55 down
keyb.buttons += [Button(["#CODE=19","S-up","#CODE=23","S-pgup","#CODE=19","S-up","#CODE=23","S-pgup"], [0,1,0,3,4,5,4,7])] # 56 up
keyb.buttons += [Button(["#CODE=18","S-right","#CODE=22","S-end","#CODE=18","S-right","#CODE=22","S-end"], [0,1,0,3,4,5,4,7])] # 57 right
keyb.buttons += [Button(["#CODE=13","#CODE=13","SC-end","SC-home","#CODE=13","#CODE=13","SC-end","SC-home",], [0,0,0,0,4,4,4,4])] # 58 ENTER
keyb.buttons += [Button(["","","","","","","",""], [1,1,1,1,5,5,5,5])] # 59 SHIFT

addressSequence, macroSequence = keyb.makeArrays()

print("const static uint16_t HWKBD_Codepage_Cyrillic_Phonetic_CP1251[] PROGMEM = {")
for i in range( 0, len( addressSequence), 8):
    s = ""
    for seq in addressSequence[i:i+8]: s += "{:s},".format(seq)
    if i//8 == 58: s = s[:-1] + " "
    s += " // Button {:02d}".format(  i // 8+1) 
    print(s)
print("};")
print()
print("const static byte HWKBD_Macropage_Cyrillic_Phonetic_CP1251[] PROGMEM = {")
for i in range( 0, len( macroSequence), 16):
    s = ""
    for j in range( i, i+16):
        if j==len( macroSequence)-1:
            s += "{:s}".format(macroSequence[j])
            break;
        s += "{:s},".format(macroSequence[j])
    print(s)
print("};")
print()
