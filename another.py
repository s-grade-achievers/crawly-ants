# f = open("depracated/new.csv", "r")
# f = f.read().split("\n")
# f = [i.split(",") for i in f if i != ""]
# # print(f)
# d = {}
# count = 0
# for i in f:
#     if i[0] not in d:
#         d[i[0]] = count
#         count += 1
# m = ""
# j = open("newer.csv", "w")
# for i in f:
#     if i[0] != m:
#         j.write("\n")
#     if d[i[0]] < 10:
#         j.write("0")
#     j.write(str(d[i[0]]))
#     j.write(i[0])
#     j.write(",")
#     if d[i[1]] < 10:
#         j.write("0")
#     j.write(str(d[i[1]]))
#     j.write(i[1])
#     j.write(",")
#     j.write(str(i[2]))
#     j.write("\n")
#     m = i[0]

# s = "0 5 2 9 8 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42"
# print(len(s.split(" ")))
