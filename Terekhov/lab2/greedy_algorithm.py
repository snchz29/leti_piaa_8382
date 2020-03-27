from sys import stdin
from math import inf

class Edge:
    end = ''
    weight = 0
    def __init__(self, e, w):
        self.end = e
        self.weight = w
    
    def __str__(self):
        return str(self.end) + " " + str(self.weight)

    def __lt__(self, other):
        return (self.weight < other.weight)
    def __le__(self, other):
        return (self.weight <= other.weight)


if __name__ == "__main__":
    start, finish = input().split()
    edges = {}
    for line in stdin:
        u,v,c = line.replace('\n', '').split()
        if (u in edges.keys()):
            edges[u].append(Edge(v, float(c)))  
        else:
            edges[u] = [Edge(v, float(c))]
    way = start
    cur = start
    while cur != finish:
        if cur in edges.keys():
            minimum = min(edges[cur])
            if minimum.weight != inf:
                minimum.weight = inf
                way += minimum.end
                print("from",cur,"to",minimum.end)
                cur = minimum.end
            else:
                try:
                    print("from", way[-1], "to", way[-2])
                    way = way[:-1]
                    cur = way[-1]
                except IndexError:
                    print("No way.")
                    exit()
        else:
            try:
                print("from", way[-1], "to", way[-2])
                way = way[:-1]
                cur = way[-1]
            except IndexError:
                print("No way.") 
                exit()
    print(way)