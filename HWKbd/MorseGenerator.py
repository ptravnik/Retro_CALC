class Button():
    def __init__(self, strokes=[], modeswitches=[]):
        self.keystrokes = ["", "", "", "", "", "", "", ""]
        self.modeswitches = [0, 1, 2, 3, 4, 5, 6, 7]
        for i, s in enumerate(strokes):
            self.keystrokes[i] = s
            self.modeswitches[i] = modeswitches[i]
            if len(s)>1: self.modeswitches[i] += 8
    def makeMode( self, mode, strokeSeqLength):
        k =self.keystrokes[mode]
        m = self.modeswitches[mode]
        c = m * 4096 # 16**3
        addrFormat = lambda c: ["0x{:04x}".format( c)]
        strokeFormat = lambda c: ["0x{:02x}".format( c)]
        if m < 8:
            if len(k) == 1: c += ord( k[0])
            return addrFormat( c), []
        c += strokeSeqLength
        tmp = []
        if k.startswith("#ACTION="):
            tmp += strokeFormat( 0)
            tmp += strokeFormat( int(k[8:]))
            tmp += strokeFormat( 0)
            return addrFormat( c), tmp
        for ch in k:
            o = ord(ch)
            if o < 256: tmp += strokeFormat( o)
            else:
                tmp += strokeFormat( o // 256)
                tmp += strokeFormat( o %  256)
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
        return addressSequence, macroSequence

keyb = Keyboard()
keyb.buttons += [Button([ "_SOS ...---...","","","","","","",""], [0,0,0,0,4,4,4,4])] # 01 esc
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 02 del
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 03 undo
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 04 cut
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 05 copy
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 06 paste
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 07 + &
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 08 - |
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 09 * ^
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 10 / %

keyb.buttons += [Button(["_1 .----","","","","","","",""], [0,0,0,0,4,4,4,4])] # 11 1
keyb.buttons += [Button(["_2 ..---","","","","","","",""], [0,0,0,0,4,4,4,4])] # 12 2
keyb.buttons += [Button(["_3 ...--","","","","","","",""], [0,0,0,0,4,4,4,4])] # 13 3
keyb.buttons += [Button(["_4 ....-","","","","","","",""], [0,0,0,0,4,4,4,4])] # 14 4
keyb.buttons += [Button(["_5 .....","","","","","","",""], [0,0,0,0,4,4,4,4])] # 15 5
keyb.buttons += [Button(["_6 -....","","","","","","",""], [0,0,0,0,4,4,4,4])] # 16 6
keyb.buttons += [Button(["_7 --...","","","","","","",""], [0,0,0,0,4,4,4,4])] # 17 7
keyb.buttons += [Button(["_8 ---..","","","","","","",""], [0,0,0,0,4,4,4,4])] # 18 8
keyb.buttons += [Button(["_9 ----.","","","","","","",""], [0,0,0,0,4,4,4,4])] # 19 9
keyb.buttons += [Button(["_0 -----","- ","","","","","",""], [0,0,0,0,4,4,4,4])] # 20 0

keyb.buttons += [Button(["_Q --.-","","","","","","",""], [0,0,0,0,4,4,4,4])] # 21 q
keyb.buttons += [Button(["_W .--","","","","","","",""], [0,0,0,0,4,4,4,4])] # 22 w
keyb.buttons += [Button(["_E .","","","","","","",""], [0,0,0,0,4,4,4,4])] # 23 e
keyb.buttons += [Button(["_R .-.","","","","","","",""], [0,0,0,0,4,4,4,4])] # 24 r
keyb.buttons += [Button(["_T -","","","","","","",""], [0,0,0,0,4,4,4,4])] # 25 t
keyb.buttons += [Button(["_Y -.--","","","","","","",""], [0,0,0,0,4,4,4,4])] # 26 y
keyb.buttons += [Button(["_U ..-","","","","","","",""], [0,0,0,0,4,4,4,4])] # 27 u
keyb.buttons += [Button(["_I ..","","","","","","",""], [0,0,0,0,4,4,4,4])] # 28 i
keyb.buttons += [Button(["_O ---","","","","","","",""], [0,0,0,0,4,4,4,4])] # 29 o
keyb.buttons += [Button(["_P .--.","","","","","","",""], [0,0,0,0,4,4,4,4])] # 30 p

keyb.buttons += [Button(["_A .-","","","","","","",""], [0,0,0,0,4,4,4,4])] # 31 a
keyb.buttons += [Button(["_S ...","","","","","","",""], [0,0,0,0,4,4,4,4])] # 32 s
keyb.buttons += [Button(["_D -..","","","","","","",""], [0,0,0,0,4,4,4,4])] # 33 d
keyb.buttons += [Button(["_F ..-","","","","","","",""], [0,0,0,0,4,4,4,4])] # 34 f
keyb.buttons += [Button(["_G --.","","","","","","",""], [0,0,0,0,4,4,4,4])] # 35 g
keyb.buttons += [Button(["_H ....","","","","","","",""], [0,0,0,0,4,4,4,4])] # 36 h
keyb.buttons += [Button(["_J .---","","","","","","",""], [0,0,0,0,4,4,4,4])] # 37 j
keyb.buttons += [Button(["_K -.-","","","","","","",""], [0,0,0,0,4,4,4,4])] # 38 k
keyb.buttons += [Button(["_L .-..","","","","","","",""], [0,0,0,0,4,4,4,4])] # 39 l
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 40 , ;

keyb.buttons += [Button(["_Z --..","","","","","","",""], [0,0,0,0,4,4,4,4])] # 41 z
keyb.buttons += [Button(["_X -..-","","","","","","",""], [0,0,0,0,4,4,4,4])] # 42 x
keyb.buttons += [Button(["_C -.-.","","","","","","",""], [0,0,0,0,4,4,4,4])] # 43 c
keyb.buttons += [Button(["_V ...-","","","","","","",""], [0,0,0,0,4,4,4,4])] # 44 v
keyb.buttons += [Button(["_space   ","","","","","","",""], [0,0,0,0,4,4,4,4])] # 45 SPACE
keyb.buttons += [Button(["_B -...","","","","","","",""], [0,0,0,0,4,4,4,4])] # 46 b
keyb.buttons += [Button(["_N -.","","","","","","",""], [0,0,0,0,4,4,4,4])] # 47 n
keyb.buttons += [Button(["_M --","","","","","","",""], [0,0,0,0,4,4,4,4])] # 48 m
keyb.buttons += [Button(["_dot -.. --- -","","","","","","",""], [0,0,0,0,4,4,4,4])] # 49 . ?
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 50 BACKSP

keyb.buttons += [Button(["","","","","","","",""], [4,4,4,4,0,0,0,0])] # 51 Rus
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 52 F1
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 53 F2
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 54 left
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 55 down
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 56 up
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 57 right
keyb.buttons += [Button(["\n","","","","\n","","",""], [0,0,0,0,4,4,4,4])] # 58 ENTER
keyb.buttons += [Button(["","","","","","","",""], [1,1,1,1,5,5,5,5])] # 59 SHIFT

addressSequence, macroSequence = keyb.makeArrays()

print("const static uint16_t HWKBD_Codepage[] PROGMEM = {")
for i in range( 0, len( addressSequence), 8):
    s = ""
    for seq in addressSequence[i:i+8]: s += "{:s},".format(seq)
    if i//8 == 58: s = s[:-1] + " "
    s += " // Button {:02d}".format(  i // 8+1) 
    print(s)
print("};")
print()
print("const static byte HWKBD_Macropage[] PROGMEM = {")
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
