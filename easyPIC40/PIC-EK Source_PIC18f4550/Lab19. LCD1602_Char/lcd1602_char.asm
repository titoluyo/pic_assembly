000000:  ef0c  goto    0x000018
000002:  f000
000018:  efab  goto    0x000156
00001a:  f000
00001c:  00fa  dw      0x00fa
00001e:  d02b  bra     0x000076
000020:  0100  movlb   0x0
000022:  6bfc  clrf    0xfc, 0x1
000024:  6bfd  clrf    0xfd, 0x1
000026:  d01c  bra     0x000060
000028:  0100  movlb   0x0
00002a:  6bfe  clrf    0xfe, 0x1
00002c:  6bff  clrf    0xff, 0x1
00002e:  0e00  movlw   0x00
000030:  5dff  subwf   0xff, 0x0, 0x1
000032:  0e0a  movlw   0x0a
000034:  b4d8  btfsc   0xd8, 0x2, 0x0
000036:  5dfe  subwf   0xfe, 0x0, 0x1
000038:  a0d8  btfss   0xd8, 0x0, 0x0
00003a:  d001  bra     0x00003e
00003c:  d001  bra     0x000040
00003e:  d001  bra     0x000042
000040:  d00c  bra     0x00005a
000042:  0100  movlb   0x0
000044:  4bfe  infsnz  0xfe, 0x1, 0x1
000046:  2bff  incf    0xff, 0x1, 0x1
000048:  0e00  movlw   0x00
00004a:  5dff  subwf   0xff, 0x0, 0x1
00004c:  0e0a  movlw   0x0a
00004e:  b4d8  btfsc   0xd8, 0x2, 0x0
000050:  5dfe  subwf   0xfe, 0x0, 0x1
000052:  a0d8  btfss   0xd8, 0x0, 0x0
000054:  d001  bra     0x000058
000056:  d001  bra     0x00005a
000058:  d7f4  bra     0x000042
00005a:  0100  movlb   0x0
00005c:  4bfc  infsnz  0xfc, 0x1, 0x1
00005e:  2bfd  incf    0xfd, 0x1, 0x1
000060:  0100  movlb   0x0
000062:  51fb  movf    0xfb, 0x0, 0x1
000064:  5dfd  subwf   0xfd, 0x0, 0x1
000066:  e102  bnz     0x00006c
000068:  51fa  movf    0xfa, 0x0, 0x1
00006a:  5dfc  subwf   0xfc, 0x0, 0x1
00006c:  a0d8  btfss   0xd8, 0x0, 0x0
00006e:  d001  bra     0x000072
000070:  d001  bra     0x000074
000072:  d7da  bra     0x000028
000074:  0012  return  0x0
000076:  d7d4  bra     0x000020
000078:  d00c  bra     0x000092
00007a:  8e95  bsf     0x95, 0x7, 0x0
00007c:  9a81  bcf     0x81, 0x5, 0x0
00007e:  8881  bsf     0x81, 0x4, 0x0
000080:  8681  bsf     0x81, 0x3, 0x0
000082:  d000  bra     0x000084
000084:  be83  btfsc   0x83, 0x7, 0x0
000086:  d001  bra     0x00008a
000088:  d001  bra     0x00008c
00008a:  d7fc  bra     0x000084
00008c:  9681  bcf     0x81, 0x3, 0x0
00008e:  9e95  bcf     0x95, 0x7, 0x0
000090:  0012  return  0x0
000092:  d7f3  bra     0x00007a
000094:  d007  bra     0x0000a4
000096:  9a81  bcf     0x81, 0x5, 0x0
000098:  9881  bcf     0x81, 0x4, 0x0
00009a:  6e83  movwf   0x83, 0x0
00009c:  8681  bsf     0x81, 0x3, 0x0
00009e:  f000  dw      0xf000
0000a0:  9681  bcf     0x81, 0x3, 0x0
0000a2:  0012  return  0x0
0000a4:  d7f8  bra     0x000096
0000a6:  d006  bra     0x0000b4
0000a8:  dfe7  rcall   0x000078
0000aa:  0100  movlb   0x0
0000ac:  6fff  movwf   0xff, 0x1
0000ae:  51ff  movf    0xff, 0x0, 0x1
0000b0:  dff1  rcall   0x000094
0000b2:  0012  return  0x0
0000b4:  d7f9  bra     0x0000a8
0000b6:  d008  bra     0x0000c8
0000b8:  dfdf  rcall   0x000078
0000ba:  8a81  bsf     0x81, 0x5, 0x0
0000bc:  9881  bcf     0x81, 0x4, 0x0
0000be:  6e83  movwf   0x83, 0x0
0000c0:  8681  bsf     0x81, 0x3, 0x0
0000c2:  f000  dw      0xf000
0000c4:  9681  bcf     0x81, 0x3, 0x0
0000c6:  0012  return  0x0
0000c8:  d7f7  bra     0x0000b8
0000ca:  00fd  dw      0x00fd
0000cc:  d014  bra     0x0000f6
0000ce:  0b0f  andlw   0x0f
0000d0:  0100  movlb   0x0
0000d2:  6ffe  movwf   0xfe, 0x1
0000d4:  0e01  movlw   0x01
0000d6:  17fd  andwf   0xfd, 0x1, 0x1
0000d8:  51fd  movf    0xfd, 0x0, 0x1
0000da:  a4d8  btfss   0xd8, 0x2, 0x0
0000dc:  d001  bra     0x0000e0
0000de:  d001  bra     0x0000e2
0000e0:  d004  bra     0x0000ea
0000e2:  51fe  movf    0xfe, 0x0, 0x1
0000e4:  0980  iorlw   0x80
0000e6:  dfdf  rcall   0x0000a6
0000e8:  d005  bra     0x0000f4
0000ea:  0100  movlb   0x0
0000ec:  51fe  movf    0xfe, 0x0, 0x1
0000ee:  0f40  addlw   0x40
0000f0:  0980  iorlw   0x80
0000f2:  dfd9  rcall   0x0000a6
0000f4:  0012  return  0x0
0000f6:  d7eb  bra     0x0000ce
0000f8:  d020  bra     0x00013a
0000fa:  0e96  movlw   0x96
0000fc:  0100  movlb   0x0
0000fe:  6ffa  movwf   0xfa, 0x1
000100:  6bfb  clrf    0xfb, 0x1
000102:  df8d  rcall   0x00001e
000104:  0e38  movlw   0x38
000106:  dfc6  rcall   0x000094
000108:  0e32  movlw   0x32
00010a:  0100  movlb   0x0
00010c:  6ffa  movwf   0xfa, 0x1
00010e:  6bfb  clrf    0xfb, 0x1
000110:  df86  rcall   0x00001e
000112:  0e38  movlw   0x38
000114:  dfbf  rcall   0x000094
000116:  0e32  movlw   0x32
000118:  0100  movlb   0x0
00011a:  6ffa  movwf   0xfa, 0x1
00011c:  6bfb  clrf    0xfb, 0x1
00011e:  df7f  rcall   0x00001e
000120:  0e38  movlw   0x38
000122:  dfb8  rcall   0x000094
000124:  0e38  movlw   0x38
000126:  dfbf  rcall   0x0000a6
000128:  0e08  movlw   0x08
00012a:  dfbd  rcall   0x0000a6
00012c:  0e01  movlw   0x01
00012e:  dfbb  rcall   0x0000a6
000130:  0e06  movlw   0x06
000132:  dfb9  rcall   0x0000a6
000134:  0e0c  movlw   0x0c
000136:  dfb7  rcall   0x0000a6
000138:  0012  return  0x0
00013a:  d7df  bra     0x0000fa
00013c:  00fa  dw      0x00fa
00013e:  d00a  bra     0x000154
000140:  c0fa  movff   0x0fa, 0x0fd
000142:  f0fd
000144:  0100  movlb   0x0
000146:  6ffc  movwf   0xfc, 0x1
000148:  51fc  movf    0xfc, 0x0, 0x1
00014a:  dfc0  rcall   0x0000cc
00014c:  0100  movlb   0x0
00014e:  51fb  movf    0xfb, 0x0, 0x1
000150:  dfb2  rcall   0x0000b6
000152:  0012  return  0x0
000154:  d7f5  bra     0x000140
000156:  d0da  bra     0x00030c
000158:  6ad3  clrf    0xd3, 0x0
00015a:  0e0f  movlw   0x0f
00015c:  6ec1  movwf   0xc1, 0x0
00015e:  0ec7  movlw   0xc7
000160:  6e93  movwf   0x93, 0x0
000162:  6a95  clrf    0x95, 0x0
000164:  dfc9  rcall   0x0000f8
000166:  0100  movlb   0x0
000168:  6bfa  clrf    0xfa, 0x1
00016a:  0e41  movlw   0x41
00016c:  6ffb  movwf   0xfb, 0x1
00016e:  0e00  movlw   0x00
000170:  dfe6  rcall   0x00013e
000172:  0100  movlb   0x0
000174:  6bfa  clrf    0xfa, 0x1
000176:  0e42  movlw   0x42
000178:  6ffb  movwf   0xfb, 0x1
00017a:  0e01  movlw   0x01
00017c:  dfe0  rcall   0x00013e
00017e:  0100  movlb   0x0
000180:  6bfa  clrf    0xfa, 0x1
000182:  0e43  movlw   0x43
000184:  6ffb  movwf   0xfb, 0x1
000186:  0e02  movlw   0x02
000188:  dfda  rcall   0x00013e
00018a:  0100  movlb   0x0
00018c:  6bfa  clrf    0xfa, 0x1
00018e:  0e44  movlw   0x44
000190:  6ffb  movwf   0xfb, 0x1
000192:  0e03  movlw   0x03
000194:  dfd4  rcall   0x00013e
000196:  0100  movlb   0x0
000198:  6bfa  clrf    0xfa, 0x1
00019a:  0e45  movlw   0x45
00019c:  6ffb  movwf   0xfb, 0x1
00019e:  0e04  movlw   0x04
0001a0:  dfce  rcall   0x00013e
0001a2:  0100  movlb   0x0
0001a4:  6bfa  clrf    0xfa, 0x1
0001a6:  0e46  movlw   0x46
0001a8:  6ffb  movwf   0xfb, 0x1
0001aa:  0e05  movlw   0x05
0001ac:  dfc8  rcall   0x00013e
0001ae:  0100  movlb   0x0
0001b0:  6bfa  clrf    0xfa, 0x1
0001b2:  0e47  movlw   0x47
0001b4:  6ffb  movwf   0xfb, 0x1
0001b6:  0e06  movlw   0x06
0001b8:  dfc2  rcall   0x00013e
0001ba:  0100  movlb   0x0
0001bc:  6bfa  clrf    0xfa, 0x1
0001be:  0e48  movlw   0x48
0001c0:  6ffb  movwf   0xfb, 0x1
0001c2:  0e07  movlw   0x07
0001c4:  dfbc  rcall   0x00013e
0001c6:  0100  movlb   0x0
0001c8:  6bfa  clrf    0xfa, 0x1
0001ca:  0e49  movlw   0x49
0001cc:  6ffb  movwf   0xfb, 0x1
0001ce:  0e08  movlw   0x08
0001d0:  dfb6  rcall   0x00013e
0001d2:  0100  movlb   0x0
0001d4:  6bfa  clrf    0xfa, 0x1
0001d6:  0e4a  movlw   0x4a
0001d8:  6ffb  movwf   0xfb, 0x1
0001da:  0e09  movlw   0x09
0001dc:  dfb0  rcall   0x00013e
0001de:  0100  movlb   0x0
0001e0:  6bfa  clrf    0xfa, 0x1
0001e2:  0e4b  movlw   0x4b
0001e4:  6ffb  movwf   0xfb, 0x1
0001e6:  0e0a  movlw   0x0a
0001e8:  dfaa  rcall   0x00013e
0001ea:  0100  movlb   0x0
0001ec:  6bfa  clrf    0xfa, 0x1
0001ee:  0e4c  movlw   0x4c
0001f0:  6ffb  movwf   0xfb, 0x1
0001f2:  0e0b  movlw   0x0b
0001f4:  dfa4  rcall   0x00013e
0001f6:  0100  movlb   0x0
0001f8:  6bfa  clrf    0xfa, 0x1
0001fa:  0e4d  movlw   0x4d
0001fc:  6ffb  movwf   0xfb, 0x1
0001fe:  0e0c  movlw   0x0c
000200:  df9e  rcall   0x00013e
000202:  0100  movlb   0x0
000204:  6bfa  clrf    0xfa, 0x1
000206:  0e4e  movlw   0x4e
000208:  6ffb  movwf   0xfb, 0x1
00020a:  0e0d  movlw   0x0d
00020c:  df98  rcall   0x00013e
00020e:  0100  movlb   0x0
000210:  6bfa  clrf    0xfa, 0x1
000212:  0e4f  movlw   0x4f
000214:  6ffb  movwf   0xfb, 0x1
000216:  0e0e  movlw   0x0e
000218:  df92  rcall   0x00013e
00021a:  0100  movlb   0x0
00021c:  6bfa  clrf    0xfa, 0x1
00021e:  0e50  movlw   0x50
000220:  6ffb  movwf   0xfb, 0x1
000222:  0e0f  movlw   0x0f
000224:  df8c  rcall   0x00013e
000226:  0e01  movlw   0x01
000228:  0100  movlb   0x0
00022a:  6ffa  movwf   0xfa, 0x1
00022c:  0e61  movlw   0x61
00022e:  6ffb  movwf   0xfb, 0x1
000230:  0e00  movlw   0x00
000232:  df85  rcall   0x00013e
000234:  0e01  movlw   0x01
000236:  0100  movlb   0x0
000238:  6ffa  movwf   0xfa, 0x1
00023a:  0e62  movlw   0x62
00023c:  6ffb  movwf   0xfb, 0x1
00023e:  0e01  movlw   0x01
000240:  df7e  rcall   0x00013e
000242:  0e01  movlw   0x01
000244:  0100  movlb   0x0
000246:  6ffa  movwf   0xfa, 0x1
000248:  0e63  movlw   0x63
00024a:  6ffb  movwf   0xfb, 0x1
00024c:  0e02  movlw   0x02
00024e:  df77  rcall   0x00013e
000250:  0e01  movlw   0x01
000252:  0100  movlb   0x0
000254:  6ffa  movwf   0xfa, 0x1
000256:  0e64  movlw   0x64
000258:  6ffb  movwf   0xfb, 0x1
00025a:  0e03  movlw   0x03
00025c:  df70  rcall   0x00013e
00025e:  0e01  movlw   0x01
000260:  0100  movlb   0x0
000262:  6ffa  movwf   0xfa, 0x1
000264:  0e65  movlw   0x65
000266:  6ffb  movwf   0xfb, 0x1
000268:  0e04  movlw   0x04
00026a:  df69  rcall   0x00013e
00026c:  0e01  movlw   0x01
00026e:  0100  movlb   0x0
000270:  6ffa  movwf   0xfa, 0x1
000272:  0e66  movlw   0x66
000274:  6ffb  movwf   0xfb, 0x1
000276:  0e05  movlw   0x05
000278:  df62  rcall   0x00013e
00027a:  0e01  movlw   0x01
00027c:  0100  movlb   0x0
00027e:  6ffa  movwf   0xfa, 0x1
000280:  0e67  movlw   0x67
000282:  6ffb  movwf   0xfb, 0x1
000284:  0e06  movlw   0x06
000286:  df5b  rcall   0x00013e
000288:  0e01  movlw   0x01
00028a:  0100  movlb   0x0
00028c:  6ffa  movwf   0xfa, 0x1
00028e:  0e68  movlw   0x68
000290:  6ffb  movwf   0xfb, 0x1
000292:  0e07  movlw   0x07
000294:  df54  rcall   0x00013e
000296:  0e01  movlw   0x01
000298:  0100  movlb   0x0
00029a:  6ffa  movwf   0xfa, 0x1
00029c:  0e69  movlw   0x69
00029e:  6ffb  movwf   0xfb, 0x1
0002a0:  0e08  movlw   0x08
0002a2:  df4d  rcall   0x00013e
0002a4:  0e01  movlw   0x01
0002a6:  0100  movlb   0x0
0002a8:  6ffa  movwf   0xfa, 0x1
0002aa:  0e6a  movlw   0x6a
0002ac:  6ffb  movwf   0xfb, 0x1
0002ae:  0e09  movlw   0x09
0002b0:  df46  rcall   0x00013e
0002b2:  0e01  movlw   0x01
0002b4:  0100  movlb   0x0
0002b6:  6ffa  movwf   0xfa, 0x1
0002b8:  0e6b  movlw   0x6b
0002ba:  6ffb  movwf   0xfb, 0x1
0002bc:  0e0a  movlw   0x0a
0002be:  df3f  rcall   0x00013e
0002c0:  0e01  movlw   0x01
0002c2:  0100  movlb   0x0
0002c4:  6ffa  movwf   0xfa, 0x1
0002c6:  0e6c  movlw   0x6c
0002c8:  6ffb  movwf   0xfb, 0x1
0002ca:  0e0b  movlw   0x0b
0002cc:  df38  rcall   0x00013e
0002ce:  0e01  movlw   0x01
0002d0:  0100  movlb   0x0
0002d2:  6ffa  movwf   0xfa, 0x1
0002d4:  0e6d  movlw   0x6d
0002d6:  6ffb  movwf   0xfb, 0x1
0002d8:  0e0c  movlw   0x0c
0002da:  df31  rcall   0x00013e
0002dc:  0e01  movlw   0x01
0002de:  0100  movlb   0x0
0002e0:  6ffa  movwf   0xfa, 0x1
0002e2:  0e6e  movlw   0x6e
0002e4:  6ffb  movwf   0xfb, 0x1
0002e6:  0e0d  movlw   0x0d
0002e8:  df2a  rcall   0x00013e
0002ea:  0e01  movlw   0x01
0002ec:  0100  movlb   0x0
0002ee:  6ffa  movwf   0xfa, 0x1
0002f0:  0e6f  movlw   0x6f
0002f2:  6ffb  movwf   0xfb, 0x1
0002f4:  0e0e  movlw   0x0e
0002f6:  df23  rcall   0x00013e
0002f8:  0e01  movlw   0x01
0002fa:  0100  movlb   0x0
0002fc:  6ffa  movwf   0xfa, 0x1
0002fe:  0e70  movlw   0x70
000300:  6ffb  movwf   0xfb, 0x1
000302:  0e0f  movlw   0x0f
000304:  df1c  rcall   0x00013e
000306:  d7ff  bra     0x000306
000308:  ef0c  goto    0x000018
00030a:  f000
00030c:  d725  bra     0x000158
300000:  ff  db      0xff
300001:  fc  db      0xfc
300002:  ff  db      0xff
300003:  fe  db      0xfe
300004:  ff  db      0xff
300005:  fd  db      0xfd
300006:  bb  db      0xbb
300007:  ff  db      0xff
