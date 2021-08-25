import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        StringBuilder bd = new StringBuilder();

        int n = sc.nextInt();
        Tree[] g = new Tree[n];
        Arrays.fill(g, null);
        int root = -1;
        for (int i=0; i<n; i++) {
            int dim = sc.nextInt();
            for (int j=0; j<dim; j++) {
                int c = sc.nextInt();
                Tree t = new Tree(i, 0);
                g[c] = t;
            }
        }
        for (int i=0; i<n; i++) {
            if (g[i] == null) {
                root = i;
                break;
            }
        }
        int query = sc.nextInt();
        for (int i=0; i<query; i++) {
            int q = sc.nextInt();
            if (q == 0) {
                int v = sc.nextInt();
                int w = sc.nextInt();
                g[v].cost += w;
            } else {
                int u = sc.nextInt();
                int sum = 0;
                for (;;) {
                    if (u == root) break;
                    sum += g[u].cost;
                    u = g[u].parent;
                }
                bd.append(sum + "\n");
            }
        }
        System.out.print(bd);
    }
}

class Tree {
    int parent;
    int cost;
    public Tree(int parent, int cost) {
        this.parent = parent;
        this.cost = cost;
    }
}