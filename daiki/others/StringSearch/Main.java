import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        String T = sc.next();
        String P = sc.next();
        int i = P.length() - 1;
        int j = P.length() - 1;
        int[] skip = new int[62];
        Arrays.fill(skip, i+1);
        String[] str = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
                "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
                "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};

        for (int a=0; a<str.length; a++) {
            skip[P[a]]
        }

        while (i >= 0) {
            if (compare(T.substring(i, i+1), P.substring(j, j+1)) == 0) {
                i--;
                j--;
            }
            else {

            }

            if (j == 0) {
                System.out.println(j);
                j = P.length() - 1;
                i
                break;
            }
        }

    }

    static int compare(String t, String p) {
        if (t.equals(p)) {
            return 0;
        }
        return -1;
    }

    static int index(String s, int[] A) {
        for (int i=0; i<A.length; i++) {
            if (s.equals(A[i]))
                return i;
        }
        return -1;
    }
}