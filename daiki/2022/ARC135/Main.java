import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        StringBuilder sb = new StringBuilder();
        int N = sc.nextInt();
        int[] S = new int[N+2];
        int[] S_minus = new int[N+2];
        int[] A = new int[N+2];
        Arrays.fill(S, 0);
        Arrays.fill(S_minus, 0);
        Arrays.fill(A, 0);
        for (int i=2; i<N+2; i++) {
            S[i] = sc.nextInt();
            if (i >= 3) {
                S_minus[i] = S[i] - S[i-1];
            }
        }

        A[0] = 0;
        for (int i=3; i<N+2; i=i+3) {
            A[i] = A[i-3] + S_minus[i];
        }

        A[1] = 0;
        int min = 0;
        for (int i=4; i<N+2; i=i+3) {
            A[i] = A[i-3] + S_minus[i];
            min = A[i] < min ? A[i] : min;
        }
        for (int i=1; i<N+2; i=i+3) {
            A[i] -= min;
        }

        A[2] = 0;
        min = 0;
        for (int i=5; i<N+2; i=i+3) {
            A[i] = A[i-3] + S_minus[i];
            min = A[i] < min ? A[i] : min;
        }
        for (int i=2; i<N+2; i=i+3) {
            A[i] -= min;
        }

        int minus = S[2] - A[0] - A[1] - A[2];

        for (int i=0; i<N+2; i=i+3) {
            A[i] += minus;
        }

        sb.append("Yes\n");

        for (int i=0; i<N+2; i++) {
            if (A[i] < 0) {
                System.out.println("No");
                break;
            } else {
                sb.append(A[i]);
                if (i == N+1) {
                    System.out.println(sb);
                } else {
                    sb.append(" ");
                }
            }
        }
    }
}