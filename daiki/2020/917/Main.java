import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int N = sc.nextInt();
        int S = sc.nextInt();
        int[] a = new int[N];
        //int count = 0;
        //int max = 0;
        //int min = 0;
        for (int i=0; i<N; i++) {
            a[i] = sc.nextInt();
            //count += a[i];
            //if (max < a[i]) max = a[i];
            //if (min > a[i]) min = a[i];
        }
        boolean flag = false;
        if (count >= S) flag = true;
        int j = 0;
        int ans = N;
        boolean p = flase;
        while (j < N) {
            int k = j;
            int sum = 0;
            while (ans > k - j && k < N) {
                sum += a[k++];
                if (sum >= S) {
                    ans = k - j;
                    p = true;
                    break;
                }
            }
            if (k == N) ;

            j++;
        }
        if (flag == false) ans = 0;
        System.out.println(ans);
    }
}