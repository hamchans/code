import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        StringBuilder sb = new StringBuilder();
        int T = sc.nextInt();
        for (int i = 0; i < T; i++) {
            long a = sc.nextLong();
            long s = sc.nextLong();
            String str = "No\n";
            if (s >= 2 * a) {
                long tmp = (s - 2 * a) & a;
                if (tmp == 0) {
                    str = "Yes\n";
                }
            }
            sb.append(str);
        }
        System.out.print(sb);
    }
}