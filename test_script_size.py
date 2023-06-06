xsize = 1024
ysize = 786
with open('data.out', 'wb') as f:
  for y in range(0,ysize):
    for x in range(0,xsize):
      r = int(min(x / (xsize/256),255))
      g = int(min(y / (ysize/256),255))
      b = 0
      f.write((b).to_bytes(1, byteorder='little'))
      f.write((g).to_bytes(1, byteorder='little'))
      f.write((r).to_bytes(1, byteorder='little'))
      f.write((0).to_bytes(1, byteorder='little'))
