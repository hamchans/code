import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int V = sc.nextInt();
        int E = sc.nextInt();
        int r = sc.nextInt();
        int[][] G = new int[V][V];
        int[] T = new int[V];
        Arrays.fill(G, INF);
        Arrays.fill(T, INF);
        T[r] = 0;


        for (int i=0; i<E; i++) {
            int s = sc.nextInt();
            int t = sc.nextInt();
            G[s][t] = sc.nextInt();

        }
        for (int i=0; i<V; i++) {
            if (G[r][i] != INF) {
                T[i] = G[r][i];
            }
        }
    }
}