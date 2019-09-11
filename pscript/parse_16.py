content = None

with open("point_16.data") as f:
    content = f.readlines()
    f.close() 

print(content)

Xs = []
Ys = []
for line in content:
    if line[:4] == 'X = ':
        Xs.append(line[4:-1])
    if line[:4] == 'Y = ':
        Ys.append(line[4:-1])

print("X")
print(Xs)
print("Y")
print(Ys)


resX = []
resY = []
for X in Xs:
    tmp = [None] * 4
    for i in range(4):
        tmp[i] = X[i*8 : (i+1)*8]

    for i in range(1, 4):
        tmp[i] = '0x' + tmp[i] + ', '
    tmp[0] = '0x' + tmp[0]

    tmp.reverse()
    resX.append(''.join(tmp))

print("resX")
for X in resX:
    print(X)

for Y in Ys:
    tmp = [None] * 4
    for i in range(4):
        tmp[i] = Y[i*8 : (i+1)*8]

    for i in range(1, 4):
        tmp[i] = '0x' + tmp[i] + ', '
    tmp[0] = '0x' + tmp[0]

    tmp.reverse()
    resY.append(''.join(tmp))

print("resY")
for Y in resY:
    print(Y)