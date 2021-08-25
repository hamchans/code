import java.util.*;
public class Main {
    static StringBuilder sb = new StringBuilder();
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        int q = sc.nextInt();
        int[] A = new int[n];
        ArrayList<C> B = new ArrayList<C>();
        Arrays.fill(A, 2147483647);
        for (int i=0; i<q; i++) {
            int com = sc.nextInt();
            if (com == 0) {
                int s = sc.nextInt();
                int t = sc.nextInt();
                int x = sc.nextInt();
                C c = new C(s, t, x);
                B.append(c);
                //update(s, t, x, A);
            }
            else {
                int s = sc.nextInt();
                int t = sc.nextInt();
                //find(s, t, A);
                int j = B.size()-1;
                while (j >= 0) {
                    Arrays.fill(A, B.get(j).s, B.get(j).t+1, 1);
                    if (A)
                    j--;
                }
            }
        }
        System.out.print(sb);
    }
    public static void update(int s, int t, int x, int[] A) {
        for (int i=s; i<=t; i++) {
            A[i] = x;
        }
    }
    public static void find(int s, int t, int[] A) {
        int min = A[s];
        for (int i=s+1; i<=t; i++) {
            if (min > A[i]) min = A[i];
        }
        sb.append(min + "\n");
    }
}
class C {
    int s;
    int t;
    int x;
    public C(int s, int t, int x) {
        this.s = s;
        this.t = t;
        this.x = x;
    }
}