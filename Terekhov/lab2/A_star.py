#Terekhov_8382_v#1
from sys import stdin
from math import inf

def h(u, finish):
    return abs(finish - u)

def recPath(fset, start, finish):
    path = []
    curr = finish
    path.insert(0, curr)
    while curr != start:
        curr = fromset[curr]
        path.insert(0, curr)
    return path

if __name__ == "__main__":
    start, finish = map(int, input().split())
    graph = {}
#   чтение графа
    for line in stdin:
        u,v,c = line.replace('\n', '').split()
        if int(u) not in graph.keys():
            graph[int(u)] = {int(v):float(c)}
        else:
            graph[int(u)][int(v)] = float(c)
#   Алгоритм А*
    closedset = []
    openset = []
    openset.append(start)
    print(start,"append to openset")
    print("openset:", openset)
    fromset = {}
    g = {}
    g[start] = 0
    f = {}
    f[start] = g[start] + h(start, finish)
    while len(openset) > 0:
#       поиск минимального по f
        curr = openset[0]
        for u in openset:
            if f[curr] > f[u]:
                curr = u
        if curr == finish:
            break
        openset.pop(openset.index(curr))
        print(curr, "pop from openset")
        print("openset:", openset)
        closedset.append(curr)
        print(curr, "append to closedset")
        print("closedset:", closedset)
        if (curr in graph.keys()):
            for neib in graph[curr]:
                # print(graph[curr][neib])
                if neib in closedset:
                    continue
                tentScore = g[curr] + graph[curr][neib]
                if neib not in openset:
                    openset.append(neib)
                    print(neib, "append to openset")
                    print("openset:", openset)
                    tentIsBetter = True
                else:
                    tentIsBetter = True if tentScore < g[neib] else False
                if tentIsBetter:
                    fromset[neib] = curr
                    print("To", neib, "from", curr)
                    print("fromset",fromset)
                    g[neib] = tentScore
                    f[neib] = g[neib] + h(neib, finish)
    print(" ".join(map(str, recPath(fromset, start, finish))))