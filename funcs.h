#ifndef FUNCS_H
#define FUNCS_H

#include <QVector>
#include <QDebug>
#include <QRandomGenerator>

#include <bits/stdc++.h>

using namespace std;

///
/// \brief The Edge class Струтура для ребра
///
struct Edge
{
    // Поток и макс.пропускная способность ребра
    int flow, capacity;

    // Ребро u--->v от ребра u
    // к ребру v.
    int u, v;

    Edge(int flow, int capacity, int u, int v);
};

///
/// \brief The Vertex class Структура для вершины
///
struct Vertex
{
    int h, e_flow;

    Vertex(int h, int e_flow);
};

///
/// \brief The Graph class Класс для графа
///
class Graph
{
    int V;    // Количество вершин
    QVector<Vertex> ver;
    QVector<Edge> edge;

    ///
    /// \brief push Функция для проталкивания потока
    /// \param u Вершина с избытком
    /// \return
    ///
    /// К потоку из u в соседнюю вершину (назовём для примера вершиной v)
    /// добавляется минимальное число среди 2-х:
    /// Избыток в вершине u (то какой поток входит в u)
    /// или то, сколько ещё может пройти из u в v (пропускная способность ребра (u, v) - текущий поток ребра (u, v),
    /// из избытка в вершине u вычтем это минимальное число,
    /// а в вершине куда поток проталкивается избыток увеличим на это число.
    ///
    bool push(int u);

    ///
    /// \brief relabel Функция для изменения высоты вершины
    /// \param u Вершина с избытком, все соседние вершины которой выше неё
    ///
    /// Изменяем высоты вершины, чтобы её избыток
    /// можно было протолкнуть
    void relabel(int u);

    ///
    /// \brief preflow Создание начального потока из истока
    /// \param s Исток
    ///
    void preflow(int s);

    ///
    /// \brief updateReverseEdgeFlow
    /// \param i Индекс ребра
    /// \param flow Добавляемый поток в результате push
    ///
    void update_reverse_edge_flow(int i, int flow);

public:
    ///
    /// \brief Graph Конструктор
    /// \param V Кол-во вершин
    ///
    Graph(int V);

    ///
    /// \brief addEdge Добавление ребра
    /// \param u Исток ребра
    /// \param v Сток ребра
    /// \param w Пропускная способность ребра
    ///
    void add_edge(int u, int v, int w);

    ///
    /// \brief getMaxFlow Нахождение максимального потока
    /// \param s Исток графа
    /// \return Число - максимальный поток
    ///
    int get_max_flow(int s);
};

///
/// \brief over_flow_vertex Поиск вершины с избытком
/// \param ver Вектор вершин
/// \return Номер вершины
///
int over_flow_vertex(QVector<Vertex>& ver);

///
/// \brief max_flow_search Создание графа и поиск максимального потока
/// \param mass Рёбра графа в виде : исток ребра, сток ребра, пропускная способность
/// \return Число - максимальный поток графа
///
int max_flow_search(QVector<QVector<int>> mass);

///
/// \brief graph_generator Создатель случайных рёбер
/// \return Вектор с рёбрами
///
/// Длина графа, наличие и пропускная способность ребра определяются случайно,
/// но всегда имеётся ребро между n-ой вершиной и (n + 1)-ой вершиной,
/// чтобы точно существовал путь от истока к стоку
QVector<QVector<int>> graph_generator();

#endif // FUNCS_H
