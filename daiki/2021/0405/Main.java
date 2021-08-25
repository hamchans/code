import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int N = sc.nextInt();
        int T = sc.nextInt();
        int[] S = new int[T+1];
        Arrays.fill(S, 0);

        for (int i=0; i<N; i++) {
            int l = sc.nextInt();
            int r = sc.nextInt();
            S[l] += 1;
            S[r] -= 1;
        }

        for (int i=0; i<T; i++) {
            S[i+1] = S[i] + S[i+1];
        }

        int max = 0;

        for (int i=0; i<T+1; i++) {
            max = max > S[i] ? max : S[i];
        }
        System.out.println(max);
    }
}