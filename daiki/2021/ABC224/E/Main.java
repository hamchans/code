import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int H = sc.nextInt();
        int W = sc.nextInt();
        int N = sc.nextInt();
        int[][] HW = new int[H][W];
        int r;
        int c;
        int a;

        for (int i=0; i<N; i++) {
            r = sc.nextInt();
            c = sc.nextInt();
            a = sc.nextInt();
            HW[r-1][c-1] = a;
        }

    }
}