import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        //StringBuilder sb = new StringBuilder();
        long N = sc.nextLong();
        double cnt = Math.pow(N, 3.0);
        double ans = 0;
        long X = Long.MAX_VALUE;

        for (int i=0; i<=1000000; i++) {
            int j = 1000000;
            int tmp = (i + j) * (i * i + j * j);
            //System.out.println(tmp);
            while (tmp >= N & j >= 0) {
                //System.out.println(i + " " + j);
                X = Math.min(X, tmp);
                j--;
            }
        }
        System.out.println(X);

/*
        if (N != 0) {
            for (double a=0; a<cnt; a++) {
                for (double b=a; b<cnt; b++) {
                    ans = (a + b) * (Math.pow(a, 2.0) + Math.pow(b, 2.0)) / N;
                    if (ans >= 1) {
                        min = min >= ans * N ? (long)(ans * N) : min;
                    }
                }
            }
            System.out.println(min);
        } else {
            System.out.println(0);
        }

 */
    }
}