
def reverse():
    # reverse the whole string
    s="dd6bda0d993da0fa46b27bbc141b868f59331afa5c7e93ab"
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

    temp = ''
    for i,x in enumerate(res):
        if not i%8:
            print(temp[2:], ',\\')
            temp = ''
        temp += x

    

def func1():
    s="7CF27B188D034F7E8A52380304B51AC3C08969E277F21B35A60B48FC47669978"
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

    l = list(range(len(s)//8))
    l.reverse()
    for i in l:
        s = ''.join(res[i*4 : (i+1)*4])
        s = s[2:]
        s += ', \\'
        print(s)

def func2():

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

reverse()