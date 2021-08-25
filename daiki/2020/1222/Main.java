import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int V = sc.nextInt();
        int E = sc.nextInt();
        int r = sc.nextInt();

        int sum = 0;

        ArrayList<Edge> G = new ArrayList<Edge>();
        double[] DP = new double[V];
        boolean[] bool = new boolean[V];
        Arrays.fill(DP, 1.0/0.0);
        Arrays.fill(bool, false);
        DP[r] = 0.0;

        for (int i=0; i<E; i++) {
            int s = sc.nextInt();
            int t = sc.nextInt();
            int w = sc.nextInt();
            Edge p = new Edge(s, t, w);
            G.add(p);
        }

        Collections.sort(G);

        for (int i=0; i<E; i++) {
            G.get(i).s;
            if (DP[G.get(i).t] != INF) {

            }
        }



    }
}

class Edge implements Comparable<Edge> {
    int start;
    int end;
    int weight;

    public Edge(int start, int end, int weight) {
        this.start = start; this.end = end; this.weight = weight;
    }

    @Override
    public int compareTo( Edge p ) {
        if ()
        return this.weight - p.weight;
    }

}