
def func1():
    s = "5421c3209c2d6c704835d82ac4c3dd90f61a8a52598b9e7ab656e9d8c8b24316"
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

s = "de2444bebc8d36e682edd27e0f271508617519b3221a8fa0b77cab3989da97c9"
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
for i in l:
    s = ''.join(res[i*8 : (i+1)*8])
    s = s[2:]
    s += ', \\'
    print(s)
