import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        ArrraList<Edge> graph = new ArrayList<Edge>();

        int n = sc.nextInt();
        for (int i=1; i<n; i++) {
            int s = sc.nextInt();
            int t = sc.nextInt();
            int w = sc.nextInt();
            Edge e = new Edge(s, t, w);
            graph.add(e);
        }

        Collection.sort(graph);




    }

    public static void dijkstra_reverse(ArrayList<Edge> g) {
        int srcNode = g.get(0).start;
        
    }
}

class Edge {
    int start;
    int end;
    int weight;

    public Edge(int start, int end, int weight) {
        this.start = start;
        this.end = end;
        this.weight = weight;
    }

    @override
    public int compareTo(Edge e) {
        return this.start - e.start;
    }

}