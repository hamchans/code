import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int M = sc.nextInt();
        int[] u = new int[M];
        int[] v = new int[M];
        int[] p = new int[8];
        for (int i=0; i<M; i++) {
            u[i] = sc.nextInt();
            v[i] = sc.nextInt();
        }
        for (int i=0; i<8; i++) {
            p[i] = sc.nextInt();
        }

        // 各コマの目標状態からのズレの合計 Cost_H を計算
        // 幅優先探索で，考えられるパターンをスタックにプッシュし，1 つずつ取り出して次の考えられるパターンをスタックにプッシュ．
        // 毎回 Cost_H を計算し，初期状態からのステップ数 Cost_G を合計した値が初期状態の Cost_H を越すかどうかを判断
        // 越さない場合，そこからさらにパターンをプッシュして繰り返し，越す場合，それ以降のその枝の探索を中止し，スタックにある別のパターンをポップして同じことを繰り返す．
    }
}