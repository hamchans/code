import java.util.*;
public class Main {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int[][] goal = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
        arrayWithDepth In = new arrayWithDepth();
        arrayWithDepth a = new arrayWithDepth();
        ArrayList<arrayWithDepth> priorityQueue = new ArrayList<arrayWithDepth>();
        ArrayList<int[][]> visited = new ArrayList<int[][]>();

        int count;

        for (int i=0; i<3; i++) {
            for (int j = 0; j < 3; j++)
                In.array[i][j] = sc.nextInt();
        }
        priorityQueue.add(In);
        visited.add(In.array);

        for (;;) {
            int costMin = cost(priorityQueue.get(0).array, goal);
            Integer costMin1 = costMin;
            count = 0;

            a = priorityQueue.get(count);
            priorityQueue.remove(count);

            if (compare(a.array, goal) == 0) {
                System.out.println(a.depth);
                break;
            }

            arrayWithDepth A = new arrayWithDepth();
            arrayWithDepth B = new arrayWithDepth();
            arrayWithDepth C = new arrayWithDepth();
            arrayWithDepth D = new arrayWithDepth();

            if (!Arrays.deepEquals(a.array, swapUp(a.array))) {
                int num = 0;
                for (int i=0; i<visited.size(); i++) {
                    if (!Arrays.deepEquals(visited.get(i), swapUp(a.array)))
                        num++;
                }
                if (num == visited.size()) {
                    A.array = swapUp(a.array);
                    A.depth = a.depth + 1;
                    priorityQueue.add(A);
                    visited.add(A.array);
                }
            }

            if (!Arrays.deepEquals(a.array, swapDown(a.array))) {
                int num = 0;
                for (int i=0; i<visited.size(); i++) {
                    if (!Arrays.deepEquals(visited.get(i), swapDown(a.array)))
                        num++;
                }
                if (num == visited.size()) {
                    B.array = swapDown(a.array);
                    B.depth = a.depth + 1;
                    priorityQueue.add(B);
                    visited.add(B.array);
                }
            }

            if (!Arrays.deepEquals(a.array, swapLeft(a.array))) {
                int num = 0;
                for (int i=0; i<visited.size(); i++) {
                    if (!Arrays.deepEquals(visited.get(i), swapLeft(a.array)))
                        num++;
                }
                if (num == visited.size()) {
                    C.array = swapLeft(a.array);
                    C.depth = a.depth + 1;
                    priorityQueue.add(C);
                    visited.add(C.array);
                }
            }

            if (!Arrays.deepEquals(a.array, swapRight(a.array))) {
                int num = 0;
                for (int i=0; i<visited.size(); i++) {
                    if (!Arrays.deepEquals(visited.get(i), swapRight(a.array)))
                        num++;
                }
                if (num == visited.size()) {
                    D.array = swapRight(a.array);
                    D.depth = a.depth + 1;
                    priorityQueue.add(D);
                    visited.add(D.array);
                }
            }

            if (priorityQueue.size() == 0) {
                System.out.println("Not found!");
                break;
            }
        }
    }

    static int compare(int[][] board, int[][] target) {
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                if (board[i][j] != target[i][j])
                    return -1;
            }
        }
        return 0;
    }

    static int cost(int[][] board, int[][] target) {
        int[] A = new int[2];
        int cost_H = 0;
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                A[0] = indexOf(board[i][j], target)[0];
                A[1] = indexOf(board[i][j], target)[1];
                if (board[i][j] != 0)
                    cost_H += Math.abs(A[0] - i) + Math.abs(A[1] - j);
            }
        }
        return cost_H;
    }

    static int[] indexOf(int n, int[][] board) {
        int[] A = new int[2];
        for (int i=0; i<board.length; i++) {
            for (int j=0; j<board[0].length; j++) {
                if (n == board[i][j]) {
                    A[0] = i;
                    A[1] = j;
                    return A;
                }
            }
        }
        A[0] = -1;
        A[1] = -1;
        return A;
    }

    static int[][] swapUp(int[][] board) {
        int[][] result = new int[3][3];
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                result[i][j] = board[i][j];
            }
        }
        int x = indexOf(0, result)[0];
        if (x == 0)
            return result;
        int y = indexOf(0, result)[1];
        result[x][y] = result[x-1][y];
        result[x-1][y] = 0;
        return result;
    }

    static int[][] swapDown(int[][] board) {
        int[][] result = new int[3][3];
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                result[i][j] = board[i][j];
            }
        }
        int x = indexOf(0, result)[0];
        if (x == 2)
            return result;
        int y = indexOf(0, result)[1];
        result[x][y] = result[x+1][y];
        result[x+1][y] = 0;
        return result;
    }

    static int[][] swapLeft(int[][] board) {
        int[][] result = new int[3][3];
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                result[i][j] = board[i][j];
            }
        }
        int y = indexOf(0, result)[1];
        if (y == 0)
            return result;
        int x = indexOf(0, result)[0];
        result[x][y] = result[x][y-1];
        result[x][y-1] = 0;
        return result;
    }

    static int[][] swapRight(int[][] board) {
        int[][] result = new int[3][3];
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                result[i][j] = board[i][j];
            }
        }
        int y = indexOf(0, result)[1];
        if (y == 2)
            return result;
        int x = indexOf(0, result)[0];
        result[x][y] = result[x][y+1];
        result[x][y+1] = 0;
        return result;
    }
}

class arrayWithDepth {
    public int[][] array = new int[3][3];
    public int depth = 0;
}