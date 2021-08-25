import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int sum = 0;

        int V = sc.nextInt();
        int E = sc.nextInt();

        ArrayList<Edge> G = new ArrayList<Edge>();

        ArrayList<ArrayList<Integer>> D = new ArrayList<ArrayList<Integer>>();
        for (int i=0; i<V; i++) {
            ArrayList<Integer> X = new ArrayList<Integer>();
            X.add(i);
            D.add(X);
        }

        int[] contain = new int[V];
        for (int i=0; i<V; i++) {
            contain[i] = i;
        }

        for (int i=0; i<E; i++) {
            int s = sc.nextInt();
            int t = sc.nextInt();
            int w = sc.nextInt();
            Edge p = new Edge(s, t, w);
            G.add(p);
        }

        Collections.sort(G);

        for (int i=0; i<E; i++) {
            //if (find(D, G.get(i).start) != find(D, G.get(i).end)) {
            if (contain[G.get(i).start] != contain[G.get(i).end]) {
                //union(D, find(D, G.get(i).start), find(D, G.get(i).end), contain);
                System.out.println(sum);
                for (int j=0; j<V; j++) {
                    System.out.print(contain[j]);
                }
                System.out.println();
                //System.out.println(contain[G.get(i).start]);
                //System.out.println(contain[G.get(i).end]);
                union(D, contain[G.get(i).start], contain[G.get(i).end], contain);
                sum += G.get(i).weight;
            }
        }
        System.out.println(sum);
    }
/*
    public static int find(ArrayList<ArrayList<Integer>> d, int n) {
        for (int i=0; i<d.size(); i++) {
            if (d.get(i).contains(n)) {
                return i;
            }
        }
        return -1;
    }

 */
    public static void union(ArrayList<ArrayList<Integer>> d, int n, int m, int[] c) {
        if (d.get(m).size() > d.get(n).size()) {
            for (int i=0; i<d.get(n).size(); i++) {
                d.get(m).add(d.get(n).get(i));
                c[d.get(n).get(i)] = m;
            }
            d.remove(n);
            for (int i=0; i<c.length; i++) {
                if (c[i] > n) c[i]--;
            }
        } else {
            for (int i = 0; i < d.get(m).size(); i++) {
                d.get(n).add(d.get(m).get(i));
                c[d.get(m).get(i)] = n;
            }
            d.remove(m);
            for (int i=0; i<c.length; i++) {
                if (c[i] > m) c[i]--;
            }
        }
        //return d;
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
        return this.weight - p.weight;
    }


}