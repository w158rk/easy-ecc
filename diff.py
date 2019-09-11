import re

def diff2():

    s8 = []
    s64 = []

    str8 = None
    str64 = None

    f8 = open("data-8")
    f64 = open("data-64")

    n8 = 0
    n64 = 0

    c8 = 0
    c64 = 0

    l8 = f8.readline()
    l64 = f64.readline()

    while l8 and l64:
        l8 = re.sub(r"\s+", '', l8)
        while l8[:2] != 'i=':
            l8 = f8.readline()
            c8 += 1
            l8 = re.sub(r"\s+", '', l8)
        l8 = f8.readline()
        c8 += 1

        l8 = f8.readline()
        c8 += 1
        l8 = re.sub(r"\s+", '', l8)
        s8.append(l8)

        l8 = f8.readline()
        c8 += 1
        l8 = re.sub(r"\s+", '', l8)
        s8.append(l8)

        l8 = f8.readline()
        c8 += 1
        l8 = re.sub(r"\s+", '', l8)        
        s8.append(l8)

        str8 = '\n'.join(s8)
        s8.clear()
        l8 = f8.readline() 
        c8 += 1

        l64 = re.sub(r"\s+", '', l64)
        while l64[:2] != 'i=':
            l64 = f64.readline()
            c64 += 1
            l64 = re.sub(r"\s+", '', l64)

        l64 = f64.readline()
        c64 += 1

        l64 = f64.readline()
        c64 += 1
        l64 = re.sub(r"\s+", '', l64)
        s64.append(l64)

        l64 = f64.readline()
        c64 += 1
        l64 = re.sub(r"\s+", '', l64)
        s64.append(l64)

        l64 = f64.readline()
        c64 += 1
        l64 = re.sub(r"\s+", '', l64)
        s64.append(l64)

        str64 = '\n'.join(s64)
        s64.clear()
        l64 = f64.readline() 
        c64 += 1

        if str8 != str64 :
            print("str8 [", c8, "]")
            print(str8)
            print("str64[", c64, ']')
            print(str64)
            input() 


    f8.close()
    f64.close()

diff2()

def diff1():
    while l8 and l64:
        l8 = re.sub(r"\s+", '', l8)
        while l8 != 'l_tmp':
            l8 = f8.readline()
            c8 += 1
            l8 = re.sub(r"\s+", '', l8)
        l8 = f8.readline()
        c8 += 1
        l8 = f8.readline()
        c8 += 1
        l8 = re.sub(r"\s+", '', l8)
        s8.append(l8)
        l8 = f8.readline()
        c8 += 1
        l8 = re.sub(r"\s+", '', l8)
        s8.append(l8)
        l8 = f8.readline()
        c8 += 1
        l8 = re.sub(r"\s+", '', l8)
        
        s8.append(l8)

        l8 = f8.readline()
        c8 += 1
        l8 = f8.readline()
        c8 += 1

        l8 = f8.readline()
        c8 += 1
        l8 = re.sub(r"\s+", '', l8)
        
        s8.append(l8)
        l8 = f8.readline()
        c8 += 1
        l8 = re.sub(r"\s+", '', l8)
        
        s8.append(l8)
        l8 = f8.readline()
        c8 += 1
        l8 = re.sub(r"\s+", '', l8)
        
        s8.append(l8)

        str8 = '\n'.join(s8)
        s8.clear()
        l8 = f8.readline() 
        c8 += 1

        l64 = re.sub(r"\s+", '', l64)
        while l64 != 'l_tmp':
            l64 = f64.readline()
            c64 += 1
            l64 = re.sub(r"\s+", '', l64)

        l64 = f64.readline()
        c64 += 1

        l64 = f64.readline()
        c64 += 1
        l64 = re.sub(r"\s+", '', l64)
        s64.append(l64)
        l64 = f64.readline()
        c64 += 1
        l64 = re.sub(r"\s+", '', l64)
        s64.append(l64)
        l64 = f64.readline()
        c64 += 1
        l64 = re.sub(r"\s+", '', l64)
        s64.append(l64)

        l64 = f64.readline()
        c64 += 1
        l64 = f64.readline()
        c64 += 1

        l64 = f64.readline()
        c64 += 1
        l64 = re.sub(r"\s+", '', l64)
        s64.append(l64)
        l64 = f64.readline()
        c64 += 1
        l64 = re.sub(r"\s+", '', l64)
        s64.append(l64)
        l64 = f64.readline()
        c64 += 1
        l64 = re.sub(r"\s+", '', l64)
        s64.append(l64)

        str64 = '\n'.join(s64)
        s64.clear()
        l64 = f64.readline() 
        c64 += 1

        if str8 != str64 :
            print("str8 [", c8, "]")
            print(str8)
            print("str64[", c64, ']')
            print(str64)
            input() 


    f8.close()
    f64.close()