#
# (Almost) Standard Cyrillic Keyboard (ЙЦУКЕН)
#

class Button():
    def __init__(self, strokes=[], modeswitches=[]):
        self.keystrokes = ["", "", "", "", "", "", "", ""]
        self.modeswitches = [0, 1, 2, 3, 4, 5, 6, 7]
        for i, s in enumerate(strokes):
            self.keystrokes[i] = s
            self.modeswitches[i] = modeswitches[i]
            if len(s.encode('utf-8'))>1: self.modeswitches[i] += 8
    def makeMode( self, mode, strokeSeqLength):
        addrFormat = lambda c: ["0x{:04x}".format( c)]
        strokeFormat = lambda c: ["0x{:02x}".format( c)]
        k =self.keystrokes[mode]
        if k.startswith("#ACTION="):
            enc = [1, int(k[8:])]
        else:
            enc = k.encode( 'utf-8')
        m = self.modeswitches[mode]
        c = m * 4096 # 16**3
        if m < 8:
            if len(k) == 1: c += ord( k[0])
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
            if len(macroSequence) > 4096:
                print("Macro sequence is longer than 4k!")
                break
        return addressSequence, macroSequence

keyb = Keyboard()
keyb.buttons += [Button([ "", "","","","","","",""], [0,0,0,0,4,4,4,4])] # 01 esc
keyb.buttons += [Button([ "", "","","","","","",""], [0,0,0,0,4,4,4,4])] # 02 del
keyb.buttons += [Button([ "", "","","","","","",""], [0,0,0,0,4,4,4,4])] # 03 undo
keyb.buttons += [Button([ "", "","","","","","",""], [0,0,0,0,4,4,4,4])] # 04 cut
keyb.buttons += [Button([ "", "","","","","","",""], [0,0,0,0,4,4,4,4])] # 05 copy
keyb.buttons += [Button([ "", "","","","","","",""], [0,0,0,0,4,4,4,4])] # 06 paste
keyb.buttons += [Button(["+","&","","","","","",""], [0,0,0,0,4,4,4,4])] # 07 + &
keyb.buttons += [Button(["-","|","","","","","",""], [0,0,0,0,4,4,4,4])] # 08 - |
keyb.buttons += [Button(["*","^","","","","","",""], [0,0,0,0,4,4,4,4])] # 09 * ^
keyb.buttons += [Button(["/","%","","","","","",""], [0,0,0,0,4,4,4,4])] # 10 / %

keyb.buttons += [Button(["1","`",   "",  "","ё","Ё",  "",  ""], [0,0,0,0,4,4,4,4])] # 11 1
keyb.buttons += [Button(["2","@",   "",  "","ъ","Ъ",  "",  ""], [0,0,0,0,4,4,4,4])] # 12 2
keyb.buttons += [Button(["3","~",   "",  "","ь","Ь",  "",  ""], [0,0,0,0,4,4,4,4])] # 13 3
keyb.buttons += [Button(["4","''",  "",  "","э","Э",  "",  ""], [0,0,0,0,4,4,4,4])] # 14 4
keyb.buttons += [Button(["5",'""',  "",  "","ю","Ю",  "",  ""], [0,0,0,0,4,4,4,4])] # 15 5
keyb.buttons += [Button(["6","()","[]","{}","х","Х",  "",  ""], [0,0,0,0,4,4,4,4])] # 16 6
keyb.buttons += [Button(["7","<", "<>",  "","ж","Ж",  "",  ""], [0,0,0,0,4,4,4,4])] # 17 7
keyb.buttons += [Button(["8",">","</>",  "", "", "",  "",  ""], [0,0,0,0,4,4,4,4])] # 18 8
keyb.buttons += [Button(["9","!",   "",  "", "", "",  "",  ""], [0,0,0,0,4,4,4,4])] # 19 9
keyb.buttons += [Button(["0","=", "==","!=","0","=","==","!="], [0,0,0,0,4,4,4,4])] # 20 0

keyb.buttons += [Button(["q","Q","","","й","Й","",""], [0,0,0,0,4,4,4,4])] # 21 q
keyb.buttons += [Button(["w","W","","","ц","Ц","",""], [0,0,0,0,4,4,4,4])] # 22 w
keyb.buttons += [Button(["e","E","","","у","У","",""], [0,0,0,0,4,4,4,4])] # 23 e
keyb.buttons += [Button(["r","R","","","к","К","",""], [0,0,0,0,4,4,4,4])] # 24 r
keyb.buttons += [Button(["t","T","","","е","Е","",""], [0,0,0,0,4,4,4,4])] # 25 t
keyb.buttons += [Button(["y","Y","","","н","Н","",""], [0,0,0,0,4,4,4,4])] # 26 y
keyb.buttons += [Button(["u","U","","","г","Г","",""], [0,0,0,0,4,4,4,4])] # 27 u
keyb.buttons += [Button(["i","i","","","ш","Ш","",""], [0,0,0,0,4,4,4,4])] # 28 i
keyb.buttons += [Button(["o","o","","","щ","Щ","",""], [0,0,0,0,4,4,4,4])] # 29 o
keyb.buttons += [Button(["p","P","","","з","З","",""], [0,0,0,0,4,4,4,4])] # 30 p

