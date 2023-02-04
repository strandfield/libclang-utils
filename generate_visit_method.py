
import json

with open('cxcursorkind.json', 'r') as f:
  data = json.load(f)

lengths = list(map(lambda s: len(s), data)) 
l = max(lengths)

for c in data:
  pad = " " * (l - len(c))
  text = f"case {c}: "
  text += pad
  text += f"f(TCursor<{c}>(c)); "
  text += pad
  text += "break;"
  print(text)