import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        StringBuilder bd = new StringBuilder();
        int n = sc.nextInt();
        int q = sc.nextInt();
        int[] a = new int[n];
        for (int i=0; i<q; i++) {
            int com = sc.nextInt();
            if (com == 0) {
                int x = sc.nextInt();
                int y = sc.nextInt();
                a[x-1] += y;
            }
            else if (com == 1) {
                int x = sc.nextInt();
                int y = sc.nextInt();
                long sum = 0;
                for (int j=x; j<=y; j++) {
                    sum += a[j-1];
                }
                bd.append(sum + "\n");
            }
        }
        System.out.print(bd);
    }
}