from random import randint

V = 5000
E = n = 1500
G = [list() for x in range(V)]
for i in range(V):
  u = randint(1, V)
  while u == i + 1:
    u = randint(1, V)
  G[i].append(u)

while n > 0:
  i = randint(0, V - 1)
  u = randint(1, V)
  while u == i + 1 and u in G[i]:
    u = randint(1, V + 1)
  G[i].append(u)
  n -= 1

fg = open("graph.txt", "w")
fg.write("%d %d\n" % (V, V + E))
for i in range(V):
  for u in G[i]:
    fg.write("%d %d\n" % (i + 1, u))

fg.close()