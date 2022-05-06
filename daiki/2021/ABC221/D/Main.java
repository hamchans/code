import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        long[] AB = new long[2000000001];
        Arrays.fill(AB, 0);
        int N = sc.nextInt();
        for (int i=0; i<N; i++) {
            int A = sc.nextInt();
            int B = sc.nextInt() + A - 1;
            AB[A] += 1;
            AB[B] -= 1;
        }
        long sum = 0;
        for (int i=1; i<N+1; i++) {
            sum += AB[i];
            System.out.print(sum);
            if (i != N+1) {
                System.out.print(" ");
            } else {
                System.out.println();
            }
        }
    }
}