import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int V = sc.nextInt();
        int E = sc.nextInt();
        int[][] G = new int[V][V];
        //int[][] T = new int[V][V];
        Arrays.fill(G, INF);
        for (int i=0; i<V; i++) G[i][i] = 0;

        for (int i=0; i<E; i++) {
            int s = sc.nextInt();
            int t = sc.nextInt();
            //int d = sc.nextInt();
            G[s][t] = sc.nextInt();
        }

        for (int i=0; i<V; i++) {
            while (true) {

            }
        }
    }
}