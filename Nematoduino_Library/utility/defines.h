#ifndef DEFINES_H
#define DEFINES_H

//
// Arduinio Uno compatibility macros
//
// For dealing with flash memory storage
// of large arrays
//

#ifdef ARDUINO_AVR_UNO
#include <avr/pgmspace.h>
#define LARGE_CONST_ARR PROGMEM
#define READ_WORD(ARR, IDX) pgm_read_word_near(ARR + IDX)
#else
#define LARGE_CONST_ARR
#define READ_WORD(ARR, IDX) ARR[IDX]
#endif

//
// Parameters
//

// Value at which neurons fire
#define THRESHOLD 30

// Maximum number of cycles a neuron
// can be idle before state reset to
// zero
#define MAX_IDLE 10

//
// Totals for various cell types
//

// Total number of all cells
#define CELLS 397

// Number of body and neck muscles
#define BODY_MUSCLES 30
#define NECK_MUSCLES 8

// Number of A and B type motor neurons
#define MOTOR_A 21
#define MOTOR_B 18

// Number of 'signature' A and B type motor neurons
// (used in determining forward or reverse locomotion)
#define SIG_MOTOR_A 2
#define SIG_MOTOR_B 5

//
// Cell IDs
//

#define N_ADAL 0
#define N_ADAR 1
#define N_ADEL 2
#define N_ADER 3
#define N_ADFL 4
#define N_ADFR 5
#define N_ADLL 6
#define N_ADLR 7
#define N_AFDL 8
#define N_AFDR 9
#define N_AIAL 10
#define N_AIAR 11
#define N_AIBL 12
#define N_AIBR 13
#define N_AIML 14
#define N_AIMR 15
#define N_AINL 16
#define N_AINR 17
#define N_AIYL 18
#define N_AIYR 19
#define N_AIZL 20
#define N_AIZR 21
#define N_ALA 22
#define N_ALML 23
#define N_ALMR 24
#define N_ALNL 25
#define N_ALNR 26
#define N_AQR 27
#define N_AS1 28
#define N_AS10 29
#define N_AS11 30
#define N_AS2 31
#define N_AS3 32
#define N_AS4 33
#define N_AS5 34
#define N_AS6 35
#define N_AS7 36
#define N_AS8 37
#define N_AS9 38
#define N_ASEL 39
#define N_ASER 40
#define N_ASGL 41
#define N_ASGR 42
#define N_ASHL 43
#define N_ASHR 44
#define N_ASIL 45
#define N_ASIR 46
#define N_ASJL 47
#define N_ASJR 48
#define N_ASKL 49
#define N_ASKR 50
#define N_AUAL 51
#define N_AUAR 52
#define N_AVAL 53
#define N_AVAR 54
#define N_AVBL 55
#define N_AVBR 56
#define N_AVDL 57
#define N_AVDR 58
#define N_AVEL 59
#define N_AVER 60
#define N_AVFL 61
#define N_AVFR 62
#define N_AVG 63
#define N_AVHL 64
#define N_AVHR 65
#define N_AVJL 66
#define N_AVJR 67
#define N_AVKL 68
#define N_AVKR 69
#define N_AVL 70
#define N_AVM 71
#define N_AWAL 72
#define N_AWAR 73
#define N_AWBL 74
#define N_AWBR 75
#define N_AWCL 76
#define N_AWCR 77
#define N_BAGL 78
#define N_BAGR 79
#define N_BDUL 80
#define N_BDUR 81
#define N_CEPDL 82
#define N_CEPDR 83
#define N_CEPVL 84
#define N_CEPVR 85
#define N_DA1 86
#define N_DA2 87
#define N_DA3 88
#define N_DA4 89
#define N_DA5 90
#define N_DA6 91
#define N_DA7 92
#define N_DA8 93
#define N_DA9 94
#define N_DB1 95
#define N_DB2 96
#define N_DB3 97
#define N_DB4 98
#define N_DB5 99
#define N_DB6 100
#define N_DB7 101
#define N_DD1 102
#define N_DD2 103
#define N_DD3 104
#define N_DD4 105
#define N_DD5 106
#define N_DD6 107
#define N_DVA 108
#define N_DVB 109
#define N_DVC 110
#define N_FLPL 111
#define N_FLPR 112
#define N_HSNL 113
#define N_HSNR 114
#define N_I1L 115
#define N_I1R 116
#define N_I2L 117
#define N_I2R 118
#define N_I3 119
#define N_I4 120
#define N_I5 121
#define N_I6 122
#define N_IL1DL 123
#define N_IL1DR 124
#define N_IL1L 125
#define N_IL1R 126
#define N_IL1VL 127
#define N_IL1VR 128
#define N_IL2DL 129
#define N_IL2DR 130
#define N_IL2L 131
#define N_IL2R 132
#define N_IL2VL 133
#define N_IL2VR 134
#define N_LUAL 135
#define N_LUAR 136
#define N_M1 137
#define N_M2L 138
#define N_M2R 139
#define N_M3L 140
#define N_M3R 141
#define N_M4 142
#define N_M5 143
#define N_MCL 144
#define N_MCR 145
#define N_NSML 146
#define N_NSMR 147
#define N_OLLL 148
#define N_OLLR 149
#define N_OLQDL 150
#define N_OLQDR 151
#define N_OLQVL 152
#define N_OLQVR 153
#define N_PDA 154
#define N_PDB 155
#define N_PDEL 156
#define N_PDER 157
#define N_PHAL 158
#define N_PHAR 159
#define N_PHBL 160
#define N_PHBR 161
#define N_PHCL 162
#define N_PHCR 163
#define N_PLML 164
#define N_PLMR 165
#define N_PLNL 166
#define N_PLNR 167
#define N_PQR 168
#define N_PVCL 169
#define N_PVCR 170
#define N_PVDL 171
#define N_PVDR 172
#define N_PVM 173
#define N_PVNL 174
#define N_PVNR 175
#define N_PVPL 176
#define N_PVPR 177
#define N_PVQL 178
#define N_PVQR 179
#define N_PVR 180
#define N_PVT 181
#define N_PVWL 182
#define N_PVWR 183
#define N_RIAL 184
#define N_RIAR 185
#define N_RIBL 186
#define N_RIBR 187
#define N_RICL 188
#define N_RICR 189
#define N_RID 190
#define N_RIFL 191
#define N_RIFR 192
#define N_RIGL 193
#define N_RIGR 194
#define N_RIH 195
#define N_RIML 196
#define N_RIMR 197
#define N_RIPL 198
#define N_RIPR 199
#define N_RIR 200
#define N_RIS 201
#define N_RIVL 202
#define N_RIVR 203
#define N_RMDDL 204
#define N_RMDDR 205
#define N_RMDL 206
#define N_RMDR 207
#define N_RMDVL 208
#define N_RMDVR 209
#define N_RMED 210
#define N_RMEL 211
#define N_RMER 212
#define N_RMEV 213
#define N_RMFL 214
#define N_RMFR 215
#define N_RMGL 216
#define N_RMGR 217
#define N_RMHL 218
#define N_RMHR 219
#define N_SAADL 220
#define N_SAADR 221
#define N_SAAVL 222
#define N_SAAVR 223
#define N_SABD 224
#define N_SABVL 225
#define N_SABVR 226
#define N_SDQL 227
#define N_SDQR 228
#define N_SIADL 229
#define N_SIADR 230
#define N_SIAVL 231
#define N_SIAVR 232
#define N_SIBDL 233
#define N_SIBDR 234
#define N_SIBVL 235
#define N_SIBVR 236
#define N_SMBDL 237
#define N_SMBDR 238
#define N_SMBVL 239
#define N_SMBVR 240
#define N_SMDDL 241
#define N_SMDDR 242
#define N_SMDVL 243
#define N_SMDVR 244
#define N_URADL 245
#define N_URADR 246
#define N_URAVL 247
#define N_URAVR 248
#define N_URBL 249
#define N_URBR 250
#define N_URXL 251
#define N_URXR 252
#define N_URYDL 253
#define N_URYDR 254
#define N_URYVL 255
#define N_URYVR 256
#define N_VA1 257
#define N_VA10 258
#define N_VA11 259
#define N_VA12 260
#define N_VA2 261
#define N_VA3 262
#define N_VA4 263
#define N_VA5 264
#define N_VA6 265
#define N_VA7 266
#define N_VA8 267
#define N_VA9 268
#define N_VB1 269
#define N_VB10 270
#define N_VB11 271
#define N_VB2 272
#define N_VB3 273
#define N_VB4 274
#define N_VB5 275
#define N_VB6 276
#define N_VB7 277
#define N_VB8 278
#define N_VB9 279
#define N_VC1 280
#define N_VC2 281
#define N_VC3 282
#define N_VC4 283
#define N_VC5 284
#define N_VC6 285
#define N_VD1 286
#define N_VD10 287
#define N_VD11 288
#define N_VD12 289
#define N_VD13 290
#define N_VD2 291
#define N_VD3 292
#define N_VD4 293
#define N_VD5 294
#define N_VD6 295
#define N_VD7 296
#define N_VD8 297
#define N_VD9 298
#define N_MANAL 299
#define N_MDL01 300
#define N_MDL02 301
#define N_MDL03 302
#define N_MDL04 303
#define N_MDL05 304
#define N_MDL06 305
#define N_MDL07 306
#define N_MDL08 307
#define N_MDL09 308
#define N_MDL10 309
#define N_MDL11 310
#define N_MDL12 311
#define N_MDL13 312
#define N_MDL14 313
#define N_MDL15 314
#define N_MDL16 315
#define N_MDL17 316
#define N_MDL18 317
#define N_MDL19 318
#define N_MDL20 319
#define N_MDL21 320
#define N_MDL22 321
#define N_MDL23 322
#define N_MDL24 323
#define N_MDR01 324
#define N_MDR02 325
#define N_MDR03 326
#define N_MDR04 327
#define N_MDR05 328
#define N_MDR06 329
#define N_MDR07 330
#define N_MDR08 331
#define N_MDR09 332
#define N_MDR10 333
#define N_MDR11 334
#define N_MDR12 335
#define N_MDR13 336
#define N_MDR14 337
#define N_MDR15 338
#define N_MDR16 339
#define N_MDR17 340
#define N_MDR18 341
#define N_MDR19 342
#define N_MDR20 343
#define N_MDR21 344
#define N_MDR22 345
#define N_MDR23 346
#define N_MDR24 347
#define N_MI 348
#define N_MVL01 349
#define N_MVL02 350
#define N_MVL03 351
#define N_MVL04 352
#define N_MVL05 353
#define N_MVL06 354
#define N_MVL07 355
#define N_MVL08 356
#define N_MVL09 357
#define N_MVL10 358
#define N_MVL11 359
#define N_MVL12 360
#define N_MVL13 361
#define N_MVL14 362
#define N_MVL15 363
#define N_MVL16 364
#define N_MVL17 365
#define N_MVL18 366
#define N_MVL19 367
#define N_MVL20 368
#define N_MVL21 369
#define N_MVL22 370
#define N_MVL23 371
#define N_MVR01 372
#define N_MVR02 373
#define N_MVR03 374
#define N_MVR04 375
#define N_MVR05 376
#define N_MVR06 377
#define N_MVR07 378
#define N_MVR08 379
#define N_MVR09 380
#define N_MVR10 381
#define N_MVR11 382
#define N_MVR12 383
#define N_MVR13 384
#define N_MVR14 385
#define N_MVR15 386
#define N_MVR16 387
#define N_MVR17 388
#define N_MVR18 389
#define N_MVR19 390
#define N_MVR20 391
#define N_MVR21 392
#define N_MVR22 393
#define N_MVR23 394
#define N_MVR24 395
#define N_MVULVA 396


#endif
