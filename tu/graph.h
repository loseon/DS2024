#include"E:\DS2024\vector\vector.h"
#include<stdio.h>
#include "E:\DS2024\stack\stack.h"
#include "E:\DS2024\list\queue.h"
#include <iostream>
#include <climits> //INT_MAX所在库
typedef enum
{
    UNDSICOVERED,
    DISCOVERED,
    VISITER
} VStatus; // 节点状态 未发现 已发现 和 已遍历
typedef enum
{
    UNDETERMINED,
    TREE,
    CROSS,
    FORWARD,
    BACKWARD
} EType; // 边在遍历树中所属类型

template <typename Tv, typename Te> // 顶点和边类型
class Graph
{
private:
    void reset()
    { // 复位
        for (int i = 0; i < Vertex_sum; i++)
        {
            status(i) = UNDSICOVERED;
            dTime(i) = fTime(i) = -1; // 状态 时间复位
            parent(i) = -1;
            priority(i) = INT_MAX; // 父节点 优先级数
            for (int j = 0; j < Vertex_sum; j++)
            {
                if (exist(i, j))
                    type(i, j) = UNDETERMINED;
            }
        }
    }

    void BFS(int, int &);               // 广度优先
    void DFS(int, int &);               // 深度优先
    void BCC(int, int &, Stack<int> &); // 基于DFS的双连通分量分解

public:
    // 顶点
    int Vertex_sum;                     // 顶点总数
    virtual int insert(Tv const &) = 0; // 插入顶点，返回编号
    virtual Tv remove(int) = 0;         // 删除顶点及其相关边，返回顶点信息
    virtual Tv &vertex(int) = 0;        // 顶点v数据
    virtual int inDegree(int) = 0;      // 顶点入度
    virtual int outDegree(int) = 0;     // 顶点出度
    virtual int firstNbr(int) = 0;      // 顶点首个邻接顶点
    virtual int nextNbr(int, int) = 0;  // 顶点下一个临接顶点
    virtual VStatus &status(int) = 0;   // 顶点的状态
    virtual int &dTime(int) = 0;        // 顶点的d时间标签
    virtual int &fTime(int) = 0;        // 顶点的f时间标签
    virtual int &parent(int) = 0;       // 父节点
    virtual int &priority(int) = 0;     // 优先级数

    // 边
    int Edge_sum;                                       // 边总数
    virtual bool exist(int, int) = 0;                   // 边是否存在
    virtual void insert(Te const &, int, int, int) = 0; // 插入
    virtual Te remove(int, int) = 0;                    // 删除
    virtual EType &type(int, int) = 0;                  // 类型检测
    virtual Te &edge(int, int) = 0;                     // 数据
    virtual int &weight(int, int) = 0;                  // 边权重
    
    // 算法
    template <typename PU>
    void pfs(int, PU); // 优先搜素框架
    template <typename PU>
    void PFS(int, PU);                   // 优先搜素框架
    void bfs(int);                       // 广度优先
    void dfs(int);                       // 深度优先
    void bcc(int);                       // 基于dfs的双连通分量
    Stack<Tv> *tSort(int);               // 基于dfs的拓扑排序
    bool TSort(int, int &, Stack<Tv> *); // 基于DFS的拓补排序算法
    void show(int);//输出节点信息
    void output_distance(int);//输出距离信息
    void printMST();//输出最小生成树信息

