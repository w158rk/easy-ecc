import sys

def filter(texts):
    res = []
    for text in texts:
        if text[:5]=='Time:':
            res.append(text)
    return res

file = sys.argv[1]
print("read from file ", file)
texts = []
with open(file) as f:
    texts += f.readlines()
    f.close() 

texts = filter(texts)

times = [[],[],[],[],[]]

i=0
current = 0
for text in texts:
    old = current 
    current = int(text[6:-1])
    times[i].append(current-old)
    i = (i+1) % 5

for time in times:
    print(time)
    tmp = sum(time)/len(time)
    print(tmp/1000000)