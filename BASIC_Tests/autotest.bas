00010 ABS(-10.1) # 10.1
00011 ABS( 5.5) # 5.5
00012 abs( 0.0) # 0.0
00013 abs(0.0001) # 1.0e-004
00020 ACOS( -0.5) # 120
00021 ACOS( 0.5)  # 60
00022 acos( -1.5) # error
00023 acos( 1.5)  # error
00024 acos( 0.33) # ca 70.73
00030 ACOSH(1) # 0
00031 ACOSH(2) # ca 1.32
00032 acosh(-2) # error
00033 acosh(2) # ca 1.32
00040 push 1 # Stack displayed
00041 push 2 # Stack displayed
00042 ADD # Stack displayed: 3
00043 push 3 # Stack displayed
00044 add # Stack displayed 6
00050 AMODE RAD # Mode: Radians
00051 amode% # 1
00052 amode grad # Mode: Gradians
00053 amode% # 2
00054 amode% = 1 # Warn: Read-only
00055 amode rad # Mode: Radians
00060 API2GCC( 10) # 1.0
00062 api2gcc( 45) # ca 0.80
00070 #APPEND() #
00071 #APPEND() #
00072 #append() #
00073 #append() #
00074 #append() #
00080 #AREAD() #
00081 #AREAD() #
00082 #aread() #
00083 #aread() #
00084 #aread() #
00090 #AREAD%() #
00091 #AREAD%() #
00092 #aread%() #
00093 #aread%() #
00094 #aread%() #
00100 #ASC() #
00101 #ASC() #
00102 #asc() #
00103 #asc() #
00104 #asc() #
00110 ASIN(1) / PI # 0.5
00111 ASIN(-1)*2 # ca 3.14
00112 asin( 1.5) # error
00113 asin( 0.5)*4 # ca 2.09
00120 ASINH(1) # ca 0.88
00121 ASINH(0) # 0.0
00122 asinh(-1) # ca -0.88
00123 asinh(-10) # ca -3.00
00130 #AT() #
00131 #AT() #
00132 #at() #
00133 #at() #
00140 amode 2 # grads
00141 ATAN( 1) # 50
00142 atan( 0) # 0
00143 atan( -1) # -50
00144 atan( -100) # ca -99.36
00150 ATANH( 1) # error
00151 ATANH( 0.76) # ca 1.00
00152 atanh( -0.1) # ca -0.1
00153 atanh( tanh(5)) # 5
00160 #AWRITE() #
00161 #AWRITE() #
00162 #awrite() #
00163 #awrite() #
00164 #awrite() #
00170 AZIMUTH( 100, 300) #  ca -20.500 gradian 
00171 amode deg # back to degrees
00172 azimuth( 642.787610, -766.044443) # ca 140 degrees
00180 AZIMUTH3( 939.692621, 342.020143, 100) # 20.0000 deg azimuth, 5.7 elevation
00182 azimuth3(939.692621, 342.020143, -100) #
00190 BAR2PSI(10) # ca 145.04
00193 bar2psi( PSI2BAR(100)) # 100
00200 BBL2CM(1000) # ca 159
00202 bbl2cm(100) # ca 15.9
00210 #BREAK() #
00211 #BREAK() #
00212 #break() #
00213 #break() #
00214 #break() #
00220 C2F(0) # 32
00222 c2f(-40) # -40
00230 C2K(0) # 273.15
00232 c2k(-273.15) # 0
00240 CATH(3,5) # 4
00242 cath(50,30) # 40
00250 CF2CM(35) # ca 1
00252 cf2cm( CM2CF(11)) # 11
00260 #CHAIN() #
00261 #CHAIN() #
00262 #chain() #
00263 #chain() #
00264 #chain() #
00270 #CHR$() #
00271 #CHR$() #
00272 #chr$() #
00273 #chr$() #
00274 #chr$() #
00280 #CIRCLE() #
00281 #CIRCLE() #
00282 #circle() #
00283 #circle() #
00284 #circle() #
00290 CLEAR CONST *** # erases user constants
00291 CLEAR VARS *** # erases user variables
00292 clear const TheUltimateAnswer # fails silently
00293 clear stack #
00294 CLEAR STACK #
00295 CLEAR PROGRAM 10, 110 # Reports lines (clear disabled)
00300 CLI # change to command line
00301 RPN # change to RPN
00302 cli #
00303 rpn #
00310 #CLOSE() #
00311 #CLOSE() #
00312 #close() #
00313 #close() #
00314 #close() #
00320 #CLS() #
00321 #CLS() #
00322 #cls() #
00323 #cls() #
00324 #cls() #
00330 CM2BBL( 159) # ca 1000
00332 cm2bbl(BBL2CM(1)) # 1
00340 CM2CF( 1) # ca 35
00342 cm2cf( 100) # ca 3500
00343 CNK( 1, 100) # 100
00344 CNK( 101, 100) # 1
00345 cnk( 3, 100) # 161700
00350 #COLOR() #
00351 #COLOR() #
00352 #color() #
00353 #color() #
00354 #color() #
00360 #CON() #
00361 #CON() #
00362 #con() #
00363 #con() #
00364 #con() #
00370 CONST TheAnswer=42.0 #
00371 CONST TheAnswer%=42 #
00372 const WrongAnswer=46 #
00373 amode grad # Radians
00374 const amode% = 10 # should faila silently
00375 amode% # should be 2
00376 amode rad #
00380 #CONT() #
00381 #CONT() #
00382 #cont() #
00383 #cont() #
00384 #cont() #
00390 #CONTINUE() #
00391 #CONTINUE() #
00392 #continue() #
00393 #continue() #
00394 #continue() #
00400 #COORDS(1,1) #
00402 #coords(2,2) #
00403 #coords() #
00404 #coords() #
00410 #COORDS3() #
00411 #COORDS3() #
00412 #coords3() #
00413 #coords3() #
00414 #coords3() #
00420 COS( PI/4) # ca 0.71
00421 COS( PI) # -1
00422 cos( -pi/2) # ca 0
00423 cos( 1e35) # accuracy warning, 0.53
00430 COSH( 1) # ca 1.54
00431 COSH( -1) # ca 1.54
00432 cosh( 0) # 1
00433 cosh(-10) # ca 11013
00440 amode deg #
00441 COT( 45) # 1
00442 cot( 90) # 0
00443 cot( 0) # error
00450 #CURSOR() #
00451 #CURSOR() #
00452 #cursor() #
00453 #cursor() #
00454 #cursor() #
00460 D2DMS(10.5) # 1000.30
00462 d2dms(-20.25) # -2015.00
00470 #DATA() #
00471 #DATA() #
00472 #data() #
00473 #data() #
00474 #data() #
00480 #DEF() #
00481 #DEF() #
00482 #def() #
00483 #def() #
00484 #def() #
00490 #DELAY() #
00491 #DELAY() #
00492 #delay() #
00493 #delay() #
00494 #delay() #
00500 #DET() #
00501 #DET() #
00502 #det() #
00503 #det() #
00504 #det() #
00510 #DIM() #
00511 #DIM() #
00512 #dim() #
00513 #dim() #
00514 #dim() #
00520 #DIR() #
00521 #DIR() #
00522 #dir() #
00523 #dir() #
00524 #dir() #
00530 #DIRECTORY() #
00531 #DIRECTORY() #
00532 #directory() #
00533 #directory() #
00534 #directory() #
00540 push 5 #
00542 push 2 #
00543 DIV #
00544 push 0.2 #
00545 div #
00550 #DMODE() #
00551 #DMODE() #
00552 #dmode() #
00553 #dmode() #
00554 #dmode() #
00570 #DNOTONE() #
00571 #DNOTONE() #
00572 #dnotone() #
00573 #dnotone() #
00574 #dnotone() #
00580 #DOT() #
00581 #DOT() #
00582 #dot() #
00583 #dot() #
00584 #dot() #
00590 #DREAD() #
00591 #DREAD() #
00592 #dread() #
00593 #dread() #
00594 #dread() #
00600 #DTONE() #
00601 #DTONE() #
00602 #dtone() #
00603 #dtone() #
00604 #dtone() #
00610 #DUMP() #
00611 #DUMP() #
00612 #dump() #
00613 #dump() #
00614 #dump() #
00620 #DWRITE() #
00621 #DWRITE() #
00622 #dwrite() #
00623 #dwrite() #
00624 #dwrite() #
00630 #EDIT() #
00631 #EDIT() #
00632 #edit() #
00633 #edit() #
00634 #edit() #
00640 #ELLIPSE() #
00641 #ELLIPSE() #
00642 #ellipse() #
00643 #ellipse() #
00644 #ellipse() #
00650 #ELSE() #
00651 #ELSE() #
00652 #else() #
00653 #else() #
00654 #else() #
00660 #END() #
00661 #END() #
00662 #end() #
00663 #end() #
00664 #end() #
00670 #ENDIF() #
00671 #ENDIF() #
00672 #endif() #
00673 #endif() #
00674 #endif() #
00680 #ENDLOOP() #
00681 #ENDLOOP() #
00682 #endloop() #
00683 #endloop() #
00684 #endloop() #
00690 EXP( 1) # 2.72
00691 exp( 1) # 2.72
00692 exp( 700) # error
00700 F2C(-40) # -40
00701 f2c(100) # 37.7
00702 FACT(0) # 1
00703 FACT(-1) # error
00704 fact(6) # 720
00705 fact(142) # ca 2.7e+245
00710 #FLOAT() #
00711 #FLOAT() #
00712 #float() #
00713 #float() #
00714 #float() #
00720 #FMAN # list root
00721 #fman #
00730 #FN() #
00731 #FN() #
00732 #fn() #
00733 #fn() #
00734 #fn() #
00740 #FOR() #
00741 #FOR() #
00742 #for() #
00743 #for() #
00744 #for() #
00750 #FREAD() #
00751 #FREAD() #
00752 #fread() #
00753 #fread() #
00754 #fread() #
00760 #FREAD$() #
00761 #FREAD$() #
00762 #fread$() #
00763 #fread$() #
00764 #fread$() #
00770 #FSIZE() #
00771 #FSIZE() #
00772 #fsize() #
00773 #fsize() #
00774 #fsize() #
00780 FT2M( 10) # 3.048
00782 ft2m(-10) # -3.048
00790 GAL2L(1) # ca 3.78
00792 gal2l(-1) # ca -3.78
00800 GCC2API( 1) # 1 g/cc = 10 API
00802 gcc2api(.8) # 45.375 API
00810 push 1 #
00811 push 2 #
00812 push 2 #
00813 push 3 #
00814 goff2 # gain 1, offset 1 (LCD)
00820 #GOSUB() #
00821 #GOSUB() #
00822 #gosub() #
00823 #gosub() #
00824 #gosub() #
00830 #GOTO() #
00831 #GOTO() #
00832 #goto() #
00833 #goto() #
00834 #goto() #
00840 #HEX$() #
00841 #HEX$() #
00842 #hex$() #
00843 #hex$() #
00844 #hex$() #
00850 #IDN() #
00851 #IDN() #
00852 #idn() #
00853 #idn() #
00854 #idn() #
00860 #IF() #
00861 #IF() #
00862 #if() #
00863 #if() #
00864 #if() #
00870 #INCH16$() #
00871 #INCH16$() #
00872 #inch16$() #
00873 #inch16$() #
00874 #inch16$() #
00880 #INCH2$() #
00881 #INCH2$() #
00882 #inch2$() #
00883 #inch2$() #
00884 #inch2$() #
00890 #INCH32$() #
00891 #INCH32$() #
00892 #inch32$() #
00893 #inch32$() #
00894 #inch32$() #
00900 #INCH4$() #
00901 #INCH4$() #
00902 #inch4$() #
00903 #inch4$() #
00904 #inch4$() #
00910 #INCH64$() #
00911 #INCH64$() #
00912 #inch64$() #
00913 #inch64$() #
00914 #inch64$() #
00920 #INCH8$() #
00921 #INCH8$() #
00922 #inch8$() #
00923 #inch8$() #
00924 #inch8$() #
00930 #INJ() #
00931 #INJ() #
00932 #inj() #
00933 #inj() #
00934 #inj() #
00940 #INKEY$() #
00941 #INKEY$() #
00942 #inkey$() #
00943 #inkey$() #
00944 #inkey$() #
00950 #INPUT() #
00951 #INPUT() #
00952 #input() #
00953 #input() #
00954 #input() #
00960 #INSCAN$() #
00961 #INSCAN$() #
00962 #inscan$() #
00963 #inscan$() #
00964 #inscan$() #
00970 #INT%() #
00971 #INT%() #
00972 #int%() #
00973 #int%() #
00974 #int%() #
00980 #INV() #
00981 #INV() #
00982 #inv() #
00983 #inv() #
00984 push 3 #
00985 OOX #
00986 oox #
00990 KG2LBS( 1) # ca 2.2
00992 kg2lbs( -1) # ca -2.2
01000 KM2MI(1.6) # ca 1
01002 km2mi(-1.6) # ca -1
01010 KM2NM(1.852) # 1.0
01012 km2nm( -1.852) # ca -1
01020 KMH2MS(3.6) # 1
01022 kmh2ms(-36) # -10
01030 L2GAL(4) # ca 1
01032 l2gal(-40) # ca-10
01040 LBS2KG(1) # ca 0.4
01043 lbs2kg(-1) # ca -0.4
01050 LCIRC(0.5) # 3.14
01053 lcirc(-5) # -31.4
01060 #LEFT$() #
01061 #LEFT$() #
01062 #left$() #
01063 #left$() #
01064 #left$() #
01070 LET xxx = 5 #
01071 LET yyy = 7 #
01072 let yyy = TheAnswer #
01073 let zzz = pi #
01074 let ttt = xxx * yyy #
01075 ttt # 42*5=210
01080 LG(1000) # 3
01081 lg(0.1) # -1
01082 lg(-1) # error
01090 LIN2(1) # 2
01091 LIN2(2) # 3
01092 lin2(3) # 4
01100 #LINE() #
01101 #LINE() #
01102 #line() #
01103 #line() #
01104 #line() #
01110 #LINETO() #
01111 #LINETO() #
01112 #lineto() #
01113 #lineto() #
01114 #lineto() #
01120 #LINEW() #
01121 #LINEW() #
01122 #linew() #
01123 #linew() #
01124 #linew() #
01130 LIST VARS #
01131 LIST CONST #
01132 list 1120, 1120+20 # lists program
01133 list vars #
01134 list CONST #
01140 LN( exp(20)) # 20
01141 LN( 2.71) # ca 1
01142 ln(-2.71) # error
01143 ln( 10) # ca 2.30
01150 #LOAD() #
01151 #LOAD() #
01152 #load() #
01153 #load() #
01154 #load() #
01160 LOG( 1024, 2) # 10
01161 LOG( EXP(2), EXP(1)) # 3
01162 log( 7^34, 7) # 33
01163 log( -2, 2) # error
01164 log( 2, 1) # error
01170 #LOOP() #
01171 #LOOP() #
01172 #loop() #
01173 #loop() #
01174 #loop() #
01180 LT2MT( 1) #
01182 lt2mt( -1) #
01190 M2FT(1) #
01192 m2ft(-1) #
01200 #M2KM() #
01201 #M2KM() #
01202 #m2km() #
01203 #m2km() #
01204 #m2km() #
01210 #MAT() #
01211 #MAT() #
01212 #mat() #
01213 #mat() #
01214 #mat() #
01220 #MATRIX() #
01221 #MATRIX() #
01222 #matrix() #
01223 #matrix() #
01224 #matrix() #
01230 MEM # shows memory
01231 mem #
01240 #MID$() #
01241 #MID$() #
01242 #mid$() #
01243 #mid$() #
01244 #mid$() #
01250 MM2IN(25.4) # 1
01252 mm2in(-50) # ca -2
01260 #MOD() #
01261 #MOD() #
01262 #mod() #
01263 #mod() #
01264 #mod() #
01270 MPA2PSI( 100) # 14.7
01272 mpa2psi( 100) # 14.7
01280 MS2KMH(1) # 3.6
01282 ms2kmh(-10) # -36
01290 MT2LT(1) #
01292 mt2lt(1) #
01300 MT2ST(1) #
01302 mt2st(1) #
01310 push 2 #
01311 push 2.5 #
01312 MUL:push 7 #
01313 mul # 35
01320 StDev = 1.0
01321 Mean = 0.0
01322 StDev
01323 Mean
01324 NDIS(0) # 1/sqrt(pi)
01325 ndis(1) # 
01330 NEG #
01331 neg #
01340 #NEW() #
01341 #NEW() #
01342 #new() #
01343 #new() #
01344 #new() #
01350 #NEXT() #
01351 #NEXT() #
01352 #next() #
01353 #next() #
01354 #next() #
01360 NM2KM(1) # 1.824
01362 nm2km(1) #
01370 #OPEN() #
01371 #OPEN() #
01372 #open() #
01373 #open() #
01374 #open() #
01380 PA2PSI(101300) # 14.7
01382 pa2psi(101300) # 14.7
01390 #PAUSE() #
01391 #PAUSE() #
01392 #pause() #
01393 #pause() #
01394 #pause() #
01400 #POINT() #
01401 #POINT() #
01402 #point() #
01403 #point() #
01404 #point() #
01410 #POKE() #
01411 #POKE() #
01412 #poke() #
01413 #poke() #
01414 #poke() #
01420 POP #
01421 pop #
01430 POW(0,0) # 1
01431 POW(0,-1) # error
01432 pow(0, 3.3) # 0
01433 pow( 1.2, 0) # 1
01434 pow( 2, 10) # 1024
01435 pow( 2, -3) # 0.125
01436 pow( -2, -3) # -0.125
01437 pow( -2, -3.2) # error
01440 #PRINT() #
01441 #PRINT() #
01442 #print() #
01443 #print() #
01444 #print() #
01450 #PRINTALT() #
01451 #PRINTALT() #
01452 #printalt() #
01453 #printalt() #
01454 #printalt() #
01460 #PROB() #
01461 #PROB() #
01462 #prob() #
01463 #prob() #
01464 #prob() #
01470 #PROBIT() #
01471 #PROBIT() #
01472 #probit() #
01473 #probit() #
01474 #probit() #
01480 #PROBR() #
01481 #PROBR() #
01482 #probr() #
01483 #probr() #
01484 #probr() #
01490 #PROGRAM() #
01491 #PROGRAM() #
01492 #program() #
01493 #program() #
01494 #program() #
01500 PSI2BAR(-145.04) # -10
01502 psi2bar( BAR2PSI(3+3)) # 6
01510 PSI2MPA(14.7) # 100
01512 psi2mpa(14.7) # 100
01520 PSI2PA( 14.7) # 101300
01522 psi2pa(14.7) # 101300
01530 PSIA2PSIG(14.7) # 0
01532 psia2psig(114.7) # 100
01540 PSIG2PSIA(0) # 14.7
01542 psig2psia(10) # 24.7
01550 PUSH 1 #
01551 PUSH 2 #
01552 push 1 #
01560 QUAD # Single root
01561 QUAD # Single root
01562 push 5 #
01563 quad # Two roots: -2.79, 1.79
01570 RADIUS( 3, 4) # 5
01572 #radius( 3,2+2) # 5
01580 #RANDOM() #
01581 #RANDOM() #
01582 #random() #
01583 #random() #
01584 #random() #
01590 #RANDOM%() #
01591 #RANDOM%() #
01592 #random%() #
01593 #random%() #
01594 #random%() #
01600 #READ() #
01601 #READ() #
01602 #read() #
01603 #read() #
01604 #read() #
01610 #RECT() #
01611 #RECT() #
01612 #rect() #
01613 #rect() #
01614 #rect() #
01620 #REGR() #
01621 #REGR() #
01622 #regr() #
01623 #regr() #
01624 #regr() #
01630 REM Remark
01631 sin(45) :rem should be ca 0.71
01650 #RESET() #
01651 #RESET() #
01652 #reset() #
01653 #reset() #
01654 #reset() #
01660 #RESTORE() #
01661 #RESTORE() #
01662 #restore() #
01663 #restore() #
01664 #restore() #
01670 #RIGHT$() #
01671 #RIGHT$() #
01672 #right$() #
01673 #right$() #
01674 #right$() #
01680 #RMD() #
01681 #RMD() #
01682 #rmd() #
01683 #rmd() #
01684 #rmd() #
01690 #RND() #
01691 #RND() #
01692 #rnd() #
01693 #rnd() #
01694 #rnd() #
01700 ROLL #
01701 ROLL #
01702 roll #
01703 roll #
01704 roll #
01710 ROOT( 1024, 10) # 2
01712 root( 42^2.3, 2.3) # 42
01720 #ROT() #
01721 #ROT() #
01722 #rot() #
01723 #rot() #
01724 #rot() #
01730 #ROT3c() #
01731 #ROT3c() #
01732 #rot3c() #
01733 #rot3c() #
01734 #rot3c() #
01740 #ROT3s() #
01741 #ROT3s() #
01742 #rot3s() #
01743 #rot3s() #
01744 #rot3s() #
01750 #ROUND() #
01751 #ROUND() #
01752 #round() #
01753 #round() #
01754 #round() #
01760 #RPN() #
01761 #RPN() #
01762 #rpn() #
01763 #rpn() #
01764 #rpn() #
01770 #RUN() #
01771 #RUN() #
01772 #run() #
01773 #run() #
01774 #run() #
01780 #SAVE() #
01781 #SAVE() #
01782 #save() #
01783 #save() #
01784 #save() #
01790 SCIRC(1) # 3.14
01792 scirc(-1) # 3.14
01800 #SCREEN() #
01801 #SCREEN() #
01802 #screen() #
01803 #screen() #
01804 #screen() #
01810 #SEED() #
01811 #SEED() #
01812 #seed() #
01813 #seed() #
01814 #seed() #
01820 SIGN(-100) # -1
01821 SIGN( 0) # 0
01822 sign(340) # 1
01830 SIN( 15*3) # ca 0.71
01831 SIN( -45) # ca 0.71
01832 sin(0) # 0
01833 sin(180) # 0
01840 SINH(1) # 1.18
01841 SINH(2) # 3.63
01842 sinh(1000) # overflow
01843 sinh(1) # -1.18
01850 SM2SFT(1) # 10.7
01852 sm2sft(-1) # -10.7
01860 SQ(3) # 9
01862 sq(4) # 16
01870 SQRT(9) # 3
01872 sqrt( 16) # 4
01880 SSPHERE(1) #
01882 ssphere(10) #
01890 ST2MT(1) #
01892 st2mt(1) #
01900 STACK(1) #
01901 STACK(2) #
01902 stack(3) #
01903 stack(4) #
01904 stack(20) #
01910 #STAT #
01911 #STAT() #
01912 #stat() #
01913 #stat() #
01914 #stat() #
01920 #STATXY() #
01921 #STATXY() #
01922 #statxy() #
01923 #statxy() #
01924 #statxy() #
01930 #STEP() #
01931 #STEP() #
01932 #step() #
01933 #step() #
01934 #step() #
01940 #STOP() #
01941 #STOP() #
01942 #stop() #
01943 #stop() #
01944 #stop() #
01950 #STORE() #
01951 #STORE() #
01952 #store() #
01953 #store() #
01954 #store() #
01960 #STR$() #
01961 #STR$() #
01962 #str$() #
01963 #str$() #
01964 #str$() #
01970 SUB #
01971 SUB #
01972 sub #
01980 SUM 10 #
01981 SUM 10.1 #
01982 sum 9.9 #
01983 sum 3.3*3 #
01984 sum 10 + 0.1 #
01985 nMean # 5
01986 Mean # 10
01987 StDev # 0.1
01990 #SUMS() #
01991 #SUMS() #
01992 #sums() #
01993 #sums() #
01994 #sums() #
02000 #SUMXY() #
02001 #SUMXY() #
02002 #sumxy() #
02003 #sumxy() #
02004 #sumxy() #
02010 SWAP #
02012 swap #
02020 TAN( 45) # 1
02021 TAN( -45) # -1
02022 tan( 0) # 0
02023 tan( 90) # error
02030 TANH( 1) # ca 0.76
02031 tanh( -2) # ca -0.96
02040 #TERM() #
02041 #TERM() #
02042 #term() #
02043 #term() #
02044 #term() #
02050 #TIME() #
02051 #TIME() #
02052 #time() #
02053 #time() #
02054 #time() #
02060 #TO() #
02061 #TO() #
02062 #to() #
02063 #to() #
02064 #to() #
02070 #TRN() #
02071 #TRN() #
02072 #trn() #
02073 #trn() #
02074 #trn() #
02080 list VARS #
02082 CLEAR vars *** #
02083 list vars #
02090 #VECTOR() #
02091 #VECTOR() #
02092 #vector() #
02093 #vector() #
02094 #vector() #
02100 #VSPHERE() #
02101 #VSPHERE() #
02102 #vsphere() #
02103 #vsphere() #
02104 #vsphere() #
02110 #WEEKDAY() #
02111 #WEEKDAY() #
02112 #weekday() #
02113 #weekday() #
02114 #weekday() #
02120 #WRITE() #
02121 #WRITE() #
02122 #write() #
02123 #write() #
02124 #write() #
02130 #ZER() #
02131 #ZER() #
02132 #zer() #
02133 #zer() #
02134 #zer() #
