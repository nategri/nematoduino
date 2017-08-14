# Crunches the JSON representation of the C. Elegans
# connectome into a binary ROM to be used and read by
# an Arduino program

import json
import struct
import binascii

hexToBinaryDict = { \
"0": "0000", \
"1": "0001", \
"2": "0010", \
"3": "0011", \
"4": "0100", \
"5": "0101", \
"6": "0110", \
"7": "0111", \
"8": "1000", \
"9": "1001", \
"a": "1010", \
"b": "1011", \
"c": "1100", \
"d": "1101", \
"e": "1110", \
"f": "1111" \
}

binaryToHexDict = {}
for k, v in hexToBinaryDict.iteritems():
  binaryToHexDict[v] = k

f = open('connectome.json', 'r')

connectomeDict = json.loads(f.read())

linkedNeurons = []
unlinkedNeurons = []
for neuron, conn in connectomeDict.iteritems():
  if len(conn.keys()) == 0:
    unlinkedNeurons.append(neuron)
  else:
    linkedNeurons.append(neuron)

linkedNeurons = sorted(linkedNeurons)
unlinkedNeurons = sorted(unlinkedNeurons)

connectomeList = linkedNeurons + unlinkedNeurons

indexDict = {}
i = 0
for neuron in connectomeList:
  indexDict[neuron] = i
  i += 1

romIndexList = []
romConnList = []

rawRomIndexList = []

rawIndexList = []
rawWeightList = []
rawNameList = []
rawUberNameList = []

i = 0
for neuron in linkedNeurons:
  conn = connectomeDict[neuron]
  #romIndexList.append(binascii.hexlify(struct.pack('>H', i)))
  rawRomIndexList.append(i)

  for connNeuron, weight in conn.iteritems():
    connIndex = indexDict[connNeuron]

    rawIndexList.append(connIndex)
    rawNameList.append(connNeuron)
    rawWeightList.append(weight)
    rawUberNameList.append(neuron)

    lowConnIndex = connIndex & 0x00FF
    hiConnIndex = connIndex >> 8 # This is 1 or 0, depending on whether it's > 255
    #if connIndex > 255:
      #hiConnIndex = 1
    #else:
      #hiConnIndex = 0

    firstByte = binascii.hexlify(struct.pack('B', lowConnIndex))

    weightByte = binascii.hexlify(struct.pack('b', weight))

    secondByte = list(weightByte)
    secondByte = [hexToBinaryDict[x] for x in secondByte]
    secondByte = ''.join(secondByte)
    secondByte = list(secondByte)

    secondByte[0] = str(hiConnIndex)

    secondByte = ''.join(secondByte)

    secondByte = binascii.hexlify(struct.pack('B', int(secondByte, 2)))

    romConnList.append(firstByte+secondByte)

    if neuron == "AVL":
      print(connIndex, connNeuron, weight)

    i += 1

rawRomIndexList.append(i)
romIndexListLen = len(rawRomIndexList)

linkedNeuronListLength = binascii.hexlify(struct.pack('>H', len(linkedNeurons)))

romIndexList = [binascii.hexlify(struct.pack('>H', x+romIndexListLen+1)) for x in rawRomIndexList]

totalRomWords = [linkedNeuronListLength] + romIndexList + romConnList

print("Connectome assembled from " + str(len(totalRomWords)*2) + " bytes")

with open('c_array.out', 'w') as output:
  output.write('const uint16_t PROGMEM NeuralROM[] = {\n')
  i = 0
  for j in range(len(totalRomWords)):
    if j == (len(totalRomWords)-1):
      output.write('0x'+totalRomWords[j]+'\n};')
    else:
      output.write('0x'+totalRomWords[j]+', ')
    i += 1

    if i == 15:
      output.write('\n')
      i = 0

print("WORD OF INTEREST:")
print(totalRomWords[1530])

with open('c_defines.out', 'w') as output:
  i = 0
  for neuron in connectomeList:
    output.write('#define N_' + neuron + ' ' + str(i) + '\n')
    i += 1
