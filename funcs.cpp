#include "funcs.h"

using namespace std;

Edge::Edge(int flow, int capacity, int u, int v)
{
    this->flow = flow;
    this->capacity = capacity;
    this->u = u;
    this->v = v;
}

Vertex::Vertex(int h, int e_flow)
{
    this->h = h;
    this->e_flow = e_flow;
}

Graph::Graph(int V)
{
    this->V = V;

    // У всех вершин по умолчанию 0 избытка и 0 высота
    for (int i = 0; i < V; i++)
        ver.push_back(Vertex(0, 0));
}

void Graph::add_edge(int u, int v, int capacity)
{
    // У всех рёбер изначально поток равен 0
    edge.push_back(Edge(0, capacity, u, v));
}

void Graph::preflow(int s)
{
    // Устанавливаем высоту истока равной ко-ву вершин в графе
    ver[s].h = ver.size();

    //
    for (int i = 0; i < edge.size(); i++)
    {
        // Если вершины исходит из истока
        if (edge[i].u == s)
        {
            // Поток ребра приравниваем пропускной способности
            edge[i].flow = edge[i].capacity;

            // Делаем избыток вершин равным потоку
            ver[edge[i].v].e_flow += edge[i].flow;

            // Добавляем обратные рёбра в остаточный граф с пропускной способностью 0
            edge.push_back(Edge(-edge[i].flow, 0, edge[i].v, s));
        }
    }
}

void Graph::update_reverse_edge_flow(int i, int flow)
{
    int u = edge[i].v, v = edge[i].u;

    for (int j = 0; j < edge.size(); j++)
    {
        if (edge[j].v == v && edge[j].u == u)
        {
            edge[j].flow -= flow;
            return;
        }
    }

    // Добавляем обратного рёбра в остаточный граф
    Edge e = Edge(0, flow, u, v);
    edge.push_back(e);
}

bool Graph::push(int u)
{
    // Проходим по всем рёбрам в поисках соседей, куда можно протолкну избыток
    for (int i = 0; i < edge.size(); i++)
    {
        // Проверка, что у ребра исток равен нашей вершине
        if (edge[i].u == u)
        {
            // Если поток по ребру равен пропускной способности, то протолкнуть больше не можем
            if (edge[i].flow == edge[i].capacity)
                continue;

            // Проверка, что высота стока ребра меньше истока
            if (ver[u].h > ver[edge[i].v].h)
            {
                int flow = min(edge[i].capacity - edge[i].flow,
                               ver[u].e_flow);

                ver[u].e_flow -= flow;

                ver[edge[i].v].e_flow += flow;

                edge[i].flow += flow;

                //Добавление обратного ребра в остаточный граф
                update_reverse_edge_flow(i, flow);

                return true;
            }
        }
    }
    return false;
}

void Graph::relabel(int u)
{
    // Для хранения минимальной высоты среди высот соседних вершин-стоков
    int mh = INT_MAX;

    // Поиск соседа-стока с минимальной высотой
    for (int i = 0; i < edge.size(); i++)
    {
        if (edge[i].u == u)
        {
            // Если поток равен пропускной способности, то не учитываем вершину
            if (edge[i].flow == edge[i].capacity)
                continue;

            // Изменение высоты
            if (ver[edge[i].v].h < mh)
            {
                mh = ver[edge[i].v].h;
                ver[u].h = mh + 1;
            }
        }
    }
}

int Graph::get_max_flow(int s)
{
    preflow(s);

    // Цикл до тех пор, пока не исчезнут все избытки у вершин
    while ( over_flow_vertex(ver) != -1)
    {
        int u =  over_flow_vertex(ver);
        if (!push(u))
            relabel(u);
    }

    // Возврат избытка последней вершины
    return ver.back().e_flow;
}

int  over_flow_vertex(QVector<Vertex>& ver)
{
    for (int i = 1; i < ver.size() - 1; i++)
       if (ver[i].e_flow > 0)
            return i;

    return -1;
}

int max_flow_search(QVector<QVector<int>> mass)
{
    int V = mass.back()[1] + 1;
    qDebug() << V;
    qDebug() << mass;
    Graph g(V);

    for (int i = 0; i < mass.size(); i++)
        g.add_edge(mass[i][0], mass[i][1], mass[i][2]);
    int s = 0;

    qDebug() << "Максимальный поток " << g.get_max_flow(s);
    return 0;
}

QVector<QVector<int>> graph_generator(int taskKey){
    srand(taskKey);
    QVector<QVector<int>> mass;
    QVector<int> b;
    int ranges = rand() % 3 + 4;
    for (int i = 0; i < ranges; i++){
        b.append(i);
        b.append(i + 1);
        b.append(rand() % 10 + 5);
        mass.append(b);
        b.clear();
        for (int j = i + 1; j < ranges; j++){
            if (rand() % 10 > 4)
            {
                b.append(i);
                b.append(j + 1);
                b.append(rand() % 10 + 5);
                mass.append(b);
                b.clear();
            }
        }
    }
    return mass;
}

QString create_task3(int taskKey)
{
    QVector<QVector<int>> i_edges = graph_generator(taskKey); //(вершина, вершина, пропускная способность) x 6

    QString edges; // Вершины слитно, через пробел пропускная способность x 6

    for (int i = 0; i < i_edges.size(); i++){
        edges += QString::number(i_edges[i][0]) + QString::number(i_edges[i][1]) + " " + QString::number(i_edges[i][2]);
        if (i != i_edges.size() - 1)
            edges += " ";
    }

    qDebug() << edges;

    return edges;
}
