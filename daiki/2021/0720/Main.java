import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int H = sc.nextInt();
        int W = sc.nextInt();
        int C = sc.nextInt();
        int[][] A = new int[H][W];
        int px = 0;
        int py = 0;
        double cost = 1.0 / 0.0;
        int ex;
        int cx = 1;
        int cy = 0;

        for (int i=0; i<H; i++) {
            for (int j=0; j<W; j++) {
                A[i][j] = sc.nextInt();
            }
        }

        while (true) {
            for (int i = 0; i < H; i++) {
                for (int j = 0; j < W; j++) {
                    if (i == px && j == py) {
                        continue;
                    }
                    ex = A[px][py] + A[i][j] + C * (Math.abs(px - i) + Math.abs(py - j));
                    if (cost > ex) {
                        cost = ex;
                        cx = i;
                        cy = j;
                    }
                }
            }
            if (px == cx && py == cy) {
                break;
            }
            px = cx;
            py = cy;
        }
        System.out.println(cost);
    }
}