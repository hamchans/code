import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int N = sc.nextInt();
        int W = sc.nextInt();
        long[] imos = new long[200001];
        Arrays.fill(imos, 0);

        for (int i=0; i<N; i++) {
            int s = sc.nextInt();
            int t = sc.nextInt();
            int p = sc.nextInt();

            imos[s] += p;
            imos[t] -= p;
        }

        boolean check = true;

        for (int i=0; i<200001; i++) {
            if (i != 0) imos[i] += imos[i - 1];
            if (imos[i] > W) {
                check = false;
                break;
            }
        }

        if (check == true) System.out.println("Yes");
        else System.out.println("No");

    }
}