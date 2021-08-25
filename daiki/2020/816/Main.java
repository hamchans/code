import java.lang.reflect.Array;
import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        StringBuilder bd = new StringBuilder();
        int n = sc.nextInt();
        ArrayList<twoD> P = new ArrayList<twoD>();
        for (int i=0; i<n; i++) {
            twoD point = new twoD();
            point.x = sc.nextInt();
            point.y = sc.nextInt();
            P.add(point);
        }
        int q = sc.nextInt();
        for (int i=0; i<q; i++) {
            int sx = sc.nextInt();
            int tx = sc.nextInt();
            int sy = sc.nextInt();
            int ty = sc.nextInt();

            int count = 0;

            for (int j=0; j<n; j++) {
                if (P.get(j).x >= sx && P.get(j).x <= tx) {
                    if (P.get(j).y >= sy && P.get(j).y <= ty) {
                        bd.append(j + "\n");
                        count++;
                    }
                }
                if (count >= 100) {
                    break;
                }
            }
            bd.append("\n");
        }
        System.out.print(bd);
    }
}

class twoD {
    int x;
    int y;
}