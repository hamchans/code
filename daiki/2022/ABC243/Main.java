import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        //StringBuilder sb = new StringBuilder();
        int N = sc.nextInt();
        long X = sc.nextLong();
        String S = sc.next();
        int cnt = 0;
        double max = Math.pow(10.0, 18.0);
        for (int i=0; i<N; i++) {
            if (S.substring(i, i+1).equals("U")) {
                if (cnt > 0) {
                    cnt--;
                } else {
                    X /= 2;
                }
            } else if (S.substring(i, i+1).equals("L")) {
                if (cnt > 0) {
                    cnt++;
                } else if (X * 2 > max) {
                    cnt = 1;
                } else {
                    X *= 2;
                }
            } else if (S.substring(i, i+1).equals("R")) {
                if (cnt > 0) {
                    cnt++;
                } else if (X * 2 > max) {
                    cnt = 1;
                } else {
                    X = 2 * X + 1;
                }
            }
        }
        System.out.println(X);
    }
}