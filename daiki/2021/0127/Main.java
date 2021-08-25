import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int q = sc.nextInt();
        double[] array = new double[q * 2];
        for (int i=0; i<q; i++) {
            double x_p0 = sc.nextInt();
            double y_p0 = sc.nextInt();
            double x_p1 = sc.nextInt();
            double y_p1 = sc.nextInt();
            double x_p2 = sc.nextInt();
            double y_p2 = sc.nextInt();
            double x_p3 = sc.nextInt();
            double y_p3 = sc.nextInt();

            double a, b, c, d, x, y;

            if (x_p0 - x_p1 == 0) {
                x = x_p0;
                c = (y_p2 - y_p3) / (x_p2 - x_p3);
                y = c * (x - x_p2) + y_p2;
            } else if (x_p2 - x_p3 == 0) {
                x = x_p2;
                a = (y_p0 - y_p1) / (x_p0 - x_p1);
                y = a * (x - x_p0) + y_p0;
            } else {
                a = (y_p0 - y_p1) / (x_p0 - x_p1);
                b =  y_p0 - a * x_p0;
                c = (y_p2 - y_p3) / (x_p2 - x_p3);
                d = y_p2 - c * x_p2;
                x = (d - b) / (a - c);
                y = (a*d - b*c) / (a - c);
            }

            array[2*i] = x;
            array[2*i+1] = y;

        }

        for (int i=0; i<q; i++) {
            System.out.printf("%.9f %.9f\n", array[2*i], array[2*i+1]);
        }
    }
}