f = open("new.csv", "r")
f = f.read().split("\n")
f = [i.split(",") for i in f if i != ""]
# print(f)
d = {}
count = 0
for i in f:
    if i[0] not in d:
        d[i[0]] = count
        count += 1
m = ""
j = open("newer.csv", "w")
for i in f:
    if i[0] != m:
        j.write("\n")
    j.write(str(d[i[0]]))
    j.write(i[0])
    j.write(",")
    j.write(str(d[i[1]]))
    j.write(i[1])
    j.write(",")
    j.write(str(i[2]))
    j.write("\n")
    m = i[0]
