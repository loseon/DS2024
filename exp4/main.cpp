#include"E:\DS2024\tu\graph.h"
using namespace std;
int main(){
     GraphMatrix<char, int> g;
    // 插入顶点
    g.insert('0');//0
    g.insert('1');//1
    g.insert('2');//2
    g.insert('3');//3
    g.insert('4');//4
    g.insert('5');//5
    g.insert('6');//6

    // 插入边：顶点与顶点之间的关系
    g.insert(1, 9, 0, 1); // A -> B 权重 5
    g.insert(1, 1, 1, 3); // B -> C 权重 8
    g.insert(1, 3, 3, 2); // B -> D 权重 3
    g.insert(1, 2, 2, 4); // C -> E 权重 2
    g.insert(1, 5, 4, 6); // D -> E 权重 7
    g.insert(1, 4, 4, 5); // E -> F 权重 4
    g.insert(1, 6, 3, 6); // A -> G 权重 9
    g.insert(1, 1, 5, 3); // G -> D 权重 1
//  这是图
//           5      8      3      2      7      4
//    A ------> B ------> C ------> E ------> F
//    |         |         |         |
//    |         v         v         v
//    9      G ------> D ------> E
//                | 
//                v
//                1

    // 测试深度优先搜索（DFS）
    std::cout << "DFS" << std::endl;
    g.dfs(0); // 从顶点 'A' 开始深度优先搜索（0 表示顶点 A）

    // 测试广度优先搜索（BFS）
    std::cout << "\nBFS" << std::endl;
    g.bfs(0);

    // 测试最短路径算法（Dijkstra）
    DijkstraPU<char, int> dijkstraPU;
    std::cout << "\nDijkstra" << std::endl;
    g.pfs(0, dijkstraPU);  
    g.output_distance(0);

    // 测试最小支撑树算法（Prim）
    std::cout << "\nPrim" << std::endl;
    PrimPU<char, int> PrimPU;
    g.pfs(0, PrimPU);
    g.printMST();
    return 0;
}