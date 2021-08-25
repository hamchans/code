import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        StringBuilder bd = new StringBuilder();
        int n = Integer.parseInt(sc.next());
        int q = Integer.parseInt(sc.next());
        int[] A = new int[n];
        for (int i=0; i<q; i++) {
            int com = sc.nextInt();
            if (com == 0) {
                int s = sc.nextInt();
                int t = sc.nextInt();
                int x = sc.nextInt();
                for (int j=s-1; j<t; j++) {
                    A[j] += x;
                }
            }
            else {
                int k = sc.nextInt();
                bd.append(A[k-1] + "\n");
            }
        }
        System.out.print(bd);
    }
}