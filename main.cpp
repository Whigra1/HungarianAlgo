#include <iostream>
#define N 5
#define INT_MAX 999999
class Number {
private:
    int originalVal;

public:
    bool isCrossOut;
    int val;
    int crossOutCounter = 0;


    Number(int num) {
        val = num;
        originalVal = num;
        isCrossOut = false;
    }

    void subtract(int n) {
        val -= n;
    }

    void add(int n) {
        val += n;
    }

    void reset() {
        val = originalVal;
    }

    void resetCrossOut() {
        isCrossOut = false;
    }

    void crossOut() {
        isCrossOut = true;
    }

    void print() {
        std::cout << "(" << val << "," << originalVal << "," << isCrossOut << ")";
    }

};

class HungarianAlgo {
public:
    int solve() {
        // Subtract row
        for (int row = 0; row < N; row++) {
            int max = findMaxNumberInRow(row);
            substractRow(row, max, true);
        }

        bool foundIndependenceZeros = tryToFindIndependenceZeros2();

        if (foundIndependenceZeros) {
            return result;
        }

        for (int col = 0; col < N; col++) {
            int min = findMinNumberInCol(col);
            substractCol(col, min);
        }

        foundIndependenceZeros = tryToFindIndependenceZeros2();

        if (foundIndependenceZeros) {
            return result;
        }

        crossOutMatrix();

        int notCrossOutMin = findNotCrossOutMin();

        recalculateMatrix(notCrossOutMin);

        foundIndependenceZeros = tryToFindIndependenceZeros2();

        if (foundIndependenceZeros) {
            return result;
        }

        return -1;
    }

    void printMatrix() {
        for (size_t row = 0; row < N; row++)
        {
            for (size_t col = 0; col < N; col++)
            {
                matrix[row][col].print();
            }
            std::cout << std::endl;
        }
    }

private:
    int result = 0;
    Number matrix[N][N] = {
            { Number(7), Number(3), Number(6), Number(9), Number(5) },
            { Number(7), Number(5), Number(7), Number(5), Number(6) },
            { Number(7), Number(6), Number(8), Number(8), Number(9) },
            { Number(3), Number(1), Number(6), Number(5), Number(7) },
            { Number(2), Number(4), Number(9), Number(9), Number(5) }
    };

    Number originalMatrix[N][N] = {
            { Number(7), Number(3), Number(6), Number(9), Number(5) },
            { Number(7), Number(5), Number(7), Number(5), Number(6) },
            { Number(7), Number(6), Number(8), Number(8), Number(9) },
            { Number(3), Number(1), Number(6), Number(5), Number(7) },
            { Number(2), Number(4), Number(9), Number(9), Number(5) }
    };

    void crossOutMatrix() {
        for (int row = 0; row < N; ++row) {
            for (int col = 0; col < N; ++col) {
                if (matrix[row][col].val == 0 && !matrix[row][col].isCrossOut) {
                    int zerosInRow = countZerosInRow(row);
                    int zerosInCol = countZerosInCol(col);
                    if (zerosInRow == zerosInCol) {
                        int rowSum = countRowSum(row);
                        int colSum = countColSum(col);
                        if (rowSum < colSum) {
                            crossOutRow(row, -1, true);
                        } else {
                            crossOutCol(col, -1, true);
                        }
                    }
                    else if (zerosInCol > zerosInRow) {
                        crossOutCol(col, -1, true);
                    } else {
                        crossOutRow(row, -1, true);
                    }
                }
            }
        }
    }

    int countRowSum(int row) {
        int sum = 0;
        for (int col = 0; col < N; ++col) {
            sum += matrix[row][col].val;
        }
        return sum;
    }

    int countColSum(int col) {
        int sum = 0;
        for (int row = 0; row < N; ++row) {
            sum += matrix[row][col].val;
        }
        return sum;
    }

    void recalculateMatrix(int notCrossOutMinVal){
        for (int row = 0; row < N; ++row) {
            for (int col = 0; col < N; ++col) {
                Number *num = &matrix[row][col];
                if (num->crossOutCounter == 0) {
                    num->subtract(notCrossOutMinVal);
                } else if (num->crossOutCounter == 2) {
                    num->add(notCrossOutMinVal);
                }
            }
        }
    }