    //  void prim(int);最小支撑树
    //  void dijkstra(int);最短路径
};
template <typename Tv>
struct Vertex
{
    // 顶点对象
    Tv data;                      // 数据
    int inDegree, outDegree;      // 入度和出度
    VStatus status;               // 节点状态
    int dTime;                    // d时间
    int fTime;                    // f时间
    int parent;                   // 父节点
    int priority;                 // 优先级
    Vertex(Tv const &d = (Tv)0) : // （Tv）0强制类型转换
                                  data(d), inDegree(0), outDegree(0), status(UNDSICOVERED),
                                  dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) // 暂时不考虑权重溢出
    {
    }
};
template <typename Te>
struct Edge
{
    Te data;    // 数据
    int weight; // 权重
    EType type; // 类型
    Edge(Te const &d, int w) : data(d), weight(w), type(UNDETERMINED) {}
};
template <typename Tv, typename Te>
class GraphMatrix : public Graph<Tv, Te>
{
private:
    Vector<Vertex<Tv>> V;         // 顶点集
    Vector<Vector<Edge<Te> *>> E; // 边集
public:
    GraphMatrix() { Graph<Tv, Te>::Vertex_sum = Graph<Tv, Te>::Edge_sum = 0; } // 构造
    ~GraphMatrix()
    { // 析构
        for (int j = 0; j < Graph<Tv, Te>::Vertex_sum; j++)
            for (int i = 0; i < Graph<Tv, Te>::Vertex_sum; i++)
                delete E[j][i]; // 逐条清除
    }
    // 顶点的基本操作
    virtual Tv &vertex(int i) { return V[i].data; }
    virtual int inDegree(int i) { return V[i].inDegree; }
    virtual int outDegree(int i) { return V[i].outDegree; }
    virtual int firstNbr(int i) { return nextNbr(i, Graph<Tv, Te>::Vertex_sum); }
    virtual int nextNbr(int i, int j)
    {
        while ((-1 < j) && (!exist(i, --j)))
            ;
        return j;
    }
    virtual VStatus &status(int i) { return V[i].status; }
    virtual int &dTime(int i) { return V[i].dTime; }       // 顶点的d时间标签
    virtual int &fTime(int i) { return V[i].fTime; }       // 顶点的f时间标签
    virtual int &parent(int i) { return V[i].parent; }     // 父节点
    virtual int &priority(int i) { return V[i].priority; } // 优先级数
    // 顶点动态操作
    virtual int insert(Tv const &vertex)
    {
        for (int j = 0; j < this->Vertex_sum; j++)
        {
            E[j].insert(NULL);
        }
        this->Vertex_sum++;
        //std::cout << Graph<Tv, Te>::Vertex_sum << std::endl;
        E.insert(Vector<Edge<Te> *>(this->Vertex_sum, this->Vertex_sum, (Edge<Te> *)NULL)); // 创建一个新顶点对应边向量
        return V.insert(Vertex<Tv>(vertex));
    }
    virtual Tv remove(int i)
    {
        for (int j = 0; j < Graph<Tv, Te>::Vertex_sum; j++)
            if (exist(i, j))
            {
                delete E[i][j];
                V[j].inDegree--;
            } // 逐条删除
        E.remove(i);
        Graph<Tv, Te>::Vertex_sum--;
        Tv vBak = vertex(i);
        V.remove(i);
        for (int j = 0; j < Graph<Tv, Te>::Vertex_sum; j++)
            if (Edge<Te> *e = E[j].remove(i))
            {
                delete e;
                V[j].outDegree--;
            }
        return vBak;
    }
    // 边的确认操作
    virtual bool exist(int i, int j)
    {
        return (0 <= i) && (i < Graph<Tv, Te>::Vertex_sum) && (0 <= j) && (j < Graph<Tv, Te>::Vertex_sum) && E[i][j] != NULL;
    }
    // 边的基本操作
    virtual EType &type(int i, int j) { return E[i][j]->type; }
    virtual Te &edge(int i, int j) { return E[i][j]->data; }
    virtual int &weight(int i, int j) { return E[i][j]->weight; }
    // 边的动态操作
    virtual void insert(Te const &edge, int w, int i, int j) // 插入权重为w的边（i，j）
    {
        if (exist(i, j))
            return; // 确定边是否存在
        E[i][j] = new Edge<Te>(edge, w);
        Graph<Tv, Te>::Edge_sum++;
        V[i].outDegree++;
        V[j].inDegree++;
    }
    virtual Te remove(int i, int j)
    {

        Te eBak = edge(i, j);
        delete E[i][j];
        E[i][j] = NULL;
        Graph<Tv, Te>::Edge_sum--;
        V[i].outDegree--;
        V[j].inDegree--;
        return eBak;
    }
};
template <typename Tv, typename Te>
void Graph<Tv,Te>::show(int v)//输出当前节点信息
{
    std::cout << "Visiting node " << v << " (data: " << vertex(v) << ",dtime: "<<dTime(v)<<",ftime: "<<fTime(v)<<")" << std::endl;//输出当前节点信息
}
template <typename Tv, typename Te> // 深度优先搜索算法（全图）
void Graph<Tv, Te>::DFS(int v, int &clock)
{
    dTime(v) = ++clock;
    status(v) = DISCOVERED;
    
    for (int u = firstNbr(v); u > -1; u = nextNbr(v, u))
        switch (status(u))
        {
        case UNDSICOVERED: // u未发现，意味着支撑树可在此拓展
            type(v, u) = TREE;
            parent(u) = v;
            DFS(u, clock);
            break;
        case DISCOVERED: // u已被发现但是未访问，意味着是被后代指向的祖先
            type(v, u) = BACKWARD;
            break;
        default: // u被访问完毕
            type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
            break;
        }
    status(v) = VISITER;
    fTime(v) = ++clock;
    
}
template <typename Tv, typename Te> // 深度优先搜索算法（全图）
void Graph<Tv, Te>::dfs(int s)
{ // assert:s∈(0,n)
    reset();
    int clock = 0;
    int v = s; // 初始化
    do
    {
        if (status(v) == UNDSICOVERED)
            DFS(v, clock);
            show(v);
    } while (s != (v = (++v % Vertex_sum))); // 按序号检查，不重不漏
}
template <typename Tv, typename Te> // 广度优先搜索算法（全图）
void Graph<Tv, Te>::bfs(int s)
{
    reset();
    int clock = 0;
    int v = s; // 初始化
    do
    {
        if (status(v) == UNDSICOVERED)
            BFS(v, clock);
            show(v); 
    } while (s != (v = (++v % Graph<Tv, Te>::Vertex_sum))); // 按序号检查，不重不漏
}
template <typename Tv, typename Te> // 深度优先搜索算法（全图）
void Graph<Tv, Te>::BFS(int v, int &clock)
{
    Queue<int> Q;
    status(v) = DISCOVERED;
    Q.enqueue(v);
    while (!Q.empty())
    {
        int v = Q.dequeue();
        dTime(v) = ++clock;
        for (int u = firstNbr(v); u > -1; u = nextNbr(v, u))
            if (status(u) == UNDSICOVERED)
            {
                status(u) = DISCOVERED;
                Q.enqueue(u);
                type(v, u) = TREE;
                parent(u) = v;
            }
            else
            {
                type(v, u) = CROSS;
            }
    }

    status(v) = VISITER;
}
template <typename Tv, typename Te>
Stack<Tv> *Graph<Tv, Te>::tSort(int s) // 基于DFS的拓扑排序
{
    reset();
    int clock = 0;
    int v = s;
    Stack<Tv> *S = new Stack<Tv>;
    do
    {
        if (status(v) == UNDSICOVERED)
            if (!TSort(v, clock, S))
            { // clock非必须
                while (!S->empty())
                {
                    S->pop();
                    break;
                }
            }
        show(v);    
    } while (s != (v = (++v % Graph<Tv, Te>::Vertex_sum)));
    return S;
}
template <typename Tv, typename Te>
bool Graph<Tv, Te>::TSort(int v, int &clock, Stack<Tv> *S)
{
    dTime(v) = ++clock;
    status(v) = DISCOVERED;
    for (int u = firstNbr(v); u > -1; u = nextNbr(v, u))
        switch (status(u))
        {
        case UNDSICOVERED: // u未发现，意味着支撑树可在此拓展
            type(v, u) = TREE;
            parent(u) = v;
            if (!TSort(u, clock, S))
                return false; // 若u其后代不能拓扑排序，故返回
            break;
        case DISCOVERED:           // u已被发现但是未访问，意味着是被后代指向的祖先
            type(v, u) = BACKWARD; // 一旦发现后向边 则返回
            return false;
        default: // u被访问完毕
            type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
            break;
        }
    status(v) = VISITER;
    S->push(vertex(v));
    return true;
}
template <typename Tv, typename Te>
void Graph<Tv, Te>::bcc(int s) // 基于dfs搜索框架的双连通域分解算法
{
    reset();
    int clock = 0;
    int v = s;
    Stack<int> S;
    do
    {
        if (status(v) == UNDSICOVERED)
            BCC(v, clock, S);
        S.pop();
    } while (s != (v = (++v % Graph<Tv, Te>::Vertex_sum)));
}
#define hca(x) (fTime(x)) // 利用此处闲置的fTime[]充当hca[]
template <typename Tv, typename Te>
void Graph<Tv, Te>::BCC(int v, int &clock, Stack<int> &S)
{
    hca(v) = dTime(v) = ++clock;
    status(v) = DISCOVERED;
    S.push(v);
    for (int u = firstNbr(v); u > -1; u = nextNbr(v, u))
        switch (status(u))
        {
        case UNDSICOVERED: // u未发现，意味着支撑树可在此拓展
            type(v, u) = TREE;
            parent(u) = v;
            BCC(u, clock, S);
            if (hca(u) < dTime(v))
                hca(v) = min(hca(v), hca(u));
            else
            {
                while (v != S.pop())
                    ;      // 依次弹出bcc中节点
                S.push(v); // 最后一个节点重新入栈
            }
            break;
        case DISCOVERED:           // u已被发现但是未访问，意味着是被后代指向的祖先
            type(v, u) = BACKWARD; // 一旦发现后向边 则返回
            if (u != parent(v))
                hca(v) = min(hca(v), dTime(u));
            break;
        default: // u被访问完毕
            type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
            break;
        }
    status(v) = VISITER;
}
#undef hca
template <typename Tv, typename Te>
void Graph<Tv, Te>::output_distance(int s)
{
        for (int w = s+1; w < Graph<Tv, Te>::Vertex_sum; w++)
    {
        std::cout << "From node " << vertex(s) << " to node " << vertex(w) <<" distance is "<< priority(w)<< std::endl;//输出当前节点信息
    }
}
template <typename Tv, typename Te>
void Graph<Tv, Te>::printMST()
{
    for (int v = 0; v < this->Vertex_sum; ++v) {
        int p = this->parent(v);  // 获取父节点
        if (p != -1) {
            std::cout << "Edge: " << vertex(p) << " -> " << vertex(v)
                 << " with weight " << this->weight(p, v) << std::endl;
        }
    }
}
template <typename Tv, typename Te>
template <typename PU>
void Graph<Tv, Te>::pfs(int s, PU prioUpdater)
{
    reset();
    int v = s;
    do
    {
        if (status(v) == UNDSICOVERED)
            PFS(v, prioUpdater);
            
    } while (s != (v = (++v % Graph<Tv, Te>::Vertex_sum)));
 
}
template <typename Tv, typename Te>
template <typename PU>
void Graph<Tv, Te>::PFS(int s, PU prioUpdater)
{
    priority(s) = 0;
    status(s) = VISITER;
    parent(s) = -1;
    while (1)
    {
        for (int w = firstNbr(s); w > -1; w = nextNbr(s, w))
            prioUpdater(this, s, w);
            
        for (int shortest = INT_MAX, w = 0; w < Graph<Tv, Te>::Vertex_sum; w++)
            if (status(w) == UNDSICOVERED && shortest > priority(w))
            {
                shortest = priority(w);
                s = w;
            }
        if (status(s) == VISITER)
        
            break;
        status(s) = VISITER;
        type(parent(s), s) = TREE;
    }
}
template <typename Tv, typename Te>
struct PrimPU
{
    virtual void operator()(Graph<Tv, Te> *g, int uk, int v)
    {
        if (g->status(v) == UNDSICOVERED)
            if (g->priority(v) > g->weight(uk, v))
            {
                g->priority(v) = g->weight(uk, v);
                g->parent(v) = uk;
            }
    }
};

template <typename Tv, typename Te>
struct DijkstraPU
{
    virtual void operator()(Graph<Tv, Te> *g, int uk, int v)
    {
        if (g->status(v) == UNDSICOVERED)
            if (g->priority(v) > g->priority(uk) + g->weight(uk, v))
            {
                g->priority(v) = g->priority(uk) + g->weight(uk, v);
                g->parent(v) = uk;
            }
    }
};