import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int N = sc.nextInt();
        int W = sc.nextInt();

        double[][] DP = new double[N+1][N*100+1];

        for (int i=0; i<N+1; i++) {
            Arrays.fill(DP[i], 1.0/0.0);
        }

        for (int i=1; i<N+1; i++) {
            int v = sc.nextInt();
            int w = sc.nextInt();
            for (int j=1; j<N*100+1; j++) {
                DP[i][j] = Math.min(DP[i-1][j], DP[i][j-1]);
                if (j - v >= 0) {
                    DP[i][j] = Math.min(DP[i][j], w);
                }
            }
        }
        System.out.println(DP[N][N*100]);







/*
        long[][] DP = new long[N+1][W+1];
        for (int i=0; i<N+1; i++) {
            Arrays.fill(DP[i], 0);
        }

        for (int i=1; i<N+1; i++) {
            int v = sc.nextInt();
            int w = sc.nextInt();
            for (int j=1; j<W+1; j++) {
                DP[i][j] = Math.max(DP[i-1][j], DP[i][j-1]);
                if (j - w >= 0) {
                    DP[i][j] = Math.max(DP[i][j], DP[i-1][j-w] + v);
                }
            }
        }
        System.out.println(DP[N][W]);

 */
    }
}