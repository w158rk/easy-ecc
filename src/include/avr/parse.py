s = "ECEC196ACCC52973581A0DB248B0A77AC7634D81F4372DDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
res = []

temp = ''

for i,x in enumerate(s):
    if i%2==0:
        res.append(temp)
        temp = ''
        temp += ', 0x'
    temp += x

res.append(temp)
res.reverse()

l = list(range(len(s)//16))
l.reverse()
for i in l:
    s = ''.join(res[i*8 : (i+1)*8])
    s = s[2:]
    s += ', \\'
    print(s)