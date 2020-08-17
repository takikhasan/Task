
package org.chocosolver.samples;

import org.chocosolver.solver.Model;
import org.chocosolver.solver.Solution;
import org.chocosolver.solver.Solver;
import org.chocosolver.solver.variables.IntVar;

/**
 * Playing around with Suguru puzzle
 * Small Suguru by Krazydad, Volume 7, Book 40, Suguru #7
 * <p>
 * @author 201714038, Takik Hasan, Sec B, CSE - 17, MIST
 * @since 17/08/2020.
 */
public class Suguru {

    public void modelAndSolve() {
        int n = 6;
        int[][] cage = new int[n][n];
        Model model = new Model("Small Suguru by Krazydad, Volume 7, Book 40, Suguru #7");
        IntVar[][] grid = new IntVar[n][n];
        // Cage 1:
        grid[0][0] = model.intVar("G_0_0", 1);
        grid[0][1] = model.intVar("G_0_1", 1, 5);
        grid[1][0] = model.intVar("G_1_0", 1, 5);
        grid[2][0] = model.intVar("G_2_0", 1, 5);
        grid[3][0] = model.intVar("G_3_0", 1, 5);
        cage[0][0] = cage[0][1] = cage[1][0] = cage[2][0] = cage[3][0] = 1;
        // Cage 2:
        grid[0][2] = model.intVar("G_0_2", 2);
        grid[1][1] = model.intVar("G_1_1", 1, 5);
        grid[1][2] = model.intVar("G_1_2", 1, 5);
        grid[1][3] = model.intVar("G_1_3", 1, 5);
        grid[2][2] = model.intVar("G_2_2", 1, 5);
        cage[0][2] = cage[1][1] = cage[1][2] = cage[1][3] = cage[2][2] = 2;
        // Cage 3:
        grid[0][3] = model.intVar("G_0_3", 1, 5);
        grid[0][4] = model.intVar("G_0_4", 1, 5);
        grid[0][5] = model.intVar("G_0_5", 1, 5);
        grid[1][4] = model.intVar("G_1_4", 1, 5);
        grid[2][4] = model.intVar("G_2_4", 3);
        cage[0][3] = cage[0][4] = cage[0][5] = cage[1][4] = cage[2][4] = 3;
        // Cage 4:
        grid[1][5] = model.intVar("G_1_5", 1, 5);
        grid[2][5] = model.intVar("G_2_5", 1, 5);
        grid[3][5] = model.intVar("G_3_5", 1, 5);
        grid[4][4] = model.intVar("G_4_4", 5);
        grid[4][5] = model.intVar("G_4_5", 1, 5);
        cage[1][5] = cage[2][5] = cage[3][5] = cage[4][4] = cage[4][5] = 4;
        // Cage 5:
        grid[2][1] = model.intVar("G_2_1", 1, 4);
        grid[3][1] = model.intVar("G_3_1", 1, 4);
        grid[4][0] = model.intVar("G_4_0", 1, 4);
        grid[4][1] = model.intVar("G_4_1", 1, 4);
        cage[2][1] = cage[3][1] = cage[4][0] = cage[4][1] = 5;
        // Cage 6:
        grid[2][3] = model.intVar("G_2_3", 1, 5);
        grid[3][2] = model.intVar("G_3_2", 1, 5);
        grid[3][3] = model.intVar("G_3_3", 1, 5);
        grid[3][4] = model.intVar("G_3_4", 1, 5);
        grid[4][3] = model.intVar("G_4_3", 1, 5);
        cage[2][3] = cage[3][2] = cage[3][3] = cage[3][4] = cage[4][3] = 6;
        // Cage 7:
        grid[4][2] = model.intVar("G_4_2", 1, 5);
        grid[5][2] = model.intVar("G_5_2", 1, 5);
        grid[5][3] = model.intVar("G_5_3", 1);
        grid[5][4] = model.intVar("G_5_4", 1, 5);
        grid[5][5] = model.intVar("G_5_5", 1, 5);
        cage[4][2] = cage[5][2] = cage[5][3] = cage[5][4] = cage[5][5] = 7;
        // Cage 8:
        grid[5][0] = model.intVar("G_5_0", 1);
        grid[5][1] = model.intVar("G_5_1", 1, 2);
        cage[5][0] = cage[5][1] = 8;

        int dx8[] = { 1, 1, 0, -1, -1, -1, 0, 1, 0 };
        int dy8[] = { 0, 1, 1, 1, 0, -1, -1, -1, 0 };

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < 8; k++) {
                    int nx = i + dx8[k];
                    int ny = j + dy8[k];
                    if (nx >= 0 && nx < n && ny >= 0 && ny < n) {
                        model.arithm(grid[i][j], "!=", grid[nx][ny]).post();
                    }
                }
                for (int nx = i; nx < n; nx++) {
                    for (int ny = 0; ny < n; ny++) {
                        if (nx == i && ny <= j) continue;
                        else {
                            if (cage[i][j] == cage[nx][ny]) {
                                model.arithm(grid[i][j], "!=", grid[nx][ny]).post();
                            }
                        }
                    }
                }
            }
        }

        Solver solver = model.getSolver();
        solver.showStatistics();
        solver.showSolutions();
        Solution solution = solver.findSolution();

        if(solution != null) {
            System.out.println(solution.toString());
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    int val = grid[i][j].getValue();
                    System.out.print(val + " ");
                }
                System.out.println();
            }
        }
        else {
            System.out.println("There is no solution.");
        }
    }

    public static void main(String[] args) {
        new Suguru().modelAndSolve();
    }

}
