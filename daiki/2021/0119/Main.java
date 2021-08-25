import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int V = sc.nextInt();
        int E = sc.nextInt();
        int[][] graph = new int[V][V];
        boolean[] check = new boolean[V];
        Arrays.fill(graph, 0);
        Arrays.fill(check, false);
        int p;

        for (int i=0; i<E; i++) {
            int s = sc.nextInt();
            int t = sc.nextInt();
            graph[s][t] = 1;
        }

        for (int i=0; i<V; i++) {
            if (graph[0][i] == 1) {
                p = i;
                break;
            }
        }

        for (;;) {
            if (graph[])
        }


        Graph point = nodes.get(0);
        int p = point.start;
        for (;;) {
            if (check[p] == false) {
                check[p] = true;
                p = point.end;
                nodes.indexOf()
                point =
            }
        }



        int Q = sc.nextInt();
        for (int i=0; i<Q; i++) {
            int u = sc.nextInt();
            int v = sc.nextInt();
        }

    }
}