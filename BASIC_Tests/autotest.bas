00010 REM This is a test
00011 rem This is a test
00012 # This is a test
00020 ABS(-10.1) # 10.1
00021 abs(10.1) # 10.1
00030 ACOS(0.5) # 60
00031 acos(0.5) # 60
00040 amode% = grad # 2
00041 amode% # should be 2
00042 amode% = deg # 0
00050 ASIN(0.5) # 30
00051 asin(0.5) # 30
00060 ATAN(1.0) # 45
00061 atan(1.0) # 45
00070 CATH(3,5) # 4
00071 cath(55,44) # 33
00080 CLEAR amode% # Protected var
00081 clear amode% # Protected var
00082 CLEAR *** # Nothing to clear
00083 clear *** # Nothing to clear
00084 CLEAR CONST True # Protected const
00085 clear const False # Protected const
00086 CLEAR CONST *** # Nothing to clear
00087 clear const *** # Nothing to clear
00088 #CLEAR PROGRAM {i{, j}} # Not implemented yet
00089 CLEAR STACK # Sets stack values to zeros.
00090 CONST UltimateAnswer2 = 7*6 + 1 # 43
00091 const UltimateAnswer = 7*6  # 42
00102 UltimateAnswer2 - UltimateAnswer # 1
00103 UltimateAnswer2 == 42 # 0 (false)
00104 UltimateAnswer2 is 43 # 1 (true)
00105 CLEAR CONST UltimateAnswer2 # Nothing
00106 clear const UltimateAnswer2 # Nothing
00107 UltimateAnswer # 42
00108 UltimateAnswer2 # Nothing
00110 COS(-UltimateAnswer) # 0.7431
00111 cos(UltimateAnswer) # 0.7431
00120 DEG # 0
00121 deg # 0
00130 EXP(2) # 7.39
00131 exp(1) # 2.72
00132 exp(pi) > pi^exp(1) # 1 (true)
00140 False # 0
00141 FALSE # 0
00150 Gain = 3 # 3
00151 Gain # 3
00160 GRAD # 2
00161 grad # 2
00170 GOFF2(.1, .5, .3, .7) # 0.1
00171 goff2(1, 5, 3, 7) # 1
00180 High% # 1
00190 INV(3) # 0.3333
00191 inv(INV(3)) # 3
00200 LCIRC(3) # 18.85
00201 lcirc(0.5) # 3.14
00210 LET aaa = 2.8 * (3+1) # 11.2
00211 let aaa = (3+1) * 2.8 * (2-1)  # 11.2
00212 LET bbb = 2.8 * (3+1) + 1//2 # 11.2
00213 let bbb = 2.8 * (3+1) + 3/%2 # 12.2
00220 LG( 120) # 2.08
00221 lg( 1200) # 3.08
00230 LIN2(3) # 5
00231 lin2(5) # 7
00240 LIST 200, 220 # Should be a list
00241 LIST 140, 140+30 # Should be a list
00242 list const # Lists all constants
00243 LIST CONST # Lists all constants
00244 list vars # Lists all variables
00245 LIST VARS # Lists all variables
00250 LN(7.39) # about 2
00251 ln(2.71) # about 1
00260 LOG(1024,2) # 10
00261 log(1000,10) - lg(1000) # 0
00270 Low% # 0
00280 NEG(-UltimateAnswer) # 42
00281 neg(42.42) # -42.42
00290 Offset # should be 2
00300 PI # about 3.14
00301 pi-PI # 0
00302 amode(rad) # 1
00303 sin( pi/2) # 1.0
00304 cos( pi/2) # almost 0
00305 amode% # 1
00306 amode% = deg # silent
00310 POW(10,18.2) # 1.585e18
00311 pow(2,-2) # 0.25
00320 prev # Previous X register value
00330 QUAD(1,2,1) # -1
00331 quad(10.2,-20.4,10.2) # 1
00340 RAD # 1
00341 rad # 1
00350 RADIUS(3,4) # 5
00351 radius(3.3,4.4)/1.1 # 5
00360 REM {any text}
00361 rem {any text}
00362 42 : rem Must be the answer
00370 ROOT(2,2) # 1.41
00371 ROOT(3,2) # 1.73
00380 SCIRC(1) # 3.14
00381 scirc(10) # 314
00390 SIGN( UltimateAnswer) # 1
00391 sign( -22) # -1
00392 SIGN( UltimateAnswer-42) # 0
00400 SIN(-45) # -0.7071
00401 amode% = grad # silent
00402 sin(-50) # -0.7071
00403 amode% = deg # silent
00410 SQ(10) # 100
00411 sq(2) # 4
00420 SQRT(10) # 3.16
00421 sqrt(100) # 10
00430 SSPHERE(0.5) # 3.14
00431 ssphere(5) # 314
00450 STACK(0) # RPN stack X
00451 stack(2) # RPN stack Z
00460 STORE # Stores INSTANT ON (partial implementation)
00461 STORE CONST # Stores user-defined constants
00470 TAN(45) # 1
00471 tan(-225) # -1
00490 True # 1
00491 TRUE # 1
00500 VSPHERE(0.909) # 3.14
00501 vsphere(9.09) # 3140
00600 end
