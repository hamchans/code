import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int N = sc.nextInt();
        int Q = sc.nextInt();
        int[] A = new int[N];
        A[0] = 0;

        for (int i=0; i<N; i++) {
            A[i] += sc.nextInt();
        }
        for (int i=0; i<Q; i++) {
            int sum = 0;
            int count = 0;
            int x = sc.nextInt();
            for (int j=0; j<N; j++) {
                if (j != 0) {
                    sum += A[j-1];
                }
                for (int k=j; k < N && A[k] - sum <= x; k++) {
                    count++;
                    System.out.print(k + " ");
                }
                System.out.println();
            }
            System.out.println(count);
        }

    }
}