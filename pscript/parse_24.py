content = None

with open("point_24.data") as f:
    content = f.readlines()
    f.close() 

print(content)

Xs = []
Ys = []
for line in content:
    if line[:6] == 'X = 0x':
        Xs.append(line[6:-1])
    if line[:6] == 'Y = 0x':
        Ys.append(line[6:-1])

print("X")
print(Xs)
print("Y")
print(Ys)


resX = []
resY = []
for X in Xs:
    tmp = [None] * 6
    for i in range(6):
        tmp[i] = X[i*8 : (i+1)*8]

    for i in range(1, 6):
        tmp[i] = '0x' + tmp[i] + ', '
    tmp[0] = '0x' + tmp[0]

    tmp.reverse()
    resX.append(''.join(tmp))

print("resX")
for X in resX:
    print(X)

for Y in Ys:
    tmp = [None] * 6
    for i in range(6):
        tmp[i] = Y[i*8 : (i+1)*8]

    for i in range(1, 6):
        tmp[i] = '0x' + tmp[i] + ', '
    tmp[0] = '0x' + tmp[0]

    tmp.reverse()
    resY.append(''.join(tmp))

print("resY")
for Y in resY:
    print(Y)