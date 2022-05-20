import java.util.*;

public class Main {
    public static void main(String[] args) {
        //int W = sc.nextInt();
        System.out.println("297");
        for (int i=1; i<100; i++) {
            System.out.print(i + " ");
        }
        for (int j=1; j<100; j++) {
            System.out.print(j*100 + " ");
        }
        for (int k=1; k<100; k++) {
            if (k == 99) {
                System.out.println(k*10000);
            } else {
                System.out.print(k*10000 + " ");
            }
        }
    }
}