keyb.buttons += [Button(["a","A","","","ф","Ф","",""], [0,0,0,0,4,4,4,4])] # 31 a
keyb.buttons += [Button(["s","S","","","ы","Ы","",""], [0,0,0,0,4,4,4,4])] # 32 s
keyb.buttons += [Button(["d","D","","","в","В","",""], [0,0,0,0,4,4,4,4])] # 33 d
keyb.buttons += [Button(["f","F","","","а","А","",""], [0,0,0,0,4,4,4,4])] # 34 f
keyb.buttons += [Button(["g","G","","","п","П","",""], [0,0,0,0,4,4,4,4])] # 35 g
keyb.buttons += [Button(["h","H","","","р","Р","",""], [0,0,0,0,4,4,4,4])] # 36 h
keyb.buttons += [Button(["j","J","","","о","О","",""], [0,0,0,0,4,4,4,4])] # 37 j
keyb.buttons += [Button(["k","K","","","л","Л","",""], [0,0,0,0,4,4,4,4])] # 38 k
keyb.buttons += [Button(["l","L","","","д","Д","",""], [0,0,0,0,4,4,4,4])] # 39 l
keyb.buttons += [Button([", ","; ",";\n","",", ","; ",";\n",""], [0,0,0,0,4,4,4,4])] # 40 , ;

keyb.buttons += [Button(["z","Z","","","я","Я","",""], [0,0,0,0,4,4,4,4])] # 41 z
keyb.buttons += [Button(["x","X","","","ч","Ч","",""], [0,0,0,0,4,4,4,4])] # 42 x
keyb.buttons += [Button(["c","C","","","с","С","",""], [0,0,0,0,4,4,4,4])] # 43 c
keyb.buttons += [Button(["v","V","","","м","М","",""], [0,0,0,0,4,4,4,4])] # 44 v
keyb.buttons += [Button([" ","_","",""," ","_","",""], [0,0,0,0,4,4,4,4])] # 45 SPACE
keyb.buttons += [Button(["b","B","","","и","И","",""], [0,0,0,0,4,4,4,4])] # 46 b
keyb.buttons += [Button(["n","N","","","т","Т","",""], [0,0,0,0,4,4,4,4])] # 47 n
keyb.buttons += [Button(["m","M","","","б","Б","",""], [0,0,0,0,4,4,4,4])] # 48 m
keyb.buttons += [Button([".","?",". ","? ",". ","? ",".","?"], [0,0,1,1,5,5,4,4])] # 49 . ?
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 50 BACKSP

keyb.buttons += [Button(["","","","","","","",""], [4,5,6,7,0,1,2,3])] # 51 Rus
keyb.buttons += [Button(["","","","","","","",""], [2,2,0,2,6,6,4,6])] # 52 F1
keyb.buttons += [Button(["","","","","","","",""], [3,3,3,0,7,7,7,0])] # 53 F2
keyb.buttons += [Button(["left","S-left","home","S-home","left","S-left","home","S-home"], [0,1,0,1,4,5,4,5])] # 54 left
keyb.buttons += [Button(["down","S-down","pgdown","S-pgdown","down","S-down","pgdown","S-pgdown"], [0,1,2,3,4,5,6,7])] # 55 down
keyb.buttons += [Button(["up","S-up","pgup","S-pgup","up","S-up","pgup","S-pgup"], [0,1,2,3,4,5,6,7])] # 56 up
keyb.buttons += [Button(["right","S-right","end","S-end","right","S-right","end","S-end"], [0,1,2,3,4,5,6,7])] # 57 right
keyb.buttons += [Button(["\n","\n","SC-end","SC-home","\n","\n","SC-end","SC-home",], [0,0,0,0,4,4,4,4])] # 58 ENTER
keyb.buttons += [Button(["","","","","","","",""], [1,1,1,1,5,5,5,5])] # 59 SHIFT

addressSequence, macroSequence = keyb.makeArrays()

print("const static uint16_t HWKBD_Codepage_Cyrillic_UTF8[] PROGMEM = {")
for i in range( 0, len( addressSequence), 8):
    s = ""
    for seq in addressSequence[i:i+8]: s += "{:s},".format(seq)
    if i//8 == 58: s = s[:-1] + " "
    s += " // Button {:02d}".format(  i // 8+1) 
    print(s)
print("};")
print()
print("const static byte HWKBD_Macropage_Cyrillic_UTF8[] PROGMEM = {")
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
