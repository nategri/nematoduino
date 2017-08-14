# Script to  parse the neural connections of connectome.py
# from https://github.com/Connectome/GoPiGo into a JSON representation

import json
import re

f = open('GoPiGo/connectome.py')

connectomeDict = {}

funcMatch = re.compile(r"def ([A-Z0-9]+)\(\)\:")
connMatch = re.compile(r"postSynaptic\['([A-Z0-9]+)'\]\[nextState\]\s*\+=\s*(-?\d+)")

postSynMatch =  re.compile(r"postSynaptic\['([A-Z0-9]+)'\]\s*=\s*\[0\,0\]")

for line in f:
  funcMatchRes = funcMatch.search(line)
  connMatchRes = connMatch.search(line)

  postSynMatchRes = postSynMatch.search(line)

  if funcMatchRes is not None:
    neuronKey = funcMatchRes.group(1)
    connectomeDict[neuronKey] = {}

  if connMatchRes is not None:
    connKey = connMatchRes.group(1)
    connWeight = int(connMatchRes.group(2))

    connectomeDict[neuronKey][connKey] = connWeight

  if postSynMatchRes is not None:
    postSynKey =  postSynMatchRes.group(1)

    if postSynKey not in connectomeDict.keys():
      connectomeDict[postSynKey] = {}

outputJson = json.dumps(connectomeDict, sort_keys=True, indent=4, separators=(',', ': '))

outputFile = open('connectome.json', 'w')
outputFile.write(outputJson)
outputFile.close()
