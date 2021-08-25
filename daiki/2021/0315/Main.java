import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        double xp1 = sc.nextDouble();
        double yp1 = sc.nextDouble();
        double xp2 = sc.nextDouble();
        double yp2 = sc.nextDouble();

        double a;
        double c;
        double x;
        double y;

        if (xp2 == xp1) {
            int q = sc.nextInt();

            for (int i=0; i<q; i++) {
                double xp0 = sc.nextDouble();
                double yp0 = sc.nextDouble();

                x = xp1 + (xp1 - xp0);
                y = yp0;

                System.out.print(x);
                System.out.print(" ");
                System.out.println(y);
            }
        } else if (yp2 == yp1) {
            int q = sc.nextInt();

            for (int i=0; i<q; i++) {
                double xp0 = sc.nextDouble();
                double yp0 = sc.nextDouble();

                x = xp0;
                y = yp1 + (yp1 - yp0);

                System.out.print(x);
                System.out.print(" ");
                System.out.println(y);
            }
        } else {
            a = (yp2 - yp1) / (xp2 - xp1);
            c = -a * xp1 + yp1;


            int q = sc.nextInt();

            for (int i=0; i<q; i++) {
                double xp0 = sc.nextDouble();
                double yp0 = sc.nextDouble();

                x = ((1 - a * a) * xp0 - 2 * a * (- yp0 + c)) / (a * a + 1);
                y = ((a * a - 1) * yp0 + 2 * (a * xp0 + c)) / (a * a + 1);

                System.out.print(x);
                System.out.print(" ");
                System.out.println(y);
            }
        }
    }
}