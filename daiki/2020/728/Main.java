import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        int q = sc.nextInt();
        long[] a = new long[n];
        long initial = 1024*1024*1024*2-1;
        StringBuilder bd = new StringBuilder();
        Arrays.fill(a, initial);
        for (int i=0; i<q; i++) {
            int com = sc.nextInt();
            if (com == 0) {
                int x = sc.nextInt();
                long y = sc.nextLong();
                a[x] = y;
            }
            else if (com == 1){
                int x = sc.nextInt();
                int y = sc.nextInt();
                long Min = initial;
                for (int j=x; j<y+1; j++) {
                    if (Min > a[j]) {
                        Min = a[j];
                    }
                }
                bd.append(Min + "\n");
            }
        }
        System.out.print(bd);
    }
}