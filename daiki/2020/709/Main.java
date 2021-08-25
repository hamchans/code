import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        int q = sc.nextInt();
        int com;
        int x;
        int y;
        int X = 0;
        int Y = 0;
        ArrayList<ArrayList<Integer>> S = new ArrayList<ArrayList<Integer>>();
        ArrayList<Integer> print = new ArrayList<Integer>();
        for (int i=0; i<n; i++) {
            S.get(i).add(i);
        }
        for (int i=0; i<q; i++) {
            com = sc.nextInt();
            x = sc.nextInt();
            y = sc.nextInt();
            if (com == 0) {
                for (int j=0; j<S.size(); j++) {
                    if (S.get(j).indexOf(x) != -1) {
                        X = j;
                        break;
                    }
                }
                for (int j=0; j<S.size(); j++) {
                    if (S.get(j).indexOf(y) != -1) {
                        Y = j;
                        break;
                    }
                }
                S.add(S.get(X));
                S.remove(S.get(Y));
            }
            else {
                for (int j=0; j<S.size(); j++) {
                    if (S.get(j).indexOf(x) != -1) {
                        X = j;
                        break;
                    }
                }
                for (int j=0; j<S.size(); j++) {
                    if (S.get(j).indexOf(y) != -1) {
                        Y = j;
                        break;
                    }
                }
                if (X == Y) {
                    print.add(1);
                }
                else {
                    print.add(0);
                }
            }

        }
        for (int i=0; i<print.size(); i++) {
            System.out.println(print.get(i));
        }
    }
}