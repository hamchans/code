/*
import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        int[][] tree = new int[n][n];
        for (int i=0; i<n; i++)
            Arrays.fill(tree[i], -1);
        Deque<Integer> stack = new ArrayDeque<Integer>();
        boolean[] check = new boolean[n];
        Arrays.fill(check, false);
        ArrayList<Integer> leaf = new ArrayList<Integer>();
        Deque<Integer> weight = new ArrayDeque<Integer>();
        int p = 0;
        int max = 0;

        for (int i=1; i<n; i++) {
            int s = sc.nextInt();
            int t = sc.nextInt();
            int we = sc.nextInt();
            tree[s][t] = we;
            tree[t][s] = we;
        }

        stack.push(p);
        check[p] = true;

        while (stack.size() != 0) {
            p = stack.pop();
            int size = stack.size();
            for (int i=0; i<n; i++) {
                if (tree[p][i] != -1 && check[i] == false) {
                    stack.push(i);
                    check[i] = true;
                }
            }
            if (p == 0 && stack.size() == 1)
                leaf.add(p);
            if (size == stack.size())
                leaf.add(p);
        }

        for (int i=0; i<leaf.size(); i++) {
            p = leaf.get(i);
            stack.push(p);
            weight.push(0);
            Arrays.fill(check, false);
            check[p] = true;
            while (stack.size() != 0) {
                p = stack.pop();
                int w = weight.pop();
                int size = stack.size();
                for (int j=0; j<n; j++) {
                    if (tree[p][j] != -1 && check[j] == false) {
                        stack.push(j);
                        check[j] = true;
                        weight.push(tree[p][j] + w);
                    }
                }

                if (size == stack.size())
                    max = max > w ? max : w;
            }
        }

        System.out.println(max);

    }
}
*/

import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        //int[][] tree = new int[n][n];
        ArrayList<Graph> tree = new ArrayList<Graph>();
        //for (int i=0; i<n; i++)
        //    Arrays.fill(tree[i], -1);
        Deque<Integer> stack = new ArrayDeque<Integer>();
        boolean[] check = new boolean[n];
        Arrays.fill(check, false);
        ArrayList<Integer> leaf = new ArrayList<Integer>();
        Deque<Integer> weight = new ArrayDeque<Integer>();
        int p = 0;
        int max = 0;

        for (int i=1; i<n; i++) {
            int s = sc.nextInt();
            int t = sc.nextInt();
            int we = sc.nextInt();
            //tree[s][t] = we;
            //tree[t][s] = we;
            Graph g = new Graph(s, t, we);
            tree.add(g);
        }

        Collections.sort(tree);

        stack.push(p);
        check[p] = true;
        int l = -1;

        while (stack.size() != 0) {
            p = stack.pop();
            int size = stack.size();
            /*
            for (int i=0; i<n; i++) {
                if (tree[p][i] != -1 && check[i] == false) {
                    stack.push(i);
                    check[i] = true;
                }
            }
            */
            do {
                l++;
                int i = tree.get(l).end;
                if (check[i] == false) {
                    stack.push(i);
                    check[i] = true;
                }
            } while (tree.get(l).start == tree.get(l+1).start);
            if (p == 0 && stack.size() == 1)
                leaf.add(p);
            if (size == stack.size())
                leaf.add(p);
        }

        for (int i=0; i<leaf.size(); i++) {
            p = leaf.get(i);
            stack.push(p);
            weight.push(0);
            Arrays.fill(check, false);
            check[p] = true;
            l = -1;
            while (stack.size() != 0) {
                p = stack.pop();
                int w = weight.pop();
                int size = stack.size();
                /*
                for (int j=0; j<n; j++) {
                    if (tree[p][j] != -1 && check[j] == false) {
                        stack.push(j);
                        check[j] = true;
                        weight.push(tree[p][j] + w);
                    }
                }
                */
                do {
                    l++;
                    int i = tree.get(l).end;
                    if (check[i] == false) {
                        stack.push(i);
                        check[i] = true;
                        weight.push();
                    }
                } while (tree.get(l).start == tree.get(l+1).start);

                if (size == stack.size())
                    max = max > w ? max : w;
            }
        }

        System.out.println(max);

    }
}

class Graph {
    int start;
    int end;
    int weight;
    public Graph(int start, int end, int weight) {
        this.start = start;
        this.end = end;
        this.weight = weight;
    }

    @override
    public int compareTo(Graph g) {
        return this.start - g.start;
    }
}