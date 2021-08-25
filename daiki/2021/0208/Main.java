import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int V = sc.nextInt();
        int E = sc.nextInt();

        //ArrayList<Edge> graph = new ArrayList<Edge>();

        int[][] graph = new int[V][V];
        for (int i=0; i<V; i++) {
            Arrays.fill(graph[i], 0);
        }

        for (int i=0; i<E; i:++) {
            int u = sc.nextInt();
            int v = sc.nextInt();
            int c = sc.nextInt();
            //Edge e = new Edge(u, v, c);
            //graph.add(e);
            graph[u][v] = c;
            //graph[v][u] = -c;
        }

        int max = maxFlow();

        System.out.println(max);

    }

    public static int maxFlow(int[][] g, int s, int t) {
        boolean[] checked = new boolean[V];
        Arrays.fill(checked, false);
        ArrayList<Integer> stack = new ArrayList<Integer>();
        int u = s;
        stack.add(u);
        while (checkedCount < V) {
            for (int i=0; i<V; i++) {
                if (checked[i] == false && g[u][i] > 0) {
                    checked[i] == true;
                    stack.add(i);

                }
            }
            checked[u] = true;
            checkedCount++;
            if (checked[u] == false && graph[][u])
            for (int i=0; i<V; i++) {
                if (checked[i] == false)
            }
        }
        for (int i=0; i<V; i++) {

        }
    }
}

class Edge {
    int start;
    int end;
    int cost;

    public Edge(int start, int end, int cost) {
        this.start = start;
        this.end = end;
        this.cost = cost;
    }

    public int compareTo(Edge e) {
        return this.start - e.start;
    }
}