import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int N = sc.nextInt();
        ArrayList<Road> R = new ArrayList<Road>();
        int[] visited = new int[N];
        int n;
        Arrays.fill(visited, 0);
        for (int i=0; i<N; i++) {
            Road r = new Road();
            r.a = sc.nextInt();
            r.b = sc.nextInt();
            R.add(r);
        }
        visited[0] = 1;
        n = 0;
        for (;;) {
            int p;
            for (int i=0; i<N; i++) {
                if (R.get(i).a == n) {
                    p = i;
                    break;
                }
            }
            while (R.get(p).a == n) {

            }
        }
    }
}

class Road {
    int a;
    int b;
}