    int countZerosInRow(int row) {
        int zeros = 0;
        for (int col = 0; col < N; ++col) {
            if (matrix[row][col].val == 0){
                zeros++;
            }
        }
        return zeros;
    }

    int countZerosInCol(int col) {
        int zeros = 0;
        for (int row = 0; row < N; ++row) {
            if (matrix[row][col].val == 0){
                zeros++;
            }
        }
        return zeros;
    }

    void crossOutRow(int row, int excludedCol, bool countCrossOuts = false) {
        for (int col = 0; col < N; ++col) {
            if (col == excludedCol) continue;
            Number *num = &matrix[row][col];
            num->crossOut();
            if (countCrossOuts) num->crossOutCounter++;
        }
    }

    void crossOutCol(int col, int excludedRow, bool countCrossOuts = false) {
        for (int row = 0; row < N; ++row) {
            if (row == excludedRow) continue;
            Number *num = &matrix[row][col];
            num->crossOut();
            if (countCrossOuts) num->crossOutCounter++;
        }
    }

    int findMinNumberInCol(int col) {
        int min = INT_MAX;
        for (int row = 0; row < N; ++row) {
            if (min > matrix[row][col].val) {
                min = matrix[row][col].val;
            }
        }
        return min;
    }

    int findMaxNumberInRow(int row) {
        int max = -1;
        for (int col = 0; col < N; ++col) {
            if (max < matrix[row][col].val) {
                max = matrix[row][col].val;
            }
        }
        return max;
    }

    void substractCol(int col, int val, bool withInverse = false) {
        for (int row = 0; row < N; ++row) {
            matrix[row][col].subtract(val);
            if (withInverse) {
                matrix[row][col].val *= -1;
            }
        }
    }

    void substractRow(int row, int val, bool withInverse = false) {
        for (int col = 0; col < N; ++col) {
            matrix[row][col].subtract(val);
            if (withInverse) {
                matrix[row][col].val *= -1;
            }
        }
    }

    bool tryToFindIndependenceZeros2() {
        int countRes = 0;
        for (int row = 0; row < N; ++row) {
            for (int col = 0; col < N; ++col) {
                if (matrix[row][col].val == 0) {
                    std:: cout << "(" << row << "," << col << ")\n";
                    bool checks[N] = { false };
                    bool takenRows[N] = { false };
                    for (int checkRow = 0; checkRow < N; ++checkRow) {
                        if (checkRow == row) continue;
                        for (int checkCol = 0; checkCol < N; ++checkCol) {
                            if (checkCol == col) continue;
                            checks[col] = true;
                            if (matrix[checkRow][checkCol].val == 0 && !takenRows[checkRow]) {
                                std:: cout << "\t(" << checkRow << "," << checkCol << ")" << std::endl;
                                checks[checkCol] = true;
                                takenRows[checkRow] = true;
                                countRes += originalMatrix[checkRow][checkCol].val;
                            }
                        }
                    }
                    // check independence zero
                    bool res = true;
                    countRes += originalMatrix[row][col].val;
                    for (bool check : checks) {
                        res = res && check;
                    }
                    if (res) {
                        std::cout << "Success------------" << std::endl;
                        result = countRes;
                        return true;
                    }
                }
            }
        }
        result = 0;
        return false;
    }

    int findNotCrossOutMin() {
        int min = INT32_MAX;
        for (int row = 0; row < N; ++row) {
            for (int col = 0; col < N; ++col) {
                Number *num = &matrix[row][col];
                if (!num->isCrossOut && num->val < min) {
                    min = num->val;
                }
            }
        }
        return min;
    }
};
int main() {
    std::cout << "Hello, World!" << std::endl;
    HungarianAlgo app = HungarianAlgo();

    int res = app.solve();

    std:: cout << res << std::endl;

    std:: cout << std::endl;
    app.printMatrix();
    return 0;
}
