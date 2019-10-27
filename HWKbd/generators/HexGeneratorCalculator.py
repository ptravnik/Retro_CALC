#
# Special RPN calculator keyboard
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
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 01 
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 02 
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 03 
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 04 
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 05
keyb.buttons += [Button([ "","","","","","","",""], [0,0,0,0,4,4,4,4])] # 06 
keyb.buttons += [Button([ "+","","","","","","",""], [0,0,0,0,4,4,4,4])] # 07 + 
keyb.buttons += [Button([ "-","","","","","","",""], [0,0,0,0,4,4,4,4])] # 08 -
keyb.buttons += [Button([ "*","","","","","","",""], [0,0,0,0,4,4,4,4])] # 09 * 
keyb.buttons += [Button([ "/","%","","","","","",""], [0,0,0,0,4,4,4,4])] # 10 / %

keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 11 
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 12 
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 13 
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 14 
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 15
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 16
keyb.buttons += [Button(["7","","","","","","",""], [0,0,0,0,4,4,4,4])] # 17 7
keyb.buttons += [Button(["8","","","","","","",""], [0,0,0,0,4,4,4,4])] # 18 8
keyb.buttons += [Button(["9","","","","","","",""], [0,0,0,0,4,4,4,4])] # 19 9
keyb.buttons += [Button(["0","00","","","","","",""], [0,0,0,0,4,4,4,4])] # 20 0

keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 21
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 22
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 23
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 24
keyb.buttons += [Button(["","T","","","T","","",""], [0,0,0,0,0,4,4,4])] # 25
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 26 
keyb.buttons += [Button(["4","u","","","u","","",""], [0,0,0,0,0,4,4,4])] # 27 4
keyb.buttons += [Button(["5","","","","","","",""], [0,0,0,0,4,4,4,4])] # 28 5
keyb.buttons += [Button(["6","","","","","","",""], [0,0,0,0,4,4,4,4])] # 29 6
keyb.buttons += [Button(["^","p","","","p","","",""], [0,0,0,0,0,4,4,4])] # 30 ^

keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 31
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 32
keyb.buttons += [Button(["","d","","","d","","",""], [0,0,0,0,0,4,4,4])] # 33
keyb.buttons += [Button(["","f","","","f","","",""], [0,0,0,0,0,4,4,4])] # 34
keyb.buttons += [Button(["","G","","","G","","",""], [0,0,0,0,0,4,4,4])] # 35
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 36
keyb.buttons += [Button(["1","","","","","","",""], [0,0,0,0,4,4,4,4])] # 37 1
keyb.buttons += [Button(["2","k","","","k","","",""], [0,0,0,0,0,4,4,4])] # 38 2
keyb.buttons += [Button(["3","","","","","","",""], [0,0,0,0,4,4,4,4])] # 39 3
keyb.buttons += [Button(["#ACTION=177","","","","","","",""], [0,0,0,0,4,4,4,4])] # 40 +/-

keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 41
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 42
keyb.buttons += [Button(["","c","","","c","","",""], [0,0,0,0,0,4,4,4])] # 43
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 44
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 45
keyb.buttons += [Button(["","","","","","","",""], [0,0,0,0,4,4,4,4])] # 46 
keyb.buttons += [Button(["0","n","","","n","","",""], [0,0,0,0,0,4,4,4])] # 47 0, n
keyb.buttons += [Button(["#ACTION=006","m","","","M","","",""], [0,0,0,0,0,4,4,4])] # 48 ee, m, M 
keyb.buttons += [Button([".","","","","","","",""], [0,0,1,1,5,5,4,4])] # 49 .
keyb.buttons += [Button(["#ACTION=008","","","","","","",""], [0,0,0,0,4,4,4,4])] # 50 BACKSP

keyb.buttons += [Button(["","","","","","","",""], [4,5,6,7,0,1,2,3])] # 51 Rus
keyb.buttons += [Button(["","","","","","","",""], [2,2,0,2,6,6,4,6])] # 52 F1
keyb.buttons += [Button(["","","","","","","",""], [3,3,3,0,7,7,7,0])] # 53 F2
keyb.buttons += [Button(["#ACTION=001","","","","","","",""], [0,0,0,0,4,4,4,4])] # 54 left
keyb.buttons += [Button(["#ACTION=002","","","","","","",""], [0,0,0,0,4,4,4,4])] # 55 down
keyb.buttons += [Button(["#ACTION=003","","","","","","",""], [0,0,0,0,4,4,4,4])] # 56 up
keyb.buttons += [Button(["#ACTION=004","","","","","","",""], [0,0,0,0,4,4,4,4])] # 57 right
keyb.buttons += [Button(["#ACTION=005","","","","","","",""], [0,0,0,0,4,4,4,4])] # 58 ENTER
keyb.buttons += [Button(["","","","","","","",""], [1,1,1,1,5,5,5,5])] # 59 SHIFT

addressSequence, macroSequence = keyb.makeArrays()

print("const static uint16_t HWKBD_Codepage_RPN[] PROGMEM = {")
for i in range( 0, len( addressSequence), 8):
    s = ""
    for seq in addressSequence[i:i+8]: s += "{:s},".format(seq)
    if i//8 == 58: s = s[:-1] + " "
    s += " // Button {:02d}".format(  i // 8+1) 
    print(s)
print("};")
print()
print("const static byte HWKBD_Macropage_RPN[] PROGMEM = {")
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
