import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        double[][] P = new double[4][2];
        int q = sc.nextInt();
        for (int k=0; k<q; k++) {
            for (int i = 0; i < 4; i++) {
                P[i][0] = sc.nextDouble();
                P[i][1] = sc.nextDouble();
            }

            double min;
            double ans = 1 / 0.0;
            int t;

            double f1 = (P[0][1] - P[1][1]) * (P[2][0] - P[0][0]) - (P[0][0] - P[1][0]) * (P[2][1] - P[0][1]);
            double f2 = (P[0][1] - P[1][1]) * (P[3][0] - P[0][0]) - (P[0][0] - P[1][0]) * (P[3][1] - P[0][1]);
            double f3 = (P[2][1] - P[3][1]) * (P[0][0] - P[2][0]) - (P[2][0] - P[3][0]) * (P[0][1] - P[2][1]);
            double f4 = (P[2][1] - P[3][1]) * (P[1][0] - P[2][0]) - (P[2][0] - P[3][0]) * (P[1][1] - P[2][1]);

            if (f1 * f2 < 0 && f3 * f4 < 0) {
                System.out.println((double)0);
            } else {
                for (int j = 1; j >= 0; j--) {
                    if (j == 1) t = 0;
                    else t = 2;
                    for (int i = 0; i < 2; i++) {
                        min = 1 / 0.0;
                        if (P[2 * j][0] == P[2 * j + 1][0]) {
                            if (P[2 * j][1] > P[2 * j + 1][1]) {
                                if (P[2 * j][1] >= P[i + t][1] && P[i + t][1] >= P[2 * j + 1][1])
                                    min = Math.abs(P[i + t][0] - P[2 * j][0]);
                                else
                                    min = dist(P[i + t], P[2 * j]) >= dist(P[i + t], P[2 * j + 1]) ? dist(P[i + t], P[2 * j + 1]) : dist(P[i + t], P[2 * j]);
                            } else {
                                if (P[2 * j + 1][1] >= P[i + t][1] && P[i + t][1] >= P[2 * j][1])
                                    min = Math.abs(P[i + t][0] - P[2 * j][0]);
                                else
                                    min = dist(P[i + t], P[2 * j]) >= dist(P[i + t], P[2 * j + 1]) ? dist(P[i + t], P[2 * j + 1]) : dist(P[i + t], P[2 * j]);
                            }
                        } else if (P[2 * j][1] == P[2 * j + 1][1]) {
                            if (P[2 * j][0] > P[2 * j + 1][0]) {
                                if (P[2 * j][0] >= P[i + t][0] && P[i + t][0] >= P[2 * j + 1][0])
                                    min = Math.abs(P[i + t][1] - P[2 * j][1]);
                                else
                                    min = dist(P[i + t], P[2 * j]) >= dist(P[i + t], P[2 * j + 1]) ? dist(P[i + t], P[2 * j + 1]) : dist(P[i + t], P[2 * j]);
                            } else {
                                if (P[2 * j + 1][0] >= P[i + t][0] && P[i + t][0] >= P[2 * j][0])
                                    min = Math.abs(P[i + t][1] - P[2 * j][1]);
                                else
                                    min = dist(P[i + t], P[2 * j]) >= dist(P[i + t], P[2 * j + 1]) ? dist(P[i + t], P[2 * j + 1]) : dist(P[i + t], P[2 * j]);
                            }
                        } else {
                            double a = (P[2 * j][1] - P[2 * j + 1][1]) / (P[2 * j][0] - P[2 * j + 1][0]);
                            double b = -a * P[2 * j][0] + P[2 * j][1];

                            double xp = (a / (a * a + 1)) * ((1 / a) * P[i + t][0] + P[i + t][1] - b);
                            double yp = a * xp + b;

                            double[] Q = {xp, yp};

                            if (P[2 * j][0] > P[2 * j + 1][0]) {
                                if (P[2 * j][0] >= Q[0] && Q[0] >= P[2 * j + 1][0])
                                    min = dist(P[i + t], Q);
                                else
                                    min = dist(P[i + t], P[2 * j]) >= dist(P[i + t], P[2 * j + 1]) ? dist(P[i + t], P[2 * j + 1]) : dist(P[i + t], P[2 * j]);
                            } else if (P[2 * j + 1][0] > P[2 * j][0]) {
                                if (P[2 * j + 1][0] >= Q[0] && Q[0] >= P[2 * j][0])
                                    min = dist(P[i + t], Q);
                                else
                                    min = dist(P[i + t], P[2 * j]) >= dist(P[i + t], P[2 * j + 1]) ? dist(P[i + t], P[2 * j + 1]) : dist(P[i + t], P[2 * j]);
                            }
                        }

                        ans = ans > min ? min : ans;
                        /*
                        System.out.print(i + " " + j + " ");
                        System.out.println(min);
                        
                         */
                    }
                }
                System.out.println(ans);
            }
        }
    }

    public static double dist(double[] P, double[] Q) {
        return Math.sqrt((Q[0] - P[0]) * (Q[0] - P[0]) + (Q[1] - P[1]) * (Q[1] - P[1]));
        //return math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }
}