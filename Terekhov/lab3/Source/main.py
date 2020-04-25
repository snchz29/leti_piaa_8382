from math import inf
import copy

stepik = False


def changeable_path(graph, reversed_graph, start, finish):
    path = [start]
    bad_nodes = []
    while path[-1] != finish:
        neighbours = [e for e in graph[path[-1]] if graph[path[-1]][e] > 0 and e not in bad_nodes and e not in path]
        if not stepik:
            print("Neighbours of", path[-1], ":", neighbours)
        if path[-1] != start:
            neighbours.extend([e for e in reversed_graph[path[-1]] if reversed_graph[path[-1]][e] > 0 and
                               e not in bad_nodes and e not in path])
            if not stepik:
                print("Changeable neighbours of", path[-1], ":", neighbours)
        if not neighbours:
            if path[-1] == start:
                if not stepik:
                    print("Can\'t find changeable path")
                return []
            else:
                if not stepik:
                    print(path[-1], "is dead end")
                bad_nodes.append(path[-1])
                path.pop()
        else:
            for u in sorted(neighbours):
                if path[-1] in graph or path[-1] in reversed_graph:
                    if not stepik:
                        print("Choose node", u)
                    path.append(u)
                    if not stepik:
                        print("Path:", path)
                    break
    return path


def main():
    n = int(input())  # количество ребер
    start = input()[0]  # исток
    finish = input()[0]  # сток
    graph = dict()  # сам граф
    flows = dict()
    for _ in range(n):
        u, v, c = input().split()  # чтение триплета
        if u in graph.keys():
            graph[u[0]][v[0]] = int(c)  # если из данной вершины уже был какой-либо путь то расширяем словарь
        else:
            graph[u[0]] = {v[0]: int(c)}  # иначе создаем первый путь
        if v in flows.keys():
            flows[v[0]][u[0]] = 0  # если из данной вершины уже был какой-либо путь то расширяем словарь
        else:
            flows[v[0]] = {u[0]: 0}  # иначе создаем первый путь
    if not stepik:
        print("Graph: ", graph)
        print("Flows: ", flows)
    while True:
        # копии для того чтобы сделать вывод об увеличении максимального потока
        new_graph = copy.deepcopy(graph)
        new_flows = copy.deepcopy(flows)
        # ищем любую цепь из истока в сток не обращая внимание на направление дуг
        path = changeable_path(new_graph, new_flows, start, finish)
        if not stepik:
            print("Find path: ", path)
        if not path:
            break
        # поиск дуги с минимальной пропускной способностью или потоком
        flow = inf
        for i in range(len(path) - 1):
            # если движение в естественном направлении, то сравнение с пропускной способностью
            try:
                if new_graph[path[i]][path[i + 1]] < flow:
                    flow = new_graph[path[i]][path[i + 1]]
            # иначе с потоком
            except KeyError:
                if new_flows[path[i]][path[i + 1]] < flow:
                    flow = new_flows[path[i]][path[i + 1]]
        if not stepik:
            print("Minimal flow: ", flow)
        for i in range(len(path) - 1):
            # уменьшаем пропускные способности и увеличиваем потоки всех дуг чередующейся цепи, если по дуге
            try:
                new_graph[path[i]][path[i + 1]] -= flow
                new_flows[path[i + 1]][path[i]] += flow
                if not stepik:
                    print("Bandwidth <{} {}> increased by {}".format(path[i], path[i + 1], flow))
            # увеличиваем пропускные способности и уменьшаем потоки всех дуг чередующейся цепи, если против дуги
            except KeyError:
                new_graph[path[i + 1]][path[i]] += flow
                new_flows[path[i]][path[i + 1]] -= flow
                if not stepik:
                    print("Bandwidth <{} {}> reduced by {}".format(path[i], path[i + 1], flow))
        # проверка увеличился ли поток если нет то выходим из цикла
        if sum(flows[finish].values()) != sum(new_flows[finish].values()):
            graph = copy.deepcopy(new_graph)
            flows = copy.deepcopy(new_flows)
            if not stepik:
                print("Update Graph: ", graph)
                print("Update Flows: ", flows)
        # иначе обновляем графы
        else:
            break
    # оптимизация двунаправленных дуг
    for u in flows:
        for v in flows[u]:
            if v in flows and u in flows[v] and flows[u][v] and flows[v][u]:
                if flows[u][v] > flows[v][u]:
                    if not stepik:
                        print("Flow <{} {}> increased by {}".format(u, v, flows[v][u]))
                    flows[u][v] -= flows[v][u]
                    flows[v][u] = 0
                else:
                    if not stepik:
                        print("Flow <{} {}> increased by {}".format(v, u, flows[u][v]))
                    flows[v][u] -= flows[u][v]
                    flows[u][v] = 0
    # составление ответа
    list_answer = []
    for u in graph:
        for v in graph[u]:
            list_answer.append(str(u) + " " + str(v) + " " + str(flows[v][u]))
    list_answer = sorted(list_answer)
    print(sum(flows[finish].values()))
    [print(ans) for ans in list_answer]


if __name__ == "__main__":
    main()
