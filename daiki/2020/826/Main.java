import java.util.*;

public class Main {
    static StringBuilder bd = new StringBuilder();
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        //int n = sc.nextInt();
        //int q = sc.nextInt();

        String num = sc.nextLine();
        //String nn = sc.next();
        //String qq = sc.next();
        //int n = Integer.parseInt(nn);
        //int q = Integer.parseInt(qq);

        //int n = Integer.parseInt(num.substring(0, 1));
        //int q = Integer.parseInt(num.substring(2, 3));
        String[] S = num.split(" ");
        int n = Integer.parseInt(S[0]);
        int q = Integer.parseInt(S[1]);


        double[] A = new double[n];
        Arrays.fill(A, Math.pow(2, 31) - 1);

        for (int i=0; i<q; i++) {
            String com = sc.nextLine();
            String[] L = com.split(" ");
            //System.out.println(com);
            //int com = sc.nextInt();
            if (L[0].equals("0")) {
            //if (com.substring(0, 1).equals("0")) {
            //if (com == 0) {
                //int s = sc.nextInt();
                //int t = sc.nextInt();
                //int x = sc.nextInt();

                //update(A, Integer.parseInt(L[1]), Integer.parseInt(L[2]), Long.parseLong(L[3]));
                Arrays.fill(A, Integer.parseInt(L[1]), Integer.parseInt(L[2])+1, Long.parseLong(L[3]));
                //update(A, s, t, x);
                //update(A, Integer.parseInt(com.substring(2, 3)), Integer.parseInt(com.substring(4, 5)), Integer.parseInt(com.substring(6, 7)));
            }
            else {
                //int ii = sc.nextInt();
                //find(A, Integer.parseInt(L[1]));
                bd.append((long)A[Integer.parseInt(L[1])] + "\n");
                //find(A, ii);
                //find(A, Integer.parseInt(com.substring(2, 3)));
            }
        }
        System.out.print(bd);
    }
    public static void update(double[] A, int s, int t, long x) {
        //for (int i=s; i<=t; i++) {
            //A[i] = x;
        //}
        Arrays.fill(A, s, t+1, x);
    }

    public static void find(double[] A, int i) {
        bd.append((int)A[i] + "\n");
    }
}