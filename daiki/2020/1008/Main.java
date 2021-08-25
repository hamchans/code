import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int V = sc.nextInt();
        int E = sc.nextInt();
        int r = sc.nextInt();

        double[] dijkstra = new double[V];
        boolean[] bool = new boolean[V];
        Arrays.fill(dijkstra, 1.0/0.0);
        Arrays.fill(bool, false);
        dijkstra[r] = 0;
        bool[r] = true;

        int[][] D = new int[V][V];

        for (int i=0; i<E; i++) {
            int s = sc.nextInt();
            int t = sc.nextInt();
            int d = sc.nextInt();
            D[s][t] = d;
        }

        


    }
}

class Node {
    int toNode;
    int cost;
}