00010 clear sums
00020 sum 147
00030 sum 152
00040 sum 165
00050 sum 163
00060 sum 168
00070 sum 163
00080 sum 175
00090 sum 166
00100 sum 170
00110 sum 167
00120 Mean # should be 163.6
00130 StDev # should be 8.3
00140 (1-prob( 175))*100 # should be ca 8.5
00150 probit( (1-0.95)/2) # should be ca 147.3
00160 probit( (1+0.95)/2) # should be ca 179.9
00170 sumxy  0.9,   0.7
00180 sumxy  7.3, -12.2
00190 sumxy  2.9,  -3.5
00200 sumxy  3.9,  -5.9
00210 sumxy  5.9, -10.1
00220 sumxy  7.7, -14.6
00230 sumxy 10.2, -19.0
00240 sumxy  1.8,  -1.4
00250 sumxy  4.9,  -7.9
00260 sumxy  9.0, -16.8
00270 lin2( 5) # should be -8.1
00280 lin2( 3) # should be -3.9
00290 Gain # should be -2.13
00300 Offset # should be 2.54
00310 end